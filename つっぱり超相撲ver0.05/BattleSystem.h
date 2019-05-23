//=============================================================================
//
// �o�g���V�X�e������ [battlesystem.h]
// Author : �ڍ� ������
//
//=============================================================================
#ifndef _BATTLESYSTEM_H_
#define _BATTLESYSTEM_H_

#include "main.h"

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
		JANKEN_GU = 0,
		JANKEN_CHOKI,
		JANKEN_PA,
	}JANKEN;

	//�U���̎��
	typedef enum
	{
		ATTACK_TYPE_YORI = 0,
		ATTACK_TYPE_OSI,
		ATTACK_TYPE_NAGE,
	}ATTACK_TYPE;

	CBattleSys();	// �R���X�g���N�^
	~CBattleSys();	// �f�X�g���N�^

	HRESULT Init();		// �o�g���V�X�e������������
	void Uninit(void);	// �o�g���V�X�e���I������
	void Update(void);	// �o�g���V�X�e���X�V����

	void Recovery(void);	//�d�������܂Ƃ�
	void Battle(int nPlayer, ATTACK_TYPE AttackType);

	static CBattleSys *Create();	// �I�u�W�F�N�g�̐���

	void Operation(void);
private:
	int		m_nCntFlame;			//�U���̃t���[����
	int		m_nImpossibleFlame;		//����s����
	int		m_aGUCounter[MAX_CHARACTER];
	int		m_aCHOKICounter[MAX_CHARACTER];
	bool	m_abPA[MAX_CHARACTER];
	bool	m_bAttack;				//�U�����Ă��邩�ǂ���
	int		m_nStartCounter;
	JANKEN	m_aJanken[MAX_CHARACTER];
};
#endif