//=============================================================================
//
// マネージャ処理 [manager.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CRenderer;
class CInputKeyboard;
class CCamera;
class CLight;
class CDebugProc;
class CScene3D;
class CPlayer;
class CEnemy;
class CMeshField;
class CShadow;
class CMask;

//=============================================================================
// クラスの定義
//=============================================================================
//=====================
// マネージャクラス
//=====================
class CManager
{
public:
	CManager();		// コンストラクタ
	~CManager();	// デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CRenderer *GetRenderer(void);
	static CInputKeyboard *GetInputKeyboard(void);
	static CCamera *GetCamera(void);
	static CScene3D *GetScene3D(void);
	static CPlayer *GetPlayer(void);
	static CEnemy *GetEnemy(void);
	static CShadow *GetShadow(void);
	static CMeshField *GetMeshField(void);
	static CMask *GetMask(void);

private:
	static CRenderer *m_pRenderer;
	static CInputKeyboard *m_pInputKeyboard;
	static CCamera *m_pCamera;
	static CLight *m_pLight;
	static CDebugProc *m_pDebugProc;
	static CScene3D *m_pScene3D;
	static CPlayer *m_pPlayer;
	static CEnemy *m_pEnemy;
	static CShadow *m_pShadow;
	static CMeshField *m_pMeshField;
	static CMask *m_pMask;
	float m_fData;
};

#endif