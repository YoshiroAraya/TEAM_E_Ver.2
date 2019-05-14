//=============================================================================
//
// �}�l�[�W������ [manager.cpp]
// Author : ���R���
//
//=============================================================================
#include "manager.h"
#include "main.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneX.h"
#include "billboard.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "debugProc.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "meshField.h"
#include "shadow.h"
#include "mask.h"
#include "dohyo.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CCamera *CManager::m_pCamera = NULL;
CLight *CManager::m_pLight = NULL;
CDebugProc *CManager::m_pDebugProc = NULL;
CScene3D *CManager::m_pScene3D = NULL;
CPlayer *CManager::m_pPlayer = NULL;
CEnemy *CManager::m_pEnemy = NULL;
CShadow *CManager::m_pShadow = NULL;
CMeshField *CManager::m_pMeshField = NULL;
CMask *CManager::m_pMask = NULL;

//=============================================================================
// �}�l�[�W���N���X�̃R���X�g���N�^
//=============================================================================
CManager::CManager()
{
	m_fData = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CManager::~CManager()
{
}

//=============================================================================
// �}�l�[�W������������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	m_fData = 0.0f;

	if (m_pRenderer == NULL)
	{
		// �����_�����O�N���X�̐���
		m_pRenderer = new CRenderer;

		if (m_pRenderer != NULL)
		{
			// ����������
			if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
			{
				return -1;
			}
		}
	}

	if (m_pCamera == NULL)
	{
		// �J�����N���X�̐���
		m_pCamera = new CCamera;

		if (m_pCamera != NULL)
		{
			m_pCamera->Init();
		}
	}

	if (m_pLight == NULL)
	{
		// ���C�g�N���X�̐���
		m_pLight = new CLight;

		if (m_pLight != NULL)
		{
			m_pLight->Init();
		}
	}

	if (m_pInputKeyboard == NULL)
	{
		// ���̓N���X�̐���
		m_pInputKeyboard = new CInputKeyboard;

		if (m_pInputKeyboard != NULL)
		{
			m_pInputKeyboard->Init(hInstance, hWnd);
		}
	}

	if (m_pMask == NULL)
	{
		m_pMask = CMask::Create();
	}

	if (m_pScene3D == NULL)
	{
		//m_pScene3D = CScene3D::Create(D3DXVECTOR3(200.0f, 0.0f, 0.0f));
	}

	CDohyo::LoadModel();
	CDohyo::LoadMat();
	CDohyo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (m_pPlayer == NULL)
	{// �v���C���[
		CPlayer::LoadModel();

		m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 50.0f, 0.0f));
	}

	if (m_pEnemy == NULL)
	{// �G�l�~�[
		CEnemy::LoadModel();

		m_pEnemy = CEnemy::Create(D3DXVECTOR3(0.0f, 50.0f, 0.0f));
	}

#ifdef _DEBUG
	if (m_pDebugProc == NULL)
	{
		// �f�o�b�N�\���N���X�̐���
		m_pDebugProc = new CDebugProc;

		if (m_pDebugProc != NULL)
		{
			m_pDebugProc->Init();
		}
	}
