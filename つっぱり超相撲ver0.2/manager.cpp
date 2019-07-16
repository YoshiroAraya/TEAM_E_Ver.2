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
#include "meshField.h"
#include "shadow.h"
#include "mask.h"
#include "dohyo.h"
#include "BattleSystem.h"
#include "fade.h"
#include "game.h"
#include "characterMove.h"
#include "sound.h"
#include "title.h"
#include "result.h"
#include "tutorial.h"
#include "ultimate.h"
#include "load.h"
#include "pause.h"
//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CCamera *CManager::m_pCamera = NULL;
CLight *CManager::m_pLight = NULL;
CDebugProc *CManager::m_pDebugProc = NULL;
CMask *CManager::m_pMask = NULL;
CGame *CManager::m_pGame = NULL;
CTitle *CManager::m_pTitle = NULL;
CResult *CManager::m_pResult = NULL;
CTutorial *CManager::m_pTutorial = NULL;
CUltimate *CManager::m_pUltimate = NULL;
CFade *CManager::m_pFade = NULL;
CCharacterMove *CManager::m_pCharacterMove = NULL;
CXInputJoyPad *CManager::m_pXInput = NULL;
CManager::MODE CManager::m_mode = CManager::MODE_TITLE;	//�Q�[���N�����̃��[�h
CSound	*CManager::m_pSound[MAX_SOUND] = {};
bool CManager::m_bCharaSelectMode = false;

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
	m_bCharaSelectMode = false;

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

	if (m_pXInput == NULL)
	{//�G�b�N�X�C���v�b�g�R���g���[���[�̐���

		m_pXInput = new CXInputJoyPad;

		if (m_pXInput != NULL)
		{
			m_pXInput->Init();
		}
	}

	if (m_pMask == NULL)
	{
		m_pMask = CMask::Create();
	}

	if (m_pCharacterMove == NULL)
	{
		m_pCharacterMove = CCharacterMove::Create();
	}

	//�T�E���h
	for (int nCnt = 0; nCnt < MAX_SOUND; nCnt++)
	{
		m_pSound[nCnt] = new CSound;
		if (m_pSound[nCnt] != NULL)
		{
			m_pSound[nCnt]->InitSound(hWnd);
		}
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

	if (m_pFade == NULL)
	{
		//�t�F�[�h�̐���
		m_pFade = CFade::Create();

		if (m_pFade != NULL)
		{
			m_pFade->SetFade(m_mode, m_pFade->FADE_IN);
		}
	}

	CLoad::LoadModel();
	CLoad::LoadTex();

	// ���[�h�ؑ�
	//SetMode(m_mode);

	return S_OK;
}

