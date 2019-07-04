//=============================================================================
//
// エフェクト処理 [effect3D.cpp]
// Author : 山下敦史
//
//=============================================================================
#include "effect2D.h"
#include "manager.h"
#include "renderer.h"
#include "billboard.h"
#include "debugProc.h"
#include "load.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_POS_X_INIT			(1.0f)									//テクスチャ座標Uの初期位置
#define TEX_POS_Y_INIT			(1.0f)									//テクスチャ座標Vの初期位置
#define TEX_LEFT				(0.0f)									//テクスチャ座標U左
#define TEX_RIGHT				(1.0f)									//テクスチャ座標U右
#define TEX_TOP					(0.0f)									//テクスチャ座標V上
#define TEX_BOT					(1.0f)									//テクスチャ座標V下

//--------------------------------------------
//静的メンバ変数宣言
//--------------------------------------------

//--------------------------------------------
//エフェクトクラス コンストラクタ
//--------------------------------------------
CEffect2D::CEffect2D() : CScene2D(7, CScene::OBJTYPE_EFFECT)
{
	m_pos = D3DXVECTOR3(0, 0, 0);						// 位置
	m_move = D3DXVECTOR3(0, 0, 0);					// 移動量
	m_posold = D3DXVECTOR3(0, 0, 0);				// 前回の位置
}

//--------------------------------------------
//エフェクトクラス デストラクタ
//--------------------------------------------
CEffect2D::~CEffect2D()
{
}

//--------------------------------------------
//エフェクトの生成
//--------------------------------------------
CEffect2D *CEffect2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
	float fWidth, float fHeight, int nLife, int TexType)
{
	CEffect2D *pEffect = NULL;

	if (pEffect == NULL)
	{
		//メモリを動的確保
		pEffect = new CEffect2D;

		if (pEffect != NULL)
		{
			pEffect->m_pos = pos;
			pEffect->m_move = move;
			pEffect->m_Col = col;
			pEffect->m_fHeight = fHeight;
			pEffect->m_fWidth = fWidth;
			pEffect->m_nLife = nLife;
			pEffect->m_nTexType = TexType;
			pEffect->Init();
		}
	}
	return pEffect;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEffect2D::Init(void)
{
	//初期化処理
	CScene2D::Init(m_pos);

	//テクスチャの設定
	CEffect2D::BindTexture(CLoad::GetTexture(m_nTexType));

	//サイズの設定
	CEffect2D::SetWidthHeight(m_fWidth, m_fHeight);

	m_fAlpha = 1.0f / (float)m_nLife;
	
	m_nCntTimer = 0;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEffect2D::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEffect2D::Update(void)
{
	if (m_nTexType == CLoad::TEXTURE_EFFECT_NORMAL001)
	{
		//パーティクル
		UpdateUI();
	}
}

//=============================================================================
// お金の更新処理
//=============================================================================
void CEffect2D::UpdateUI(void)
{
	//自分用の死亡フラグ変数
	bool bDestroy = false;

	m_nCntTimer++;


	/*D3DXVECTOR3 Parpos1 = D3DXVECTOR3(sinf(fAngle) * fLength1, 0.0f, cosf(fAngle) * fLength1);
	D3DXVECTOR3 Parpos2 = D3DXVECTOR3(sinf(fAngle) * fLength2, 0.0f, cosf(fAngle) * fLength2);*/

	if (m_nLife > 0)
	{
		m_nLife--;

		//位置を更新		
		m_pos += m_move;
		
		//徐々に透明にしていく
		//m_Col.a = m_Col.a - m_fAlpha;
		m_Col.a -= 0.02f;

		//一定以下になったら0に
		if (m_Col.a < 0.01f)
		{
			m_Col.a = 0;
		}
		m_fWidth += 3.0f;
		m_fHeight += 3.0f;
		//色を設定
		CScene2D::SetCol(m_Col);

		//設定処理
		CScene2D::SetWidthHeight(m_fWidth, m_fHeight);
		CScene2D::SetPosition(m_pos);
	}
	else if (m_nLife <= 0)
	{
		//自分を消すフラグを立てる
		bDestroy = true;
	}

	if (bDestroy == true)
	{
		//自分を消す(破棄)
		Uninit();
	}
	/*CDebugProc::Print("c", "エフェクト");*/
}

//=============================================================================
// 描画処理
//=============================================================================
void CEffect2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	// αブレンディングを加算合成に設定
	/*pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);*/

	CScene2D::Draw();

	// αブレンディングを元に戻す
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	////ライトを有効にする
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}