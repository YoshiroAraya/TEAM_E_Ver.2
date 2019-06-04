//=============================================================================
//
// ニュースキャスター処理 [newsCaster.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "newsCaster.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "camera.h"
#include "bullet.h"
#include "scene3D.h"
#include "meshField.h"
#include "shadow.h"
#include "title.h"
#include "loadModel.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 *CNewsCaster::m_pTexture = NULL;				// テクスチャ

//=============================================================================
// マクロ
//=============================================================================
#define DOHYOSIZE (20)

//=============================================================================
// ニュースキャスタークラスのコンストラクタ
//=============================================================================
CNewsCaster::CNewsCaster() : CSceneX(DOHYO_PRIORITY)
{
	// 値をクリア
	m_pVtxBuff = NULL;						// 頂点バッファへのポインタ
	m_nCntTimer = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CNewsCaster::~CNewsCaster()
{
}

//=============================================================================
// オブジェクトの生成処理
//=============================================================================
CNewsCaster *CNewsCaster::Create(D3DXVECTOR3 pos)
{
	CNewsCaster *pDohyo = NULL;

	if (pDohyo == NULL)
	{
		// オブジェクトクラスの生成
		pDohyo = new CNewsCaster;

		if (pDohyo != NULL)
		{
			pDohyo->BindModel(CLoadModel::GetBuffMat(CLoadModel::MODEL_NEWSCASTER), CLoadModel::GetNumMat(CLoadModel::MODEL_NEWSCASTER), CLoadModel::GetMesh(CLoadModel::MODEL_NEWSCASTER));
			pDohyo->BindMat(m_pTexture);
			pDohyo->Init(pos);
		}
	}

	return pDohyo;
}

//=============================================================================
// ニュースキャスター初期化処理
//=============================================================================
HRESULT CNewsCaster::Init(D3DXVECTOR3 pos)
{
	// 2Dオブジェクト初期化処理
	CSceneX::Init(pos);

	// オブジェクトの種類の設定
	SetObjType(CScene::OBJTYPE_SCENEX);

	m_nCntTimer = 0;

	return S_OK;
}

//=============================================================================
// ニュースキャスター終了処理
//=============================================================================
void CNewsCaster::Uninit(void)
{
	// 2Dオブジェクト終了処理
	CSceneX::Uninit();
}

//=============================================================================
// ニュースキャスター更新処理
//=============================================================================
void CNewsCaster::Update(void)
{
	// 位置取得
	D3DXVECTOR3 pos;
	pos = CSceneX::GetPosition();

	// 位置取得
	D3DXVECTOR3 rot;
	rot = CSceneX::GetRot();

	// タイトル取得
	CTitle *pTitle;
	pTitle = CManager::GetTitle();

	if (rot.x >= -0.7f)
	{
		rot.x -= 0.01f;
	}
	else if (rot.x < -0.7f)
	{
		m_nCntTimer++;
		int nTime = m_nCntTimer / 60;

		if (nTime >= 1)
		{
			pTitle->SetState(CTitle::STATE_TITLE);
		}
	}

	CSceneX::SetPosition(pos);
	CSceneX::SetRot(rot);

	if (pTitle->GetState() == CTitle::STATE_TITLE)
	{
		Uninit();
	}

#ifdef _DEBUG
	CDebugProc::Print("cf", "向き : ", rot.x);
#endif
}

//=============================================================================
// ニュースキャスター描画処理
//=============================================================================
void CNewsCaster::Draw(void)
{
	// 2Dオブジェクト描画処理
	CSceneX::Draw();
}

//=============================================================================
// ブロックのモデル読み込み処理
//=============================================================================
HRESULT CNewsCaster::LoadMat(void)
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
	pMat = (D3DXMATERIAL*)CLoadModel::GetBuffMat(CLoadModel::MODEL_NEWSCASTER)->GetBufferPointer();

	// マテリアルの数分テクスチャを入れるものを動的に確保
	m_pTexture = new LPDIRECT3DTEXTURE9[CLoadModel::GetNumMat(CLoadModel::MODEL_NEWSCASTER)];

	for (int nCntMat = 0; nCntMat < (int)CLoadModel::GetNumMat(CLoadModel::MODEL_NEWSCASTER); nCntMat++)
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
void CNewsCaster::UnloadMat(void)
{
	if (m_pTexture != NULL)
	{// テクスチャのポインタのNULLチェック(家)
		for (int nCntMat = 0; nCntMat < (int)CLoadModel::GetNumMat(CLoadModel::MODEL_NEWSCASTER); nCntMat++)
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