//=============================================================================
// �}�l�[�W���I������
//=============================================================================
void CManager::Uninit(void)
{
	if (m_pMask != NULL)
	{// �t�F�[�h�̏I��
		m_pMask->Uninit();
		delete m_pMask;
		m_pMask = NULL;
	}

	if (m_pCharacterMove != NULL)
	{// �ړ������̏I��
		m_pCharacterMove->Uninit();
		delete m_pCharacterMove;
		m_pCharacterMove = NULL;
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

	if (m_pXInput != NULL)
	{// �����_�����O�N���X�̔j��
	 // �I������

	 // ���������J��
		delete m_pXInput;
		m_pXInput = NULL;
	}

	//�t�F�[�h�N���X�̔j��
	if (m_pFade != NULL)
	{
		// �I������
		m_pFade->Uninit();

		//�������̊J��
		delete m_pFade;

		//NULL�ɂ���
		m_pFade = NULL;
	}

	//�T�E���h
	for (int nCnt = 0; nCnt < MAX_SOUND; nCnt++)
	{
		if (m_pSound[nCnt] != NULL)
		{
			m_pSound[nCnt]->UninitSound();
			delete m_pSound[nCnt];
			m_pSound[nCnt] = NULL;
		}
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

	switch (m_mode)
	{
		//�^�C�g�����[�h�̏I������
	case CManager::MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		break;

	case CManager::MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Uninit();
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		break;

		//�Q�[�����[�h�̏I������
	case CManager::MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		break;

		//���U���g���[�h�̏I������
	case CManager::MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}
		break;

		//�K�E�Z���[�h�̏I������
	case CManager::MODE_ULTIMATE:
		if (m_pUltimate != NULL)
		{
			m_pUltimate->Uninit();
			delete m_pUltimate;
			m_pUltimate = NULL;
		}
		break;
	}

	CLoad::UnloadModel();
	CLoad::UnloadTex();

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

	bool bPause = CPause::GetPauseBool();


	if (m_pRenderer != NULL)
	{// �����_���[�X�V����
		m_pRenderer->Update();
	}

	if (bPause == false)
	{
		if (m_pCamera != NULL)
		{// �J�����X�V����
			m_pCamera->Update();
		}

		if (m_pLight != NULL)
		{// ���C�g�X�V����
			m_pLight->Update();
		}
	}


	if (m_pInputKeyboard != NULL)
	{// �L�[�{�[�h���͍X�V����
		m_pInputKeyboard->Update();
	}

	if (m_pXInput != NULL)
	{
		m_pXInput->Update();
	}

	if (m_pMask != NULL)
	{// �t�F�[�h�X�V����
		m_pMask->Update();
	}

	if (m_pFade != NULL)
	{//�t�F�[�h�̍X�V����
		m_pFade->Update();
	}

	switch (m_mode)
	{
		//�^�C�g�����[�h�̍X�V����
	case CManager::MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Update();
		}
		break;

	case CManager::MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Update();
		}
		break;

		//�Q�[�����[�h�̍X�V����
	case CManager::MODE_GAME:
		if (m_pGame != NULL)
		{
			if (bPause == false)
			{
				m_pGame->Update();
			}
		}
		break;

		//���U���g���[�h�̍X�V����
	case CManager::MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Update();
		}
		break;

		//�K�E�Z���[�h�̍X�V����
	case CManager::MODE_ULTIMATE:
		if (m_pUltimate != NULL)
		{
			m_pUltimate->Update();
		}
		break;
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
// �}�X�N�̎擾
//=============================================================================
CCharacterMove *CManager::GetCharacterMove(void)
{
	return m_pCharacterMove;
}

//=============================================================================
// �}�X�N�̎擾
//=============================================================================
CMask *CManager::GetMask(void)
{
	return m_pMask;
}

//=============================================================================
// �}�l�[�W���[�N���X �T�E���h���擾
//=============================================================================
CSound *CManager::GetSound(int nNum)
{
	return m_pSound[nNum];
}

