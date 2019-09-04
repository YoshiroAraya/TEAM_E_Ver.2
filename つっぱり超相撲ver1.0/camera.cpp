//=============================================================================
//
// �J�������� [camera.cpp]
// Author : ���R���
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
// �}�N����`
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
// �ÓI�����o�ϐ��錾
//=============================================================================
CCamera::STATE CCamera::m_State = STATE_NONE;

//=============================================================================
// �J�����N���X�̃R���X�g���N�^
//=============================================================================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���_
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����_
	m_recU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	m_fLength = 0.0f;
	m_nStartCounter = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
// �J�����̏���������
//=============================================================================
void CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(0.0f, GAME_CAMERA_Y, GAME_CAMERA_Z);	// ���_
	m_posR = D3DXVECTOR3(0.0f, GAME_RCAMERA_Y, GAME_RCAMERA_Z);		// �����_
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
		m_posV = D3DXVECTOR3(0.0f, 100.0f, -GAME_CAMERA_Y);	// ���_
		m_posR = D3DXVECTOR3(0.0f, 80.0f, 0.0f);		// �����_
		m_recU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void CCamera::Update(void)
{
	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();
	// �v���C���[�擾
	CPlayer *pPlayer = NULL;
	// �G�擾
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
				// ����̃J�������[�N
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
		{// �Q�[����
			if (pPlayer != NULL)
			{
				// �v���C���[�̉��`�J�������[�N
				PlayerUlt(pPlayer, pEnemy);
			}

			if (pEnemy != NULL)
			{
				// �G�̉��`�J�������[�N
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
				// ����̃J�������[�N
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
		{// �Q�[����
			if (pPlayer != NULL)
			{
				// �v���C���[�̉��`�J�������[�N
				PlayerUlt(pPlayer, pEnemy);
			}

			if (pEnemy != NULL)
			{
				// �G�̉��`�J�������[�N
				EnemyUlt(pPlayer, pEnemy);
			}
		}
	}

	else if (mode == CManager::MODE_TITLE)
	{
		// �^�C�g���擾
		CTitle *pTitle;
		pTitle = CManager::GetTitle();

		if (pTitle != NULL)
		{
			if (pTitle->GetState() == CTitle::STATE_TITLE)
			{
				m_posV = TITLE_POSV;	// ���_
				m_posR = TITLE_POSR;		// �����_
			}
			else if(pTitle->GetState() == CTitle::STATE_NEWS)
			{
				m_posV = NEWS_POSV;	// ���_
				m_posR = NEWS_POSR;		// �����_
			}
			else if(pTitle->GetState() == CTitle::STATE_CHARASELECT)
			{
				m_posR = SELECT_CAMERA_POSR;		// �����_

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
		m_posV = D3DXVECTOR3(-25.0f, 130.0f, -120.0f);	// ���_
		m_posR = D3DXVECTOR3(200.0f, 70.0f, 30.0f);		// �����_
	}

#if 1
	if (pInputKeyboard->GetPress(DIK_A) == true)
	{// �������Ɉړ�
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{// �����Ɉړ�
			m_posV.x -= sinf(m_rot.y + D3DX_PI * 0.75f) * CAMERA_SPEED;
			m_posV.z -= cosf(m_rot.y + D3DX_PI * 0.75f) * CAMERA_SPEED;

			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{// ����O�ړ�
			m_posV.x -= sinf(m_rot.y + D3DX_PI * 0.25f) * CAMERA_SPEED;
			m_posV.z -= cosf(m_rot.y + D3DX_PI * 0.25f) * CAMERA_SPEED;

			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		}
		else
		{// ��
			m_posV.x -= sinf(m_rot.y + D3DX_PI * 0.5f) * CAMERA_SPEED;
			m_posV.z -= cosf(m_rot.y + D3DX_PI * 0.5f) * CAMERA_SPEED;

			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true)
	{// �E�����Ɉړ�
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{// �E�O�ړ�
			m_posV.x -= sinf(m_rot.y - D3DX_PI * 0.75f) * CAMERA_SPEED;
			m_posV.z -= cosf(m_rot.y - D3DX_PI * 0.75f) * CAMERA_SPEED;

			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{// �E���ړ�
			m_posV.x -= sinf(m_rot.y - D3DX_PI * 0.25f) * CAMERA_SPEED;
			m_posV.z -= cosf(m_rot.y - D3DX_PI * 0.25f) * CAMERA_SPEED;

			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		}
		else
		{// �E
			m_posV.x -= sinf(m_rot.y - D3DX_PI * 0.5f) * CAMERA_SPEED;
			m_posV.z -= cosf(m_rot.y - D3DX_PI * 0.5f) * CAMERA_SPEED;

			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true)
	{// �O�����Ɉړ�
		m_posV.x += sinf(m_rot.y) * CAMERA_SPEED;
		m_posV.z += cosf(m_rot.y) * CAMERA_SPEED;

		m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true)
	{// ��O�����Ɉړ�
		m_posV.x -= sinf(m_rot.y) * CAMERA_SPEED;
		m_posV.z -= cosf(m_rot.y) * CAMERA_SPEED;

		m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
	}

	// �����_��]
	if (pInputKeyboard->GetPress(DIK_Q) == true)
	{// �������ɉ�]
		m_rot.y -= 0.03f;

		if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI * 2.0f;
		}

		m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
	}
	else if (pInputKeyboard->GetPress(DIK_E) == true)
	{// �������ɉ�]
		m_rot.y += 0.03f;

		if (m_rot.y > D3DX_PI)
		{
			m_rot.y -= D3DX_PI * 2.0f;
		}

		m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
	}
	else if (pInputKeyboard->GetPress(DIK_T) == true)
	{// ��
		m_rot.x += 3.0f;

		m_posR.y = m_rot.x;
	}
	else if (pInputKeyboard->GetPress(DIK_B) == true)
	{// ��
		m_rot.x -= 3.0f;

		m_posR.y = m_rot.x;
	}

	// ���_��]
	if (pInputKeyboard->GetPress(DIK_Z) == true)
	{// �������ɉ�]
		m_rot.y -= 0.03f;

		if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI * 2.0f;
		}

		m_posV.x = m_posR.x + sinf(D3DX_PI + m_rot.y) * m_fLength;
		m_posV.z = m_posR.z + cosf(D3DX_PI + m_rot.y) * m_fLength;
	}
	else if (pInputKeyboard->GetPress(DIK_C) == true)
	{// �������ɉ�]
		m_rot.y += 0.03f;

		if (m_rot.y > D3DX_PI)
		{
			m_rot.y -= D3DX_PI * 2.0f;
		}

		m_posV.x = m_posR.x + sinf(D3DX_PI + m_rot.y) * m_fLength;
		m_posV.z = m_posR.z + cosf(D3DX_PI + m_rot.y) * m_fLength;
	}
	else if (pInputKeyboard->GetPress(DIK_Y) == true)
	{// ��
		m_posV.y += 3.0f;
	}
	else if (pInputKeyboard->GetPress(DIK_N) == true)
	{// ��
		m_posV.y -= 3.0f;
	}
#endif

#ifdef _DEBUG
//	CDebugProc::Print("cfccfccfc", "posV     : x", m_posV.x, "f", "   y", m_posV.y, "f", " z", m_posV.z, "f");
//	CDebugProc::Print("cfccfccfc", "posR     : x", m_posR.x, "f", "   y", m_posR.y, "f", " z", m_posR.z, "f");
#endif
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void CCamera::SetCamera(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),						// ����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// ��ʔ䗦
		10.0f,										// �J��������̋����i��O�j
		10000.0f);									// �J��������̋����i���j

													// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_recU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// �����̎擾
//=============================================================================
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// ����J�������[�N
//=============================================================================
void CCamera::Start(CPlayer *pPlayer, CEnemy *pEnemy)
{
	// �b��
	int nTime = m_nStartCounter / 60;

	m_nStartCounter++;

	if (nTime < HIGASI_TIME && pPlayer != NULL)
	{// �����̓���
		m_State = STATE_HIGASHI;

		m_posV = D3DXVECTOR3(pPlayer->GetPosition().x + START_CAMERA_X, pPlayer->GetPosition().y + START_CAMERA_Y, pPlayer->GetPosition().z - START_CAMERA_Z);	// ���_
		m_posR = D3DXVECTOR3(pPlayer->GetPosition().x, pPlayer->GetPosition().y + 80.0f, pPlayer->GetPosition().z);		// �����_
	}
	else if (nTime >= HIGASI_TIME && nTime < NISI_TIME && pEnemy != NULL)
	{// �����̓���
		m_State = STATE_NISHI;

		m_posV = D3DXVECTOR3(pEnemy->GetPosition().x - START_CAMERA_X, pEnemy->GetPosition().y + START_CAMERA_Y, pEnemy->GetPosition().z - START_CAMERA_Z);	// ���_
		m_posR = D3DXVECTOR3(pEnemy->GetPosition().x, pEnemy->GetPosition().y + 80.0f, pEnemy->GetPosition().z);		// �����_
	}
	else if (nTime >= NISI_TIME)
	{// �J�����̈���
		m_State = STATE_NORMAL;
		m_posV.x = 0.0f;
		m_posR = D3DXVECTOR3(0.0f, GAME_RCAMERA_Y, GAME_RCAMERA_Z);

		if (m_posV.y <= GAME_CAMERA_Y)
		{// y���ړ�
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
// �v���C���[�̉��`�J�������[�N
//=============================================================================
void CCamera::PlayerUlt(CPlayer *pPlayer, CEnemy *pEnemy)
{
	CBattleSys *pBattleSys = CGame::GetBatlteSys();

	if (pPlayer->GetState() == CPlayer::STATE_ULT)
	{// �K�E��ł�
		if (pPlayer->GetDirection() == CPlayer::DIRECTION_RIGHT)
		{// �E�������Ă���
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

			m_posR = D3DXVECTOR3(pPlayer->GetPosition().x, pPlayer->GetPosition().y + 80.0f, pPlayer->GetPosition().z);		// �����_
		}
		else if (pPlayer->GetDirection() == CPlayer::DIRECTION_LEFT)
		{// ���������Ă���
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

			m_posR = D3DXVECTOR3(pPlayer->GetPosition().x, pPlayer->GetPosition().y + 80.0f, pPlayer->GetPosition().z);		// �����_
		}

		if (pBattleSys != NULL)
		{
			if (pBattleSys->GetUlt(0) == true)
			{
				SetAGuard(true);
				SetGuard(true);
				m_posR = D3DXVECTOR3(pPlayer->GetPosition().x, pPlayer->GetPosition().y + 80.0f, pPlayer->GetPosition().z);		// �����_

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
		{// ���ꂽ�Ƃ��̃J�������[�N
			m_posR = D3DXVECTOR3(pEnemy->GetPosition().x, pEnemy->GetPosition().y + 20.0f, pEnemy->GetPosition().z);
			if (pPlayer->GetDirection() == CPlayer::DIRECTION_RIGHT)
			{// �E�������Ă���
				m_posV = D3DXVECTOR3(-25.0f, 130.0f, -120.0f);	// ���_
			}
			if (pPlayer->GetDirection() == CPlayer::DIRECTION_LEFT)
			{// �E�������Ă���
				m_posV = D3DXVECTOR3(25.0f, 130.0f, -120.0f);	// ���_
			}
		}
	}
}

//=============================================================================
// �G�̉��`�J�������[�N
//=============================================================================
void CCamera::EnemyUlt(CPlayer *pPlayer, CEnemy *pEnemy)
{
	CBattleSys *pBattleSys = CGame::GetBatlteSys();

	if (pEnemy->GetState() == CEnemy::STATE_ULT)
	{
		if (pEnemy->GetDirection() == CEnemy::DIRECTION_RIGHT)
		{// �E�������Ă���
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

			m_posR = D3DXVECTOR3(pEnemy->GetPosition().x, pEnemy->GetPosition().y + 80.0f, pEnemy->GetPosition().z);		// �����_
		}
		else if (pEnemy->GetDirection() == CEnemy::DIRECTION_LEFT)
		{// ���������Ă���
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

			m_posR = D3DXVECTOR3(pEnemy->GetPosition().x, pEnemy->GetPosition().y + 80.0f, pEnemy->GetPosition().z);		// �����_
		}

		if (pBattleSys != NULL)
		{
			if (pBattleSys->GetUlt(1) == true)
			{
				SetAGuard(true);
				SetGuard(true);

				m_posR = D3DXVECTOR3(pEnemy->GetPosition().x, pEnemy->GetPosition().y + 80.0f, pEnemy->GetPosition().z);		// �����_

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
		{// ���ꂽ�Ƃ��̃J�������[�N
			m_posR = D3DXVECTOR3(pPlayer->GetPosition().x, pPlayer->GetPosition().y + 20.0f, pPlayer->GetPosition().z);
			if (pEnemy->GetDirection() == CEnemy::DIRECTION_RIGHT)
			{// �E�������Ă���
				m_posV = D3DXVECTOR3(-25.0f, 130.0f, -120.0f);	// ���_
			}
			if (pEnemy->GetDirection() == CEnemy::DIRECTION_LEFT)
			{// �E�������Ă���
				m_posV = D3DXVECTOR3(25.0f, 130.0f, -120.0f);	// ���_
			}
		}
	}
}