//=============================================================================
//
// 時間の処理 [touzai.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "time.h"
#include "renderer.h"
#include "input.h"
#include "scene2D.h"
#include "manager.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "camera.h"
#include "title.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PAUSE_POS_X		((SCREEN_WIDTH / 2))		// 時間の左上X座標
#define PAUSE_POS_Y		((SCREEN_HEIGHT / 2))		// 時間の左上Y座標
#define PAUSE_WIDTH		((SCREEN_WIDTH / 2))		// 時間の幅
#define PAUSE_HEIGHT	((SCREEN_HEIGHT / 2))		// 時間の高さ
#define TOUZAI_SIZE		(150.0f)						// 時間のサイズ

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CTime::m_pTexture = NULL;

//=============================================================================
// 時間の生成処理
//=============================================================================
CTime *CTime::Create(D3DXVECTOR3 pos)
{
	CTime *pTouzai = NULL;

	if (pTouzai == NULL)
	{
		// 背景クラスの生成
		pTouzai = new CTime;

		if (pTouzai != NULL)
		{
			pTouzai->Init(pos);
			pTouzai->BindTexture(m_pTexture);
		}
	}
	return pTouzai;
}

//=============================================================================
// 時間クラスのコンストラクタ
//=============================================================================
CTime::CTime() : CScene2D(7)
{
	// 値をクリア
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	m_nSelect = 0;
	m_fScale = 0.0f;
}

//=============================================================================
// 時間クラスのデストラクタ
//=============================================================================
CTime::~CTime()
{
}

//=============================================================================
// 時間初期化処理
//=============================================================================
HRESULT CTime::Init(D3DXVECTOR3 pos)
{
	m_nSelect = 0;
	m_fScale = 50.0f;
	m_bReset = true;

	CScene2D::Init(pos);

	CScene2D::SetWidthHeight(m_fScale, m_fScale - 30.0f);

	// オブジェクトの種類の設定
	CScene2D::SetObjType(CScene::OBJTYPE_UI);

	m_Col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 時間終了処理
//=============================================================================
void CTime::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 時間更新処理
//=============================================================================
void CTime::Update(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();

	// プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	// エネミーの取得
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;
	pVtxBuff = CScene2D::GetBuff();

	// タイトル取得
	CTitle *pTitle;
	pTitle = CManager::GetTitle();

	D3DXVECTOR3 pos = CScene2D::GetPosition();

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

						// 頂点バッファをロックし、頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (pTitle != NULL)
	{
		if (pTitle->GetState() == CTitle::STATE_NEWS)
		{
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 0.5f);
		}
		else if(pTitle->GetState() == CTitle::STATE_TITLE)
		{
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	CScene2D::SetBuff(pVtxBuff);
	CScene2D::SetPos(pos, 0.0f, m_fScale, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	if (CCamera::GetState() == CCamera::STATE_NORMAL)
	{
		Uninit();
	}
}

//=============================================================================
// 時間描画処理
//=============================================================================
void CTime::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// 時間のテクスチャ読み込み処理
//=============================================================================
HRESULT CTime::Load(void)
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
	D3DXCreateTextureFromFile(pDevice, TIME, &m_pTexture);

	return S_OK;
}

//=============================================================================
// 時間のテクスチャ解放処理
//=============================================================================
void CTime::Unload(void)
{
	if (m_pTexture != NULL)
	{// テクスチャの破棄
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}