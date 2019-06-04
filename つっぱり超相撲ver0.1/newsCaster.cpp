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
#include "loadModel.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
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
			pDohyo->BindModel(CLoadModel::GetBuffMat(CLoadModel::MODEL_NEWSCASTER), CLoadModel::GetNumMat(CLoadModel::MODEL_NEWSCASTER), CLoadModel::GetMesh(CLoadModel::MODEL_NEWSCASTER));
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

		if (nTime >= 1)
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
	pMat = (D3DXMATERIAL*)CLoadModel::GetBuffMat(CLoadModel::MODEL_NEWSCASTER)->GetBufferPointer();

	// �}�e���A���̐����e�N�X�`����������̂𓮓I�Ɋm��
	m_pTexture = new LPDIRECT3DTEXTURE9[CLoadModel::GetNumMat(CLoadModel::MODEL_NEWSCASTER)];

	for (int nCntMat = 0; nCntMat < (int)CLoadModel::GetNumMat(CLoadModel::MODEL_NEWSCASTER); nCntMat++)
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
		for (int nCntMat = 0; nCntMat < (int)CLoadModel::GetNumMat(CLoadModel::MODEL_NEWSCASTER); nCntMat++)
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