//=============================================================================
//
// メッシュフィールドの処理 [meshField.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "meshField.h"
#include "main.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"

//=============================================================================
// グローバル変数宣言
//=============================================================================
float g_aHeight[POLYGON_Z + 1][POLYGON_X + 1];

//=============================================================================
// メッシュフィールドのコンストラクタ
//=============================================================================
CMeshField::CMeshField(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 値をクリア
	m_pTexture = NULL;	// テクスチャへのポインタ
	m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	m_pIdxBuff = NULL;
	m_nNumVerTex = 0;
	m_nNumIndex = 0;
	m_nNumPolygon = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bRand = false;

	for (int nCntNor = 0; nCntNor < NUM_POLYGON; nCntNor++)
	{
		m_aNor[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CMeshField::~CMeshField()
{
}

//=============================================================================
// オブジェクトの生成処理
//=============================================================================
CMeshField *CMeshField::Create(D3DXVECTOR3 pos)
{
	CMeshField *pMeshField = NULL;

	if (pMeshField == NULL)
	{
		// オブジェクトクラスの生成
		pMeshField = new CMeshField;

		if (pMeshField != NULL)
		{
			pMeshField->Init(pos);
		}
	}

	return pMeshField;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMeshField::Init(D3DXVECTOR3 pos)
{
	m_pos = pos;	// 位置の設定

	// 値の初期化
	m_pTexture = NULL;	// テクスチャへのポインタ
	m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	m_pIdxBuff = NULL;
	m_nNumVerTex = 0;
	m_nNumIndex = 0;
	m_nNumPolygon = 0;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bRand = false;

	for (int nCntZ = 0; nCntZ < POLYGON_Z + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < POLYGON_X + 1; nCntX++)
		{
			g_aHeight[nCntZ][nCntX] = 0.0f;
		}
	}

	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{// デバイスの取得
		pDevice = pRenderer->GetDevice();
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, MESHFIELD_TEXTURE_NAME, &m_pTexture);

	// 頂点情報の作成
	int nVtxCounter = 0;
	int nIdxCounter = 0;

	int nCntVtxZ;
	int nCntVtxX;
	int nCntIdxZ;
	int nCntIdxX;

	// 頂点数
	m_nNumVerTex = (POLYGON_X + 1) * (POLYGON_Z + 1);

	// インデックス数
	m_nNumIndex = (POLYGON_X + 1) * (POLYGON_Z + 1)
		+ (2 * (POLYGON_Z - 1))
		+ (POLYGON_X + 1) * (POLYGON_Z - 1);

	// ポリゴン数
	m_nNumPolygon = m_nNumIndex - 2;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVerTex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,										// 16ビットのデータを確保
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntVtxZ = 0; nCntVtxZ < POLYGON_Z + 1; nCntVtxZ++)
	{
		for (nCntVtxX = 0; nCntVtxX < POLYGON_X + 1; nCntVtxX++)
		{
			// 頂点座標の設定
			pVtx[(nCntVtxZ + nCntVtxX) + nVtxCounter].pos = D3DXVECTOR3(-MESHFIELD_SIZE + (nCntVtxX * MESHFIELD_SIZE), 0.0f, MESHFIELD_SIZE - (nCntVtxZ * MESHFIELD_SIZE));
			// 法線の設定
			pVtx[(nCntVtxZ + nCntVtxX) + nVtxCounter].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// 頂点カラーの設定
			pVtx[(nCntVtxZ + nCntVtxX) + nVtxCounter].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// テクスチャ座標の設定
			pVtx[(nCntVtxZ + nCntVtxX) + nVtxCounter].tex = 
				D3DXVECTOR2(0.0f + (nCntVtxX /** (10.0f / POLYGON_X)*/), 0.0f + (nCntVtxZ /** (10.0f / POLYGON_Z)*/));
		}
		nVtxCounter += POLYGON_X;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

#if(1)
	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntPolygon = 0;

	int nData = 0;
	int nData2 = 0;
	int nData3 = 0;

	for (int nCntZ = 0; nCntZ < POLYGON_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX < POLYGON_X; nCntX++)
		{
			D3DXVECTOR3 *pPos0, *pPos1, *pPos2, *pPos3;
			D3DXVECTOR3 vec0, vec1, vec2;
			D3DXVECTOR3 nor;

			// 一方のポリゴンの２つのベクトルから法線を算出
			pPos0 = &pVtx[nCntX + nCntZ + nCntPolygon].pos;
			pPos1 = &pVtx[(nCntX + nCntZ + nCntPolygon) + (POLYGON_X + 1)].pos;
			pPos2 = &pVtx[(nCntX + nCntZ + nCntPolygon) + (POLYGON_X + 1) + 1].pos;
			pPos3 = &pVtx[(nCntX + nCntZ + nCntPolygon) + 1].pos;

			vec0 = *pPos1 - *pPos0;
			vec1 = *pPos2 - *pPos0;
			vec2 = *pPos3 - *pPos0;

			// 外積を使って各ポリゴンの法線を求める
			D3DXVec3Cross(&nor, &vec1, &vec0);
			// 正規化する
			D3DXVec3Normalize(&nor, &nor);
			// 法線を保存
			m_aNor[(nCntZ * POLYGON_Z) + (nCntX * POLYGON_X) + nCntPolygon] = nor;

			// 外積を使って各ポリゴンの法線を求める
			D3DXVec3Cross(&nor, &vec2, &vec1);
			// 正規化する
			D3DXVec3Normalize(&nor, &nor);
			// 法線を保存
			m_aNor[(nCntZ * POLYGON_Z) + (nCntX * POLYGON_X) + nCntPolygon + 1] = nor;
		}

		nCntPolygon += POLYGON_X;
	}

	for (int nCntPolygonZ = 0; nCntPolygonZ < POLYGON_Z + 1; nCntPolygonZ++)
	{
		for (int nCntPolygonX = 0; nCntPolygonX < POLYGON_X + 1; nCntPolygonX++)
		{
			if (nCntPolygonZ == 0)
			{// 上端
				if (nCntPolygonX == 0)
				{// 左端
					pVtx[0].nor = (m_aNor[0] + m_aNor[1]) / 2;
				}
				else if (nCntPolygonX == POLYGON_X)
				{// 右端
					pVtx[POLYGON_X].nor = m_aNor[POLYGON_X + (POLYGON_X - 1)];
				}
				else
				{// 上端の真ん中
					pVtx[nCntPolygonX].nor =
						(m_aNor[(nCntPolygonX * 2) - 1] + m_aNor[((nCntPolygonX * 2) - 1) + 1] + m_aNor[((nCntPolygonX * 2) - 1) + 2]) / 3;
				}
			}
			else if (nCntPolygonZ == POLYGON_Z)
			{// 下端
				if (nCntPolygonX == 0)
				{// 左端
					pVtx[POLYGON_Z * (POLYGON_X + 1)].nor = m_aNor[2 * (POLYGON_X * (POLYGON_Z - 1))];
				}
				else if (nCntPolygonX == POLYGON_X)
				{// 右端
					pVtx[(POLYGON_X * POLYGON_Z) + (POLYGON_X + POLYGON_Z)].nor =
						(m_aNor[(POLYGON_X * (POLYGON_X * (POLYGON_Z - 1))) + (2 * (POLYGON_X - 1))] +
							m_aNor[((2 * (POLYGON_X * (POLYGON_Z - 1))) + (2 * (POLYGON_X - 1))) + 1]) / 2;
				}
				else
				{// 下端の真ん中
					pVtx[(POLYGON_Z * (POLYGON_X + 1)) + nCntPolygonX].nor =
						(m_aNor[(POLYGON_Z - 1) * (POLYGON_X * 2) + (nCntPolygonX * 2) - 2] +
							m_aNor[((POLYGON_Z - 1) * (POLYGON_X * 2) + (nCntPolygonX * 2) - 2) + 1] +
							m_aNor[((POLYGON_Z - 1) * (POLYGON_X * 2) + (nCntPolygonX * 2) - 2) + 2]) / 3;
				}
			}
			else
			{// 真ん中
				if (nCntPolygonX == 0)
				{// 左端
					pVtx[(POLYGON_X + 1) * nCntPolygonZ].nor =
						(m_aNor[(nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)] +
							m_aNor[(nCntPolygonZ * (2 * POLYGON_X))] +
							m_aNor[(nCntPolygonZ * (2 * POLYGON_X) + 1)]) / 3;
				}
				else if (nCntPolygonX == POLYGON_X)
				{// 右端
					pVtx[((nCntPolygonZ + 1) * POLYGON_X) + nCntPolygonZ].nor =
						(m_aNor[((POLYGON_X - 1) * 2 + ((POLYGON_X * 2)* (nCntPolygonZ - 1)))]
							+ m_aNor[((POLYGON_X - 1) * 2 + ((POLYGON_X * 2)* (nCntPolygonZ - 1))) + 1]
							+ m_aNor[((POLYGON_X - 1) * 2 + ((POLYGON_X * 2)* (nCntPolygonZ - 1))) + ((POLYGON_X * 2) + 1)]) / 3;
				}
				else
				{
					pVtx[(POLYGON_X + 2) + (nCntPolygonX - 1) + ((nCntPolygonZ * (POLYGON_X + 1)) - (POLYGON_X + 1))].nor =
						(m_aNor[((nCntPolygonX - 1) * 2) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X))] +
							m_aNor[((((nCntPolygonX - 1) * 2) + 1) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)))] +
							m_aNor[((((nCntPolygonX - 1) * 2) + 2) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)))] +
							m_aNor[((2 * POLYGON_X) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X))) + ((nCntPolygonX * 2) - 2)] +
							m_aNor[((2 * POLYGON_X) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)) + 1) + ((nCntPolygonX * 2) - 2)] +
							m_aNor[((2 * POLYGON_X) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)) + 2) + ((nCntPolygonX * 2) - 2)]) / 6;
				}
			}
		}
	}

	//====================================================================
	// 各頂点の法線を算出する
	//====================================================================
	//pVtx[0].nor = (m_aNor[0] + m_aNor[1]) / 2;
	//pVtx[1].nor = (m_aNor[1] + m_aNor[2] + m_aNor[3]) / 3;
	//pVtx[2].nor = m_aNor[3];
	//pVtx[3].nor = (m_aNor[0] + m_aNor[5] + m_aNor[4]) / 3;
	//pVtx[4].nor = (m_aNor[0] + m_aNor[1] + m_aNor[2] + m_aNor[5] + m_aNor[6] + m_aNor[7]) / 6;
	//pVtx[5].nor = (m_aNor[3] + m_aNor[2] + m_aNor[7]) / 3;
	//pVtx[6].nor = m_aNor[4];
	/*pVtx[7].nor = (m_aNor[4] + m_aNor[5] + m_aNor[6]) / 3;*/
	//pVtx[8].nor = (m_aNor[6] + m_aNor[7]) / 2;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

