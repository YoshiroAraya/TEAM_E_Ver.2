//=============================================================================
//
// オブジェクト処理 [Object3DModel.cpp]
// Author : 目黒 未来也
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
#include "load.h"
#include "enemy.h"
#include "ultimate.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================


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
	m_nTypeTex = 0;
	m_pTexture = NULL;
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
C3DObject *C3DObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	C3DObject *pCustomer = NULL;

	if (pCustomer == NULL)
	{
		// オブジェクトクラスの生成
		pCustomer = new C3DObject;

		if (pCustomer != NULL)
		{
			pCustomer->BindModel(CLoad::GetBuffMat(nType), CLoad::GetNumMat(nType), CLoad::GetMesh(nType));
 			pCustomer->m_nTypeTex = nType;
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
	m_rot = D3DXVECTOR3(sinf(D3DX_PI * CUSTOMER_ROT), cosf(D3DX_PI * CUSTOMER_ROT), cosf(D3DX_PI * CUSTOMER_ROT));


	//// デバイスを取得
	//LPDIRECT3DDEVICE9 pDevice = NULL;
	//D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ

	//// マテリアルデータへのポインタを取得
	//pMat = (D3DXMATERIAL*)CLoad::GetBuffMat(m_nTypeTex)->GetBufferPointer();

	//for (int nCntMat = 0; nCntMat < (int)CLoad::GetNumMat(m_nTypeTex); nCntMat++)
	//{
	//	// 入れる前に空にする
	//	//m_pTexture = NULL;

	//	if (pMat[nCntMat].pTextureFilename != NULL)
	//	{
	//		// テクスチャの生成
	//		D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, m_pTexture);
	//	}
	//}

	//	CLoad::GetBuffMat(m_nTypeTex);

	//m_pTexture = CLoad::GetTexture(m_nTypeTex);

	//CSceneX::BindMat(m_pTexture);

	return S_OK;
}

//=============================================================================
// オブジェクト終了処理
//=============================================================================
void C3DObject::Uninit(void)
{
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
		break;
	}

	// 位置の設定
	CSceneX::SetPosition(pos);
}

//=============================================================================
// オブジェクト描画処理
//=============================================================================
void C3DObject::Draw(void)
{
	// 2Dオブジェクト描画処理
	CSceneX::Draw();
}


////=============================================================================
//// ブロックのモデル読み込み処理
////=============================================================================
//HRESULT C3DObject::LoadMat(void)
//{
//	// レンダラーを取得
//	CRenderer *pRenderer;
//	pRenderer = CManager::GetRenderer();
//
//	// デバイスを取得
//	LPDIRECT3DDEVICE9 pDevice = NULL;
//
//	if (pRenderer != NULL)
//	{
//		pDevice = pRenderer->GetDevice();
//	}
//
//	D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ
//
//										// マテリアルデータへのポインタを取得
//	pMat = (D3DXMATERIAL*)CLoad::GetBuffMat(m_nTypeTex)->GetBufferPointer();
//
//	// マテリアルの数分テクスチャを入れるものを動的に確保
//	m_pTexture = new LPDIRECT3DTEXTURE9[CLoad::GetNumMat(m_nTypeTex)];
//
//	for (int nCntMat = 0; nCntMat < (int)CLoad::GetNumMat(m_nTypeTex); nCntMat++)
//	{
//		// 入れる前に空にする
//		m_pTexture[nCntMat] = NULL;
//
//		if (pMat[nCntMat].pTextureFilename != NULL)
//		{
//			// テクスチャの生成
//			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &m_pTexture[nCntMat]);
//		}
//	}
//
//	return S_OK;
//}
//
////=============================================================================
//// ブロックのモデル解放処理
////=============================================================================
//void C3DObject::UnloadMat(void)
//{
//	if (m_pTexture != NULL)
//	{// テクスチャのポインタのNULLチェック(家)
//		for (int nCntMat = 0; nCntMat < (int)CLoad::GetNumMat(m_nTypeTex); nCntMat++)
//		{
//			if (m_pTexture[nCntMat] != NULL)
//			{// ポインタの中のNULLチェック(家具)
//				m_pTexture[nCntMat]->Release();
//				m_pTexture[nCntMat] = NULL;
//			}
//		}
//
//		// メモリの開放(解体)
//		delete[] m_pTexture;
//		// NULLにする(更地)
//		m_pTexture = NULL;
//	}
//}