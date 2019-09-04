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
#include "BattleSystem.h"
#include "tutorial.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_SPEED	(2.0f)

#define HIGASI_TIME		(5)
#define NISI_TIME		(10)

#define GAME_CAMERA_X	(0.0f)
#define GAME_CAMERA_Y	(160.0f)
#define GAME_CAMERA_Z	(-330.0f)
#define GAME_RCAMERA_X	(0.0f)
#define GAME_RCAMERA_Y	(110.0f)
#define GAME_RCAMERA_Z	(50.0f)

#define START_CAMERA_X	(90.0f)
#define START_CAMERA_Y	(80.0f)
#define START_CAMERA_Z	(90.0f)

#define SELECT_CAMERA_POSR (D3DXVECTOR3(0.0f, 70.0f, 0.0f))
#define TITLE_POSV (D3DXVECTOR3(-340.0f, 400.0f, -400.0f))
#define TITLE_POSR (D3DXVECTOR3(0.0f, 25.0f, 0.0f))
#define NEWS_POSV (D3DXVECTOR3(0.0f, 25.0f, -200.0f))
#define NEWS_POSR (D3DXVECTOR3(0.0f, 25.0f, 0.0f))

#define SELECT_CAMERA_SPEED (1.0f)
#define SELECT_CAMERA_POSX (-150.0f)
#define SELECT_CAMERA_POSY (130.0f)
#define SELECT_CAMERA_POSZ (30.0f)

