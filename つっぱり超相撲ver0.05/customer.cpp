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

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPD3DXMESH CCustomer::m_pMesh[MAX_CUSTOMERTYPE] = {};			// ���b�V�����i���_���j�ւ̃|�C���^
LPD3DXBUFFER CCustomer::m_pBuffMat[MAX_CUSTOMERTYPE] = {};		// �}�e���A�����ւ̃|�C���^
DWORD CCustomer::m_nNumMat[MAX_CUSTOMERTYPE] = {};				// �}�e���A�����̐�
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
CCustomer *CCustomer::Create(D3DXVECTOR3 pos,int nType)
{
	CCustomer *pCustomer = NULL;
	
	if (pCustomer == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pCustomer = new CCustomer;

		if (pCustomer != NULL)
		{
			pCustomer->BindModel(m_pBuffMat[nType], m_nNumMat[nType], m_pMesh[nType]);
			pCustomer->BindMat(m_pTexture);
			pCustomer->Init(pos);
		}
	}

	return pCustomer;
}

//=============================================================================
// �v���C���[����������
//=============================================================================
HRESULT CCustomer::Init(D3DXVECTOR3 pos)
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

//=============================================================================
// �v���C���[�̃��f���ǂݍ��ݏ���
//=============================================================================
HRESULT CCustomer::LoadModel(void)
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
		&m_pBuffMat[0],
		NULL,
		&m_nNumMat[0],
		&m_pMesh[0]);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODEL_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[1],
		NULL,
		&m_nNumMat[1],
		&m_pMesh[1]);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(PLAYER_MODEL_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[2],
		NULL,
		&m_nNumMat[2],
		&m_pMesh[2]);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(ENEMY_MODEL_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[3],
		NULL,
		&m_nNumMat[3],
		&m_pMesh[3]);

	return S_OK;
}

//=============================================================================
// �v���C���[�̃e�N�X�`���������
//=============================================================================
void CCustomer::UnloadModel(void)
{
	// ���b�V���̊J��
	if (m_pMesh != NULL)
	{
		for (int nCntMesh = 0; nCntMesh < MAX_CUSTOMERTYPE; nCntMesh++)
		{
			m_pMesh[nCntMesh]->Release();
			m_pMesh[nCntMesh] = NULL;
		}
	}

	// �}�e���A���̊J��
	if (m_pBuffMat != NULL)
	{
		for (int nCntMat = 0; nCntMat < MAX_CUSTOMERTYPE; nCntMat++)
		{
			m_pBuffMat[nCntMat]->Release();
			m_pBuffMat[nCntMat] = NULL;
		}
	}
}

//=============================================================================
// �u���b�N�̃��f���ǂݍ��ݏ���
//=============================================================================
HRESULT CCustomer::LoadMat(void)
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

	for (int nCntNum = 0; nCntNum < MAX_CUSTOMERTYPE; nCntNum++)
	{

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)m_pBuffMat[nCntNum]->GetBufferPointer();
	}

	return S_OK;
}

//=============================================================================
// �u���b�N�̃��f���������
//=============================================================================
void CCustomer::UnloadMat(void)
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