#endif

#if(0)
	//====================================================================
	// 各ポリゴンの法線を求める
	//====================================================================
	D3DXVECTOR3 aVec[3] = {};
	D3DXVECTOR3 aNor[8] = {};

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//================================================
	// 左上ポリゴン
	//================================================
	// ベクトルを求める
	aVec[0] = pVtx[3].pos - pVtx[0].pos;
	aVec[1] = pVtx[4].pos - pVtx[0].pos;
	aVec[2] = pVtx[1].pos - pVtx[0].pos;

	// 外積を使って法線を求める
	D3DXVec3Cross(&aNor[0], &aVec[1], &aVec[0]); // 法線Ａ
	D3DXVec3Cross(&aNor[1], &aVec[2], &aVec[1]); // 法線Ｂ

	// 正規化する
	D3DXVec3Normalize(&aNor[0], &aNor[0]); // 法線Ａ
	D3DXVec3Normalize(&aNor[1], &aNor[1]); // 法線Ｂ

	//================================================
	// 右上ポリゴン
	//================================================
	// ベクトルを求める
	aVec[0] = pVtx[4].pos - pVtx[1].pos;
	aVec[1] = pVtx[5].pos - pVtx[1].pos;
	aVec[2] = pVtx[2].pos - pVtx[1].pos;

	// 外積を使って法線を求める
	D3DXVec3Cross(&aNor[2], &aVec[1], &aVec[0]); // 法線Ｃ
	D3DXVec3Cross(&aNor[3], &aVec[2], &aVec[1]); // 法線Ｄ

	// 正規化する
	D3DXVec3Normalize(&aNor[2], &aNor[2]); // 法線Ｃ
	D3DXVec3Normalize(&aNor[3], &aNor[3]); // 法線Ｄ

	//================================================
	// 左下ポリゴン
	//================================================
	// ベクトルを求める
	aVec[0] = pVtx[6].pos - pVtx[3].pos;
	aVec[1] = pVtx[7].pos - pVtx[3].pos;
	aVec[2] = pVtx[4].pos - pVtx[3].pos;

	// 外積を使って法線を求める
	D3DXVec3Cross(&aNor[4], &aVec[1], &aVec[0]); // 法線Ｅ
	D3DXVec3Cross(&aNor[5], &aVec[2], &aVec[1]); // 法線Ｆ

	// 正規化する
	D3DXVec3Normalize(&aNor[4], &aNor[4]); // 法線Ｅ
	D3DXVec3Normalize(&aNor[5], &aNor[5]); // 法線Ｆ

	//================================================
	// 右下ポリゴン
	//================================================
	// ベクトルを求める
	aVec[0] = pVtx[7].pos - pVtx[4].pos;
	aVec[1] = pVtx[8].pos - pVtx[4].pos;
	aVec[2] = pVtx[5].pos - pVtx[4].pos;

	// 外積を使って法線を求める
	D3DXVec3Cross(&aNor[6], &aVec[1], &aVec[0]); // 法線Ｇ
	D3DXVec3Cross(&aNor[7], &aVec[2], &aVec[1]); // 法線Ｈ

	// 正規化する
	D3DXVec3Normalize(&aNor[6], &aNor[6]); // 法線Ｇ
	D3DXVec3Normalize(&aNor[7], &aNor[7]); // 法線Ｈ
	
	//====================================================================
	// 各頂点の法線を算出する
	//====================================================================
	pVtx[0].nor = (aNor[0] + aNor[1]) / 2;
	pVtx[1].nor = (aNor[1] + aNor[2] + aNor[3]) / 3;
	pVtx[2].nor = aNor[3];
	pVtx[3].nor = (aNor[0] + aNor[5] + aNor[4]) / 3;
	pVtx[4].nor = (aNor[0] + aNor[1] + aNor[2] + aNor[5] + aNor[6] + aNor[7]) / 6;
	pVtx[5].nor = (aNor[3] + aNor[2] + aNor[7]) / 3;
	pVtx[6].nor = aNor[4];
	pVtx[7].nor = (aNor[4] + aNor[5] + aNor[6]) / 3;
	pVtx[8].nor = (aNor[6] + aNor[7]) / 2;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

