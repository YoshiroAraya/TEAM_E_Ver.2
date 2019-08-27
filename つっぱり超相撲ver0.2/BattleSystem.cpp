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
#include "tutorial.h"
#include "UITime.h"
#include "gauge.h"
#include "SansoGauge.h"
#include "UltimateGauge.h"
#include "Banimation.h"
#include "tutorial.h"

#include "pause.h"
//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �}�N��
//=============================================================================
#define YORI_MOVE			(5.0f)
#define NAGE_MOVE			(60.0f)
#define OSI_MOVE			(20.0f)
#define COUNTER_MOVE		(20.0f)
#define TUPARI_MOVE			(10.0f)
#define TUPARI_MOVE_ULT		(5.0f)
#define KNOCKUP_MOVE		(3.0f)
#define KNOCKUP_MOVE_ULT	(2.0f)
#define GUARD_MOVE			(8.0f)

#define NAGE_DIVIDED		(2.0f)
#define NAGE_HINSI			(2.0f)
#define OSI_RECOIL			(1.0f)
#define TUPPARI_RECOIL		(0.5f)
#define HINSI_T_MOVE		(2.8f)
#define HINSI_N_MOVE		(1.4f)
#define HINSI_O_MOVE		(1.9f)

#define JANKEN_TUPARI_MOVE		(5.0f)
#define JANKEN_BUTI_MOVE		(5.0f)
#define JANKEN_KAWASI_MOVE		(30.1f)
#define J_TUPARI_KNOCKUP_MOVE	(15.0f)
#define J_BUTI_KNOCKUP_MOVE		(20.0f)
#define JANKEN_DAMAGE			(100.0f)


#define BATTLE_FLAME		(20)

