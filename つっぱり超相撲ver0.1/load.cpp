//=============================================================================
//
// テクスチャ処理 [texture.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "load.h"
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "debugProc.h"
#include "manager.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 *CLoad::m_pTexture = NULL;
LPD3DXMESH *CLoad::m_pMesh = NULL;			// メッシュ情報（頂点情報）へのポインタ
LPD3DXBUFFER *CLoad::m_pBuffMat = NULL;	// マテリアル情報へのポインタ
DWORD *CLoad::m_nNumMat = 0;				// マテリアル情報の数

const char *CLoad::m_apModelFilename[] =
{// モデル
	"data\\MODEL\\dohyo.x",
	"data\\MODEL\\rikisi.x",
	"data\\MODEL\\rikisi2.x",
	"data\\MODEL\\customer.x",
	"data\\MODEL\\shadow000.x",
	"data\\MODEL\\rikisi.x",
};

const char *CLoad::m_apTexFilename[] =
{// テクスチャ
	"data/TEXTURE/dohyo.png",
	"data/TEXTURE/TITLE/newBG.png",
	"data/TEXTURE/sand2.jpg",
	"data/TEXTURE/wall.png",
	"data\\TEXTURE\\TITLE\\title.png",
	"data/TEXTURE/time.png",
	"data/TEXTURE/.png",						// ゲージ
	"data/TEXTURE/JANKEN/Buchikamashi.png",
	"data/TEXTURE/JANKEN/Henka.png",
	"data/TEXTURE/JANKEN/Stuppari.png",
	"data/TEXTURE/touzai.png",
	"data\\TEXTURE\\EFFECT\\particle000.jpg",
	"data\\TEXTURE\\EFFECT\\コンポ _0.png",
	"data/TEXTURE/EFFECT/コンポ 10.png",
};

//=============================================================================
// コンストラクタ
//=============================================================================
CLoad::CLoad()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CLoad::~CLoad()
{
}

//=============================================================================
// テクスチャ読み込み処理
//=============================================================================
HRESULT CLoad::LoadModel(void)
{
	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	if (m_pTexture != NULL)
	{
		// メモリを解放する
		delete[] m_pTexture;
		m_pTexture = NULL;
	}

	int nModelData = (sizeof m_apModelFilename);
	int nModelSize = (sizeof m_apModelFilename[0]);
	int nNumModel = nModelData / nModelSize;

	// 動的に確保
	m_pMesh = new LPD3DXMESH[nNumModel];
	m_pBuffMat = new LPD3DXBUFFER[nNumModel];
	m_nNumMat = new DWORD[nNumModel];

	for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
	{
		D3DXLoadMeshFromX(m_apModelFilename[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_pBuffMat[nCntModel],
			NULL,
			&m_nNumMat[nCntModel],
			&m_pMesh[nCntModel]);
	}

	return S_OK;
}

//=============================================================================
// テクスチャ解放処理
//=============================================================================
void CLoad::UnloadModel(void)
{
	if (m_pBuffMat != NULL)
	{
		// メモリを解放する
		delete[] m_pBuffMat;
		m_pBuffMat = NULL;
	}

	if (m_nNumMat != NULL)
	{
		// メモリを解放する
		delete[] m_nNumMat;
		m_nNumMat = NULL;
	}

	if (m_pMesh != NULL)
	{
		// メモリを解放する
		delete[] m_pMesh;
		m_pMesh = NULL;
	}
}

//=============================================================================
// テクスチャ読み込み処理
//=============================================================================
HRESULT CLoad::LoadTex(void)
{
	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	if (m_pTexture != NULL)
	{
		// メモリを解放する
		delete[] m_pTexture;
		m_pTexture = NULL;
	}

	int nTexData = (sizeof m_apTexFilename);
	int nTexSize = (sizeof m_apTexFilename[0]);
	int nNumTex = nTexData / nTexSize;

	// テクスチャの数を動的に確保
	m_pTexture = new LPDIRECT3DTEXTURE9[nNumTex];

	for (int nCntTex = 0; nCntTex < nNumTex; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, m_apTexFilename[nCntTex], &m_pTexture[nCntTex]);
	}

	return S_OK;
}

//=============================================================================
// テクスチャ解放処理
//=============================================================================
void CLoad::UnloadTex(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		// メモリを解放する
		delete[] m_pTexture;
		m_pTexture = NULL;
	}
}

//=============================================================================
// テクスチャ取得
//=============================================================================
LPDIRECT3DTEXTURE9 CLoad::GetTexture(int nIdx)
{
	return m_pTexture[nIdx];
}

//=============================================================================
// メッシュ取得
//=============================================================================
LPD3DXMESH CLoad::GetMesh(int nIdx)
{
	return m_pMesh[nIdx];
}

//=============================================================================
// マテリアル情報取得
//=============================================================================
LPD3DXBUFFER CLoad::GetBuffMat(int nIdx)
{
	return m_pBuffMat[nIdx];
}

//=============================================================================
// マテリアル情報の数取得
//=============================================================================
DWORD CLoad::GetNumMat(int nIdx)
{
	return m_nNumMat[nIdx];
}