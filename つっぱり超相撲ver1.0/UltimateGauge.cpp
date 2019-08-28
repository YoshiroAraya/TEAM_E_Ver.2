//=============================================================================
//
// ゲージ処理 [UltimateGauge.h]
// Author : 目黒未来也
//
//=============================================================================
#include "scene.h"
#include "UltimateGauge.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "load.h"
//--------------------------------------------
//マクロ定義
//--------------------------------------------
#define MAX_GAUG_WIDE	(450)
#define GAUGE_POSX		(350)
//=============================================================================
// シーンクラスのコンストラクタ
//=============================================================================
CUltimateGauge::CUltimateGauge(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
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
CUltimateGauge *CUltimateGauge::Create(D3DXVECTOR3 pos)
{
	CUltimateGauge *pGauge = NULL;

	if (pGauge == NULL)
	{
		// オブジェクトクラスの生成
		pGauge = new CUltimateGauge;

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
CUltimateGauge::~CUltimateGauge()
{
}

//=============================================================================
// ゲージの初期化処理
//=============================================================================
HRESULT CUltimateGauge::Init(D3DXVECTOR3 pos)
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
	m_pScene2D[0] = CScene2D::Create(D3DXVECTOR3(0 + GAUGE_POSX, pos.y, 0.0f));
	m_pScene2D[0]->SetWidthHeight(MAX_GAUG_WIDE / 2, 20.0f);
	m_pScene2D[0]->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	m_pScene2D[0]->BindTexture(NULL);
	m_bUlt[0] = false;

	//2p側
	m_pScene2D[1] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH - GAUGE_POSX, pos.y, 0.0f));
	m_pScene2D[1]->SetWidthHeight(MAX_GAUG_WIDE / 2, 20.0f);
	m_pScene2D[1]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pScene2D[1]->BindTexture(NULL);
	m_bUlt[1] = false;

	m_pScene2D[0]->SetRIghtLeft(-MAX_GAUG_WIDE, 0.0f);
	m_pScene2D[1]->SetRIghtLeft(0.0f, -MAX_GAUG_WIDE);

	m_fRight = -MAX_GAUG_WIDE;
	m_fLeft = -MAX_GAUG_WIDE;			// 左右の値

	m_pHPgaugeBG[0] = CScene2D::Create(D3DXVECTOR3(GAUGE_POSX, pos.y + 0.19f, 0.0f), 2);
	m_pHPgaugeBG[0]->SetWidthHeight(233.0f, 25.0f);
	m_pHPgaugeBG[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pHPgaugeBG[0]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_EXGAUGEBG));

	m_pHPgaugeBG[1] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH - GAUGE_POSX, pos.y + 0.19f, 0.0f), 2);
	m_pHPgaugeBG[1]->SetWidthHeight(233.0f, 25.0f);
	m_pHPgaugeBG[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pHPgaugeBG[1]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_EXGAUGEBG));
	m_pHPgaugeBG[1]->SetRot(D3DX_PI);

	return S_OK;
}

//=============================================================================
// ゲージの終了処理
//=============================================================================
void CUltimateGauge::Uninit(void)
{

	// オブジェクトの解放
	Release();
}

//=============================================================================
// ゲージの更新処理
//=============================================================================
void CUltimateGauge::Update(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();


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
	if (m_fLeft > 0 )
	{
		m_fLeft = 0;
	}
	else if (m_fLeft < -MAX_GAUG_WIDE)
	{
		m_fLeft = -MAX_GAUG_WIDE;
	}
	if (m_fLeft == 0)
	{
		m_bUlt[1] = true;
	}
	else if (m_fLeft <= 0)
	{
		m_bUlt[1] = false;
	}

	if (m_fRight > 0)
	{
		m_fRight = 0;
	}
	else if (m_fRight < -MAX_GAUG_WIDE)
	{
		m_fRight = -MAX_GAUG_WIDE;
	}
	if (m_fRight == 0)
	{
		m_bUlt[0] = true;
	}
	else if (m_fRight <= 0)
	{
		m_bUlt[0] = false;
	}

	//ゲージの左右を調整
	m_pScene2D[0]->SetRIghtLeft(0.0f, m_fLeft);
	m_pScene2D[1]->SetRIghtLeft(m_fRight, 0.0f);

	//CDebugProc::Print("cf", "m_fRight    : ", m_fRight);
	//CDebugProc::Print("cf", "m_fLeft    : ", m_fLeft);
#endif
}

//=============================================================================
// ゲージの描画処理
//=============================================================================
void CUltimateGauge::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();


}

//=============================================================================
// オブジェクトの取得
//=============================================================================
D3DXVECTOR3 CUltimateGauge::GetPosition(void)
{
	return m_Pos;
}

//=============================================================================
// オブジェクトの設定
//=============================================================================
void CUltimateGauge::SetPosition(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
// 左右
//=============================================================================
void CUltimateGauge::SetGaugeRightLeft(float fRight, float fLeft)
{
	m_fRight += fRight;
	m_fLeft += fLeft;
	//上限を超えないように
	if (m_fLeft > 0)
	{
		m_fLeft = 0;
	}
	else if (m_fLeft < -MAX_GAUG_WIDE)
	{
		m_fLeft = -MAX_GAUG_WIDE;
	}

	if (m_fRight > 0)
	{
		m_fRight = 0;
	}
	else if (m_fRight < -MAX_GAUG_WIDE)
	{
		m_fRight = -MAX_GAUG_WIDE;
	}

	//ゲージの左右を調整
	m_pScene2D[0]->SetRIghtLeft(0.0f, m_fLeft);
	m_pScene2D[1]->SetRIghtLeft(m_fRight, 0.0f);
}
