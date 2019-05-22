//=============================================================================
//
// ポーズの処理 [pause.cpp]
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
#define PAUSE_POS_X		((SCREEN_WIDTH / 2))		// ポーズの左上X座標
#define PAUSE_POS_Y		((SCREEN_HEIGHT / 2))		// ポーズの左上Y座標
#define PAUSE_WIDTH		((SCREEN_WIDTH / 2))		// ポーズの幅
#define PAUSE_HEIGHT	((SCREEN_HEIGHT / 2))		// ポーズの高さ
#define PAUSE_SIZE		(200)						// ポーズのサイズ

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CJankenUI::m_apTexture[MAX_PAUSE] = {};
CJankenUI::MODE CJankenUI::m_Mode = CJankenUI::MODE_GU;

//=============================================================================
// ポーズの生成処理
//=============================================================================
CJankenUI *CJankenUI::Create(D3DXVECTOR3 pos)
{
	CJankenUI *pPause = NULL;

	if (pPause == NULL)
	{
		// 背景クラスの生成
		pPause = new CJankenUI;

		if (pPause != NULL)
		{
			pPause->Init(pos);

			//D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 200.0f, 200.0f
		}
	}
	return pPause;
}

//=============================================================================
// ポーズクラスのコンストラクタ
//=============================================================================
CJankenUI::CJankenUI(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 値をクリア
	for (int nCntPause = 0; nCntPause < MAX_PAUSE_TEXTURE; nCntPause++)
	{
		m_apScene2D[nCntPause] = NULL;
	}

	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{
		m_aCol[nCntPause] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}

	m_nSelect = 0;
}

//=============================================================================
// ポーズクラスのデストラクタ
//=============================================================================
CJankenUI::~CJankenUI()
{
}

//=============================================================================
// ポーズ初期化処理
//=============================================================================
HRESULT CJankenUI::Init(D3DXVECTOR3 pos)
{
	m_nSelect = 0;
	m_Mode = MODE_GU;

	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
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
	m_apScene2D[3]->Init(D3DXVECTOR3(pos.x + 800.0f, pos.y, 0.0f));
	m_apScene2D[3]->SetWidthHeight(50.0f, 50.0f);
	m_apScene2D[3]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_apScene2D[4] = new CScene2D(7);
	m_apScene2D[4]->BindTexture(m_apTexture[1]);
	m_apScene2D[4]->Init(D3DXVECTOR3(pos.x + 50.0f + 800.0f, pos.y + 60.0f, 0.0f));
	m_apScene2D[4]->SetWidthHeight(50.0f, 50.0f);
	m_apScene2D[4]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_apScene2D[5] = new CScene2D(7);
	m_apScene2D[5]->BindTexture(m_apTexture[2]);
	m_apScene2D[5]->Init(D3DXVECTOR3(pos.x - 50.0f + 800.0f, pos.y + 60.0f, 0.0f));
	m_apScene2D[5]->SetWidthHeight(50.0f, 50.0f);
	m_apScene2D[5]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	return S_OK;
}

//=============================================================================
// ポーズ終了処理
//=============================================================================
void CJankenUI::Uninit(void)
{
	// 2Dオブジェクト終了処理
	for (int nCntPause = 0; nCntPause < MAX_PAUSE_TEXTURE; nCntPause++)
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
// ポーズ更新処理
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
		for (int nCntUI = 0; nCntUI < MAX_PAUSE; nCntUI++)
		{
			m_apScene2D[nCntUI]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
		}
	}

	if (pEnemy->GetState() == CEnemy::STATE_NOKOTTA)
	{
		for (int nCntUI = 3; nCntUI < MAX_PAUSE_TEXTURE; nCntUI++)
		{
			m_apScene2D[nCntUI]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
		}
	}

	//if (pInputKeyboard->GetTrigger(DIK_DOWN) == true || pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_DOWN) == true
	//	|| pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_LS_DOWN) == true)
	//{
	//	pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
	//	m_aSelect[m_nSelect] = SELECT_NONE;
	//	m_nSelect = (m_nSelect + 1) % 3;
	//	m_aSelect[m_nSelect] = SELECT_SELECT;
	//}
	//if (pInputKeyboard->GetTrigger(DIK_UP) == true || pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_UP) == true
	//	|| pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_LS_UP) == true)
	//{
	//	pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
	//	m_aSelect[m_nSelect] = SELECT_NONE;
	//	m_nSelect = (m_nSelect + 2) % 3;
	//	m_aSelect[m_nSelect] = SELECT_SELECT;
	//}

	//for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	//{
	//	if (m_aSelect[nCntPause] == SELECT_SELECT)
	//	{// 選択
	//		m_aCol[m_nSelect] = D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f);
	//	}
	//	else
	//	{// 選択していない
	//		m_aCol[m_nSelect] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//	}

	//	if (m_apScene2D[nCntPause + 1] != NULL)
	//	{// 情報が入っていたら色を設定する
	//		m_apScene2D[nCntPause + 1]->SetCol(m_aCol[m_nSelect]);
	//	}
	//}
}

//=============================================================================
// ポーズ描画処理
//=============================================================================
void CJankenUI::Draw(void)
{
}

//=============================================================================
// ポーズのテクスチャ読み込み処理
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
	D3DXCreateTextureFromFile(pDevice, PAUSE_NAME, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, PAUSET_TEXTURE_NAME0, &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, PAUSET_TEXTURE_NAME1, &m_apTexture[2]);

	return S_OK;
}

//=============================================================================
// ポーズのテクスチャ解放処理
//=============================================================================
void CJankenUI::Unload(void)
{
	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{
		if (m_apTexture[nCntPause] != NULL)
		{// テクスチャの破棄
			m_apTexture[nCntPause]->Release();
			m_apTexture[nCntPause] = NULL;
		}
	}
}

//=============================================================================
// ポーズセレクトの取得
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