#endif

	if (m_pMeshField == NULL)
	{
		// ���b�V���t�B�[���h�̐���
	//	m_pMeshField = CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	// 2D�|���S���̐���
	//CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH -50.0f, 50.0f, 0.0f));

	// 3D�|���S���̐���
	//CScene3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 3D���f��
	//CSceneX::Create(D3DXVECTOR3(0.0f, 25.0f, 0.0f));

	// �e�̃e�N�X�`����ǂݍ���
	CBullet::Load();

	CShadow::Load();

	if (m_pShadow == NULL)
	{
		m_pShadow = CShadow::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	return S_OK;
}

//=============================================================================
// �}�l�[�W���I������
//=============================================================================
void CManager::Uninit(void)
{
	CBullet::Unload();

	CPlayer::UnloadModel();
	CDohyo::UnloadModel();
	CDohyo::UnloadMat();
	CShadow::UnLoad();

	//m_pScene3D = NULL;
	m_pPlayer = NULL;
	m_pEnemy = NULL;
	m_pMeshField = NULL;
	m_pShadow = NULL;

	if (m_pMask != NULL)
	{// �t�F�[�h�̏I��
		m_pMask->Uninit();
		delete m_pMask;
		m_pMask = NULL;
	}

	if (m_pRenderer != NULL)
	{// �����_�����O�N���X�̔j��
		// �I������
		m_pRenderer->Uninit();

		// ���������J��
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	if (m_pCamera != NULL)
	{// �J�����N���X�̔j��
	 // �I������
		m_pCamera->Uninit();

		// ���������J��
		delete m_pCamera;
		m_pCamera = NULL;
	}

	if (m_pLight != NULL)
	{// ���C�g�N���X�̔j��
	 // �I������
		m_pLight->Uninit();

		// ���������J��
		delete m_pLight;
		m_pLight = NULL;
	}

	if (m_pInputKeyboard != NULL)
	{// �����_�����O�N���X�̔j��
		 // �I������
		m_pInputKeyboard->Uninit();

		// ���������J��
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

#ifdef _DEBUG
	if (m_pDebugProc != NULL)
	{// �f�o�b�N�\���N���X�̔j��
		// �I������
		m_pDebugProc->Uninit();

		// ���������J��
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}
#endif

	// �S�ẴI�u�W�F�N�g�����
	CScene::ReleseAll();
}

//=============================================================================
// �}�l�[�W���X�V����
//=============================================================================
void CManager::Update(void)
{
#ifdef _DEBUG
	// �f�o�b�N�\��������
	CDebugProc::ReleseStr();
#endif

	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//if (pInputKeyboard->GetTrigger(DIK_G) == true)
	//{
	//	// 2D�|���S���̐���
	//	CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + m_fData, SCREEN_HEIGHT / 2, 0.0f));

	//	m_fData += 80.0f;
	//}

	if (m_pRenderer != NULL)
	{// �����_���[�X�V����
		m_pRenderer->Update();
	}

	if (m_pCamera != NULL)
	{// �J�����X�V����
		m_pCamera->Update();
	}

	if (m_pLight != NULL)
	{// �J�����X�V����
		m_pLight->Update();
	}

	if (m_pInputKeyboard != NULL)
	{// �L�[�{�[�h���͍X�V����
		m_pInputKeyboard->Update();
	}

	if (m_pMask != NULL)
	{// �L�[�{�[�h���͍X�V����
		m_pMask->Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	if (m_pCamera != NULL)
	{// �J������ݒ�
		m_pCamera->SetCamera();
	}

	if (m_pRenderer != NULL)
	{// �����_���[�`�揈��
		m_pRenderer->Draw();
	}
}

//=============================================================================
// �����_���[�̎擾
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
// ���͏��̎擾
//=============================================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

//=============================================================================
// �J�����̎擾
//=============================================================================
CCamera *CManager::GetCamera(void)
{
	return m_pCamera;
}

//=============================================================================
// 3D�|���S���̎擾
//=============================================================================
CScene3D *CManager::GetScene3D(void)
{
	return m_pScene3D;
}

//=============================================================================
// �v���C���[�̎擾
//=============================================================================
CPlayer *CManager::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// �v���C���[�̎擾
//=============================================================================
CEnemy * CManager::GetEnemy(void)
{
	return m_pEnemy;
}

//=============================================================================
// �e�̎擾
//=============================================================================
CShadow *CManager::GetShadow(void)
{
	return m_pShadow;
}


//=============================================================================
// ���b�V���t�B�[���h�̎擾
//=============================================================================
CMeshField *CManager::GetMeshField(void)
{
	return m_pMeshField;
}

//=============================================================================
// �}�X�N�̎擾
//=============================================================================
CMask *CManager::GetMask(void)
{
	return m_pMask;
}