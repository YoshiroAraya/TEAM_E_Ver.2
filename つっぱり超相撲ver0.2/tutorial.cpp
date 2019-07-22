//=============================================================================
//
// チュートリアル処理 [tutorial.cpp]
// Author : 目黒 未来也
//
//=============================================================================
#include "tutorial.h"
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

#include "number.h"
#include "UITime.h"
#include "pause.h"


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
CScene3D *CTutorial::m_pScene3D = NULL;
CPlayer *CTutorial::m_pPlayer = NULL;
CEnemy *CTutorial::m_pEnemy = NULL;
CShadow *CTutorial::m_pShadow = NULL;
CMeshField *CTutorial::m_pMeshField = NULL;
CBattleSys *CTutorial::m_pBatlteSys = NULL;
CGauge *CTutorial::m_pGauge = NULL;
CUltimateGauge *CTutorial::m_pUltimateGauge = NULL;
CSansoGauge *CTutorial::m_pSansoGauge = NULL;
CUITime *CTutorial::m_pUITime = NULL;

bool CTutorial::m_bHit = false;
CTutorial::STATE CTutorial::m_State = CTutorial::STATE_START;
CTutorial::WINNER CTutorial::m_Winner = CTutorial::WINNER_NONE;

//=============================================================================
//	コンストラクタ
//=============================================================================
CTutorial::CTutorial()
{
	m_n1P = 0;
	m_n2P = 0;
	m_nWin1P = 0;
	m_nWin2P = 0;
	m_WinerNum = 0;
	// 値をクリア
	for (int nCnt = 0; nCnt < MAX_TUTORIAL; nCnt++)
	{
		m_apScene2D[nCnt] = NULL;
	}

	// 値をクリア
	for (int nCnt = 0; nCnt < MAX_TUTORIAL; nCnt++)
	{
		m_apScene3D[nCnt] = NULL;
	}
}

