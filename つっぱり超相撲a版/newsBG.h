//=============================================================================
//
// ニュースの背景処理 [newsBG.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _NEWSBG_H_
#define _NEWSBG_H_

#include "scene3D.h"

//========================================
// 前方宣言
//========================================
class CScene;

//========================================
// クラスの定義
//========================================

//=====================
// オブジェクトクラス
//=====================


//影のクラス（シーン3Dの派生クラス）
class CNewsBG : public CScene3D
{
public:
	CNewsBG();										//コンストラクタ
	~CNewsBG();										//デストラクタ

	HRESULT Init(void);								//初期化処理
	void Uninit(void);								//終了処理
	void Update(void);								//更新処理
	void Draw(void);								//描画処理

													//静的メンバ関数
	static CNewsBG *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth);

	//void SetLifeBar(int nLife);
private:
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