#define YORI_FLAME			(20)
#define NAGE_FLAME			(15)
#define OSI_FLAME			(20)
#define TUPARI_FLAME		(2)
#define TUPARI_RECOVERY		(30)
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
	m_aJanken[0] = JANKEN_GU_BUTI;
	m_aJanken[1] = JANKEN_GU_BUTI;
	m_nStartCounter = 0;
	m_nCntPushP1 = 0;
	m_nCntPushP2 = 0;
	m_nFlamePush = 0;
	m_AttackTurn = ATTACK_TURN_NORMAL;
	m_nUltTimer = 0;

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
	m_aJanken[0] = JANKEN_GU_BUTI;
	m_aJanken[1] = JANKEN_GU_BUTI;
	m_nStartCounter = 0;
	m_nCntPushP1 = 0;
	m_nCntPushP2 = 0;
	m_nFlamePush = 0;
	m_AttackTurn = ATTACK_TURN_NORMAL;
	m_nUltTimer = 0;
	m_bPlayerUlt = false;
	m_bEnemyUlt = false;
	m_abUlt[0] = false;
	m_abUlt[1] = false;

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

	CPlayer *pPlayer = NULL;
	CEnemy *pEnemy = NULL;

	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{	//
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();
	}
	else if (mode == CManager::MODE_GAME)
	{	//
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();
	}

	//�L�[�{�[�h/�R���g���[���[����
	Operation();
	//�J�E���^�[����
	//CounterAttack();
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
	CPlayer *pPlayer = NULL;
	// �G�l�~�[�̎擾
	CEnemy *pEnemy = NULL;
	bool bHit = false;
	// �Q�[�W�̎擾
	CGauge *pGauge = NULL;
	CUltimateGauge *pULTGauge = NULL;
	CGame::STATE GState = CGame::STATE_GAME;
	CTutorial::STATE TState = CTutorial::STATE_GAME;

	// ���[�h�擾
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{	// �v���C���[�̎擾
		pPlayer = CTutorial::GetPlayer();
		// �G�l�~�[�̎擾
		pEnemy = CTutorial::GetEnemy();
		//�Q�[�W�̎擾
		pGauge = CTutorial::GetGauge();
		pULTGauge = CTutorial::GetUltimateGauge();
		TState = CTutorial::GetState();
		bHit = CTutorial::GetHit();
	}
	else if (mode == CManager::MODE_GAME)
	{	// �v���C���[�̎擾
		pPlayer = CGame::GetPlayer();
		// �G�l�~�[�̎擾
		pEnemy = CGame::GetEnemy();
		//�Q�[�W�̎擾
		pGauge = CGame::GetGauge();
		pULTGauge = CGame::GetUltimateGauge();
		GState = CGame::GetState();
		bHit = CGame::GetHit();
	}

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
		if (GState == CGame::STATE_GAME
			|| TState == CTutorial::STATE_GAME)
		{
			if (pPlayer->GetState() == CPlayer::STATE_JANKEN
				&& pPlayer->GetbJanken() == true)
			{
#ifdef _DEBUG
				CDebugProc::Print("c", " e����񂯂� ");
#endif
				if (pInputKeyboard->GetPress(DIK_Z) == true ||
					pXInput->GetPress(XPLAYER_B_BUTTON, 0) == true)
				{
					m_aJanken[0] = JANKEN_GU_BUTI;
					pPlayer->SetState(CPlayer::STATE_NOKOTTA);
					pPlayer->SetMotionType(0, CPlayer::MOTION_SYAGAMI);
					pPlayer->SetbMotionEnd(0, true);
					pPlayer->SetMotionType(1, CPlayer::MOTION_SYAGAMI);
					pPlayer->SetbMotionEnd(1, true);
				}
				else if (pInputKeyboard->GetPress(DIK_X) == true ||
					pXInput->GetPress(XPLAYER_Y_BUTTON, 0) == true)
				{
					m_aJanken[0] = JANKEN_CHOKI_TUPPA;
					pPlayer->SetState(CPlayer::STATE_NOKOTTA);
					pPlayer->SetMotionType(0, CPlayer::MOTION_SYAGAMI);
					pPlayer->SetbMotionEnd(0, true);
					pPlayer->SetMotionType(1, CPlayer::MOTION_SYAGAMI);
					pPlayer->SetbMotionEnd(1, true);
				}
				else if (pInputKeyboard->GetPress(DIK_C) == true ||
					pXInput->GetPress(XPLAYER_X_BUTTON, 0) == true)
				{
					m_aJanken[0] = JANKEN_PA_KAWASI;
					pPlayer->SetState(CPlayer::STATE_NOKOTTA);
					pPlayer->SetMotionType(0, CPlayer::MOTION_SYAGAMI);
					pPlayer->SetbMotionEnd(0, true);
					pPlayer->SetMotionType(1, CPlayer::MOTION_SYAGAMI);
					pPlayer->SetbMotionEnd(1, true);
					m_abPA[0] = true;
				}
			}

			if (pEnemy->GetState() == CEnemy::STATE_JANKEN
				&& pEnemy->GetbJanken() == true)
			{
#ifdef _DEBUG
				CDebugProc::Print("c", " e����񂯂� ");
#endif
				//CPU�Ɛl�Ԃ̂���񂯂񑀍�
				if (pEnemy->GetMode() == CEnemy::MODE_CPU)
				{	//����񂯂�������_���Ɍ���
					int nRandJanken = rand() % 3;

					switch (nRandJanken)
					{
					case 0:
						m_aJanken[1] = JANKEN_GU_BUTI;
						pEnemy->SetState(CEnemy::STATE_NOKOTTA);
						pEnemy->SetMotionType(0, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(1, true);
						break;
					case 1:
						m_aJanken[1] = JANKEN_CHOKI_TUPPA;
						pEnemy->SetState(CEnemy::STATE_NOKOTTA);
						pEnemy->SetMotionType(0, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(1, true);
						break;
					case 2:
						m_aJanken[1] = JANKEN_PA_KAWASI;
						pEnemy->SetState(CEnemy::STATE_NOKOTTA);
						m_abPA[1] = true;
						pEnemy->SetMotionType(0, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(1, true);
						break;
					}

				}
				else
				{	//�l�Ԃ̑���
					if (pInputKeyboard->GetPress(DIK_B) == true ||
						pXInput->GetPress(XENEMY_B_BUTTON, 1) == true)
					{
						m_aJanken[1] = JANKEN_GU_BUTI;
						pEnemy->SetState(CEnemy::STATE_NOKOTTA);
						pEnemy->SetMotionType(0, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(1, true);
					}
					else if (pInputKeyboard->GetPress(DIK_N) == true ||
						pXInput->GetPress(XENEMY_Y_BUTTON, 1) == true)
					{
						m_aJanken[1] = JANKEN_CHOKI_TUPPA;
						pEnemy->SetState(CEnemy::STATE_NOKOTTA);
						pEnemy->SetMotionType(0, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(1, true);
					}
					else if (pInputKeyboard->GetPress(DIK_M) == true ||
						pXInput->GetPress(XENEMY_X_BUTTON, 1) == true)
					{
						m_aJanken[1] = JANKEN_PA_KAWASI;
						pEnemy->SetState(CEnemy::STATE_NOKOTTA);
						m_abPA[1] = true;
						pEnemy->SetMotionType(0, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(1, true);
					}
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
				if (m_aJanken[0] == JANKEN_GU_BUTI && m_aJanken[1] == JANKEN_GU_BUTI)
				{// �O�[�ƃO�[
					{
						pPlayer->SetMove(D3DXVECTOR3(JANKEN_BUTI_MOVE, 0.0f, 0.0f));
						pEnemy->SetMove(D3DXVECTOR3(-JANKEN_BUTI_MOVE, 0.0f, 0.0f));
					}
				}
				else if (m_aJanken[0] == JANKEN_GU_BUTI && m_aJanken[1] == JANKEN_CHOKI_TUPPA)
				{// �O�[�ƃ`���L
					m_aGUCounter[0]++;
					if (m_aGUCounter[0] < GU_COUNTER)
					{
						pPlayer->SetMove(D3DXVECTOR3(JANKEN_BUTI_MOVE, 0.0f, 0.0f));
					}
					else if (m_aGUCounter[0] >= GU_COUNTER)
					{
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						m_aGUCounter[0] = 0;
					}
					m_aCHOKICounter[1]++;
					if (m_aCHOKICounter[1] < CHOKI_COUNTER)
					{
						pEnemy->SetMove(D3DXVECTOR3(-JANKEN_TUPARI_MOVE, 0.0f, 0.0f));
					}
					if (bHit == true)
					{
						pEnemy->SetMove(D3DXVECTOR3(J_BUTI_KNOCKUP_MOVE * 3, 5.0f, 0.0f));
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						if (mode == CManager::MODE_TUTORIAL)
						{
							CTutorial::SetHit(false);
						}
						else if (mode == CManager::MODE_GAME)
						{
							CGame::SetHit(false);
						}
						//���C�t�Q�[�W����
						pGauge->SetGaugeRightLeft(-JANKEN_DAMAGE, 0.0f );
						pULTGauge->SetGaugeRightLeft(0.0f, 30.0f);
					}

				}
				else if (m_aJanken[0] == JANKEN_GU_BUTI && m_aJanken[1] == JANKEN_PA_KAWASI)
				{// �O�[�ƃp�[
					m_aGUCounter[0]++;

					if (m_aGUCounter[0] < GU_COUNTER)
					{
						pPlayer->SetMove(D3DXVECTOR3(JANKEN_BUTI_MOVE, 0.0f, 0.0f));
					}
					else if (m_aGUCounter[0] >= GU_COUNTER)
					{
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						m_aGUCounter[0] = 0;
					}

					if (m_abPA[1] == true)
					{
						pEnemy->SetMove(D3DXVECTOR3(-JANKEN_KAWASI_MOVE, 10.0f, 0.0f));
						m_abPA[1] = false;
						//���C�t�Q�[�W����
						pGauge->SetGaugeRightLeft(0.0f, -JANKEN_DAMAGE );
						pULTGauge->SetGaugeRightLeft(30.0f, 0.0f);
					}
				}
				else if (m_aJanken[0] == JANKEN_CHOKI_TUPPA && m_aJanken[1] == JANKEN_GU_BUTI)
				{// �`���L�ƃO�[
					if (bHit == true)
					{
						pPlayer->SetMove(D3DXVECTOR3(-J_BUTI_KNOCKUP_MOVE * 3, 5.0f, 0.0f));
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						if (mode == CManager::MODE_TUTORIAL)
						{
							CTutorial::SetHit(false);
						}
						else if (mode == CManager::MODE_GAME)
						{
							CGame::SetHit(false);
						}						//���C�t�Q�[�W����
						pGauge->SetGaugeRightLeft(0.0f, -JANKEN_DAMAGE);
						pULTGauge->SetGaugeRightLeft(30.0f, 0.0f);
					}

					m_aCHOKICounter[0]++;
					if (m_aCHOKICounter[0] < CHOKI_COUNTER)
					{
						pPlayer->SetMove(D3DXVECTOR3(JANKEN_TUPARI_MOVE, 0.0f, 0.0f));
					}
					m_aGUCounter[1]++;

					if (m_aGUCounter[1] < GU_COUNTER)
					{
						pEnemy->SetMove(D3DXVECTOR3(-JANKEN_BUTI_MOVE, 0.0f, 0.0f));
					}

				}
				else if (m_aJanken[0] == JANKEN_CHOKI_TUPPA && m_aJanken[1] == JANKEN_CHOKI_TUPPA)
				{// �`���L�ƃ`���L
					m_aCHOKICounter[0]++;

					if (m_aCHOKICounter[0] < CHOKI_COUNTER)
					{
						pEnemy->SetMove(D3DXVECTOR3(-JANKEN_TUPARI_MOVE, 0.0f, 0.0f));
						pPlayer->SetMove(D3DXVECTOR3(JANKEN_TUPARI_MOVE, 0.0f, 0.0f));
					}

					if (m_aCHOKICounter[0] > CHOKI_COUNTER && CHOKI_COUNTER + 5 >= m_aCHOKICounter[0])
					{
						pEnemy->SetMove(D3DXVECTOR3(J_TUPARI_KNOCKUP_MOVE, 3.0f, 0.0f));
					}
					else if (m_aCHOKICounter[0] > CHOKI_COUNTER + 5)
					{
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						m_aCHOKICounter[0] = 0;
					}

					m_aCHOKICounter[1]++;

					if (m_aCHOKICounter[1] > CHOKI_COUNTER && CHOKI_COUNTER + 5 >= m_aCHOKICounter[1])
					{
						pPlayer->SetMove(D3DXVECTOR3(-J_TUPARI_KNOCKUP_MOVE, 3.0f, 0.0f));
					}
					else if (m_aCHOKICounter[1] > CHOKI_COUNTER + 5)
					{
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						m_aCHOKICounter[1] = 0;
					}
				}
				else if (m_aJanken[0] == JANKEN_CHOKI_TUPPA && m_aJanken[1] == JANKEN_PA_KAWASI)
				{// �`���L�ƃp�[
					m_aCHOKICounter[0]++;

					if (m_aCHOKICounter[0] < CHOKI_COUNTER)
					{
						pPlayer->SetMove(D3DXVECTOR3(JANKEN_TUPARI_MOVE, 0.0f, 0.0f));
					}
					if (m_aCHOKICounter[0] > CHOKI_COUNTER && CHOKI_COUNTER + 5 >= m_aCHOKICounter[0])
					{
						pEnemy->SetMove(D3DXVECTOR3(J_TUPARI_KNOCKUP_MOVE, 3.0f, 0.0f));
					}
					else if (m_aCHOKICounter[0] > CHOKI_COUNTER + 5)
					{
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						m_aCHOKICounter[0] = 0;
						//���C�t�Q�[�W����
						pGauge->SetGaugeRightLeft(-JANKEN_DAMAGE, 0.0f);
						pULTGauge->SetGaugeRightLeft(0.0f, 30.0f);
					}

					if (m_abPA[1] == true)
					{
						pEnemy->SetMove(D3DXVECTOR3(-JANKEN_KAWASI_MOVE, 10.0f, 0.0f));
						m_abPA[1] = false;
					}

				}
				else if (m_aJanken[0] == JANKEN_PA_KAWASI && m_aJanken[1] == JANKEN_GU_BUTI)
				{// �p�[�ƃO�[
					if (m_abPA[0] == true)
					{
						pPlayer->SetMove(D3DXVECTOR3(JANKEN_KAWASI_MOVE, 10.0f, 0.0f));
						m_abPA[0] = false;
						//���C�t�Q�[�W����
						pGauge->SetGaugeRightLeft(-JANKEN_DAMAGE, 0.0f);
						pULTGauge->SetGaugeRightLeft(0.0f, 30.0f);
					}

					m_aGUCounter[1]++;

					if (m_aGUCounter[1] < GU_COUNTER)
					{
						pEnemy->SetMove(D3DXVECTOR3(-JANKEN_BUTI_MOVE, 0.0f, 0.0f));
					}
					else if (m_aGUCounter[1] >= GU_COUNTER)
					{
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						m_aGUCounter[1] = 0;
					}


				}
				else if (m_aJanken[0] == JANKEN_PA_KAWASI && m_aJanken[1] == JANKEN_CHOKI_TUPPA)
				{// �p�[�ƃ`���L
					if (m_abPA[0] == true)
					{
						pPlayer->SetMove(D3DXVECTOR3(JANKEN_KAWASI_MOVE, 10.0f, 0.0f));
						m_abPA[0] = false;
					}

					m_aCHOKICounter[1]++;
					if (m_aCHOKICounter[1] < CHOKI_COUNTER)
					{
						pEnemy->SetMove(D3DXVECTOR3(-JANKEN_TUPARI_MOVE, 0.0f, 0.0f));
					}

					if (m_aCHOKICounter[1] > CHOKI_COUNTER && CHOKI_COUNTER + 5 >= m_aCHOKICounter[1])
					{
						pPlayer->SetMove(D3DXVECTOR3(-J_TUPARI_KNOCKUP_MOVE, 3.0f, 0.0f));
					}
					else if (m_aCHOKICounter[1] > CHOKI_COUNTER + 5)
					{
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						m_aCHOKICounter[1] = 0;
						//���C�t�Q�[�W����
						pGauge->SetGaugeRightLeft(0.0f, -JANKEN_DAMAGE );
						pULTGauge->SetGaugeRightLeft(30.0f, 0.0f);
					}
				}
				else if (m_aJanken[0] == JANKEN_PA_KAWASI && m_aJanken[1] == JANKEN_PA_KAWASI)
				{// �p�[�ƃp�[
					if (m_abPA[0] == true)
					{
						pPlayer->SetMove(D3DXVECTOR3(JANKEN_KAWASI_MOVE, 10.0f, 0.0f));
						m_abPA[0] = false;
					//	pPlayer->SetState(CPlayer::STATE_KUMI);
					}

					if (m_abPA[1] == true)
					{
						pEnemy->SetMove(D3DXVECTOR3(-JANKEN_KAWASI_MOVE, 10.0f, 0.0f));
						m_abPA[1] = false;
					//	pEnemy->SetState(CEnemy::STATE_KUMI);
					}
				}

				//���[�V�����̕ω�
				if (m_aJanken[0] == JANKEN_GU_BUTI && pPlayer->GetMotionType(0) != CPlayer::MOTION_BUTIKAMASI)
				{
					pPlayer->SetMotionType(0, CPlayer::MOTION_BUTIKAMASI);
					pPlayer->SetbMotionEnd(0, true);
					pPlayer->SetMotionType(1, CPlayer::MOTION_BUTIKAMASI);
					pPlayer->SetbMotionEnd(1, true);
				}
				else if (m_aJanken[0] == JANKEN_CHOKI_TUPPA && pPlayer->GetMotionType(0) != CPlayer::MOTION_TSUPPARI)
				{
					if (pPlayer->GetMotionType(1) != CPlayer::MOTION_TSUPPARI)
					{
						pPlayer->SetMotionType(0, CPlayer::MOTION_TSUPPARI);
						pPlayer->SetbMotionEnd(0, true);
						pPlayer->SetMotionType(1, CPlayer::MOTION_TSUPPARI);
						pPlayer->SetbMotionEnd(1, true);
					}
				}
				else if (m_aJanken[0] == JANKEN_PA_KAWASI && pPlayer->GetMotionType(0) != CPlayer::MOTION_KAWASI)
				{
					pPlayer->SetMotionType(0, CPlayer::MOTION_KAWASI);
					pPlayer->SetbMotionEnd(0, true);
					pPlayer->SetMotionType(1, CPlayer::MOTION_KAWASI);
					pPlayer->SetbMotionEnd(1, true);
				}

				if (m_aJanken[1] == JANKEN_GU_BUTI && pEnemy->GetMotionType(0) != CEnemy::MOTION_BUTIKAMASI)
				{
					pEnemy->SetMotionType(0, CEnemy::MOTION_BUTIKAMASI);
					pEnemy->SetbMotionEnd(0, true);
					pEnemy->SetMotionType(1, CEnemy::MOTION_BUTIKAMASI);
					pEnemy->SetbMotionEnd(1, true);
				}
				else if (m_aJanken[1] == JANKEN_CHOKI_TUPPA && pEnemy->GetMotionType(0) != CEnemy::MOTION_TSUPPARI)
				{
					if (pEnemy->GetMotionType(1) != CEnemy::MOTION_TSUPPARI)
					{
						pEnemy->SetMotionType(0, CEnemy::MOTION_TSUPPARI);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_TSUPPARI);
						pEnemy->SetbMotionEnd(1, true);
					}
				}
				else if (m_aJanken[1] == JANKEN_PA_KAWASI && pEnemy->GetMotionType(0) != CEnemy::MOTION_KAWASI)
				{
					pEnemy->SetMotionType(0, CEnemy::MOTION_KAWASI);
					pEnemy->SetbMotionEnd(0, true);
					pEnemy->SetMotionType(1, CEnemy::MOTION_KAWASI);
					pEnemy->SetbMotionEnd(1, true);
				}
			}
		}

		//�m����
		if (pPlayer->GetDying() == true)
		{
			m_fMoveDying[0] = HINSI_T_MOVE;
		}
		else if (pPlayer->GetDying() == false)
		{
			m_fMoveDying[0] = 1.0f;
		}

		if (pEnemy->GetDying() == true)
		{
			m_fMoveDying[1] = HINSI_T_MOVE;
		}
		else if (pEnemy->GetDying() == false)
		{
			m_fMoveDying[1] = 1.0f;
		}

		//�d�Ȃ�Ȃ��悤��
		if (pPlayer->GetState() == CPlayer::STATE_KUMI
			&& pEnemy->GetState() == CEnemy::STATE_KUMI)
		{
			//���݂��̒��S������ -10 +10�Ԃ�0  -150 0�Ԃ�-75  -50 +150�Ԃ� 50
			float DiffDis = (p1pos.x + p2pos.x) / 2;

			if (pPlayer->GetDirection() == CPlayer::DIRECTION_RIGHT
				&& pEnemy->GetDirection() == CEnemy::DIRECTION_LEFT)
			{
				//POS����(�Œ�)
				pPlayer->SetPosition(D3DXVECTOR3(DiffDis - 18, p1pos.y, p1pos.z));
				pEnemy->SetPosition(D3DXVECTOR3(DiffDis + 18, p2pos.y, p2pos.z));
			}
			else if (pPlayer->GetDirection() == CPlayer::DIRECTION_LEFT
				&& pEnemy->GetDirection() == CEnemy::DIRECTION_RIGHT)
			{
				//POS����(�Œ�)
				pPlayer->SetPosition(D3DXVECTOR3(DiffDis + 18, p1pos.y, p1pos.z));
				pEnemy->SetPosition(D3DXVECTOR3(DiffDis - 18, p2pos.y, p2pos.z));
			}
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
		if (pPlayer->GetState() == CPlayer::STATE_NEUTRAL && m_bAttack == false)
		{
			if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true && pPlayer->GetRecovery() == false ||
				pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true && pPlayer->GetRecovery() == false)
			{
				pPlayer->SetMotionType(0, CPlayer::MOTION_TSUPPARI);
				pPlayer->SetbMotionEnd(0, true);
				pPlayer->SetMotionType(1, CPlayer::MOTION_TSUPPARI);
				pPlayer->SetbMotionEnd(1, true);
				pPlayer->SetbDash(false);
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
		}
		//�G�l�~�[�̂��ς�
		if (pEnemy->GetState() == CEnemy::STATE_NEUTRAL && m_bAttack == false && pEnemy->GetMode() == CEnemy::MODE_P2)
		{
			if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true && pEnemy->GetRecovery() == false ||
				pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true && pEnemy->GetRecovery() == false)
			{
				pEnemy->SetMotionType(0, CEnemy::MOTION_TSUPPARI);
				pEnemy->SetbMotionEnd(0, true);
				pEnemy->SetMotionType(1, CEnemy::MOTION_TSUPPARI);
				pEnemy->SetbMotionEnd(1, true);
				pEnemy->SetbDash(false);
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

		if (pEnemy->GetUltDamage() == false)
		{// �ʏ펞
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
				pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
				pULTGauge->SetGaugeRightLeft(0.0f, 30.0f);
			}
		}
		else
		{// �˂�����̉��`���󂯂Ă���Ƃ�
			//�_���[�W�Ȃ琁����� �v���C���[�̂��ς�
			if (pEnemy->GetState() == CEnemy::STATE_DAMAGE)
			{
				//��ԕω�
				pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				//�_���[�W
				pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
				pULTGauge->SetGaugeRightLeft(0.0f, 30.0f);

				if (pEnemy->GetDying() == true)
				{
					m_abUlt[0] = false;
					m_bPlayerUlt = false;
					switch (pPlayer->GetDirection())
					{
					case CPlayer::DIRECTION_LEFT:
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_TUPPARI, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(((-TUPARI_MOVE * 17.0f)), KNOCKUP_MOVE_ULT * 2.0f, 0.0f));
						break;
					case CPlayer::DIRECTION_RIGHT:
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_TUPPARI, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(((TUPARI_MOVE * 17.0f)), KNOCKUP_MOVE_ULT * 2.0f, 0.0f));
						break;
					}
				}
				else
				{
					switch (pPlayer->GetDirection())
					{
					case CPlayer::DIRECTION_LEFT:
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_TUPPARI, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3((-TUPARI_MOVE * m_fMoveDying[1]), KNOCKUP_MOVE_ULT, 0.0f));
						break;
					case CPlayer::DIRECTION_RIGHT:
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_TUPPARI, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3((TUPARI_MOVE * m_fMoveDying[1]), KNOCKUP_MOVE_ULT, 0.0f));
						break;
					}
				}
			}
			if (pEnemy->GetWallHit() == true)
			{
				pEnemy->SetUltDamage(false);
			}
		}
		
		if (pPlayer->GetUltDamage() == false)
		{
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
				pULTGauge->SetGaugeRightLeft(0.0f, 30.0f);
			}
		}
		else
		{// �˂�����̉��`���󂯂Ă���Ƃ�
		 //�_���[�W�Ȃ琁����� �v���C���[�̂��ς�
			if (pPlayer->GetState() == CPlayer::STATE_DAMAGE)
			{
				//��ԕω�
				pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				//�_���[�W
				pGauge->SetGaugeRightLeft(HEEL, -DAMAGE);
				pULTGauge->SetGaugeRightLeft(30.0f, 0.0f);

				if (pPlayer->GetDying() == true)
				{
					m_abUlt[1] = false;
					m_bEnemyUlt = false;
					switch (pEnemy->GetDirection())
					{
					case CEnemy::DIRECTION_LEFT:
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_TUPPARI, D3DXVECTOR3(((-TUPARI_MOVE * 17.0f)), KNOCKUP_MOVE_ULT * 2.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
						break;
					case CEnemy::DIRECTION_RIGHT:
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_TUPPARI, D3DXVECTOR3(((TUPARI_MOVE * 17.0f)), KNOCKUP_MOVE_ULT * 2.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
						break;
					}
				}
				else
				{
					switch (pEnemy->GetDirection())
					{
					case CEnemy::DIRECTION_LEFT:
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_TUPPARI, D3DXVECTOR3((-TUPARI_MOVE * m_fMoveDying[0]), KNOCKUP_MOVE_ULT, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
						break;
					case CEnemy::DIRECTION_RIGHT:
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_TUPPARI, D3DXVECTOR3((TUPARI_MOVE * m_fMoveDying[0]), KNOCKUP_MOVE_ULT, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
						break;
					}
				}
			}
			if (pPlayer->GetWallHit() == true)
			{
				pPlayer->SetUltDamage(false);
			}
		}

		//������x�擾
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
			//�y�U�[�ő̗͂���//���ς��Ȃ�c��
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
			if (pEnemy->GetDying() == false)
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
			pPlayer->InitStatus();
			pEnemy->InitStatus();
			pPlayer->SetPosition(D3DXVECTOR3(-80.0f, 20.0f, 0.0f));
			pEnemy->SetPosition(D3DXVECTOR3(80.0f, 20.0f, 0.0f));
			pPlayer->SetState(CPlayer::STATE_JANKEN);
			pEnemy->SetState(CEnemy::STATE_JANKEN);
			m_bAttack = false;
			m_nCntAttackFlame = 0;
			m_nStartCounter = 0;
			pPlayer->SetMotionType(0, CPlayer::MOTION_NEUTRAL);
			pPlayer->SetMotionType(1, CPlayer::MOTION_NEUTRAL);
			pEnemy->SetMotionType(0, CEnemy::MOTION_NEUTRAL);
			pEnemy->SetMotionType(1, CEnemy::MOTION_NEUTRAL);
			m_aJanken[0] = JANKEN_GU_BUTI;
			m_aJanken[1] = JANKEN_GU_BUTI;

			for (int nCntPlayer = 0; nCntPlayer < MAX_CHARACTER; nCntPlayer++)
			{
				m_aGUCounter[nCntPlayer] = 0;
				m_aCHOKICounter[nCntPlayer] = 0;
				m_abPA[nCntPlayer] = false;
			}

			if (mode == CManager::MODE_TUTORIAL)
			{
				CTutorial::SetWinner(CTutorial::WINNER_NONE);
				CTutorial::SetHit(true);
				CManager::GetTutorial()->SetbUI(true);
			}
			else if (mode == CManager::MODE_GAME)
			{
				CGame::SetWinner(CGame::WINNER_NONE);
				CGame::SetHit(true);
				CManager::GetGame()->SetbUI(true);
			}
		}

		if (pPlayer->GetUltDis() == true && pInputKeyboard->GetTrigger(DIK_5) == true)
		{
			m_bPlayerUlt = true;

			pULTGauge->SetGaugeRightLeft(pULTGauge->GetGaugeRight(), -600.0f);
		}
		if (pEnemy->GetUltDis() == true && pInputKeyboard->GetTrigger(DIK_6) == true)
		{
			m_bEnemyUlt = true;

			pULTGauge->SetGaugeRightLeft(0.0f, pULTGauge->GetGaugeLeft());
		}

		if (m_bPlayerUlt == true)
		{// ���`�i�v���C���[�j
			if ((float)m_nUltTimer / 60.0f >= 1.5f)
			{// ������x�̕b������������
				m_abUlt[0] = true;

				// �˂�����
				pPlayer->SetState(CPlayer::STATE_TSUPPARI);
				pPlayer->SetMotionType(0, CPlayer::MOTION_TSUPPARI);
				pPlayer->SetbMotionEnd(0, true);
				pPlayer->SetMotionType(1, CPlayer::MOTION_TSUPPARI);
				pPlayer->SetbMotionEnd(1, true);
				pPlayer->SetbDash(false);
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

				m_nUltTimer = 70;
			}
			else
			{
				m_nUltTimer++;

			}

			// �v���C���[�����`��Ԃɂ���
			pPlayer->SetState(CPlayer::STATE_ULT);

			CBAnimation *pAnimation = pPlayer->GetAnimation();

			if (pAnimation != NULL)
			{// �I�[����傫������
				pAnimation->SetBillboard(pPlayer->GetPosition(), 150.0f, 100.0f);
			}
		}
		if (m_bEnemyUlt == true)
		{// ���`�i�G�l�~�[�j
			if ((float)m_nUltTimer / 60.0f >= 1.5f)
			{// ������x�̕b������������
				m_abUlt[1] = true;

				// �˂�����
				pEnemy->SetState(CEnemy::STATE_TSUPPARI);
				pEnemy->SetMotionType(0, CEnemy::MOTION_TSUPPARI);
				pEnemy->SetbMotionEnd(0, true);
				pEnemy->SetMotionType(1, CEnemy::MOTION_TSUPPARI);
				pEnemy->SetbMotionEnd(1, true);
				pEnemy->SetbDash(false);
				//�����Ă���� �v���C���[
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

				m_nUltTimer = 70;
			}
			else
			{
				m_nUltTimer++;

			}

			// �v���C���[�����`��Ԃɂ���
			pEnemy->SetState(CEnemy::STATE_ULT);

			CBAnimation *pAnimation = pEnemy->GetAnimation();

			if (pAnimation != NULL)
			{// �I�[����傫������
				pAnimation->SetBillboard(pEnemy->GetPosition(), 150.0f, 100.0f);
			}
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
	CPlayer *pPlayer = NULL;
	// �G�l�~�[�̎擾
	CEnemy *pEnemy = NULL;
	// ���[�h�擾
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();

		CTutorial::SetWinner(CTutorial::WINNER_NONE);
		CTutorial::SetHit(true);
		CTutorial::SetHit(false);
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();

		CGame::SetWinner(CGame::WINNER_NONE);
		CGame::SetHit(true);
		CGame::SetHit(false);
	}

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
	CPlayer *pPlayer = NULL;
	// �G�l�~�[
	CEnemy *pEnemy = NULL;
	// �Q�[�W
	CGauge *pGauge = NULL;
	CUltimateGauge *pULTGauge = NULL;

	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();
		pGauge = CTutorial::GetGauge();
		pULTGauge = CTutorial::GetUltimateGauge();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();
		pGauge = CGame::GetGauge();
		pULTGauge = CGame::GetUltimateGauge();
	}

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
		if (m_bPlayerUlt == false)
		{
			pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
			pULTGauge->SetGaugeRightLeft(0.0f, 30.0f);
		}
	}
	else
	{
		pGauge->SetGaugeRightLeft(HEEL, -DAMAGE);
		pULTGauge->SetGaugeRightLeft(30.0f, 0.0f);
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
		pEnemy->SetState(CEnemy::STATE_NAGE);
		pPlayer->SetState(CPlayer::STATE_NAGE);

		break;
	case ATTACK_TYPE_TUPPARI:
		if (pPlayer->GetDohyo() == CPlayer::DOHYO_HAZI && pPlayer->GetDying() == false)
		{//�v���C���[�̗̑͂�����ꍇ�œy�U�[�̂��ς�U��
			if (nPlayer == 0)
			{	//�v���C���[�̂��ς�
				if (pPlayer->GetUltDamage() == true)
				{
					pEnemy->SetMove(D3DXVECTOR3(P2move.x / 1.5f, 0.0f, 0.0f));
				}
				else
				{
					pEnemy->SetMove(D3DXVECTOR3(P2move.x / TUPPARI_RECOIL, 0.0f, 0.0f));
				}
				pPlayer->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
			}
			else
			{	//�G�l�~�[�̂��ς�
				if (pPlayer->GetUltDamage() == true)
				{
					pEnemy->SetMove(D3DXVECTOR3(-P1move.x / 1.5f, 0.0f, 0.0f));
				}
				else
				{
					pEnemy->SetMove(D3DXVECTOR3(-P1move.x / TUPPARI_RECOIL, 0.0f, 0.0f));
				}
				pPlayer->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
			}
		}
		else if (pEnemy->GetDohyo() == CEnemy::DOHYO_HAZI && pEnemy->GetDying() == false)
		{//�G�l�~�[�̗̑͂�����ꍇ�œy�U�[�̂��ς�U��
			if (nPlayer == 0)
			{	//�v���C���[�̂��ς�
				if (pEnemy->GetUltDamage() == true)
				{
					pPlayer->SetMove(D3DXVECTOR3(-P2move.x / 1.5f, 0.0f, 0.0f));
				}
				else
				{
					pPlayer->SetMove(D3DXVECTOR3(-P2move.x / TUPPARI_RECOIL, 0.0f, 0.0f));
				}
				
				pEnemy->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
			}
			else
			{	//�G�l�~�[�̂��ς�
				if (pEnemy->GetUltDamage() == true)
				{
					pPlayer->SetMove(D3DXVECTOR3(P1move.x / 1.5f, KNOCKUP_MOVE, 0.0f));
				}
				else 
				{
					pPlayer->SetMove(D3DXVECTOR3(P1move.x / TUPPARI_RECOIL, KNOCKUP_MOVE, 0.0f));
				}
				
				pEnemy->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
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
			pGauge->SetGaugeRightLeft(0, HEEL);
		}
		else
		{
			pGauge->SetGaugeRightLeft(HEEL, 0);
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
	CPlayer *pPlayer = NULL;
	// �G�l�~�[
	CEnemy *pEnemy = NULL;
	// �Q�[�W
	CSansoGauge *pSansoGauge = NULL;
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();
		pSansoGauge = CTutorial::GetSansoGauge();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();
		pSansoGauge = CGame::GetSansoGauge();
	}

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
					pSansoGauge->SetSansoGaugeRightLeft(SANSO_KUMI, 0);
				}
			}
			else
			{
				//�_�f��
				pSansoGauge->SetSansoGaugeRightLeft(SANSO_NEUTRAL, 0);
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
					pSansoGauge->SetSansoGaugeRightLeft(0, SANSO_KUMI);
				}
			}
			else
			{
				//�_�f��
				pSansoGauge->SetSansoGaugeRightLeft(0, SANSO_NEUTRAL);
			}

			m_nFlamePush++;
			//�_�f����
			fPushCntP1 *= SANSO_SIKAKE;
			fPushCntP2 *= SANSO_SIKAKE;
			pSansoGauge->SetSansoGaugeRightLeft(-fPushCntP1, -fPushCntP2);
		}
		else
		{
			m_nFlamePush = 0;
			m_nCntPushP1 = 0;
			m_nCntPushP2 = 0;
			//�_�f��
			pSansoGauge->SetSansoGaugeRightLeft(SANSO_NEUTRAL, SANSO_NEUTRAL);
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
	CPlayer *pPlayer = NULL;
	// �G�l�~�[
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

	//�T�E���h���̎擾
	CSound *pSound = CManager::GetSound(0);
	//�m�����̈ړ���
	float fHinshiOshi = 1.0f;
	float fHinshiNage = 1.0f;

	if (pEnemy != NULL)
	{
		if (pEnemy->GetDying() == true)
		{
			fHinshiNage = HINSI_N_MOVE;
			fHinshiOshi = HINSI_O_MOVE;
		}
		if (pEnemy->GetCounter() == false)
		{	//�����Ă���� �v���C���[
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
						MotionSetYORI(0);
					}
					else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
					{	//����
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_OSI, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-OSI_MOVE * fHinshiOshi, KNOCKUP_MOVE, 0.0f));
						pPlayer->SetMotionType(0, CPlayer::MOTION_TSUPPARI);
						pPlayer->SetbMotionEnd(0, true);
						pPlayer->SetMotionType(1, CPlayer::MOTION_TSUPPARI);
						pPlayer->SetbMotionEnd(1, true);
						pPlayer->SetbDash(false);
					}
				}
				else if (pInputKeyboard->GetPress(PLAYER_RIGHT) == true ||
					pXInput->GetPress(XPLAYER_RIGHT, 0) == true)
				{
					if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
					{	//����
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_NAGE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3((NAGE_MOVE * fHinshiNage), KNOCKUP_MOVE, 0.0f));
						pPlayer->SetMotionType(0, CPlayer::MOTION_NAGE);
						//pPlayer->SetbMotionEnd(0, true);
						pPlayer->SetMotionType(1, CPlayer::MOTION_NAGE);
						//pPlayer->SetbMotionEnd(1, true);
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
						MotionSetYORI(0);
					}
					else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
					{	//����
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_OSI, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(OSI_MOVE * fHinshiOshi, KNOCKUP_MOVE, 0.0f));
						pPlayer->SetMotionType(0, CPlayer::MOTION_TSUPPARI);
						pPlayer->SetbMotionEnd(0, true);
						pPlayer->SetMotionType(1, CPlayer::MOTION_TSUPPARI);
						pPlayer->SetbMotionEnd(1, true);
						pPlayer->SetbDash(false);
					}
				}
				else if (pInputKeyboard->GetPress(PLAYER_LEFT) == true ||
					pXInput->GetPress(XPLAYER_LEFT, 0) == true)
				{
					if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
					{	//����
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_NAGE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3((-NAGE_MOVE * fHinshiNage), KNOCKUP_MOVE, 0.0f));
						pPlayer->SetMotionType(0, CPlayer::MOTION_NAGE);
						pPlayer->SetbMotionEnd(0, true);
						pPlayer->SetMotionType(1, CPlayer::MOTION_NAGE);
						pPlayer->SetbMotionEnd(1, true);
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

	CManager::MODE mode;
	mode = CManager::GetMode();

	// �v���C���[�̎擾
	CPlayer *pPlayer = NULL;
	// �G�l�~�[
	CEnemy *pEnemy = NULL;

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
	//�T�E���h���̎擾
	CSound *pSound = CManager::GetSound(0);

	//�m�����̈ړ���
	float fHinshiOshi = 1.0f;
	float fHinshiNage = 1.0f;

	if (pPlayer != NULL)
	{
		if (pPlayer->GetDying() == true)
		{
			fHinshiNage = HINSI_N_MOVE;
			fHinshiOshi = HINSI_O_MOVE;
		}

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
						MotionSetYORI(1);
					}
					else if (pInputKeyboard->GetTrigger(ENEMY_B_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_B_BUTTON, 1) == true)
					{	//����
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_OSI, D3DXVECTOR3(-OSI_MOVE * fHinshiOshi, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
						pEnemy->SetMotionType(0, CEnemy::MOTION_TSUPPARI);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_TSUPPARI);
						pEnemy->SetbMotionEnd(1, true);
						pEnemy->SetbDash(false);
					}
				}
				else if (pInputKeyboard->GetPress(ENEMY_RIGHT) == true ||
					pXInput->GetPress(XENEMY_RIGHT, 1) == true)
				{
					if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
					{	//����
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_NAGE, D3DXVECTOR3(NAGE_MOVE * fHinshiNage, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
						pEnemy->SetMotionType(0, CEnemy::MOTION_NAGE);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_NAGE);
						pEnemy->SetbMotionEnd(1, true);
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
						MotionSetYORI(1);
					}
					else if (pInputKeyboard->GetTrigger(ENEMY_B_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_B_BUTTON, 1) == true)
					{	//����
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_OSI, D3DXVECTOR3(OSI_MOVE * fHinshiOshi, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
						pEnemy->SetMotionType(0, CEnemy::MOTION_TSUPPARI);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_TSUPPARI);
						pEnemy->SetbMotionEnd(1, true);
						pEnemy->SetbDash(false);
					}
				}
				else if (pInputKeyboard->GetPress(ENEMY_LEFT) == true ||
					pXInput->GetPress(XENEMY_LEFT, 1) == true)
				{
					if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
					{	//����
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_NAGE, D3DXVECTOR3(-NAGE_MOVE * fHinshiNage, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
						pEnemy->SetMotionType(0, CEnemy::MOTION_NAGE);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_NAGE);
						pEnemy->SetbMotionEnd(1, true);
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
	CPlayer *pPlayer = NULL;
	// �G�l�~�[
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

//=============================================================================
// �o�g���̃��Z�b�g����
//=============================================================================
void CBattleSys::ResetBattle(void)
{
	// �v���C���[�̎擾
	CPlayer *pPlayer = NULL;
	// �G�l�~�[
	CEnemy *pEnemy = NULL;
	// �Q�[�W�̎擾
	CGauge *pGauge = NULL;
	CSansoGauge *pSansoGauge = NULL;
	CUltimateGauge *pULTGauge = NULL;
	// �^�C���̎擾
	CUITime *pTime = NULL;
	//���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();
		pGauge = CTutorial::GetGauge();
		pSansoGauge = CTutorial::GetSansoGauge();
		pULTGauge = CTutorial::GetUltimateGauge();
		pTime = CTutorial::GetTime();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();
		pGauge = CGame::GetGauge();
		pSansoGauge = CGame::GetSansoGauge();
		pULTGauge = CGame::GetUltimateGauge();
		pTime = CGame::GetTime();
	}

	pPlayer->InitStatus();
	pEnemy->InitStatus();
	pPlayer->SetPosition(D3DXVECTOR3(-80.0f, 20.0f, 0.0f));
	pEnemy->SetPosition(D3DXVECTOR3(80.0f, 20.0f, 0.0f));
	pPlayer->SetState(CPlayer::STATE_JANKEN);
	pEnemy->SetState(CEnemy::STATE_JANKEN);
	m_bAttack = false;
	m_nCntAttackFlame = 0;
	m_nStartCounter = 0;
	pPlayer->SetMotionType(0, CPlayer::MOTION_NEUTRAL);
	pPlayer->SetMotionType(1, CPlayer::MOTION_NEUTRAL);
	pEnemy->SetMotionType(0, CEnemy::MOTION_NEUTRAL);
	pEnemy->SetMotionType(1, CEnemy::MOTION_NEUTRAL);
	m_aJanken[0] = JANKEN_GU_BUTI;
	m_aJanken[1] = JANKEN_GU_BUTI;
	m_bPlayerUlt = false;
	m_bEnemyUlt = false;
	m_abUlt[0] = false;
	m_abUlt[1] = false;
	m_nUltTimer = 0;
	pPlayer->SetUltDis(false);
	pEnemy->SetUltDis(false);
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != NULL)
	{
		pCamera->Init();
	}

	for (int nCntPlayer = 0; nCntPlayer < MAX_CHARACTER; nCntPlayer++)
	{
		m_aGUCounter[nCntPlayer] = 0;
		m_aCHOKICounter[nCntPlayer] = 0;
		m_abPA[nCntPlayer] = false;
	}

	pGauge->SetGaugeRightLeft(600, 600);
	pSansoGauge->SetSansoGaugeRightLeft(600, 600);
	pULTGauge->SetGaugeRightLeft(-600, -600);

	if (mode == CManager::MODE_TUTORIAL)
	{
		CTutorial::SetWinner(CTutorial::WINNER_NONE);
		CTutorial::SetHit(true);
		CManager::GetTutorial()->SetbUI(true);
	}
	else if (mode == CManager::MODE_GAME)
	{
		CGame::SetWinner(CGame::WINNER_NONE);
		CGame::SetHit(true);
		CManager::GetGame()->SetbUI(true);
	}

	pTime->SetTime(60);
}

