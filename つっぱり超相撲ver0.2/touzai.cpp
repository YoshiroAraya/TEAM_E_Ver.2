//=============================================================================
//
// 東西の処理 [touzai.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "touzai.h"
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
// 東西の生成処理
//=============================================================================
CTouzai *CTouzai::Create(D3DXVECTOR3 pos)
{
	CTouzai *pTouzai = NULL;

	if (pTouzai == NULL)
	{
		// 背景クラスの生成
		pTouzai = new CTouzai;

		if (pTouzai != NULL)
		{
			pTouzai->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_TOUZAI));
			pTouzai->Init(pos);
		}
	}
	return pTouzai;
}

//=============================================================================
// 東西クラスのコンストラクタ
//=============================================================================
CTouzai::CTouzai() : CScene2D(3)
{
	// 値をクリア
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	m_nSelect = 0;
	m_fScale = 0.0f;
}

//=============================================================================
// 東西クラスのデストラクタ
//=============================================================================
CTouzai::~CTouzai()
{
}

//=============================================================================
// 東西初期化処理
//=============================================================================
HRESULT CTouzai::Init(D3DXVECTOR3 pos)
{
	m_nSelect = 0;
	m_fScale = 10.0f;
	m_bReset = true;

	CScene2D::Init(pos);

	CScene2D::SetWidthHeight(m_fScale, m_fScale);

	// オブジェクトの種類の設定
	CScene2D::SetObjType(CScene::OBJTYPE_UI);

	return S_OK;
}

//=============================================================================
// 東西終了処理
//=============================================================================
void CTouzai::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 東西更新処理
//=============================================================================
void CTouzai::Update(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;
	pVtxBuff = CScene2D::GetBuff();

	D3DXVECTOR3 pos = CScene2D::GetPosition();

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (CCamera::GetState() == CCamera::STATE_HIGASHI)
	{
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

		if (m_fScale <= TOUZAI_SIZE)
		{
			m_fScale += 5.0f;

			if (m_fScale > TOUZAI_SIZE)
			{
				m_fScale = TOUZAI_SIZE;
			}
		}

		pos = D3DXVECTOR3(200.0f, 150.0f, 0.0f);
	}
	else if(CCamera::GetState() == CCamera::STATE_NISHI)
	{
		if (m_bReset == true)
		{
			m_fScale = 10.0f;

			m_bReset = false;
		}

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		if (m_fScale <= TOUZAI_SIZE)
		{
			m_fScale += 5.0f;

			if (m_fScale > TOUZAI_SIZE)
			{
				m_fScale = TOUZAI_SIZE;
			}
		}

		pos = D3DXVECTOR3(SCREEN_WIDTH - 200.0f, 150.0f, 0.0f);
	}

	CScene2D::SetBuff(pVtxBuff);
	CScene2D::SetPos(pos, 0.0f, m_fScale, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	if (CCamera::GetState() == CCamera::STATE_NORMAL)
	{
		Uninit();
	}
}

//=============================================================================
// 東西描画処理
//=============================================================================
void CTouzai::Draw(void)
{
	CScene2D::Draw();
}