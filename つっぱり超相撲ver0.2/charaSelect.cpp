//=============================================================================
//
// 決定テクスチャの処理 [charaSelect.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "charaSelect.h"
#include "renderer.h"
#include "input.h"
#include "scene2D.h"
#include "manager.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "camera.h"
#include "load.h"
#include "title.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PAUSE_POS_X		((SCREEN_WIDTH / 2))		// 東西の左上X座標
#define PAUSE_POS_Y		((SCREEN_HEIGHT / 2))		// 東西の左上Y座標
#define PAUSE_WIDTH		((SCREEN_WIDTH / 2))		// 東西の幅
#define PAUSE_HEIGHT	((SCREEN_HEIGHT / 2))		// 東西の高さ
#define SELECT_SIZE		(50.0f)						// 東西のサイズ

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// 決定テクスチャの生成処理
//=============================================================================
CCharaSelect *CCharaSelect::Create(D3DXVECTOR3 pos)
{
	CCharaSelect *pCharaSelect = NULL;

	if (pCharaSelect == NULL)
	{
		// 背景クラスの生成
		pCharaSelect = new CCharaSelect;

		if (pCharaSelect != NULL)
		{
			pCharaSelect->Init(pos);
		}
	}
	return pCharaSelect;
}

//=============================================================================
// 決定テクスチャクラスのコンストラクタ
//=============================================================================
CCharaSelect::CCharaSelect(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 値をクリア
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	m_nSelect = 0;
	m_fScale = 0.0f;

	for (int nCntPause = 0; nCntPause < MAX_SELECT; nCntPause++)
	{
		m_apScene2D[nCntPause] = NULL;
	}
}

//=============================================================================
// 決定テクスチャクラスのデストラクタ
//=============================================================================
CCharaSelect::~CCharaSelect()
{
}

//=============================================================================
// 決定テクスチャ初期化処理
//=============================================================================
HRESULT CCharaSelect::Init(D3DXVECTOR3 pos)
{
	CNumPlayer::MODE mode;
	mode = CNumPlayer::GetMode();

	m_nSelect = 0;
	m_fScale = SELECT_SIZE;
	m_bReset = true;

	m_apScene2D[0] = new CScene2D(SELECT_PRIORITY);
	m_apScene2D[0]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_CHARASELECT));
	m_apScene2D[0]->Init(pos);
	m_apScene2D[0]->SetWidthHeight(100.0f, 100.0f);
	m_apScene2D[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	if (mode != NULL)
	{
		if (mode == CNumPlayer::MODE_2P)
		{
			m_apScene2D[1] = new CScene2D(SELECT_PRIORITY);
			m_apScene2D[1]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_CHARASELECT));
			m_apScene2D[1]->Init(D3DXVECTOR3(SCREEN_WIDTH - pos.x, pos.y, pos.z));
			m_apScene2D[1]->SetWidthHeight(100.0f, 100.0f);
			m_apScene2D[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	
	return S_OK;
}

//=============================================================================
// 決定テクスチャ終了処理
//=============================================================================
void CCharaSelect::Uninit(void)
{
	// 2Dオブジェクト終了処理
	for (int nCntPause = 0; nCntPause < MAX_SELECT; nCntPause++)
	{
		if (m_apScene2D[nCntPause] != NULL)
		{
			m_apScene2D[nCntPause]->Uninit();
			m_apScene2D[nCntPause] = NULL;
		}
	}

	// オブジェクトの解放
	Release();
}

//=============================================================================
// 決定テクスチャ更新処理
//=============================================================================
void CCharaSelect::Update(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();

	CNumPlayer::MODE mode;
	mode = CNumPlayer::GetMode();

	// タイトル取得
	CTitle *pTitle = CManager::GetTitle();

	if (pTitle->GetCharaSelect(0) == false)
	{// 選択中
		// テクスチャ座標の設定
		m_apScene2D[0]->SetTex(D3DXVECTOR2(0.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
	}
	else if(pTitle->GetCharaSelect(0) == true)
	{// 決定
		// テクスチャ座標の設定
		m_apScene2D[0]->SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.5f));
	}

	if (mode != NULL)
	{
		if (mode == CNumPlayer::MODE_2P)
		{
			if (pTitle->GetCharaSelect(1) == false)
			{// 選択中
			 // テクスチャ座標の設定
				m_apScene2D[1]->SetTex(D3DXVECTOR2(0.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
			}
			else if (pTitle->GetCharaSelect(1) == true)
			{// 決定
			 // テクスチャ座標の設定
				m_apScene2D[1]->SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.5f));
			}
		}
	}
}

//=============================================================================
// 決定テクスチャ描画処理
//=============================================================================
void CCharaSelect::Draw(void)
{
}