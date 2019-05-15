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

//========================================
// �N���X�̒�`
//========================================
//=====================
// �o�g���V�X�e���N���X
//=====================
class CBattleSys
{
public:
	CBattleSys();	// �R���X�g���N�^
	~CBattleSys();	// �f�X�g���N�^

	HRESULT Init();		// �o�g���V�X�e������������
	void Uninit(void);	// �o�g���V�X�e���I������
	void Update(void);	// �o�g���V�X�e���X�V����

	static CBattleSys *Create();	// �I�u�W�F�N�g�̐���

private:
	int		m_nCntFlame;			//�U���̃t���[����
	int		m_nImpossibleFlame;		//����s����
	bool	m_bAttack;				//�U�����Ă��邩�ǂ���
};
#endif