//=============================================================================
//
// アニメーションの処理 [animation.cpp]
// Author : Yamashita
//
//=============================================================================
#include "main.h"
#include "animation.h"
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "load.h"
#include "player.h"
#include "game.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
//	コンストラクタ
//=============================================================================
CAnimation::CAnimation() : CScene3D(7,CScene3D::OBJTYPE_EFFECT)
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
CAnimation::~CAnimation()
{

}

//=============================================================================
// アニメーションの初期化処理
//=============================================================================
HRESULT CAnimation::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXCOLOR col, float fHeight, float fWidth, float fUV_U, float fUV_V, float fCntSpeed, int nTotalAnim,int nRoop,
	int nDrawType)
{
	//色を代入
	m_col = col;



	CScene3D::SetSize(fHeight, fWidth);
	CScene3D::SetRot(rot);


	//初期化
	CScene3D::Init(pos);
	//テクスチャの貼り付け
	CScene3D::BindTexture(CLoad::GetTexture(CLoad::TEXTURE_ANIMATION));
	CScene3D::SetColor(m_col);
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
void CAnimation::Uninit(void)
{
	//終了処理
	CScene3D::Uninit();
}

//=============================================================================
// アニメーションの更新処理
//=============================================================================
void CAnimation::Update(void)
{
	CGame *pGame = NULL;
	CPlayer *pPlayer = pGame->GetPlayer();
	D3DXVECTOR3 pos;

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
	else if(m_nRoop == 1)
	{//ループしない

		//寿命を減らす
		m_nLife--;

		//透明度を減らす
		m_col.a -= 0.05f;

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
		SetColor(m_col);
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

	CScene3D::SetPos(D3DXVECTOR3(pos.x,pos.y + 45.0f,pos.z - 10.0f));
}

//=============================================================================
// アニメーションを進める更新処理
//=============================================================================
void CAnimation::UpdateAnim(void)
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
void CAnimation::Draw(void)
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
		CScene3D::Draw();

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
		CScene3D::Draw();
	}
}

//=============================================================================
// アニメーションの生成処理
//=============================================================================
CAnimation *CAnimation::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXCOLOR col,float fHeight,float fWidth, float fUV_U, float fUV_V, float fCntSpeed, int nTotalAnim,int nRoop,
	int nDrawType)
{
	CAnimation *pAnimation = {};

	if (pAnimation == NULL)
	{//NULLだったら

	 //メモリの動的確保
		pAnimation = new CAnimation;

		if (pAnimation != NULL)
		{
			// ポリゴンの初期化処理
			pAnimation->Init(pos,rot, col,fWidth,fHeight,fUV_U,fUV_V,fCntSpeed,nTotalAnim,nRoop,nDrawType);
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