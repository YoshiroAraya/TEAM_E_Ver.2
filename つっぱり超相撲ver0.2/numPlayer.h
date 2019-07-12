//=============================================================================
//
// 人数選択処理 [numPlayer.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _NUMPLAYER_H_
#define _NUMPLAYER_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_NUMPLAYER			(3)

//=========================
// 人数選択クラス
//=========================
class CNumPlayer : public CScene
{
public:
	typedef enum
	{
		MODE_1P = 0,
		MODE_2P,
		MODE_TUTORIAL,
		MODE_MAX
	} MODE;

	typedef enum
	{
		SELECT_NONE = 0,	// 選ばれていない状態
		SELECT_SELECT,		// 選ばれている状態
		SELECT_MAX
	} SELECT;

	CNumPlayer(int nPriority = 7, OBJTYPE objType = OBJTYPE_UI);									// コンストラクタ
	~CNumPlayer();													// デストラクタ

	static CNumPlayer *Create(D3DXVECTOR3 pos);								// 人数選択を生成

	HRESULT Init(D3DXVECTOR3 pos);	// 人数選択初期化処理
	void Uninit(void);											// 人数選択終了処理
	void Update(void);											// 人数選択更新処理
	void Draw(void);											// 人数選択描画処理
	static	MODE GetMode(void);									// 人数選択を取得
	int GetSelect(void);
	static bool GetDecision(void) { return m_bDecision; }

private:
	CScene2D *m_apScene2D[MAX_NUMPLAYER];
	SELECT m_aSelect[MAX_NUMPLAYER];
	D3DXCOLOR m_aCol[MAX_NUMPLAYER];
	static MODE m_Mode;
	int	m_nSelect;
	static bool m_bDecision;
};
#endif