//=============================================================================
//
// ポリゴン処理 [Bullet.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

//=============================================================================
// プレイヤークラスのコンストラクタ
//=============================================================================
CBullet::CBullet() : CBillboard(3)
{
	// 値をクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nLife = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBullet::~CBullet()
{
}

//=============================================================================
// 弾の生成処理
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col)
{
	CBullet *pBullet = NULL;

	if (pBullet == NULL)
	{
		// オブジェクトクラスの生成
		pBullet = new CBullet;

		if (pBullet != NULL)
		{
			pBullet->Init(pos);
			pBullet->m_move = move;
			pBullet->m_col = col;
			pBullet->BindTexture(m_pTexture);
		}
	}
	return pBullet;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos)
{
	CBillboard::Init(pos);

	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nLife = 50;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBullet::Uninit(void)
{	
	CBillboard::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBullet::Update(void)
{
	// 頂点バッファを取得
	LPDIRECT3DVERTEXBUFFER9 VtxBuff;
	VtxBuff = CBillboard::GetVtxBuff();

	// 位置取得
	D3DXVECTOR3 pos;
	pos = CBillboard::GetPosition();

	VERTEX_3D *pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし頂点データのポインタを取得
	VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 弾を動かす
	pos.x += m_move.x;
	pos.y += m_move.y;
	pos.z += m_move.z;

	// 弾のライフを減らす
	m_nLife--;

	if (m_nLife <= 0)
	{//	弾削除
		Uninit();
	}

	//頂点バッファをアンロックする
	VtxBuff->Unlock();

	CBillboard::SetPosition(pos);

#ifdef _DEBUG
	//CDebugProc::Print("cfccfccfc", "BulletPos : x", pos.x, "f", "   y", pos.y, "f", "  z", pos.z, "f");
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CBullet::Draw(void)
{
	CBillboard::Draw();
}

//=============================================================================
// プレイヤーのテクスチャ読み込み処理
//=============================================================================
HRESULT CBullet::Load(void)
{
	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, BULLET_TEXTURENAME, &m_pTexture);

	return S_OK;
}

//=============================================================================
// プレイヤーのテクスチャ解放処理
//=============================================================================
void CBullet::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}