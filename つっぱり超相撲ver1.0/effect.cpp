//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "billboard.h"
#include "debugProc.h"
#include "load.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURENAME002			 "data\\TEXTURE\\EFFECT\\effect003.png"		//テクスチャのファイル名
#define TEXTURENAME003			 "data\\TEXTURE\\EFFECT\\effect004.png"		//テクスチャのファイル名
#define TEXTURENAME004			 "data\\TEXTURE\\EFFECT\\effect005.png"		//テクスチャのファイル名

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
CEffect::CEffect() :CBillboard(7, CScene::OBJTYPE_EFFECT)
{
	m_pos = D3DXVECTOR3(0,0,0);						// 位置
	m_move = D3DXVECTOR3(0, 0, 0);					// 移動量
	m_posold = D3DXVECTOR3(0, 0, 0);				// 前回の位置


	//m_TexType = EFFECTTEX_NORMAL000;
}

//--------------------------------------------
//エフェクトクラス デストラクタ
//--------------------------------------------
CEffect::~CEffect()
{
}

//--------------------------------------------
//エフェクトの生成
//--------------------------------------------
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
	float fWidth, float fHeight, int nNumMax, int nLife, int TexType)
{
	CEffect *pEffect = NULL;

	if (pEffect == NULL)
	{
		//メモリを動的確保
		pEffect = new CEffect;

		if (pEffect != NULL)
		{
			pEffect->m_pos = pos;
			pEffect->m_move = move;
			pEffect->m_Col = col;
			pEffect->m_fHeight = fHeight;
			pEffect->m_fWidth = fWidth;
			pEffect->m_nNumMax = nNumMax;
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
HRESULT CEffect::Init(void)
{

	CBillboard::BindTexture(CLoad::GetTexture(m_nTexType));
	CBillboard::Init(m_pos);
	CBillboard::SetCol(m_Col);
	CBillboard::SetBillboard(m_pos, m_fHeight, m_fWidth);

	m_fAlpha = 1.0f / (float)m_nLife;
	//m_fAlpha = m_fAlpha / 60;
	m_nCntTimer = 0;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEffect::Uninit(void)
{
	CBillboard::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEffect::Update(void)
{
	if (m_nTexType == CLoad::TEXTURE_EFFECT_NORMAL002)
	{
		//煙のエフェクト
		UpdateSmoke();
	}

	if (m_nTexType == CLoad::TEXTURE_EFFECT_NORMAL000)
	{
		//塩パーティクル
		UpdateSalt();
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CEffect::UpdateSmoke(void)
{
	//自分用の死亡フラグ変数
	bool bDestroy = false;

	m_nCntTimer++;

	if (m_nLife > 0)
	{
		m_nLife--;

		//重力
		m_move.y += cosf(D3DX_PI * 0) * 0.05f;

		//位置を更新
		m_pos += m_move;
		
		//徐々に透明にしていく
		m_Col.a -= 0.009f;
		//一定以下になったら0に
		if (m_Col.a < 0.01f)
		{
			m_Col.a = 0;
		}

		//色を設定
		CBillboard::SetCol(m_Col);
		m_fHeight += 1.5f;
		m_fWidth += 1.5f;
		//設定処理
		CBillboard::SetBillboard(m_pos, m_fHeight, m_fWidth);
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

		//falseに戻す
		bDestroy = false;
	}
}

//=============================================================================
// お金の更新処理
//=============================================================================
void CEffect::UpdateSalt(void)
{
	//自分用の死亡フラグ変数
	bool bDestroy = false;

	m_nCntTimer++;

	if (m_nLife > 0)
	{
		m_nLife--;

		//重力
		m_move.y -= cosf(D3DX_PI * 0) * 0.2f;

		//位置を更新		
		m_pos += m_move;
		//m_rot.x += 0.2f;

		//徐々に透明にしていく
		m_Col.a = m_Col.a - m_fAlpha;

		//一定以下になったら0に
		if (m_Col.a < 0.01f)
		{
			m_Col.a = 0;
		}
		//色を設定
		CBillboard::SetCol(m_Col);
	
		//設定処理
		CBillboard::SetBillboard(m_pos, m_fHeight, m_fWidth);
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
void CEffect::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//if (m_pBillBoard != NULL)
	{
		CBillboard::Draw();
	}

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}