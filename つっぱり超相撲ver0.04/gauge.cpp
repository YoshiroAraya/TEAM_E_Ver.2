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
#include "gauge.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
//--------------------------------------------
//�ÓI�����o�ϐ��錾
//--------------------------------------------
LPDIRECT3DTEXTURE9			CGauge::m_pTexture = {};


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
	m_pScene2D[0] = CScene2D::Create(D3DXVECTOR3(300.0f, 580.0f, 0.0f));
	m_pScene2D[0]->SetWidthHeight(300.0f, 50.0f);
	m_pScene2D[0]->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	m_pScene2D[0]->BindTexture(NULL);

	//2p��
	m_pScene2D[1] = CScene2D::Create(D3DXVECTOR3(980.0f, 580.0f, 0.0f));
	m_pScene2D[1]->SetWidthHeight(300.0f, 50.0f);
	m_pScene2D[1]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pScene2D[1]->BindTexture(NULL);

	return S_OK;
}

//=============================================================================
// �Q�[�W�̏I������
//=============================================================================
void CGauge::Uninit(void)
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
void CGauge::Update(void)
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
// �e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CGauge::Load(void)
{
	//�f�o�C�X���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, GAUGE_NAME, &m_pTexture);

	return S_OK;
}
//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CGauge::UnLoad(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// ���E
//=============================================================================
void CGauge::SetGaugeRightLeft(float fRight, float fLeft)
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
	else if (m_fLeft > -570)
	{
		pEnemy->SetDying(false);
	}
	else if (m_fLeft <= -570)
	{
		pEnemy->SetDying(true);
	}
	else if (m_fLeft < -600)
	{
		m_fLeft = -600;
	}


	if (m_fRight > 0)
	{
		m_fRight = 0;
	}
	else if (m_fRight > -570)
	{
		pPlayer->SetDying(false);
	}
	else if (m_fRight <= -570)
	{
		pPlayer->SetDying(true);
	}
	else if (m_fRight < -600)
	{
		m_fRight = -600;
	}

	//�Q�[�W�̍��E�𒲐�
	m_pScene2D[0]->SetRIghtLeft(m_fRight, 0.0f);
	m_pScene2D[1]->SetRIghtLeft(0.0f, m_fLeft);
}