//=============================================================================
// �K�[�h���ɍU�������Ƃ��̃m�b�N�o�b�N����
//=============================================================================
void CBattleSys::GuardKnockBack(int nAttack)
{
	// �v���C���[�̎擾
	CPlayer *pPlayer = NULL;
	// �G�l�~�[
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

	if (nAttack == 0)
	{
		//�����Ă���� �v���C���[
		switch (pPlayer->GetDirection())
		{
		case CPlayer::DIRECTION_LEFT:
			pEnemy->SetMove(D3DXVECTOR3(-GUARD_MOVE, 0.0f, 0.0f));
			break;
		case CPlayer::DIRECTION_RIGHT:
			pEnemy->SetMove(D3DXVECTOR3(GUARD_MOVE, 0.0f, 0.0f));
			break;
		}
	}
	else if (nAttack == 1)
	{
		//�����Ă���� �G�l�~�[
		switch (pEnemy->GetDirection())
		{
		case CEnemy::DIRECTION_LEFT:
			pPlayer->SetMove(D3DXVECTOR3(-GUARD_MOVE, 0.0f, 0.0f));
			break;
		case CEnemy::DIRECTION_RIGHT:
			pPlayer->SetMove(D3DXVECTOR3(GUARD_MOVE, 0.0f, 0.0f));
			break;
		}
	}
}

