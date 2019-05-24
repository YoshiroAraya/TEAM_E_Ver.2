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
#define GAUGE_NAME		"data/TEXTURE/player000.png"	// プレイヤーのテクスチャ名
#define BULLET_NAME		"data/TEXTURE/bullet000.png"	// 弾のテクスチャ名
#define MAX_TEX_TYPE	(2)								// テクスチャの種類
#define MAX_GAUGE (2)

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

	HRESULT Init(D3DXVECTOR3 pos);				// 2Dオブジェクト初期化処理
	void Uninit(void);							// 2Dオブジェクト終了処理
	void Update(void);							// 2Dオブジェクト更新処理
	void Draw(void);							// 2Dオブジェクト描画処理

	D3DXVECTOR3 GetPosition(void);						// 位置を取得
	void SetPosition(D3DXVECTOR3 pos);					// 位置を設定

	static CGauge *Create(D3DXVECTOR3 pos);			// オブジェクトの生成
	static HRESULT Load(void);
	static void UnLoad(void);

	void SetGaugeRightLeft(float fRight, float fLeft);

private:
	static LPDIRECT3DTEXTURE9		m_pTexture;					// テクスチャへのポインタ

	CScene2D						*m_pScene2D[MAX_GAUGE];		// 2dへのポインタ

	D3DXVECTOR3						m_Pos;						// ポリゴンの位置
	float							m_fRight, m_fLeft;			// 左右の値
};

#endif