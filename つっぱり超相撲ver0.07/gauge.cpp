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
#include "gauge.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
//--------------------------------------------
//静的メンバ変数宣言
//--------------------------------------------
LPDIRECT3DTEXTURE9			CGauge::m_pTexture = {};


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
	m_pScene2D[0] = CScene2D::Create(D3DXVECTOR3(300.0f, 580.0f, 0.0f));
	m_pScene2D[0]->SetWidthHeight(300.0f, 50.0f);
	m_pScene2D[0]->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	m_pScene2D[0]->BindTexture(NULL);

	//2p側
	m_pScene2D[1] = CScene2D::Create(D3DXVECTOR3(980.0f, 580.0f, 0.0f));
	m_pScene2D[1]->SetWidthHeight(300.0f, 50.0f);
	m_pScene2D[1]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pScene2D[1]->BindTexture(NULL);

	return S_OK;
}

//=============================================================================
// ゲージの終了処理
//=============================================================================
void CGauge::Uninit(void)
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
void CGauge::Update(void)
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
// テクスチャの読み込み
//=============================================================================
HRESULT CGauge::Load(void)
{
	//デバイスを取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, GAUGE_NAME, &m_pTexture);

	return S_OK;
}
//=============================================================================
// テクスチャの破棄
//=============================================================================
void CGauge::UnLoad(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 左右
//=============================================================================
void CGauge::SetGaugeRightLeft(float fRight, float fLeft)
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
	else if (m_fLeft > -570)
	{
		pEnemy->SetDying(false);
	}
	else if (m_fLeft <= -570)
	{
		pEnemy->SetDying(true);
	}
	else if (m_fLeft < -600)
	{
		m_fLeft = -600;
	}


	if (m_fRight > 0)
	{
		m_fRight = 0;
	}
	else if (m_fRight > -570)
	{
		pPlayer->SetDying(false);
	}
	else if (m_fRight <= -570)
	{
		pPlayer->SetDying(true);
	}
	else if (m_fRight < -600)
	{
		m_fRight = -600;
	}

	//ゲージの左右を調整
	m_pScene2D[0]->SetRIghtLeft(m_fRight, 0.0f);
	m_pScene2D[1]->SetRIghtLeft(0.0f, m_fLeft);
}