//=============================================================================
// ���̃��[�V��������
//=============================================================================
void CBattleSys::MotionSetYORI(int nAttack)
{
	// �v���C���[�̎擾
	CPlayer *pPlayer = NULL;
	// �G�l�~�[
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

	if (nAttack == 0)
	{
		pPlayer->SetMotionType(0, CPlayer::MOTION_TUKAMI_AGERU);
		pPlayer->SetbMotionEnd(0, false);
		pPlayer->SetMotionType(1, CPlayer::MOTION_TUKAMI_AGERU);
		pPlayer->SetbMotionEnd(1, false);
		pEnemy->SetMotionType(0, CEnemy::MOTION_TUKAMI_AGERARERU);
		pEnemy->SetbMotionEnd(0, false);
		pEnemy->SetMotionType(1, CEnemy::MOTION_TUKAMI_AGERARERU);
		pEnemy->SetbMotionEnd(1, false);
	}
	else if(nAttack == 1)
	{
		pEnemy->SetMotionType(0, CEnemy::MOTION_TUKAMI_AGERU);
		pEnemy->SetbMotionEnd(0, false);
		pEnemy->SetMotionType(1, CEnemy::MOTION_TUKAMI_AGERU);
		pEnemy->SetbMotionEnd(1, false);
		pPlayer->SetMotionType(0, CPlayer::MOTION_TUKAMI_AGERARERU);
		pPlayer->SetbMotionEnd(0, false);
		pPlayer->SetMotionType(1, CPlayer::MOTION_TUKAMI_AGERARERU);
		pPlayer->SetbMotionEnd(1, false);
	}
}

