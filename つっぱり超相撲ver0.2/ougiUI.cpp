//=============================================================================
//
// ���`UI�̏��� [ougiUI.h]
// Author : �ڍ�������
//
//=============================================================================
#include "scene.h"
#include "ougiUI.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "input.h"
#include "gauge.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "tutorial.h"
#include "load.h"
//--------------------------------------------
//�ÓI�����o�ϐ��錾
//--------------------------------------------


//=============================================================================
// �V�[���N���X�̃R���X�g���N�^
//=============================================================================
COugiUI::COugiUI(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	// �l���N���A
	//m_pTexture = NULL;

	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRight = 0.0f;
	m_fLeft = 0.0f;			// ���E�̒l

}

//=============================================================================
// �I�u�W�F�N�g�̐�������
//=============================================================================
COugiUI *COugiUI::Create(D3DXVECTOR3 pos)
{
	COugiUI *pOugiUI = NULL;

	if (pOugiUI == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pOugiUI = new COugiUI;

		if (pOugiUI != NULL)
		{
			pOugiUI->Init(pos);
		}
	}

	return pOugiUI;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
COugiUI::~COugiUI()
{
}

//=============================================================================
// �Q�[�W�̏���������
//=============================================================================
HRESULT COugiUI::Init(D3DXVECTOR3 pos)
{
	// �Q�[�W�̈ʒu��ݒ�
	m_Pos = pos;

	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	//SetObjType(CScene::OBJTYPE_SCENE2D);

	// �����_���[���擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	//1p��
	m_pScene2D[0] = CScene2D::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	m_pScene2D[0]->SetWidthHeight(300.0f, 30.0f);
	m_pScene2D[0]->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	m_pScene2D[0]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_OUGIUI00));


	////2p��
	//m_pScene2D[1] = CScene2D::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	//m_pScene2D[1]->SetWidthHeight(300.0f, 30.0f);
	//m_pScene2D[1]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//m_pScene2D[1]->BindTexture(NULL);

	return S_OK;
}

//=============================================================================
// �Q�[�W�̏I������
//=============================================================================
void COugiUI::Uninit(void)
{

	// �I�u�W�F�N�g�̉��
	Release();
}

//=============================================================================
// �Q�[�W�̍X�V����
//=============================================================================
void COugiUI::Update(void)
{
	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

#ifdef _DEBUG

#if 1


	//�Q�[�W�̍��E�𒲐�
	m_pScene2D[0]->SetRIghtLeft(0.0f, m_fLeft);
	//m_pScene2D[1]->SetRIghtLeft(m_fRight, 0.0f);

#endif
	//CDebugProc::Print("cf", "m_fRight    : ", m_fRight);
	//CDebugProc::Print("cf", "m_fLeft    : ", m_fLeft);
#endif
}

//=============================================================================
// �Q�[�W�̕`�揈��
//=============================================================================
void COugiUI::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
}

//=============================================================================
// �I�u�W�F�N�g�̎擾
//=============================================================================
D3DXVECTOR3 COugiUI::GetPosition(void)
{
	return m_Pos;
}