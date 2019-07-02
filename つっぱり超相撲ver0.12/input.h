//=============================================================================
//
// ���͏��� [input.h]
// Author :�@���R���
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_KEY_MAX			(256)	// �L�[�̍ő吔
#define DI_JOY_I_INPUT (1000)
#define MAX_CONTROLLERS (4)		// �R���g���[���[4��
#define INPUT_DEADZONE  (0.24f * FLOAT(0x7FFF) )
#define XINPUT_STICK_MAX (32767)
#define XINPUT_STICK_MIN (-32768)
//========================================
// �N���X�̒�`
//========================================
//=====================
// ���̓N���X
//=====================
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// ����������
	virtual void Uninit(void);								// �I������
	virtual void Update(void) = 0;							// �X�V����

protected:
	static LPDIRECTINPUT8	m_pInput;
	LPDIRECTINPUTDEVICE8	m_pDevice;
};

//=====================
// �L�[�{�[�h�N���X
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
	BYTE m_aKeyState[NUM_KEY_MAX];			// �L�[�{�[�h�̓��͏��i�v���X���j
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	// �L�[�{�[�h�̓��͏��i�g���K�[���j
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];			//�L�[�{�[�h���͏��(�����[�X)
};

//*****************************************************************************
// �W���C�p�b�h�N���X(XInput)
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