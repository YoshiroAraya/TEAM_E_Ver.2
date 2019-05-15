//=============================================================================
//
// プレイヤー処理 [dohyo.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "dohyo.h"
#include "input.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "camera.h"
#include "bullet.h"
#include "scene3D.h"
#include "meshField.h"
#include "shadow.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPD3DXMESH CDohyo::m_pMesh = NULL;			// メッシュ情報（頂点情報）へのポインタ
LPD3DXBUFFER CDohyo::m_pBuffMat = NULL;	// マテリアル情報へのポインタ
DWORD CDohyo::m_nNumMat = 0;				// マテリアル情報の数
LPDIRECT3DTEXTURE9 *CDohyo::m_pTexture = NULL;				// テクスチャ

//=============================================================================
// プレイヤークラスのコンストラクタ
//=============================================================================
CDohyo::CDohyo() : CSceneX(DOHYO_PRIORITY)
{
	// 値をクリア
	m_pVtxBuff = NULL;						// 頂点バッファへのポインタ
}

//=============================================================================
// デストラクタ
//=============================================================================
CDohyo::~CDohyo()
{
}

//=============================================================================
// オブジェクトの生成処理
//=============================================================================
CDohyo *CDohyo::Create(D3DXVECTOR3 pos)
{
	CDohyo *pDohyo = NULL;

	if (pDohyo == NULL)
	{
		// オブジェクトクラスの生成
		pDohyo = new CDohyo;

		if (pDohyo != NULL)
		{
			pDohyo->BindModel(m_pBuffMat, m_nNumMat, m_pMesh);
			pDohyo->BindMat(m_pTexture);
			pDohyo->Init(pos);
		}
	}

	return pDohyo;
}

//=============================================================================
// プレイヤー初期化処理
//=============================================================================
HRESULT CDohyo::Init(D3DXVECTOR3 pos)
{
	// 2Dオブジェクト初期化処理
	CSceneX::Init(pos);

	// オブジェクトの種類の設定
	SetObjType(CScene::OBJTYPE_DOHYO);

	return S_OK;
}

//=============================================================================
// プレイヤー終了処理
//=============================================================================
void CDohyo::Uninit(void)
{
	// 2Dオブジェクト終了処理
	CSceneX::Uninit();
}

//=============================================================================
// プレイヤー更新処理
//=============================================================================
void CDohyo::Update(void)
{

}

//=============================================================================
// プレイヤー描画処理
//=============================================================================
void CDohyo::Draw(void)
{
	// 2Dオブジェクト描画処理
	CSceneX::Draw();
}

//=============================================================================
// プレイヤーのモデル読み込み処理
//=============================================================================
HRESULT CDohyo::LoadModel(void)
{
	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// Xファイルの読み込み
	D3DXLoadMeshFromX(DOHYO_MODEL_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	return S_OK;
}

//=============================================================================
// プレイヤーのテクスチャ解放処理
//=============================================================================
void CDohyo::UnloadModel(void)
{
	// メッシュの開放
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// マテリアルの開放
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
}

//=============================================================================
// ブロックのモデル読み込み処理
//=============================================================================
HRESULT CDohyo::LoadMat(void)
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
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// マテリアルの数分テクスチャを入れるものを動的に確保
	m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
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
void CDohyo::UnloadMat(void)
{
	if (m_pTexture != NULL)
	{// テクスチャのポインタのNULLチェック(家)
		for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
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