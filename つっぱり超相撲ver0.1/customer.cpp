//=============================================================================
//
// �v���C���[���� [dohyo.cpp]
// Author : ���R���
//
//=============================================================================
#include "customer.h"
#include "input.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "camera.h"
#include "bullet.h"
#include "scene3D.h"
#include "meshField.h"
#include "shadow.h"
#include "loadModel.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 *CCustomer::m_pTexture = NULL;				// �e�N�X�`��

//=============================================================================
// �v���C���[�N���X�̃R���X�g���N�^
//=============================================================================
CCustomer::CCustomer() : CSceneX(DOHYO_PRIORITY)
{
	// �l���N���A
	m_pVtxBuff = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
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
CCustomer *CCustomer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	CCustomer *pCustomer = NULL;

	if (pCustomer == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pCustomer = new CCustomer;

		if (pCustomer != NULL)
		{
			pCustomer->BindModel(CLoadModel::GetBuffMat(nType), CLoadModel::GetNumMat(nType), CLoadModel::GetMesh(nType));
			//pCustomer->BindMat(m_pTexture);
			pCustomer->Init(pos, rot);
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
	SetObjType(CScene::OBJTYPE_DOHYO);

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

}

//=============================================================================
// �v���C���[�`�揈��
//=============================================================================
void CCustomer::Draw(void)
{
	// 2D�I�u�W�F�N�g�`�揈��
	CSceneX::Draw();
}