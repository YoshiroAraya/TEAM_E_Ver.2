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
	m_bTurnRight = false;
	m_bTurnLeft = false;
	m_nCntTurn = 0;
	m_nCntReturn = 0;
	m_Character[0] = CHARACTER_PLAYER;
	m_Character[1] = CHARACTER_PLAYER;
	CNewsCaster::LoadMat();
	CDohyo::LoadMat();

	CNewsBG::Create(D3DXVECTOR3(0.0f, 25.0f, 40.0f), D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f), 100.0f, 180.0f);
	CNewsCaster::Create(D3DXVECTOR3(0.0f, -20.0f, -50.0f));

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
	{// ニュースの画面からタイトル画面へ
		m_state = CTitle::STATE_TITLE;
	}
	else if (m_state == CTitle::STATE_TITLE)
	{// タイトル画面
		if (m_bSetDohyo == true)
		{
			// タイトルで使うオブジェクトを設置
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
				m_pPlayer = CPlayer::Create(D3DXVECTOR3(-30.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
			}
			if (m_pEnemy == NULL)
			{
				m_pEnemy = CEnemy::Create(D3DXVECTOR3(30.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
			}

			//CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 50, 50, CLogo::TYPE_TEST);
			m_bSetDohyo = false;
		}
		if (CNumPlayer::GetDecision() == true)
		{// 人数選択したらキャラクター選択へ
			m_state = STATE_CHARASELECT;
		}

	}
	else if (m_state == CTitle::STATE_CHARASELECT)
	{// キャラクター選択

		if (m_bSetDohyo == true)
		{
			// タイトルで使うオブジェクトを設置
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
				m_pPlayer = CPlayer::Create(D3DXVECTOR3(-30.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
			}
			if (m_pEnemy == NULL)
			{
				m_pEnemy = CEnemy::Create(D3DXVECTOR3(30.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
			}

			//CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 50, 50, CLogo::TYPE_TEST);
			m_bSetDohyo = false;
		}


		if (m_state == STATE_CHARASELECT && m_bTurnRight == false && m_bTurnLeft == false)
		{
			if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
			{// 右に回転
				m_bTurnRight = true;
			}
			else if(pInputKeyboard->GetTrigger(DIK_LEFT) == true)
			{// 左に回転
				m_bTurnLeft = true;
			}
		}

		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
		{
			if (m_pPlayer->GetSelect() == true || m_pEnemy->GetSelect() == true)
			{// エンターを押した回数をカウント
				m_nCntReturn++;
			}
		}

		if (mode == CNumPlayer::MODE_1P)
		{// 1Pモードの時
			if (m_nCntReturn == 1)
			{
				if (m_pPlayer->GetSelect() == true && pInputKeyboard->GetTrigger(DIK_RETURN) == true)
				{// プレイヤーを選択
					m_Character[0] = CHARACTER_PLAYER;
					m_pPlayer->SetSelect(false);
				}
				else if (m_pEnemy->GetSelect() == true && pInputKeyboard->GetTrigger(DIK_RETURN) == true)
				{// エネミーを選択
					m_Character[0] = CHARACTER_ENEMY;
					m_pEnemy->SetSelect(false);
				}

				// 敵のキャラ選択
				m_Character[1] = CHARACTER_ENEMY;

				pFade->SetFade(pManager->MODE_GAME, pFade->FADE_OUT);
				m_nCntReturn = 0;

				// 選んだキャラを保存
				SaveCharacter();
			}
		}
		else if (mode == CNumPlayer::MODE_2P)
		{// 2Pモードの時
			if (m_nCntReturn == 1)
			{// 1P選択
				if (m_pPlayer->GetSelect() == true && pInputKeyboard->GetTrigger(DIK_RETURN) == true)
				{// プレイヤーを選択
					m_Character[0] = CHARACTER_PLAYER;

				}
				else if (m_pEnemy->GetSelect() == true && pInputKeyboard->GetTrigger(DIK_RETURN) == true)
				{// エネミーを選択
					m_Character[0] = CHARACTER_ENEMY;
				}
			}
			else if (m_nCntReturn == 2)
			{// 2P選択
				if (m_pPlayer->GetSelect() == true && pInputKeyboard->GetTrigger(DIK_RETURN) == true)
				{// プレイヤーを選択
					m_Character[1] = CHARACTER_PLAYER;
					m_pPlayer->SetSelect(false);
				}
				else if (m_pEnemy->GetSelect() == true && pInputKeyboard->GetTrigger(DIK_RETURN) == true)
				{// エネミーを選択
					m_Character[1] = CHARACTER_ENEMY;
				}
				pFade->SetFade(pManager->MODE_GAME, pFade->FADE_OUT);
				m_nCntReturn = 0;

				// 選んだキャラを保存
				SaveCharacter();
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
			CDebugProc::Print("c", "プレイヤー：true");
		}
		else if (m_pPlayer->GetSelect() == false)
		{
			CDebugProc::Print("c", "プレイヤー：false");
		}

		if (m_pEnemy->GetSelect() == true)
		{
			CDebugProc::Print("c", "エネミー：true");
		}
		else if (m_pEnemy->GetSelect() == false)
		{
			CDebugProc::Print("c", "エネミー：false");
		}

		CDebugProc::Print("cn", "m_Character[0] = ", m_Character[0]);
		CDebugProc::Print("cn", "m_Character[1] = ", m_Character[1]);
#endif
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

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CTitle::SaveCharacter(void)
{
	FILE *pFile;

	// ファイルを開く
	pFile = fopen("data\\TEXT\\charaSave.txt", "w");

	if (pFile != NULL)
	{// ファイルが開けたら

		// モデルの総数
		fprintf(pFile, "%d\n", m_Character[0]);
		fprintf(pFile, "%d\n", m_Character[1]);

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開けなかったら
		printf("開けませんでした\n");
	}
}