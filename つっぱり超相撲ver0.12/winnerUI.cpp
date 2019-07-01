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
	//m_pTexture = NULL;

	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRight = 0.0f;
	m_fLeft = 0.0f;			// 左右の値

}

//=============================================================================
// オブジェクトの生成処理
//=============================================================================
CWinnerUI *CWinnerUI::Create(D3DXVECTOR3 pos,int nType)
{
	CWinnerUI *pWinnerUI = NULL;

	if (pWinnerUI == NULL)
	{
		// オブジェクトクラスの生成
		pWinnerUI = new CWinnerUI;

		if (pWinnerUI != NULL)
		{
			pWinnerUI->m_nType = nType;
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

	// オブジェクトの種類の設定
	//SetObjType(CScene::OBJTYPE_SCENE2D);

	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	//1p側
	for (int nCnt1 = 0; nCnt1 < 2; nCnt1++)
	{
		m_pScene2D[nCnt1] = CScene2D::Create(D3DXVECTOR3(400.0f - (nCnt1 * 100.0f), pos.y, 0.0f));
		m_pScene2D[nCnt1]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_WINNER_UI));
		m_pScene2D[nCnt1]->SetWidthHeight(50.0f, 50.0f);
		m_pScene2D[nCnt1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//2p側
	for (int nCnt2 = 0; nCnt2 < 2; nCnt2++)
	{
		m_pScene2D[nCnt2 + 2] = CScene2D::Create(D3DXVECTOR3(850.0f + (nCnt2 * 100.0f), pos.y, 0.0f));
		m_pScene2D[nCnt2 + 2]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_WINNER_UI));
		m_pScene2D[nCnt2 + 2]->SetWidthHeight(50.0f, 50.0f);
		m_pScene2D[nCnt2 + 2]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
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

	int nWinner = CGame::GetWinner();

	//保存用変数
	int n1player = 0;
	int n2player = 0;

	if (nWinner == 1)
	{//1p側
		n1player += 1;
		if (n1player == 1)
		{
			m_pScene2D[0]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else if (n1player == 2)
		{
			m_pScene2D[1]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			n1player = 0;
		}
	}

	
	if (nWinner == 2)
	{//2p側
		n2player += 1;
		if (n2player == 1)
		{
			m_pScene2D[2]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else if (n2player == 2)
		{
			m_pScene2D[3]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			n2player = 0;
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