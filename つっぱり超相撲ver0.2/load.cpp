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
	"data\\MODEL\\dohyo2.x",
	"data\\MODEL\\rikisi.x",
	"data\\MODEL\\rikisi2.x",
	"data\\MODEL\\customer.x",
	"data\\MODEL\\shadow000.x",
	"data\\MODEL\\newscaster.x",
};

const char *CLoad::m_apTexFilename[] =
{// テクスチャ
	"data/TEXTURE/dohyo.png",					//0
	"data/TEXTURE/TITLE/newBG.png",				//1
	"data/TEXTURE/sand2.jpg",					//2
	"data/TEXTURE/wall.png",					//3
	"data\\TEXTURE\\TITLE\\title.png",			//4
	"data/TEXTURE/time.png",					//5
	"data/TEXTURE/.png",						//6 ゲージ
	"data/TEXTURE/JANKEN/Buchikamashi.png",		//7
	"data/TEXTURE/JANKEN/Henka.png",			//8
	"data/TEXTURE/JANKEN/Stuppari.png",			//9
	"data/TEXTURE/touzai.png",					//10
	"data/TEXTURE/TITLE/1P.png",				//11
	"data/TEXTURE/TITLE/2P.png",				//12
	"data/TEXTURE/TITLE/tutorial01.png",		//13
	"data\\TEXTURE\\EFFECT\\particle000.jpg",	//14
	"data\\TEXTURE\\EFFECT\\money.png",			//15
	"data/TEXTURE/EFFECT/16.png",				//16
	"data/TEXTURE/EFFECT/somke.png",			//17
	"data/TEXTURE/circle.png",					//18
	"data/TEXTURE/UI/Tyanco.png",				//19
	"data/TEXTURE/UI/number002.png"	,			//20
	"data/TEXTURE/UI/Select00.png",				//21
	"data/TEXTURE/UI/Select01.png",				//22
	"data/TEXTURE/UI/Select02.png",				//23
	"data/TEXTURE/UI/pause100.png",				//24
	"data/TEXTURE/UI/Select03.png",				//25
	"data/TEXTURE/UI/Select04.png",				//26
	"data/TEXTURE/UI/Select02.png",				//27
	"data/TEXTURE/UI/pause100.png",				//28
	"data/TEXTURE/UI/select.png",				//29
	"data/TEXTURE/EFFECT/all.png",				//30
	"data/TEXTURE/UI/tutorial00.jpg",			//31
	"data/TEXTURE/UI/tutorial01.jpg",			//32
	"data/TEXTURE/UI/tutorial02.jpg",			//33
	"data/TEXTURE/UI/sensu.png",				//34
	"data/TEXTURE/UI/line.png",					//35
	"data/TEXTURE/UI/ippon.png",				//36
	"data/TEXTURE/UI/HPgauge.png",				//37
	"data/TEXTURE/UI/EXgauge.png",				//38
	"data/TEXTURE/UI/ResultWinner.png",			//39
	"data/TEXTURE/EFFECT/10.png",				//40
	"data/TEXTURE/EFFECT/19.png",				//41
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

	if (m_pMesh != NULL)
	{
		// メモリを解放する
		delete[] m_pMesh;
		m_pMesh = NULL;
	}

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