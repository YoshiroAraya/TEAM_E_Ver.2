//=============================================================================
//
// �I�u�W�F�N�g���� [Object3DModel.cpp]
// Author : �ڍ� ������
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
#include "load.h"
#include "enemy.h"
#include "ultimate.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================


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
	m_nTypeTex = 0;
	m_pTexture = NULL;
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
C3DObject *C3DObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	C3DObject *pCustomer = NULL;

	if (pCustomer == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pCustomer = new C3DObject;

		if (pCustomer != NULL)
		{
			pCustomer->BindModel(CLoad::GetBuffMat(nType), CLoad::GetNumMat(nType), CLoad::GetMesh(nType));
 			pCustomer->m_nTypeTex = nType;
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
	m_rot = D3DXVECTOR3(sinf(D3DX_PI * CUSTOMER_ROT), cosf(D3DX_PI * CUSTOMER_ROT), cosf(D3DX_PI * CUSTOMER_ROT));


	//// �f�o�C�X���擾
	//LPDIRECT3DDEVICE9 pDevice = NULL;
	//D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^

	//// �}�e���A���f�[�^�ւ̃|�C���^���擾
	//pMat = (D3DXMATERIAL*)CLoad::GetBuffMat(m_nTypeTex)->GetBufferPointer();

	//for (int nCntMat = 0; nCntMat < (int)CLoad::GetNumMat(m_nTypeTex); nCntMat++)
	//{
	//	// �����O�ɋ�ɂ���
	//	//m_pTexture = NULL;

	//	if (pMat[nCntMat].pTextureFilename != NULL)
	//	{
	//		// �e�N�X�`���̐���
	//		D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, m_pTexture);
	//	}
	//}

	//	CLoad::GetBuffMat(m_nTypeTex);

	//m_pTexture = CLoad::GetTexture(m_nTypeTex);

	//CSceneX::BindMat(m_pTexture);

	return S_OK;
}

//=============================================================================
// �I�u�W�F�N�g�I������
//=============================================================================
void C3DObject::Uninit(void)
{
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
		break;
	}

	// �ʒu�̐ݒ�
	CSceneX::SetPosition(pos);
}

//=============================================================================
// �I�u�W�F�N�g�`�揈��
//=============================================================================
void C3DObject::Draw(void)
{
	// 2D�I�u�W�F�N�g�`�揈��
	CSceneX::Draw();
}


////=============================================================================
//// �u���b�N�̃��f���ǂݍ��ݏ���
////=============================================================================
//HRESULT C3DObject::LoadMat(void)
//{
//	// �����_���[���擾
//	CRenderer *pRenderer;
//	pRenderer = CManager::GetRenderer();
//
//	// �f�o�C�X���擾
//	LPDIRECT3DDEVICE9 pDevice = NULL;
//
//	if (pRenderer != NULL)
//	{
//		pDevice = pRenderer->GetDevice();
//	}
//
//	D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^
//
//										// �}�e���A���f�[�^�ւ̃|�C���^���擾
//	pMat = (D3DXMATERIAL*)CLoad::GetBuffMat(m_nTypeTex)->GetBufferPointer();
//
//	// �}�e���A���̐����e�N�X�`����������̂𓮓I�Ɋm��
//	m_pTexture = new LPDIRECT3DTEXTURE9[CLoad::GetNumMat(m_nTypeTex)];
//
//	for (int nCntMat = 0; nCntMat < (int)CLoad::GetNumMat(m_nTypeTex); nCntMat++)
//	{
//		// �����O�ɋ�ɂ���
//		m_pTexture[nCntMat] = NULL;
//
//		if (pMat[nCntMat].pTextureFilename != NULL)
//		{
//			// �e�N�X�`���̐���
//			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &m_pTexture[nCntMat]);
//		}
//	}
//
//	return S_OK;
//}
//
////=============================================================================
//// �u���b�N�̃��f���������
////=============================================================================
//void C3DObject::UnloadMat(void)
//{
//	if (m_pTexture != NULL)
//	{// �e�N�X�`���̃|�C���^��NULL�`�F�b�N(��)
//		for (int nCntMat = 0; nCntMat < (int)CLoad::GetNumMat(m_nTypeTex); nCntMat++)
//		{
//			if (m_pTexture[nCntMat] != NULL)
//			{// �|�C���^�̒���NULL�`�F�b�N(�Ƌ�)
//				m_pTexture[nCntMat]->Release();
//				m_pTexture[nCntMat] = NULL;
//			}
//		}
//
//		// �������̊J��(���)
//		delete[] m_pTexture;
//		// NULL�ɂ���(�X�n)
//		m_pTexture = NULL;
//	}
//}