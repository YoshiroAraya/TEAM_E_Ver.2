//=============================================================================
//
// 勝者UIの処理 [winnerUI.h]
// Author : 山下敦史
//
//=============================================================================
#ifndef _WINNERUI_H_
#define _WINNERUI_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEX_TYPE	(2)								// テクスチャの種類
#define MAX_UI (6)

//========================================
// クラスの定義
//========================================
//=====================
// オブジェクトクラス
//=====================
class CWinnerUI : public CScene2D
{
public:
	CWinnerUI();											// コンストラクタ
	~CWinnerUI();										// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos);				// 2Dオブジェクト初期化処理
	void Uninit(void);							// 2Dオブジェクト終了処理
	void Update(void);							// 2Dオブジェクト更新処理
	void Draw(void);							// 2Dオブジェクト描画処理

	D3DXVECTOR3 GetPosition(void);						// 位置を取得
	void SetPosition(D3DXVECTOR3 pos);					// 位置を設定

	static CWinnerUI *Create(D3DXVECTOR3 pos);			// オブジェクトの生成
	void SetDrawSyouhai(bool bDraw) { m_pSyouhai2D->SetbDraw(bDraw); };
	void SetbWinner(bool bWinner) { m_bWinner = bWinner; };
private:
	CScene2D						*m_pScene2D[MAX_UI];		// 2dへのポインタ
	CScene2D						*m_pTyanko2D[MAX_UI];		// 2dへのポインタ
	CScene2D						*m_pSyouhai2D;				// 2dへのポインタ

	D3DXVECTOR3						m_Pos;						// ポリゴンの位置
	float							m_fRight, m_fLeft;			// 左右の値

	int								m_n1player;
	int								m_n2player;
	bool							m_bWinner;
};

#endif
