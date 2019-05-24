//=============================================================================
//
// 影の処理
// Author : 長山拓実
//
//=============================================================================
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "player.h"
#include "mask.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_SHADOW			"data\\MODEL\\shadow000.x"	//テクスチャのファイル名
LPD3DXMESH CShadow::m_pMesh = NULL;			// メッシュ情報（頂点情報）へのポインタ
LPD3DXBUFFER CShadow::m_pBuffMat = NULL;	// マテリアル情報へのポインタ
DWORD CShadow::m_nNumMat = 0;				// マテリアル情報の数

//--------------------------------------------
//静的メンバ変数宣言
//--------------------------------------------
LPDIRECT3DTEXTURE9			CShadow::m_pTexture = NULL;

//--------------------------------------------
//シーン3Dクラス コンストラクタ
//--------------------------------------------
CShadow::CShadow()
{
	m_pos = D3DXVECTOR3(0,0,0);			//位置
	m_rot = D3DXVECTOR3(0, 0, 0);		//向き
	D3DXMatrixIdentity(&m_mtxWorld);	//ワールドマトリックス
}

//--------------------------------------------
//シーン3Dクラス デストラクタ
//--------------------------------------------
CShadow::~CShadow()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CShadow * CShadow::Create(D3DXVECTOR3 pos)
{
	CShadow *pShadow = NULL;

	if (pShadow == NULL)
	{
		// オブジェクトクラスの生成
		pShadow = new CShadow;

		if (pShadow != NULL)
		{
			pShadow->m_pos = pos;
			pShadow->BindModel(m_pBuffMat, m_nNumMat, m_pMesh);
			pShadow->Init();
		}
	}

	return pShadow;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CShadow::Init(void)
{
	// 2Dオブジェクト初期化処理
	CSceneX::Init(m_pos);

	// オブジェクトの種類の設定
	SetObjType(CScene::OBJTYPE_SHADOW);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CShadow::Uninit(void)
{
	// 2Dオブジェクト終了処理
	CSceneX::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CShadow::Update(void)
{
	CSceneX::SetPosition(m_pos);
}

//=============================================================================
// 描画処理
//=============================================================================
void CShadow::Draw(void)
{
	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	pDevice->SetRenderState(D3DRS_STENCILENABLE, true);	// ステンシルテストを有効にする
	//pDevice->SetRenderState(D3DRS_ZENABLE, false);	// Zテストを有効にする

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);	// 見えていない状態にする

	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);			// ステンシルの対象となるものを全員有効にする
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);						// ステンシルテスト→合格 / Zテスト合格
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);						// ステンシルテスト→合格 / Zテスト不合格
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);						// ステンシルテスト→不合格

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);				// カリング表

	// 描画処理
	CSceneX::Draw();

	pDevice->SetRenderState(D3DRS_STENCILREF, 1);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);			// ステンシルの対象となるものを1になっているものを有効にする
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);						// ステンシルテスト→合格 / Zテスト合格
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_ZERO);						// ステンシルテスト→合格 / Zテスト不合格
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);						// ステンシルテスト→不合格

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリング裏

	// 描画処理
	CSceneX::Draw();

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000f);	// 見えている状態にする

	pDevice->SetRenderState(D3DRS_STENCILREF, 2);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);			// ステンシルの対象となるものを2になっているものを有効にする
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);						// ステンシルテスト→合格 / Zテスト合格
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);						// ステンシルテスト→合格 / Zテスト不合格
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);						// ステンシルテスト→不合格

	// マスクの取得
	CMask *pMask;
	pMask = CManager::GetMask();

	pMask->Draw();

	pDevice->SetRenderState(D3DRS_STENCILENABLE, false);	// ステンシルテストを無効にする
	//pDevice->SetRenderState(D3DRS_ZENABLE, true);	// Zテストを有効にする

}

//=============================================================================
// 位置設定処理
//=============================================================================
void CShadow::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// テクスチャロード処理
//=============================================================================
HRESULT CShadow::Load(void)
{
	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_SHADOW,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	return S_OK;
}
//=============================================================================
// テクスチャ破棄処理
//=============================================================================
void CShadow::UnLoad(void)
{
	// メッシュの開放
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// マテリアルの開放
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
}
