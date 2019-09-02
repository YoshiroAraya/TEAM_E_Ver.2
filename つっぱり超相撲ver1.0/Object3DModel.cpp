//=============================================================================
//
// オブジェクト処理 [Object3DModel.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "Object3DModel.h"
#include "input.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "camera.h"
#include "scene3D.h"
#include "meshField.h"
#include "shadow.h"
#include "enemy.h"
#include "ultimate.h"

#include "player.h"
#include "enemy.h"
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 *C3DObject::m_pTexture = NULL;				// テクスチャ

#define CUSTOMER_ROT			((rand() % 628) / 100.0f)		//全方向

//=============================================================================
// オブジェクトクラスのコンストラクタ
//=============================================================================
C3DObject::C3DObject() : CSceneX(DOHYO_PRIORITY)
{
	// 値をクリア
	m_pVtxBuff = NULL;						// 頂点バッファへのポインタ
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pBuffMat = NULL;		//マテリアル情報へのポインタ
	m_pTexture = NULL;		//テクスチャへのポインタ
	m_nNumMat = 0;			//マテリアル情報の数
}

//=============================================================================
// デストラクタ
//=============================================================================
C3DObject::~C3DObject()
{

}

//=============================================================================
// オブジェクトの生成処理
//=============================================================================
C3DObject *C3DObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE nObjType, CLoad::MODEL nModelType)
{
	C3DObject *pCustomer = NULL;

	if (pCustomer == NULL)
	{
		// オブジェクトクラスの生成
		pCustomer = new C3DObject;

		if (pCustomer != NULL)
		{
			pCustomer->BindModel(CLoad::GetBuffMat(nModelType), CLoad::GetNumMat(nModelType), CLoad::GetMesh(nModelType));
			pCustomer->m_Type = nObjType;
			pCustomer->Init(pos, rot);
		}
	}

	return pCustomer;
}

//=============================================================================
// オブジェクト初期化処理
//=============================================================================
HRESULT C3DObject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// オブジェクト初期化処理
	CSceneX::Init(pos);

	//オブジェクトの向き
	CSceneX::SetRot(rot);

	// オブジェクトの種類の設定
	SetObjType(CScene::OBJTYPE_DOHYO);
	m_move = D3DXVECTOR3(sinf((rand() % 628) / 100.0f) * ((rand() % 7 + 1)), -1.0f, cosf((rand() % 628) / 100.0f) * ((rand() % 7 + 1)));
	//m_rot = D3DXVECTOR3(sinf(D3DX_PI * CUSTOMER_ROT), cosf(D3DX_PI * CUSTOMER_ROT), cosf(D3DX_PI * CUSTOMER_ROT));

	if (m_Type == OBJECTTYPE_GYOUZI)
	{
		//デバイスを取得
		CRenderer *pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

		m_pBuffMat = CLoad::GetBuffMat(CLoad::MODEL_GYOUZI);
		m_nNumMat = CLoad::GetNumMat(CLoad::MODEL_GYOUZI);

		//マテリアル情報からテクスチャの取得
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
		//テクスチャをマテリアルの数分動的確保
		m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];
		//マテリアルの数回す
		for (int nCntMatTex = 0; nCntMatTex < (int)m_nNumMat; nCntMatTex++)
		{
			//NULLを入れる 中身を空に
			m_pTexture[nCntMatTex] = NULL;

			if (pMat[nCntMatTex].pTextureFilename != NULL)
			{
				// テクスチャの設定
				D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
					pMat[nCntMatTex].pTextureFilename,	// ファイルの名前
					&m_pTexture[nCntMatTex]);		// テクスチャへのポインタ
			}
		}

		CSceneX::BindMat(m_pTexture);
	}


	return S_OK;
}

//=============================================================================
// オブジェクト終了処理
//=============================================================================
void C3DObject::Uninit(void)
{
	if (m_pTexture != NULL)
	{
		for (int nCnt = 0; nCnt < (int)m_nNumMat; nCnt++)
		{
			//ポインタ内のポインタのNULLチェック (家具)
			if (m_pTexture[nCnt] != NULL)
			{
				//テクスチャ破棄
				m_pTexture[nCnt]->Release();
				m_pTexture[nCnt] = NULL;
			}
		}
		//メモリを開放 (解体)
		delete[] m_pTexture;
		//NULLを入れる (更地)
		m_pTexture = NULL;
	}

	// 2Dオブジェクト終了処理
	CSceneX::Uninit();
}

//=============================================================================
// オブジェクト更新処理
//=============================================================================
void C3DObject::Update(void)
{
	CManager::MODE mode;
	mode = CManager::GetMode();

	// 敵取得
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

	// 位置を取得
	D3DXVECTOR3 pos;
	pos = CSceneX::GetPosition();

	switch (mode)
	{
	case CManager::MODE_GAME:

		D3DXVECTOR3 p1pos, p2pos;
		// プレイヤーの取得
		CPlayer *pPlayer = NULL;
		pPlayer = CGame::GetPlayer();
		// エネミーの取得
		CEnemy *pEnemy = NULL;
		pEnemy = CGame::GetEnemy();

		if (pPlayer != NULL)
		{	//プレイヤー1の位置を取得
			p1pos = pPlayer->GetPosition();
		}
		if (pPlayer != NULL)
		{	//プレイヤー2の位置を取得
			p2pos = pEnemy->GetPosition();
		}

		float DiffDis = (p1pos.x + p2pos.x) / 2;
		D3DXVECTOR3 testpos((p1pos.x + p2pos.x) / 2, (p1pos.y + p2pos.y) / 2, (p1pos.z + p2pos.z) / 2);
		// 目的の角度
		if (pEnemy != NULL)
		{
			m_fDestAngle = atan2f((testpos.x - sinf(m_rot.y)) - pos.x, (testpos.z - cosf(m_rot.y)) - pos.z);
			m_fDestAngle += D3DX_PI;
		}
		// 差分
		m_fDiffAngle = m_fDestAngle - m_rot.y;

		if (m_fDiffAngle > D3DX_PI)
		{
			m_fDiffAngle -= D3DX_PI * 2.0f;
		}
		if (m_fDiffAngle < -D3DX_PI)
		{
			m_fDiffAngle += D3DX_PI * 2.0f;
		}

		m_rot.y += m_fDiffAngle * 0.1f;

		if (m_rot.y > D3DX_PI)
		{
			m_rot.y -= D3DX_PI * 2.0f;
		}
		if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI * 2.0f;
		}

		//向きの慣性
		m_fDiffAngle = m_fDestAngle - m_rot.y;

		//角度の設定
		if (m_fDiffAngle > D3DX_PI)
		{
			m_fDiffAngle -= D3DX_PI* 2.0f;
		}
		if (m_fDiffAngle < -D3DX_PI)
		{
			m_fDiffAngle += D3DX_PI* 2.0f;
		}

		m_rot.y += m_fDiffAngle * 0.1f;

		if (m_rot.y > D3DX_PI)
		{
			m_rot.y -= D3DX_PI* 2.0f;
		}
		if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI* 2.0f;
		}
		break;
	}

	// 位置の設定
	CSceneX::SetPosition(pos);
	CSceneX::SetRot(m_rot);

}

//=============================================================================
// オブジェクト描画処理
//=============================================================================
void C3DObject::Draw(void)
{
	// 2Dオブジェクト描画処理
	CSceneX::Draw();
}