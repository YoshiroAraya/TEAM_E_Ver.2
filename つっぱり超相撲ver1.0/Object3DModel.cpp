//=============================================================================
//
// �I�u�W�F�N�g���� [Object3DModel.cpp]
// Author : ���R���
//
//=============================================================================
#include "Object3DModel.h"
#include "input.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "camera.h"
#include "scene3D.h"
#include "meshField.h"
#include "shadow.h"
#include "enemy.h"
#include "ultimate.h"

#include "player.h"
#include "enemy.h"
//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 *C3DObject::m_pTexture = NULL;				// �e�N�X�`��

#define CUSTOMER_ROT			((rand() % 628) / 100.0f)		//�S����

//=============================================================================
// �I�u�W�F�N�g�N���X�̃R���X�g���N�^
//=============================================================================
C3DObject::C3DObject() : CSceneX(DOHYO_PRIORITY)
{
	// �l���N���A
	m_pVtxBuff = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pBuffMat = NULL;		//�}�e���A�����ւ̃|�C���^
	m_pTexture = NULL;		//�e�N�X�`���ւ̃|�C���^
	m_nNumMat = 0;			//�}�e���A�����̐�
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
C3DObject::~C3DObject()
{

}

//=============================================================================
// �I�u�W�F�N�g�̐�������
//=============================================================================
C3DObject *C3DObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE nObjType, CLoad::MODEL nModelType)
{
	C3DObject *pCustomer = NULL;

	if (pCustomer == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pCustomer = new C3DObject;

		if (pCustomer != NULL)
		{
			pCustomer->BindModel(CLoad::GetBuffMat(nModelType), CLoad::GetNumMat(nModelType), CLoad::GetMesh(nModelType));
			pCustomer->m_Type = nObjType;
			pCustomer->Init(pos, rot);
		}
	}

	return pCustomer;
}

//=============================================================================
// �I�u�W�F�N�g����������
//=============================================================================
HRESULT C3DObject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �I�u�W�F�N�g����������
	CSceneX::Init(pos);

	//�I�u�W�F�N�g�̌���
	CSceneX::SetRot(rot);

	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(CScene::OBJTYPE_DOHYO);
	m_move = D3DXVECTOR3(sinf((rand() % 628) / 100.0f) * ((rand() % 7 + 1)), -1.0f, cosf((rand() % 628) / 100.0f) * ((rand() % 7 + 1)));
	//m_rot = D3DXVECTOR3(sinf(D3DX_PI * CUSTOMER_ROT), cosf(D3DX_PI * CUSTOMER_ROT), cosf(D3DX_PI * CUSTOMER_ROT));

	if (m_Type == OBJECTTYPE_GYOUZI)
	{
		//�f�o�C�X���擾
		CRenderer *pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^

		m_pBuffMat = CLoad::GetBuffMat(CLoad::MODEL_GYOUZI);
		m_nNumMat = CLoad::GetNumMat(CLoad::MODEL_GYOUZI);

		//�}�e���A����񂩂�e�N�X�`���̎擾
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
		//�e�N�X�`�����}�e���A���̐������I�m��
		m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];
		//�}�e���A���̐���
		for (int nCntMatTex = 0; nCntMatTex < (int)m_nNumMat; nCntMatTex++)
		{
			//NULL������ ���g�����
			m_pTexture[nCntMatTex] = NULL;

			if (pMat[nCntMatTex].pTextureFilename != NULL)
			{
				// �e�N�X�`���̐ݒ�
				D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
					pMat[nCntMatTex].pTextureFilename,	// �t�@�C���̖��O
					&m_pTexture[nCntMatTex]);		// �e�N�X�`���ւ̃|�C���^
			}
		}

		CSceneX::BindMat(m_pTexture);
	}


	return S_OK;
}

//=============================================================================
// �I�u�W�F�N�g�I������
//=============================================================================
void C3DObject::Uninit(void)
{
	if (m_pTexture != NULL)
	{
		for (int nCnt = 0; nCnt < (int)m_nNumMat; nCnt++)
		{
			//�|�C���^���̃|�C���^��NULL�`�F�b�N (�Ƌ�)
			if (m_pTexture[nCnt] != NULL)
			{
				//�e�N�X�`���j��
				m_pTexture[nCnt]->Release();
				m_pTexture[nCnt] = NULL;
			}
		}
		//���������J�� (���)
		delete[] m_pTexture;
		//NULL������ (�X�n)
		m_pTexture = NULL;
	}

	// 2D�I�u�W�F�N�g�I������
	CSceneX::Uninit();
}

//=============================================================================
// �I�u�W�F�N�g�X�V����
//=============================================================================
void C3DObject::Update(void)
{
	CManager::MODE mode;
	mode = CManager::GetMode();

	// �G�擾
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

	// �ʒu���擾
	D3DXVECTOR3 pos;
	pos = CSceneX::GetPosition();

	switch (mode)
	{
	case CManager::MODE_GAME:

		D3DXVECTOR3 p1pos, p2pos;
		// �v���C���[�̎擾
		CPlayer *pPlayer = NULL;
		pPlayer = CGame::GetPlayer();
		// �G�l�~�[�̎擾
		CEnemy *pEnemy = NULL;
		pEnemy = CGame::GetEnemy();

		if (pPlayer != NULL)
		{	//�v���C���[1�̈ʒu���擾
			p1pos = pPlayer->GetPosition();
		}
		if (pPlayer != NULL)
		{	//�v���C���[2�̈ʒu���擾
			p2pos = pEnemy->GetPosition();
		}

		float DiffDis = (p1pos.x + p2pos.x) / 2;
		D3DXVECTOR3 testpos((p1pos.x + p2pos.x) / 2, (p1pos.y + p2pos.y) / 2, (p1pos.z + p2pos.z) / 2);
		// �ړI�̊p�x
		if (pEnemy != NULL)
		{
			m_fDestAngle = atan2f((testpos.x - sinf(m_rot.y)) - pos.x, (testpos.z - cosf(m_rot.y)) - pos.z);
			m_fDestAngle += D3DX_PI;
		}
		// ����
		m_fDiffAngle = m_fDestAngle - m_rot.y;

		if (m_fDiffAngle > D3DX_PI)
		{
			m_fDiffAngle -= D3DX_PI * 2.0f;
		}
		if (m_fDiffAngle < -D3DX_PI)
		{
			m_fDiffAngle += D3DX_PI * 2.0f;
		}

		m_rot.y += m_fDiffAngle * 0.1f;

		if (m_rot.y > D3DX_PI)
		{
			m_rot.y -= D3DX_PI * 2.0f;
		}
		if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI * 2.0f;
		}

		//�����̊���
		m_fDiffAngle = m_fDestAngle - m_rot.y;

		//�p�x�̐ݒ�
		if (m_fDiffAngle > D3DX_PI)
		{
			m_fDiffAngle -= D3DX_PI* 2.0f;
		}
		if (m_fDiffAngle < -D3DX_PI)
		{
			m_fDiffAngle += D3DX_PI* 2.0f;
		}

		m_rot.y += m_fDiffAngle * 0.1f;

		if (m_rot.y > D3DX_PI)
		{
			m_rot.y -= D3DX_PI* 2.0f;
		}
		if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI* 2.0f;
		}
		break;
	}

	// �ʒu�̐ݒ�
	CSceneX::SetPosition(pos);
	CSceneX::SetRot(m_rot);

}

//=============================================================================
// �I�u�W�F�N�g�`�揈��
//=============================================================================
void C3DObject::Draw(void)
{
	// 2D�I�u�W�F�N�g�`�揈��
	CSceneX::Draw();
}