//=============================================================================
//
// 3D�I�u�W�F�N�g���� [customer.h]
// Author : �ڍ��@������
//
//=============================================================================
#ifndef _3DOBJECT_H_
#define _3DOBJECT_H_

#include "main.h"
#include "sceneX.h"

//========================================
// �}�N����`
//========================================
#define MAX_3DOBJECTTYPE (1) //�I�u�W�F�N�g�̎��

//========================================
// �N���X�̒�`
//========================================
//=====================
// �I�u�W�F�N�g�N���X
//=====================
class C3DObject : public CSceneX
{
public:
	typedef enum
	{//�I�u�W�F�N�g�̎��
		OBJECTTYPE_TROPHY = 0,
	}OBJECTTYPE;

	C3DObject();	// �R���X�g���N�^
	~C3DObject();	// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// �I�u�W�F�N�g����������
	void Uninit(void);				// �I�u�W�F�N�g�I������
	void Update(void);				// �I�u�W�F�N�g�X�V����
	void Draw(void);				// �I�u�W�F�N�g�`�揈��

	static C3DObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);	// �I�u�W�F�N�g�̐���

private:
	static LPDIRECT3DTEXTURE9		*m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X
	OBJECTTYPE m_nType;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rot;
	int m_nLife;
};
#endif
