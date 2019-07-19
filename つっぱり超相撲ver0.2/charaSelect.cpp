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

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PAUSE_POS_X		((SCREEN_WIDTH / 2))		// 東西の左上X座標
#define PAUSE_POS_Y		((SCREEN_HEIGHT / 2))		// 東西の左上Y座標
#define PAUSE_WIDTH		((SCREEN_WIDTH / 2))		// 東西の幅
#define PAUSE_HEIGHT	((SCREEN_HEIGHT / 2))		// 東西の高さ
#define TOUZAI_SIZE		(150.0f)						// 東西のサイズ

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
			pCharaSelect->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_CHARASELECT));
			pCharaSelect->Init(pos);
		}
	}
	return pCharaSelect;
}

//=============================================================================
// 決定テクスチャクラスのコンストラクタ
//=============================================================================
CCharaSelect::CCharaSelect() : CScene2D(3)
{
	// 値をクリア
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	m_nSelect = 0;
	m_fScale = 0.0f;
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
	m_nSelect = 0;
	m_fScale = 150.0f;
	m_bReset = true;

	CScene2D::Init(pos);

	CScene2D::SetWidthHeight(m_fScale, m_fScale);

	// オブジェクトの種類の設定
	CScene2D::SetObjType(CScene::OBJTYPE_UI);

	return S_OK;
}

//=============================================================================
// 決定テクスチャ終了処理
//=============================================================================
void CCharaSelect::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 決定テクスチャ更新処理
//=============================================================================
void CCharaSelect::Update(void)
{
	//// 入力情報を取得
	//CInputKeyboard *pInputKeyboard;
	//pInputKeyboard = CManager::GetInputKeyboard();
	//CXInputJoyPad *pXInput = NULL;
	//pXInput = CManager::GetXInput();

	//LPDIRECT3DVERTEXBUFFER9 pVtxBuff;
	//pVtxBuff = CScene2D::GetBuff();

	//D3DXVECTOR3 pos = CScene2D::GetPosition();

	//VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	//					// 頂点バッファをロックし、頂点データへのポインタを取得
	//pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//if (CCamera::GetState() == CCamera::STATE_HIGASHI)
	//{
	//	// テクスチャ座標の設定
	//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	//	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

	//	if (m_fScale <= TOUZAI_SIZE)
	//	{
	//		m_fScale += 5.0f;

	//		if (m_fScale > TOUZAI_SIZE)
	//		{
	//			m_fScale = TOUZAI_SIZE;
	//		}
	//	}

	//	pos = D3DXVECTOR3(200.0f, 150.0f, 0.0f);
	//}
	//else if (CCamera::GetState() == CCamera::STATE_NISHI)
	//{
	//	if (m_bReset == true)
	//	{
	//		m_fScale = 10.0f;

	//		m_bReset = false;
	//	}

	//	// テクスチャ座標の設定
	//	pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
	//	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//	pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
	//	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//	if (m_fScale <= TOUZAI_SIZE)
	//	{
	//		m_fScale += 5.0f;

	//		if (m_fScale > TOUZAI_SIZE)
	//		{
	//			m_fScale = TOUZAI_SIZE;
	//		}
	//	}

	//	pos = D3DXVECTOR3(SCREEN_WIDTH - 200.0f, 150.0f, 0.0f);
	//}

	//CScene2D::SetBuff(pVtxBuff);
	//CScene2D::SetPos(pos, 0.0f, m_fScale, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//=============================================================================
// 決定テクスチャ描画処理
//=============================================================================
void CCharaSelect::Draw(void)
{
	CScene2D::Draw();
}