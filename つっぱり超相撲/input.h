//=============================================================================
//
// 入力処理 [input.h]
// Author :　長山拓実
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX			(256)	// キーの最大数

//========================================
// クラスの定義
//========================================
//=====================
// 入力クラス
//=====================
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化処理
	virtual void Uninit(void);								// 終了処理
	virtual void Update(void) = 0;							// 更新処理

protected:
	static LPDIRECTINPUT8	m_pInput;
	LPDIRECTINPUTDEVICE8	m_pDevice;
};

//=====================
// キーボードクラス
//=====================
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);

private:
	BYTE m_aKeyState[NUM_KEY_MAX];			// キーボードの入力情報（プレス情報）
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	// キーボードの入力情報（トリガー情報）
};
#endif