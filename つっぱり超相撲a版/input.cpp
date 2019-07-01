//=============================================================================
//
// ���͏��� [input.cpp]
// Author :�@���R���
//
//=============================================================================
#include "input.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInput::CInput()
{
	m_pDevice = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInput::~CInput()
{
}

//=============================================================================
// ���͏���������
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		// DirectInput�I�u�W�F�N�g�̐���
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}
	
	return S_OK;
}

//=============================================================================
// ���͏I������
//=============================================================================
void CInput::Uninit(void)
{
	// DirectInput�I�u�W�F�N�g�̊J��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}

	// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();	// �L�[�{�[�h�ւ̃A�N�Z�X�������
		m_pDevice->Release();
		m_pDevice = NULL;
	}
}

//=============================================================================
// �L�[�{�[�h���͂̃R���X�g���N�^
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
// �L�[�{�[�h���͂̃f�X�g���N�^
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{
}

//=============================================================================
// �L�[�{�[�h���͏���������
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h���͏I������
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	// ���͏I������
	CInput::Uninit();
}

//=============================================================================
// �L�[�{�[�h���͍X�V����
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	// �L�[�{�[�h�̓��͏��
	int nCntKey;

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyState[nCntKey] = aKeyState[nCntKey];	// �L�[�{�[�h���͏��ۑ�
		}
	}
	else
	{
		m_pDevice->Acquire();	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//=============================================================================
// �L�[�{�[�h�̓��͏��(�v���X���)���擾
//=============================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return(m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̓��͏��(�g���K�[���)���擾
//=============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CXInputJoyPad::CXInputJoyPad()
{

}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CXInputJoyPad::~CXInputJoyPad()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CXInputJoyPad::Init()
{
	ZeroMemory(m_Controllers, sizeof(CONTROLER_STATE) * MAX_CONTROLLERS);

	return S_OK;
}
//=============================================================================
// �X�V����
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

		// ��񎝂��Ă�
		xijs = m_Controllers[dCnt].state.Gamepad.wButtons;

		// �W���C�p�b�h���͏��ۑ�(�g���K�[)
		m_aKeyStateTrigger[dCnt] = (m_aKeyState[dCnt] ^ xijs)& xijs;

		// �W���C�p�b�h���͏��ۑ�(�����[�X)
		m_aKeyStateRelese[dCnt] = (m_aKeyState[dCnt] ^ xijs)& xijs;

		// �W���C�p�b�h���͏��ۑ�(�v���X)
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
// XInput�̓��͏��(�v���X���)���擾
//=============================================================================
bool CXInputJoyPad::GetPress(int nButton, int indexpad)
{
	return(m_aKeyState[indexpad] & nButton) ? true : false;
}
//=============================================================================
// XInput�̓��͏��(�g���K�[���)���擾
//=============================================================================
bool CXInputJoyPad::GetTrigger(int nButton, int indexpad)
{
	return(m_aKeyStateTrigger[indexpad] & nButton) ? true : false;
}
//=============================================================================
// �W���C�p�b�h�̓��͏��(�����[�X���)���擾
//=============================================================================
bool CXInputJoyPad::GetRelese(int nButton, int indexpad)
{
	return(m_aKeyStateRelese[indexpad] & nButton) ? true : false;
}
//=============================================================================
// ���X�e�B�b�N
//=============================================================================
float CXInputJoyPad::GetLeftAxiz(int indexpad)
{
	float Axiz = atan2f(m_LeftAxizX[indexpad], m_LeftAxizY[indexpad]);

	return Axiz;
}
//=============================================================================
// �E�X�e�B�b�N
//=============================================================================
float CXInputJoyPad::GetRightAxiz(int indexpad)
{
	float Axiz = atan2f(m_RightAxizX[indexpad], m_RightAxizY[indexpad]);

	return Axiz;
}
//=============================================================================
// �X�e�B�b�N�̎擾
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