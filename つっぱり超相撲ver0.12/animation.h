//=============================================================================
//
// アニメーションの処理 [animation.h]
// Author : Yamashita
//
//=============================================================================
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "main.h"
#include "scene3D.h"

//マクロ定義
#define MAX_COUNTER (5)
#define MAX_PATTERN (4)
//#define EXPLOSION_UV_U (1.0f)
//#define EXPLOSION_UV_V (0.25f)

class CScene;

//エクスプロージョンクラス（シーン2D派生）
class CAnimation : public CScene3D
{
public:
	CAnimation();
	~CAnimation();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXCOLOR col, float fHeight, float fWidth,float fUV_U, float fUV_V, int nCntSpeed, int nTotalAnim,int nRoop,int nDrawType);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void UpdateAnim();

	//静的メンバ関数
	static CAnimation *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fWidth,float fUV_U,float fUV_V,int nCntSpeed,int nTotalAnim,int nRoop,int nDrawType);

private:
	//	D3DXVECTOR3 m_pos;							//位置
	D3DXCOLOR m_col;								//色
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// 頂点バッファへのポインタ

	int m_nCounterAnim;								//アニメーションカウンター
	int m_nPatternAnim;								//アニメーションパターンNO
	D3DXVECTOR3 m_pos;								//位置
	int m_nCntSpeed;								//アニメーション再生スピード
	int m_nTotalAnim;								//アニメーションの合計枚数
	float m_fUV_U;									//テクスチャUV(U)
	float m_fUV_V;									//テクスチャUV(V)
	bool m_bUse;									//ループ再生をしているかしていないか
	int  m_nRoop;									//ループ再生するかしないか
	int m_nLife;									//テクスチャの寿命
	int m_nDrawType;								//描画タイプ

};
#endif
