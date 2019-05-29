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

	CNewsCaster::LoadModel();
	CNewsCaster::LoadMat();

	CNewsBG::Load();

	CNewsBG::Create(D3DXVECTOR3(0.0f, 25.0f, 40.0f), D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f), 100.0f, 180.0f);
	CNewsCaster::Create(D3DXVECTOR3(0.0f, 0.0f, -130.0f));
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	CNewsCaster::UnloadModel();
	CNewsCaster::UnloadMat();

	CNewsBG::Unload();

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