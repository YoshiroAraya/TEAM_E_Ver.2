//=============================================================================
//
// シーン2D処理 [scene2D.h]
// Author : 長山拓実
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "input.h"

//=============================================================================
// シーンクラスのコンストラクタ
//=============================================================================
CScene2D::CScene2D(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 値をクリア
	//m_pTexture = NULL;

	//m_pVtxBuff = NULL;

	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fSpin = 0.0f;	//回転
	m_Col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//色
	m_fScale = 0.0f;	//拡大
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fRight = 0.0f;
	m_fLeft = 0.0f;
	m_bDraw = true;
}

//=============================================================================
// オブジェクトの生成処理
//=============================================================================
CScene2D *CScene2D::Create(D3DXVECTOR3 pos)
{
	CScene2D *pScene2D = NULL;

	if (pScene2D == NULL)
	{
		// オブジェクトクラスの生成
		pScene2D = new CScene2D;

		if (pScene2D != NULL)
		{
			pScene2D->Init(pos);
		}
	}

	return pScene2D;
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene2D::~CScene2D()
{
}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos)
{
	// ポリゴンの位置を設定
	m_Pos = pos;
	m_bDraw = true;

	// オブジェクトの種類の設定
	//SetObjType(CScene::OBJTYPE_SCENE2D);

	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// テクスチャの生成
	//D3DXCreateTextureFromFile(pDevice, PLAYER_NAME, &m_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x - 50.0f, m_Pos.y - 50.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x + 50.0f, m_Pos.y - 50.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x - 50.0f, m_Pos.y + 50.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x + 50.0f, m_Pos.y + 50.0f, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CScene2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクトの解放
	Release();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CScene2D::Update(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

#ifdef _DEBUG
	//CDebugProc::Print("cfccfccfc", "2DPos    : x", m_Pos.x, "f", " y", m_Pos.y, "f", " z", m_Pos.z, "f");
#endif
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CScene2D::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	if (m_bDraw == true)
	{	// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=============================================================================
// オブジェクトの取得
//=============================================================================
D3DXVECTOR3 CScene2D::GetPosition(void)
{
	return m_Pos;
}

//=============================================================================
// オブジェクトの設定
//=============================================================================
void CScene2D::SetPosition(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
// テクスチャの設定
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	//持ってきた情報を代入
	m_pTexture = pTexture;
}

//=============================================================================
//シーン2D位置設定
//=============================================================================
void CScene2D::SetPos(D3DXVECTOR3 pos, float fSpin, float fScale, D3DXCOLOR col)
{
	m_Pos = pos;		//位置
	m_fSpin = fSpin;	//回転
	m_Col = col;		//色
	m_fScale = fScale;	//拡大

	VERTEX_2D *pVtx;//頂点情報へのポインタ

					//頂点バッファをロックし頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = m_Pos.x + sinf(D3DX_PI * -0.75f + m_fSpin) *  (m_fWidth + m_fScale);
	pVtx[0].pos.y = m_Pos.y + cosf(D3DX_PI * -0.75f + m_fSpin) *  (m_fHeight + m_fScale);
	pVtx[1].pos.x = m_Pos.x + sinf(D3DX_PI * 0.75f + m_fSpin) *  (m_fWidth + m_fScale);
	pVtx[1].pos.y = m_Pos.y + cosf(D3DX_PI * 0.75f + m_fSpin) *  (m_fHeight + m_fScale);
	pVtx[2].pos.x = m_Pos.x + sinf(D3DX_PI * -0.25f + m_fSpin) *  (m_fWidth + m_fScale);
	pVtx[2].pos.y = m_Pos.y + cosf(D3DX_PI * -0.25f + m_fSpin) *  (m_fHeight + m_fScale);
	pVtx[3].pos.x = m_Pos.x + sinf(D3DX_PI * 0.25f + m_fSpin) *  (m_fWidth + m_fScale);
	pVtx[3].pos.y = m_Pos.y + cosf(D3DX_PI * 0.25f + m_fSpin) *  (m_fHeight + m_fScale);

	//頂点カラー
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;


	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

//=============================================================================
//シーン2D回転設定
//=============================================================================
void CScene2D::SetRot(float fSpin)
{
	m_fSpin = fSpin;	//回転

	VERTEX_2D *pVtx;//頂点情報へのポインタ

					//頂点バッファをロックし頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
#if 1
	pVtx[0].pos.x = m_Pos.x + sinf(D3DX_PI * -0.75f + m_fSpin) * m_fLength;
	pVtx[0].pos.y = m_Pos.y + cosf(D3DX_PI * -0.75f + m_fSpin) *  m_fLength;
	pVtx[1].pos.x = m_Pos.x + sinf(D3DX_PI * 0.75f + m_fSpin) *  m_fLength;
	pVtx[1].pos.y = m_Pos.y + cosf(D3DX_PI * 0.75f + m_fSpin) *  m_fLength;
	pVtx[2].pos.x = m_Pos.x + sinf(D3DX_PI * -0.25f + m_fSpin) *  m_fLength;
	pVtx[2].pos.y = m_Pos.y + cosf(D3DX_PI * -0.25f + m_fSpin) *  m_fLength;
	pVtx[3].pos.x = m_Pos.x + sinf(D3DX_PI * 0.25f + m_fSpin) *  m_fLength;
	pVtx[3].pos.y = m_Pos.y + cosf(D3DX_PI * 0.25f + m_fSpin) *  m_fLength;
#endif
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

//=============================================================================
//シーン2D大きさ設定
//=============================================================================
void CScene2D::SetWidthHeight(float fWidth, float fHeight)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;
	//頂点バッファをロックし頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = m_Pos.x + sinf(D3DX_PI * -0.75f) * m_fWidth;
	pVtx[0].pos.y = m_Pos.y + cosf(D3DX_PI * -0.75f) * m_fHeight;
	pVtx[1].pos.x = m_Pos.x + sinf(D3DX_PI * 0.75f) * m_fWidth;
	pVtx[1].pos.y = m_Pos.y + cosf(D3DX_PI * 0.75f) * m_fHeight;
	pVtx[2].pos.x = m_Pos.x + sinf(D3DX_PI * -0.25f) * m_fWidth;
	pVtx[2].pos.y = m_Pos.y + cosf(D3DX_PI * -0.25f) * m_fHeight;
	pVtx[3].pos.x = m_Pos.x + sinf(D3DX_PI * 0.25f) * m_fWidth;
	pVtx[3].pos.y = m_Pos.y + cosf(D3DX_PI * 0.25f) * m_fHeight;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//シーン2D左右の大きさ設定
//=============================================================================
void CScene2D::SetRIghtLeft(float fRight, float fLeft)
{
	m_fRight = fRight;
	m_fLeft = fLeft;

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;
	//頂点バッファをロックし頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = m_Pos.x + sinf(D3DX_PI * -0.75f) * (m_fWidth + m_fLeft);
	pVtx[0].pos.y = m_Pos.y + cosf(D3DX_PI * -0.75f) * m_fHeight;
	pVtx[1].pos.x = m_Pos.x + sinf(D3DX_PI * 0.75f) * (m_fWidth + m_fRight);
	pVtx[1].pos.y = m_Pos.y + cosf(D3DX_PI * 0.75f) * m_fHeight;
	pVtx[2].pos.x = m_Pos.x + sinf(D3DX_PI * -0.25f) * (m_fWidth + m_fLeft);
	pVtx[2].pos.y = m_Pos.y + cosf(D3DX_PI * -0.25f) * m_fHeight;
	pVtx[3].pos.x = m_Pos.x + sinf(D3DX_PI * 0.25f) * (m_fWidth + m_fRight);
	pVtx[3].pos.y = m_Pos.y + cosf(D3DX_PI * 0.25f) * m_fHeight;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//シーン2D色の変更
//=============================================================================
void CScene2D::SetCol(D3DXCOLOR col)
{
	m_Col = col;		//色

	VERTEX_2D *pVtx;
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
