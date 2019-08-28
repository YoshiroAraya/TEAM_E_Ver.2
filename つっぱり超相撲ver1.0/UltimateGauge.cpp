//=============================================================================
//
// �Q�[�W���� [UltimateGauge.h]
// Author : �ڍ�������
//
//=============================================================================
#include "scene.h"
#include "UltimateGauge.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "load.h"
//--------------------------------------------
//�}�N����`
//--------------------------------------------
#define MAX_GAUG_WIDE	(450)
#define GAUGE_POSX		(350)
//=============================================================================
// �V�[���N���X�̃R���X�g���N�^
//=============================================================================
CUltimateGauge::CUltimateGauge(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
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
CUltimateGauge *CUltimateGauge::Create(D3DXVECTOR3 pos)
{
	CUltimateGauge *pGauge = NULL;

	if (pGauge == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pGauge = new CUltimateGauge;

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
CUltimateGauge::~CUltimateGauge()
{
}

//=============================================================================
// �Q�[�W�̏���������
//=============================================================================
HRESULT CUltimateGauge::Init(D3DXVECTOR3 pos)
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
	m_pScene2D[0] = CScene2D::Create(D3DXVECTOR3(0 + GAUGE_POSX, pos.y, 0.0f));
	m_pScene2D[0]->SetWidthHeight(MAX_GAUG_WIDE / 2, 20.0f);
	m_pScene2D[0]->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	m_pScene2D[0]->BindTexture(NULL);
	m_bUlt[0] = false;

	//2p��
	m_pScene2D[1] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH - GAUGE_POSX, pos.y, 0.0f));
	m_pScene2D[1]->SetWidthHeight(MAX_GAUG_WIDE / 2, 20.0f);
	m_pScene2D[1]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pScene2D[1]->BindTexture(NULL);
	m_bUlt[1] = false;

	m_pScene2D[0]->SetRIghtLeft(-MAX_GAUG_WIDE, 0.0f);
	m_pScene2D[1]->SetRIghtLeft(0.0f, -MAX_GAUG_WIDE);

	m_fRight = -MAX_GAUG_WIDE;
	m_fLeft = -MAX_GAUG_WIDE;			// ���E�̒l

	m_pHPgaugeBG[0] = CScene2D::Create(D3DXVECTOR3(GAUGE_POSX, pos.y + 0.19f, 0.0f), 2);
	m_pHPgaugeBG[0]->SetWidthHeight(233.0f, 25.0f);
	m_pHPgaugeBG[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pHPgaugeBG[0]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_EXGAUGEBG));

	m_pHPgaugeBG[1] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH - GAUGE_POSX, pos.y + 0.19f, 0.0f), 2);
	m_pHPgaugeBG[1]->SetWidthHeight(233.0f, 25.0f);
	m_pHPgaugeBG[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pHPgaugeBG[1]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_EXGAUGEBG));
	m_pHPgaugeBG[1]->SetRot(D3DX_PI);

	return S_OK;
}

//=============================================================================
// �Q�[�W�̏I������
//=============================================================================
void CUltimateGauge::Uninit(void)
{

	// �I�u�W�F�N�g�̉��
	Release();
}

//=============================================================================
// �Q�[�W�̍X�V����
//=============================================================================
void CUltimateGauge::Update(void)
{
	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();


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
	if (m_fLeft > 0 )
	{
		m_fLeft = 0;
	}
	else if (m_fLeft < -MAX_GAUG_WIDE)
	{
		m_fLeft = -MAX_GAUG_WIDE;
	}
	if (m_fLeft == 0)
	{
		m_bUlt[1] = true;
	}
	else if (m_fLeft <= 0)
	{
		m_bUlt[1] = false;
	}

	if (m_fRight > 0)
	{
		m_fRight = 0;
	}
	else if (m_fRight < -MAX_GAUG_WIDE)
	{
		m_fRight = -MAX_GAUG_WIDE;
	}
	if (m_fRight == 0)
	{
		m_bUlt[0] = true;
	}
	else if (m_fRight <= 0)
	{
		m_bUlt[0] = false;
	}

	//�Q�[�W�̍��E�𒲐�
	m_pScene2D[0]->SetRIghtLeft(0.0f, m_fLeft);
	m_pScene2D[1]->SetRIghtLeft(m_fRight, 0.0f);

	//CDebugProc::Print("cf", "m_fRight    : ", m_fRight);
	//CDebugProc::Print("cf", "m_fLeft    : ", m_fLeft);
#endif
}

//=============================================================================
// �Q�[�W�̕`�揈��
//=============================================================================
void CUltimateGauge::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();


}

//=============================================================================
// �I�u�W�F�N�g�̎擾
//=============================================================================
D3DXVECTOR3 CUltimateGauge::GetPosition(void)
{
	return m_Pos;
}

//=============================================================================
// �I�u�W�F�N�g�̐ݒ�
//=============================================================================
void CUltimateGauge::SetPosition(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
// ���E
//=============================================================================
void CUltimateGauge::SetGaugeRightLeft(float fRight, float fLeft)
{
	m_fRight += fRight;
	m_fLeft += fLeft;
	//����𒴂��Ȃ��悤��
	if (m_fLeft > 0)
	{
		m_fLeft = 0;
	}
	else if (m_fLeft < -MAX_GAUG_WIDE)
	{
		m_fLeft = -MAX_GAUG_WIDE;
	}

	if (m_fRight > 0)
	{
		m_fRight = 0;
	}
	else if (m_fRight < -MAX_GAUG_WIDE)
	{
		m_fRight = -MAX_GAUG_WIDE;
	}

	//�Q�[�W�̍��E�𒲐�
	m_pScene2D[0]->SetRIghtLeft(0.0f, m_fLeft);
	m_pScene2D[1]->SetRIghtLeft(m_fRight, 0.0f);
}
