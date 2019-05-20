//=============================================================================
//
// �}�l�[�W������ [manager.h]
// Author : ���R���
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

//=============================================================================
// �O���錾
//=============================================================================
class CRenderer;
class CInputKeyboard;
class CCamera;
class CLight;
class CDebugProc;
class CMask;
class CFade;
class CGame;
class CCharacterMove;
class CXInputJoyPad;

//=============================================================================
// �}�N��
//=============================================================================
#define PLAYER_RIGHT		(DIK_D)
#define PLAYER_LEFT			(DIK_A)
#define PLAYER_A_BUTTON		(DIK_F)
#define PLAYER_B_BUTTON		(DIK_G)

#define ENEMY_RIGHT			(DIK_NUMPAD6)
#define ENEMY_LEFT			(DIK_NUMPAD4)
#define ENEMY_A_BUTTON		(DIK_ADD)
#define ENEMY_B_BUTTON		(DIK_NUMPADENTER)

//=============================================================================
// �}�N��(�R���g���[���[)
//=============================================================================
#define XPLAYER_RIGHT			(XINPUT_GAMEPAD_DPAD_RIGHT)
#define XPLAYER_LEFT			(XINPUT_GAMEPAD_DPAD_LEFT)
#define XPLAYER_A_BUTTON		(XINPUT_GAMEPAD_A) //�O�[
#define XPLAYER_Y_BUTTON		(XINPUT_GAMEPAD_Y) //�`���L
#define XPLAYER_X_BUTTON		(XINPUT_GAMEPAD_X) //�p�[
#define XPLAYER_B_BUTTON		(XINPUT_GAMEPAD_B)

#define XENEMY_RIGHT			(XINPUT_GAMEPAD_DPAD_RIGHT)
#define XENEMY_LEFT				(XINPUT_GAMEPAD_DPAD_LEFT)
#define XENEMY_A_BUTTON			(XINPUT_GAMEPAD_A) //�O�[
#define XENEMY_Y_BUTTON			(XINPUT_GAMEPAD_Y) //�`���L
#define XENEMY_X_BUTTON			(XINPUT_GAMEPAD_X) //�p�[
#define XENEMY_B_BUTTON			(XINPUT_GAMEPAD_B)


//=============================================================================
// �N���X�̒�`
//=============================================================================
//=====================
// �}�l�[�W���N���X
//=====================
class CManager
{
public:

	typedef enum
	{//���[�h�̍\����
		MODE_NONE = 0,
		MODE_TITLE,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_MAX
	}MODE;

	CManager();		// �R���X�g���N�^
	~CManager();	// �f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CRenderer *GetRenderer(void);
	static CInputKeyboard *GetInputKeyboard(void);
	static CCamera *GetCamera(void);
	static CMask *GetMask(void);
	static CCharacterMove *GetCharacterMove(void);
	static CFade *GetFade(void) { return m_pFade; };
	MODE GetMode(void) { return m_mode; };
	static void SetMode(MODE mode);
	static CGame *GetGame(void) { return m_pGame; }
	static CXInputJoyPad *GetXInput(void) { return m_pXInput; }

private:
	static CRenderer *m_pRenderer;
	static CInputKeyboard *m_pInputKeyboard;
	static CCamera *m_pCamera;
	static CLight *m_pLight;
	static CDebugProc *m_pDebugProc;
	static CMask *m_pMask;
	static CFade *m_pFade;
	static CCharacterMove *m_pCharacterMove;
	static CGame *m_pGame;
	static CXInputJoyPad *m_pXInput;
	float m_fData;
	static MODE m_mode;
};

#endif