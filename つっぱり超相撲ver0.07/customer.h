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

	CCustomer();	// �R���X�g���N�^
	~CCustomer();	// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// �q����������
	void Uninit(void);				// �q�I������
	void Update(void);				// �q�X�V����
	void Draw(void);				// �q�`�揈��

	static HRESULT LoadModel(void);	// ���f���ǂݍ���
	static void UnloadModel(void);	// ���f���e�N�X�`�����
	static HRESULT LoadMat(void);	// �}�e���A���ǂݍ���
	static void UnloadMat(void);	// �}�e���A�����

	static CCustomer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);	// �I�u�W�F�N�g�̐���

private:
	static LPD3DXMESH		m_pMesh[MAX_CUSTOMERTYPE];		// ���b�V�����i���_���j�ւ̃|�C���^
	static LPD3DXBUFFER		m_pBuffMat[MAX_CUSTOMERTYPE];		// �}�e���A�����ւ̃|�C���^
	static DWORD			m_nNumMat[MAX_CUSTOMERTYPE];		// �}�e���A�����̐�
	static LPDIRECT3DTEXTURE9		*m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X
	CUSTOMER m_nType;
};
#endif
