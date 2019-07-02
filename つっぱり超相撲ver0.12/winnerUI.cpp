//=============================================================================
//
// ����UI�̏��� [winnerUI.h]
// Author :		�R���֎j
//
//=============================================================================
#include "scene.h"
#include "winnerUI.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "input.h"
#include "gauge.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "load.h"
//--------------------------------------------
//�ÓI�����o�ϐ��錾
//--------------------------------------------


//=============================================================================
// �V�[���N���X�̃R���X�g���N�^
//=============================================================================
CWinnerUI::CWinnerUI() : CScene2D(7, CScene::OBJTYPE_SCENE2D)
{
	// �l���N���A
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_n1player = 0;
	m_n2player = 0;
	m_bWinner = false;

}

//=============================================================================
// �I�u�W�F�N�g�̐�������
//=============================================================================
CWinnerUI *CWinnerUI::Create(D3DXVECTOR3 pos)
{
	CWinnerUI *pWinnerUI = NULL;

	if (pWinnerUI == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pWinnerUI = new CWinnerUI;

		if (pWinnerUI != NULL)
		{
			pWinnerUI->Init(pos);
		}
	}

	return pWinnerUI;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CWinnerUI::~CWinnerUI()
{
}

//=============================================================================
// �Q�[�W�̏���������
//=============================================================================
HRESULT CWinnerUI::Init(D3DXVECTOR3 pos)
{
	// �Q�[�W�̈ʒu��ݒ�
	m_Pos = pos;

	m_n1player = 0;
	m_n2player = 0;
	m_bWinner = false;
	// �����_���[���擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	//1p��
	for (int nCnt1 = 0; nCnt1 < 3; nCnt1++)
	{
		m_pScene2D[nCnt1] = CScene2D::Create(D3DXVECTOR3(400.0f - (nCnt1 * 100.0f), pos.y, 0.0f));
		m_pScene2D[nCnt1]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_WINNER_UI));
		m_pScene2D[nCnt1]->SetWidthHeight(50.0f, 50.0f);
		m_pScene2D[nCnt1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//2p��
	for (int nCnt2 = 0; nCnt2 < 3; nCnt2++)
	{
		m_pScene2D[nCnt2 + 3] = CScene2D::Create(D3DXVECTOR3(850.0f + (nCnt2 * 100.0f), pos.y, 0.0f));
		m_pScene2D[nCnt2 + 3]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_WINNER_UI));
		m_pScene2D[nCnt2 + 3]->SetWidthHeight(50.0f, 50.0f);
		m_pScene2D[nCnt2 + 3]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	return S_OK;
}

//=============================================================================
// �Q�[�W�̏I������
//=============================================================================
void CWinnerUI::Uninit(void)
{
	//�I������
	Release();

}

//=============================================================================
// �Q�[�W�̍X�V����
//=============================================================================
void CWinnerUI::Update(void)
{
	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//��������
	int nWinner = CGame::GetWinner();


	if (nWinner == 1)
	{//1p��
		m_bWinner = true;
		if (m_bWinner == true)
		{
			m_n1player += 1;
			m_bWinner = false;
		}

		if (m_n1player == 1)
		{
			m_pScene2D[0]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else if (m_n1player == 2)
		{
			m_pScene2D[1]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else if (m_n1player == 3)
		{
			m_pScene2D[2]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}


	if (nWinner == 2)
	{//2p��

		m_bWinner = true;
		if (m_bWinner == true)
		{
			m_n2player += 1;
			m_bWinner = false;

		}
		if (m_n2player == 1)
		{
			m_pScene2D[3]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else if (m_n2player == 2)
		{
			m_pScene2D[4]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		}
		else if (m_n2player == 3)
		{
			m_pScene2D[5]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		}
	}
}

//=============================================================================
// �Q�[�W�̕`�揈��
//=============================================================================
void CWinnerUI::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//CScene2D::Draw();
}

//=============================================================================
// �I�u�W�F�N�g�̎擾
//=============================================================================
D3DXVECTOR3 CWinnerUI::GetPosition(void)
{
	return m_Pos;
}

//=============================================================================
// �I�u�W�F�N�g�̐ݒ�
//=============================================================================
void CWinnerUI::SetPosition(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}