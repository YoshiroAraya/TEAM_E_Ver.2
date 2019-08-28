//=============================================================================
//
// エフェクト処理 [effect3D.cpp]
// Author : 山下敦史
//
//=============================================================================
#include "effect3D.h"
#include "manager.h"
#include "renderer.h"
#include "billboard.h"
#include "debugProc.h"
#include "load.h"
#include "scene3D.h"

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
CEffect3D::CEffect3D() : CScene3D(7, CScene::OBJTYPE_EFFECT)
{
	m_pos = D3DXVECTOR3(0, 0, 0);						// 位置
	m_move = D3DXVECTOR3(0, 0, 0);					// 移動量
	m_posold = D3DXVECTOR3(0, 0, 0);				// 前回の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------
//エフェクトクラス デストラクタ
//--------------------------------------------
CEffect3D::~CEffect3D()
{
}

//--------------------------------------------
//エフェクトの生成
//--------------------------------------------
CEffect3D *CEffect3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
	float fWidth, float fHeight, int nNumMax, int nLife, int TexType)
{
	CEffect3D *pEffect = NULL;

	if (pEffect == NULL)
	{
		//メモリを動的確保
		pEffect = new CEffect3D;

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
HRESULT CEffect3D::Init(void)
{
	
	//テクスチャの設定
	CEffect3D::BindTexture(CLoad::GetTexture(m_nTexType));

	//サイズの設定
	CEffect3D::SetSize(m_fHeight, m_fWidth);

	//初期化処理
	CScene3D::Init(m_pos);

	m_fAlpha = 1.0f / (float)m_nLife;
	//m_fAlpha = m_fAlpha / 60;
	m_nCntTimer = 0;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEffect3D::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEffect3D::Update(void)
{
	if (m_nTexType == CLoad::TEXTURE_EFFECT_NORMAL001)
	{
		//お金パーティクル
		UpdateMoney();
	}

	if(m_nTexType == CLoad::TEXTURE_EFFECT_NORMAL000)
	{
		//塩パーティクル
		UpdateSalt();
	}

	if (m_nTexType == CLoad::TEXTURE_EFFECT_CONFETTI)
	{
		//塩パーティクル
		UpdateConfetti();
	}
}

//=============================================================================
// お金の更新処理
//=============================================================================
void CEffect3D::UpdateMoney(void)
{
	//自分用の死亡フラグ変数
	bool bDestroy = false;

	m_nCntTimer++;
	m_DrawType = 0;

	/*D3DXVECTOR3 Parpos1 = D3DXVECTOR3(sinf(fAngle) * fLength1, 0.0f, cosf(fAngle) * fLength1);
	D3DXVECTOR3 Parpos2 = D3DXVECTOR3(sinf(fAngle) * fLength2, 0.0f, cosf(fAngle) * fLength2);*/

	if (m_nLife > 0)
	{
		m_nLife--;

		//重力
		m_move.y -= cosf(D3DX_PI * 0) * 0.005f;

		//位置を更新		
		m_pos += m_move;
		m_rot.x += 0.06f;

		//徐々に透明にしていく
		//m_Col.a = m_Col.a - m_fAlpha;

		//一定以下になったら0に
		if (m_Col.a < 0.01f)
		{
			m_Col.a = 0;
		}
		//色を設定
		CScene3D::SetColor(m_Col);

		//設定処理
		CScene3D::SetSize(m_fHeight, m_fWidth);
		CScene3D::SetPos(m_pos);
		CScene3D::SetRot(m_rot);
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
// お金の更新処理
//=============================================================================
void CEffect3D::UpdateSalt(void)
{
	//自分用の死亡フラグ変数
	bool bDestroy = false;

	m_nCntTimer++;
	m_DrawType = 0;

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
		CScene3D::SetColor(m_Col);

		//設定処理
		CScene3D::SetSize(m_fHeight, m_fWidth);
		CScene3D::SetPos(m_pos);
		CScene3D::SetRot(m_rot);
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
// お金の更新処理
//=============================================================================
void CEffect3D::UpdateUI(void)
{
	//自分用の死亡フラグ変数
	bool bDestroy = false;

	m_nCntTimer++;
	m_DrawType = 0;

	/*D3DXVECTOR3 Parpos1 = D3DXVECTOR3(sinf(fAngle) * fLength1, 0.0f, cosf(fAngle) * fLength1);
	D3DXVECTOR3 Parpos2 = D3DXVECTOR3(sinf(fAngle) * fLength2, 0.0f, cosf(fAngle) * fLength2);*/

	if (m_nLife > 0)
	{
		m_nLife--;

		//位置を更新		
		m_pos += m_move;
		m_rot.x += 0.2f;

		//徐々に透明にしていく
		m_Col.a = m_Col.a - m_fAlpha;

		//一定以下になったら0に
		if (m_Col.a < 0.01f)
		{
			m_Col.a = 0;
		}
		//色を設定
		CScene3D::SetColor(m_Col);

		//設定処理
		CScene3D::SetSize(m_fHeight, m_fWidth);
		CScene3D::SetPos(m_pos);
		CScene3D::SetRot(m_rot);
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
// お金の更新処理
//=============================================================================
void CEffect3D::UpdateConfetti(void)
{
	//自分用の死亡フラグ変数
	bool bDestroy = false;

	m_nCntTimer++;
	m_DrawType = 1;

	/*D3DXVECTOR3 Parpos1 = D3DXVECTOR3(sinf(fAngle) * fLength1, 0.0f, cosf(fAngle) * fLength1);
	D3DXVECTOR3 Parpos2 = D3DXVECTOR3(sinf(fAngle) * fLength2, 0.0f, cosf(fAngle) * fLength2);*/

	if (m_nLife > 0)
	{
		m_nLife--;

		//重力
		m_move.y -= cosf(D3DX_PI * 0) * 0.000009f;

		//位置を更新		
		m_pos += m_move;
		m_rot.x += 0.06f;

		//徐々に透明にしていく
		//m_Col.a = m_Col.a - m_fAlpha;

		//一定以下になったら0に
		if (m_Col.a < 0.01f)
		{
			m_Col.a = 0;
		}
		//色を設定
		CScene3D::SetColor(m_Col);

		//設定処理
		CScene3D::SetSize(m_fHeight, m_fWidth);
		CScene3D::SetPos(m_pos);
		CScene3D::SetRot(m_rot);
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
void CEffect3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	if (m_DrawType == 0)
	{

		// αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		CScene3D::Draw();

		// αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	else if (m_DrawType == 1)
	{
		CScene3D::Draw();
	}

	//ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}