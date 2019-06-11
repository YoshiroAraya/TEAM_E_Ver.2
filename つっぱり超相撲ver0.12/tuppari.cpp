//=============================================================================
//
// つっぱり処理 [tuppari.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "tuppari.h"
#include "input.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TUPPARI_SIZE	(22.0f)	// サイズの調節

//=============================================================================
// つっぱりクラスのコンストラクタ
//=============================================================================
CTuppari::CTuppari(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 値をクリア
	m_nNumMat = 0;							// マテリアル情報の数
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
}

//=============================================================================
// デストラクタ
//=============================================================================
CTuppari::~CTuppari()
{
}

//=============================================================================
// オブジェクトの生成処理
//=============================================================================
CTuppari *CTuppari::Create(D3DXVECTOR3 pos)
{
	CTuppari *pSceneX = NULL;

	if (pSceneX == NULL)
	{
		// オブジェクトクラスの生成
		pSceneX = new CTuppari;

		if (pSceneX != NULL)
		{
			pSceneX->Init(pos);
		}
	}

	return pSceneX;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTuppari::Init(D3DXVECTOR3 pos)
{
	// オブジェクトの種類の設定
	//SetObjType(CScene::OBJTYPE_SCENEX);

	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// 位置・向きの初期設定
	m_pos = pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 頂点座標の設定
	SetVtx();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTuppari::Uninit(void)
{
	// オブジェクトの解放
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTuppari::Update(void)
{


#ifdef _DEBUG
	//CDebugProc::Print("cfccfccfc", "ModelPos : x", m_pos.x, "f", "   y", m_pos.y, "f", "  z", m_pos.z, "f");
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CTuppari::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;				// 計算用マトリックス

	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

}

//=============================================================================
// 頂点座標の設定処理
//=============================================================================
void CTuppari::SetVtx(void)
{
	// 頂点数を取得

	// 頂点の最小値と最大値を代入
	//m_VtxMin = D3DXVECTOR3(10000, 10000, 10000);
	//m_VtxMax = D3DXVECTOR3(-10000, -10000, -10000);

}

//=============================================================================
// ブロックとの当たり判定処理
//=============================================================================
bool CTuppari::Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 radius)
{
	bool bLand = false;	// 乗っていない状態

	D3DXVECTOR3 ScaleVtxMax;
	D3DXVECTOR3 ScaleVtxMin;



	if (pos->z - radius.z <= m_pos.z + TUPPARI_SIZE && pos->z + radius.z >= m_pos.z - TUPPARI_SIZE)
	{// zの範囲の中
		if (pos->x - radius.x <= m_pos.x + TUPPARI_SIZE && pos->x + radius.x >= m_pos.x - TUPPARI_SIZE)
		{// xの範囲の中
			bLand = true;
		}
	}


	return bLand;	// ブロックに乗っているかどうかを返す
}

//=============================================================================
// 位置の取得
//=============================================================================
D3DXVECTOR3 CTuppari::GetPosition(void)
{
	return m_pos;
}

//=============================================================================
// 位置の設定
//=============================================================================
void CTuppari::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// 位置の取得
//=============================================================================
D3DXVECTOR3 CTuppari::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// 位置の設定
//=============================================================================
void CTuppari::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// 頂点情報の最大値の取得
//=============================================================================
D3DXVECTOR3 CTuppari::GetVtxMax(void)
{
	return m_VtxMax;
}

//=============================================================================
// 頂点情報の最大値の設定
//=============================================================================
void CTuppari::SetVtxMax(D3DXVECTOR3 VtxMax)
{
	m_VtxMax = VtxMax;
}

//=============================================================================
// 頂点情報の最小値の取得
//=============================================================================
D3DXVECTOR3 CTuppari::GetVtxMin(void)
{
	return m_VtxMin;
}

//=============================================================================
// 頂点情報の最小値の設定
//=============================================================================
void CTuppari::SetVtxMin(D3DXVECTOR3 VtxMin)
{
	m_VtxMin = VtxMin;
}