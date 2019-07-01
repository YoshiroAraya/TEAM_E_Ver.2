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
#include "load.h"
//--------------------------------------------
//静的メンバ変数宣言
//--------------------------------------------


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
	{
		m_pScene2D[nCnt1] = CScene2D::Create(D3DXVECTOR3(400.0f - (nCnt1 * 100.0f), pos.y, 0.0f));
		m_pScene2D[nCnt1]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_WINNER_UI));
		m_pScene2D[nCnt1]->SetWidthHeight(50.0f, 50.0f);
		m_pScene2D[nCnt1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//2p側
	for (int nCnt2 = 0; nCnt2 < 3; nCnt2++)
	{
		m_pScene2D[nCnt2 + 3] = CScene2D::Create(D3DXVECTOR3(850.0f + (nCnt2 * 100.0f), pos.y, 0.0f));
		m_pScene2D[nCnt2 + 3]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_WINNER_UI));
		m_pScene2D[nCnt2 + 3]->SetWidthHeight(50.0f, 50.0f);
		m_pScene2D[nCnt2 + 3]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

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
		m_bWinner = true;
		if (m_bWinner == true)
		{
			m_n1player += 1;
			m_bWinner = false;
		}

		if (m_n1player == 1)
		{
			m_pScene2D[0]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else if (m_n1player == 2)
		{
			m_pScene2D[1]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else if (m_n1player == 3)
		{
			m_pScene2D[2]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}


	if (nWinner == 2)
	{//2p側

		m_bWinner = true;
		if (m_bWinner == true)
		{
			m_n2player += 1;
			m_bWinner = false;

		}
		if (m_n2player == 1)
		{
			m_pScene2D[3]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else if (m_n2player == 2)
		{
			m_pScene2D[4]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		}
		else if (m_n2player == 3)
		{
			m_pScene2D[5]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

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