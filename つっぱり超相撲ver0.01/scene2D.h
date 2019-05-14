//=============================================================================
//
// �V�[������ [scene2D.h]
// Author : ���R���
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_NAME		"data/TEXTURE/.png"	// �v���C���[�̃e�N�X�`����
#define BULLET_NAME		"data/TEXTURE/bullet000.png"	// �e�̃e�N�X�`����
#define MAX_TEX_TYPE	(2)								// �e�N�X�`���̎��

//========================================
// �N���X�̒�`
//========================================
//=====================
// �I�u�W�F�N�g�N���X
//=====================
class CScene2D : public CScene
{
public:
	CScene2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_SCENE2D);											// �R���X�g���N�^
	~CScene2D();										// �f�X�g���N�^

	virtual HRESULT Init(D3DXVECTOR3 pos);				// 2D�I�u�W�F�N�g����������
	virtual void Uninit(void);							// 2D�I�u�W�F�N�g�I������
	virtual void Update(void);							// 2D�I�u�W�F�N�g�X�V����
	virtual void Draw(void);							// 2D�I�u�W�F�N�g�`�揈��

	D3DXVECTOR3 GetPosition(void);						// �ʒu���擾
	void SetPosition(D3DXVECTOR3 pos);					// �ʒu��ݒ�

	static CScene2D *Create(D3DXVECTOR3 pos);			// �I�u�W�F�N�g�̐���

private:
	LPDIRECT3DTEXTURE9		m_pTexture;					// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3				m_Pos;						// �|���S���̈ʒu
};

#endif