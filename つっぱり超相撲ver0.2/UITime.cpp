//---------------------------------------------------------------------
//	�^�C������(UITime.cpp)
//	Author : Mikiya Meguro
//---------------------------------------------------------------------
#include "UITime.h"
#include "number.h"
#include "manager.h"
#include "fade.h"
#include "game.h"
#include "tutorial.h"
#include "player.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define INIT_TIME		(60)

//--------------------------------------------
//�^�C���N���X �R���X�g���N�^
//--------------------------------------------
CUITime::CUITime(int nPriority) : CScene(nPriority)
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_fLength = 0;
	m_nTime = 0;
	m_nTimerCnt = 0;
	m_nFadeCnt = 0;
	m_nColorFlash = 0;

	for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
	{
		m_apNumber[nCnt] = NULL;
	}
}

//--------------------------------------------
//�^�C���N���X �f�X�g���N�^
//--------------------------------------------
CUITime::~CUITime()
{
}

//--------------------------------------------
//�I�u�W�F�N�g�̐���
//--------------------------------------------
CUITime *CUITime::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CUITime *pTime;
	pTime = new CUITime;
	pTime->m_pos = pos;
	pTime->m_fWidth = fWidth;
	pTime->m_fHeight = fHeight;
	pTime->Init(pos);
	return pTime;
}


//=============================================================================
// �^�C���̏���������
//=============================================================================
HRESULT CUITime::Init(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
	{
		m_apNumber[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
	{
		if (m_apNumber[nCnt] == NULL)
		{	//�����̏�����
			m_apNumber[nCnt] = CNumber::Create(D3DXVECTOR3((m_pos.x - (m_fWidth * 1.2f) * nCnt), m_pos.y, m_pos.z),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), m_fWidth, m_fHeight);
		}
	}
	//�^�C��������
	m_nTime = INIT_TIME;
	m_nTimerCnt = 0;
	m_nFadeCnt = 0;
	m_nColorFlash = 0;

	AddTime(0);
	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(OBJTYPE_SCENE2D);
	return S_OK;
}

//=============================================================================
// �^�C���̏I������
//=============================================================================
void CUITime::Uninit(void)
{
	//������j��
	Release();
}

//=============================================================================
// �^�C���̍X�V����
//=============================================================================
void CUITime::Update(void)
{
	CManager *pManager = NULL;
	CFade *pFade = pManager->GetFade();

	// �v���C���[�̎擾
	CPlayer *pPlayer = NULL;
	//���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();
	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
	}

	float AlphaCol = 1.0f;

	if (CFade::GetFade() == CFade::FADE_NONE)
	{
		//10�b�ȉ�
		if (m_nTime <= 10)
		{
			SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, AlphaCol));
		}
#if 1
		if (pPlayer->GetState() != CPlayer::STATE_JANKEN
			&& pPlayer->GetState() != CPlayer::STATE_NOKOTTA
			&& mode == CManager::MODE_GAME)
		{
			if (m_nTime > 0)
			{	//�J�E���g���Z
				m_nTimerCnt++;
				if (m_nTimerCnt >= 60)
				{	//�J�E���g������
					m_nTimerCnt = 0;
					//���ۂ̒l������
					m_nTime -= 1;
					//�X�V���邾��
					AddTime(0);
				}
			}
			else if (m_nTime <= 0)
			{
				//�������Ԃ�0��
				m_nTime = 0;
				//�t�F�[�h�܂ł̃J�E���g
				m_nFadeCnt++;
				if (m_nFadeCnt >= 60)
				{
					CManager::GetGame()->TimeOver();
				}
			}
		}
#endif
	}
}

//=============================================================================
// �^�C���̕`�揈��
//=============================================================================
void CUITime::Draw(void)
{

}

//=============================================================================
// �����̐ݒ菈��
//=============================================================================
void CUITime::SetTime(int nTime)
{
	m_nTime = nTime;
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	AddTime(0);
}

//=============================================================================
// �����̉��Z����
//=============================================================================
void CUITime::AddTime(int nValue)
{
	int nAnswer = 0;
	int nDigit = 1;
	//�^�C�����Z
	m_nTime += nValue;

	for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
	{
		//�����̌v�Z
		nAnswer = m_nTime % (nDigit * 10) / nDigit;
		//���̌��̐������Z�b�g
		if (m_apNumber[nCnt] != NULL)
		{
			m_apNumber[nCnt]->SetNumber(nAnswer);
		}
		//���̌���
		nDigit *= 10;
	}
}
//=============================================================================
// ���݂̃^�C����Ԃ�
//=============================================================================
int CUITime::GetTime()
{
	return m_nTime;
}
//=============================================================================
// �F��ݒ�
//=============================================================================
void CUITime::SetColor(D3DXCOLOR col)
{
	for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
	{
		m_apNumber[nCnt]->SetColor(col);
	}
}