#endif

	WORD *pIdx;	// インデックスデータへのポインタ

	// インデックスバッファをロックし、インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntIdxZ = 0; nCntIdxZ < POLYGON_Z; nCntIdxZ++)
	{
		for (nCntIdxX = 0; nCntIdxX < POLYGON_X + 1; nCntIdxX++, nIdxCounter++)
		{
			pIdx[0] = nIdxCounter + POLYGON_X + 1;
			pIdx[1] = nIdxCounter;
			pIdx += 2;

			if (nCntIdxZ < POLYGON_Z - 1 && nCntIdxX == POLYGON_X)
			{
				pIdx[0] = nIdxCounter;
				pIdx[1] = nIdxCounter + (POLYGON_X + 1) + 1;
				pIdx += 2;
			}
		}
	}

	// インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

	LoadHeight();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMeshField::Uninit(void)
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

	// インデックスバッファの開放
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// オブジェクトの解放
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMeshField::Update(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

#if(1)
	int nCntPolygon = 0;
	int nCntNorPolygon = 0;

	for (int nCntZ = 0; nCntZ < POLYGON_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX < POLYGON_X; nCntX++)
		{
			D3DXVECTOR3 *pPos0, *pPos1, *pPos2, *pPos3;
			D3DXVECTOR3 vec0, vec1, vec2;
			D3DXVECTOR3 nor;

			// 一方のポリゴンの２つのベクトルから法線を算出
			pPos0 = &pVtx[nCntX + nCntZ + nCntPolygon].pos;
			pPos1 = &pVtx[(nCntX + nCntZ + nCntPolygon) + (POLYGON_X + 1)].pos;
			pPos2 = &pVtx[(nCntX + nCntZ + nCntPolygon) + (POLYGON_X + 1) + 1].pos;
			pPos3 = &pVtx[(nCntX + nCntZ + nCntPolygon) + 1].pos;

			vec0 = *pPos1 - *pPos0;
			vec1 = *pPos2 - *pPos0;
			vec2 = *pPos3 - *pPos0;

			// 外積を使って各ポリゴンの法線を求める
			D3DXVec3Cross(&nor, &vec1, &vec0);
			// 正規化する
			D3DXVec3Normalize(&nor, &nor);
			// 法線を保存
			m_aNor[(nCntZ * 2) + (nCntX * 2) + nCntNorPolygon] = nor;

			// 外積を使って各ポリゴンの法線を求める
			D3DXVec3Cross(&nor, &vec2, &vec1);
			// 正規化する
			D3DXVec3Normalize(&nor, &nor);
			// 法線を保存
			m_aNor[(nCntZ * 2) + (nCntX * 2) + nCntNorPolygon + 1] = nor;
		}

		nCntPolygon += POLYGON_X;
		nCntNorPolygon += (POLYGON_X * 2) - 2;
	}

	int nData0 = 0;
	int nData1 = 0;

	for (int nCntPolygonZ = 0; nCntPolygonZ < POLYGON_Z + 1; nCntPolygonZ++)
	{
		for (int nCntPolygonX = 0; nCntPolygonX < POLYGON_X + 1; nCntPolygonX++)
		{
			if (nCntPolygonZ == 0)
			{// 上端
				if (nCntPolygonX == 0)
				{// 左端
					pVtx[0].nor = (m_aNor[0] + m_aNor[1]) / 2;
				}
				else if (nCntPolygonX == POLYGON_X)
				{// 右端
					pVtx[POLYGON_X].nor = m_aNor[POLYGON_X + (POLYGON_X - 1)];
				}
				else
				{// 上端の真ん中
					pVtx[nCntPolygonX].nor =
						(m_aNor[(nCntPolygonX * 2) - 1] + m_aNor[((nCntPolygonX * 2) - 1) + 1] + m_aNor[((nCntPolygonX * 2) - 1) + 2]) / 3;
				}
			}
			else if (nCntPolygonZ == POLYGON_Z)
			{// 下端
				if (nCntPolygonX == 0)
				{// 左端
					pVtx[POLYGON_Z * (POLYGON_X + 1)].nor = m_aNor[2 * (POLYGON_X * (POLYGON_Z - 1))];
				}
				else if (nCntPolygonX == POLYGON_X)
				{// 右端
					pVtx[(POLYGON_X * POLYGON_Z) + (POLYGON_X + POLYGON_Z)].nor =
						(m_aNor[(2 * (POLYGON_X * (POLYGON_Z - 1))) + (2 * (POLYGON_X - 1))] +
							m_aNor[((2 * (POLYGON_X * (POLYGON_Z - 1))) + (2 * (POLYGON_X - 1))) + 1]) / 2;
				}
				else
				{// 下端の真ん中
					pVtx[(POLYGON_Z * (POLYGON_X + 1)) + nCntPolygonX].nor =
						(m_aNor[(POLYGON_Z - 1) * (POLYGON_X * 2) + (nCntPolygonX * 2) - 2] +
							m_aNor[((POLYGON_Z - 1) * (POLYGON_X * 2) + (nCntPolygonX * 2) - 2) + 1] +
							m_aNor[((POLYGON_Z - 1) * (POLYGON_X * 2) + (nCntPolygonX * 2) - 2) + 2]) / 3;
				}
			}
			else
			{// 真ん中
				if (nCntPolygonX == 0)
				{// 左端
					pVtx[(POLYGON_X + 1) * nCntPolygonZ].nor =
						(m_aNor[(nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)] +
						m_aNor[(nCntPolygonZ * (2 * POLYGON_X))] +
						m_aNor[(nCntPolygonZ * (2 * POLYGON_X) + 1)]) / 3;
				}
				else if (nCntPolygonX == POLYGON_X)
				{// 右端
					pVtx[((nCntPolygonZ + 1) * POLYGON_X) + nCntPolygonZ].nor =
						(m_aNor[((POLYGON_X - 1) * 2 + ((POLYGON_X * 2)* (nCntPolygonZ - 1)))]
							+ m_aNor[((POLYGON_X - 1) * 2 + ((POLYGON_X * 2)* (nCntPolygonZ - 1))) + 1]
							+ m_aNor[((POLYGON_X - 1) * 2 + ((POLYGON_X * 2)* (nCntPolygonZ - 1))) + ((POLYGON_X * 2) + 1)]) / 3;
				}
				else
				{
					pVtx[(POLYGON_X + 2) + (nCntPolygonX - 1) + ((nCntPolygonZ * (POLYGON_X + 1)) - (POLYGON_X + 1))].nor =
						(m_aNor[((nCntPolygonX - 1) * 2) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X))] +
						 m_aNor[((((nCntPolygonX - 1) * 2) + 1) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)))] +
						 m_aNor[((((nCntPolygonX - 1) * 2) + 2) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)))] +
						 m_aNor[((2 * POLYGON_X) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X))) + ((nCntPolygonX * 2) - 1)] +
						 m_aNor[((2 * POLYGON_X) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)) + 1) + ((nCntPolygonX * 2) - 1)] +
						 m_aNor[((2 * POLYGON_X) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)) + 2) + ((nCntPolygonX * 2) - 1)]) / 6;

					nData0 = (POLYGON_X + 2) + (nCntPolygonX - 1) + ((nCntPolygonZ * (POLYGON_X + 1)) - (POLYGON_X + 1));
					/*nData1 = ((2 * (POLYGON_X * (POLYGON_Z - 1))) + (2 * (POLYGON_X - 1))) + 1;*/
				}
			}
		}
	}

	int nVtxCounter = 0;

	if (pInputKeyboard->GetTrigger(DIK_0) == true)
	{// 0キーが押されたら
		SaveHeight();
	}

	//if (pInputKeyboard->GetTrigger(DIK_9) == true)
	//{// 9キーが押されたら
	//	LoadHeight();
	//}
	
