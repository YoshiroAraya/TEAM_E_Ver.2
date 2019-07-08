//=============================================================================
//
// �Q�[�W���� [SansoGauge.h]
// Author : �ڍ�������
//
//=============================================================================
#include "scene.h"
#include "SansoGauge.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "game.h"

//--------------------------------------------
//�ÓI�����o�ϐ��錾
//--------------------------------------------
LPDIRECT3DTEXTURE9			CSansoGauge::m_pTexture = {};


//=============================================================================
// �V�[���N���X�̃R���X�g���N�^
//=============================================================================
CSansoGauge::CSansoGauge(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
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
CSansoGauge *CSansoGauge::Create(D3DXVECTOR3 pos)
{
	CSansoGauge *pSansoGauge = NULL;

	if (pSansoGauge == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pSansoGauge = new CSansoGauge;

		if (pSansoGauge != NULL)
		{
			pSansoGauge->Init(pos);
		}
	}
	return pSansoGauge;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSansoGauge::~CSansoGauge()
{
}

//=============================================================================
// �Q�[�W�̏���������
//=============================================================================
HRESULT CSansoGauge::Init(D3DXVECTOR3 pos)
{
	// �Q�[�W�̈ʒu��ݒ�
	m_Pos = pos;

	// �����_���[���擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	//1p��
	m_pScene2D[0] = CScene2D::Create(D3DXVECTOR3(285.0f, pos.y, 0.0f));
	m_pScene2D[0]->SetWidthHeight(280.0f, 35.0f);
	m_pScene2D[0]->SetCol(D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f));
	m_pScene2D[0]->BindTexture(NULL);

	//2p��
	m_pScene2D[1] = CScene2D::Create(D3DXVECTOR3(995.0f, pos.y, 0.0f));
	m_pScene2D[1]->SetWidthHeight(280.0f, 35.0f);
	m_pScene2D[1]->SetCol(D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f));
	m_pScene2D[1]->BindTexture(NULL);

	return S_OK;
}

//=============================================================================
// �Q�[�W�̏I������
//=============================================================================
void CSansoGauge::Uninit(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// �I�u�W�F�N�g�̉��
	Release();
}

//=============================================================================
// �Q�[�W�̍X�V����
//=============================================================================
void CSansoGauge::Update(void)
{
	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

#ifdef _DEBUG

#if 1
	if (pInputKeyboard->GetPress(DIK_1) == true)
	{
		m_fRight -= 10.0f;
	}
	if (pInputKeyboard->GetPress(DIK_2) == true)
	{
		m_fRight += 10.0f;
	}

	if (pInputKeyboard->GetPress(DIK_3) == true)
	{
		m_fLeft += 10.0f;
	}
	if (pInputKeyboard->GetPress(DIK_4) == true)
	{
		m_fLeft -= 10.0f;
	}

	if (m_fLeft > 0 )
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
	m_pScene2D[0]->SetRIghtLeft(m_fRight, 0.0f);
	m_pScene2D[1]->SetRIghtLeft(0.0f, m_fLeft);

#endif
	//CDebugProc::Print("cfccfccfc", "2DPos    : x", m_Pos.x, "f", " y", m_Pos.y, "f", " z", m_Pos.z, "f");
#endif
}

//=============================================================================
// �Q�[�W�̕`�揈��
//=============================================================================
void CSansoGauge::Draw(void)
{
}

//=============================================================================
// �I�u�W�F�N�g�̎擾
//=============================================================================
D3DXVECTOR3 CSansoGauge::GetPosition(void)
{
	return m_Pos;
}

//=============================================================================
// �I�u�W�F�N�g�̐ݒ�
//=============================================================================
void CSansoGauge::SetPosition(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}


//=============================================================================
// ���E
//=============================================================================
void CSansoGauge::SetSansoGaugeRightLeft(float fRight, float fLeft)
{
	// �v���C���[�̎擾
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	// �G�l�~�[�̎擾
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

	m_fRight += fRight;
	m_fLeft += fLeft;
	//����𒴂��Ȃ��悤��
	if (m_fLeft > 0)
	{
		m_fLeft = 0;
	}
	else if (m_fLeft <= -550 && m_fLeft > -600)
	{
		pEnemy->SetRecovery(true);
		pEnemy->SetRecoveryTime(120);
		//m_fLeft = -600;
	}
	else if (m_fLeft <= -600)
	{
		m_fLeft = -600;
	}

	if (m_fRight > 0)
	{
		m_fRight = 0;
	}
	else if (m_fRight <= -550 && m_fRight > -600)
	{
		pPlayer->SetRecovery(true);
		pPlayer->SetRecoveryTime(120);
		//m_fRight = -600;
	}
	else if (m_fRight <= -600)
	{
		m_fRight = -600;
	}

	//�Q�[�W�̍��E�𒲐�
	m_pScene2D[0]->SetRIghtLeft(m_fRight, 0.0f);
	m_pScene2D[1]->SetRIghtLeft(0.0f, m_fLeft);
}
