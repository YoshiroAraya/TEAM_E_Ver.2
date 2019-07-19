//=============================================================================
//
// 決定テクスチャの処理 [charaSelect.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _CHARASELECT_H_
#define _CHARASELECT_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// 前方宣言
//=============================================================================

//*****************************************************************************
// マクロ定義
//*****************************************************************************					

//=========================
// ポーズクラス
//=========================
class CCharaSelect : public CScene2D
{
public:
	CCharaSelect();									// コンストラクタ
	~CCharaSelect();								// デストラクタ

	static CCharaSelect *Create(D3DXVECTOR3 pos);	// 決定テクスチャを生成

	HRESULT Init(D3DXVECTOR3 pos);					// 決定テクスチャ初期化処理
	void Uninit(void);								// 決定テクスチャ終了処理
	void Update(void);								// 決定テクスチャ更新処理
	void Draw(void);								// 決定テクスチャ描画処理

private:
	D3DXCOLOR m_Col;
	int	m_nSelect;
	float m_fScale;
	bool m_bReset;
};
#endif