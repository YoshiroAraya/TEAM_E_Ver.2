//=============================================================================
//
// �L�����N�^�[�̈ړ����� [characterMove.cpp]
// Author : ���R���
//
//=============================================================================
#include "characterMove.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "camera.h"
#include "title.h"

//=============================================================================
// �L�����N�^�[�̈ړ��̃R���X�g���N�^
//=============================================================================
CCharacterMove::CCharacterMove()
{
	// �l���N���A
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0.0f;
	//m_fRot = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCharacterMove::~CCharacterMove()
{
}

//=============================================================================
// �I�u�W�F�N�g�̐�������
//=============================================================================
CCharacterMove *CCharacterMove::Create()
{
	CCharacterMove *pCharacterMove = NULL;

	if (pCharacterMove == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pCharacterMove = new CCharacterMove;

		if (pCharacterMove != NULL)
		{
			pCharacterMove->Init();
		}
	}

	return pCharacterMove;
}

//=============================================================================
// �o�g���V�X�e������������
//=============================================================================
HRESULT CCharacterMove::Init()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0.0f;
	//m_fRot = 0.0f;

	return S_OK;
}

//=============================================================================
// �o�g���V�X�e���I������
//=============================================================================
void CCharacterMove::Uninit(void)
{
}

//=============================================================================
// �E�ɐi��
//=============================================================================
D3DXVECTOR3 CCharacterMove::MoveRight(D3DXVECTOR3 move, float fSpeed)
{
	// �J�����擾
	CCamera *pCamera;
	pCamera = CManager::GetCamera();
	// �J�����̌������擾
	D3DXVECTOR3 cameraRot;
	cameraRot = pCamera->GetRot();

	move.x -= sinf(cameraRot.y - D3DX_PI * 0.5f) * fSpeed;

	return move;
}

//=============================================================================
// ���ɐi��
//=============================================================================
D3DXVECTOR3 CCharacterMove::MoveLeft(D3DXVECTOR3 move, float fSpeed)
{
	// �J�����擾
	CCamera *pCamera;
	pCamera = CManager::GetCamera();
	// �J�����̌������擾
	D3DXVECTOR3 cameraRot;
	cameraRot = pCamera->GetRot();

	move.x -= sinf(cameraRot.y + D3DX_PI * 0.5f) * fSpeed;

	return move;
}

//=============================================================================
// �L������]
//=============================================================================
bool CCharacterMove::CharaTurn(D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, float fRot, float fLength)
{
	// �^�C�g���擾
	CTitle *pTitle;
	pTitle = CManager::GetTitle();

	bool bSelect = false;

	if (pTitle != NULL)
	{
		if (pTitle->GetState() == CTitle::STATE_CHARASELECT && pTitle->GetTurnR() == true || pTitle->GetTurnL() == true)
		{
			if (pTitle->GetTurnR() == true)
			{// �E�ɉ�]
				rot->y -= 0.1f;
			}
			else if (pTitle->GetTurnL() == true)
			{// ���ɉ�]
				rot->y += 0.1f;
			}

			if (rot->y > D3DX_PI)
			{
				rot->y -= D3DX_PI* 2.0f;
			}
			if (rot->y < -D3DX_PI)
			{
				rot->y += D3DX_PI* 2.0f;
			}

			if (fRot >= 0.999f && fRot < 1.0f)
			{// ���̔���
				if (pTitle->GetTurnR() == true)
				{// ��]���~�߂�
					pTitle->SetTurnR(false);
				}
				else if (pTitle->GetTurnL() == true)
				{// ��]���~�߂�
					pTitle->SetTurnL(false);
				}

				// ������C��
				fRot = 1.0f;
				rot->y = D3DX_PI * -0.5f;
			}
			else if (fRot <= -0.999f && fRot > -1.0f)
			{// ��O�̔���
				if (pTitle->GetTurnR() == true)
				{// ��]���~�߂�
					pTitle->SetTurnR(false);
				}
				else if (pTitle->GetTurnL() == true)
				{// ��]���~�߂�
					pTitle->SetTurnL(false);
				}

				// ������C��
				fRot = -1.0f;
				rot->y = D3DX_PI * 0.5f;
			}

			// �~�ɉ����Ĉʒu�𓮂���
			pos->x = 0.0f + fRot * fLength;
			pos->z = 0.0f + cosf(D3DX_PI + rot->y) * fLength;
		}
		else if (pTitle->GetState() == CTitle::STATE_CHARASELECT && pTitle->GetTurnR() == false || pTitle->GetTurnL() == false)
		{// ��]���Ă��Ȃ��Ƃ�
			if (fRot >= 0.999f && fRot <= 1.0f)
			{// ���̔���
				// ������C��
				fRot = 1.0f;
				rot->y = D3DX_PI * -0.5f;

				// �I�����Ă��Ȃ�
				bSelect = false;
			}
			else if (fRot <= -0.999f && fRot >= -1.0f)
			{// ��O�̔���
				// ������C��
				fRot = -1.0f;
				rot->y = D3DX_PI * 0.5f;

				// �I������Ă���
				bSelect = true;
			}

			pos->x = 0.0f + fRot * fLength;
			pos->z = 0.0f + cosf(D3DX_PI + rot->y) * fLength;
		}
	}

	return bSelect;
}