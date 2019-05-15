//=============================================================================
//
// 3Dポリゴン処理 [scene3D.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "scene3D.h"
#include "main.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "player.h"
#include "game.h"

//=============================================================================
// 3Dポリゴンクラスのコンストラクタ
//=============================================================================
CScene3D::CScene3D(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 値をクリア
	m_pTexture = NULL;						// テクスチャへのポインタ
	m_pVtxBuff = NULL;						// 頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き

	//for (int nCntNor = 0; nCntNor < 2; nCntNor++)
	//{
	//	m_aNor[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 法線
	//}

	for (int nCntNor = 0; nCntNor < NUM_VTX; nCntNor++)
	{
		m_aVec[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 法線
		m_aPos[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 法線
		m_aNor[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 法線
	}
	

	m_fSize = 0.0f;							// 大きさ
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene3D::~CScene3D()
{
}

//=============================================================================
// オブジェクトの生成処理
//=============================================================================
CScene3D *CScene3D::Create(D3DXVECTOR3 pos)
{
	CScene3D *pScene3D = NULL;

	if (pScene3D == NULL)
	{
		// オブジェクトクラスの生成
		pScene3D = new CScene3D;

		if (pScene3D != NULL)
		{
			pScene3D->Init(pos);
		}
	}

	return pScene3D;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene3D::Init(D3DXVECTOR3 pos)
{
	// ポリゴンの情報を設定
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//for (int nCntNor = 0; nCntNor < 2; nCntNor++)
	//{// 法線
	//	m_aNor[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//}

	for (int nCntNor = 0; nCntNor < NUM_VTX; nCntNor++)
	{
		m_aVec[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 法線
		m_aPos[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 法線
		m_aNor[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 法線
	}

	//m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ポリゴンの位置を設定
	m_pos = pos;

	m_fSize = GROUND_SIZE ;

	// オブジェクトの種類の設定
	//SetObjType(CScene::OBJTYPE_SCENE3D);

	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, GROUND_TEXTURE_NAME, &m_pTexture);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fSize, 0.0f, m_fSize);
	pVtx[1].pos = D3DXVECTOR3(m_fSize, 0.0f, m_fSize);
	pVtx[2].pos = D3DXVECTOR3(-m_fSize, 0.0f, -m_fSize);
	pVtx[3].pos = D3DXVECTOR3(m_fSize, 0.0f, -m_fSize);

	//pVtx[0].pos = D3DXVECTOR3(-m_fSize, pVtx[0].pos.y, m_fSize);
	//pVtx[1].pos = D3DXVECTOR3(pVtx[1].pos.x, pVtx[1].pos.y, m_fSize);
	//pVtx[2].pos = D3DXVECTOR3(-m_fSize, pVtx[2].pos.y, pVtx[2].pos.z);
	//pVtx[3].pos = D3DXVECTOR3(pVtx[3].pos.x, pVtx[3].pos.y, pVtx[3].pos.z);

	// 法線の設定
	// ベクトルを求める
	m_aVec[0] = pVtx[2].pos - pVtx[3].pos;
	m_aVec[1] = pVtx[1].pos - pVtx[3].pos;
	m_aVec[2] = pVtx[1].pos - pVtx[0].pos;
	m_aVec[3] = pVtx[2].pos - pVtx[0].pos;

	// 外積を使って法線を求める
	D3DXVec3Cross(&m_aNor[0], &m_aVec[0], &m_aVec[1]);
	D3DXVec3Cross(&m_aNor[1], &m_aVec[2], &m_aVec[3]);

	// 正規化する
	D3DXVec3Normalize(&m_aNor[0], &m_aNor[0]);
	D3DXVec3Normalize(&m_aNor[1], &m_aNor[1]);

	// 法線の設定
	/*pVtx[0].nor = (m_aNor[0] + m_aNor[1]) / 2;
	pVtx[1].nor = m_aNor[1];
	pVtx[2].nor = m_aNor[0];
	pVtx[3].nor = (m_aNor[0] + m_aNor[1]) / 2;*/

	pVtx[0].nor = m_aNor[1];
	pVtx[1].nor = (m_aNor[0] + m_aNor[1]) / 2;
	pVtx[2].nor = (m_aNor[0] + m_aNor[1]) / 2;
	pVtx[3].nor = m_aNor[0];
	
	/*pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);*/

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	for (int nCntPos = 0; nCntPos < NUM_VTX; nCntPos++)
	{
		m_aPos[nCntPos] = pVtx[nCntPos].pos;
		m_aNor[nCntPos] = pVtx[nCntPos].nor;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScene3D::Uninit(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクトの解放
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScene3D::Update(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

#ifdef _DEBUG
	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (pInputKeyboard->GetPress(DIK_I) == true)
	{
		pVtx[1].pos.y += 1.0f;
		pVtx[2].pos.y += 1.0f;
	}
	if (pInputKeyboard->GetPress(DIK_K) == true)
	{
		pVtx[1].pos.y -= 1.0f;
		pVtx[2].pos.y -= 1.0f;
	}

	// 頂点座標の設定
	//pVtx[0].pos = D3DXVECTOR3(-m_fSize, pVtx[0].pos.y, m_fSize);
	//pVtx[1].pos = D3DXVECTOR3(m_fSize, pVtx[1].pos.y, m_fSize);
	//pVtx[2].pos = D3DXVECTOR3(-m_fSize, pVtx[2].pos.y, -m_fSize);
	//pVtx[3].pos = D3DXVECTOR3(m_fSize, pVtx[3].pos.y, -m_fSize);

	pVtx[0].pos = D3DXVECTOR3(-m_fSize, pVtx[0].pos.y, -m_fSize);
	pVtx[1].pos = D3DXVECTOR3(-m_fSize, pVtx[1].pos.y, m_fSize);
	pVtx[2].pos = D3DXVECTOR3(m_fSize, pVtx[2].pos.y, -m_fSize);
	pVtx[3].pos = D3DXVECTOR3(m_fSize, pVtx[3].pos.y, m_fSize);
	//pVtx[0].pos = D3DXVECTOR3(-m_fSize, pVtx[0].pos.y, m_fSize);
	//pVtx[1].pos = D3DXVECTOR3(pVtx[1].pos.x, pVtx[1].pos.y, m_fSize);
	//pVtx[2].pos = D3DXVECTOR3(-m_fSize, pVtx[2].pos.y, pVtx[2].pos.z);
	//pVtx[3].pos = D3DXVECTOR3(pVtx[3].pos.x, pVtx[3].pos.y, pVtx[3].pos.z);

	// 法線の設定
	// ベクトルを求める
	m_aVec[0] = pVtx[2].pos - pVtx[3].pos;
	m_aVec[1] = pVtx[1].pos - pVtx[3].pos;
	m_aVec[2] = pVtx[1].pos - pVtx[0].pos;
	m_aVec[3] = pVtx[2].pos - pVtx[0].pos;

	// 外積を使って法線を求める
	D3DXVec3Cross(&m_aNor[0], &m_aVec[0], &m_aVec[1]);
	D3DXVec3Cross(&m_aNor[1], &m_aVec[2], &m_aVec[3]);

	// 正規化する
	D3DXVec3Normalize(&m_aNor[0], &m_aNor[0]);
	D3DXVec3Normalize(&m_aNor[1], &m_aNor[1]);

	// 法線の設定
	//pVtx[0].nor = (m_aNor[0] + m_aNor[1]) / 2;
	//pVtx[1].nor = m_aNor[1];
	//pVtx[2].nor = m_aNor[0];
	//pVtx[3].nor = (m_aNor[0] + m_aNor[1]) / 2;

	pVtx[0].nor = m_aNor[1];
	pVtx[1].nor = (m_aNor[0] + m_aNor[1]) / 2;
	pVtx[2].nor = (m_aNor[0] + m_aNor[1]) / 2;
	pVtx[3].nor = m_aNor[0];

	for (int nCntPos = 0; nCntPos < NUM_VTX; nCntPos++)
	{
		m_aPos[nCntPos] = pVtx[nCntPos].pos;
		m_aNor[nCntPos] = pVtx[nCntPos].nor;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	if (pInputKeyboard->GetPress(DIK_4) == true)
	{
		m_rot.y += 0.1f;
	}

	//CDebugProc::Print("cfccfccfc", "3DPos    : x", m_pos.x, "f", "   y", m_pos.y, "f", "   z", m_pos.z, "f");
	for (int nCntNor = 0; nCntNor < 2; nCntNor++)
	{
		CDebugProc::Print("cncfcfcfc", "m_aNor[", nCntNor, "] : (", m_aNor[nCntNor].x,", ", m_aNor[nCntNor].y, ", ", m_aNor[nCntNor].z, ")");
	}

	for (int nCntNor = 0; nCntNor < NUM_VTX; nCntNor++)
	{
		CDebugProc::Print("f", pVtx[nCntNor].pos.y);
	}


	if (pInputKeyboard->GetTrigger(DIK_2) == true)
	{
		Uninit();
	}
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CScene3D::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// デバイスを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

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

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 法線を取得
//=============================================================================
D3DXVECTOR3 CScene3D::GetNor(int nIdx)
{
	D3DXVECTOR3 vecA[2];
	return m_aNor[nIdx];
}

//=============================================================================
// 高さを取得
//============================================================================
float CScene3D::GetHeight(D3DXVECTOR3 pos)
{
	// プレイヤーを取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();

	//// 右側の三角にいるかどうかを取得
	//bool bRTriangle;
	//bRTriangle = pPlayer->GetRTriangle();

	//if (bRTriangle == true)
	//{
	//	pos.y = (((m_aNor[0].x * (pos.x - m_aPos[0].x) + m_aNor[0].z * (pos.z - m_aPos[0].z)) / -m_aNor[0].y) + m_aPos[0].y);
	//}
	//else
	//{
	//	pos.y = (((m_aNor[3].x * (pos.x - m_aPos[3].x) + m_aNor[3].z * (pos.z - m_aPos[3].z)) / -m_aNor[3].y) + m_aPos[3].y);
	//}

	
	return pos.y;
}