//=============================================================================
//
// 壁の処理 [field.cpp]
// Author : Atsushi Yamashita
//
//=============================================================================
#include "newsBG.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "title.h"
#include "load.h"
#include <string>
#include <time.h>

//============================================================================
//	静的メンバ変数宣言
//============================================================================

//============================================================================
//	マクロ定義
//============================================================================
#define MAX_HEIGHT (20)   //（高さ）
#define MAX_WITH   (10)   //（幅）

#define MAX_SIZE3D (70)  //3Dポリゴンの大きさ
//=============================================================================
//	コンストラクタ
//=============================================================================
CNewsBG::CNewsBG() : CScene3D(4, CScene::OBJTYPE_DOHYOCIRCLE)
{
	//m_pTexture = NULL;					// テクスチャへのポインタ
	m_pVtxBuff = NULL;						// 頂点バッファへのポインタ

	m_pos = D3DXVECTOR3(0, 0, 0);			// ポリゴンの位置
	m_rot = D3DXVECTOR3(0, 0, 0);			// ポリゴンの向き
	m_move = D3DXVECTOR3(0, 0, 0);			// ポリゴンの動き

}

//=============================================================================
//デストラクタ
//=============================================================================
CNewsBG::~CNewsBG()
{

}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT CNewsBG::Init(void)
{
	//テクスチャの貼り付け
	BindTexture(CLoad::GetTexture(CLoad::TEXTURE_NEWS));

	//初期化処理
	CScene3D::SetSize(m_fHeight, m_fWidth);
	CScene3D::Init(m_pos);
	CScene3D::SetRot(m_rot);

	//色の設定
	CScene3D::SetColor(D3DXCOLOR(1.0f * (211.0f / 255.0f), 1.0f * (237.0f / 255.0f), 1.0f * (251.0f / 255.0f), 1.0f));

	CScene3D::SetNor(D3DXVECTOR3(0.0f, 0.0f, 1.0f));

	//CScene3D::SetPosition(D3DXVECTOR3(m_pos), m_fWidth, m_fHeight);

	m_fCol = 0.0f;
	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CNewsBG::Uninit(void)
{
	//円の終了処理
	CScene3D::Uninit();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CNewsBG::Update(void)
{
	// タイトル取得
	CTitle *pTitle;
	pTitle = CManager::GetTitle();

	if (pTitle->GetState() == CTitle::STATE_TITLE)
	{
		Uninit();
	}

	//CDebugProc::print(1, "\n\n\nLength %.1f", fLength);
	//CScene3D::SetPosition(D3DXVECTOR3(m_pos), m_fHeight, m_fWidth);
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CNewsBG::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	//描画処理
	CScene3D::Draw();
}

//=============================================================================
//円の生成処理
//=============================================================================
CNewsBG *CNewsBG::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth)
{
	CNewsBG *pNewsBG = {};

	if (pNewsBG == NULL)
	{
		//メモリを動的確保
		pNewsBG = new CNewsBG;

		if (pNewsBG != NULL)
		{//NULLチェック

			pNewsBG->m_pos = pos;
			pNewsBG->m_rot = rot;
			pNewsBG->m_fHeight = fHeight;
			pNewsBG->m_fWidth = fWidth;


			//初期化処理
			pNewsBG->Init();
		}
		else
		{
			MessageBox(0, "NULLでした", "警告", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "NULLじゃないです", "警告", MB_OK);
	}
	return pNewsBG;
}