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

//=============================================================================
// �L�����N�^�[�̈ړ��̃R���X�g���N�^
//=============================================================================
CCharacterMove::CCharacterMove()
{
	// �l���N���A
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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