//=============================================================================
//デストラクタ
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
void CTutorial::Init(void)
{
	m_bHit = false;
	m_bUI = true;
	m_State = STATE_GAME;
	m_Winner = WINNER_NONE;
	//インスタンス
	CManager *pManager = NULL;

	LoadChara();

	CDohyo::LoadMat();
	CDohyo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_pGauge = CGauge::Create(D3DXVECTOR3(100, 100, 0));
	//CWinnerUI::Create(D3DXVECTOR3(300, 50, 0));
	m_pUltimateGauge = CUltimateGauge::Create(D3DXVECTOR3(100, 150, 0));
	m_pSansoGauge = CSansoGauge::Create(D3DXVECTOR3(100,680, 0));

	if (m_pScene3D == NULL)
	{
		//m_pScene3D = CScene3D::Create(D3DXVECTOR3(200.0f, 0.0f, 0.0f));
	}

	if (m_pPlayer == NULL)
	{// プレイヤー
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(-80.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	}

	if (m_pEnemy == NULL)
	{// エネミー
		m_pEnemy = CEnemy::Create(D3DXVECTOR3(80.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), CEnemy::MODE_CPU);
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

	//CTouzai::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

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

	//m_apScene2D[0] = new CScene2D(4);
	//m_apScene2D[0]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_TUTORIAL00));
	//m_apScene2D[0]->Init(D3DXVECTOR3(240, 100, 0));
	//m_apScene2D[0]->SetWidthHeight(250.0f, 200.0f);
	//m_apScene2D[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//m_apScene2D[1] = new CScene2D(4);
	//m_apScene2D[1]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_TUTORIAL01));
	//m_apScene2D[1]->Init(D3DXVECTOR3(240 + 400.0f, 100, 0.0f));
	//m_apScene2D[1]->SetWidthHeight(250.0f, 200.0f);
	//m_apScene2D[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//m_apScene2D[2] = new CScene2D(4);
	//m_apScene2D[2]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_TUTORIAL00));
	//m_apScene2D[2]->Init(D3DXVECTOR3(220 + 800.0f, 150, 0.0f));
	//m_apScene2D[2]->SetWidthHeight(350.0f, 200.0f);
	//m_apScene2D[2]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));


	m_apScene3D[0] = CScene3D::Create(D3DXVECTOR3(0, 50, 150));
	m_apScene3D[0]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_TUTORIAL00));
	m_apScene3D[0]->SetSize(100, 150);
	m_apScene3D[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apScene3D[0]->Init(D3DXVECTOR3(0, 130, 200));
	m_apScene3D[0]->SetRot(D3DXVECTOR3(-1.3f, 0, 0));

}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CTutorial::Uninit(void)
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

	// 2Dオブジェクト終了処理
	for (int nCnt = 0; nCnt < MAX_TUTORIAL; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{
			m_apScene2D[nCnt]->Uninit();
			m_apScene2D[nCnt] = NULL;
		}
	}

	// 2Dオブジェクト終了処理
	for (int nCnt = 0; nCnt < MAX_TUTORIAL; nCnt++)
	{
		if (m_apScene3D[nCnt] != NULL)
		{
			m_apScene3D[nCnt]->Uninit();
			m_apScene3D[nCnt] = NULL;
		}
	}

	//全ての終了処理
	CScene::ReleseAll();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CTutorial::Update(void)
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
		pFade->SetFade(pManager->MODE_TUTORIAL, pFade->FADE_OUT);

	}
	if (pInputKeyboard->GetTrigger(DIK_9) == true)
	{
		pFade->SetFade(pManager->MODE_ULTIMATE, pFade->FADE_OUT);
	}

	if (m_pBatlteSys != NULL)
	{
		m_pBatlteSys->Update();
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
		m_Winner = WINNER_NONE;
		m_pBatlteSys->ResetBattle();
	//	m_pUITime->SetTime(TIME_INI);
	}
	else if (m_Winner == WINNER_PLAYER2)
	{
		m_Winner = WINNER_NONE;
		m_pBatlteSys->ResetBattle();
	//	m_pUITime->SetTime(TIME_INI);
	}

	if (m_pPlayer->GetState() == CPlayer::STATE_JANKEN)
	{
		m_apScene3D[0]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_TUTORIAL00));
	}
	else if (m_pPlayer->GetState() == CPlayer::STATE_NEUTRAL)
	{
		m_apScene3D[0]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_TUTORIAL01));
	}
	else if (m_pPlayer->GetState() == CPlayer::STATE_KUMI)
	{
		m_apScene3D[0]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_TUTORIAL02));
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

		for (int nCnt = 0; nCnt < 1; nCnt++)
		{
			CEffect2D::Create(D3DXVECTOR3(300.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1, 1, 1, 0.5f),
			100, 100, 200, CLoad::TEXTURE_EFFECT_NORMAL001);
			//お金
			moveRand.x = sinf((rand() % 628) / 100.0f) * ((rand() % 1 + 1));
			moveRand.y = cosf((rand() % 628) / 20.0f) * ((rand() % 8 + 5));
			moveRand.z = cosf((rand() % 628) / 100.0f) * ((rand() % 1 + 1));
			//moveRand.x = rand() % 2 - 2;

			//CEffect3D::Create(D3DXVECTOR3(0.0f, 400.0f, 0.0f), D3DXVECTOR3(moveRand.x, moveRand.y, moveRand.z), D3DXCOLOR(1, 1, 1, 1),
				//20, 20, 1, 200, CLoad::TEXTURE_EFFECT_NORMAL001);

			//煙(エラー)
			//CEffect::Create(D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(moveRand.x, 0.5f, moveRand.z), D3DXCOLOR(1, 1, 1, 1),
				//10, 10, 1, 60, CLoad::TEXTURE_EFFECT_NORMAL002);

			////塩
			//moveRand.x = sinf((rand() % 628) / 100.0f) * ((rand() % 3 + 1));
			//moveRand.y = cosf((rand() % 628) / 20.0f) * ((rand() % 6 + 3));
			//moveRand.z = (float)((rand() % 7 + 3));
			////moveRand.x = rand() % 2 - 2;
			//PosRand.x = (float)(rand() % 300 - 300);

			//CEffect3D::Create(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(moveRand.x, moveRand.y, -moveRand.z), D3DXCOLOR(1, 1, 1, 1),
			//	6, 6, 1, 200, CLoad::TEXTURE_EFFECT_NORMAL000);
		}
	}
#endif
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CTutorial::Draw(void)
{

}

//=============================================================================
// 3Dポリゴンの取得
//=============================================================================
CScene3D *CTutorial::GetScene3D(void)
{
	return m_pScene3D;
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
CPlayer *CTutorial::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
CEnemy * CTutorial::GetEnemy(void)
{
	return m_pEnemy;
}

//=============================================================================
// 影の取得
//=============================================================================
CShadow *CTutorial::GetShadow(void)
{
	return m_pShadow;
}

//=============================================================================
// メッシュフィールドの取得
//=============================================================================
CMeshField *CTutorial::GetMeshField(void)
{
	return m_pMeshField;
}

//=============================================================================
// ゲージの取得
//=============================================================================
CGauge *CTutorial::GetGauge(void)
{
	return m_pGauge;
}

//=============================================================================
// 時間切れ
//=============================================================================
void CTutorial::TimeOver(void)
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
void CTutorial::SaveWinner(void)
{
	FILE *pFileW;

	// ファイルを開く
	pFileW = fopen("data\\TEXT\\Winner.txt", "w");

	if (pFileW != NULL)
	{// ファイルが開けたら
		//モデルの総数
		fprintf(pFileW, "%d\n", m_WinerNum);
		//ファイルを閉じる
		fclose(pFileW);
	}
	else
	{// ファイルが開けなかったら
		printf("開けませんでした\n");
	}
}

//=============================================================================
// ブロックとの当たり判定処理
//=============================================================================
bool CTutorial::Collision(D3DXVECTOR3 *pos0, float fRadius0, D3DXVECTOR3 *pos1, float fRadius1)
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
void CTutorial::LoadChara(void)
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

