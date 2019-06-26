//=============================================================================
//
// エフェクト処理 [effect.h]
// Author :  Meguro Mikiya
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "billboard.h"

//*********************************************************************
//ビルボードクラスの定義
//*********************************************************************
class CEffect : public CBillboard //派生クラス
{
public:
	typedef enum
	{
		EFFECTTYPE_00 = 0,
		EFFECTTYPE_01,
		EFFECTTYPE_MAX
	}EFFECTTYPE;

	CEffect();
	~CEffect();
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move,D3DXCOLOR col,
							float fWidth, float fHeight,int nNumMax,int nLife, int TexType);
	
private:
	//メンバ変数
	D3DXVECTOR3					m_pos;						// 位置
	D3DXVECTOR3					m_move;						// 移動量
	D3DXVECTOR3					m_posold;					// 前回の位置
	float						m_fWidth, m_fHeight;		// 幅 高さ
	D3DXMATRIX					m_mtrxWorld;				// ワールドマトリックス
	D3DXCOLOR					m_Col;						// カラー
	int							m_nNumMax;					// エフェクトのポリゴン数
	int							m_nLife;					// エフェクトが消えるまでの時間
	float						m_fAlpha;					// エフェクトの透明度
	int							m_nCntTimer;				// タイマー
	CBillboard					*m_pBillBoard;

	int					m_nTexType;
	EFFECTTYPE					m_EffectType;
};
#endif
