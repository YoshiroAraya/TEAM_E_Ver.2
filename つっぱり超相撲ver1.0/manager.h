//=============================================================================
//
// マネージャ処理 [manager.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
#include "sound.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CRenderer;
class CInputKeyboard;
class CCamera;
class CLight;
class CDebugProc;
class CMask;
class CFade;
class CGame;
class CTitle;
class CResult;
class CUltimate;
class CCharacterMove;
class CXInputJoyPad;
class CSound;
class CTutorial;
//=============================================================================
// マクロ
//=============================================================================
#define PLAYER_RIGHT		(DIK_D)
#define PLAYER_LEFT			(DIK_A)
#define PLAYER_A_BUTTON		(DIK_F)
#define PLAYER_B_BUTTON		(DIK_G)
#define PLAYER_C_BUTTON		(DIK_S)

#define ENEMY_RIGHT			(DIK_NUMPAD6)
#define ENEMY_LEFT			(DIK_NUMPAD4)
#define ENEMY_A_BUTTON		(DIK_ADD)
#define ENEMY_B_BUTTON		(DIK_NUMPADENTER)
#define ENEMY_C_BUTTON		(DIK_NUMPAD5)
#define PLAYER_C_BUTTON		(DIK_S)
#define MAX_SOUND			(3)

//=============================================================================
// マクロ(コントローラー)
//=============================================================================
#define XPLAYER_RIGHT			(XINPUT_GAMEPAD_DPAD_RIGHT)
#define XPLAYER_LEFT			(XINPUT_GAMEPAD_DPAD_LEFT)
#define XPLAYER_A_BUTTON		(XINPUT_GAMEPAD_A) //グー
#define XPLAYER_Y_BUTTON		(XINPUT_GAMEPAD_Y) //チョキ
#define XPLAYER_X_BUTTON		(XINPUT_GAMEPAD_X) //パー
#define XPLAYER_B_BUTTON		(XINPUT_GAMEPAD_B)
#define XPLAYER_RB_BUTTON		(XINPUT_GAMEPAD_RIGHT_SHOULDER) //パー
#define XPLAYER_LB_BUTTON		(XINPUT_GAMEPAD_LEFT_SHOULDER)

#define XENEMY_RIGHT			(XINPUT_GAMEPAD_DPAD_RIGHT)
#define XENEMY_LEFT				(XINPUT_GAMEPAD_DPAD_LEFT)
#define XENEMY_A_BUTTON			(XINPUT_GAMEPAD_A) //グー
#define XENEMY_Y_BUTTON			(XINPUT_GAMEPAD_Y) //チョキ
#define XENEMY_X_BUTTON			(XINPUT_GAMEPAD_X) //パー
#define XENEMY_B_BUTTON			(XINPUT_GAMEPAD_B)
#define XENEMY_RB_BUTTON		(XINPUT_GAMEPAD_RIGHT_SHOULDER) //パー
#define XENEMY_LB_BUTTON		(XINPUT_GAMEPAD_LEFT_SHOULDER)


//m_pos.x += sinf(pXInputJoyPad->GetLeftAxiz(0)) * PLAYER_MOVESPEED;
//m_pos.y -= cosf(pXInputJoyPad->GetLeftAxiz(0)) * PLAYER_MOVESPEED;

//=============================================================================
// クラスの定義
//=============================================================================
//=====================
// マネージャクラス
//=====================
class CManager
{
public:

	typedef enum
	{//モードの構造体
		MODE_NONE = 0,
		MODE_TITLE,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_ULTIMATE,
		MODE_MAX
	}MODE;

	CManager();		// コンストラクタ
	~CManager();	// デストラクタ

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
	static MODE GetMode(void) { return m_mode; };
	static void SetMode(MODE mode);
	static CGame *GetGame(void) { return m_pGame; }
	static CTitle *GetTitle(void) { return m_pTitle; }
	static CTutorial *GetTutorial(void) { return m_pTutorial; }
	static CResult *GetResult(void) { return m_pResult; }
	static CUltimate *GetUltimate(void) { return m_pUltimate; }
	static CXInputJoyPad *GetXInput(void) { return m_pXInput; }
	static CSound *GetSound(int nNum);
	static void SetCharaSelectMode(bool bCharaSelect) { m_bCharaSelectMode = bCharaSelect; };
	static void SetNumPlayerMode(int mode) { m_nNumPlayerMode = mode; };
	static int GetNumPlayerMode(void) { return m_nNumPlayerMode; };

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
	static CTitle *m_pTitle;
	static CTutorial *m_pTutorial;
	static CResult *m_pResult;
	static CUltimate *m_pUltimate;
	static bool m_bCharaSelectMode;

	static CXInputJoyPad *m_pXInput;
	float m_fData;
	static MODE m_mode;
	static CSound			*m_pSound[MAX_SOUND];	// サウンドのポインタ
	static int m_nNumPlayerMode;
};

#endif