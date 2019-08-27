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
#include "load.h"
#include "enemy.h"
#include "ultimate.h"

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