#endif

#ifdef _DEBUG
	/*for (int nCntNor = 0; nCntNor < 25; nCntNor++)
	{
		CDebugProc::Print("cncfcfcfc", "法線[", nCntNor, "] : (", pVtx[nCntNor].nor.x, ", ", pVtx[nCntNor].nor.y, ", ", pVtx[nCntNor].nor.z, ")");
	}*/

	/*for (int nCntNor = 0; nCntNor < 25; nCntNor++)
	{
		CDebugProc::Print("cncfcfcfc", "法線[", nCntNor, "] : (", m_aNor[nCntNor].x, ", ", m_aNor[nCntNor].y, ", ", m_aNor[nCntNor].z, ")");
	}*/

	//for (int nCntNor = 0; nCntNor < 16; nCntNor++)
	//{
	//	CDebugProc::Print("cncfcfcfc", "pos[", nCntNor, "] : (", pVtx[nCntNor].pos.x, ", ", pVtx[nCntNor].pos.y, ", ", pVtx[nCntNor].pos.z, ")");
	//}

#endif

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void CMeshField::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{// デバイスの取得
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

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVerTex, 0, m_nNumPolygon);
}

//=============================================================================
// 高さを取得
//============================================================================
float CMeshField::GetHeight(D3DXVECTOR3 pos)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	bool bX = false;
	bool bZ = false;
	bool abX[POLYGON_X];
	bool abZ[POLYGON_Z];
	bool bRand = false;

	for (int nCount = 0; nCount < POLYGON_X; nCount++)
	{
		abX[nCount] = false;
	}

	for (int nCount = 0; nCount < POLYGON_Z; nCount++)
	{
		abZ[nCount] = false;
	}

	// 自分が今どのポリゴンに乗っているか
	for (int nCntZ = 0; nCntZ < POLYGON_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX < POLYGON_X; nCntX++)
		{
			if (pos.x >= -MESHFIELD_SIZE + (nCntX * MESHFIELD_SIZE) && pos.x < (nCntX * MESHFIELD_SIZE))
			{
				bX = true;
				abX[nCntX] = true;
			}

			if (pos.z <= MESHFIELD_SIZE - (nCntZ * MESHFIELD_SIZE) && pos.z > (nCntZ * -MESHFIELD_SIZE))
			{
				bZ = true;
				abZ[nCntZ] = true;
			}
		}
	}

	int nCount = 0;

	for (int nCntZ = 0; nCntZ < POLYGON_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX < POLYGON_X; nCntX++)
		{
			if (abX[nCntX] == true && abZ[nCntZ] == true)
			{
				//CDebugProc::Print("nc", nCntX + nCntZ + nCount, ": 中");
				bRand = true;
			}
			else
			{
				//CDebugProc::Print("nc", nCntX + nCntZ + nCount, ": 外");

				bRand = false;
			}
		}
		nCount+= 2;
	}


	int nCntPolygon = 0;
	int nCntNorPolygon = 0;

	for (int nCntZ = 0; nCntZ < POLYGON_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX < POLYGON_X; nCntX++)
		{
			if (abX[nCntX] == true && abZ[nCntZ] == true)
			{
				D3DXVECTOR3 *pPos0, *pPos1, *pPos2, *pPos3;
				D3DXVECTOR3 vec0, vec1, vec2, vec3;
				D3DXVECTOR3 nor0, nor1;
				float fData = 0.0f;

				// 一方のポリゴンの２つのベクトルから法線を算出
				pPos0 = &pVtx[nCntX + nCntZ + nCntPolygon].pos;							// 左上
				pPos1 = &pVtx[(nCntX + nCntZ + nCntPolygon) + (POLYGON_X + 1)].pos;		// 左下
				pPos2 = &pVtx[(nCntX + nCntZ + nCntPolygon) + (POLYGON_X + 1) + 1].pos;	// 右下
				pPos3 = &pVtx[(nCntX + nCntZ + nCntPolygon) + 1].pos;					// 右上

				vec0 = *pPos1 - *pPos0;	// 左の辺のベクトル
				vec1 = *pPos2 - *pPos0;	// 左上から右下のベクトル
				vec2 = *pPos3 - *pPos0;	// 上の辺のベクトル
				vec3 = pos - *pPos0;

				// 外積を使って各ポリゴンの法線を求める
				D3DXVec3Cross(&nor0, &vec1, &vec0);
				// 正規化する
				D3DXVec3Normalize(&nor0, &nor0);

				// 外積を使って各ポリゴンの法線を求める
				D3DXVec3Cross(&nor1, &vec2, &vec1);
				// 正規化する
				D3DXVec3Normalize(&nor1, &nor1);

				fData = ((vec1.z * vec3.x) - (vec1.x * vec3.z));

				if (fData > 0)
				{
					pos.y = (((nor0.x * (pos.x - pPos0->x) + nor0.z * (pos.z - pPos0->z)) / -nor0.y) + pPos0->y);
				}
				else
				{
					pos.y = (((nor1.x * (pos.x - pPos2->x) + nor1.z * (pos.z - pPos2->z)) / -nor1.y) + pPos2->y);
				}

				break;
			}
		}

		nCntPolygon += POLYGON_X;
		nCntNorPolygon += (POLYGON_X * 2) - 2;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return pos.y;
}

