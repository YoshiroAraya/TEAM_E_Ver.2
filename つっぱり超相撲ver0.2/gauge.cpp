//=============================================================================
//
// �Q�[�W���� [gauge.h]
// Author : �ڍ�������
//
//=============================================================================
#include "scene.h"
#include "gauge.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "tutorial.h"
//--------------------------------------------
//�ÓI�����o�ϐ��錾
//--------------------------------------------


//=============================================================================
// �V�[���N���X�̃R���X�g���N�^
//=============================================================================
CGauge::CGauge(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
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
CGauge *CGauge::Create(D3DXVECTOR3 pos)
{
	CGauge *pGauge = NULL;

	if (pGauge == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pGauge = new CGauge;

		if (pGauge != NULL)
		{
			pGauge->Init(pos);
		}
	}

	return pGauge;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGauge::~CGauge()
{
}

//=============================================================================
// �Q�[�W�̏���������
//=============================================================================
HRESULT CGauge::Init(D3DXVECTOR3 pos)
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
	m_pScene2D[0] = CScene2D::Create(D3DXVECTOR3(300.0f, pos.y, 0.0f));
	m_pScene2D[0]->SetWidthHeight(300.0f, 30.0f);
	m_pScene2D[0]->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	m_pScene2D[0]->BindTexture(NULL);
	m_bUlt[0] = false;

	//2p��
	m_pScene2D[1] = CScene2D::Create(D3DXVECTOR3(980.0f, pos.y, 0.0f));
	m_pScene2D[1]->SetWidthHeight(300.0f, 30.0f);
	m_pScene2D[1]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pScene2D[1]->BindTexture(NULL);
	m_bUlt[1] = false;

	return S_OK;
}

//=============================================================================
// �Q�[�W�̏I������
//=============================================================================
void CGauge::Uninit(void)
{

	// �I�u�W�F�N�g�̉��
	Release();
}

//=============================================================================
// �Q�[�W�̍X�V����
//=============================================================================
void CGauge::Update(void)
{
	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

#ifdef _DEBUG

#if 1
	if (pInputKeyboard->GetPress(DIK_1) == true)
	{
		m_fLeft += 10.0f;
	}
	if (pInputKeyboard->GetPress(DIK_2) == true)
	{
		m_fLeft -= 10.0f;
	}

	if (pInputKeyboard->GetPress(DIK_3) == true)
	{
		m_fRight -= 10.0f;
	}
	if (pInputKeyboard->GetPress(DIK_4) == true)
	{
		m_fRight += 10.0f;
	}

	//�Q�[�W�̍ő�ƍŏ�
	if (m_fLeft > 0)
	{
		m_fLeft = 0;
	}
	else if (m_fLeft < -600)
	{
		m_fLeft = -600;
	}

	if (m_fRight > 0)
	{
		m_fRight = 0;
	}
	else if (m_fRight < -600)
	{
		m_fRight = -600;
	}

	//�Q�[�W�̍��E�𒲐�
	m_pScene2D[0]->SetRIghtLeft(0.0f, m_fLeft);
	m_pScene2D[1]->SetRIghtLeft(m_fRight, 0.0f);

#endif
	//CDebugProc::Print("cf", "m_fRight    : ", m_fRight);
	//CDebugProc::Print("cf", "m_fLeft    : ", m_fLeft);
#endif
}

//=============================================================================
// �Q�[�W�̕`�揈��
//=============================================================================
void CGauge::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();


}

//=============================================================================
// �I�u�W�F�N�g�̎擾
//=============================================================================
D3DXVECTOR3 CGauge::GetPosition(void)
{
	return m_Pos;
}

//=============================================================================
// �I�u�W�F�N�g�̐ݒ�
//=============================================================================
void CGauge::SetPosition(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
// ���E
//=============================================================================
void CGauge::SetGaugeRightLeft(float fRight, float fLeft)
{
	// �v���C���[�̎擾
	CPlayer *pPlayer = NULL;
	// �G�l�~�[�̎擾
	CEnemy *pEnemy = NULL;

	CManager::MODE mode;
	mode = CManager::GetMode();
	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();
	}

	m_fRight += fRight;
	m_fLeft += fLeft;
	//����𒴂��Ȃ��悤��
	if (m_fLeft > 0)
	{
		m_fLeft = 0;
	}
	else if (m_fLeft > -590)
	{
		pPlayer->SetDying(false);
	}
	else if (m_fLeft <= -590)
	{
		pPlayer->SetDying(true);
	}
	else if (m_fLeft < -600)
	{
		m_fLeft = -600;
	}

	if (m_fRight > 0)
	{
		m_fRight = 0;
	}
	else if (m_fRight > -590)
	{
		pEnemy->SetDying(false);
	}
	else if (m_fRight <= -590)
	{
		pEnemy->SetDying(true);
	}
	else if (m_fRight < -600)
	{
		m_fRight = -600;
	}

	//�Q�[�W�̍��E�𒲐�
	m_pScene2D[0]->SetRIghtLeft(0.0f, m_fLeft);
	m_pScene2D[1]->SetRIghtLeft(m_fRight, 0.0f);
}