//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void CManager::SetMode(MODE mode)
{
	switch (m_mode)
	{
	case CManager::MODE_TITLE:
		//�^�C�g���N���X�̔j��
		if (m_pTitle != NULL)
		{
			// �I������
			m_pTitle->Uninit();

			//�������̊J��
			delete m_pTitle;

			//NULL�ɂ���
			m_pTitle = NULL;
		}
		break;

	case CManager::MODE_TUTORIAL:
		//�`���[�g���A���N���X�̔j��
		if (m_pTutorial != NULL)
		{
			// �I������
			m_pTutorial->Uninit();
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		break;

	case CManager::MODE_GAME:
		//�Q�[���N���X�̔j��
		if (m_pGame != NULL)
		{
			// �I������
			m_pGame->Uninit();

			delete m_pGame;

			m_pGame = NULL;
		}
		break;

	case CManager::MODE_RESULT:
		//���U���g�N���X�̔j��
		if (m_pResult != NULL)
		{
			// �I������
			m_pResult->Uninit();

			//�������̊J��
			delete m_pResult;

			//NULL�ɂ���
			m_pResult = NULL;
		}
		break;

	case CManager::MODE_ULTIMATE:
		//���U���g�N���X�̔j��
		if (m_pUltimate != NULL)
		{
			// �I������
			m_pUltimate->Uninit();

			//�������̊J��
			delete m_pUltimate;

			//NULL�ɂ���
			m_pUltimate = NULL;
		}
		break;
	}
	m_mode = mode;

	switch (mode)
	{
	case CManager::MODE_TITLE:
		//�^�C�g���̏�����
		if (m_pTitle == NULL)
		{
			//�L�[�{�[�h�̃������𓮓I�m��
			m_pTitle = new CTitle;

			if (m_pTitle != NULL)
			{
				// ����������
				m_pTitle->Init();

				if (m_bCharaSelectMode == true)
				{
					m_pTitle->SetState(CTitle::STATE_CHARASELECT);
				}
			}
			else
			{
				MessageBox(0, "NULL����Ȃ��ł�", "�x��", MB_OK);
			}
		}
		else
		{
			MessageBox(0, "aaaNULL�ł���", "�x��", MB_OK);
		}

		break;
	case CManager::MODE_TUTORIAL:
		//�Q�[���̏�����
		if (m_pTutorial == NULL)
		{
			//�Q�[���̃������𓮓I�m��
			m_pTutorial = new CTutorial;

			if (m_pTutorial != NULL)
			{	// ����������
				m_pTutorial->Init();
			}
			else
			{
				MessageBox(0, "NULL����Ȃ��ł�", "�x��", MB_OK);
			}
		}
		else
		{
			MessageBox(0, "NULL�ł���", "�x��", MB_OK);
		}
		break;
	case CManager::MODE_GAME:
		//�Q�[���̏�����
		if (m_pGame == NULL)
		{
			//�Q�[���̃������𓮓I�m��
			m_pGame = new CGame;

			if (m_pGame != NULL)
			{
				//m_pSound[0]->PlaySound(m_pSound[0]->SOUND_LABEL_BGM_GAME);
				// ����������
				m_pGame->Init();
			}
			else
			{
				MessageBox(0, "NULL����Ȃ��ł�", "�x��", MB_OK);
			}
		}
		else
		{
			MessageBox(0, "NULL�ł���", "�x��", MB_OK);
		}
		break;

	case CManager::MODE_RESULT:
		//���U���g�̏�����
		if (m_pResult == NULL)
		{
			//���U���g�̃������𓮓I�m��
			m_pResult = new CResult;

			if (m_pResult != NULL)
			{
				// ����������
				m_pResult->Init();
			}
			else
			{
				MessageBox(0, "NULL����Ȃ��ł�", "�x��", MB_OK);
			}
		}
		else
		{
			MessageBox(0, "NULL�ł���", "�x��", MB_OK);
		}
		break;

	case CManager::MODE_ULTIMATE:
		//���U���g�̏�����
		if (m_pUltimate == NULL)
		{
			//���U���g�̃������𓮓I�m��
			m_pUltimate = new CUltimate;

			if (m_pUltimate != NULL)
			{
				// ����������
				m_pUltimate->Init();
			}
			else
			{
				MessageBox(0, "NULL����Ȃ��ł�", "�x��", MB_OK);
			}
		}
		else
		{
			MessageBox(0, "NULL�ł���", "�x��", MB_OK);
		}
		break;

		//case CManager::MODE_TUTORIAL:
		//	//�`���[�g�[���A���̏�����
		//	if (m_pTutorial == NULL)
		//	{
		//		//�L�[�{�[�h�̃������𓮓I�m��
		//		m_pTutorial = new CTutorial;

		//		if (m_pTutorial != NULL)
		//		{
		//			// ����������
		//			m_pTutorial->Init();
		//		}
		//		else
		//		{
		//			MessageBox(0, "NULL����Ȃ��ł�", "�x��", MB_OK);
		//		}
		//	}
		//	else
		//	{
		//		MessageBox(0, "NULL�ł���", "�x��", MB_OK);
		//	}
		//	break;

	}
}
