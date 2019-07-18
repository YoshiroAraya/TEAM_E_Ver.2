//---------------------------------------------------------------------
//	ナンバー処理(number.cpp)
//	Author : Mikiya Meguro
//---------------------------------------------------------------------
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "load.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUMBER_TEXTURENAME  "data\\TEXTURE\\UI\\number001.png"		//テクスチャのファイル名
#define NUMBER_SIZE			(15)									//頂点座標Xの移動量

//--------------------------------------------
//静的メンバ変数宣言
//--------------------------------------------
LPDIRECT3DTEXTURE9 CNumber ::m_pTexture = NULL;

//--------------------------------------------
//ナンバークラス コンストラクタ
//--------------------------------------------
CNumber::CNumber()
{
	m_pos = D3DXVECTOR3(0,0,0);
	m_col = D3DXCOLOR(0,0,0,0);
	m_fWidth = 0;	// 幅
	m_fHeight = 0;	// 高さ
	m_fScale = 0;	// 拡大
	m_nNumber = 0;	// 数値
}

//--------------------------------------------
//ナンバークラス デストラクタ
//--------------------------------------------
CNumber::~CNumber()
{
}

//--------------------------------------------
//オブジェクトの生成
//--------------------------------------------
CNumber *CNumber::Create(D3DXVECTOR3 pos,D3DXCOLOR col, float fWidth, float fHeight)
{
	CNumber *pNumber;

	pNumber = new CNumber;

	pNumber->m_pos = pos;
	pNumber->m_col = col;
	pNumber->m_fWidth = fWidth;
	pNumber->m_fHeight = fHeight;
	pNumber->Init();

	return pNumber;
}


//=============================================================================
// 数字の初期化処理
//=============================================================================
HRESULT CNumber::Init(void)
{
	VERTEX_2D *pVtx;//頂点情報へのポインタ

	//デバイスを取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	m_pTexture = CLoad::GetTexture(CLoad::TEXTURE_NUMBER_UI);

	// 頂点情報を設定
	//頂点バッファをロックし頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標

	pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI * -0.75f) *  (m_fWidth + m_fScale);
	pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI * -0.75f) *  (m_fHeight + m_fScale);
	pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI * 0.75f) *  (m_fWidth + m_fScale);
	pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI * 0.75f) *  (m_fHeight + m_fScale);
	pVtx[2].pos.x = m_pos.x + sinf(D3DX_PI * -0.25f) *  (m_fWidth + m_fScale);
	pVtx[2].pos.y = m_pos.y + cosf(D3DX_PI * -0.25f) *  (m_fHeight + m_fScale);
	pVtx[3].pos.x = m_pos.x + sinf(D3DX_PI * 0.25f) *  (m_fWidth + m_fScale);
	pVtx[3].pos.y = m_pos.y + cosf(D3DX_PI * 0.25f) *  (m_fHeight + m_fScale);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	//頂点
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//オブジェクトタイプの設定
	CScene::SetObjType(OBJTYPE_SCENE2D);

	//値の初期化
	m_fScale = 0;	// 拡大
	m_nNumber = 0;	// 数値

	return S_OK;
}

//=============================================================================
// 数字の終了処理
//=============================================================================
void CNumber::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	//自分を消す(オブジェクトを破棄)
	Release();
}

//=============================================================================
// 数字の更新処理
//=============================================================================
void CNumber::Update(void)
{

}

//=============================================================================
// 数字の描画処理
//=============================================================================
void CNumber::Draw(void)
{
	//デバイスを取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// 数字の描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,	//開始する頂点のインデックス
		2); //描画するプリミティブ数
}

//=============================================================================
// 数字の設定処理
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	m_nNumber = nNumber;

	VERTEX_2D *pVtx;//頂点情報へのポインタ

	//デバイスを取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 頂点情報を設定
	//頂点バッファをロックし頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f + (nNumber * 0.1f), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (nNumber * 0.1f), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}


//=============================================================================
// 数字の取得処理
//=============================================================================
int CNumber::GetNumber(void)
{
	return m_nNumber;
}


//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
HRESULT CNumber::Load(void)
{
	//デバイスを取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice,
		NUMBER_TEXTURENAME,
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄処理
//=============================================================================
void CNumber::UnLoad(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
// カラーのセット
//=============================================================================
void CNumber::SetColor(D3DXCOLOR col)
{
	VERTEX_2D *pVtx;//頂点情報へのポインタ

	//デバイスを取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 頂点情報を設定
	//頂点バッファをロックし頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラー
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

void CNumber::SetScale(float scale)
{
	//拡大値
	m_fScale = scale;

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;
	//頂点バッファをロックし頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI * -0.75f) *  (m_fWidth + m_fScale);
	pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI * -0.75f) *  (m_fHeight + m_fScale);
	pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI * 0.75f) *  (m_fWidth + m_fScale);
	pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI * 0.75f) *  (m_fHeight + m_fScale);
	pVtx[2].pos.x = m_pos.x + sinf(D3DX_PI * -0.25f) *  (m_fWidth + m_fScale);
	pVtx[2].pos.y = m_pos.y + cosf(D3DX_PI * -0.25f) *  (m_fHeight + m_fScale);
	pVtx[3].pos.x = m_pos.x + sinf(D3DX_PI * 0.25f) *  (m_fWidth + m_fScale);
	pVtx[3].pos.y = m_pos.y + cosf(D3DX_PI * 0.25f) *  (m_fHeight + m_fScale);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}
