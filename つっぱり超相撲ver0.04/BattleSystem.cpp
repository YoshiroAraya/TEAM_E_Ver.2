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

#include "gauge.h"
//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �}�N��
//=============================================================================
#define YORI_MOVE	(5.0f)
#define NAGE_MOVE	(25.0f)
#define OSI_MOVE	(8.0f)

#define YORI_FLAME	(20)
#define NAGE_FLAME	(30)
#define OSI_FLAME	(20)
#define GU_COUNTER	(70)
#define CHOKI_COUNTER	(10)

#define DAMAGE			(30)
#define HEEL			(20)
//=============================================================================
// �o�g���V�X�e���N���X�̃R���X�g���N�^
//=============================================================================
CBattleSys::CBattleSys()
{
	// �l���N���A
	m_nCntFlame = 0;
	m_nImpossibleFlame = 0;
	m_bAttack = false;

	for (int nCntPlayer = 0; nCntPlayer < MAX_CHARACTER; nCntPlayer++)
	{
		m_aGUCounter[nCntPlayer] = 0;
		m_aCHOKICounter[nCntPlayer] = 0;
		m_abPA[nCntPlayer] = false;
	}
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
	m_aJanken[0] = JANKEN_GU;
	m_aJanken[1] = JANKEN_GU;
	for (int nCntPlayer = 0; nCntPlayer < MAX_CHARACTER; nCntPlayer++)
	{
		m_aGUCounter[nCntPlayer] = 0;
		m_aCHOKICounter[nCntPlayer] = 0;
		m_abPA[nCntPlayer] = false;
	}

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
	pPlayer = CGame::GetPlayer();
	// �G�l�~�[�̎擾
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();
	//�Q�[�W�̎擾
	CGauge *pGauge;
	pGauge = CGame::GetGauge();

#ifdef _DEBUG

	//�L�[�{�[�h/�R���g���[���[����
	Operation();



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

	CDebugProc::Print("cn", " �s���s�t���[�� ", m_nCntFlame);

#endif
}

//=============================================================================
// �L�[�{�[�h/�R���g���[���[�̑��쏈��
//=============================================================================
void CBattleSys::Operation(void)
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

	CGauge *pGauge;
	pGauge = CGame::GetGauge();

