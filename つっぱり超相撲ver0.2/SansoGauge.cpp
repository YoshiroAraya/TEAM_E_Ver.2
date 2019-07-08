//=============================================================================
//
// ゲージ処理 [SansoGauge.h]
// Author : 目黒未来也
//
//=============================================================================
#include "scene.h"
#include "SansoGauge.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "game.h"

//--------------------------------------------
//静的メンバ変数宣言
//--------------------------------------------
LPDIRECT3DTEXTURE9			CSansoGauge::m_pTexture = {};


//=============================================================================
// シーンクラスのコンストラクタ
//=============================================================================
CSansoGauge::CSansoGauge(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
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
CSansoGauge *CSansoGauge::Create(D3DXVECTOR3 pos)
{
	CSansoGauge *pSansoGauge = NULL;

	if (pSansoGauge == NULL)
	{
		// オブジェクトクラスの生成
		pSansoGauge = new CSansoGauge;

		if (pSansoGauge != NULL)
		{
			pSansoGauge->Init(pos);
		}
	}
	return pSansoGauge;
}

//=============================================================================
// デストラクタ
//=============================================================================
CSansoGauge::~CSansoGauge()
{
}

//=============================================================================
// ゲージの初期化処理
//=============================================================================
HRESULT CSansoGauge::Init(D3DXVECTOR3 pos)
{
	// ゲージの位置を設定
	m_Pos = pos;

	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	//1p側
	m_pScene2D[0] = CScene2D::Create(D3DXVECTOR3(285.0f, pos.y, 0.0f));
	m_pScene2D[0]->SetWidthHeight(280.0f, 35.0f);
	m_pScene2D[0]->SetCol(D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f));
	m_pScene2D[0]->BindTexture(NULL);

	//2p側
	m_pScene2D[1] = CScene2D::Create(D3DXVECTOR3(995.0f, pos.y, 0.0f));
	m_pScene2D[1]->SetWidthHeight(280.0f, 35.0f);
	m_pScene2D[1]->SetCol(D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f));
	m_pScene2D[1]->BindTexture(NULL);

	return S_OK;
}

//=============================================================================
// ゲージの終了処理
//=============================================================================
void CSansoGauge::Uninit(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// オブジェクトの解放
	Release();
}

//=============================================================================
// ゲージの更新処理
//=============================================================================
void CSansoGauge::Update(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

#ifdef _DEBUG

#if 1
	if (pInputKeyboard->GetPress(DIK_1) == true)
	{
		m_fRight -= 10.0f;
	}
	if (pInputKeyboard->GetPress(DIK_2) == true)
	{
		m_fRight += 10.0f;
	}

	if (pInputKeyboard->GetPress(DIK_3) == true)
	{
		m_fLeft += 10.0f;
	}
	if (pInputKeyboard->GetPress(DIK_4) == true)
	{
		m_fLeft -= 10.0f;
	}

	if (m_fLeft > 0 )
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
	m_pScene2D[0]->SetRIghtLeft(m_fRight, 0.0f);
	m_pScene2D[1]->SetRIghtLeft(0.0f, m_fLeft);

#endif
	//CDebugProc::Print("cfccfccfc", "2DPos    : x", m_Pos.x, "f", " y", m_Pos.y, "f", " z", m_Pos.z, "f");
#endif
}

//=============================================================================
// ゲージの描画処理
//=============================================================================
void CSansoGauge::Draw(void)
{
}

//=============================================================================
// オブジェクトの取得
//=============================================================================
D3DXVECTOR3 CSansoGauge::GetPosition(void)
{
	return m_Pos;
}

//=============================================================================
// オブジェクトの設定
//=============================================================================
void CSansoGauge::SetPosition(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}


//=============================================================================
// 左右
//=============================================================================
void CSansoGauge::SetSansoGaugeRightLeft(float fRight, float fLeft)
{
	// プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	// エネミーの取得
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

	m_fRight += fRight;
	m_fLeft += fLeft;
	//上限を超えないように
	if (m_fLeft > 0)
	{
		m_fLeft = 0;
	}
	else if (m_fLeft <= -550 && m_fLeft > -600)
	{
		pEnemy->SetRecovery(true);
		pEnemy->SetRecoveryTime(120);
		//m_fLeft = -600;
	}
	else if (m_fLeft <= -600)
	{
		m_fLeft = -600;
	}

	if (m_fRight > 0)
	{
		m_fRight = 0;
	}
	else if (m_fRight <= -550 && m_fRight > -600)
	{
		pPlayer->SetRecovery(true);
		pPlayer->SetRecoveryTime(120);
		//m_fRight = -600;
	}
	else if (m_fRight <= -600)
	{
		m_fRight = -600;
	}

	//ゲージの左右を調整
	m_pScene2D[0]->SetRIghtLeft(m_fRight, 0.0f);
	m_pScene2D[1]->SetRIghtLeft(0.0f, m_fLeft);
}
