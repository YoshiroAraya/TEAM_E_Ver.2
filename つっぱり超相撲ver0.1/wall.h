#pragma once
//=============================================================================
//
// 壁の処理 [wall.h]
// Author : 山下敦史
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "scene3D.h"

class CScene;
//影のクラス（シーン3Dの派生クラス）
class CWall : public CScene3D
{
public:

	CWall();										//コンストラクタ
	~CWall();										//デストラクタ

	HRESULT Init(void);								//初期化処理
	void Uninit(void);								//終了処理
	void Update(void);								//更新処理
	void Draw(void);								//描画処理

	//静的メンバ関数
	static CWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth);

	static HRESULT Load(void);						//テクスチャ読み込み
	static void Unload(void);

	//void SetLifeBar(int nLife);
private:
	static LPDIRECT3DTEXTURE9		m_pTextures;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9         m_pVtxBuff;		// 頂点バッファへのポインタ

	D3DXVECTOR3				m_pos;					// ポリゴンの位置
	D3DXVECTOR3				m_rot;					// ポリゴンの回転
	D3DXVECTOR3				m_move;					// ポリゴンの移動量
	D3DXCOLOR				m_col;
	LPDIRECT3DTEXTURE9		m_pTexture;
	D3DXMATRIX				m_mtxWorld;				//ワールドマトリックス

	int m_nType;
	float m_fCol;
	int m_nCnt;
	float m_fWidth;
	float m_fHeight;
	float m_fSize;
	bool m_bDestroy;
};
#endif