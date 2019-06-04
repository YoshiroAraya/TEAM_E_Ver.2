//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "title.h"
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
#include "loadModel.h"

//============================================================================
//	マクロ定義
//============================================================================

//============================================================================
//静的メンバ変数宣言
//============================================================================

//=============================================================================
//	コンストラクタ
//=============================================================================
CTitle::CTitle()
{

}

//=============================================================================
//デストラクタ
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
void CTitle::Init(void)
{
	m_state = STATE_NEWS;
	m_bSetDohyo = true;

	CLoadModel::Load();

	CNewsCaster::LoadMat();
	CDohyo::LoadMat();
	CDohyoCircle::Load();

	CNewsBG::Load();

	CNewsBG::Create(D3DXVECTOR3(0.0f, 25.0f, 40.0f), D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f), 100.0f, 180.0f);
	CNewsCaster::Create(D3DXVECTOR3(0.0f, 0.0f, -130.0f));

	CField::Load();
	CWall::Load();
	CLogo::Load();
	CTime::Load();
	CTime::Create(D3DXVECTOR3(100.0f, 55.0f, 0.0f));
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	CNewsCaster::UnloadMat();
	//CDohyo::UnloadModel();
	CDohyo::UnloadMat();
	CDohyoCircle::Unload();
	CField::Unload();
	CWall::Unload();
	CNewsBG::Unload();
	CLogo::Unload();
	CTime::Unload();

	CLoadModel::Unload();

	//全ての終了処理
	CScene::ReleseAll();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CTitle::Update(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	CManager *pManager = NULL;
	CFade *pFade = pManager->GetFade();

	if (m_state == CTitle::STATE_TITLE)
	{
		if (m_bSetDohyo == true)
		{
			CDohyo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CDohyoCircle::Create(D3DXVECTOR3(0, 25.0f, 0), 0.0f, 0.0f);
			CField::Create(D3DXVECTOR3(0, -10.0f, 0), 700.0f);
			CWall::Create(D3DXVECTOR3(0, 200.0f, 500), D3DXVECTOR3(300.0f, 0.0f, 0.0f), 200.0f, 700.0f);
			CWall::Create(D3DXVECTOR3(-550, 200.0f, 0), D3DXVECTOR3(300.0f, 300.0f, 0.0f), 200.0f, 700.0f);
			CWall::Create(D3DXVECTOR3(0, 200.0f, -500), D3DXVECTOR3(300.0f, 600.0f, 0.0f), 200.0f, 700.0f);
			CWall::Create(D3DXVECTOR3(550, 200.0f, 0), D3DXVECTOR3(300.0f, 900.0f, 0.0f), 200.0f, 700.0f);
			CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, CLogo::TYPE_TITLE);
			//CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 50, 50, CLogo::TYPE_TEST);
			m_bSetDohyo = false;
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		pFade->SetFade(pManager->MODE_GAME, pFade->FADE_OUT);
	}

#ifdef _DEBUG
	CDebugProc::Print("c", "タイトル");
#endif
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CTitle::Draw(void)
{

}