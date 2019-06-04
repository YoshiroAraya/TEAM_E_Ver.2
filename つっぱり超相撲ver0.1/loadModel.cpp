//=============================================================================
//
// テクスチャ処理 [texture.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "loadModel.h"
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "debugProc.h"
#include "manager.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 *CLoadModel::m_pTexture = NULL;
LPD3DXMESH *CLoadModel::m_pMesh = NULL;			// メッシュ情報（頂点情報）へのポインタ
LPD3DXBUFFER *CLoadModel::m_pBuffMat = NULL;	// マテリアル情報へのポインタ
DWORD *CLoadModel::m_nNumMat = 0;				// マテリアル情報の数

const char *CLoadModel::m_apFilename[] =
{
	"data\\MODEL\\dohyo.x",
	"data\\MODEL\\rikisi.x",
	"data\\MODEL\\rikisi2.x",
	"data\\MODEL\\customer.x",
	"data\\MODEL\\shadow000.x",
	"data\\MODEL\\rikisi.x",
};

//=============================================================================
// コンストラクタ
//=============================================================================
CLoadModel::CLoadModel()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CLoadModel::~CLoadModel()
{
}

//=============================================================================
// テクスチャ読み込み処理
//=============================================================================
HRESULT CLoadModel::Load(void)
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

	int nModelData = (sizeof m_apFilename);
	int nModelSize = (sizeof m_apFilename[0]);
	int nNumModel = nModelData / nModelSize;

	// テクスチャの数を動的に確保
	//m_pTexture = new LPDIRECT3DTEXTURE9[nNumTex];
	//m_pTexture = (LPDIRECT3DTEXTURE9)malloc(sizeof(int) * nNumTex);

	m_pMesh = new LPD3DXMESH[nNumModel];
	m_pBuffMat = new LPD3DXBUFFER[nNumModel];
	m_nNumMat = new DWORD[nNumModel];

	for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
	{
		D3DXLoadMeshFromX(m_apFilename[nCntModel],
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
void CLoadModel::Unload(void)
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
// メッシュ取得
//=============================================================================
LPD3DXMESH CLoadModel::GetMesh(int nIdx)
{
	return m_pMesh[nIdx];
}

//=============================================================================
// マテリアル情報取得
//=============================================================================
LPD3DXBUFFER CLoadModel::GetBuffMat(int nIdx)
{
	return m_pBuffMat[nIdx];
}

//=============================================================================
// マテリアル情報の数取得
//=============================================================================
DWORD CLoadModel::GetNumMat(int nIdx)
{
	return m_nNumMat[nIdx];
}