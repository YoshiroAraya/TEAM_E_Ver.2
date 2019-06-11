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
#include "scene3D.h"
#include "meshField.h"
#include "shadow.h"
#include "load.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 *CDohyo::m_pTexture = NULL;				// テクスチャ

//=============================================================================
// マクロ
//=============================================================================
#define DOHYOSIZE (20)

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
			pDohyo->BindModel(CLoad::GetBuffMat(CLoad::MODEL_DOHYO), CLoad::GetNumMat(CLoad::MODEL_DOHYO), CLoad::GetMesh(CLoad::MODEL_DOHYO));
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

	CSceneX::SetVtxMax(D3DXVECTOR3(CSceneX::GetVtxMax().x - DOHYOSIZE, CSceneX::GetVtxMax().y, CSceneX::GetVtxMax().z - DOHYOSIZE));
	CSceneX::SetVtxMin(D3DXVECTOR3(CSceneX::GetVtxMin().x + DOHYOSIZE, CSceneX::GetVtxMin().y, CSceneX::GetVtxMin().z + DOHYOSIZE));

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
	pMat = (D3DXMATERIAL*)CLoad::GetBuffMat(CLoad::MODEL_DOHYO)->GetBufferPointer();

	// マテリアルの数分テクスチャを入れるものを動的に確保
	m_pTexture = new LPDIRECT3DTEXTURE9[CLoad::GetNumMat(CLoad::MODEL_DOHYO)];

	for (int nCntMat = 0; nCntMat < (int)CLoad::GetNumMat(CLoad::MODEL_DOHYO); nCntMat++)
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
		for (int nCntMat = 0; nCntMat < (int)CLoad::GetNumMat(CLoad::MODEL_DOHYO); nCntMat++)
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