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
#include "bullet.h"
#include "scene3D.h"
#include "meshField.h"
#include "shadow.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPD3DXMESH CDohyo::m_pMesh = NULL;			// ���b�V�����i���_���j�ւ̃|�C���^
LPD3DXBUFFER CDohyo::m_pBuffMat = NULL;	// �}�e���A�����ւ̃|�C���^
DWORD CDohyo::m_nNumMat = 0;				// �}�e���A�����̐�
LPDIRECT3DTEXTURE9 *CDohyo::m_pTexture = NULL;				// �e�N�X�`��

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
			pDohyo->BindModel(m_pBuffMat, m_nNumMat, m_pMesh);
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
// �v���C���[�̃��f���ǂݍ��ݏ���
//=============================================================================
HRESULT CDohyo::LoadModel(void)
{
	// �����_���[���擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(DOHYO_MODEL_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	return S_OK;
}

//=============================================================================
// �v���C���[�̃e�N�X�`���������
//=============================================================================
void CDohyo::UnloadModel(void)
{
	// ���b�V���̊J��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// �}�e���A���̊J��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
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
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �}�e���A���̐����e�N�X�`����������̂𓮓I�Ɋm��
	m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
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
		for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
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