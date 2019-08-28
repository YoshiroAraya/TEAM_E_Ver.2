//=============================================================================
//
// �v���C���[���� [dohyo.cpp]
// Author : ���R���
//
//=============================================================================
#include "customer.h"
#include "input.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "camera.h"
#include "scene3D.h"
#include "meshField.h"
#include "shadow.h"
#include "load.h"
#include "enemy.h"
#include "ultimate.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 *CCustomer::m_pTexture = NULL;				// �e�N�X�`��

#define CUSTOMER_ROT			((rand() % 628) / 100.0f)		//�S����

//=============================================================================
// �v���C���[�N���X�̃R���X�g���N�^
//=============================================================================
CCustomer::CCustomer() : CSceneX(DOHYO_PRIORITY)
{
	// �l���N���A
	m_pVtxBuff = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCustomer::~CCustomer()
{

}

//=============================================================================
// �I�u�W�F�N�g�̐�������
//=============================================================================
CCustomer *CCustomer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, CUSTOMER_POS customerpos)
{
	CCustomer *pCustomer = NULL;

	if (pCustomer == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pCustomer = new CCustomer;

		if (pCustomer != NULL)
		{
			pCustomer->BindModel(CLoad::GetBuffMat(nType), CLoad::GetNumMat(nType), CLoad::GetMesh(nType));
			//pCustomer->BindMat(m_pTexture);
			pCustomer->Init(pos, rot);
			pCustomer->m_CustomerPos = customerpos;
		}
	}

	return pCustomer;
}

//=============================================================================
// �v���C���[����������
//=============================================================================
HRESULT CCustomer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �I�u�W�F�N�g����������
	CSceneX::Init(pos);

	//�I�u�W�F�N�g�̌���
	CSceneX::SetRot(rot);

	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(CScene::OBJTYPE_CUSTOMER);
	m_move = D3DXVECTOR3(sinf((rand() % 628) / 100.0f) * ((rand() % 7 + 1)), -1.0f, cosf((rand() % 628) / 100.0f) * ((rand() % 7 + 1)));
	m_rot = D3DXVECTOR3(sinf(D3DX_PI * CUSTOMER_ROT), cosf(D3DX_PI * CUSTOMER_ROT), cosf(D3DX_PI * CUSTOMER_ROT));

	return S_OK;
}

//=============================================================================
// �v���C���[�I������
//=============================================================================
void CCustomer::Uninit(void)
{
	// 2D�I�u�W�F�N�g�I������
	CSceneX::Uninit();
}

//=============================================================================
// �v���C���[�X�V����
//=============================================================================
void CCustomer::Update(void)
{
	CManager::MODE mode;
	mode = CManager::GetMode();

	// �G�擾
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();
	// �v���C���[�擾
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();

	// �ʒu���擾
	D3DXVECTOR3 pos;
	pos = CSceneX::GetPosition();

	switch (mode)
	{
	case CManager::MODE_GAME:
	/*	if (m_CustomerPos == POSITION_RIGHT)
		{
			if (pEnemy != NULL)
			{
				if (pEnemy->GetWallHit() == true)
				{
					pos -= m_move * 3.5f;

					CSceneX::SetRot(m_rot);
				}
			}
		}*/

		if (pEnemy->GetDying() == true)
		{// �G�����񂾂Ƃ�
			if (pEnemy->GetDirection() == CEnemy::DIRECTION_LEFT)
			{// ���������Ă�����
				if (m_CustomerPos == POSITION_RIGHT)
				{
					pos -= m_move * 3.5f;

					CSceneX::SetRot(m_rot);
				}
			}
			else if(pEnemy->GetDirection() == CEnemy::DIRECTION_RIGHT)
			{
				if (m_CustomerPos == POSITION_LEFT)
				{
					pos -= m_move * 3.5f;

					CSceneX::SetRot(m_rot);
				}
			}
		}
		break;
	}

	// �ʒu�̐ݒ�
	CSceneX::SetPosition(pos);
}

//=============================================================================
// �v���C���[�`�揈��
//=============================================================================
void CCustomer::Draw(void)
{
	// 2D�I�u�W�F�N�g�`�揈��
	CSceneX::Draw();
}