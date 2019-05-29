//=============================================================================
//
// �j���[�X�L���X�^�[���� [newsCaster.cpp]
// Author : ���R���
//
//=============================================================================
#include "newsCaster.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "camera.h"
#include "bullet.h"
#include "scene3D.h"
#include "meshField.h"
#include "shadow.h"
#include "title.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPD3DXMESH CNewsCaster::m_pMesh = NULL;			// ���b�V�����i���_���j�ւ̃|�C���^
LPD3DXBUFFER CNewsCaster::m_pBuffMat = NULL;	// �}�e���A�����ւ̃|�C���^
DWORD CNewsCaster::m_nNumMat = 0;				// �}�e���A�����̐�
LPDIRECT3DTEXTURE9 *CNewsCaster::m_pTexture = NULL;				// �e�N�X�`��

//=============================================================================
// �}�N��
//=============================================================================
#define DOHYOSIZE (20)

//=============================================================================
// �j���[�X�L���X�^�[�N���X�̃R���X�g���N�^
//=============================================================================
CNewsCaster::CNewsCaster() : CSceneX(DOHYO_PRIORITY)
{
	// �l���N���A
	m_pVtxBuff = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
	m_nCntTimer = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CNewsCaster::~CNewsCaster()
{
}

//=============================================================================
// �I�u�W�F�N�g�̐�������
//=============================================================================
CNewsCaster *CNewsCaster::Create(D3DXVECTOR3 pos)
{
	CNewsCaster *pDohyo = NULL;

	if (pDohyo == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pDohyo = new CNewsCaster;

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
// �j���[�X�L���X�^�[����������
//=============================================================================
HRESULT CNewsCaster::Init(D3DXVECTOR3 pos)
{
	// 2D�I�u�W�F�N�g����������
	CSceneX::Init(pos);

	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(CScene::OBJTYPE_SCENEX);

	m_nCntTimer = 0;

	return S_OK;
}

//=============================================================================
// �j���[�X�L���X�^�[�I������
//=============================================================================
void CNewsCaster::Uninit(void)
{
	// 2D�I�u�W�F�N�g�I������
	CSceneX::Uninit();
}

//=============================================================================
// �j���[�X�L���X�^�[�X�V����
//=============================================================================
void CNewsCaster::Update(void)
{
	// �ʒu�擾
	D3DXVECTOR3 pos;
	pos = CSceneX::GetPosition();

	// �ʒu�擾
	D3DXVECTOR3 rot;
	rot = CSceneX::GetRot();

	// �^�C�g���擾
	CTitle *pTitle;
	pTitle = CManager::GetTitle();

	if (rot.x >= -0.7f)
	{
		rot.x -= 0.01f;
	}
	else if (rot.x < -0.7f)
	{
		m_nCntTimer++;
		int nTime = m_nCntTimer / 60;

		if (nTime > 2)
		{
			pTitle->SetState(CTitle::STATE_TITLE);
		}
	}

	CSceneX::SetPosition(pos);
	CSceneX::SetRot(rot);

	if (pTitle->GetState() == CTitle::STATE_TITLE)
	{
		Uninit();
	}

#ifdef _DEBUG
	CDebugProc::Print("cf", "���� : ", rot.x);
#endif
}

//=============================================================================
// �j���[�X�L���X�^�[�`�揈��
//=============================================================================
void CNewsCaster::Draw(void)
{
	// 2D�I�u�W�F�N�g�`�揈��
	CSceneX::Draw();
}

//=============================================================================
// �j���[�X�L���X�^�[�̃��f���ǂݍ��ݏ���
//=============================================================================
HRESULT CNewsCaster::LoadModel(void)
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
	D3DXLoadMeshFromX(PLAYER_MODEL_NAME,
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
// �j���[�X�L���X�^�[�̃e�N�X�`���������
//=============================================================================
void CNewsCaster::UnloadModel(void)
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
HRESULT CNewsCaster::LoadMat(void)
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
void CNewsCaster::UnloadMat(void)
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