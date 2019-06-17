//=============================================================================
//
// �o�g���V�X�e������ [BattleSystem.cpp]
// Author : �ڍ� ������ & ���R ��� & �R�� �֎j
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
#include "SansoGauge.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �}�N��
//=============================================================================
#define YORI_MOVE		(5.0f)
#define NAGE_MOVE		(35.5f)
#define OSI_MOVE		(8.0f)
#define COUNTER_MOVE	(20.0f)
#define TUPARI_MOVE		(8.6f)
#define KNOCKUP_MOVE	(3.0f)
#define NAGE_DIVIDED	(4.0f)
#define NAGE_HINSI		(2.0f)
#define OSI_RECOIL		(2.0f)
#define TUPPARI_RECOIL	(2.0f)
#define HINSI_MOVE		(2.0f)

#define BATTLE_FLAME		(20)

#define YORI_FLAME			(20)
#define NAGE_FLAME			(30)
#define OSI_FLAME			(20)
#define TUPARI_FLAME		(2)
#define TUPARI_RECOVERY		(20)
#define COUNTER_FLAME		(40)

#define GU_COUNTER			(40)
#define CHOKI_COUNTER		(10)

#define DAMAGE				(30)
#define HEEL				(20)
#define START_SECOND		(2)			// �ΐ킪�n�܂�܂ł̕b��
#define DOHYO_HAZI_NUM		(173.0f)

#define SANSO_NEUTRAL		(1.5f)		//�g��ł��Ȃ��Ƃ��̉񕜗�
#define SANSO_KUMI			(0.3f)		//�g��ł���Ƃ��̉񕜗�
#define SANSO_SIKAKE		(20)		//�������Ƃ��̏���ʂ̔{��

//=============================================================================
// �o�g���V�X�e���N���X�̃R���X�g���N�^
//=============================================================================
CBattleSys::CBattleSys()
{
	// �l���N���A
	m_nCntAttackFlame = 0;
	m_nImpossibleFlame = 0;
	m_bAttack = false;
	m_nStartCounter = 0;
	m_nCntPushP1 = 0;
	m_nCntPushP2 = 0;
	m_nFlamePush = 0;
	m_AttackTurn = ATTACK_TURN_NORMAL;

	for (int nCntPlayer = 0; nCntPlayer < MAX_CHARACTER; nCntPlayer++)
	{
		m_aGUCounter[nCntPlayer] = 0;
		m_aCHOKICounter[nCntPlayer] = 0;
		m_abPA[nCntPlayer] = false;
		m_fMoveDying[nCntPlayer] = { 0.0f };	//�m�����̈ړ���
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
	m_nCntAttackFlame = 0;
	m_nImpossibleFlame = 0;
	m_bAttack = false;
	m_aJanken[0] = JANKEN_GU;
	m_aJanken[1] = JANKEN_GU;
	m_nStartCounter = 0;
	m_nCntPushP1 = 0;
	m_nCntPushP2 = 0;
	m_nFlamePush = 0;
	m_AttackTurn = ATTACK_TURN_NORMAL;

	for (int nCntPlayer = 0; nCntPlayer < MAX_CHARACTER; nCntPlayer++)
	{
		m_aGUCounter[nCntPlayer] = 0;
		m_aCHOKICounter[nCntPlayer] = 0;
		m_abPA[nCntPlayer] = false;
		m_fMoveDying[nCntPlayer] = { 1.0f };
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


	//�L�[�{�[�h/�R���g���[���[����
	Operation();
	//�J�E���^�[����
	CounterAttack();
	//�A�ŉ񐔔���
	PushJudge();

	if (m_bAttack == true)
	{
		m_nCntAttackFlame--;
		if (m_nCntAttackFlame <= 0)
		{
			if (pPlayer != NULL)
			{
				//��Ԃ�ʏ��Ԃ�
				if (pPlayer->GetState() == CPlayer::STATE_TSUPPARI || pPlayer->GetState() == CPlayer::STATE_NAGE)
				{
					pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				}
			}

			if (pEnemy != NULL)
			{
				if (pEnemy->GetState() == CEnemy::STATE_TSUPPARI || pEnemy->GetState() == CPlayer::STATE_NAGE)
				{
					pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				}
			}
			m_nCntAttackFlame = 0;
			m_bAttack = false;
		}
	}

#ifdef _DEBUG
	if (pPlayer != NULL && pEnemy != NULL)
	{
		if (pPlayer->GetState() == CPlayer::STATE_NEUTRAL
			&& pEnemy->GetState() == CEnemy::STATE_NEUTRAL
			&& m_bAttack == false)
		{
			CDebugProc::Print("c", " �ʏ� ");
		}
	}
	CDebugProc::Print("cn", " �s���s�t���[�� ", m_nCntAttackFlame);

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
	// �Q�[�W�̎擾
	CGauge *pGauge;
	pGauge = CGame::GetGauge();

	//�T�E���h���̎擾
	CSound *pSound = CManager::GetSound(0);

	D3DXVECTOR3 p1pos, p2pos;

	if (pPlayer != NULL)
	{	//�v���C���[1�̈ʒu���擾
		p1pos = pPlayer->GetPosition();
	}
	if (pPlayer != NULL)
	{	//�v���C���[2�̈ʒu���擾
		p2pos = pEnemy->GetPosition();
	}

	if (pPlayer != NULL && pEnemy != NULL)
	{
		if (CGame::GetState() == CGame::STATE_GAME)
		{

			if (pPlayer->GetState() == CPlayer::STATE_JANKEN)
			{
#ifdef _DEBUG
				CDebugProc::Print("c", " e����񂯂� ");
#endif
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
				else if (pInputKeyboard->GetPress(DIK_C) == true ||
					pXInput->GetPress(XPLAYER_X_BUTTON, 0) == true)
				{
					m_aJanken[0] = JANKEN_PA;
					pPlayer->SetState(CPlayer::STATE_NOKOTTA);
					m_abPA[0] = true;
				}
			}

			if (pEnemy->GetState() == CEnemy::STATE_JANKEN)
			{
#ifdef _DEBUG
				CDebugProc::Print("c", " e����񂯂� ");
#endif
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
		}

		if (pPlayer->GetState() == CPlayer::STATE_NOKOTTA && pEnemy->GetState() == CEnemy::STATE_NOKOTTA)
		{// 2�l�̂���񂯂񂪌��܂�����
			int nTime = (int)(m_nStartCounter / 60);

			if (nTime < START_SECOND)
			{
				m_nStartCounter++;
			}

			if (nTime >= START_SECOND)
			{
				if (m_aJanken[0] == JANKEN_GU && m_aJanken[1] == JANKEN_GU)
				{// �O�[�ƃO�[
					if (CGame::GetHit() == false)
					{
						pPlayer->SetMove(D3DXVECTOR3(3.0f, 0.0f, 0.0f));
						pEnemy->SetMove(D3DXVECTOR3(-3.0f, 0.0f, 0.0f));
					}
				}
				else if (m_aJanken[0] == JANKEN_GU && m_aJanken[1] == JANKEN_CHOKI)
				{// �O�[�ƃ`���L
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

					if (CGame::GetHit() == true)
					{
						pEnemy->SetMove(D3DXVECTOR3(OSI_MOVE * 3, 5.0f, 0.0f));
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						CGame::SetHit(false);
					}
				}
				else if (m_aJanken[0] == JANKEN_GU && m_aJanken[1] == JANKEN_PA)
				{// �O�[�ƃp�[
					m_aGUCounter[0]++;

					if (m_aGUCounter[0] < GU_COUNTER)
					{
						pPlayer->SetMove(D3DXVECTOR3(3.0f, 0.0f, 0.0f));
					}
					else if (m_aGUCounter[0] >= GU_COUNTER)
					{
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						m_aGUCounter[0] = 0;
					}

					if (m_abPA[1] == true)
					{
						pEnemy->SetMove(D3DXVECTOR3(0.0, 10.0f, 0.0f));
						m_abPA[1] = false;
					}
				}
				else if (m_aJanken[0] == JANKEN_CHOKI && m_aJanken[1] == JANKEN_GU)
				{// �`���L�ƃO�[
					if (CGame::GetHit() == true)
					{
						pPlayer->SetMove(D3DXVECTOR3(-OSI_MOVE * 3, 5.0f, 0.0f));
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						CGame::SetHit(false);
					}

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
				else if (m_aJanken[0] == JANKEN_CHOKI && m_aJanken[1] == JANKEN_CHOKI)
				{// �`���L�ƃ`���L
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
				else if (m_aJanken[0] == JANKEN_CHOKI && m_aJanken[1] == JANKEN_PA)
				{// �`���L�ƃp�[
					m_aCHOKICounter[0]++;

					if (m_aCHOKICounter[0] > CHOKI_COUNTER && CHOKI_COUNTER + 5 >= m_aCHOKICounter[0])
					{
						pEnemy->SetMove(D3DXVECTOR3(OSI_MOVE, 3.0f, 0.0f));
					}
					else if (m_aCHOKICounter[0] > CHOKI_COUNTER + 5)
					{
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						m_aCHOKICounter[0] = 0;
					}

					if (m_abPA[1] == true)
					{
						pEnemy->SetMove(D3DXVECTOR3(0.0, 10.0f, 0.0f));
						m_abPA[1] = false;
					}
				}
				else if (m_aJanken[0] == JANKEN_PA && m_aJanken[1] == JANKEN_GU)
				{// �p�[�ƃO�[
					if (m_abPA[0] == true)
					{
						pPlayer->SetMove(D3DXVECTOR3(0.0, 10.0f, 0.0f));
						m_abPA[0] = false;
					}

					m_aGUCounter[1]++;

					if (m_aGUCounter[1] < GU_COUNTER)
					{
						pEnemy->SetMove(D3DXVECTOR3(-3.0f, 0.0f, 0.0f));
					}
					else if (m_aGUCounter[1] >= GU_COUNTER)
					{
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						m_aGUCounter[1] = 0;
					}
				}
				else if (m_aJanken[0] == JANKEN_PA && m_aJanken[1] == JANKEN_CHOKI)
				{// �p�[�ƃ`���L
					if (m_abPA[0] == true)
					{
						pPlayer->SetMove(D3DXVECTOR3(0.0, 10.0f, 0.0f));
						m_abPA[0] = false;
					}

					m_aCHOKICounter[1]++;

					if (m_aCHOKICounter[1] > CHOKI_COUNTER && CHOKI_COUNTER + 5 >= m_aCHOKICounter[1])
					{
						pPlayer->SetMove(D3DXVECTOR3(-OSI_MOVE, 3.0f, 0.0f));
					}
					else if (m_aCHOKICounter[1] > CHOKI_COUNTER + 5)
					{
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						m_aCHOKICounter[1] = 0;
					}
				}
				else if (m_aJanken[0] == JANKEN_PA && m_aJanken[1] == JANKEN_PA)
				{// �p�[�ƃp�[
					if (m_abPA[0] == true)
					{
						pPlayer->SetMove(D3DXVECTOR3(0.0, 10.0f, 0.0f));
						m_abPA[0] = false;
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
					}

					if (m_abPA[1] == true)
					{
						pEnemy->SetMove(D3DXVECTOR3(0.0, 10.0f, 0.0f));
						m_abPA[1] = false;
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
					}
				}
			}
		}


		//�m����
		if (pPlayer->GetDying() == true)
		{
			m_fMoveDying[0] = HINSI_MOVE;
		}
		else if (pPlayer->GetDying() == false)
		{
			m_fMoveDying[0] = 1.0f;
		}

		if (pEnemy->GetDying() == true)
		{
			m_fMoveDying[1] = HINSI_MOVE;
		}
		else if (pEnemy->GetDying() == false)
		{
			m_fMoveDying[1] = 1.0f;
		}

		//2p�̕�������(��������ɓ���) �C��
		if (pPlayer->GetState() == CPlayer::STATE_KUMI
			&& pEnemy->GetState() == CEnemy::STATE_KUMI
			&& m_bAttack == false)
		{
			CDebugProc::Print("c", " �g�ݍ��� ");

			if (m_AttackTurn == ATTACK_TURN_PLAYER1)
			{
				P1Attack();
				//m_AttackTurn = ATTACK_TURN_NORMAL;
			}
			else if (m_AttackTurn == ATTACK_TURN_PLAYER2)
			{
				P2Attack();
				//m_AttackTurn = ATTACK_TURN_NORMAL;
			}
		}

		//���ς�
		else if (pPlayer->GetState() == CPlayer::STATE_NEUTRAL
			&& pEnemy->GetState() == CEnemy::STATE_NEUTRAL
			&& m_bAttack == false)
		{
			if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true && pPlayer->GetRecovery() == false ||
				pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true && pPlayer->GetRecovery() == false)
			{
				//�����Ă���� �v���C���[
				switch (pPlayer->GetDirection())
				{
				case CPlayer::DIRECTION_LEFT:
					pPlayer->SetState(CPlayer::STATE_TSUPPARI);
					pPlayer->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x - 10, p1pos.y, p1pos.z));
					m_nCntAttackFlame = TUPARI_FLAME;
					pPlayer->SetRecovery(true);
					pPlayer->SetRecoveryTime(TUPARI_RECOVERY);
					m_bAttack = true;
					break;
				case CPlayer::DIRECTION_RIGHT:
					pPlayer->SetState(CPlayer::STATE_TSUPPARI);
					pPlayer->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x + 10, p1pos.y, p1pos.z));
					m_nCntAttackFlame = TUPARI_FLAME;
					pPlayer->SetRecovery(true);
					pPlayer->SetRecoveryTime(TUPARI_RECOVERY);
					m_bAttack = true;
					break;
				}
			}
			if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true && pEnemy->GetRecovery() == false ||
				pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true && pEnemy->GetRecovery() == false)
			{
				//�����Ă���� �G�l�~�[
				switch (pEnemy->GetDirection())
				{
				case CEnemy::DIRECTION_LEFT:
					pEnemy->SetState(CEnemy::STATE_TSUPPARI);
					pEnemy->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x - 10, p1pos.y, p1pos.z));
					m_nCntAttackFlame = TUPARI_FLAME;
					pEnemy->SetRecovery(true);
					pEnemy->SetRecoveryTime(TUPARI_RECOVERY);
					m_bAttack = true;
					break;
				case CEnemy::DIRECTION_RIGHT:
					pEnemy->SetState(CEnemy::STATE_TSUPPARI);
					pEnemy->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x + 10, p1pos.y, p1pos.z));
					m_nCntAttackFlame = TUPARI_FLAME;
					pEnemy->SetRecovery(true);
					pEnemy->SetRecoveryTime(TUPARI_RECOVERY);
					m_bAttack = true;
					break;
				}
			}
		}


		//�_���[�W�Ȃ琁����� �v���C���[�̂��ς�
		if (pEnemy->GetState() == CEnemy::STATE_DAMAGE)
		{
			switch (pPlayer->GetDirection())
			{
			case CPlayer::DIRECTION_LEFT:
				pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
				Battle(0, ATTACK_TYPE_TUPPARI, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3((-TUPARI_MOVE * m_fMoveDying[1]), KNOCKUP_MOVE, 0.0f));
				break;
			case CPlayer::DIRECTION_RIGHT:
				pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
				Battle(0, ATTACK_TYPE_TUPPARI, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3((TUPARI_MOVE * m_fMoveDying[1]), KNOCKUP_MOVE, 0.0f));
				break;
			}
			//��ԕω�
			pEnemy->SetState(CEnemy::STATE_NEUTRAL);
			//�_���[�W
			pGauge->SetGaugeRightLeft(HEEL, -DAMAGE);
		}

		//�_���[�W�Ȃ琁����� �G�l�~�[�̂��ς�
		if (pPlayer->GetState() == CPlayer::STATE_DAMAGE)
		{
			switch (pEnemy->GetDirection())
			{
			case CEnemy::DIRECTION_LEFT:
				pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
				Battle(1, ATTACK_TYPE_TUPPARI, D3DXVECTOR3((-TUPARI_MOVE * m_fMoveDying[0]), KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				break;
			case CEnemy::DIRECTION_RIGHT:
				pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
				Battle(1, ATTACK_TYPE_TUPPARI, D3DXVECTOR3((TUPARI_MOVE * m_fMoveDying[0]), KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				break;
			}
			//��ԕω�
			if (pPlayer != NULL)
			{
				pPlayer->SetState(CPlayer::STATE_NEUTRAL);
			}
			//�_���[�W
			pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
		}


		if (pPlayer != NULL)
		{	//�v���C���[1�̈ʒu���擾
			p1pos = pPlayer->GetPosition();
		}
		if (pEnemy != NULL)
		{	//�v���C���[2�̈ʒu���擾
			p2pos = pEnemy->GetPosition();
		}

		if (pPlayer != NULL)
		{
			//�y�U�[�ő̗͂�����Ȃ�c��
			if (/*pPlayer->GetDohyo() == CPlayer::DOHYO_HAZI && */pPlayer->GetDying() == false)
			{
				if (pPlayer->GetPosition().x > DOHYO_HAZI_NUM)
				{
					pPlayer->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					pPlayer->SetPosition(D3DXVECTOR3(DOHYO_HAZI_NUM, p1pos.y, p1pos.z));
				}
				else if (pPlayer->GetPosition().x < -DOHYO_HAZI_NUM)
				{
					pPlayer->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					pPlayer->SetPosition(D3DXVECTOR3(-DOHYO_HAZI_NUM, p1pos.y, p1pos.z));
				}
			}
		}

		if (pEnemy != NULL)
		{
			if (/*pEnemy->GetDohyo() == CEnemy::DOHYO_HAZI && */pEnemy->GetDying() == false)
			{
				if (pEnemy->GetPosition().x > DOHYO_HAZI_NUM)
				{
					pEnemy->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					pEnemy->SetPosition(D3DXVECTOR3(DOHYO_HAZI_NUM, p2pos.y, p2pos.z));
				}
				else if (pEnemy->GetPosition().x < -DOHYO_HAZI_NUM)
				{
					pEnemy->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					pEnemy->SetPosition(D3DXVECTOR3(-DOHYO_HAZI_NUM, p2pos.y, p2pos.z));
				}
			}
		}
		//���Z�b�g
		if (pInputKeyboard->GetTrigger(DIK_R) == true ||
			pXInput->GetTrigger(XINPUT_GAMEPAD_START, 0) == true)
		{
			pPlayer->SetPosition(D3DXVECTOR3(-20.0f, 50.0f, 0.0f));
			pEnemy->SetPosition(D3DXVECTOR3(20.0f, 50.0f, 0.0f));
			pPlayer->SetState(CPlayer::STATE_NEUTRAL);
			pEnemy->SetState(CEnemy::STATE_NEUTRAL);
			m_bAttack = false;
			m_nCntAttackFlame = 0;
			pGauge->SetGaugeRightLeft(600, 600);
			CGame::SetWinner(CGame::WINNER_NONE);
			CGame::SetHit(false);
		}
	}
#ifdef _DEBUG
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
	if (pPlayer != NULL)
	{
		pPlayer->SetState(CPlayer::STATE_NEUTRAL);
		pPlayer->SetRecovery(true);
		pPlayer->SetRecoveryTime(20);
	}
	if (pEnemy != NULL)
	{
		pEnemy->SetState(CEnemy::STATE_NEUTRAL);
		pEnemy->SetRecovery(true);
		pEnemy->SetRecoveryTime(20);
	}
}

//=============================================================================
// �o�g�������܂Ƃ�
//=============================================================================
void CBattleSys::Battle(int nPlayer, ATTACK_TYPE AttackType, D3DXVECTOR3 P1move, D3DXVECTOR3 P2move)
{
	// �v���C���[�̎擾
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	// �G�l�~�[�̎擾
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

	CGauge *pGauge;
	pGauge = CGame::GetGauge();

	//�v���C���[1�̈ʒu���擾
	D3DXVECTOR3 p1pos;
	if (pPlayer != NULL)
	{
		p1pos = pPlayer->GetPosition();
	}
	//�v���C���[2�̈ʒu���擾
	D3DXVECTOR3 p2pos;
	if (pPlayer != NULL)
	{
		p2pos = pEnemy->GetPosition();
	}
	//�_���[�W����
	if (nPlayer == 0)
	{
		pGauge->SetGaugeRightLeft(HEEL, -DAMAGE);
	}
	else
	{
		pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
	}

	//�U���̎��
	switch (AttackType)
	{
	case ATTACK_TYPE_YORI:
		if (pEnemy->GetDohyo() == CEnemy::DOHYO_HAZI && pEnemy->GetDying() == false
			|| pPlayer->GetDohyo() == CPlayer::DOHYO_HAZI && pPlayer->GetDying() == false)
		{//�̗͂�����ꍇ�œy�U�[�̊��U��
		 //�����Ă���� �v���C���[
			switch (pPlayer->GetDirection())
			{
			case CPlayer::DIRECTION_LEFT:
				if (pPlayer->GetDohyoHaziLR() == CPlayer::HAZI_RIGHT && pPlayer->GetDying() == false && nPlayer == 1)
				{
					pPlayer->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
				}
				else if (pEnemy->GetDohyoHaziLR() == CEnemy::HAZI_LEFT && pEnemy->GetDying() == false && nPlayer == 0)
				{
					pPlayer->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
				}
				else
				{//�ʏ�
					pPlayer->SetMove(P1move);
					pEnemy->SetMove(P2move);
				}
				break;
			case CPlayer::DIRECTION_RIGHT:
				if (pPlayer->GetDohyoHaziLR() == CPlayer::HAZI_LEFT && pPlayer->GetDying() == false && nPlayer == 1)
				{
					pPlayer->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
				}
				else if (pEnemy->GetDohyoHaziLR() == CEnemy::HAZI_RIGHT && pEnemy->GetDying() == false && nPlayer == 0)
				{
					pPlayer->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
				}
				else
				{//�ʏ�
					pPlayer->SetMove(P1move);
					pEnemy->SetMove(P2move);
				}
				break;
			}
		}
		else
		{
			pPlayer->SetMove(P1move);
			pEnemy->SetMove(P2move);
		}

		//�t���[���ݒ�
		m_nCntAttackFlame = YORI_FLAME;
		break;
	case ATTACK_TYPE_OSI:
		if (pEnemy->GetDohyo() == CEnemy::DOHYO_HAZI && pEnemy->GetDying() == false
			|| pPlayer->GetDohyo() == CPlayer::DOHYO_HAZI && pPlayer->GetDying() == false)
		{//�̗͂�����ꍇ�œy�U�[�̉����U��
			//�����Ă���� �v���C���[
			switch (pPlayer->GetDirection())
			{
			case CPlayer::DIRECTION_LEFT:
				if (pPlayer->GetDohyoHaziLR() == CPlayer::HAZI_RIGHT && pPlayer->GetDying() == false && nPlayer == 1)
				{
					pPlayer->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(-P1move.x / OSI_RECOIL, 0.0f, 0.0f));
				}
				else if (pEnemy->GetDohyoHaziLR() == CEnemy::HAZI_LEFT && pEnemy->GetDying() == false && nPlayer == 0)
				{
					pPlayer->SetMove(D3DXVECTOR3(-P2move.x / OSI_RECOIL, 0.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
				}
				else
				{//�ʏ�
					pPlayer->SetMove(P1move);
					pEnemy->SetMove(P2move);
				}
				break;
			case CPlayer::DIRECTION_RIGHT:
				if (pPlayer->GetDohyoHaziLR() == CPlayer::HAZI_LEFT && pPlayer->GetDying() == false && nPlayer == 1)
				{
					pPlayer->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(-P1move.x / OSI_RECOIL, 0.0f, 0.0f));
				}
				else if (pEnemy->GetDohyoHaziLR() == CEnemy::HAZI_RIGHT && pEnemy->GetDying() == false && nPlayer == 0)
				{
					pPlayer->SetMove(D3DXVECTOR3(-P2move.x / OSI_RECOIL, 0.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
				}
				else
				{//�ʏ�
					pPlayer->SetMove(P1move);
					pEnemy->SetMove(P2move);
				}
				break;
			}
		}
		else
		{
			pPlayer->SetMove(P1move);
			pEnemy->SetMove(P2move);
		}
		//�d��
		Recovery();
		//�t���[���ݒ�
		m_nCntAttackFlame = OSI_FLAME;
		break;
	case ATTACK_TYPE_NAGE:

		if (pEnemy->GetDohyo() == CEnemy::DOHYO_HAZI && pEnemy->GetDying() == false
			|| pPlayer->GetDohyo() == CPlayer::DOHYO_HAZI && pPlayer->GetDying() == false)
		{//�̗͂�����ꍇ�œy�U�[�̓����U��
			if (pPlayer->GetDohyo() == CPlayer::DOHYO_HAZI && nPlayer == 0)
			{//�v���C���[(����)���[�œ������̍U��
				pPlayer->SetMove(D3DXVECTOR3(-P2move.x / NAGE_DIVIDED, 0.0f, 0.0f));
				pEnemy->SetMove(D3DXVECTOR3(P2move.x / NAGE_DIVIDED, KNOCKUP_MOVE, 0.0f));
			}
			else if (pEnemy->GetDohyo() == CEnemy::DOHYO_HAZI && nPlayer == 1)
			{//�G�l�~�[(����)���[�œ������̍U��
				pPlayer->SetMove(D3DXVECTOR3(P1move.x / NAGE_DIVIDED, KNOCKUP_MOVE, 0.0f));
				pEnemy->SetMove(D3DXVECTOR3(-P1move.x / NAGE_DIVIDED, 0.0f, 0.0f));
			}
			else
			{//�������鑤���[�œ������̍U��
				pPlayer->SetMove(P1move);
				pEnemy->SetMove(P2move);
			}
		}
		else
		{//�y�U�[�ȊO�̓����U��
			pPlayer->SetMove(P1move);
			pEnemy->SetMove(P2move);
		}
		//�t���[���ݒ�
		m_nCntAttackFlame = NAGE_FLAME;
		//�d��
		Recovery();
		CGame::SetHit(false);
		pEnemy->SetState(CEnemy::STATE_NAGE);
		pPlayer->SetState(CPlayer::STATE_NAGE);

		break;
	case ATTACK_TYPE_TUPPARI:
		if (pEnemy->GetDohyo() == CEnemy::DOHYO_HAZI && pEnemy->GetDying() == false
			|| pPlayer->GetDohyo() == CPlayer::DOHYO_HAZI && pPlayer->GetDying() == false)
		{//�̗͂�����ꍇ�œy�U�[�̂��ς�U��
			if (nPlayer == 0)
			{
				pPlayer->SetMove(D3DXVECTOR3(-P2move.x / TUPPARI_RECOIL, 0.0f, 0.0f));
				pEnemy->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
			}
			else
			{
				pPlayer->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
				pEnemy->SetMove(D3DXVECTOR3(-P1move.x / TUPPARI_RECOIL, 0.0f, 0.0f));
			}
		}
		else
		{
			pPlayer->SetMove(P1move);
			pEnemy->SetMove(P2move);
		}

		break;
	case ATTACK_TYPE_COUNTER:

		if (pEnemy->GetDohyo() == CEnemy::DOHYO_HAZI && pEnemy->GetDying() == false
			|| pPlayer->GetDohyo() == CPlayer::DOHYO_HAZI && pPlayer->GetDying() == false)
		{//�̗͂�����ꍇ�œy�U�[�̃J�E���^�[�U��
			if (pPlayer->GetDohyo() == CPlayer::DOHYO_HAZI && nPlayer == 0)
			{//�v���C���[���[�œ������̍U��
				pPlayer->SetMove(D3DXVECTOR3(-P2move.x, 0.0f, 0.0f));
				pEnemy->SetMove(D3DXVECTOR3(P2move.x, 0.0f, 0.0f));
			}
			else if (pEnemy->GetDohyo() == CEnemy::DOHYO_HAZI && nPlayer == 1)
			{//�G�l�~�[���[�œ������̍U��
				pPlayer->SetMove(D3DXVECTOR3(P1move.x, 0.0f, 0.0f));
				pEnemy->SetMove(D3DXVECTOR3(-P1move.x, 0.0f, 0.0f));
			}
			else
			{//�������鑤���[�œ������̍U��
				pPlayer->SetMove(D3DXVECTOR3(P1move.x, P1move.y, P1move.z));
				pEnemy->SetMove(D3DXVECTOR3(P2move.x, P2move.y, P2move.z));
			}
		}
		else
		{
			pPlayer->SetMove(P1move);
			pEnemy->SetMove(P2move);
		}

		//�ǉ���
		if (nPlayer == 0)
		{
			pGauge->SetGaugeRightLeft(HEEL, 0);
		}
		else
		{
			pGauge->SetGaugeRightLeft(0, HEEL);
		}

		break;
	}
	m_bAttack = true;
	m_AttackTurn = ATTACK_TURN_NORMAL;
}


//=============================================================================
// �A�ŉ񐔔��菈��
//=============================================================================
void CBattleSys::PushJudge(void)
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
	// �Q�[�W�̎擾
	CSansoGauge *pGauge;
	pGauge = CGame::GetSansoGauge();

	float fPushCntP1 = 0, fPushCntP2 = 0;


	if (pPlayer != NULL && pEnemy != NULL)
	{
		//�A�Ŕ���
		if (pPlayer->GetState() == CPlayer::STATE_KUMI
			&& pEnemy->GetState() == CEnemy::STATE_KUMI)
		{
			if (pPlayer->GetRecovery() == false)
			{
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					fPushCntP1++;
					m_nCntPushP1++;
				}
				else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
				{
					fPushCntP1++;
					m_nCntPushP1++;
				}
				else
				{
					//�_�f��
					pGauge->SetSansoGaugeRightLeft(SANSO_KUMI, 0);
				}
			}
			else
			{
				//�_�f��
				pGauge->SetSansoGaugeRightLeft(SANSO_NEUTRAL, 0);
			}

			if (pEnemy->GetRecovery() == false)
			{
				if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true ||
					pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
				{
					fPushCntP2++;
					m_nCntPushP2++;
				}
				else if (pInputKeyboard->GetTrigger(ENEMY_B_BUTTON) == true ||
					pXInput->GetTrigger(XENEMY_B_BUTTON, 1) == true)
				{
					fPushCntP2++;
					m_nCntPushP2++;
				}
				else
				{
					//�_�f��
					pGauge->SetSansoGaugeRightLeft(0, SANSO_KUMI);
				}
			}
			else
			{
				//�_�f��
				pGauge->SetSansoGaugeRightLeft(0, SANSO_NEUTRAL);
			}

			m_nFlamePush++;
			//�_�f����
			fPushCntP1 *= SANSO_SIKAKE;
			fPushCntP2 *= SANSO_SIKAKE;
			pGauge->SetSansoGaugeRightLeft(-fPushCntP1, -fPushCntP2);
		}
		else
		{
			m_nFlamePush = 0;
			m_nCntPushP1 = 0;
			m_nCntPushP2 = 0;
			//�_�f��
			pGauge->SetSansoGaugeRightLeft(SANSO_NEUTRAL, SANSO_NEUTRAL);
		}
	}

	if (m_nFlamePush > BATTLE_FLAME)
	{
		//m_nCntPushP1 *= 7;
		//m_nCntPushP2 *= 7;
		//�_�f����
		//pGauge->SetSansoGaugeRightLeft(-(float)m_nCntPushP1, -(float)m_nCntPushP2);

		//���s����
		if (m_nCntPushP1 > m_nCntPushP2)
		{
			m_AttackTurn = ATTACK_TURN_PLAYER1;
		}
		else if(m_nCntPushP1 < m_nCntPushP2)
		{
			m_AttackTurn = ATTACK_TURN_PLAYER2;
		}
		else
		{
			m_AttackTurn = ATTACK_TURN_NORMAL;
		}

		m_nFlamePush = 0;
		m_nCntPushP1 = 0;
		m_nCntPushP2 = 0;
	}

#ifdef _DEBUG
	CDebugProc::Print("cn", "�A�ŉ񐔃J�E���g�t���[�� : ", m_nFlamePush);
	CDebugProc::Print("cn", "�v���C���[�̘A�ŉ� : ", m_nCntPushP1);
	CDebugProc::Print("cn", "�G�l�~�[�̘A�ŉ� : ", m_nCntPushP2);
	if (m_AttackTurn == ATTACK_TURN_NORMAL)
	{
		CDebugProc::Print("c", "�ʏ폟�s����");
	}
	else if (m_AttackTurn == ATTACK_TURN_PLAYER1)
	{
		CDebugProc::Print("c", "�v���C���[1�̏���");
	}
	else if (m_AttackTurn == ATTACK_TURN_PLAYER2)
	{
		CDebugProc::Print("c", "�v���C���[2�̏���");
	}

#endif
}

//=============================================================================
// P1�U������
//=============================================================================
void CBattleSys::P1Attack(void)
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
	//�T�E���h���̎擾
	CSound *pSound = CManager::GetSound(0);

	if (pEnemy != NULL)
	{
		if (pEnemy->GetCounter() == false)
		{
			//�����Ă���� �v���C���[
			switch (pPlayer->GetDirection())
			{
			case CPlayer::DIRECTION_LEFT:
				if (pInputKeyboard->GetPress(PLAYER_LEFT) == true ||
					pXInput->GetPress(XPLAYER_LEFT, 0) == true)
				{
					if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
					{
						//���
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_YORI, D3DXVECTOR3(-YORI_MOVE, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(-YORI_MOVE, KNOCKUP_MOVE, 0.0f));

					}
					else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
					{	//����
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_OSI, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-OSI_MOVE * m_fMoveDying[1], KNOCKUP_MOVE, 0.0f));
					}
				}
				else if (pInputKeyboard->GetPress(PLAYER_RIGHT) == true ||
					pXInput->GetPress(XPLAYER_RIGHT, 0) == true)
				{
					if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
					{	//����
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_NAGE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3((NAGE_MOVE * m_fMoveDying[1]), KNOCKUP_MOVE, 0.0f));
					}
				}
				break;

			case CPlayer::DIRECTION_RIGHT:
				if (pInputKeyboard->GetPress(PLAYER_RIGHT) == true ||
					pXInput->GetPress(XPLAYER_RIGHT, 0) == true)
				{
					if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
					{	//���
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_YORI, D3DXVECTOR3(YORI_MOVE, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(YORI_MOVE, KNOCKUP_MOVE, 0.0f));
					}
					else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
					{	//����
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_OSI, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(OSI_MOVE * m_fMoveDying[1], KNOCKUP_MOVE, 0.0f));
					}
				}
				else if (pInputKeyboard->GetPress(PLAYER_LEFT) == true ||
					pXInput->GetPress(XPLAYER_LEFT, 0) == true)
				{
					if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
					{	//����
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_NAGE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3((-NAGE_MOVE * m_fMoveDying[1]), KNOCKUP_MOVE, 0.0f));
					}
				}
				break;
			}
		}
		else
		{//�J�E���^�[���Ƀ{�^����������

			switch (pPlayer->GetDirection())
			{
			case CPlayer::DIRECTION_LEFT:
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					Battle(1, ATTACK_TYPE_COUNTER, D3DXVECTOR3(COUNTER_MOVE, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				}
				else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
				{
					Battle(1, ATTACK_TYPE_COUNTER, D3DXVECTOR3(COUNTER_MOVE, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				}
				break;

			case CPlayer::DIRECTION_RIGHT:
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					Battle(1, ATTACK_TYPE_COUNTER, D3DXVECTOR3(-COUNTER_MOVE, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				}
				else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
				{
					Battle(1, ATTACK_TYPE_COUNTER, D3DXVECTOR3(-COUNTER_MOVE, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				}
				break;
			}
		}
	}
}

//=============================================================================
// P2�U������
//=============================================================================
void CBattleSys::P2Attack(void)
{
	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();
	// �G�l�~�[�̎擾
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();
	// �v���C���[�̎擾
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();

	//�T�E���h���̎擾
	CSound *pSound = CManager::GetSound(0);
	if (pPlayer != NULL)
	{
		if (pPlayer->GetCounter() == false)
		{
			//�����Ă���� �G�l�~�[
			switch (pEnemy->GetDirection())
			{
			case CEnemy::DIRECTION_LEFT:
				if (pInputKeyboard->GetPress(ENEMY_LEFT) == true ||
					pXInput->GetPress(XENEMY_LEFT, 1) == true)
				{
					if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
					{	//���
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_YORI, D3DXVECTOR3(-YORI_MOVE, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(-YORI_MOVE, KNOCKUP_MOVE, 0.0f));
					}
					else if (pInputKeyboard->GetTrigger(ENEMY_B_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_B_BUTTON, 1) == true)
					{	//����
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_OSI, D3DXVECTOR3(-OSI_MOVE * m_fMoveDying[0], KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					}
				}
				else if (pInputKeyboard->GetPress(ENEMY_RIGHT) == true ||
					pXInput->GetPress(XENEMY_RIGHT, 1) == true)
				{
					if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
					{	//����
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_NAGE, D3DXVECTOR3(NAGE_MOVE * m_fMoveDying[0], KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					}
				}
				break;
			case CEnemy::DIRECTION_RIGHT:
				if (pInputKeyboard->GetPress(ENEMY_RIGHT) == true ||
					pXInput->GetPress(XENEMY_RIGHT, 1) == true)
				{
					if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
					{	//���
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_YORI, D3DXVECTOR3(YORI_MOVE, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(YORI_MOVE, KNOCKUP_MOVE, 0.0f));
					}
					else if (pInputKeyboard->GetTrigger(ENEMY_B_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_B_BUTTON, 1) == true)
					{	//����
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_OSI, D3DXVECTOR3(OSI_MOVE * m_fMoveDying[0], KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					}
				}
				else if (pInputKeyboard->GetPress(ENEMY_LEFT) == true ||
					pXInput->GetPress(XENEMY_LEFT, 1) == true)
				{
					if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
					{	//����
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_NAGE, D3DXVECTOR3(-NAGE_MOVE * m_fMoveDying[0], KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					}
				}
				break;
			}
		}
		else
		{//�J�E���^�[���Ƀ{�^����������
			switch (pEnemy->GetDirection())
			{
			case CEnemy::DIRECTION_LEFT:
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					Battle(0, ATTACK_TYPE_COUNTER, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(COUNTER_MOVE, 0.0f, 0.0f));
				}
				else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
				{
					Battle(0, ATTACK_TYPE_COUNTER, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(COUNTER_MOVE, 0.0f, 0.0f));
				}
				break;

			case CEnemy::DIRECTION_RIGHT:
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					Battle(0, ATTACK_TYPE_COUNTER, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-COUNTER_MOVE, 0.0f, 0.0f));
				}
				else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
				{
					Battle(0, ATTACK_TYPE_COUNTER, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-COUNTER_MOVE, 0.0f, 0.0f));
				}
				break;
			}
		}
	}
}

//=============================================================================
// �J�E���^�[����
//=============================================================================
void CBattleSys::CounterAttack(void)
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
	if (pPlayer != NULL && pEnemy != NULL)
	{
		if (pPlayer->GetCounter() == false && pPlayer->GetRecovery() == false)
		{
			//�����Ă���� �v���C���[
			switch (pPlayer->GetDirection())
			{
			case CPlayer::DIRECTION_LEFT:
				if (pInputKeyboard->GetPress(PLAYER_RIGHT) == true ||
					pXInput->GetPress(XPLAYER_RIGHT, 0) == true)
				{
					if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
					{	//�J�E���^�[
						pPlayer->SetCounter(true);
						pPlayer->SetCounterTime(COUNTER_FLAME);
					}
				}
				break;
			case CPlayer::DIRECTION_RIGHT:
				if (pInputKeyboard->GetPress(PLAYER_LEFT) == true ||
					pXInput->GetPress(XPLAYER_LEFT, 0) == true)
				{
					if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
					{	//�J�E���^�[
						pPlayer->SetCounter(true);
						pPlayer->SetCounterTime(COUNTER_FLAME);
					}
				}
				break;
			}
		}

		if (pEnemy->GetCounter() == false && pEnemy->GetRecovery() == false)
		{
			//�����Ă���� �G�l�~�[
			switch (pEnemy->GetDirection())
			{
			case CEnemy::DIRECTION_LEFT:
				if (pInputKeyboard->GetPress(ENEMY_RIGHT) == true ||
					pXInput->GetPress(XENEMY_RIGHT, 1) == true)
				{
					if (pInputKeyboard->GetTrigger(ENEMY_B_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_B_BUTTON, 1) == true)
					{	//�J�E���^�[
						pEnemy->SetCounter(true);
						pEnemy->SetCounterTime(COUNTER_FLAME);
					}
				}
				break;
			case CEnemy::DIRECTION_RIGHT:
				if (pInputKeyboard->GetPress(ENEMY_LEFT) == true ||
					pXInput->GetPress(XENEMY_LEFT, 1) == true)
				{
					if (pInputKeyboard->GetTrigger(ENEMY_B_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_B_BUTTON, 1) == true)
					{	//�J�E���^�[
						pEnemy->SetCounter(true);
						pEnemy->SetCounterTime(COUNTER_FLAME);
					}
				}
				break;
			}
		}
	}
}