#ifdef _DEBUG

	if (pPlayer->GetState() == CPlayer::STATE_JANKEN)
	{
		CDebugProc::Print("c", " p����񂯂� ");

		if (pInputKeyboard->GetPress(DIK_Z) == true || 
			pXInput->GetPress(XPLAYER_B_BUTTON, 0) == true)
		{
			m_aJanken[0] = JANKEN_GU;
			pPlayer->SetState(CPlayer::STATE_NOKOTTA);
		}
		else if (pInputKeyboard->GetPress(DIK_X) == true ||
			pXInput->GetPress(XPLAYER_Y_BUTTON, 0) == true)
		{
			m_aJanken[0] = JANKEN_CHOKI;
			pPlayer->SetState(CPlayer::STATE_NOKOTTA);
		}
		else if (pInputKeyboard->GetPress(DIK_Z) == true || 
			pXInput->GetPress(XPLAYER_X_BUTTON, 0) == true)
		{
			m_aJanken[0] = JANKEN_PA;
			pPlayer->SetState(CPlayer::STATE_NOKOTTA);
			m_abPA[0] = true;
		}
	}

	if (pEnemy->GetState() == CEnemy::STATE_JANKEN)
	{
		CDebugProc::Print("c", " e����񂯂� ");

		if (pInputKeyboard->GetPress(DIK_B) == true || 
			pXInput->GetPress(XENEMY_B_BUTTON, 1) == true)
		{
			m_aJanken[1] = JANKEN_GU;
			pEnemy->SetState(CEnemy::STATE_NOKOTTA);
		}
		else if (pInputKeyboard->GetPress(DIK_N) == true || 
			pXInput->GetPress(XENEMY_Y_BUTTON, 1) == true)
		{
			m_aJanken[1] = JANKEN_CHOKI;
			pEnemy->SetState(CEnemy::STATE_NOKOTTA);
		}
		else if (pInputKeyboard->GetPress(DIK_M) == true || 
			pXInput->GetPress(XENEMY_X_BUTTON, 1) == true)
		{
			m_aJanken[1] = JANKEN_PA;
			pEnemy->SetState(CEnemy::STATE_NOKOTTA);
			m_abPA[1] = true;
		}
	}

	if (pPlayer->GetState() == CPlayer::STATE_NOKOTTA && pEnemy->GetState() == CEnemy::STATE_NOKOTTA)
	{// 2�l�̂���񂯂񂪌��܂�����
	 //===================================
	 // �L����1
	 //===================================
		if (m_aJanken[0] == JANKEN_GU)
		{
			m_aGUCounter[0]++;

			if (m_aGUCounter[0] < GU_COUNTER)
			{
				pPlayer->SetMove(D3DXVECTOR3(3.0f, 0.0f, 0.0f));
			}
			else if (m_aGUCounter[0] >= GU_COUNTER)
			{
				pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				m_aGUCounter[0] = 0;
			}
		}
		else if (m_aJanken[0] == JANKEN_CHOKI)
		{
			m_aCHOKICounter[0]++;

			if (m_aCHOKICounter[0] > CHOKI_COUNTER && CHOKI_COUNTER + 5 >= m_aCHOKICounter[0])
			{
				pEnemy->SetMove(D3DXVECTOR3(OSI_MOVE, 3.0f, 0.0f));
			}
			else if (m_aCHOKICounter[0] > CHOKI_COUNTER + 5)
			{
				pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				m_aCHOKICounter[0] = 0;
			}
		}
		else if (m_aJanken[0] == JANKEN_PA)
		{
			if (m_abPA[0] == true)
			{
				pPlayer->SetMove(D3DXVECTOR3(0.0, 10.0f, 0.0f));
				m_abPA[0] = false;
			}

			if (pEnemy->GetState() == CEnemy::STATE_NEUTRAL || m_aJanken[1] == JANKEN_PA)
			{
				pPlayer->SetState(CPlayer::STATE_NEUTRAL);
			}
		}

		//===================================
		// �L����2
		//===================================
		if (m_aJanken[1] == JANKEN_GU)
		{
			m_aGUCounter[1]++;

			if (m_aGUCounter[1] < GU_COUNTER)
			{
				pEnemy->SetMove(D3DXVECTOR3(-3.0f, 0.0f, 0.0f));
			}
			else if (m_aGUCounter[1] >= GU_COUNTER)
			{
				pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				m_aGUCounter[1] = 0;
			}
		}
		else if (m_aJanken[1] == JANKEN_CHOKI)
		{
			m_aCHOKICounter[1]++;

			if (m_aCHOKICounter[1] > CHOKI_COUNTER && CHOKI_COUNTER + 5 >= m_aCHOKICounter[1])
			{
				pPlayer->SetMove(D3DXVECTOR3(-OSI_MOVE, 3.0f, 0.0f));
			}
			else if (m_aCHOKICounter[1] > CHOKI_COUNTER + 5)
			{
				pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				m_aCHOKICounter[1] = 0;
			}
		}
		else if (m_aJanken[1] == JANKEN_PA)
		{
			if (m_abPA[1] == true)
			{
				pEnemy->SetMove(D3DXVECTOR3(0.0, 10.0f, 0.0f));
				m_abPA[1] = false;
			}

			if (pPlayer->GetState() == CPlayer::STATE_NEUTRAL || m_aJanken[0] == JANKEN_PA)
			{
				pEnemy->SetState(CEnemy::STATE_NEUTRAL);
			}
		}
	}

	//�m�����̈ړ���
	float fMoveDying[2] = { 1.0f,1.0f };
	//�m����
	if (pPlayer->GetDying() == true)
	{
	}
	if (pEnemy->GetDying() == true)
	{
	}

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
			if (pInputKeyboard->GetPress(PLAYER_LEFT) == true || 
				pXInput->GetPress(XPLAYER_LEFT, 0) == true)
			{
				if (pInputKeyboard->GetPress(PLAYER_A_BUTTON) == true || 
					pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					CDebugProc::Print("c", " ��� ");
					pPlayer->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = YORI_FLAME;
					pGauge->SetGaugeRightLeft(HEEL, -DAMAGE);
					m_bAttack = true;
				}
				else if (pInputKeyboard->GetPress(PLAYER_B_BUTTON) == true || 
					pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
				{
					CDebugProc::Print("c", " ���� ");

					if (pEnemy->GetDying() == true)
					{
						pEnemy->SetMove(D3DXVECTOR3(-OSI_MOVE * 5, 3.0f, 0.0f));
					}
					else
					{
						pEnemy->SetMove(D3DXVECTOR3(-OSI_MOVE, 3.0f, 0.0f));
					}
					m_nCntFlame = OSI_FLAME;
					pGauge->SetGaugeRightLeft(HEEL, -DAMAGE);
					m_bAttack = true;
					//�d��
					Recovery();
				}
			}
			else if (pInputKeyboard->GetPress(PLAYER_RIGHT) == true || 
				pXInput->GetPress(XPLAYER_RIGHT, 0) == true)
			{
				if (pInputKeyboard->GetPress(PLAYER_A_BUTTON) == true || 
					pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					CDebugProc::Print("c", " ������ ");
					if (pEnemy->GetDying() == true)
					{
						pEnemy->SetMove(D3DXVECTOR3(NAGE_MOVE * 2, 3.0f, 0.0f));
					}
					else
					{
						pEnemy->SetMove(D3DXVECTOR3(NAGE_MOVE, 3.0f, 0.0f));
					}
					m_bAttack = true;
					m_nCntFlame = NAGE_FLAME;
					pGauge->SetGaugeRightLeft(HEEL, -DAMAGE);
					//�d��
					Recovery();
				}
			}
			break;
		case CPlayer::DIRECTION_RIGHT:
			if (pInputKeyboard->GetPress(PLAYER_RIGHT) == true || 
				pXInput->GetPress(XPLAYER_RIGHT, 0) == true)
			{
				if (pInputKeyboard->GetPress(PLAYER_A_BUTTON) == true || 
					pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					CDebugProc::Print("c", " ��� ");
					pPlayer->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					pGauge->SetGaugeRightLeft(HEEL, -DAMAGE);
					m_nCntFlame = YORI_FLAME;
					m_bAttack = true;
				}
				else if (pInputKeyboard->GetPress(PLAYER_B_BUTTON) == true || 
					pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
				{
					CDebugProc::Print("c", " ���� ");
					if (pEnemy->GetDying() == true)
					{
						pEnemy->SetMove(D3DXVECTOR3(OSI_MOVE * 5, 3.0f, 0.0f));
					}
					else
					{
						pEnemy->SetMove(D3DXVECTOR3(OSI_MOVE, 3.0f, 0.0f));
					}
					pGauge->SetGaugeRightLeft(HEEL, -DAMAGE);
					m_nCntFlame = OSI_FLAME;
					m_bAttack = true;
					//�d��
					Recovery();
				}
			}
			else if (pInputKeyboard->GetPress(PLAYER_LEFT) == true || 
				pXInput->GetPress(XPLAYER_LEFT, 0) == true)
			{
				if (pInputKeyboard->GetPress(PLAYER_A_BUTTON) == true || 
					pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					CDebugProc::Print("c", " ������ ");
					if (pEnemy->GetDying() == true)
					{
						pEnemy->SetMove(D3DXVECTOR3(-NAGE_MOVE * 2, 3.0f, 0.0f));
					}
					else
					{
						pEnemy->SetMove(D3DXVECTOR3(-NAGE_MOVE, 3.0f, 0.0f));
					}
					pGauge->SetGaugeRightLeft(HEEL, -DAMAGE);
					m_bAttack = true;
					m_nCntFlame = NAGE_FLAME;
					//�d��
					Recovery();
				}
			}
			break;
		}

		//�����Ă���� �G�l�~�[
		switch (pEnemy->GetDirection())
		{
		case CEnemy::DIRECTION_LEFT:
			if (pInputKeyboard->GetPress(ENEMY_LEFT) == true || 
				pXInput->GetPress(XENEMY_LEFT, 1) == true)
			{
				if (pInputKeyboard->GetPress(ENEMY_A_BUTTON) == true || 
					pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
				{
					CDebugProc::Print("c", " ��� ");
					pPlayer->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
					m_bAttack = true;
					m_nCntFlame = YORI_FLAME;
				}
				else if (pInputKeyboard->GetPress(ENEMY_B_BUTTON) == true || 
					pXInput->GetTrigger(XENEMY_B_BUTTON, 1) == true)
				{
					CDebugProc::Print("c", " ���� ");

					if (pPlayer->GetDying() == true)
					{
						pPlayer->SetMove(D3DXVECTOR3(-OSI_MOVE * 5, 3.0f, 0.0f));
					}
					else
					{
						pPlayer->SetMove(D3DXVECTOR3(-OSI_MOVE, 3.0f, 0.0f));
					}
					pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
					m_nCntFlame = OSI_FLAME;
					m_bAttack = true;
					//�d��
					Recovery();
				}
			}
			else if (pInputKeyboard->GetPress(ENEMY_RIGHT) == true || 
				pXInput->GetPress(XENEMY_RIGHT, 1) == true)
			{
				if (pInputKeyboard->GetPress(ENEMY_A_BUTTON) == true ||
					pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
				{
					CDebugProc::Print("c", " ������ ");
					if (pPlayer->GetDying() == true)
					{
						pPlayer->SetMove(D3DXVECTOR3(NAGE_MOVE * 2, 3.0f, 0.0f));
					}
					else
					{
						pPlayer->SetMove(D3DXVECTOR3(NAGE_MOVE, 3.0f, 0.0f));
					}
					m_bAttack = true;
					pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
					m_nCntFlame = NAGE_FLAME;
					//�d��
					Recovery();
				}
			}

			break;
		case CEnemy::DIRECTION_RIGHT:
			if (pInputKeyboard->GetPress(ENEMY_RIGHT) == true || 
				pXInput->GetPress(XENEMY_RIGHT, 1) == true)
			{
				if (pInputKeyboard->GetPress(ENEMY_A_BUTTON) == true || 
					pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
				{
					CDebugProc::Print("c", " ��� ");
					pPlayer->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					m_bAttack = true;
					pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
					m_nCntFlame = YORI_FLAME;
				}
				else if (pInputKeyboard->GetPress(ENEMY_A_BUTTON) == true || 
					pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
				{
					CDebugProc::Print("c", " ���� ");
					if (pPlayer->GetDying() == true)
					{
						pPlayer->SetMove(D3DXVECTOR3(OSI_MOVE * 5, 3.0f, 0.0f));
					}
					else
					{
						pPlayer->SetMove(D3DXVECTOR3(OSI_MOVE, 3.0f, 0.0f));
					}					pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
					m_nCntFlame = OSI_FLAME;
					m_bAttack = true;
					//�d��
					Recovery();
				}
			}
			else if (pInputKeyboard->GetPress(ENEMY_LEFT) == true || 
				pXInput->GetPress(XENEMY_LEFT, 1) == true)
			{
				if (pInputKeyboard->GetPress(ENEMY_A_BUTTON) == true || 
					pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
				{
					CDebugProc::Print("c", " ������ ");
					if (pPlayer->GetDying() == true)
					{
						pPlayer->SetMove(D3DXVECTOR3(-NAGE_MOVE * 2, 3.0f, 0.0f));
					}
					else
					{
						pPlayer->SetMove(D3DXVECTOR3(-NAGE_MOVE, 3.0f, 0.0f));
					}					m_bAttack = true;
					pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
					m_nCntFlame = NAGE_FLAME;
					//�d��
					Recovery();
				}
			}
			break;
		}
	}
	//���Z�b�g
	if (pXInput->GetTrigger(XINPUT_GAMEPAD_START, 0) == true)
	{
		pPlayer->SetPosition(D3DXVECTOR3(-20.0f, 50.0f, 0.0f));
		pEnemy->SetPosition(D3DXVECTOR3(20.0f, 50.0f, 0.0f));
		pPlayer->SetState(CPlayer::STATE_NEUTRAL);
		pEnemy->SetState(CEnemy::STATE_NEUTRAL);
		m_bAttack = false;
		m_nCntFlame = 0;
		CGame::SetHit(false);
	}

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

//=============================================================================
// �o�g�������܂Ƃ�
//=============================================================================
void CBattleSys::Battle(int nPlayer, ATTACK_TYPE AttackType)
{

}