//=============================================================================
// CPU�̃o�g������
//=============================================================================
void CBattleSys::CPUBattle(CEnemy::CPUACTION CpuAction)
{
	// �v���C���[�̎擾
	CPlayer *pPlayer = NULL;
	// �G�l�~�[
	CEnemy *pEnemy = NULL;
	// �Q�[�W�̎擾
	CGauge *pGauge = NULL;
	CUltimateGauge *pULTGauge = NULL;

	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();
		pGauge = CTutorial::GetGauge();
		pULTGauge = CTutorial::GetUltimateGauge();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();
		pGauge = CGame::GetGauge();
		pULTGauge = CGame::GetUltimateGauge();
	}

	//�T�E���h���̎擾
	CSound *pSound = CManager::GetSound(0);
	D3DXVECTOR3 p1pos;
	//�m�����̈ړ���
	float fHinshiOshi = 1.0f;
	float fHinshiNage = 1.0f;

	if (pPlayer != NULL)
	{	//�v���C���[1�̈ʒu���擾
		p1pos = pPlayer->GetPosition();

		if (pPlayer->GetDying() == true)
		{
			fHinshiNage = HINSI_N_MOVE;
			fHinshiOshi = HINSI_O_MOVE;
		}
	}

	//�����Ă���� �G�l�~�[
	switch (pEnemy->GetDirection())
	{
	case CEnemy::DIRECTION_LEFT:
		switch (CpuAction)
		{
		case CEnemy::CPUACTION_TUPPARI:
			if (pEnemy->GetState() == CEnemy::STATE_NEUTRAL && m_bAttack == false)
			{
				pEnemy->SetMotionType(0, CEnemy::MOTION_TSUPPARI);
				pEnemy->SetbMotionEnd(0, true);
				pEnemy->SetMotionType(1, CEnemy::MOTION_TSUPPARI);
				pEnemy->SetbMotionEnd(1, true);
				pEnemy->SetbDash(false);
				pEnemy->SetState(CEnemy::STATE_TSUPPARI);
				pEnemy->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x - 10, p1pos.y, p1pos.z));
				m_nCntAttackFlame = TUPARI_FLAME;
				pEnemy->SetRecovery(true);
				pEnemy->SetRecoveryTime(TUPARI_RECOVERY);
				m_bAttack = true;
			}
			break;
		case CEnemy::CPUACTION_YORI:
			//���
			pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
			Battle(1, ATTACK_TYPE_YORI, D3DXVECTOR3(-YORI_MOVE, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(-YORI_MOVE, KNOCKUP_MOVE, 0.0f));
			MotionSetYORI(1);
			break;
		case CEnemy::CPUACTION_NAGE:
			//����
			pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
			Battle(1, ATTACK_TYPE_NAGE, D3DXVECTOR3(NAGE_MOVE * fHinshiNage, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pEnemy->SetMotionType(0, CEnemy::MOTION_NAGE);
			pEnemy->SetbMotionEnd(0, true);
			pEnemy->SetMotionType(1, CEnemy::MOTION_NAGE);
			pEnemy->SetbMotionEnd(1, true);
			break;
		case CEnemy::CPUACTION_OSHI:
			//����
			pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
			Battle(1, ATTACK_TYPE_OSI, D3DXVECTOR3(-OSI_MOVE * fHinshiOshi, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pEnemy->SetMotionType(0, CEnemy::MOTION_TSUPPARI);
			pEnemy->SetbMotionEnd(0, true);
			pEnemy->SetMotionType(1, CEnemy::MOTION_TSUPPARI);
			pEnemy->SetbMotionEnd(1, true);
			pEnemy->SetbDash(false);
			break;
		}
		break;
	case CEnemy::DIRECTION_RIGHT:
		switch (CpuAction)
		{
		case CEnemy::CPUACTION_TUPPARI:
			if (pEnemy->GetState() == CEnemy::STATE_NEUTRAL && m_bAttack == false)
			{
				pEnemy->SetMotionType(0, CEnemy::MOTION_TSUPPARI);
				pEnemy->SetbMotionEnd(0, true);
				pEnemy->SetMotionType(1, CEnemy::MOTION_TSUPPARI);
				pEnemy->SetbMotionEnd(1, true);
				pEnemy->SetbDash(false);

				pEnemy->SetState(CEnemy::STATE_TSUPPARI);
				pEnemy->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x + 10, p1pos.y, p1pos.z));
				m_nCntAttackFlame = TUPARI_FLAME;
				pEnemy->SetRecovery(true);
				pEnemy->SetRecoveryTime(TUPARI_RECOVERY);
				m_bAttack = true;
			}
			break;
		case CEnemy::CPUACTION_YORI:
			//���
			pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
			Battle(1, ATTACK_TYPE_YORI, D3DXVECTOR3(YORI_MOVE, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(YORI_MOVE, KNOCKUP_MOVE, 0.0f));
			MotionSetYORI(1);
			break;
		case CEnemy::CPUACTION_NAGE:
			//����
			pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
			Battle(1, ATTACK_TYPE_NAGE, D3DXVECTOR3(-NAGE_MOVE * fHinshiNage, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pEnemy->SetMotionType(0, CEnemy::MOTION_NAGE);
			pEnemy->SetbMotionEnd(0, true);
			pEnemy->SetMotionType(1, CEnemy::MOTION_NAGE);
			pEnemy->SetbMotionEnd(1, true);
			break;
		case CEnemy::CPUACTION_OSHI:
			//����
			pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
			Battle(1, ATTACK_TYPE_OSI, D3DXVECTOR3(OSI_MOVE * fHinshiOshi, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pEnemy->SetMotionType(0, CEnemy::MOTION_TSUPPARI);
			pEnemy->SetbMotionEnd(0, true);
			pEnemy->SetMotionType(1, CEnemy::MOTION_TSUPPARI);
			pEnemy->SetbMotionEnd(1, true);
			pEnemy->SetbDash(false);
			break;
		}
	}
}
