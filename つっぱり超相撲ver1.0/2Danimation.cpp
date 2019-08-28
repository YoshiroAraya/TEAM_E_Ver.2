//=============================================================================
//
// アニメーションの処理 [animation.cpp]
// Author : Yamashita
//
//=============================================================================
#include "main.h"
#include "2Danimation.h"
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "load.h"
#include "player.h"
#include "game.h"
#include "tutorial.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
//	コンストラクタ
//=============================================================================
C2DAnimation::C2DAnimation() : CScene2D(7, CScene2D::OBJTYPE_EFFECT)
{
	m_fUV_U = 0.0f;
	m_fUV_V = 0.0f;
	m_nLife = 0;
	m_nDrawType = 0;
	m_nCounterAnim = 0;
	m_fCntSpeed = 0.0f;
	m_nPatternAnim = 0;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
//デストラクタ
//=============================================================================
C2DAnimation::~C2DAnimation()
{

}

//=============================================================================
// アニメーションの初期化処理
//=============================================================================
HRESULT C2DAnimation::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, float fUV_U, float fUV_V, float fCntSpeed, int nTotalAnim, int nRoop,
	int nDrawType)
{
	//色を代入
	m_col = col;


	//初期化
	CScene2D::Init(pos);
	//テクスチャの貼り付け
	CScene2D::BindTexture(CLoad::GetTexture(CLoad::TEXTURE_ANIMATION_OUGI));
	CScene2D::SetPos(pos,0.0f,fScale,m_col);
	//UVの値を代入
	m_fUV_U = fUV_U;
	m_fUV_V = fUV_V;

	//アニメーションの設定
	SetAnimation(0, m_fUV_U, m_fUV_V);

	//アニメーションの速さ
	m_fCntSpeed = fCntSpeed;

	//アニメーションの合計枚数
	m_nTotalAnim = nTotalAnim;

	//アニメーションループするかしないか
	m_nRoop = nRoop;

	//描画タイプ
	m_nDrawType = nDrawType;


	m_bUse = true;
	return S_OK;
}

//=============================================================================
// アニメーションの終了処理
//=============================================================================
void C2DAnimation::Uninit(void)
{
	//終了処理
	CScene2D::Uninit();
}

//=============================================================================
// アニメーションの更新処理
//=============================================================================
void C2DAnimation::Update(void)
{
	CGame *pGame = NULL;
	CPlayer *pPlayer = NULL;
	D3DXVECTOR3 pos;


	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
	}

	if (pPlayer != NULL)
	{
		pos = pPlayer->GetPosition();
	}
	//テクスチャの破棄フラグ
	bool bDestroy = false;

	if (m_nRoop == 0)
	{//ループする

	 //アニメーションを進める更新処理
		UpdateAnim();
	}
	else if (m_nRoop == 1)
	{//ループしない

	 //寿命を減らす
		m_nLife--;

		//透明度を減らす
		//m_col.a -= 0.05f;

		if (m_bUse == true)
		{
			//アニメーションを進める更新処理
			UpdateAnim();
		}

		if (m_nTotalAnim - 1 == m_nPatternAnim)
		{//アニメーションを止める
			m_bUse = false;
		}
		//色の値を更新する
		SetCol(m_col);
	}

	//色が0.0fになったら
	if (m_col.a <= 0.0f)
	{
		//破棄フラグがたった
		bDestroy = true;
	}

	if (bDestroy == true)
	{
		//テクスチャを破棄
		Uninit();
	}

}

//=============================================================================
// アニメーションを進める更新処理
//=============================================================================
void C2DAnimation::UpdateAnim(void)
{
	//アニメーションのカウンターを進める
	m_nCounterAnim++;

	if ((m_nCounterAnim % (int)m_fCntSpeed) == 0)
	{
		//パターン更新
		m_nPatternAnim = (m_nPatternAnim + 1) % m_nTotalAnim;

		//アニメーションの設定
		SetAnimation(m_nPatternAnim, m_fUV_U, m_fUV_V);
	}
}

//=============================================================================
//アニメーションの描画処理
//=============================================================================
void C2DAnimation::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	if (m_nDrawType == 0)
	{
		//アルファテスト(透明色を描画しないように)
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 500);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		// αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		//描画処理
		CScene2D::Draw();

		// αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//レンダーステートの設定を元に戻す
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
	else
	{
		//描画処理
		CScene2D::Draw();
	}
}

//=============================================================================
// アニメーションの生成処理
//=============================================================================
C2DAnimation *C2DAnimation::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, float fUV_U, float fUV_V, float fCntSpeed, int nTotalAnim,
	int nRoop,int nDrawType)
{
	C2DAnimation *pAnimation = {};

	if (pAnimation == NULL)
	{//NULLだったら

	 //メモリの動的確保
		pAnimation = new C2DAnimation;

		if (pAnimation != NULL)
		{
			// ポリゴンの初期化処理
			pAnimation->Init(pos, col, fScale, fUV_U, fUV_V, fCntSpeed, nTotalAnim, nRoop, nDrawType);
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

	return pAnimation;
}