//=============================================================================
//
// 勝者UIの処理 [winnerUI.h]
// Author :		山下敦史
//
//=============================================================================
#include "scene.h"
#include "winnerUI.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "input.h"
#include "gauge.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "tutorial.h"
#include "load.h"
//--------------------------------------------
//静的メンバ変数宣言
//--------------------------------------------


//--------------------------------------------
//マクロ
//--------------------------------------------
#define UI_WIDTH		(40.0f)
#define UI_HEIGHT		(40.0f)
#define UI_SPACE		(67.0f)
#define UI_CIRCLE		(7.0f)
#define PLAYER_UI_POS_X	(482.0f)
#define ENEMY_UI_POS_X	(798.0f)

//=============================================================================
// シーンクラスのコンストラクタ
//=============================================================================
CWinnerUI::CWinnerUI() : CScene2D(7, CScene::OBJTYPE_SCENE2D)
{
	// 値をクリア
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_n1player = 0;
	m_n2player = 0;
	m_bWinner = false;
}

//=============================================================================
// オブジェクトの生成処理
//=============================================================================
CWinnerUI *CWinnerUI::Create(D3DXVECTOR3 pos)
{
	CWinnerUI *pWinnerUI = NULL;

	if (pWinnerUI == NULL)
	{
		// オブジェクトクラスの生成
		pWinnerUI = new CWinnerUI;

		if (pWinnerUI != NULL)
		{
			pWinnerUI->Init(pos);
		}
	}

	return pWinnerUI;
}

//=============================================================================
// デストラクタ
//=============================================================================
CWinnerUI::~CWinnerUI()
{
}

//=============================================================================
// ゲージの初期化処理
//=============================================================================
HRESULT CWinnerUI::Init(D3DXVECTOR3 pos)
{
	// ゲージの位置を設定
	m_Pos = pos;

	m_n1player = 0;
	m_n2player = 0;
	m_bWinner = false;
	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	//1p側
	for (int nCnt1 = 0; nCnt1 < 3; nCnt1++)
	{	//サークル
		m_pScene2D[nCnt1] = CScene2D::Create(D3DXVECTOR3(PLAYER_UI_POS_X - (nCnt1 * UI_SPACE), pos.y, 0.0f), 3);
		m_pScene2D[nCnt1]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_WINNER_UI));
		m_pScene2D[nCnt1]->SetWidthHeight(UI_WIDTH + UI_CIRCLE, UI_HEIGHT + UI_CIRCLE);
		m_pScene2D[nCnt1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		//ちゃんこ
		m_pTyanko2D[nCnt1] = CScene2D::Create(D3DXVECTOR3(PLAYER_UI_POS_X - (nCnt1 * UI_SPACE), pos.y, 0.0f), 4);
		m_pTyanko2D[nCnt1]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_TYANKO_UI));
		m_pTyanko2D[nCnt1]->SetWidthHeight(UI_WIDTH, UI_HEIGHT);
		m_pTyanko2D[nCnt1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pTyanko2D[nCnt1]->SetbDraw(false);
	}

	//2p側
	for (int nCnt2 = 0; nCnt2 < 3; nCnt2++)
	{	//サークル
		m_pScene2D[nCnt2 + 3] = CScene2D::Create(D3DXVECTOR3(ENEMY_UI_POS_X + (nCnt2 * UI_SPACE), pos.y, 0.0f), 3);
		m_pScene2D[nCnt2 + 3]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_WINNER_UI));
		m_pScene2D[nCnt2 + 3]->SetWidthHeight(UI_WIDTH + UI_CIRCLE, UI_HEIGHT + UI_CIRCLE);
		m_pScene2D[nCnt2 + 3]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		//ちゃんこ
		m_pTyanko2D[nCnt2 + 3] = CScene2D::Create(D3DXVECTOR3(ENEMY_UI_POS_X + (nCnt2 * UI_SPACE), pos.y, 0.0f), 4);
		m_pTyanko2D[nCnt2 + 3]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_TYANKO_UI));
		m_pTyanko2D[nCnt2 + 3]->SetWidthHeight(UI_WIDTH, UI_HEIGHT);
		m_pTyanko2D[nCnt2 + 3]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pTyanko2D[nCnt2 + 3]->SetbDraw(false);
	}

	m_pSyouhai2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 0, SCREEN_HEIGHT / 2, 0.0f), 4);
	m_pSyouhai2D->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_IPPON));
	m_pSyouhai2D->SetWidthHeight(800, 450);
	m_pSyouhai2D->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pSyouhai2D->SetbDraw(false);

	return S_OK;
}

//=============================================================================
// ゲージの終了処理
//=============================================================================
void CWinnerUI::Uninit(void)
{
	//終了処理
	Release();

}

//=============================================================================
// ゲージの更新処理
//=============================================================================
void CWinnerUI::Update(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//勝利判定
	int nWinner = CGame::GetWinner();


	if (nWinner == 1)
	{//1p側
		if (m_bWinner == true)
		{
			m_n1player += 1;
			m_bWinner = false;
		}

		if (m_n1player == 1)
		{
			m_pScene2D[0]->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
			m_pTyanko2D[0]->SetbDraw(true);
		}
		else if (m_n1player == 2)
		{
			m_pScene2D[1]->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
			m_pTyanko2D[1]->SetbDraw(true);
		}
		else if (m_n1player == 3)
		{
			m_pScene2D[2]->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
			m_pTyanko2D[2]->SetbDraw(true);
		}
	}


	if (nWinner == 2)
	{//2p側
		if (m_bWinner == true)
		{
			m_n2player += 1;
			m_bWinner = false;
		}
		if (m_n2player == 1)
		{
			m_pScene2D[3]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			m_pTyanko2D[3]->SetbDraw(true);
		}
		else if (m_n2player == 2)
		{
			m_pScene2D[4]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			m_pTyanko2D[4]->SetbDraw(true);
		}
		else if (m_n2player == 3)
		{
			m_pScene2D[5]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			m_pTyanko2D[5]->SetbDraw(true);
		}
	}
}

//=============================================================================
// ゲージの描画処理
//=============================================================================
void CWinnerUI::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//CScene2D::Draw();
}

//=============================================================================
// オブジェクトの取得
//=============================================================================
D3DXVECTOR3 CWinnerUI::GetPosition(void)
{
	return m_Pos;
}

//=============================================================================
// オブジェクトの設定
//=============================================================================
void CWinnerUI::SetPosition(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}