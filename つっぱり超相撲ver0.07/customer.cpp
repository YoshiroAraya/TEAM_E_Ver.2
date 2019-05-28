//=============================================================================
//
// プレイヤー処理 [dohyo.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "customer.h"
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
LPD3DXMESH CCustomer::m_pMesh[MAX_CUSTOMERTYPE] = {};			// メッシュ情報（頂点情報）へのポインタ
LPD3DXBUFFER CCustomer::m_pBuffMat[MAX_CUSTOMERTYPE] = {};		// マテリアル情報へのポインタ
DWORD CCustomer::m_nNumMat[MAX_CUSTOMERTYPE] = {};				// マテリアル情報の数
LPDIRECT3DTEXTURE9 *CCustomer::m_pTexture = NULL;				// テクスチャ

//=============================================================================
// プレイヤークラスのコンストラクタ
//=============================================================================
CCustomer::CCustomer() : CSceneX(DOHYO_PRIORITY)
{
	// 値をクリア
	m_pVtxBuff = NULL;						// 頂点バッファへのポインタ
}

//=============================================================================
// デストラクタ
//=============================================================================
CCustomer::~CCustomer()
{

}

//=============================================================================
// オブジェクトの生成処理
//=============================================================================
CCustomer *CCustomer::Create(D3DXVECTOR3 pos,int nType)
{
	CCustomer *pCustomer = NULL;
	
	if (pCustomer == NULL)
	{
		// オブジェクトクラスの生成
		pCustomer = new CCustomer;

		if (pCustomer != NULL)
		{
			pCustomer->BindModel(m_pBuffMat[nType], m_nNumMat[nType], m_pMesh[nType]);
			pCustomer->BindMat(m_pTexture);
			pCustomer->Init(pos);
		}
	}

	return pCustomer;
}

//=============================================================================
// プレイヤー初期化処理
//=============================================================================
HRESULT CCustomer::Init(D3DXVECTOR3 pos)
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
void CCustomer::Uninit(void)
{
	// 2Dオブジェクト終了処理
	CSceneX::Uninit();
}

//=============================================================================
// プレイヤー更新処理
//=============================================================================
void CCustomer::Update(void)
{

}

//=============================================================================
// プレイヤー描画処理
//=============================================================================
void CCustomer::Draw(void)
{
	// 2Dオブジェクト描画処理
	CSceneX::Draw();
}

//=============================================================================
// プレイヤーのモデル読み込み処理
//=============================================================================
HRESULT CCustomer::LoadModel(void)
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
		&m_pBuffMat[0],
		NULL,
		&m_nNumMat[0],
		&m_pMesh[0]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[1],
		NULL,
		&m_nNumMat[1],
		&m_pMesh[1]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX(PLAYER_MODEL_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[2],
		NULL,
		&m_nNumMat[2],
		&m_pMesh[2]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX(ENEMY_MODEL_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[3],
		NULL,
		&m_nNumMat[3],
		&m_pMesh[3]);

	return S_OK;
}

//=============================================================================
// プレイヤーのテクスチャ解放処理
//=============================================================================
void CCustomer::UnloadModel(void)
{
	// メッシュの開放
	if (m_pMesh != NULL)
	{
		for (int nCntMesh = 0; nCntMesh < MAX_CUSTOMERTYPE; nCntMesh++)
		{
			m_pMesh[nCntMesh]->Release();
			m_pMesh[nCntMesh] = NULL;
		}
	}

	// マテリアルの開放
	if (m_pBuffMat != NULL)
	{
		for (int nCntMat = 0; nCntMat < MAX_CUSTOMERTYPE; nCntMat++)
		{
			m_pBuffMat[nCntMat]->Release();
			m_pBuffMat[nCntMat] = NULL;
		}
	}
}

//=============================================================================
// ブロックのモデル読み込み処理
//=============================================================================
HRESULT CCustomer::LoadMat(void)
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

	for (int nCntNum = 0; nCntNum < MAX_CUSTOMERTYPE; nCntNum++)
	{

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_pBuffMat[nCntNum]->GetBufferPointer();
	}

	return S_OK;
}

//=============================================================================
// ブロックのモデル解放処理
//=============================================================================
void CCustomer::UnloadMat(void)
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