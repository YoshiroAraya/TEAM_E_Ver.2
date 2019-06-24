//=============================================================================
//
// �y�U���� [customer.h]
// Author : �R���֎j
//
//=============================================================================
#ifndef _CUSTOMER_H_
#define _CUSTOMER_H_

#include "main.h"
#include "sceneX.h"

//========================================
// �}�N����`
//========================================
#define MAX_CUSTOMERTYPE (4) //�q�̎��

//========================================
// �N���X�̒�`
//========================================
//=====================
// �y�U�N���X
//=====================
class CCustomer : public CSceneX
{
public:
	typedef enum
	{//�q�̎��
		CUSTOMERTYPE_00 = 0,
		CUSTOMERTYPE_01,
		CUSTOMERTYPE_02,
		CUSTOMERTYPE_03,
	}CUSTOMER;

	typedef enum
	{//�q�̎��
		POSITION = 0,
		POSITION_RIGHT,
	}CUSTOMER_POS;

	CCustomer();	// �R���X�g���N�^
	~CCustomer();	// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// �q����������
	void Uninit(void);				// �q�I������
	void Update(void);				// �q�X�V����
	void Draw(void);				// �q�`�揈��

	static CCustomer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, CUSTOMER_POS customerpos);	// �I�u�W�F�N�g�̐���

private:
	static LPDIRECT3DTEXTURE9		*m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X
	CUSTOMER m_nType;
	CUSTOMER_POS m_CustomerPos;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rot;
	int m_nLife;
};
#endif
