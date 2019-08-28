//=============================================================================
//
// モデル処理 [model.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "model.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//--------------------------------------------
//モデルクラス コンストラクタ
//--------------------------------------------
CModel::CModel()
{
	m_pMeshModel = NULL;		//メッシュ情報へのポインタ
	m_pBuffMatModel = NULL;		//マテリアル情報へのポインタ
	m_pTextureModel = NULL;		//テクスチャへのポインタ
	m_nNumMatModel = 0;			//マテリアル情報の数
	m_aModel = {};
	m_bShadow = false;
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	m_pParent = NULL;			//親モデルへのポインタ
}

//--------------------------------------------
//モデルクラス デストラクタ
//--------------------------------------------
CModel::~CModel()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CModel * CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CModel *pModel;

	pModel = new CModel;

	pModel->m_aModel.pos = pos;
	pModel->m_aModel.rot = rot;

	pModel->Init();

	return pModel;
}


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModel::Init(void)
{

	m_VtxMinModel = D3DXVECTOR3(10000, 0, 10000);
	m_VtxMaxModel = D3DXVECTOR3(-10000, 0, -10000);
	m_aModel.nType = 0;
	m_aModel.bUse = false;
	m_bShadow = false;


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModel::Uninit(void)
{
	//ポインタのNULLチェック (家)
	if (m_pTextureModel != NULL)
	{
		for (int nCnt = 0; nCnt < (int)m_nNumMatModel; nCnt++)
		{
			//ポインタ内のポインタのNULLチェック (家具)
			if (m_pTextureModel[nCnt] != NULL)
			{
				//テクスチャ破棄
				m_pTextureModel[nCnt]->Release();
				m_pTextureModel[nCnt] = NULL;
			}
		}
		//メモリを開放 (解体)
		delete[] m_pTextureModel;
		//NULLを入れる (更地)
		m_pTextureModel = NULL;
	}

	if (m_pMeshModel != NULL)
	{
		//メッシュ破棄
		m_pMeshModel = NULL;
	}

	if (m_pBuffMatModel != NULL)
	{
		//マテリアル破棄
		m_pBuffMatModel = NULL;
	}

	if (m_pParent != NULL)
	{
		//マテリアル破棄
		m_pParent = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CModel::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CModel::Draw(void)
{
	//デバイスを取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DXMATRIX mtxParent;

	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_aModel.rot.y, m_aModel.rot.x, m_aModel.rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_aModel.pos.x, m_aModel.pos.y, m_aModel.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//親のマトリックスを取得
	if (m_pParent != NULL)
	{
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{	//Drawで呼ばれたとき最新のmtxをpDeviceから取得
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}
	//親の情報を入れる	掛け合わせている
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);
	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMatModel; nCntMat++)
	{
		//マテリアルの設定
		if (pMat != NULL)
		{
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				if (m_bShadow == false)
				{
					// テクスチャ
					pDevice->SetTexture(0, m_pTextureModel[nCntMat]);
				}
				else
				{
					// テクスチャ
					pDevice->SetTexture(0, NULL);
				}
			}
			else
			{	// テクスチャを使っていない
				pDevice->SetTexture(0, NULL);
			}
		}
		//オブジェクト(パーツ)の描画
		m_pMeshModel->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//*****************************************************************************
//モデル位置設定処理
//*****************************************************************************
void CModel::Setpos(D3DXVECTOR3 pos)
{
	m_aModel.pos = pos;
}
//*****************************************************************************
//モデル回転設定処理
//*****************************************************************************
void CModel::Setrot(D3DXVECTOR3 rot)
{
	m_aModel.rot = rot;
}

//*****************************************************************************
//モデルポインタ処理
//*****************************************************************************
CModel::MODEL *CModel::GetModel(void)
{
	return &m_aModel;
}

//=============================================================================
// モデルの設定処理
//=============================================================================
void CModel::SetModel(D3DXVECTOR3 pos, int nType)
{
#if 1
	int nNumVtex;	//頂点数
	DWORD sizeFvF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;	//頂点バッファへのポインタ
	D3DXVECTOR3 vtx;

	//頂点数を取得
	nNumVtex = m_pMeshModel->GetNumVertices();
	//頂点フォーマットのサイズを取得
	sizeFvF = D3DXGetFVFVertexSize(m_pMeshModel->GetFVF());

	//頂点　ロック
	m_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
	for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
	{	//頂点座標の代入
		vtx = *(D3DXVECTOR3*)pVtxBuff;
		if (m_VtxMinModel.x > vtx.x)
		{
			m_VtxMinModel.x = vtx.x;
		}
		if (m_VtxMinModel.y > vtx.y)
		{
			m_VtxMinModel.y = vtx.y;
		}
		if (m_VtxMinModel.z > vtx.z)
		{
			m_VtxMinModel.z = vtx.z;
		}

		if (m_VtxMaxModel.x < vtx.x)
		{
			m_VtxMaxModel.x = vtx.x;
		}
		if (m_VtxMaxModel.y < vtx.y)
		{
			m_VtxMaxModel.y = vtx.y;
		}
		if (m_VtxMaxModel.z < vtx.z)
		{
			m_VtxMaxModel.z = vtx.z;
		}
		pVtxBuff += sizeFvF;

		//頂点バッファをアンロック
		m_pMeshModel->UnlockVertexBuffer();
	}

#endif
}

//=============================================================================
// マテリアル割り当て処理
//=============================================================================
void CModel::BindModel(DWORD nNumMat, LPD3DXMESH Mesh, LPD3DXBUFFER Buff)
{
	//デバイスを取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	m_pBuffMatModel = Buff;
	m_nNumMatModel = nNumMat;
	m_pMeshModel = Mesh;

	//マテリアル情報からテクスチャの取得
	pMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();
	//テクスチャをマテリアルの数分動的確保
	m_pTextureModel = new LPDIRECT3DTEXTURE9[m_nNumMatModel];
	//マテリアルの数回す
	for (int nCntMatTex = 0; nCntMatTex < (int)m_nNumMatModel; nCntMatTex++)
	{
		//NULLを入れる 中身を空に
		m_pTextureModel[nCntMatTex] = NULL;

		if (pMat[nCntMatTex].pTextureFilename != NULL)
		{
			// テクスチャの設定
			D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
				pMat[nCntMatTex].pTextureFilename,	// ファイルの名前
				&m_pTextureModel[nCntMatTex]);		// テクスチャへのポインタ
		}
	}
}

//=============================================================================
// 親の設定処理
//=============================================================================
void CModel::SetParent(CModel * pModel)
{
	m_pParent = pModel;
}
//=============================================================================
// ワールドマトリックスの取得処理
//=============================================================================
D3DXMATRIX CModel::GetMtxWorld(void)
{
	return m_mtxWorld;
}
//=============================================================================
// ワールドマトリックスの設定処理
//=============================================================================
void CModel::SetMtxWorld(D3DXMATRIX MtxWorld)
{
	m_mtxWorld = MtxWorld;
}

//=============================================================================
// 回転の設定処理
//=============================================================================
D3DXVECTOR3 CModel::Getrot(void)
{
	return m_aModel.rot;
}
