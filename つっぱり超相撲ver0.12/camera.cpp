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
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "title.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_SPEED	(2.0f)

#define HIGASI_TIME	(5)
#define NISI_TIME	(10)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CCamera::STATE CCamera::m_State = STATE_NONE;

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
	m_nStartCounter = 0;
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
	m_posV = D3DXVECTOR3(0.0f, 200.0f, -280.0f);	// 視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 注視点
	m_recU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = sqrtf((m_posV.x - m_posR.x) * (m_posV.x - m_posR.x) + (m_posV.z - m_posR.z) * (m_posV.z - m_posR.z));
	m_nStartCounter = 0;
	m_State = STATE_NONE;

	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_RESULT)
	{
		m_posV = D3DXVECTOR3(0.0f, 100.0f, -200.0f);	// 視点
		m_posR = D3DXVECTOR3(0.0f, 80.0f, 0.0f);		// 注視点
		m_recU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
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
	// プレイヤー取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	// 敵取得
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{
		if (CGame::GetState() == CGame::STATE_START)
		{
			// 秒数
			int nTime = m_nStartCounter / 60;

			m_nStartCounter++;

			if (nTime < HIGASI_TIME && pPlayer != NULL)
			{// 東側の入場
				m_State = STATE_HIGASHI;

				m_posV = D3DXVECTOR3(pPlayer->GetPosition().x + 85.0f, pPlayer->GetPosition().y + 60.0f, pPlayer->GetPosition().z - 50.0f);	// 視点
				m_posR = D3DXVECTOR3(pPlayer->GetPosition().x, pPlayer->GetPosition().y + 80.0f, pPlayer->GetPosition().z);		// 注視点
			}
			else if (nTime >= HIGASI_TIME && nTime < NISI_TIME && pEnemy != NULL)
			{// 西側の入場
				m_State = STATE_NISHI;

				m_posV = D3DXVECTOR3(pEnemy->GetPosition().x - 85.0f, pEnemy->GetPosition().y + 60.0f, pEnemy->GetPosition().z - 50.0f);	// 視点
				m_posR = D3DXVECTOR3(pEnemy->GetPosition().x, pEnemy->GetPosition().y + 80.0f, pEnemy->GetPosition().z);		// 注視点
			}
			else if (nTime >= NISI_TIME)
			{// カメラの引き
				m_State = STATE_NORMAL;
				m_posV.x = 0.0f;
				m_posR = D3DXVECTOR3(0.0f, 0.0f, 50.0f);

				if (m_posV.y <= 200.0f)
				{// y軸移動
					m_posV.y += 2.6f;

					if (m_posV.y > 200.0f)
					{
						m_posV.y = 200.0f;
					}
				}

				if (m_posV.z >= -280.0f)
				{
					m_posV.z -= 4.0f;

					if (m_posV.z < -280.0f)
					{
						m_posV.z = -280.0f;
					}
				}

				if (m_posV.y == 200.0f && m_posV.z == -280.0f)
				{
					CGame::SetState(CGame::STATE_GAME);
				}
			}

			if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
			{
				m_State = STATE_NORMAL;
				m_posV = D3DXVECTOR3(0.0f, 200.0f, -280.0f);
				m_posR = D3DXVECTOR3(0.0f, 0.0f, 50.0f);

				if (pPlayer != NULL && pEnemy != NULL)
				{
					pPlayer->SetPosition(D3DXVECTOR3(-80.0f, 30.0f, 0.0f));
					pEnemy->SetPosition(D3DXVECTOR3(80.0f, 30.0f, 0.0f));

					pPlayer->SetMotionType(0, CPlayer::MOTION_NEUTRAL);
					pPlayer->SetbMotionEnd(0, true);
					pPlayer->SetMotionType(1, CPlayer::MOTION_NEUTRAL);
					pPlayer->SetbMotionEnd(1, true);
					pEnemy->SetMotionType(0, CEnemy::MOTION_NEUTRAL);
					pEnemy->SetbMotionEnd(0, true);
					pEnemy->SetMotionType(1, CEnemy::MOTION_NEUTRAL);
					pEnemy->SetbMotionEnd(1, true);
				}

				CGame::SetState(CGame::STATE_GAME);
			}
		}
		else if (CGame::GetState() == CGame::STATE_GAME)
		{// ゲーム中
			if (pPlayer->GetState() == CPlayer::STATE_ULT)
			{// 必殺を打つ時
				if (pPlayer->GetDirection() == CPlayer::DIRECTION_RIGHT)
				{// 右を向いている
					if (m_posV.x <= pPlayer->GetPosition().x + 85.0f)
					{
						m_posV.x += 10.0f;

						if (m_posV.x > pPlayer->GetPosition().x + 85.0f)
						{
							m_posV.x = pPlayer->GetPosition().x + 85.0f;
						}
					}
					if (m_posV.y >= pPlayer->GetPosition().y + 60.0f)
					{
						m_posV.y -= 10.0f;

						if (m_posV.y < pPlayer->GetPosition().y + 60.0f)
						{
							m_posV.y = pPlayer->GetPosition().y + 60.0f;
						}
					}
					if (m_posV.z <= pPlayer->GetPosition().z - 50.0f)
					{
						m_posV.z += 15.0f;

						if (m_posV.z > pPlayer->GetPosition().z - 50.0f)
						{
							m_posV.z = pPlayer->GetPosition().z - 50.0f;
						}
					}
					//m_posV = D3DXVECTOR3(pPlayer->GetPosition().x + 85.0f, pPlayer->GetPosition().y + 60.0f, pPlayer->GetPosition().z - 50.0f);	// 視点
					m_posR = D3DXVECTOR3(pPlayer->GetPosition().x, pPlayer->GetPosition().y + 80.0f, pPlayer->GetPosition().z);		// 注視点
				}
				else if (pPlayer->GetDirection() == CPlayer::DIRECTION_LEFT)
				{// 左を向いている
					if (m_posV.x >= pPlayer->GetPosition().x - 85.0f)
					{
						m_posV.x -= 10.0f;

						if (m_posV.x < pPlayer->GetPosition().x - 85.0f)
						{
							m_posV.x = pPlayer->GetPosition().x - 85.0f;
						}
					}
					if (m_posV.y >= pPlayer->GetPosition().y + 60.0f)
					{
						m_posV.y -= 10.0f;

						if (m_posV.y < pPlayer->GetPosition().y + 60.0f)
						{
							m_posV.y = pPlayer->GetPosition().y + 60.0f;
						}
					}
					if (m_posV.z <= pPlayer->GetPosition().z - 50.0f)
					{
						m_posV.z += 15.0f;

						if (m_posV.z > pPlayer->GetPosition().z - 50.0f)
						{
							m_posV.z = pPlayer->GetPosition().z - 50.0f;
						}
					}

					//m_posV = D3DXVECTOR3(pPlayer->GetPosition().x - 85.0f, pPlayer->GetPosition().y + 60.0f, pPlayer->GetPosition().z - 50.0f);	// 視点
					m_posR = D3DXVECTOR3(pPlayer->GetPosition().x, pPlayer->GetPosition().y + 80.0f, pPlayer->GetPosition().z);		// 注視点
				}
			}
			if (pEnemy->GetState() == CEnemy::STATE_ULT)
			{
				if (pEnemy->GetDirection() == CEnemy::DIRECTION_RIGHT)
				{// 右を向いている
					if (m_posV.x <= pEnemy->GetPosition().x + 85.0f)
					{
						m_posV.x += 10.0f;

						if (m_posV.x > pEnemy->GetPosition().x + 85.0f)
						{
							m_posV.x = pEnemy->GetPosition().x + 85.0f;
						}
					}
					if (m_posV.y >= pEnemy->GetPosition().y + 60.0f)
					{
						m_posV.y -= 10.0f;

						if (m_posV.y < pEnemy->GetPosition().y + 60.0f)
						{
							m_posV.y = pEnemy->GetPosition().y + 60.0f;
						}
					}
					if (m_posV.z <= pEnemy->GetPosition().z - 50.0f)
					{
						m_posV.z += 15.0f;

						if (m_posV.z > pEnemy->GetPosition().z - 50.0f)
						{
							m_posV.z = pEnemy->GetPosition().z - 50.0f;
						}
					}
					//m_posV = D3DXVECTOR3(pEnemy->GetPosition().x + 85.0f, pEnemy->GetPosition().y + 60.0f, pEnemy->GetPosition().z - 50.0f);	// 視点
					m_posR = D3DXVECTOR3(pEnemy->GetPosition().x, pEnemy->GetPosition().y + 80.0f, pEnemy->GetPosition().z);		// 注視点
				}
				else if (pEnemy->GetDirection() == CEnemy::DIRECTION_LEFT)
				{// 左を向いている
					if (m_posV.x >= pEnemy->GetPosition().x - 85.0f)
					{
						m_posV.x -= 10.0f;

						if (m_posV.x < pEnemy->GetPosition().x - 85.0f)
						{
							m_posV.x = pEnemy->GetPosition().x - 85.0f;
						}
					}
					if (m_posV.y >= pEnemy->GetPosition().y + 60.0f)
					{
						m_posV.y -= 10.0f;

						if (m_posV.y < pEnemy->GetPosition().y + 60.0f)
						{
							m_posV.y = pEnemy->GetPosition().y + 60.0f;
						}
					}
					if (m_posV.z <= pEnemy->GetPosition().z - 50.0f)
					{
						m_posV.z += 15.0f;

						if (m_posV.z > pEnemy->GetPosition().z - 50.0f)
						{
							m_posV.z = pEnemy->GetPosition().z - 50.0f;
						}
					}

					//m_posV = D3DXVECTOR3(pEnemy->GetPosition().x - 85.0f, pEnemy->GetPosition().y + 60.0f, pEnemy->GetPosition().z - 50.0f);	// 視点
					m_posR = D3DXVECTOR3(pEnemy->GetPosition().x, pEnemy->GetPosition().y + 80.0f, pEnemy->GetPosition().z);		// 注視点
				}
			}
		}
	}
	else if (mode == CManager::MODE_TITLE)
	{
		// タイトル取得
		CTitle *pTitle;
		pTitle = CManager::GetTitle();

		if (pTitle != NULL)
		{
			if (pTitle->GetState() == CTitle::STATE_TITLE)
			{
				m_posV = D3DXVECTOR3(-340.0f, 400.0f, -400.0f);	// 視点
				m_posR = D3DXVECTOR3(0.0f, 25.0f, 0.0f);		// 注視点
			}
			else if(pTitle->GetState() == CTitle::STATE_NEWS)
			{
				m_posV = D3DXVECTOR3(0.0f, 25.0f, -200.0f);	// 視点
				m_posR = D3DXVECTOR3(0.0f, 25.0f, 0.0f);		// 注視点
			}
			else if(pTitle->GetState() == CTitle::STATE_CHARASELECT)
			{
				m_posV = D3DXVECTOR3(-150.0f, 130.0f, 30.0f);	// 視点
				m_posR = D3DXVECTOR3(0.0f, 70.0f, 0.0f);		// 注視点
			}
		}
	}
	else if (mode == CManager::MODE_ULTIMATE)
	{
		m_posV = D3DXVECTOR3(-25.0f, 130.0f, -120.0f);	// 視点
		m_posR = D3DXVECTOR3(200.0f, 70.0f, 30.0f);		// 注視点
	}

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
	CDebugProc::Print("cfccfccfc", "posV     : x", m_posV.x, "f", "   y", m_posV.y, "f", " z", m_posV.z, "f");
	CDebugProc::Print("cfccfccfc", "posR     : x", m_posR.x, "f", "   y", m_posR.y, "f", " z", m_posR.z, "f");
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
		10000.0f);									// カメラからの距離（奥）

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