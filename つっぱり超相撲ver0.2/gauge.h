//=============================================================================
//
// ゲージ処理 [scene2D.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEX_TYPE	(2)								// テクスチャの種類
#define MAX_GAUGE (2)
#define NUM_PLAYER (2)

//========================================
// クラスの定義
//========================================
//=====================
// オブジェクトクラス
//=====================
class CGauge : public CScene2D
{
public:
	CGauge(int nPriority = 3, OBJTYPE objType = OBJTYPE_SCENE2D);											// コンストラクタ
	~CGauge();										// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos);	// 2Dオブジェクト初期化処理
	void Uninit(void);				// 2Dオブジェクト終了処理
	void Update(void);				// 2Dオブジェクト更新処理
	void Draw(void);				// 2Dオブジェクト描画処理

	D3DXVECTOR3 GetPosition(void);		// 位置を取得
	void SetPosition(D3DXVECTOR3 pos);	// 位置を設定

	bool GetUlt(int nPlayer) { return m_bUlt[nPlayer]; }	// 必殺フラグを取得

	static CGauge *Create(D3DXVECTOR3 pos);		// オブジェクトの生成

	void SetGaugeRightLeft(float fRight, float fLeft);	// ゲージの値を設定
	float GetGaugeRight(void) { return m_fRight; };	// ゲージの値を取得
	float GetGaugeLeft(void) { return m_fLeft; };	// ゲージの値を取得

private:
	CScene2D		*m_pScene2D[MAX_GAUGE];		// 2dへのポインタ
	CScene2D		*m_pHPgaugeBG[MAX_GAUGE];	// 2dへのポインタ

	D3DXVECTOR3		m_Pos;						// ポリゴンの位置
	float			m_fRight, m_fLeft;			// 左右の値
	bool			m_bUlt[NUM_PLAYER];			// 必殺フラグ
};

#endif