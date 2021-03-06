//=============================================================================
//
// エフェクト処理 [effect3D.h]
// Author :  山下敦史
//
//=============================================================================
#ifndef _EFFECT3D_H_
#define _EFFECT_H_

#include "main.h"
#include "scene2d.h"

//*********************************************************************
//ビルボードクラスの定義
//*********************************************************************
class CEffect2D : public CScene2D //派生クラス
{
public:
	typedef enum
	{
		EFFECTTYPE_SALT = 0,
		EFFECTTYPE_MONEY,
		EFFECTTYPE_MAX
	}EFFECTTYPE;

	CEffect2D();
	~CEffect2D();
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEffect2D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
		float fWidth, float fHeight, int nLife, int TexType);

	void UpdateUI(void);
private:
	//メンバ変数
	D3DXVECTOR3					m_pos;						// 位置
	D3DXVECTOR3					m_move;						// 移動量
	D3DXVECTOR3					m_posold;					// 前回の位置
	float						m_fWidth, m_fHeight;		// 幅 高さ
	D3DXCOLOR					m_Col;						// カラー
	int							m_nNumMax;					// エフェクトのポリゴン数
	int							m_nLife;					// エフェクトが消えるまでの時間
	float						m_fAlpha;					// エフェクトの透明度
	int							m_nCntTimer;				// タイマー
	int							m_nTexType;
	EFFECTTYPE					m_EffectType;
};


#endif
