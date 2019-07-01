//=============================================================================
//
// �L�����N�^�[�̈ړ����� [characterMove.h]
// Author : ���R���
//
//=============================================================================
#ifndef _CHARACTERMOVE_H_
#define _CHARACTERMOVE_H_

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
class CCharacterMove
{
public:
	typedef enum
	{
		MOVESTATE_RIGHT = 0,
		MOVESTATE_LEFT,
	}MOVESTATE;

	CCharacterMove();	// �R���X�g���N�^
	~CCharacterMove();	// �f�X�g���N�^

	static CCharacterMove *Create();	// �I�u�W�F�N�g�̐���

	HRESULT Init();		// �o�g���V�X�e������������
	void Uninit(void);	// �o�g���V�X�e���I������

	D3DXVECTOR3 MoveLeft(D3DXVECTOR3 move, float fSpeed);	// �E�ɐi��
	D3DXVECTOR3 MoveRight(D3DXVECTOR3 move, float fSpeed);	// ���ɐi��
	bool CharaTurn(D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, float fRot, float fLength);

private:
	D3DXVECTOR3				m_move;			// �ړ���
	D3DXVECTOR3				m_pos;			// �ړ���
	D3DXVECTOR3				m_rot;			// �ړ���
	float					m_fLength;		// �ړ���
	//float					m_fRot;
};
#endif