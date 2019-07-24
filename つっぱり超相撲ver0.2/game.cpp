//=============================================================================
//
// メイン処理 [game.cpp]
// Author :
//
//=============================================================================
#include "game.h"
#include "manager.h"
#include "light.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "CPU.h"
#include "scene3D.h"
#include "game.h"
#include "meshField.h"
#include "dohyo.h"
#include "shadow.h"
#include "debugProc.h"
#include "scene.h"
#include "fade.h"
#include "input.h"
#include "BattleSystem.h"
#include "customer.h"
#include "dohyocircle.h"
#include <time.h>
#include "gauge.h"
#include "SansoGauge.h"
#include "UltimateGauge.h"
#include "effect.h"
#include "jankenUI.h"
#include "field.h"
#include "wall.h"
#include "touzai.h"
#include "animation.h"
#include "load.h"
#include "effect3D.h"
#include "winnerUI.h"
#include "effect2D.h"
#include "2Danimation.h"
#include "ougiUI.h"

#include "number.h"
#include "UITime.h"
#include "pause.h"
#include "numPlayer.h"

//============================================================================
//	マクロ定義
//============================================================================
#define SIZE_X (SCREEN_WIDTH)
#define SIZE_Y (SCREEN_HEIGHT)
#define COLISIONSIZE (20.0f)
#define TIME_INI		(60)
//============================================================================
//静的メンバ変数宣言
//============================================================================
CScene3D *CGame::m_pScene3D = NULL;
CPlayer *CGame::m_pPlayer = NULL;
CEnemy *CGame::m_pEnemy = NULL;
CShadow *CGame::m_pShadow = NULL;
CMeshField *CGame::m_pMeshField = NULL;
CBattleSys *CGame::m_pBatlteSys = NULL;
CGauge *CGame::m_pGauge = NULL;
CUltimateGauge *CGame::m_pUltimateGauge = NULL;
CSansoGauge *CGame::m_pSansoGauge = NULL;
CUITime *CGame::m_pUITime = NULL;

bool CGame::m_bHit = false;
CGame::STATE CGame::m_State = CGame::STATE_START;
CGame::WINNER CGame::m_Winner = CGame::WINNER_NONE;

//=============================================================================
//	コンストラクタ
//=============================================================================
CGame::CGame()
{
	m_n1P = 0;
	m_n2P = 0;
	m_nWin1P = 0;
	m_nWin2P = 0;
	m_WinerNum = 0;
	m_nBattleResetTimer = 0;
}

