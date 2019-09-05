//=============================================================================
//
// �o�g���V�X�e������ [battlesystem.h]
// Author : �ڍ� ������
//
//=============================================================================
#ifndef _BATTLESYSTEM_H_
#define _BATTLESYSTEM_H_

#include "main.h"
#include "enemy.h"
//========================================
// �}�N����`
//========================================
#define MAX_CHARACTER (2)

//========================================
// �N���X�̒�`
//========================================
//=====================
// �o�g���V�X�e���N���X
//=====================
class CBattleSys
{
public:
	//3������
	typedef enum
	{
		JANKEN_GU_BUTI = 0,
		JANKEN_CHOKI_TUPPA,
		JANKEN_PA_KAWASI,
	}JANKEN;

	//�U���̎��
	typedef enum
	{
		ATTACK_TYPE_YORI = 0,
		ATTACK_TYPE_OSI,
		ATTACK_TYPE_NAGE,
		ATTACK_TYPE_TUPPARI,
		ATTACK_TYPE_COUNTER,
	}ATTACK_TYPE;

	//�ǂ������U���\��
	typedef enum
	{
		ATTACK_TURN_NORMAL = 0,
		ATTACK_TURN_PLAYER1,
		ATTACK_TURN_PLAYER2,
	}ATTACK_TURN;

	CBattleSys();	// �R���X�g���N�^
	~CBattleSys();	// �f�X�g���N�^

	HRESULT Init();		// �o�g���V�X�e������������
	void Uninit(void);	// �o�g���V�X�e���I������
	void Update(void);	// �o�g���V�X�e���X�V����

	void Recovery(void);	//�d�������܂Ƃ�
	void Battle(int nPlayer, ATTACK_TYPE AttackType, D3DXVECTOR3 P1move, D3DXVECTOR3 P2move);
	void PushJudge(void);	//�A�Ŕ���
	void P1Attack(void);
	void P2Attack(void);
	void CounterAttack(void);
	void ResetBattle(void);
	void GuardKnockBack(int nAttack);
	void MotionSetYORI(int nAttack);
	void UltDamage(void);
	void PlusCntPushP2(int nCnt) { m_nCntPushP2 += nCnt; };
	bool GetUlt(int nCharacter) { return m_abUlt[nCharacter]; }
	bool GetEnemyUlt(void) { return m_bEnemyUlt; }
	bool GetPlayerUlt(void) { return m_bPlayerUlt; }
	void CPUBattle(CEnemy::CPUACTION CpuAction);
	void CustomerReset(void);
	ATTACK_TURN GetAttackTurn(void) { return m_AttackTurn; };

	static CBattleSys *Create();	// �I�u�W�F�N�g�̐���

	void Operation(void);

private:
	int		m_nCntAttackFlame;			//�U���̃t���[����
	int		m_nImpossibleFlame;		//����s����
	int		m_aGUCounter[MAX_CHARACTER];
	int		m_aCHOKICounter[MAX_CHARACTER];
	int		m_nUltTimer;
	bool	m_abPA[MAX_CHARACTER];
	bool	m_bAttack;				//�U�����Ă��邩�ǂ���
	bool	m_bPlayerUlt;
	bool	m_bEnemyUlt;
	bool	m_abUlt[MAX_CHARACTER];
	int		m_nStartCounter;
	int		m_nCntPushP1, m_nCntPushP2;	//�ǂ̂��炢�A�ł�����
	int		m_nFlamePush;
	float	m_fMoveDying[2] = { 1.0f,1.0f };	//�m�����̈ړ���
	JANKEN	m_aJanken[MAX_CHARACTER];
	ATTACK_TURN m_AttackTurn;
	bool	m_bSound;
	bool	m_bSound2;

	int		m_nCntSound;

	int		m_nCntUltTimer;
};
#endif