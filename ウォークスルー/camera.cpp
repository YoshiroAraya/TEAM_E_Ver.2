//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_SPEED	(4.0f)

//=============================================================================
// カメラクラスのコンストラクタ
//=============================================================================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 注視点
	m_recU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	m_fLength = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
// カメラの初期化処理
//=============================================================================
void CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(0.0f, 120.0f, -220.0f);	// 視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 注視点
	m_recU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = sqrtf((m_posV.x - m_posR.x) * (m_posV.x - m_posR.x) + (m_posV.z - m_posR.z) * (m_posV.z - m_posR.z));
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void CCamera::Update(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

#if 0
	if (pInputKeyboard->GetPress(DIK_A) == true)
	{// 左方向に移動
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{// 左奥に移動
			m_posV.x -= sinf(m_rot.y + D3DX_PI * 0.75f) * CAMERA_SPEED;
			m_posV.z -= cosf(m_rot.y + D3DX_PI * 0.75f) * CAMERA_SPEED;

			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{// 左手前移動
			m_posV.x -= sinf(m_rot.y + D3DX_PI * 0.25f) * CAMERA_SPEED;
			m_posV.z -= cosf(m_rot.y + D3DX_PI * 0.25f) * CAMERA_SPEED;

			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		}
		else
		{// 左
			m_posV.x -= sinf(m_rot.y + D3DX_PI * 0.5f) * CAMERA_SPEED;
			m_posV.z -= cosf(m_rot.y + D3DX_PI * 0.5f) * CAMERA_SPEED;

			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true)
	{// 右方向に移動
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{// 右前移動
			m_posV.x -= sinf(m_rot.y - D3DX_PI * 0.75f) * CAMERA_SPEED;
			m_posV.z -= cosf(m_rot.y - D3DX_PI * 0.75f) * CAMERA_SPEED;

			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{// 右後ろ移動
			m_posV.x -= sinf(m_rot.y - D3DX_PI * 0.25f) * CAMERA_SPEED;
			m_posV.z -= cosf(m_rot.y - D3DX_PI * 0.25f) * CAMERA_SPEED;

			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		}
		else
		{// 右
			m_posV.x -= sinf(m_rot.y - D3DX_PI * 0.5f) * CAMERA_SPEED;
			m_posV.z -= cosf(m_rot.y - D3DX_PI * 0.5f) * CAMERA_SPEED;

			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true)
	{// 前方向に移動
		m_posV.x += sinf(m_rot.y) * CAMERA_SPEED;
		m_posV.z += cosf(m_rot.y) * CAMERA_SPEED;

		m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true)
	{// 手前方向に移動
		m_posV.x -= sinf(m_rot.y) * CAMERA_SPEED;
		m_posV.z -= cosf(m_rot.y) * CAMERA_SPEED;

		m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
	}

	// 注視点回転
	if (pInputKeyboard->GetPress(DIK_Q) == true)
	{// 左方向に回転
		m_rot.y -= 0.03f;

		if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI * 2.0f;
		}

		m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
	}
	else if (pInputKeyboard->GetPress(DIK_E) == true)
	{// 左方向に回転
		m_rot.y += 0.03f;

		if (m_rot.y > D3DX_PI)
		{
			m_rot.y -= D3DX_PI * 2.0f;
		}

		m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
	}
	else if (pInputKeyboard->GetPress(DIK_T) == true)
	{// 上
		m_rot.x += 3.0f;

		m_posR.y = m_rot.x;
	}
	else if (pInputKeyboard->GetPress(DIK_B) == true)
	{// 下
		m_rot.x -= 3.0f;

		m_posR.y = m_rot.x;
	}

	// 視点回転
	if (pInputKeyboard->GetPress(DIK_Z) == true)
	{// 左方向に回転
		m_rot.y -= 0.03f;

		if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI * 2.0f;
		}

		m_posV.x = m_posR.x + sinf(D3DX_PI + m_rot.y) * m_fLength;
		m_posV.z = m_posR.z + cosf(D3DX_PI + m_rot.y) * m_fLength;
	}
	else if (pInputKeyboard->GetPress(DIK_C) == true)
	{// 左方向に回転
		m_rot.y += 0.03f;

		if (m_rot.y > D3DX_PI)
		{
			m_rot.y -= D3DX_PI * 2.0f;
		}

		m_posV.x = m_posR.x + sinf(D3DX_PI + m_rot.y) * m_fLength;
		m_posV.z = m_posR.z + cosf(D3DX_PI + m_rot.y) * m_fLength;
	}
	else if (pInputKeyboard->GetPress(DIK_Y) == true)
	{// 上
		m_posV.y += 3.0f;
	}
	else if (pInputKeyboard->GetPress(DIK_N) == true)
	{// 下
		m_posV.y -= 3.0f;
	}
#endif

#ifdef _DEBUG
	//CDebugProc::Print("cfccfccfc", "posV     : x", m_posV.x, "f", "   y", m_posV.y, "f", " z", m_posV.z, "f");
#endif
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void CCamera::SetCamera(void)
{
	// デバイスの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),						// 視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// 画面比率
		10.0f,										// カメラからの距離（手前）
		1000.0f);									// カメラからの距離（奥）

													// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_recU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// 向きの取得
//=============================================================================
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_rot;
}