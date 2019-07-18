//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : 目黒未来也
//
//=============================================================================
#include "result.h"
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
#include "camera.h"
#include "modeSelect.h"

//============================================================================
//	マクロ定義
//============================================================================

//============================================================================
//静的メンバ変数宣言
//============================================================================
CPlayer *CResult::m_pPlayer = NULL;
CEnemy *CResult::m_pEnemy = NULL;

//=============================================================================
//	コンストラクタ
//=============================================================================
CResult::CResult()
{
	m_nCntTurn = 0;
	m_nCntReturn = 0;
	m_nCntDrawTimer = 0;
	m_bDrawSelect = false;
}

//=============================================================================
//デストラクタ
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
void CResult::Init(void)
{
	m_bSetDohyo = true;
	m_bTurn = false;
	m_nCntTurn = 0;
	m_nCntReturn = 0;
	m_bDrawSelect = false;
	CDohyo::LoadMat();

	CDohyo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CDohyoCircle::Create(D3DXVECTOR3(0, 25.0f, 0), 0.0f, 0.0f);
	CField::Create(D3DXVECTOR3(0, -10.0f, 0), 700.0f);
	CWall::Create(D3DXVECTOR3(0, 200.0f, 500), D3DXVECTOR3(300.0f, 0.0f, 0.0f), 200.0f, 700.0f);
	CWall::Create(D3DXVECTOR3(-550, 200.0f, 0), D3DXVECTOR3(300.0f, 300.0f, 0.0f), 200.0f, 700.0f);
	CWall::Create(D3DXVECTOR3(0, 200.0f, -500), D3DXVECTOR3(300.0f, 600.0f, 0.0f), 200.0f, 700.0f);
	CWall::Create(D3DXVECTOR3(550, 200.0f, 0), D3DXVECTOR3(300.0f, 900.0f, 0.0f), 200.0f, 700.0f);

	LoadWinner();

	CManager::GetCamera()->Init();
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CResult::Uninit(void)
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
void CResult::Update(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	CManager *pManager = NULL;
	CFade *pFade = pManager->GetFade();

	CNumPlayer::MODE mode;
	mode = CNumPlayer::GetMode();

	//任意のキー
	//if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	//{
	////	pFade->SetFade(pManager->MODE_TITLE, pFade->FADE_OUT);

	//}

	m_nCntDrawTimer++;
	if (m_nCntDrawTimer > 120)
	{
		if (m_bDrawSelect == false)
		{
			CModeSelect::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), 150);
			m_bDrawSelect = true;
		}
	}


#ifdef _DEBUG
	CDebugProc::Print("c", "リザルト");
#endif
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CResult::Draw(void)
{

}

//=============================================================================
// 勝者の読み込み処理
//=============================================================================
void CResult::LoadWinner(void)
{
	FILE *pFile = NULL;

	pFile = fopen("data\\TEXT\\Winner.txt", "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%d", &m_nWinner);

		fclose(pFile);
	}

	if (m_nWinner == 1)
	{
		if (m_pPlayer == NULL)
		{// プレイヤー
			m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 26.0f, -50.0f), D3DXVECTOR3(0.0f, D3DX_PI * 2.0f, 0.0f));
		}
	}
	else if(m_nWinner == 2)
	{
		if (m_pEnemy == NULL)
		{// エネミー
			m_pEnemy = CEnemy::Create(D3DXVECTOR3(0.0f, 26.0f, -50.0f), D3DXVECTOR3(0.0f,D3DX_PI * 2.0f, 0.0f), CEnemy::MODE_P2);
		}
	}
}