//=============================================================================
// 高さを設定
//============================================================================
void CMeshField::SetHeight(D3DXVECTOR3 pos, float fValue, float fRange)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < POLYGON_Z + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < POLYGON_X + 1; nCntX++)
		{
			// posから対象の頂点までの距離
			float fLength = sqrtf
				// xの距離
				((pos.x - pVtx->pos.x) * (pos.x - pVtx->pos.x) +
				// zの距離
				(pos.z - pVtx->pos.z) * (pos.z - pVtx->pos.z));

			if (fLength < fRange)
			{// 対象の頂点が範囲内
				// 範囲内での距離の比率に応じた高さ
				float fHeight = cosf((fLength / fRange) * (D3DX_PI * 0.5f)) * fValue;

				pVtx->pos.y += fHeight;
			}

			// 高さを代入
			g_aHeight[nCntZ][nCntX] = pVtx->pos.y;

			pVtx++;
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 高さをセーブ
//=============================================================================
void CMeshField::SaveHeight(void)
{
	FILE *pFile = NULL;

	// ファイルに書き込む
	pFile = fopen(TOUL_FILENAME, "wb");

	if (pFile != NULL)
	{
		fwrite(&g_aHeight[0][0], sizeof(float), POLYGON_Z * POLYGON_X, pFile);	// データのアドレス,データのサイズ,データの個数,ファイルポインタ

		fclose(pFile);
	}
	else
	{
		printf("number.txtを開けませんでした。\n");
	}

}

//=============================================================================
// 高さをロード
//=============================================================================
void CMeshField::LoadHeight(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	FILE *pFile = NULL;

	pFile = fopen(TOUL_FILENAME, "rb");

	if (pFile != NULL)
	{
		fread(&g_aHeight[0][0], sizeof(float), POLYGON_Z * POLYGON_X, pFile);	// データのアドレス,データのサイズ,データの個数,ファイルポインタ

		fclose(pFile);
	}
	else
	{
		printf("number.txtを開けませんでした。\n");
	}

	for (int nCntZ = 0; nCntZ < POLYGON_Z + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < POLYGON_X + 1; nCntX++)
		{// posを代入
			pVtx->pos.y = g_aHeight[nCntZ][nCntX];

			pVtx++;
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}