//=============================================================================
//
// 入力処理 [input.cpp]
// Author :　長山拓実
//
//=============================================================================
#include "input.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CInput::CInput()
{
	m_pDevice = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CInput::~CInput()
{
}

//=============================================================================
// 入力初期化処理
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		// DirectInputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}
	
	return S_OK;
}

//=============================================================================
// 入力終了処理
//=============================================================================
void CInput::Uninit(void)
{
	// DirectInputオブジェクトの開放
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}

	// 入力デバイス(キーボード)の開放
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();	// キーボードへのアクセス権を放棄
		m_pDevice->Release();
		m_pDevice = NULL;
	}
}

//=============================================================================
// キーボード入力のコンストラクタ
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		m_aKeyState[nCntKey] = 0;
		m_aKeyStateTrigger[nCntKey] = 0;
	}
}

//=============================================================================
// キーボード入力のデストラクタ
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{
}

//=============================================================================
// キーボード入力初期化処理
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// 入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// キーボード入力終了処理
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	// 入力終了処理
	CInput::Uninit();
}

//=============================================================================
// キーボード入力更新処理
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	// キーボードの入力情報
	int nCntKey;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyState[nCntKey] = aKeyState[nCntKey];	// キーボード入力情報保存
		}
	}
	else
	{
		m_pDevice->Acquire();	// キーボードへのアクセス権を獲得
	}
}

//=============================================================================
// キーボードの入力情報(プレス情報)を取得
//=============================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return(m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーボードの入力情報(トリガー情報)を取得
//=============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CXInputJoyPad::CXInputJoyPad()
{

}
//=============================================================================
// デストラクタ
//=============================================================================
CXInputJoyPad::~CXInputJoyPad()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CXInputJoyPad::Init()
{
	ZeroMemory(m_Controllers, sizeof(CONTROLER_STATE) * MAX_CONTROLLERS);

	return S_OK;
}
//=============================================================================
// 更新処理
//=============================================================================
void CXInputJoyPad::Update(void)
{
	// 
	UpdateControllerState();

	WORD xijs;

	for (DWORD dCnt = 0; dCnt < MAX_CONTROLLERS; dCnt++)
	{
		if ((m_Controllers[dCnt].state.Gamepad.sThumbLX < INPUT_DEADZONE &&
			m_Controllers[dCnt].state.Gamepad.sThumbLX > -INPUT_DEADZONE) &&
			(m_Controllers[dCnt].state.Gamepad.sThumbLY < INPUT_DEADZONE &&
				m_Controllers[dCnt].state.Gamepad.sThumbLY > -INPUT_DEADZONE))
		{
			m_Controllers[dCnt].state.Gamepad.sThumbLX = 0;
			m_Controllers[dCnt].state.Gamepad.sThumbLY = 0;
		}

		if ((m_Controllers[dCnt].state.Gamepad.sThumbRX < INPUT_DEADZONE &&
			m_Controllers[dCnt].state.Gamepad.sThumbRX > -INPUT_DEADZONE) &&
			(m_Controllers[dCnt].state.Gamepad.sThumbRY < INPUT_DEADZONE &&
				m_Controllers[dCnt].state.Gamepad.sThumbRY > -INPUT_DEADZONE))
		{
			m_Controllers[dCnt].state.Gamepad.sThumbRX = 0;
			m_Controllers[dCnt].state.Gamepad.sThumbRY = 0;
		}

		m_LeftAxizX[dCnt] = (float)m_Controllers[dCnt].state.Gamepad.sThumbLX;
		m_LeftAxizY[dCnt] = (float)m_Controllers[dCnt].state.Gamepad.sThumbLY;
		m_RightAxizX[dCnt] = (float)m_Controllers[dCnt].state.Gamepad.sThumbRX;
		m_RightAxizY[dCnt] = (float)m_Controllers[dCnt].state.Gamepad.sThumbRY;

		// 情報持ってる
		xijs = m_Controllers[dCnt].state.Gamepad.wButtons;

		// ジョイパッド入力情報保存(トリガー)
		m_aKeyStateTrigger[dCnt] = (m_aKeyState[dCnt] ^ xijs)& xijs;

		// ジョイパッド入力情報保存(リリース)
		m_aKeyStateRelese[dCnt] = (m_aKeyState[dCnt] ^ xijs)& xijs;

		// ジョイパッド入力情報保存(プレス)
		m_aKeyState[dCnt] = xijs;
	}
}
//=============================================================================
// 
//=============================================================================
HRESULT CXInputJoyPad::UpdateControllerState(void)
{
	DWORD dwResult;
	for (DWORD dCnt = 0; dCnt < MAX_CONTROLLERS; dCnt++)
	{
		dwResult = XInputGetState(dCnt, &m_Controllers[dCnt].state);
		if (dwResult == ERROR_SUCCESS)
		{
			m_Controllers[dCnt].bConnected = true;
		}
		else
		{
			m_Controllers[dCnt].bConnected = false;
		}
	}
	return S_OK;
}
//=============================================================================
// XInputの入力情報(プレス情報)を取得
//=============================================================================
bool CXInputJoyPad::GetPress(int nButton, int indexpad)
{
	return(m_aKeyState[indexpad] & nButton) ? true : false;
}
//=============================================================================
// XInputの入力情報(トリガー情報)を取得
//=============================================================================
bool CXInputJoyPad::GetTrigger(int nButton, int indexpad)
{
	return(m_aKeyStateTrigger[indexpad] & nButton) ? true : false;
}
//=============================================================================
// ジョイパッドの入力情報(リリース情報)を取得
//=============================================================================
bool CXInputJoyPad::GetRelese(int nButton, int indexpad)
{
	return(m_aKeyStateRelese[indexpad] & nButton) ? true : false;
}
//=============================================================================
// 左スティック
//=============================================================================
float CXInputJoyPad::GetLeftAxiz(int indexpad)
{
	float Axiz = atan2f(m_LeftAxizX[indexpad], m_LeftAxizY[indexpad]);

	return Axiz;
}
//=============================================================================
// 右スティック
//=============================================================================
float CXInputJoyPad::GetRightAxiz(int indexpad)
{
	float Axiz = atan2f(m_RightAxizX[indexpad], m_RightAxizY[indexpad]);

	return Axiz;
}
//=============================================================================
// スティックの取得
//=============================================================================
bool CXInputJoyPad::GetStick(int nLR, int indexpad)
{
	if (m_Controllers[indexpad].bConnected == true)
	{
		if (nLR == 0)
		{
			if (m_Controllers[indexpad].state.Gamepad.sThumbLX < XINPUT_STICK_MIN * 0.1f || m_Controllers[indexpad].state.Gamepad.sThumbLX > XINPUT_STICK_MAX * 0.1f ||
				m_Controllers[indexpad].state.Gamepad.sThumbLY < XINPUT_STICK_MIN * 0.1f || m_Controllers[indexpad].state.Gamepad.sThumbLY > XINPUT_STICK_MAX * 0.1f)
			{
				return true;
			}
		}
		else if (nLR == 1)
		{
			if (m_Controllers[indexpad].state.Gamepad.sThumbRX < XINPUT_STICK_MIN * 0.1f || m_Controllers[indexpad].state.Gamepad.sThumbRX > XINPUT_STICK_MAX * 0.1f ||
				m_Controllers[indexpad].state.Gamepad.sThumbRY < XINPUT_STICK_MIN * 0.1f || m_Controllers[indexpad].state.Gamepad.sThumbRY > XINPUT_STICK_MAX * 0.1f)
			{
				return true;
			}
		}
	}
	return false;
}