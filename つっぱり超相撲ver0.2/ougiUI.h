//=============================================================================
//
// ゲージ処理 [scene2D.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _OUGIUI_H_
#define _OUGIUI_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEX_TYPE	(2)								// テクスチャの種類
#define MAX_OUGIUI (1)


//========================================
// クラスの定義
//========================================
//=====================
// オブジェクトクラス
//=====================
class COugiUI : public CScene2D
{
public:
	COugiUI(int nPriority = 3, OBJTYPE objType = OBJTYPE_SCENE2D);											// コンストラクタ
	~COugiUI();										// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos);	// 2Dオブジェクト初期化処理
	void Uninit(void);				// 2Dオブジェクト終了処理
	void Update(void);				// 2Dオブジェクト更新処理
	void Draw(void);				// 2Dオブジェクト描画処理

	D3DXVECTOR3 GetPosition(void);		// 位置を取得
	static COugiUI *Create(D3DXVECTOR3 pos);		// オブジェクトの生成

private:
	CScene2D		*m_pScene2D[MAX_OUGIUI];		// 2dへのポインタ

	D3DXVECTOR3		m_Pos;						// ポリゴンの位置
	float			m_fRight, m_fLeft;			// 左右の値
};

#endif