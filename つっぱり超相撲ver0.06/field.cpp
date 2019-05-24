//=============================================================================
//
// 床の処理 [field.cpp]
// Author : Atsushi Yamashita
//
//=============================================================================
#include "field.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "bullet.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include <string>
#include <time.h>

//============================================================================
//	静的メンバ変数宣言
//============================================================================
LPDIRECT3DTEXTURE9 CField::m_pTextures = NULL;

//============================================================================
//	マクロ定義
//============================================================================
#define MAX_HEIGHT (20)   //（高さ）
#define MAX_WITH   (10)   //（幅）

#define MAX_SIZE3D (70)  //3Dポリゴンの大きさ
//=============================================================================
//	コンストラクタ
//=============================================================================
CField::CField() : CScene3D(4, CScene::OBJTYPE_DOHYOCIRCLE)
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
CField::~CField()
{

}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
HRESULT CField::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/sand2.jpg", &m_pTextures);

	return S_OK;
}

//=============================================================================
// テクスチャの開放処理
//=============================================================================
void CField::Unload(void)
{
	//テクスチャの破棄
	if (m_pTextures != NULL)
	{
		m_pTextures->Release();
		m_pTextures = NULL;
	}
}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT CField::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	//テクスチャの貼り付け
	BindTexture(m_pTextures);

	//初期化処理
	CScene3D::SetSize(m_fSize, m_fSize);
	CScene3D::Init(m_pos);

	//色の設定
	CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//CScene3D::SetPosition(D3DXVECTOR3(m_pos), m_fWidth, m_fHeight);

	m_fCol = 0.0f;
	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CField::Uninit(void)
{
	//円の終了処理
	CScene3D::Uninit();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CField::Update(void)
{
	


	//CDebugProc::print(1, "\n\n\nLength %.1f", fLength);
	//CScene3D::SetPosition(D3DXVECTOR3(m_pos), m_fHeight, m_fWidth);
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CField::Draw(void)
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
CField *CField::Create(D3DXVECTOR3 pos, float fSize)
{
	CField *pField = {};

	if (pField == NULL)
	{
		//メモリを動的確保
		pField = new CField;

		if (pField != NULL)
		{//NULLチェック

			pField->m_pos = pos;
			pField->m_fSize = fSize;
			

			//初期化処理
			pField->Init();
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
	return pField;
}