//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

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
	static STATE m_State;
};

#endif