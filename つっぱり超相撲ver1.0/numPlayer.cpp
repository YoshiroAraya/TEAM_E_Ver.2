//=============================================================================
//
// 人数選択処理 [numPlayer.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "numPlayer.h"
#include "renderer.h"
#include "input.h"
#include "scene2D.h"
#include "manager.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "load.h"
#include "title.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUMPLAYER_POS_X		((SCREEN_WIDTH / 2))		// ３すくみの左上X座標
#define NUMPLAYER_POS_Y		((SCREEN_HEIGHT / 2))		// ３すくみの左上Y座標
#define PAUSE_WIDTH		((SCREEN_WIDTH / 2))		// ３すくみの幅
#define PAUSE_HEIGHT	((SCREEN_HEIGHT / 2))		// ３すくみの高さ
#define PAUSE_SIZE		(200)						// ３すくみのサイズ

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CNumPlayer::MODE CNumPlayer::m_Mode = CNumPlayer::MODE_2P;
bool CNumPlayer::m_bDecision = false;

//=============================================================================
// ３すくみの生成処理
//=============================================================================
CNumPlayer *CNumPlayer::Create(D3DXVECTOR3 pos)
{
	CNumPlayer *pNumPlayer = NULL;

	if (pNumPlayer == NULL)
	{
		// 背景クラスの生成
		pNumPlayer = new CNumPlayer;

		if (pNumPlayer != NULL)
		{
			pNumPlayer->Init(pos);
		}
	}
	return pNumPlayer;
}

//=============================================================================
// ３すくみクラスのコンストラクタ
//=============================================================================
CNumPlayer::CNumPlayer(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 値をクリア
	for (int nCntPause = 0; nCntPause < MAX_NUMPLAYER; nCntPause++)
	{
		m_apScene2D[nCntPause] = NULL;
	}

	for (int nCntPause = 0; nCntPause < MAX_NUMPLAYER; nCntPause++)
	{
		m_aCol[nCntPause] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}

	m_nSelect = 0;
}

//=============================================================================
// ３すくみクラスのデストラクタ
//=============================================================================
CNumPlayer::~CNumPlayer()
{
}

//=============================================================================
// ３すくみ初期化処理
//=============================================================================
HRESULT CNumPlayer::Init(D3DXVECTOR3 pos)
{
	m_nSelect = 0;
	m_Mode = MODE_1P;
	m_bDecision = false;

	for (int nCntPause = 0; nCntPause < MAX_NUMPLAYER; nCntPause++)
	{
		m_aSelect[nCntPause] = SELECT_NONE;
		m_aCol[nCntPause] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	m_apScene2D[0] = new CScene2D(7);
	m_apScene2D[0]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_1P));
	m_apScene2D[0]->Init(pos);
	m_apScene2D[0]->SetWidthHeight(250.0f, 100.0f);
	m_apScene2D[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	// 1Pを選択状態にする
	m_aSelect[0] = SELECT_SELECT;

	m_apScene2D[1] = new CScene2D(7);
	m_apScene2D[1]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_2P));
	m_apScene2D[1]->Init(D3DXVECTOR3(pos.x + 400.0f, pos.y, 0.0f));
	m_apScene2D[1]->SetWidthHeight(250.0f, 100.0f);
	m_apScene2D[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_apScene2D[2] = new CScene2D(7);
	m_apScene2D[2]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_TUTORIAL));
	m_apScene2D[2]->Init(D3DXVECTOR3(pos.x + 800.0f, pos.y, 0.0f));
	m_apScene2D[2]->SetWidthHeight(300.0f, 100.0f);
	m_apScene2D[2]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));


	return S_OK;
}

//=============================================================================
// ３すくみ終了処理
//=============================================================================
void CNumPlayer::Uninit(void)
{
	// 2Dオブジェクト終了処理
	for (int nCntPause = 0; nCntPause < MAX_NUMPLAYER; nCntPause++)
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
void CNumPlayer::Update(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();
	CManager *pManager = NULL;
	// プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	// エネミーの取得
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

	// タイトル取得
	CTitle *pTitle;
	pTitle = CManager::GetTitle();

	if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true
		|| pXInput->GetTrigger(XPLAYER_RIGHT, 0) == true
		|| pXInput->GetTrigger(XENEMY_RIGHT, 1) == true)
	{
		//pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
		m_aSelect[m_nSelect] = SELECT_NONE;
		m_nSelect = (m_nSelect + 1) % 3;
		//if (m_nSelect == 0)
		//{
		//	m_nSelect = (m_nSelect + 1);
		//}
		m_aSelect[m_nSelect] = SELECT_SELECT;
	}
	if (pInputKeyboard->GetTrigger(DIK_LEFT) == true
		|| pXInput->GetTrigger(XPLAYER_LEFT, 0) == true
		|| pXInput->GetTrigger(XENEMY_LEFT, 1) == true)
	{
		//pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
		m_aSelect[m_nSelect] = SELECT_NONE;
		m_nSelect = (m_nSelect + 2) % 3;
		/*if (m_nSelect == 1)
		{
			m_nSelect = (m_nSelect - 1);
		}*/
		m_aSelect[m_nSelect] = SELECT_SELECT;
	}

	for (int nCntPause = 0; nCntPause < MAX_NUMPLAYER; nCntPause++)
	{
		if (m_aSelect[nCntPause] == SELECT_SELECT)
		{// 選択
			m_aCol[m_nSelect] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{// 選択していない
			m_aCol[m_nSelect] = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
		}

		if (m_apScene2D[nCntPause] != NULL)
		{// 情報が入っていたら色を設定する
			m_apScene2D[nCntPause]->SetCol(m_aCol[m_nSelect]);
		}
	}

	if (pTitle->GetState() == CTitle::STATE_TITLE && pInputKeyboard->GetTrigger(DIK_RETURN) == true
		|| pTitle->GetState() == CTitle::STATE_TITLE && pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true
		|| pTitle->GetState() == CTitle::STATE_TITLE && pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
	{
		m_bDecision = true;

		if (m_aSelect[0] == SELECT_SELECT)
		{
			m_Mode = MODE_1P;
			pManager->SetNumPlayerMode(0);
		}
		else if (m_aSelect[1] == SELECT_SELECT)
		{
			m_Mode = MODE_2P;
			pManager->SetNumPlayerMode(1);
		}
		else if (m_aSelect[2] == SELECT_SELECT)
		{
			m_Mode = MODE_TUTORIAL;
			pManager->SetNumPlayerMode(2);
		}
	}

	if (pTitle->GetState() == CTitle::STATE_CHARASELECT)
	{
		Uninit();
	}
}

//=============================================================================
// ３すくみ描画処理
//=============================================================================
void CNumPlayer::Draw(void)
{
}

//=============================================================================
// ３すくみセレクトの取得
//=============================================================================
CNumPlayer::MODE CNumPlayer::GetMode(void)
{
	return m_Mode;
}

//=============================================================================
// セレクトの取得
//=============================================================================
int CNumPlayer::GetSelect(void)
{
	return m_nSelect;
}