//=============================================================================
//
// �e�̏���
// Author : ���R���
//
//=============================================================================
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "player.h"
#include "mask.h"
#include "load.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//--------------------------------------------
//�ÓI�����o�ϐ��錾
//--------------------------------------------
LPDIRECT3DTEXTURE9			CShadow::m_pTexture = NULL;

//--------------------------------------------
//�V�[��3D�N���X �R���X�g���N�^
//--------------------------------------------
CShadow::CShadow() : CSceneX(7)
{
	m_pos = D3DXVECTOR3(0,0,0);			//�ʒu
	m_rot = D3DXVECTOR3(0, 0, 0);		//����
	D3DXMatrixIdentity(&m_mtxWorld);	//���[���h�}�g���b�N�X
}

//--------------------------------------------
//�V�[��3D�N���X �f�X�g���N�^
//--------------------------------------------
CShadow::~CShadow()
{
}

//=============================================================================
// ��������
//=============================================================================
CShadow * CShadow::Create(D3DXVECTOR3 pos)
{
	CShadow *pShadow = NULL;

	if (pShadow == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pShadow = new CShadow;

		if (pShadow != NULL)
		{
			pShadow->m_pos = pos;
			pShadow->BindModel(CLoad::GetBuffMat(CLoad::MODEL_SHADOW), CLoad::GetNumMat(CLoad::MODEL_SHADOW), CLoad::GetMesh(CLoad::MODEL_SHADOW));
			pShadow->Init();
		}
	}

	return pShadow;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CShadow::Init(void)
{
	// 2D�I�u�W�F�N�g����������
	CSceneX::Init(m_pos);

	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(CScene::OBJTYPE_SHADOW);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CShadow::Uninit(void)
{
	// 2D�I�u�W�F�N�g�I������
	CSceneX::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CShadow::Update(void)
{
	CSceneX::SetPosition(m_pos);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CShadow::Draw(void)
{
	// �����_���[���擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	pDevice->SetRenderState(D3DRS_STENCILENABLE, true);	// �X�e���V���e�X�g��L���ɂ���
	//pDevice->SetRenderState(D3DRS_ZENABLE, false);	// Z�e�X�g��L���ɂ���

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);	// �����Ă��Ȃ���Ԃɂ���

	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);			// �X�e���V���̑ΏۂƂȂ���̂�S���L���ɂ���
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);						// �X�e���V���e�X�g�����i / Z�e�X�g���i
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);						// �X�e���V���e�X�g�����i / Z�e�X�g�s���i
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);						// �X�e���V���e�X�g���s���i

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);				// �J�����O�\

	// �`�揈��
	CSceneX::Draw();

	pDevice->SetRenderState(D3DRS_STENCILREF, 1);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);			// �X�e���V���̑ΏۂƂȂ���̂�1�ɂȂ��Ă�����̂�L���ɂ���
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);						// �X�e���V���e�X�g�����i / Z�e�X�g���i
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_ZERO);						// �X�e���V���e�X�g�����i / Z�e�X�g�s���i
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);						// �X�e���V���e�X�g���s���i

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// �J�����O��

	// �`�揈��
	CSceneX::Draw();

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000f);	// �����Ă����Ԃɂ���

	pDevice->SetRenderState(D3DRS_STENCILREF, 2);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);			// �X�e���V���̑ΏۂƂȂ���̂�2�ɂȂ��Ă�����̂�L���ɂ���
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);						// �X�e���V���e�X�g�����i / Z�e�X�g���i
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);						// �X�e���V���e�X�g�����i / Z�e�X�g�s���i
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);						// �X�e���V���e�X�g���s���i

	// �}�X�N�̎擾
	CMask *pMask;
	pMask = CManager::GetMask();

	pMask->Draw();

	pDevice->SetRenderState(D3DRS_STENCILENABLE, false);	// �X�e���V���e�X�g�𖳌��ɂ���
	//pDevice->SetRenderState(D3DRS_ZENABLE, true);	// Z�e�X�g��L���ɂ���

}

//=============================================================================
// �ʒu�ݒ菈��
//=============================================================================
void CShadow::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}