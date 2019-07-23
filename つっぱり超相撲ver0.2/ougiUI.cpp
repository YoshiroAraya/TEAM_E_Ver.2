//=============================================================================
//
// 奥義UIの処理 [ougiUI.h]
// Author : 目黒未来也
//
//=============================================================================
#include "scene.h"
#include "ougiUI.h"
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


//=============================================================================
// シーンクラスのコンストラクタ
//=============================================================================
COugiUI::COugiUI(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
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
COugiUI *COugiUI::Create(D3DXVECTOR3 pos)
{
	COugiUI *pOugiUI = NULL;

	if (pOugiUI == NULL)
	{
		// オブジェクトクラスの生成
		pOugiUI = new COugiUI;

		if (pOugiUI != NULL)
		{
			pOugiUI->Init(pos);
		}
	}

	return pOugiUI;
}

//=============================================================================
// デストラクタ
//=============================================================================
COugiUI::~COugiUI()
{
}

//=============================================================================
// ゲージの初期化処理
//=============================================================================
HRESULT COugiUI::Init(D3DXVECTOR3 pos)
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
	m_pScene2D[0] = CScene2D::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	m_pScene2D[0]->SetWidthHeight(300.0f, 30.0f);
	m_pScene2D[0]->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	m_pScene2D[0]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_OUGIUI00));


	////2p側
	//m_pScene2D[1] = CScene2D::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	//m_pScene2D[1]->SetWidthHeight(300.0f, 30.0f);
	//m_pScene2D[1]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//m_pScene2D[1]->BindTexture(NULL);

	return S_OK;
}

//=============================================================================
// ゲージの終了処理
//=============================================================================
void COugiUI::Uninit(void)
{

	// オブジェクトの解放
	Release();
}

//=============================================================================
// ゲージの更新処理
//=============================================================================
void COugiUI::Update(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

#ifdef _DEBUG

#if 1


	//ゲージの左右を調整
	m_pScene2D[0]->SetRIghtLeft(0.0f, m_fLeft);
	//m_pScene2D[1]->SetRIghtLeft(m_fRight, 0.0f);

#endif
	//CDebugProc::Print("cf", "m_fRight    : ", m_fRight);
	//CDebugProc::Print("cf", "m_fLeft    : ", m_fLeft);
#endif
}

//=============================================================================
// ゲージの描画処理
//=============================================================================
void COugiUI::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
}

//=============================================================================
// オブジェクトの取得
//=============================================================================
D3DXVECTOR3 COugiUI::GetPosition(void)
{
	return m_Pos;
}