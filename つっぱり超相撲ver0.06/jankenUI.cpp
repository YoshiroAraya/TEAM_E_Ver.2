//=============================================================================
//
// ３すくみの処理 [pause.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "jankenUI.h"
#include "renderer.h"
#include "input.h"
#include "scene2D.h"
#include "manager.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PAUSE_POS_X		((SCREEN_WIDTH / 2))		// ３すくみの左上X座標
#define PAUSE_POS_Y		((SCREEN_HEIGHT / 2))		// ３すくみの左上Y座標
#define PAUSE_WIDTH		((SCREEN_WIDTH / 2))		// ３すくみの幅
#define PAUSE_HEIGHT	((SCREEN_HEIGHT / 2))		// ３すくみの高さ
#define PAUSE_SIZE		(200)						// ３すくみのサイズ

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CJankenUI::m_apTexture[MAX_JANKENUI] = {};
CJankenUI::MODE CJankenUI::m_Mode = CJankenUI::MODE_GU;

//=============================================================================
// ３すくみの生成処理
//=============================================================================
CJankenUI *CJankenUI::Create(D3DXVECTOR3 pos)
{
	CJankenUI *pJankenUI = NULL;

	if (pJankenUI == NULL)
	{
		// 背景クラスの生成
		pJankenUI = new CJankenUI;

		if (pJankenUI != NULL)
		{
			pJankenUI->Init(pos);
		}
	}
	return pJankenUI;
}

//=============================================================================
// ３すくみクラスのコンストラクタ
//=============================================================================
CJankenUI::CJankenUI(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 値をクリア
	for (int nCntPause = 0; nCntPause < MAX_JANKENUI_TEXTURE; nCntPause++)
	{
		m_apScene2D[nCntPause] = NULL;
	}

	for (int nCntPause = 0; nCntPause < MAX_JANKENUI; nCntPause++)
	{
		m_aCol[nCntPause] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}

	m_nSelect = 0;
}

//=============================================================================
// ３すくみクラスのデストラクタ
//=============================================================================
CJankenUI::~CJankenUI()
{
}

//=============================================================================
// ３すくみ初期化処理
//=============================================================================
HRESULT CJankenUI::Init(D3DXVECTOR3 pos)
{
	m_nSelect = 0;
	m_Mode = MODE_GU;

	for (int nCntPause = 0; nCntPause < MAX_JANKENUI; nCntPause++)
	{
		m_aSelect[nCntPause] = SELECT_NONE;
		m_aCol[nCntPause] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	m_apScene2D[0] = new CScene2D(7);
	m_apScene2D[0]->BindTexture(m_apTexture[0]);
	m_apScene2D[0]->Init(pos);
	m_apScene2D[0]->SetWidthHeight(50.0f, 50.0f);
	m_apScene2D[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_apScene2D[1] = new CScene2D(7);
	m_apScene2D[1]->BindTexture(m_apTexture[1]);
	m_apScene2D[1]->Init(D3DXVECTOR3(pos.x + 50.0f, pos.y + 60.0f, 0.0f));
	m_apScene2D[1]->SetWidthHeight(50.0f, 50.0f);
	m_apScene2D[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_apScene2D[2] = new CScene2D(7);
	m_apScene2D[2]->BindTexture(m_apTexture[2]);
	m_apScene2D[2]->Init(D3DXVECTOR3(pos.x - 50.0f, pos.y + 60.0f, 0.0f));
	m_apScene2D[2]->SetWidthHeight(50.0f, 50.0f);
	m_apScene2D[2]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_apScene2D[3] = new CScene2D(7);
	m_apScene2D[3]->BindTexture(m_apTexture[0]);
	m_apScene2D[3]->Init(D3DXVECTOR3(SCREEN_WIDTH - pos.x, pos.y, 0.0f));
	m_apScene2D[3]->SetWidthHeight(50.0f, 50.0f);
	m_apScene2D[3]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_apScene2D[4] = new CScene2D(7);
	m_apScene2D[4]->BindTexture(m_apTexture[1]);
	m_apScene2D[4]->Init(D3DXVECTOR3(SCREEN_WIDTH - pos.x + 50.0f, pos.y + 60.0f, 0.0f));
	m_apScene2D[4]->SetWidthHeight(50.0f, 50.0f);
	m_apScene2D[4]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_apScene2D[5] = new CScene2D(7);
	m_apScene2D[5]->BindTexture(m_apTexture[2]);
	m_apScene2D[5]->Init(D3DXVECTOR3(SCREEN_WIDTH - pos.x - 50.0f, pos.y + 60.0f, 0.0f));
	m_apScene2D[5]->SetWidthHeight(50.0f, 50.0f);
	m_apScene2D[5]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	return S_OK;
}

//=============================================================================
// ３すくみ終了処理
//=============================================================================
void CJankenUI::Uninit(void)
{
	// 2Dオブジェクト終了処理
	for (int nCntPause = 0; nCntPause < MAX_JANKENUI_TEXTURE; nCntPause++)
	{
		if (m_apScene2D[nCntPause] != NULL)
		{
			m_apScene2D[nCntPause]->Uninit();
			m_apScene2D[nCntPause] = NULL;
		}
	}

	// オブジェクトの解放
	Release();
}

//=============================================================================
// ３すくみ更新処理
//=============================================================================
void CJankenUI::Update(void)
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

	if (pPlayer->GetState() == CPlayer::STATE_NOKOTTA)
	{
		for (int nCntUI = 0; nCntUI < MAX_JANKENUI; nCntUI++)
		{
			m_apScene2D[nCntUI]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
		}
	}

	if (pEnemy->GetState() == CEnemy::STATE_NOKOTTA)
	{
		for (int nCntUI = 3; nCntUI < MAX_JANKENUI_TEXTURE; nCntUI++)
		{
			m_apScene2D[nCntUI]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
		}
	}

	if (pPlayer->GetState() == CPlayer::STATE_NEUTRAL && pEnemy->GetState() == CEnemy::STATE_NEUTRAL
		|| pPlayer->GetState() == CPlayer::STATE_KUMI && pEnemy->GetState() == CEnemy::STATE_KUMI)
	{
		Uninit();
	}
}

//=============================================================================
// ３すくみ描画処理
//=============================================================================
void CJankenUI::Draw(void)
{
}

//=============================================================================
// ３すくみのテクスチャ読み込み処理
//=============================================================================
HRESULT CJankenUI::Load(void)
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
	D3DXCreateTextureFromFile(pDevice, BUCHIKAMASHI, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, HENKA, &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, STUPPARI, &m_apTexture[2]);

	return S_OK;
}

//=============================================================================
// ３すくみのテクスチャ解放処理
//=============================================================================
void CJankenUI::Unload(void)
{
	for (int nCntPause = 0; nCntPause < MAX_JANKENUI; nCntPause++)
	{
		if (m_apTexture[nCntPause] != NULL)
		{// テクスチャの破棄
			m_apTexture[nCntPause]->Release();
			m_apTexture[nCntPause] = NULL;
		}
	}
}

//=============================================================================
// ３すくみセレクトの取得
//=============================================================================
CJankenUI::MODE CJankenUI::GetMode(void)
{
	return m_Mode;
}

//=============================================================================
// セレクトの取得
//=============================================================================
int CJankenUI::GetSelect(void)
{
	return m_nSelect;
}