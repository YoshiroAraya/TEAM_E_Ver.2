//=============================================================================
//
// ����e�N�X�`���̏��� [charaSelect.cpp]
// Author : ���R���
//
//=============================================================================
#include "charaSelect.h"
#include "renderer.h"
#include "input.h"
#include "scene2D.h"
#include "manager.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "camera.h"
#include "load.h"
#include "title.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PAUSE_POS_X		((SCREEN_WIDTH / 2))		// �����̍���X���W
#define PAUSE_POS_Y		((SCREEN_HEIGHT / 2))		// �����̍���Y���W
#define PAUSE_WIDTH		((SCREEN_WIDTH / 2))		// �����̕�
#define PAUSE_HEIGHT	((SCREEN_HEIGHT / 2))		// �����̍���
#define SELECT_SIZE		(50.0f)						// �����̃T�C�Y

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// ����e�N�X�`���̐�������
//=============================================================================
CCharaSelect *CCharaSelect::Create(D3DXVECTOR3 pos)
{
	CCharaSelect *pCharaSelect = NULL;

	if (pCharaSelect == NULL)
	{
		// �w�i�N���X�̐���
		pCharaSelect = new CCharaSelect;

		if (pCharaSelect != NULL)
		{
			pCharaSelect->Init(pos);
		}
	}
	return pCharaSelect;
}

//=============================================================================
// ����e�N�X�`���N���X�̃R���X�g���N�^
//=============================================================================
CCharaSelect::CCharaSelect(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �l���N���A
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	m_nSelect = 0;
	m_fScale = 0.0f;

	for (int nCntPause = 0; nCntPause < MAX_SELECT; nCntPause++)
	{
		m_apScene2D[nCntPause] = NULL;
	}
}

//=============================================================================
// ����e�N�X�`���N���X�̃f�X�g���N�^
//=============================================================================
CCharaSelect::~CCharaSelect()
{
}

//=============================================================================
// ����e�N�X�`������������
//=============================================================================
HRESULT CCharaSelect::Init(D3DXVECTOR3 pos)
{
	CNumPlayer::MODE mode;
	mode = CNumPlayer::GetMode();

	m_nSelect = 0;
	m_fScale = SELECT_SIZE;
	m_bReset = true;

	m_apScene2D[0] = new CScene2D(SELECT_PRIORITY);
	m_apScene2D[0]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_CHARASELECT));
	m_apScene2D[0]->Init(pos);
	m_apScene2D[0]->SetWidthHeight(100.0f, 100.0f);
	m_apScene2D[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	if (mode != NULL)
	{
		if (mode == CNumPlayer::MODE_2P)
		{
			m_apScene2D[1] = new CScene2D(SELECT_PRIORITY);
			m_apScene2D[1]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_CHARASELECT));
			m_apScene2D[1]->Init(D3DXVECTOR3(SCREEN_WIDTH - pos.x, pos.y, pos.z));
			m_apScene2D[1]->SetWidthHeight(100.0f, 100.0f);
			m_apScene2D[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	
	return S_OK;
}

//=============================================================================
// ����e�N�X�`���I������
//=============================================================================
void CCharaSelect::Uninit(void)
{
	// 2D�I�u�W�F�N�g�I������
	for (int nCntPause = 0; nCntPause < MAX_SELECT; nCntPause++)
	{
		if (m_apScene2D[nCntPause] != NULL)
		{
			m_apScene2D[nCntPause]->Uninit();
			m_apScene2D[nCntPause] = NULL;
		}
	}

	// �I�u�W�F�N�g�̉��
	Release();
}

//=============================================================================
// ����e�N�X�`���X�V����
//=============================================================================
void CCharaSelect::Update(void)
{
	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();

	CNumPlayer::MODE mode;
	mode = CNumPlayer::GetMode();

	// �^�C�g���擾
	CTitle *pTitle = CManager::GetTitle();

	if (pTitle->GetCharaSelect(0) == false)
	{// �I��
		// �e�N�X�`�����W�̐ݒ�
		m_apScene2D[0]->SetTex(D3DXVECTOR2(0.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
	}
	else if(pTitle->GetCharaSelect(0) == true)
	{// ����
		// �e�N�X�`�����W�̐ݒ�
		m_apScene2D[0]->SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.5f));
	}

	if (mode != NULL)
	{
		if (mode == CNumPlayer::MODE_2P)
		{
			if (pTitle->GetCharaSelect(1) == false)
			{// �I��
			 // �e�N�X�`�����W�̐ݒ�
				m_apScene2D[1]->SetTex(D3DXVECTOR2(0.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
			}
			else if (pTitle->GetCharaSelect(1) == true)
			{// ����
			 // �e�N�X�`�����W�̐ݒ�
				m_apScene2D[1]->SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.5f));
			}
		}
	}
}

//=============================================================================
// ����e�N�X�`���`�揈��
//=============================================================================
void CCharaSelect::Draw(void)
{
}