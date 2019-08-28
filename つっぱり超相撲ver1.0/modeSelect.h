//=============================================================================
//
// ポーズ処理 [pause.h]
// Author : Mikiya Meguro
//
//=============================================================================
#ifndef _MODESELECT_H_
#define _MODESELECT_H_

#include "main.h"
#include "input.h"
#include "scene.h"

//*****************************************************************************
//　前方宣言
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MODESELECTMENU			(3)							//ランキングの数

//*********************************************************************
//ゲームクラスの定義
//*********************************************************************
class CModeSelect : public CScene //派生クラス
{
public:
	typedef enum
	{
		SELECTMODE_NONE = 0,
		SELECTMODE_CONTINUE,		//続行状態
		SELECTMODE_RETRY,			//リトライ状態
		SELECTMODE_QUIT,			//終了
		SELECTMODE_MAX				//状態の総数
	}SELECTMODE;					//列挙型

	typedef enum
	{
		SELECTTYPE_NONE = 0,	//選ばれていない状態
		SELECTTYPE_SELECT,		//選ばれている	状態
		SELECTTYPE_MAX			//状態の総数
	}SELECT;

	typedef struct
	{
		SELECT		select;		//セレクト
		D3DXVECTOR3 pos;		//位置
		D3DXCOLOR	col;		//カラー
	}MODESELECTSELECT;

	CModeSelect(int nPriority = 7);
	~CModeSelect();
	static CModeSelect *Create(D3DXVECTOR3 pos, float fLength);
	virtual HRESULT Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static SELECTMODE *GetModeSelectMode(void);
	static HRESULT Load(void);
	static void UnLoad(void);
	static bool GetModeSelectBool(void);
	static void SetModeSelectBool(bool ModeSelectBool);
	//メンバ変数
private:
	D3DXVECTOR3					m_InitPos;
	float						m_fWidth, m_fHeight;	// 幅高さ
	D3DXVECTOR3					m_TexMove;


	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_MODESELECTMENU];			// テクスチャポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;							// 頂点バッファへのポインタ
	static LPDIRECT3DTEXTURE9	m_pTextureBG;						// テクスチャポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffBG;						// 頂点バッファへのポインタ
	CScene2D					*m_apPolygonBG;						// メニューのポリゴン

	MODESELECTSELECT			m_aModeSelectMenu[MAX_MODESELECTMENU];		// ポーズメニュー
	D3DXVECTOR3					m_Pos[MAX_MODESELECTMENU];				// 位置
	static SELECTMODE			m_SelectMode;						// 選択してるモード
	int							m_nSelect;							// 選択している番号
	CScene2D					*m_apPolygon[MAX_MODESELECTMENU];		// メニューのポリゴン
	static	bool				m_bModeSelect;							// ポーズの状態
};

#endif