#define PLAYER_POS (D3DXVECTOR3(-80.0f, 30.0f, 0.0f))
#define ENEMY_POS (D3DXVECTOR3(80.0f, 30.0f, 0.0f))

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
	m_posV = D3DXVECTOR3(0.0f, GAME_CAMERA_Y, GAME_CAMERA_Z);	// 視点
	m_posR = D3DXVECTOR3(0.0f, GAME_RCAMERA_Y, GAME_RCAMERA_Z);		// 注視点
	m_recU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = sqrtf((m_posV.x - m_posR.x) * (m_posV.x - m_posR.x) + (m_posV.z - m_posR.z) * (m_posV.z - m_posR.z));
	m_nStartCounter = 0;
	m_State = STATE_NONE;
	m_bGuard = true;
	for (int nCntGuard = 0; nCntGuard < 3; nCntGuard++)
	{
		m_abGuard[nCntGuard] = true;
	}

	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{
		m_posV = D3DXVECTOR3(0.0f, GAME_CAMERA_Y, GAME_CAMERA_Z);
		m_posR = D3DXVECTOR3(0.0f, GAME_RCAMERA_Y, GAME_RCAMERA_Z);
		m_recU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
	else if (mode == CManager::MODE_RESULT)
	{
		m_posV = D3DXVECTOR3(0.0f, 100.0f, -GAME_CAMERA_Y);	// 視点
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
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();
	// プレイヤー取得
	CPlayer *pPlayer = NULL;
	// 敵取得
	CEnemy *pEnemy = NULL;

	CManager::MODE mode;
	mode = CManager::GetMode();
	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();
	}

	if (mode == CManager::MODE_GAME)
	{
		if (CGame::GetState() == CGame::STATE_START)
		{
			if (pPlayer != NULL && pEnemy != NULL)
			{
				// 入場のカメラワーク
				Start(pPlayer, pEnemy);

				if (pInputKeyboard->GetTrigger(DIK_SPACE) == true
					|| pXInput->GetPress(XPLAYER_A_BUTTON, 0) == true
					|| pXInput->GetPress(XENEMY_A_BUTTON, 1) == true)
				{
					m_State = STATE_NORMAL;
					m_posV = D3DXVECTOR3(0.0f, GAME_CAMERA_Y, GAME_CAMERA_Z);
					m_posR = D3DXVECTOR3(0.0f, GAME_RCAMERA_Y, GAME_RCAMERA_Z);

					if (pPlayer != NULL && pEnemy != NULL)
					{
						pPlayer->SetPosition(PLAYER_POS);
						pEnemy->SetPosition(ENEMY_POS);

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
		}
		else if (CGame::GetState() == CGame::STATE_GAME)
		{// ゲーム中
			if (pPlayer != NULL)
			{
				// プレイヤーの奥義カメラワーク
				PlayerUlt(pPlayer, pEnemy);
			}

			if (pEnemy != NULL)
			{
				// 敵の奥義カメラワーク
				EnemyUlt(pPlayer, pEnemy);
			}
		}
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{
		if (CTutorial::GetState() == CTutorial::STATE_START)
		{
			if (pPlayer != NULL && pEnemy != NULL)
			{
				// 入場のカメラワーク
				Start(pPlayer, pEnemy);

				if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
				{
					m_State = STATE_NORMAL;
					m_posV = D3DXVECTOR3(0.0f, GAME_CAMERA_Y, GAME_CAMERA_Z);
					m_posR = D3DXVECTOR3(0.0f, GAME_RCAMERA_Y, GAME_RCAMERA_Z);

					if (pPlayer != NULL && pEnemy != NULL)
					{
						pPlayer->SetPosition(PLAYER_POS);
						pEnemy->SetPosition(ENEMY_POS);

						pPlayer->SetMotionType(0, CPlayer::MOTION_NEUTRAL);
						pPlayer->SetbMotionEnd(0, true);
						pPlayer->SetMotionType(1, CPlayer::MOTION_NEUTRAL);
						pPlayer->SetbMotionEnd(1, true);
						pEnemy->SetMotionType(0, CEnemy::MOTION_NEUTRAL);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_NEUTRAL);
						pEnemy->SetbMotionEnd(1, true);
					}
					CTutorial::SetState(CTutorial::STATE_GAME);
				}
			}
		}
		else if (CTutorial::GetState() == CTutorial::STATE_GAME)
		{// ゲーム中
			if (pPlayer != NULL)
			{
				// プレイヤーの奥義カメラワーク
				PlayerUlt(pPlayer, pEnemy);
			}

			if (pEnemy != NULL)
			{
				// 敵の奥義カメラワーク
				EnemyUlt(pPlayer, pEnemy);
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
				m_posV = TITLE_POSV;	// 視点
				m_posR = TITLE_POSR;		// 注視点
			}
			else if(pTitle->GetState() == CTitle::STATE_NEWS)
			{
				m_posV = NEWS_POSV;	// 視点
				m_posR = NEWS_POSR;		// 注視点
			}
			else if(pTitle->GetState() == CTitle::STATE_CHARASELECT)
			{
				m_posR = SELECT_CAMERA_POSR;		// 注視点

				if (m_posV.x < SELECT_CAMERA_POSX)
				{
					m_posV.x += 7.0f * SELECT_CAMERA_SPEED;

					if (m_posV.x > SELECT_CAMERA_POSX)
					{
						m_posV.x = SELECT_CAMERA_POSX;
					}
				}
				else
				{
					m_posV.x = SELECT_CAMERA_POSX;
				}

				if (m_posV.y > SELECT_CAMERA_POSY)
				{
					m_posV.y -= 10.0f * SELECT_CAMERA_SPEED;

					if (m_posV.y < SELECT_CAMERA_POSY)
					{
						m_posV.y = SELECT_CAMERA_POSY;
					}
				}
				else
				{
					m_posV.y = SELECT_CAMERA_POSY;
				}

				if (m_posV.z < SELECT_CAMERA_POSZ)
				{
					m_posV.z += 15.0f * SELECT_CAMERA_SPEED;

					if (m_posV.z > SELECT_CAMERA_POSZ)
					{
						m_posV.z = SELECT_CAMERA_POSZ;
					}
				}
				else
				{
					m_posV.z = SELECT_CAMERA_POSZ;
				}
			}
		}
	}
	else if (mode == CManager::MODE_ULTIMATE)
	{
		m_posV = D3DXVECTOR3(-25.0f, 130.0f, -120.0f);	// 視点
		m_posR = D3DXVECTOR3(200.0f, 70.0f, 30.0f);		// 注視点
	}

#if 1
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
//	CDebugProc::Print("cfccfccfc", "posV     : x", m_posV.x, "f", "   y", m_posV.y, "f", " z", m_posV.z, "f");
//	CDebugProc::Print("cfccfccfc", "posR     : x", m_posR.x, "f", "   y", m_posR.y, "f", " z", m_posR.z, "f");
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

//=============================================================================
// 入場カメラワーク
//=============================================================================
void CCamera::Start(CPlayer *pPlayer, CEnemy *pEnemy)
{
	// 秒数
	int nTime = m_nStartCounter / 60;

	m_nStartCounter++;

	if (nTime < HIGASI_TIME && pPlayer != NULL)
	{// 東側の入場
		m_State = STATE_HIGASHI;

		m_posV = D3DXVECTOR3(pPlayer->GetPosition().x + START_CAMERA_X, pPlayer->GetPosition().y + START_CAMERA_Y, pPlayer->GetPosition().z - START_CAMERA_Z);	// 視点
		m_posR = D3DXVECTOR3(pPlayer->GetPosition().x, pPlayer->GetPosition().y + 80.0f, pPlayer->GetPosition().z);		// 注視点
	}
	else if (nTime >= HIGASI_TIME && nTime < NISI_TIME && pEnemy != NULL)
	{// 西側の入場
		m_State = STATE_NISHI;

		m_posV = D3DXVECTOR3(pEnemy->GetPosition().x - START_CAMERA_X, pEnemy->GetPosition().y + START_CAMERA_Y, pEnemy->GetPosition().z - START_CAMERA_Z);	// 視点
		m_posR = D3DXVECTOR3(pEnemy->GetPosition().x, pEnemy->GetPosition().y + 80.0f, pEnemy->GetPosition().z);		// 注視点
	}
	else if (nTime >= NISI_TIME)
	{// カメラの引き
		m_State = STATE_NORMAL;
		m_posV.x = 0.0f;
		m_posR = D3DXVECTOR3(0.0f, GAME_RCAMERA_Y, GAME_RCAMERA_Z);

		if (m_posV.y <= GAME_CAMERA_Y)
		{// y軸移動
			m_posV.y += 2.6f;

			if (m_posV.y > GAME_CAMERA_Y)
			{
				m_posV.y = GAME_CAMERA_Y;
			}
		}

		if (m_posV.z >= GAME_CAMERA_Z)
		{
			m_posV.z -= 10.0f;

			if (m_posV.z < GAME_CAMERA_Z)
			{
				m_posV.z = GAME_CAMERA_Z;
			}
		}

		if (m_posV.y == GAME_CAMERA_Y && m_posV.z == GAME_CAMERA_Z)
		{
			CGame::SetState(CGame::STATE_GAME);
		}
	}
}

//=============================================================================
// プレイヤーの奥義カメラワーク
//=============================================================================
void CCamera::PlayerUlt(CPlayer *pPlayer, CEnemy *pEnemy)
{
	CBattleSys *pBattleSys = CGame::GetBatlteSys();

	if (pPlayer->GetState() == CPlayer::STATE_ULT)
	{// 必殺を打つ時
		if (pPlayer->GetDirection() == CPlayer::DIRECTION_RIGHT)
		{// 右を向いている
			if (m_posV.x <= pPlayer->GetPosition().x + 85.0f)
			{
				m_abGuard[0] = false;
				m_posV.x += 10.0f;

				if (m_posV.x > pPlayer->GetPosition().x + 85.0f)
				{
					m_posV.x = pPlayer->GetPosition().x + 85.0f;
				}
			}
			else if (m_posV.x == pPlayer->GetPosition().x + 85.0f)
			{
				m_abGuard[0] = true;
			}

			if (m_posV.y >= pPlayer->GetPosition().y + 60.0f)
			{
				m_abGuard[1] = false;
				m_posV.y -= 10.0f;

				if (m_posV.y < pPlayer->GetPosition().y + 60.0f)
				{
					m_posV.y = pPlayer->GetPosition().y + 60.0f;
				}
			}
			else if (m_posV.y == pPlayer->GetPosition().y + 60.0f)
			{
				m_abGuard[1] = true;
			}

			if (m_posV.z <= pPlayer->GetPosition().z - 50.0f)
			{
				m_abGuard[2] = false;
				m_posV.z += 15.0f;

				if (m_posV.z > pPlayer->GetPosition().z - 50.0f)
				{
					m_posV.z = pPlayer->GetPosition().z - 50.0f;
				}
			}
			else if (m_posV.z == pPlayer->GetPosition().z - 50.0f)
			{
				m_abGuard[2] = true;
			}

			if (m_abGuard[0] == false || m_abGuard[1] == false || m_abGuard[2] == false)
			{
				m_bGuard = false;
			}
			else if (m_abGuard[0] == true && m_abGuard[1] == true && m_abGuard[2] == true)
			{
				m_bGuard = true;
			}

			m_posR = D3DXVECTOR3(pPlayer->GetPosition().x, pPlayer->GetPosition().y + 80.0f, pPlayer->GetPosition().z);		// 注視点
		}
		else if (pPlayer->GetDirection() == CPlayer::DIRECTION_LEFT)
		{// 左を向いている
			if (m_posV.x >= pPlayer->GetPosition().x - 85.0f)
			{
				m_abGuard[0] = false;
				m_posV.x -= 10.0f;

				if (m_posV.x < pPlayer->GetPosition().x - 85.0f)
				{
					m_posV.x = pPlayer->GetPosition().x - 85.0f;
				}
			}
			else if (m_posV.x == pPlayer->GetPosition().x - 85.0f)
			{
				m_abGuard[0] = true;
			}

			if (m_posV.y >= pPlayer->GetPosition().y + 60.0f)
			{
				m_abGuard[1] = false;
				m_posV.y -= 10.0f;

				if (m_posV.y < pPlayer->GetPosition().y + 60.0f)
				{
					m_posV.y = pPlayer->GetPosition().y + 60.0f;
				}
			}
			else if (m_posV.y == pPlayer->GetPosition().y + 60.0f)
			{
				m_abGuard[1] = true;
			}

			if (m_posV.z <= pPlayer->GetPosition().z - 50.0f)
			{
				m_abGuard[2] = false;
				m_posV.z += 15.0f;

				if (m_posV.z > pPlayer->GetPosition().z - 50.0f)
				{
					m_posV.z = pPlayer->GetPosition().z - 50.0f;
				}
			}
			else if (m_posV.z == pPlayer->GetPosition().z - 50.0f)
			{
				m_abGuard[2] = true;
			}

			if (m_abGuard[0] == false || m_abGuard[1] == false || m_abGuard[2] == false)
			{
				m_bGuard = false;
			}
			else if (m_abGuard[0] == true && m_abGuard[1] == true && m_abGuard[2] == true)
			{
				m_bGuard = true;
			}

			m_posR = D3DXVECTOR3(pPlayer->GetPosition().x, pPlayer->GetPosition().y + 80.0f, pPlayer->GetPosition().z);		// 注視点
		}

		if (pBattleSys != NULL)
		{
			if (pBattleSys->GetUlt(0) == true)
			{
				SetAGuard(true);
				SetGuard(true);
				m_posR = D3DXVECTOR3(pPlayer->GetPosition().x, pPlayer->GetPosition().y + 80.0f, pPlayer->GetPosition().z);		// 注視点

				m_rot.y += 0.03f;

				if (m_rot.y > D3DX_PI)
				{
					m_rot.y -= D3DX_PI * 2.0f;
				}

				m_posV.x = m_posR.x + sinf(D3DX_PI + m_rot.y) * m_fLength;
				m_posV.z = m_posR.z + cosf(D3DX_PI + m_rot.y) * m_fLength;
				m_posV.y = 150.0f;
			}
		}

		if (pEnemy->GetDying() == true)
		{// やられたときのカメラワーク
			m_posR = D3DXVECTOR3(pEnemy->GetPosition().x, pEnemy->GetPosition().y + 20.0f, pEnemy->GetPosition().z);
			if (pPlayer->GetDirection() == CPlayer::DIRECTION_RIGHT)
			{// 右を向いている
				m_posV = D3DXVECTOR3(-25.0f, 130.0f, -120.0f);	// 視点
			}
			if (pPlayer->GetDirection() == CPlayer::DIRECTION_LEFT)
			{// 右を向いている
				m_posV = D3DXVECTOR3(25.0f, 130.0f, -120.0f);	// 視点
			}
		}
	}
}

//=============================================================================
// 敵の奥義カメラワーク
//=============================================================================
void CCamera::EnemyUlt(CPlayer *pPlayer, CEnemy *pEnemy)
{
	CBattleSys *pBattleSys = CGame::GetBatlteSys();

	if (pEnemy->GetState() == CEnemy::STATE_ULT)
	{
		if (pEnemy->GetDirection() == CEnemy::DIRECTION_RIGHT)
		{// 右を向いている
			if (m_posV.x <= pEnemy->GetPosition().x + 85.0f)
			{
				m_abGuard[0] = false;
				m_posV.x += 10.0f;

				if (m_posV.x > pEnemy->GetPosition().x + 85.0f)
				{
					m_posV.x = pEnemy->GetPosition().x + 85.0f;
				}
			}
			else if (m_posV.x == pEnemy->GetPosition().x + 85.0f)
			{
				m_abGuard[0] = true;
			}

			if (m_posV.y >= pEnemy->GetPosition().y + 60.0f)
			{
				m_abGuard[1] = false;
				m_posV.y -= 10.0f;

				if (m_posV.y < pEnemy->GetPosition().y + 60.0f)
				{
					m_posV.y = pEnemy->GetPosition().y + 60.0f;
				}
			}
			else if (m_posV.y == pEnemy->GetPosition().y + 60.0f)
			{
				m_abGuard[1] = true;
			}

			if (m_posV.z <= pEnemy->GetPosition().z - 50.0f)
			{
				m_abGuard[2] = false;
				m_posV.z += 15.0f;

				if (m_posV.z > pEnemy->GetPosition().z - 50.0f)
				{
					m_posV.z = pEnemy->GetPosition().z - 50.0f;
				}
			}
			else if (m_posV.z == pEnemy->GetPosition().z - 50.0f)
			{
				m_abGuard[2] = true;
			}

			if (m_abGuard[0] == false || m_abGuard[1] == false || m_abGuard[2] == false)
			{
				m_bGuard = false;
			}
			else if (m_abGuard[0] == true && m_abGuard[1] == true && m_abGuard[2] == true)
			{
				m_bGuard = true;
			}

			m_posR = D3DXVECTOR3(pEnemy->GetPosition().x, pEnemy->GetPosition().y + 80.0f, pEnemy->GetPosition().z);		// 注視点
		}
		else if (pEnemy->GetDirection() == CEnemy::DIRECTION_LEFT)
		{// 左を向いている
			if (m_posV.x >= pEnemy->GetPosition().x - 85.0f)
			{
				m_abGuard[0] = false;
				m_posV.x -= 10.0f;

				if (m_posV.x < pEnemy->GetPosition().x - 85.0f)
				{
					m_posV.x = pEnemy->GetPosition().x - 85.0f;
				}
			}
			else if (m_posV.x == pEnemy->GetPosition().x - 85.0f)
			{
				m_abGuard[0] = true;
			}

			if (m_posV.y >= pEnemy->GetPosition().y + 60.0f)
			{
				m_abGuard[1] = false;
				m_posV.y -= 10.0f;

				if (m_posV.y < pEnemy->GetPosition().y + 60.0f)
				{
					m_posV.y = pEnemy->GetPosition().y + 60.0f;
				}
			}
			else if (m_posV.y == pEnemy->GetPosition().y + 60.0f)
			{
				m_abGuard[1] = true;
			}

			if (m_posV.z <= pEnemy->GetPosition().z - 50.0f)
			{
				m_abGuard[2] = false;
				m_posV.z += 15.0f;

				if (m_posV.z > pEnemy->GetPosition().z - 50.0f)
				{
					m_posV.z = pEnemy->GetPosition().z - 50.0f;
				}
			}
			else if (m_posV.z == pEnemy->GetPosition().z - 50.0f)
			{
				m_abGuard[2] = true;
			}

			if (m_abGuard[0] == false || m_abGuard[1] == false || m_abGuard[2] == false)
			{
				m_bGuard = false;
			}
			else if (m_abGuard[0] == true && m_abGuard[1] == true && m_abGuard[2] == true)
			{
				m_bGuard = true;
			}

			m_posR = D3DXVECTOR3(pEnemy->GetPosition().x, pEnemy->GetPosition().y + 80.0f, pEnemy->GetPosition().z);		// 注視点
		}

		if (pBattleSys != NULL)
		{
			if (pBattleSys->GetUlt(1) == true)
			{
				SetAGuard(true);
				SetGuard(true);

				m_posR = D3DXVECTOR3(pEnemy->GetPosition().x, pEnemy->GetPosition().y + 80.0f, pEnemy->GetPosition().z);		// 注視点

				m_rot.y += 0.03f;

				if (m_rot.y > D3DX_PI)
				{
					m_rot.y -= D3DX_PI * 2.0f;
				}

				m_posV.x = m_posR.x + sinf(D3DX_PI + m_rot.y) * m_fLength;
				m_posV.z = m_posR.z + cosf(D3DX_PI + m_rot.y) * m_fLength;
				m_posV.y = 150.0f;
			}
		}

		if (pPlayer->GetDying() == true)
		{// やられたときのカメラワーク
			m_posR = D3DXVECTOR3(pPlayer->GetPosition().x, pPlayer->GetPosition().y + 20.0f, pPlayer->GetPosition().z);
			if (pEnemy->GetDirection() == CEnemy::DIRECTION_RIGHT)
			{// 右を向いている
				m_posV = D3DXVECTOR3(-25.0f, 130.0f, -120.0f);	// 視点
			}
			if (pEnemy->GetDirection() == CEnemy::DIRECTION_LEFT)
			{// 右を向いている
				m_posV = D3DXVECTOR3(25.0f, 130.0f, -120.0f);	// 視点
			}
		}
	}
}