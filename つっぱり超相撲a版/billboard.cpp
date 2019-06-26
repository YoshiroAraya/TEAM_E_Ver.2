//=============================================================================
//
// ビルボード処理 [Billboard.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "billboard.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURENAME000			 "data\\TEXTURE\\EFFECT\\particle000.jpg"		//テクスチャのファイル名

//=============================================================================
// 3Dポリゴンクラスのコンストラクタ
//=============================================================================
CBillboard::CBillboard(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 値をクリア
	m_pTexture = NULL;						// テクスチャへのポインタ
	m_pVtxBuff = NULL;						// 頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
}

//=============================================================================
// デストラクタ
//=============================================================================
CBillboard::~CBillboard()
{
}

//=============================================================================
// オブジェクトの生成処理
//=============================================================================
CBillboard *CBillboard::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CBillboard *pSceneBillboard = NULL;

	if (pSceneBillboard == NULL)
	{
		// オブジェクトクラスの生成
		pSceneBillboard = new CBillboard;

		if (pSceneBillboard != NULL)
		{
			pSceneBillboard->Init(pos);
			pSceneBillboard->m_fHeight = fHeight;
			pSceneBillboard->m_fWidth = fWidth;
		}
	}

	return pSceneBillboard;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBillboard::Init(D3DXVECTOR3 pos)
{
	// ポリゴンの情報を設定
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ポリゴンの位置を設定
	m_pos = pos;

	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	//テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice, TEXTURENAME000, &m_pTexture);


	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;
	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight,0.0f );
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, 0.0f, 0.0f);

	//法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBillboard::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクトの解放
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBillboard::Update(void)
{
#ifdef _DEBUG
	//CDebugProc::Print("cfccfccfc", "BillboardPos    : x", m_pos.x, "f", "   y", m_pos.y, "f", "   z", m_pos.z, "f");
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CBillboard::Draw(void)
{
	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	D3DXMATRIX  mtxView, mtxTrans;				//計算用マトリックス

	//アルファテスト(透明色を描画しないように)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//逆行列を設定
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	/*m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;*/
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ビルボードの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,	//開始する頂点のインデックス
		2); //描画するプリミティブ数


	//レンダーステートの設定を元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 頂点バッファの取得
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CBillboard::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
// 頂点バッファの設定
//=============================================================================
void CBillboard::SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 VtxBuff)
{
	m_pVtxBuff = VtxBuff;
}

//=============================================================================
// ビルボードの設定処理
//=============================================================================
void CBillboard::SetBillboard(D3DXVECTOR3 pos, float fHeight, float fWidth)
{
	VERTEX_3D *pVtx;//頂点情報へのポインタ
					//頂点バッファをロックし頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	m_pos = pos;

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(-fWidth, fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-fWidth, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fWidth, 0.0f, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// オブジェクトの取得
//=============================================================================
D3DXVECTOR3 CBillboard::GetPosition(void)
{
	return m_pos;
}

//=============================================================================
// オブジェクトの設定
//=============================================================================
void CBillboard::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// 共有テクスチャを割り当てる
//=============================================================================
void CBillboard::BindTexture(LPDIRECT3DTEXTURE9 Texture)
{
	m_pTexture = Texture;
}


//=============================================================================
//ビルボード色設定
//=============================================================================
void CBillboard::SetCol(D3DXCOLOR col)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;
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

//=============================================================================
// アニメーションの設定処理
//=============================================================================
void CBillboard::SetAnimation(int m_PatternAnim, float fUV_U, float fUV_V)
{
	VERTEX_3D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの座標
	pVtx[0].tex = D3DXVECTOR2(fUV_U * m_PatternAnim, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fUV_U * m_PatternAnim + fUV_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(fUV_U * m_PatternAnim, fUV_V);
	pVtx[3].tex = D3DXVECTOR2(fUV_U * m_PatternAnim + fUV_U, fUV_V);

	////テクスチャの座標
	//pVtx[0].tex = D3DXVECTOR2(EXPLOSION_UV_U, EXPLOSION_UV_V * m_PatternAnim);
	//pVtx[1].tex = D3DXVECTOR2(0.0f, EXPLOSION_UV_V * m_PatternAnim);
	//pVtx[2].tex = D3DXVECTOR2(EXPLOSION_UV_U, EXPLOSION_UV_V * m_PatternAnim + EXPLOSION_UV_V);
	//pVtx[3].tex = D3DXVECTOR2(0.0f, EXPLOSION_UV_V * m_PatternAnim + EXPLOSION_UV_V);

	m_PatternAnim = 0;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// サイズの設定
//=============================================================================
void CBillboard::SetSize(float fHeight, float fWidth)
{
	//取得したサイズを代入
	m_fHeight = fHeight;
	m_fWidth = fWidth;
}