//=============================================================================
//デストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
void CGame::Init(void)
{
	m_bHit = false;
	m_bUI = true;
	m_State = STATE_START;
	m_Winner = WINNER_NONE;
	//インスタンス
	CManager *pManager = NULL;

	LoadChara();

	CDohyo::LoadMat();
	CDohyo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_pGauge = CGauge::Create(D3DXVECTOR3(100, 100, 0));
	CWinnerUI::Create(D3DXVECTOR3(300, 50, 0));
	m_pUltimateGauge = CUltimateGauge::Create(D3DXVECTOR3(100, 150, 0));
	m_pSansoGauge = CSansoGauge::Create(D3DXVECTOR3(100, 680, 0));

	int nCntZ;
	int nCnt;

	for (nCntZ = 0; nCntZ < 2; nCntZ++)
	{//奥客
		for (nCnt = 0; nCnt < 3; nCnt++)
		{//客

			CCustomer::Create(D3DXVECTOR3(-140.0f + (nCnt * 160.0f), 1.0f, 270.0f + (nCntZ * 160.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3, CCustomer::POSITION);
		}
	}

	for (nCntZ = 0; nCntZ < 2; nCntZ++)
	{//右客
		for (nCnt = 0; nCnt < 3; nCnt++)
		{//客

			CCustomer::Create(D3DXVECTOR3(270.0f + (nCnt * 150.0f), 1.0f, -80.0f + (nCntZ * 160.0f)), D3DXVECTOR3(0.0f, -300.0f, 0.0f), 3, CCustomer::POSITION_RIGHT);
		}
	}

	for (nCntZ = 0; nCntZ < 2; nCntZ++)
	{//左客
		for (nCnt = 0; nCnt < 3; nCnt++)
		{//客

			CCustomer::Create(D3DXVECTOR3(-580.0f + (nCnt * 150.0f), 1.0f, -80.0f + (nCntZ * 160.0f)), D3DXVECTOR3(0.0f, 300.0f, 0.0f), 3, CCustomer::POSITION);
		}
	}

	if (m_pScene3D == NULL)
	{
		//m_pScene3D = CScene3D::Create(D3DXVECTOR3(200.0f, 0.0f, 0.0f));
	}

	if (m_pPlayer == NULL)
	{// プレイヤー
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(-170.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	}

	if (m_pEnemy == NULL)
	{// エネミー
		if (CNumPlayer::GetMode() == CNumPlayer::MODE_1P)
		{
			m_pEnemy = CEnemy::Create(D3DXVECTOR3(170.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), CEnemy::MODE_CPU);
		}
		else if (CNumPlayer::GetMode() == CNumPlayer::MODE_2P)
		{
			m_pEnemy = CEnemy::Create(D3DXVECTOR3(170.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), CEnemy::MODE_P2);
		}
	}

	if (m_pMeshField == NULL)
	{
		// メッシュフィールドの生成
		//	m_pMeshField = CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	CDohyoCircle::Create(D3DXVECTOR3(0, 25.0f, 0), 0.0f, 0.0f);

	CField::Create(D3DXVECTOR3(0, -10.0f, 0), 700.0f);

	CWall::Create(D3DXVECTOR3(0, 200.0f, 500), D3DXVECTOR3(300.0f, 0.0f, 0.0f), 200.0f, 700.0f);
	CWall::Create(D3DXVECTOR3(-550, 200.0f, 0), D3DXVECTOR3(300.0f, 300.0f, 0.0f), 200.0f, 700.0f);

	CWall::Create(D3DXVECTOR3(0, 200.0f, -500), D3DXVECTOR3(300.0f, 600.0f, 0.0f), 200.0f, 700.0f);
	CWall::Create(D3DXVECTOR3(550, 200.0f, 0), D3DXVECTOR3(300.0f, 900.0f, 0.0f), 200.0f, 700.0f);

	// 2Dポリゴンの生成
	//CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH -50.0f, 50.0f, 0.0f));

	// 3Dポリゴンの生成
	//CScene3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 3Dモデル
	//CSceneX::Create(D3DXVECTOR3(0.0f, 25.0f, 0.0f));

	if (m_pShadow == NULL)
	{
		m_pShadow = CShadow::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (m_pBatlteSys == NULL)
	{
		m_pBatlteSys = CBattleSys::Create();
	}

	CTouzai::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != NULL)
	{
		pCamera->Init();
	}

	//制限時間
	m_pUITime = CUITime::Create(D3DXVECTOR3(680, 70, 0), 70, 70);
	//ポーズ生成
	CPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), 150);

	m_nTime = 0;
	m_nWin1P = 0;
	m_nWin2P = 0;
	m_WinerNum = 0;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CGame::Uninit(void)
{
	CDohyo::UnloadMat();
	m_pPlayer = NULL;
	m_pEnemy = NULL;
	m_pMeshField = NULL;
	m_pShadow = NULL;
	m_pGauge = NULL;

	if (m_pBatlteSys != NULL)
	{
		//メモリの開放
		delete m_pBatlteSys;
		m_pBatlteSys = NULL;
	}


	//全ての終了処理
	CScene::ReleseAll();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CGame::Update(void)
{
	CManager *pManager = NULL;
	CFade *pFade = pManager->GetFade();

	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	// ランダムな値を更新
	srand((unsigned int)time(NULL));


	if (m_pPlayer != NULL && m_pEnemy != NULL)
	{
		m_bHit = Collision(&m_pPlayer->GetPosition(), COLISIONSIZE, &m_pEnemy->GetPosition(), COLISIONSIZE);
	}

	CDebugProc::Print("c", "ゲームモード");

	m_nTime++;

	//任意のキー←
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
	//	pFade->SetFade(pManager->MODE_RESULT, pFade->FADE_OUT);
	}
	if (pInputKeyboard->GetTrigger(DIK_BACKSPACE) == true)
	{
		pFade->SetFade(pManager->MODE_GAME, pFade->FADE_OUT);

	}
	if (pInputKeyboard->GetTrigger(DIK_9) == true)
	{
		pFade->SetFade(pManager->MODE_ULTIMATE, pFade->FADE_OUT);
	}

	if (CScene::GetbPause() == false)
	{
		if (m_pBatlteSys != NULL)
		{
			m_pBatlteSys->Update();
		}
	}

	if (m_pUITime != NULL)
	{
	//	m_pUITime->Update();
	}

	if (m_State == STATE_GAME)
	{
		if (m_bUI == true)
		{// じゃんけんのUIを出す
			CJankenUI::Create(D3DXVECTOR3(200.0f, 500.0f, 0.0f));
			m_bUI = false;
			m_pPlayer->SetbJanken(true);
			m_pEnemy->SetbJanken(true);
		}
	}

	//勝敗決定
	if (m_Winner == WINNER_PLAYER1)
	{
		m_nBattleResetTimer++;
		if (m_nBattleResetTimer > 120)
		{
			m_nWin1P++;
			m_Winner = WINNER_NONE;
			if (m_nWin1P != 3)
			{
				m_pBatlteSys->ResetBattle();
				m_pUITime->SetTime(TIME_INI);
			}
			//タイマー初期化
			m_nBattleResetTimer = 0;
		}
	}
	else if (m_Winner == WINNER_PLAYER2)
	{
		m_nBattleResetTimer++;
		if (m_nBattleResetTimer > 120)
		{
			m_nWin2P++;
			m_Winner = WINNER_NONE;
			if (m_nWin2P != 3)
			{
				m_pBatlteSys->ResetBattle();
				m_pUITime->SetTime(TIME_INI);
			}
			//タイマー初期化
			m_nBattleResetTimer = 0;
		}

	}

	//勝ちの数が3回目
	if (m_nWin1P == 3 || m_nWin2P == 3)
	{
		if (m_nWin1P == 3)
		{
			m_WinerNum = 1;
		}
		else if (m_nWin2P == 3)
		{
			m_WinerNum = 2;
		}
		SaveWinner();
		pFade->SetFade(pManager->MODE_RESULT, pFade->FADE_OUT);
	}
#ifdef _DEBUG

	if (pInputKeyboard->GetTrigger(DIK_7) == true)
	{
		m_Winner = WINNER_PLAYER1;
	}
	if (pInputKeyboard->GetTrigger(DIK_8) == true)
	{
		m_Winner = WINNER_PLAYER2;
	}

	if (m_bHit == true)
	{
		CDebugProc::Print("c", "当たっている");
	}
	else
	{
		CDebugProc::Print("c", "当たっていない");
	}


	CDebugProc::Print("cn", " プレイヤーの勝ち数  : ", m_nWin1P);
	CDebugProc::Print("cn", " エネミーの勝ち数  : ", m_nWin2P);

	//エフェクト用関数
	D3DXVECTOR3 moveRand;
	D3DXVECTOR3 PosRand;
	D3DXVECTOR3 effectmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//任意のキー←
	if (pInputKeyboard->GetTrigger(DIK_J) == true)
	{
		//アニメーションテクスチャの生成
		//(Pos / Rot / Col / Height / Width / UV_U / UV_V / アニメーションスピード / アニメーションの数 /
		//ループするかしないか(0:する/ 1:しない) / 加算合成するかしないか(0:する/ 1:しない))

		//	CAnimation::Create(D3DXVECTOR3(0, 100.0f, 0), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		//	100.0f, 100.0f, 0.1666666666666667f, 1.0f, 3, 5, 1, 0);

		////オーラ
		//CAnimation::Create(D3DXVECTOR3(0, 100, 0), D3DXVECTOR3(300, 0, 0), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f),
		//	50.0f, 50.0f, 0.0625f, 1.0f, 1.5f, 16, 0, 0);


		//アニメーションテクスチャの生成
		//(Pos / Col / Height / Width / UV_U / UV_V / アニメーションスピード / アニメーションの数 /
		//ループするかしないか(0:する/ 1:しない) / 加算合成するかしないか(0:する/ 1:しない))
		//奥義アニメーション
		C2DAnimation::Create(D3DXVECTOR3(200, 300, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			300.0f, 0.0333333333333333f, 1.0f, 1, 30, 0, 1);

		COugiUI::Create(D3DXVECTOR3(200, 300, 0));

		for (int nCnt = 0; nCnt < 1; nCnt++)
		{
			//CEffect2D::Create(D3DXVECTOR3(300.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1, 1, 1, 0.5f),
			//100, 100, 200, CLoad::TEXTURE_EFFECT_NORMAL001);
			////お金
			//moveRand.x = sinf((rand() % 628) / 100.0f) * ((rand() % 1 + 1));
			//moveRand.y = cosf((rand() % 628) / 20.0f) * ((rand() % 8 + 5));
			//moveRand.z = cosf((rand() % 628) / 100.0f) * ((rand() % 1 + 1));
			//moveRand.x = rand() % 2 - 2;

			//CEffect3D::Create(D3DXVECTOR3(0.0f, 400.0f, 0.0f), D3DXVECTOR3(moveRand.x, moveRand.y, moveRand.z), D3DXCOLOR(1, 1, 1, 1),
				//20, 20, 1, 200, CLoad::TEXTURE_EFFECT_NORMAL001);

			//煙(エラー)
			//CEffect::Create(D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(moveRand.x, 0.5f, moveRand.z), D3DXCOLOR(1, 1, 1, 1),
				//10, 10, 1, 60, CLoad::TEXTURE_EFFECT_NORMAL002);


		}
	}
#endif
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// 3Dポリゴンの取得
//=============================================================================
CScene3D *CGame::GetScene3D(void)
{
	return m_pScene3D;
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
CEnemy * CGame::GetEnemy(void)
{
	return m_pEnemy;
}

//=============================================================================
// 影の取得
//=============================================================================
CShadow *CGame::GetShadow(void)
{
	return m_pShadow;
}

//=============================================================================
// メッシュフィールドの取得
//=============================================================================
CMeshField *CGame::GetMeshField(void)
{
	return m_pMeshField;
}

//=============================================================================
// ゲージの取得
//=============================================================================
CGauge *CGame::GetGauge(void)
{
	return m_pGauge;
}

//=============================================================================
// 時間切れ
//=============================================================================
void CGame::TimeOver(void)
{
	float Life1P, Life2P;	// 左右の値

	Life1P = m_pGauge->GetGaugeRight();
	Life2P = m_pGauge->GetGaugeLeft();

	if (Life1P < Life2P)
	{
		m_Winner = WINNER_PLAYER1;
	}
	else if(Life2P < Life1P)
	{
		m_Winner = WINNER_PLAYER2;
	}
}

//=============================================================================
// 勝者をセーブ
//=============================================================================
void CGame::SaveWinner(void)
{
	FILE *pFileW;

	// ファイルを開く
	pFileW = fopen("data\\TEXT\\Winner.txt", "w");

	if (pFileW != NULL)
	{// ファイルが開けたら
		//勝者のNo
		fprintf(pFileW, "%d\n", m_WinerNum);
		if (m_WinerNum == 1)
		{	//どのキャラか
			fprintf(pFileW, "%d\n", m_n1P);
		}
		else
		{	//どのキャラか
			fprintf(pFileW, "%d\n", m_n2P);
		}

		//ファイルを閉じる
		fclose(pFileW);
	}
	else
	{// ファイルが開けなかったら
		MessageBox(0, "NULLでした", "警告", MB_OK);
	}
}

//=============================================================================
// ブロックとの当たり判定処理
//=============================================================================
bool CGame::Collision(D3DXVECTOR3 *pos0, float fRadius0, D3DXVECTOR3 *pos1, float fRadius1)
{
	bool bHit = false;	// 当たっていない状態

	// 中心と中心の差を求める
	D3DXVECTOR3 DiffLength = D3DXVECTOR3(pos0->x - pos1->x, pos0->y - pos1->y, pos0->z - pos1->z);

	// 中心から中心のベクトルの長さを算出
	float fLength = sqrtf((DiffLength.x * DiffLength.x) + (DiffLength.y * DiffLength.y) + (DiffLength.z * DiffLength.z));

	if (fLength < fRadius0 + fRadius1)
	{// 長さが半径の和より小さければ当たっている
		bHit = true;
	}

	return bHit;	// ブロックに当たっているかどうかを返す
}

//=============================================================================
// 使われているモデルのロード
//=============================================================================
void CGame::LoadChara(void)
{
	FILE *pFile = NULL;

	CManager::MODE mode;
	mode = CManager::GetMode();

	int nCharType = 0;

	pFile = fopen("data\\TEXT\\charaSave.txt", "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%d", &m_n1P);
		fscanf(pFile, "%d", &m_n2P);

		fclose(pFile);
	}
}

