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
#define DI_JOY_I_INPUT (1000)
#define MAX_CONTROLLERS (4)		// コントローラー4つ分
#define INPUT_DEADZONE  (0.24f * FLOAT(0x7FFF) )
#define XINPUT_STICK_MAX (32767)
#define XINPUT_STICK_MIN (-32768)
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
	bool GetRelese(int nKey);

private:
	BYTE m_aKeyState[NUM_KEY_MAX];			// キーボードの入力情報（プレス情報）
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	// キーボードの入力情報（トリガー情報）
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];			//キーボード入力情報(リリース)
};

//*****************************************************************************
// ジョイパッドクラス(XInput)
//*****************************************************************************
class CXInputJoyPad
{
public:
	struct CONTROLER_STATE
	{
		XINPUT_STATE state;
		bool bConnected;
	};

	CXInputJoyPad();
	~CXInputJoyPad();

	HRESULT Init(void);
	void Update(void);
	bool GetPress(int nButton, int indexpad);
	bool GetTrigger(int nButton, int indexpad);
	bool GetRelese(int nButton, int indexpad);
	bool GetStick(int nLR, int indexpad);
	float GetLeftAxiz(int indexpad);
	float GetRightAxiz(int indexpad);

private:
	HRESULT UpdateControllerState(void);

	CONTROLER_STATE m_Controllers[MAX_CONTROLLERS];
	WORD m_aKeyState[MAX_CONTROLLERS];
	WORD m_aKeyStateTrigger[MAX_CONTROLLERS];
	WORD m_aKeyStateRelese[MAX_CONTROLLERS];
	WORD m_Stick[MAX_CONTROLLERS];

	float m_LeftAxizX[MAX_CONTROLLERS];
	float m_LeftAxizY[MAX_CONTROLLERS];
	float m_RightAxizX[MAX_CONTROLLERS];
	float m_RightAxizY[MAX_CONTROLLERS];


	//WCHAR m_szMessage[4][1024] = { 0 };
	//HWND    m_hWnd;
	//bool    m_bDeadZoneOn = true;
};
#endif