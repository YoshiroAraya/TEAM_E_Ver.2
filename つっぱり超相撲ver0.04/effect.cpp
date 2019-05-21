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

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURENAME000			 "data\\TEXTURE\\EFFECT\\particle000.jpg"		//テクスチャのファイル名
#define TEXTURENAME001			 "data\\TEXTURE\\EFFECT\\effect002.png"		//テクスチャのファイル名
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
LPDIRECT3DTEXTURE9			CEffect::m_pTexture[EFFECTTEX_MAX] = {};

//--------------------------------------------
//エフェクトクラス コンストラクタ
//--------------------------------------------
CEffect::CEffect()
{
	m_pos = D3DXVECTOR3(0,0,0);						// 位置
	m_move = D3DXVECTOR3(0, 0, 0);					// 移動量
	m_posold = D3DXVECTOR3(0, 0, 0);				// 前回の位置


	m_TexType = EFFECTTEX_NORMAL000;
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
	float fWidth, float fHeight, int nNumMax, int nLife, EFFECTTEX TexType)
{
	CEffect *pEffect;

	pEffect = new CEffect;

	pEffect->m_pos = pos;
	pEffect->m_move = move;
	pEffect->m_Col = col;
	pEffect->m_fHeight = fHeight;
	pEffect->m_fWidth = fWidth;
	pEffect->m_nNumMax = nNumMax;
	pEffect->m_nLife = nLife;
	pEffect->m_TexType = TexType;

	pEffect->Init();

	return pEffect;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEffect::Init(void)
{
	//デバイスを取得
	//CRenderer *pRenderer = CManager::GetRenderer();
	//LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	m_pBillBoard = CBillboard::Create(m_pos, m_fWidth, m_fHeight);
	m_pBillBoard->BindTexture(m_pTexture[0]);
	m_pBillBoard->SetPosition(m_pos);
	m_pBillBoard->SetCol(m_Col);

	//オブジェクト種類の設定
	//if (m_TexType == EFFECTTEX_NORMAL000 || m_TexType == EFFECTTEX_SMOKE)
	//{
	//	CScene::SetObjType(CScene::OBJTYPE_EFFECTADD);
	//	m_pBillBoard->SetObjType(CScene::OBJTYPE_EFFECTADD);
	//}
	//else
	//{
	//	CScene::SetObjType(CScene::OBJTYPE_EFFECT);
	//	m_pBillBoard->SetObjType(CScene::OBJTYPE_EFFECT);
	//}

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
	if (m_pBillBoard != NULL)
	{
		m_pBillBoard->Uninit();
		m_pBillBoard = NULL;
	}
	//自分を消す(破棄)
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEffect::Update(void)
{
	m_nCntTimer++;

	if (m_nLife > 0)
	{
		m_nLife--;

		//重力
		//m_move.y -= cosf(D3DX_PI * 0) * 0.3f;
		//位置を更新
		//m_pos = m_pBillBoard->GetPosition();
		m_pos.y += m_move.y;
		//地面ではねる
		if (m_pos.y < 0.0f)
		{
		//	m_move *= -1;
		}

		//徐々に透明にしていく
		m_Col.a = m_Col.a - m_fAlpha;
		//一定以下になったら0に
		if (m_Col.a < 0.01f)
		{
			m_Col.a = 0;
		}
		//色を設定
		m_pBillBoard->SetCol(m_Col);

		//設定処理
		m_pBillBoard->SetBillboard(m_pos, m_fHeight, m_fWidth);
	}
	else if (m_nLife == 0)
	{
		//自分を消す(破棄)
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEffect::Draw(void)
{

}
//=============================================================================
// テクスチャロード処理
//=============================================================================
HRESULT CEffect::Load(void)
{
	//デバイスを取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	for (int nCnt = 0; nCnt < EFFECTTEX_MAX; nCnt++)
	{
		m_pTexture[nCnt] = NULL;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME000, &m_pTexture[0]);
	//D3DXCreateTextureFromFile(pDevice, TEXTURENAME001, &m_pTexture[1]);
	//D3DXCreateTextureFromFile(pDevice, TEXTURENAME002, &m_pTexture[2]);
	//D3DXCreateTextureFromFile(pDevice, TEXTURENAME003, &m_pTexture[3]);
	//D3DXCreateTextureFromFile(pDevice, TEXTURENAME004, &m_pTexture[4]);

	return S_OK;
}
//=============================================================================
// テクスチャ破棄処理
//=============================================================================
void CEffect::UnLoad(void)
{
	for (int nCnt = 0; nCnt < EFFECTTEX_MAX; nCnt++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}
