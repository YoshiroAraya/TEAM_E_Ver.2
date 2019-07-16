//=============================================================================
//
// ポーズの処理 [jankenUI.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _JANKENUI_H_
#define _JANKENUI_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_JANKENUI_TEXTURE	(6)
#define MAX_JANKENUI			(3)

//=========================
// ポーズクラス
//=========================
class CJankenUI : public CScene
{
public:
	typedef enum
	{
		MODE_GU = 0,		// グー
		MODE_CHOKI,		// チョキ
		MODE_PA,		// パー
		MODE_MAX
	} MODE;

	typedef enum
	{
		SELECT_NONE = 0,	// 選ばれていない状態
		SELECT_SELECT,		// 選ばれている状態
		SELECT_MAX
	} SELECT;

	CJankenUI(int nPriority = 2, OBJTYPE objType = OBJTYPE_UI);									// コンストラクタ
	~CJankenUI();													// デストラクタ

	static CJankenUI *Create(D3DXVECTOR3 pos);								// ポーズを生成

	HRESULT Init(D3DXVECTOR3 pos);	// ポーズ初期化処理
	void Uninit(void);											// ポーズ終了処理
	void Update(void);											// ポーズ更新処理
	void Draw(void);											// ポーズ描画処理
	static	MODE GetMode(void);									// ポーズを取得
	int GetSelect(void);

private:
	CScene2D *m_apScene2D[MAX_JANKENUI_TEXTURE];
	SELECT m_aSelect[MAX_JANKENUI];
	D3DXCOLOR m_aCol[MAX_JANKENUI];
	static MODE m_Mode;
	int	m_nSelect;
};
#endif