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

	static CFade *GetFade(void) { return m_pFade; };
	MODE GetMode(void) { return m_mode; };
	static void SetMode(MODE mode);

private:
	static CRenderer *m_pRenderer;
	static CInputKeyboard *m_pInputKeyboard;
	static CCamera *m_pCamera;
	static CLight *m_pLight;
	static CDebugProc *m_pDebugProc;
	static CMask *m_pMask;
	static CFade *m_pFade;
	static CGame *m_pGame;
	float m_fData;
	static MODE m_mode;
};

#endif