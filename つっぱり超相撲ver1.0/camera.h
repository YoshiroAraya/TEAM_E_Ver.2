//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
//　前方宣言
//*****************************************************************************
class CEnemy;
class CPlayer;

//========================================
// クラスの定義
//========================================
//=====================
// カメラクラス
//=====================
class CCamera
{
public:
	typedef enum
	{
		STATE_NONE = 0,
		STATE_NORMAL,
		STATE_HIGASHI,
		STATE_NISHI
	}STATE;

	CCamera();		// コンストラクタ
	~CCamera();		// デストラクタ

	void Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

	void Start(CPlayer *pPlayer, CEnemy *pEnemy);
	void EnemyUlt(CPlayer *pPlayer, CEnemy *pEnemy);
	void PlayerUlt(CPlayer *pPlayer, CEnemy *pEnemy);

	void SetAGuard(bool bGuard)
	{
		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			m_abGuard[nCnt] = bGuard;
		}
	}

	void SetGuard(bool bGuard) { m_bGuard = bGuard; }
	bool GetGuard(void) { return m_bGuard; }
	void SetUltCamera(bool bUlt) { m_bUlt = bUlt; }

	D3DXVECTOR3 GetRot(void);						// 位置の取得
	static STATE GetState(void) { return m_State; }

private:
	D3DXVECTOR3 m_posV;				// 視点
	D3DXVECTOR3 m_posR;				// 注視点
	D3DXVECTOR3 m_recU;				// 上方向ベクトル
	D3DXVECTOR3 m_rot;				// 上方向ベクトル
	float m_fLength;				// 注視点からの距離
	//float fAngle;				
	D3DXMATRIX	m_mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX	m_mtxView;			// ビューマトリックス
	int m_nStartCounter;
	bool m_abGuard[3];
	bool m_bGuard;
	bool m_bUlt;
	static STATE m_State;
};

#endif