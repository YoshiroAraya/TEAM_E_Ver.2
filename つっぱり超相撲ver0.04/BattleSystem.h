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
	//����
	typedef enum
	{
		JANKEN_GU = 0,
		JANKEN_CHOKI,
		JANKEN_PA,
	}JANKEN;

	CBattleSys();	// �R���X�g���N�^
	~CBattleSys();	// �f�X�g���N�^

	HRESULT Init();		// �o�g���V�X�e������������
	void Uninit(void);	// �o�g���V�X�e���I������
	void Update(void);	// �o�g���V�X�e���X�V����

	void Recovery(void);	//�d�������܂Ƃ�
	static CBattleSys *Create();	// �I�u�W�F�N�g�̐���

private:
	int		m_nCntFlame;			//�U���̃t���[����
	int		m_nImpossibleFlame;		//����s����
	int		m_aGUCounter[MAX_CHARACTER];
	int		m_aCHOKICounter[MAX_CHARACTER];
	bool	m_abPA[MAX_CHARACTER];
	bool	m_bAttack;				//�U�����Ă��邩�ǂ���
	JANKEN	m_aJanken[MAX_CHARACTER];
};
#endif