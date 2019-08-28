//=============================================================================
//
// �v���C���[���� [dohyo.cpp]
// Author : ���R���
//
//=============================================================================
#include "dohyo.h"
#include "input.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "camera.h"
#include "scene3D.h"
#include "meshField.h"
#include "shadow.h"
#include "load.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 *CDohyo::m_pTexture = NULL;				// �e�N�X�`��

//=============================================================================
// �}�N��
//=============================================================================
#define DOHYOSIZE (20)

//=============================================================================
// �v���C���[�N���X�̃R���X�g���N�^
//=============================================================================
CDohyo::CDohyo() : CSceneX(DOHYO_PRIORITY)
{
	// �l���N���A
	m_pVtxBuff = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CDohyo::~CDohyo()
{
}

//=============================================================================
// �I�u�W�F�N�g�̐�������
//=============================================================================
CDohyo *CDohyo::Create(D3DXVECTOR3 pos)
{
	CDohyo *pDohyo = NULL;

	if (pDohyo == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pDohyo = new CDohyo;

		if (pDohyo != NULL)
		{
			pDohyo->BindModel(CLoad::GetBuffMat(CLoad::MODEL_DOHYO), CLoad::GetNumMat(CLoad::MODEL_DOHYO), CLoad::GetMesh(CLoad::MODEL_DOHYO));
			pDohyo->BindMat(m_pTexture);
			pDohyo->Init(pos);
		}
	}

	return pDohyo;
}

//=============================================================================
// �v���C���[����������
//=============================================================================
HRESULT CDohyo::Init(D3DXVECTOR3 pos)
{
	// 2D�I�u�W�F�N�g����������
	CSceneX::Init(pos);

	CSceneX::SetVtxMax(D3DXVECTOR3(CSceneX::GetVtxMax().x - DOHYOSIZE, CSceneX::GetVtxMax().y, CSceneX::GetVtxMax().z - DOHYOSIZE));
	CSceneX::SetVtxMin(D3DXVECTOR3(CSceneX::GetVtxMin().x + DOHYOSIZE, CSceneX::GetVtxMin().y, CSceneX::GetVtxMin().z + DOHYOSIZE));

	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(CScene::OBJTYPE_DOHYO);

	return S_OK;
}

//=============================================================================
// �v���C���[�I������
//=============================================================================
void CDohyo::Uninit(void)
{
	// 2D�I�u�W�F�N�g�I������
	CSceneX::Uninit();
}

//=============================================================================
// �v���C���[�X�V����
//=============================================================================
void CDohyo::Update(void)
{

}

//=============================================================================
// �v���C���[�`�揈��
//=============================================================================
void CDohyo::Draw(void)
{
	// 2D�I�u�W�F�N�g�`�揈��
	CSceneX::Draw();
}

//=============================================================================
// �u���b�N�̃��f���ǂݍ��ݏ���
//=============================================================================
HRESULT CDohyo::LoadMat(void)
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

	D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^

										// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)CLoad::GetBuffMat(CLoad::MODEL_DOHYO)->GetBufferPointer();

	// �}�e���A���̐����e�N�X�`����������̂𓮓I�Ɋm��
	m_pTexture = new LPDIRECT3DTEXTURE9[CLoad::GetNumMat(CLoad::MODEL_DOHYO)];

	for (int nCntMat = 0; nCntMat < (int)CLoad::GetNumMat(CLoad::MODEL_DOHYO); nCntMat++)
	{
		// �����O�ɋ�ɂ���
		m_pTexture[nCntMat] = NULL;

		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// �e�N�X�`���̐���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &m_pTexture[nCntMat]);
		}
	}

	return S_OK;
}

//=============================================================================
// �u���b�N�̃��f���������
//=============================================================================
void CDohyo::UnloadMat(void)
{
	if (m_pTexture != NULL)
	{// �e�N�X�`���̃|�C���^��NULL�`�F�b�N(��)
		for (int nCntMat = 0; nCntMat < (int)CLoad::GetNumMat(CLoad::MODEL_DOHYO); nCntMat++)
		{
			if (m_pTexture[nCntMat] != NULL)
			{// �|�C���^�̒���NULL�`�F�b�N(�Ƌ�)
				m_pTexture[nCntMat]->Release();
				m_pTexture[nCntMat] = NULL;
			}
		}

		// �������̊J��(���)
		delete[] m_pTexture;
		// NULL�ɂ���(�X�n)
		m_pTexture = NULL;
	}
}