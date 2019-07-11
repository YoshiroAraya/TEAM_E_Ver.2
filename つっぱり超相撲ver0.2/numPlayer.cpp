//=============================================================================
//
// �l���I������ [numPlayer.cpp]
// Author : ���R���
//
//=============================================================================
#include "numPlayer.h"
#include "renderer.h"
#include "input.h"
#include "scene2D.h"
#include "manager.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "load.h"
#include "title.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUMPLAYER_POS_X		((SCREEN_WIDTH / 2))		// �R�����݂̍���X���W
#define NUMPLAYER_POS_Y		((SCREEN_HEIGHT / 2))		// �R�����݂̍���Y���W
#define PAUSE_WIDTH		((SCREEN_WIDTH / 2))		// �R�����݂̕�
#define PAUSE_HEIGHT	((SCREEN_HEIGHT / 2))		// �R�����݂̍���
#define PAUSE_SIZE		(200)						// �R�����݂̃T�C�Y

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CNumPlayer::MODE CNumPlayer::m_Mode = CNumPlayer::MODE_1P;
bool CNumPlayer::m_bDecision = false;

//=============================================================================
// �R�����݂̐�������
//=============================================================================
CNumPlayer *CNumPlayer::Create(D3DXVECTOR3 pos)
{
	CNumPlayer *pNumPlayer = NULL;

	if (pNumPlayer == NULL)
	{
		// �w�i�N���X�̐���
		pNumPlayer = new CNumPlayer;

		if (pNumPlayer != NULL)
		{
			pNumPlayer->Init(pos);
		}
	}
	return pNumPlayer;
}

//=============================================================================
// �R�����݃N���X�̃R���X�g���N�^
//=============================================================================
CNumPlayer::CNumPlayer(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �l���N���A
	for (int nCntPause = 0; nCntPause < MAX_NUMPLAYER; nCntPause++)
	{
		m_apScene2D[nCntPause] = NULL;
	}

	for (int nCntPause = 0; nCntPause < MAX_NUMPLAYER; nCntPause++)
	{
		m_aCol[nCntPause] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}

	m_nSelect = 0;
}

//=============================================================================
// �R�����݃N���X�̃f�X�g���N�^
//=============================================================================
CNumPlayer::~CNumPlayer()
{
}

//=============================================================================
// �R�����ݏ���������
//=============================================================================
HRESULT CNumPlayer::Init(D3DXVECTOR3 pos)
{
	m_nSelect = 0;
	m_Mode = MODE_1P;
	m_bDecision = false;

	for (int nCntPause = 0; nCntPause < MAX_NUMPLAYER; nCntPause++)
	{
		m_aSelect[nCntPause] = SELECT_NONE;
		m_aCol[nCntPause] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	m_apScene2D[0] = new CScene2D(7);
	m_apScene2D[0]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_1P));
	m_apScene2D[0]->Init(pos);
	m_apScene2D[0]->SetWidthHeight(250.0f, 100.0f);
	m_apScene2D[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	// 1P��I����Ԃɂ���
	m_aSelect[0] = SELECT_SELECT;

	m_apScene2D[1] = new CScene2D(7);
	m_apScene2D[1]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_2P));
	m_apScene2D[1]->Init(D3DXVECTOR3(pos.x - 25.0f, pos.y + 150.0f, 0.0f));
	m_apScene2D[1]->SetWidthHeight(250.0f, 100.0f);
	m_apScene2D[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_apScene2D[2] = new CScene2D(7);
	m_apScene2D[2]->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_TUTORIAL));
	m_apScene2D[2]->Init(D3DXVECTOR3(pos.x - 25.0f, pos.y + 300.0f, 0.0f));
	m_apScene2D[2]->SetWidthHeight(300.0f, 100.0f);
	m_apScene2D[2]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));


	return S_OK;
}

//=============================================================================
// �R�����ݏI������
//=============================================================================
void CNumPlayer::Uninit(void)
{
	// 2D�I�u�W�F�N�g�I������
	for (int nCntPause = 0; nCntPause < MAX_NUMPLAYER; nCntPause++)
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
// �R�����ݍX�V����
//=============================================================================
void CNumPlayer::Update(void)
{
	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();

	// �v���C���[�̎擾
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	// �G�l�~�[�̎擾
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

	// �^�C�g���擾
	CTitle *pTitle;
	pTitle = CManager::GetTitle();

	if (pInputKeyboard->GetTrigger(DIK_DOWN) == true/* || pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_DOWN) == true
		|| pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_LS_DOWN) == true*/)
	{
		//pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
		m_aSelect[m_nSelect] = SELECT_NONE;
		m_nSelect = (m_nSelect + 1) % 3;
		//if (m_nSelect == 0)
		//{
		//	m_nSelect = (m_nSelect + 1);
		//}
		m_aSelect[m_nSelect] = SELECT_SELECT;
	}
	if (pInputKeyboard->GetTrigger(DIK_UP) == true/* || pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_UP) == true
		|| pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_LS_UP) == true*/)
	{
		//pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
		m_aSelect[m_nSelect] = SELECT_NONE;
		m_nSelect = (m_nSelect + 2) % 3;
		/*if (m_nSelect == 1)
		{
			m_nSelect = (m_nSelect - 1);
		}*/
		m_aSelect[m_nSelect] = SELECT_SELECT;
	}

	for (int nCntPause = 0; nCntPause < MAX_NUMPLAYER; nCntPause++)
	{
		if (m_aSelect[nCntPause] == SELECT_SELECT)
		{// �I��
			m_aCol[m_nSelect] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{// �I�����Ă��Ȃ�
			m_aCol[m_nSelect] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}

		if (m_apScene2D[nCntPause] != NULL)
		{// ��񂪓����Ă�����F��ݒ肷��
			m_apScene2D[nCntPause]->SetCol(m_aCol[m_nSelect]);
		}
	}

	if (pTitle->GetState() == CTitle::STATE_TITLE && pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		m_bDecision = true;

		if (m_aSelect[0] == SELECT_SELECT)
		{
			m_Mode = MODE_1P;
		}
		else if (m_aSelect[1] == SELECT_SELECT)
		{
			m_Mode = MODE_2P;
		}
		else if (m_aSelect[2] == SELECT_SELECT)
		{
			m_Mode = MODE_2P;
		}
	}

	if (pTitle->GetState() == CTitle::STATE_CHARASELECT)
	{
		Uninit();
	}
}

//=============================================================================
// �R�����ݕ`�揈��
//=============================================================================
void CNumPlayer::Draw(void)
{
}

//=============================================================================
// �R�����݃Z���N�g�̎擾
//=============================================================================
CNumPlayer::MODE CNumPlayer::GetMode(void)
{
	return m_Mode;
}

//=============================================================================
// �Z���N�g�̎擾
//=============================================================================
int CNumPlayer::GetSelect(void)
{
	return m_nSelect;
}