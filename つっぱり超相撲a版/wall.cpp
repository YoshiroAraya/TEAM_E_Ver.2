//=============================================================================
//
// 壁の処理 [field.cpp]
// Author : Atsushi Yamashita
//
//=============================================================================
#include "wall.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
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
CWall::CWall() : CScene3D(4, CScene::OBJTYPE_DOHYOCIRCLE)
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
CWall::~CWall()
{

}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT CWall::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	//テクスチャの貼り付け
	BindTexture(CLoad::GetTexture(CLoad::TEXTURE_WALL));

	//初期化処理
	CScene3D::SetSize(m_fHeight,m_fWidth);
	CScene3D::Init(m_pos);
	CScene3D::SetRot(m_rot);

	//色の設定
	CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//CScene3D::SetPosition(D3DXVECTOR3(m_pos), m_fWidth, m_fHeight);

	m_fCol = 0.0f;
	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CWall::Uninit(void)
{
	//円の終了処理
	CScene3D::Uninit();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CWall::Update(void)
{



	//CDebugProc::print(1, "\n\n\nLength %.1f", fLength);
	//CScene3D::SetPosition(D3DXVECTOR3(m_pos), m_fHeight, m_fWidth);
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CWall::Draw(void)
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
CWall *CWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot ,float fHeight,float fWidth)
{
	CWall *pWall = {};

	if (pWall == NULL)
	{
		//メモリを動的確保
		pWall = new CWall;

		if (pWall != NULL)
		{//NULLチェック

			pWall->m_pos = pos;
			pWall->m_rot = rot;
			pWall->m_fHeight = fHeight;
			pWall->m_fWidth = fWidth;


			//初期化処理
			pWall->Init();
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
	return pWall;
}