//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "title.h"
#include "debugProc.h"
#include "input.h"
#include "manager.h"
#include "fade.h"
#include "newsBG.h"
#include "newsCaster.h"
#include "dohyo.h"
#include "dohyocircle.h"
#include "field.h"
#include "wall.h"
#include "logo.h"
#include "time.h"
#include "load.h"
#include "numPlayer.h"
#include "player.h"
#include "enemy.h"

//============================================================================
//	マクロ定義
//============================================================================

//============================================================================
//静的メンバ変数宣言
//============================================================================
CPlayer *CTitle::m_pPlayer = NULL;
CEnemy *CTitle::m_pEnemy = NULL;

//=============================================================================
//	コンストラクタ
//=============================================================================
CTitle::CTitle()
{
	m_nCntTurn = 0;
	m_nCntReturn = 0;
}

//=============================================================================
//デストラクタ
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
void CTitle::Init(void)
{
	m_state = STATE_NEWS;
	m_bSetDohyo = true;
	m_bTurn = false;
	m_nCntTurn = 0;
	m_nCntReturn = 0;
	m_Character = CHARACTER_PLAYER;
	CNewsCaster::LoadMat();
	CDohyo::LoadMat();

	CNewsBG::Create(D3DXVECTOR3(0.0f, 25.0f, 40.0f), D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f), 100.0f, 180.0f);
	CNewsCaster::Create(D3DXVECTOR3(0.0f, 0.0f, -130.0f));

	CTime::Create(D3DXVECTOR3(100.0f, 55.0f, 0.0f));
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	m_pPlayer = NULL;
	m_pEnemy = NULL;
	CNewsCaster::UnloadMat();
	CDohyo::UnloadMat();

	//全ての終了処理
	CScene::ReleseAll();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CTitle::Update(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	CManager *pManager = NULL;
	CFade *pFade = pManager->GetFade();

	CNumPlayer::MODE mode;
	mode = CNumPlayer::GetMode();

	if (m_state == CTitle::STATE_NEWS && pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		m_state = CTitle::STATE_TITLE;
	}
	else if (m_state == CTitle::STATE_TITLE)
	{
		if (m_bSetDohyo == true)
		{
			CDohyo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CDohyoCircle::Create(D3DXVECTOR3(0, 25.0f, 0), 0.0f, 0.0f);
			CField::Create(D3DXVECTOR3(0, -10.0f, 0), 700.0f);
			CWall::Create(D3DXVECTOR3(0, 200.0f, 500), D3DXVECTOR3(300.0f, 0.0f, 0.0f), 200.0f, 700.0f);
			CWall::Create(D3DXVECTOR3(-550, 200.0f, 0), D3DXVECTOR3(300.0f, 300.0f, 0.0f), 200.0f, 700.0f);
			CWall::Create(D3DXVECTOR3(0, 200.0f, -500), D3DXVECTOR3(300.0f, 600.0f, 0.0f), 200.0f, 700.0f);
			CWall::Create(D3DXVECTOR3(550, 200.0f, 0), D3DXVECTOR3(300.0f, 900.0f, 0.0f), 200.0f, 700.0f);
			CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50.0f, 0.0f), SCREEN_WIDTH / 2, 300.0f, CLoad::TEXTURE_TITLE);
			CNumPlayer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 500.0f, 0.0f));
			if (m_pPlayer == NULL)
			{
				m_pPlayer = CPlayer::Create(D3DXVECTOR3(-30.0f, 50.0f, 0.0f));
			}
			if (m_pEnemy == NULL)
			{
				m_pEnemy = CEnemy::Create(D3DXVECTOR3(30.0f, 50.0f, 0.0f));
			}

			//CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 50, 50, CLogo::TYPE_TEST);
			m_bSetDohyo = false;
		}
		if (CNumPlayer::GetDecision() == true)
		{
			m_state = STATE_CHARASELECT;
		}

	}
	else if (m_state == CTitle::STATE_CHARASELECT)
	{
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
		{
			m_nCntReturn++;
		}

		if (mode == CNumPlayer::MODE_1P)
		{
			if (m_nCntReturn == 1)
			{
				pFade->SetFade(pManager->MODE_GAME, pFade->FADE_OUT);
				m_nCntReturn = 0;
			}
		}
		else if (mode == CNumPlayer::MODE_2P)
		{
			if (m_nCntReturn == 2)
			{
				pFade->SetFade(pManager->MODE_GAME, pFade->FADE_OUT);
				m_nCntReturn = 0;
			}
		}

#ifdef _DEBUG
		if (mode == CNumPlayer::MODE_1P)
		{
			CDebugProc::Print("c", "1P");
		}
		else if (mode == CNumPlayer::MODE_2P)
		{
			CDebugProc::Print("c", "2P");
		}

		if (m_pPlayer->GetSelect() == true)
		{
			CDebugProc::Print("c", "プレイヤー");
		}
		if (m_pEnemy->GetSelect() == true)
		{
			CDebugProc::Print("c", "エネミー");
		}
#endif
	}

	if (m_state == STATE_CHARASELECT && pInputKeyboard->GetTrigger(DIK_RIGHT) == true && m_bTurn == false)
	{
		m_bTurn = true;
	}

#ifdef _DEBUG
	CDebugProc::Print("c", "タイトル");
#endif
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CTitle::Draw(void)
{

}