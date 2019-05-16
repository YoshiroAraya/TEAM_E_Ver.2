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

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CAMERA_SPEED	(4.0f)

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
	m_posV = D3DXVECTOR3(0.0f, 120.0f, -220.0f);	// ���_
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����_
	m_recU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = sqrtf((m_posV.x - m_posR.x) * (m_posV.x - m_posR.x) + (m_posV.z - m_posR.z) * (m_posV.z - m_posR.z));
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
	//CDebugProc::Print("cfccfccfc", "posV     : x", m_posV.x, "f", "   y", m_posV.y, "f", " z", m_posV.z, "f");
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