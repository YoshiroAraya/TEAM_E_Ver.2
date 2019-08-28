//=============================================================================
//
// パーティクルの処理 [particleX.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "particleX.h"
#include "manager.h"
#include "renderer.h"
#include "load.h"
//#include "explosion.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BLOCKDEBRIS_NAME	"data\\MODEL\\woodDebris.x"	// ブロックの破片のモデル名

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 *CParticleX::m_pTexture = 0;			// テクスチャ情報

//=============================================================================
// パーティクルの生成処理
//=============================================================================
CParticleX *CParticleX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, int nLife, PARTICLE_TYPE type)
{
	CParticleX *pParticle = NULL;

	if (pParticle == NULL)
	{
		// パーティクルクラスの生成
		pParticle = new CParticleX;

		if (pParticle != NULL)
		{
			pParticle->m_move = move;						// 移動量の設定
			pParticle->m_nLife = nLife;						// ライフの設定
			pParticle->BindModel(CLoad::GetBuffMat(CLoad::MODEL_CUSTMOR0), CLoad::GetNumMat(CLoad::MODEL_CUSTMOR0), CLoad::GetMesh(CLoad::MODEL_CUSTMOR0));
			pParticle->BindMat(m_pTexture);
			pParticle->Init(pos);							// 初期化処理
			pParticle->SetRot(rot);							// 回転の設定
			pParticle->m_ParticleType = type;				// 種類の設定
		}
	}
	return pParticle;
}

//=============================================================================
// パーティクルクラスのコンストラクタ
//=============================================================================
CParticleX::CParticleX() : CSceneX(5)
{
	// 値をクリア
	m_nLife = 0;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pVtxBuff = NULL;						// 頂点バッファへのポインタ
	D3DXMatrixIdentity(&m_mtxWorld);		// ワールドマトリックス
	m_ParticleType = TYPE_NORMAL;
	m_pMesh = NULL;		// メッシュ情報（頂点情報）へのポインタ
	m_pBuffMat = NULL;	// マテリアル情報へのポインタ
	m_nNumMat = 0;			// マテリアル情報の数
}

//=============================================================================
// パーティクルクラスのデストラクタ
//=============================================================================
CParticleX::~CParticleX()
{
}

//=============================================================================
// パーティクルの初期化処理
//=============================================================================
HRESULT CParticleX::Init(D3DXVECTOR3 pos)
{
	// オブジェクトの種類の設定
	SetObjType(CScene::OBJTYPE_PARTICLE);

	// 初期化処理
	CSceneX::Init(pos);

	// タイプの初期設定
	m_ParticleType = TYPE_NORMAL;

	return S_OK;
}

//=============================================================================
// パーティクルの終了処理
//=============================================================================
void CParticleX::Uninit(void)
{
	// 2Dオブジェクト終了処理
	CSceneX::Uninit();
}

//=============================================================================
// パーティクルの更新処理
//=============================================================================
void CParticleX::Update(void)
{
	// 位置を取得
	D3DXVECTOR3 pos;
	pos = CSceneX::GetPosition();

	pos -= m_move;
	m_nLife--;

	switch (m_ParticleType)
	{
		// 通常状態
	case TYPE_NORMAL:
		break;
		// 上昇状態
	case TYPE_UP:
		// 重力減算
		m_move.y -= cosf(D3DX_PI * 0.0f) * 0.5f;
		break;
		// 下降状態
	case TYPE_DOWN:
		// 重力加算
		m_move.y += cosf(D3DX_PI * 0.0f) * 0.5f;
		break;
	}

	// 位置の設定
	CSceneX::SetPosition(pos);

	if (m_nLife <= 0)
	{
		// 消す
		Uninit();
	}
}

//=============================================================================
// パーティクルの描画処理
//=============================================================================
void CParticleX::Draw(void)
{
	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// 頂点法線の自動正規化	開始
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	CSceneX::Draw();

	// 頂点法線の自動正規化	終了
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//=============================================================================
// ブロックのモデル読み込み処理
//=============================================================================
HRESULT CParticleX::LoadMat(void)
{
	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}


	D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ

										// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)CLoad::GetBuffMat(CLoad::MODEL_CUSTMOR0)->GetBufferPointer();

	// マテリアルの数分テクスチャを入れるものを動的に確保
	m_pTexture = new LPDIRECT3DTEXTURE9[CLoad::GetNumMat(CLoad::MODEL_CUSTMOR0)];

	for (int nCntMat = 0; nCntMat < (int)CLoad::GetNumMat(CLoad::MODEL_CUSTMOR0); nCntMat++)
	{
		// 入れる前に空にする
		m_pTexture[nCntMat] = NULL;

		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// テクスチャの生成
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &m_pTexture[nCntMat]);
		}
	}

	return S_OK;
}

//=============================================================================
// ブロックのモデル解放処理
//=============================================================================
void CParticleX::UnloadMat(void)
{
	if (m_pTexture != NULL)
	{// テクスチャのポインタのNULLチェック(家)
		for (int nCntMat = 0; nCntMat < (int)CLoad::GetNumMat(0); nCntMat++)
		{
			if (m_pTexture[nCntMat] != NULL)
			{// ポインタの中のNULLチェック(家具)
				m_pTexture[nCntMat]->Release();
				m_pTexture[nCntMat] = NULL;
			}
		}

		// メモリの開放(解体)
		delete[] m_pTexture;
		// NULLにする(更地)
		m_pTexture = NULL;
	}
}