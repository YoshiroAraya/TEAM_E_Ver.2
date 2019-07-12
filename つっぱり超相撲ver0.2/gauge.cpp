//=============================================================================
//
// ゲージ処理 [gauge.h]
// Author : 目黒未来也
//
//=============================================================================
#include "scene.h"
#include "gauge.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "tutorial.h"
//--------------------------------------------
//静的メンバ変数宣言
//--------------------------------------------


//=============================================================================
// シーンクラスのコンストラクタ
//=============================================================================
CGauge::CGauge(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
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
CGauge *CGauge::Create(D3DXVECTOR3 pos)
{
	CGauge *pGauge = NULL;

	if (pGauge == NULL)
	{
		// オブジェクトクラスの生成
		pGauge = new CGauge;

		if (pGauge != NULL)
		{
			pGauge->Init(pos);
		}
	}

	return pGauge;
}

//=============================================================================
// デストラクタ
//=============================================================================
CGauge::~CGauge()
{
}

//=============================================================================
// ゲージの初期化処理
//=============================================================================
HRESULT CGauge::Init(D3DXVECTOR3 pos)
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
	m_pScene2D[0] = CScene2D::Create(D3DXVECTOR3(300.0f, pos.y, 0.0f));
	m_pScene2D[0]->SetWidthHeight(300.0f, 30.0f);
	m_pScene2D[0]->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	m_pScene2D[0]->BindTexture(NULL);
	m_bUlt[0] = false;

	//2p側
	m_pScene2D[1] = CScene2D::Create(D3DXVECTOR3(980.0f, pos.y, 0.0f));
	m_pScene2D[1]->SetWidthHeight(300.0f, 30.0f);
	m_pScene2D[1]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pScene2D[1]->BindTexture(NULL);
	m_bUlt[1] = false;

	return S_OK;
}

//=============================================================================
// ゲージの終了処理
//=============================================================================
void CGauge::Uninit(void)
{

	// オブジェクトの解放
	Release();
}

//=============================================================================
// ゲージの更新処理
//=============================================================================
void CGauge::Update(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

#ifdef _DEBUG

#if 1
	if (pInputKeyboard->GetPress(DIK_1) == true)
	{
		m_fLeft += 10.0f;
	}
	if (pInputKeyboard->GetPress(DIK_2) == true)
	{
		m_fLeft -= 10.0f;
	}

	if (pInputKeyboard->GetPress(DIK_3) == true)
	{
		m_fRight -= 10.0f;
	}
	if (pInputKeyboard->GetPress(DIK_4) == true)
	{
		m_fRight += 10.0f;
	}

	//ゲージの最大と最小
	if (m_fLeft > 0)
	{
		m_fLeft = 0;
	}
	else if (m_fLeft < -600)
	{
		m_fLeft = -600;
	}

	if (m_fRight > 0)
	{
		m_fRight = 0;
	}
	else if (m_fRight < -600)
	{
		m_fRight = -600;
	}

	//ゲージの左右を調整
	m_pScene2D[0]->SetRIghtLeft(0.0f, m_fLeft);
	m_pScene2D[1]->SetRIghtLeft(m_fRight, 0.0f);

#endif
	//CDebugProc::Print("cf", "m_fRight    : ", m_fRight);
	//CDebugProc::Print("cf", "m_fLeft    : ", m_fLeft);
#endif
}

//=============================================================================
// ゲージの描画処理
//=============================================================================
void CGauge::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();


}

//=============================================================================
// オブジェクトの取得
//=============================================================================
D3DXVECTOR3 CGauge::GetPosition(void)
{
	return m_Pos;
}

//=============================================================================
// オブジェクトの設定
//=============================================================================
void CGauge::SetPosition(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
// 左右
//=============================================================================
void CGauge::SetGaugeRightLeft(float fRight, float fLeft)
{
	// プレイヤーの取得
	CPlayer *pPlayer = NULL;
	// エネミーの取得
	CEnemy *pEnemy = NULL;

	CManager::MODE mode;
	mode = CManager::GetMode();
	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();
	}

	m_fRight += fRight;
	m_fLeft += fLeft;
	//上限を超えないように
	if (m_fLeft > 0)
	{
		m_fLeft = 0;
	}
	else if (m_fLeft > -590)
	{
		pPlayer->SetDying(false);
	}
	else if (m_fLeft <= -590)
	{
		pPlayer->SetDying(true);
	}
	else if (m_fLeft < -600)
	{
		m_fLeft = -600;
	}

	if (m_fRight > 0)
	{
		m_fRight = 0;
	}
	else if (m_fRight > -590)
	{
		pEnemy->SetDying(false);
	}
	else if (m_fRight <= -590)
	{
		pEnemy->SetDying(true);
	}
	else if (m_fRight < -600)
	{
		m_fRight = -600;
	}

	//ゲージの左右を調整
	m_pScene2D[0]->SetRIghtLeft(0.0f, m_fLeft);
	m_pScene2D[1]->SetRIghtLeft(m_fRight, 0.0f);
}
