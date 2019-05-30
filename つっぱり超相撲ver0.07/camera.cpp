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

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CAMERA_SPEED	(2.0f)

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
	m_posV = D3DXVECTOR3(0.0f, 200.0f, -280.0f);	// ���_
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����_
	m_recU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = sqrtf((m_posV.x - m_posR.x) * (m_posV.x - m_posR.x) + (m_posV.z - m_posR.z) * (m_posV.z - m_posR.z));
	m_nStartCounter = 0;
	m_State = STATE_NONE;
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
	// �v���C���[�擾
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	// �G�擾
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{
		if (CGame::GetState() == CGame::STATE_START)
		{
			int nTime = m_nStartCounter / 60;

			m_nStartCounter++;

			if (nTime < 3 && pPlayer != NULL)
			{
				m_State = STATE_HIGASHI;
				m_posV = D3DXVECTOR3(pPlayer->GetPosition().x + 35.0f, pPlayer->GetPosition().y + 17.0f, pPlayer->GetPosition().z - 40.0f);	// ���_
				m_posR = D3DXVECTOR3(pPlayer->GetPosition().x, pPlayer->GetPosition().y + 18.0f, pPlayer->GetPosition().z);		// �����_
			}
			else if (nTime >= 3 && nTime < 6 && pEnemy != NULL)
			{
				m_State = STATE_NISHI;
				m_posV = D3DXVECTOR3(pEnemy->GetPosition().x - 35.0f, pEnemy->GetPosition().y + 17.0f, pEnemy->GetPosition().z - 40.0f);	// ���_
				m_posR = D3DXVECTOR3(pEnemy->GetPosition().x, pEnemy->GetPosition().y + 18.0f, pEnemy->GetPosition().z);		// �����_
			}
			else if (nTime >= 6)
			{
				m_State = STATE_NORMAL;
				m_posV.x = 0.0f;
				m_posR = D3DXVECTOR3(0.0f, 0.0f, 50.0f);

				if (m_posV.y <= 200.0f)
				{// y���ړ�
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
				pPlayer->SetPosition(D3DXVECTOR3(-20.0f, 30.0f, 0.0f));
				pEnemy->SetPosition(D3DXVECTOR3(20.0f, 30.0f, 0.0f));

				CGame::SetState(CGame::STATE_GAME);
			}
		}
	}

	if (mode == CManager::MODE_TITLE)
	{
		m_posV = D3DXVECTOR3(0.0f, 25.0f, -200.0f);	// ���_
		m_posR = D3DXVECTOR3(0.0f, 25.0f, 0.0f);		// �����_
	}

#if 0
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
	CDebugProc::Print("cfccfccfc", "posV     : x", m_posV.x, "f", "   y", m_posV.y, "f", " z", m_posV.z, "f");
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
		1000.0f);									// �J��������̋����i���j

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