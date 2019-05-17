//=============================================================================
//
// �o�g���V�X�e������ [BattleSystem.cpp]
// Author : �ڍ� ������
//
//=============================================================================
#include "BattleSystem.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "camera.h"

#include "player.h"
#include "enemy.h"
#include "game.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �}�N��
//=============================================================================
#define YORI_MOVE	(5.0f)
#define NAGE_MOVE	(8.0f)
#define OSI_MOVE	(8.0f)

#define YORI_FLAME	(20)
#define NAGE_FLAME	(30)
#define OSI_FLAME	(20)
//=============================================================================
// �o�g���V�X�e���N���X�̃R���X�g���N�^
//=============================================================================
CBattleSys::CBattleSys()
{
	// �l���N���A
	m_nCntFlame = 0;
	m_nImpossibleFlame = 0;
	m_bAttack = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBattleSys::~CBattleSys()
{
}


//=============================================================================
// �I�u�W�F�N�g�̐�������
//=============================================================================
CBattleSys *CBattleSys::Create()
{
	CBattleSys *pBattleSys = NULL;

	if (pBattleSys == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pBattleSys = new CBattleSys;

		if (pBattleSys != NULL)
		{
			pBattleSys->Init();
		}
	}

	return pBattleSys;
}

//=============================================================================
// �o�g���V�X�e������������
//=============================================================================
HRESULT CBattleSys::Init()
{
	// 2D�I�u�W�F�N�g����������
	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	//SetObjType(CScene::OBJTYPE_PLAYER);

	// �l���N���A
	m_nCntFlame = 0;
	m_nImpossibleFlame = 0;
	m_bAttack = false;

	return S_OK;
}

//=============================================================================
// �o�g���V�X�e���I������
//=============================================================================
void CBattleSys::Uninit(void)
{
}

//=============================================================================
// �o�g���V�X�e���X�V����
//=============================================================================
void CBattleSys::Update(void)
{
	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// �v���C���[�̎擾
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer ();
	// �G�l�~�[�̎擾
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();


#ifdef _DEBUG

	//2p�̕�������(��������ɓ���)
	if (pPlayer->GetState() == CPlayer::STATE_KUMI
		&& pEnemy->GetState() == CEnemy::STATE_KUMI
		&& m_bAttack == false)
	{
		CDebugProc::Print("c", " �g�ݍ��� ");

		//�����Ă���� �v���C���[
		switch (pPlayer->GetDirection())
		{
		case CPlayer::DIRECTION_LEFT:
			if (pInputKeyboard->GetPress(PLAYER_LEFT) == true)
			{
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true)
				{
					CDebugProc::Print("c", " ��� ");
					pPlayer->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = YORI_FLAME;
					m_bAttack = true;
				}
				else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true)
				{
					CDebugProc::Print("c", " ���� ");
					pEnemy->SetMove(D3DXVECTOR3(-OSI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = OSI_FLAME;
					m_bAttack = true;
					//�d��
					Recovery();
				}
			}
			else if (pInputKeyboard->GetPress(PLAYER_RIGHT) == true)
			{
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true)
				{
					CDebugProc::Print("c", " ������ ");
					pPlayer->SetMove(D3DXVECTOR3(-NAGE_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(NAGE_MOVE, 3.0f, 0.0f));
					m_bAttack = true;
					m_nCntFlame = NAGE_FLAME;
					//CGame::SetHit(false);
					//pPlayer->SetState(CPlayer::STATE_NEUTRAL);
					//pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				}
			}
			break;
		case CPlayer::DIRECTION_RIGHT:
			if (pInputKeyboard->GetPress(PLAYER_RIGHT) == true)
			{
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true)
				{
					CDebugProc::Print("c", " ��� ");
					pPlayer->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = YORI_FLAME;
					m_bAttack = true;
				}
				else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true)
				{
					CDebugProc::Print("c", " ���� ");
					pEnemy->SetMove(D3DXVECTOR3(OSI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = OSI_FLAME;
					m_bAttack = true;
					//�d��
					Recovery();
				}
			}
			else if (pInputKeyboard->GetPress(PLAYER_LEFT) == true)
			{
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true)
				{
					CDebugProc::Print("c", " ������ ");
					pPlayer->SetMove(D3DXVECTOR3(NAGE_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(-NAGE_MOVE, 3.0f, 0.0f));
					m_bAttack = true;
					m_nCntFlame = NAGE_FLAME;
					//CGame::SetHit(false);
					//pPlayer->SetState(CPlayer::STATE_NEUTRAL);
					//pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				}
			}
			break;
		}

		//�����Ă���� �G�l�~�[
		switch (pEnemy->GetDirection())
		{
		case CEnemy::DIRECTION_LEFT:
			if (pInputKeyboard->GetPress(ENEMY_LEFT) == true)
			{
				if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true)
				{
					CDebugProc::Print("c", " ��� ");
					pPlayer->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					m_bAttack = true;
					m_nCntFlame = YORI_FLAME;
				}
				else if (pInputKeyboard->GetTrigger(ENEMY_B_BUTTON) == true)
				{
					CDebugProc::Print("c", " ���� ");
					pPlayer->SetMove(D3DXVECTOR3(-OSI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = OSI_FLAME;
					m_bAttack = true;
					//�d��
					Recovery();
				}
			}
			else if (pInputKeyboard->GetPress(ENEMY_RIGHT) == true)
			{
				if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true)
				{
					CDebugProc::Print("c", " ������ ");
					pPlayer->SetMove(D3DXVECTOR3(NAGE_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(-NAGE_MOVE, 3.0f, 0.0f));
					m_bAttack = true;
					m_nCntFlame = NAGE_FLAME;
					//CGame::SetHit(false);
					//pPlayer->SetState(CPlayer::STATE_NEUTRAL);
					//pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				}
			}

			break;
		case CEnemy::DIRECTION_RIGHT:
			if (pInputKeyboard->GetPress(ENEMY_RIGHT) == true)
			{
				if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true)
				{
					CDebugProc::Print("c", " ��� ");
					pPlayer->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					m_bAttack = true;
					m_nCntFlame = YORI_FLAME;
				}
				else if (pInputKeyboard->GetTrigger(ENEMY_B_BUTTON) == true)
				{
					CDebugProc::Print("c", " ���� ");
					pPlayer->SetMove(D3DXVECTOR3(OSI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = OSI_FLAME;
					m_bAttack = true;
					//�d��
					Recovery();
				}
			}
			else if (pInputKeyboard->GetPress(ENEMY_LEFT) == true)
			{
				if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true)
				{
					CDebugProc::Print("c", " ������ ");
					pPlayer->SetMove(D3DXVECTOR3(-NAGE_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(NAGE_MOVE, 3.0f, 0.0f));
					m_bAttack = true;
					m_nCntFlame = NAGE_FLAME;
					//CGame::SetHit(false);
					//pPlayer->SetState(CPlayer::STATE_NEUTRAL);
					//pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				}
			}
			break;
		}
	}
	//���Z�b�g
	if (pInputKeyboard->GetTrigger(DIK_R) == true)
	{
		pPlayer->SetPosition(D3DXVECTOR3(-20.0f, 50.0f, 0.0f));
		pEnemy->SetPosition(D3DXVECTOR3(20.0f, 50.0f, 0.0f));
		pPlayer->SetState(CPlayer::STATE_NEUTRAL);
		pEnemy->SetState(CEnemy::STATE_NEUTRAL);
		m_bAttack = false;
		m_nCntFlame = 0;
		CGame::SetHit(false);
	}

	if (pPlayer->GetState() == CPlayer::STATE_NEUTRAL
		&& pEnemy->GetState() == CEnemy::STATE_NEUTRAL
		&& m_bAttack == false)
	{
		CDebugProc::Print("c", " �ʏ� ");
	}

	if (m_bAttack == true)
	{
		m_nCntFlame--;
		if (m_nCntFlame <= 0)
		{
			m_nCntFlame = 0;
			m_bAttack = false;
		}
	}

	CDebugProc::Print("cn", " �s���s�t���[�� ",m_nCntFlame);

#endif
}


//=============================================================================
// �d�������܂Ƃ�
//=============================================================================
void CBattleSys::Recovery(void)
{
	// �v���C���[�̎擾
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	// �G�l�~�[�̎擾
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

	CGame::SetHit(false);
	pPlayer->SetState(CPlayer::STATE_NEUTRAL);
	pEnemy->SetState(CEnemy::STATE_NEUTRAL);
	pPlayer->SetRecovery(true);
	pPlayer->SetRecoveryTime(20);
	pEnemy->SetRecovery(true);
	pEnemy->SetRecoveryTime(20);
}