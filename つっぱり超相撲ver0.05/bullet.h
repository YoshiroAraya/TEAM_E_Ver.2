//=============================================================================
//
// �e�̏��� [bullet.h]
// Author : ���R���
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
#include "billboard.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_BULLET	(256)	// �e�̐�
#define BULLET_TEXTURENAME		 "data\\TEXTURE\\bullet000.png"			//�e�N�X�`���̃t�@�C����
#define TEX_POS_X_INIT			(1.0f)									//�e�N�X�`�����WU�̏����ʒu
#define TEX_POS_Y_INIT			(1.0f)									//�e�N�X�`�����WV�̏����ʒu
#define TEX_LEFT				(0.0f)									//�e�N�X�`�����WU��
#define TEX_RIGHT				(1.0f)									//�e�N�X�`�����WU�E
#define TEX_TOP					(0.0f)									//�e�N�X�`�����WV��
#define TEX_BOT					(1.0f)									//�e�N�X�`�����WV��

//========================================
// �N���X�̒�`
//========================================
//=====================
// �v���C���[�N���X
//=====================
class CBullet : public CBillboard
{
public:
	CBullet();	// �R���X�g���N�^
	~CBullet();	// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos);	// �v���C���[����������
	void Uninit(void);	// �v���C���[�I������
	void Update(void);	// �v���C���[�X�V����
	void Draw(void);	// �v���C���[�`�揈��

	static HRESULT Load(void);	// �e�N�X�`���ǂݍ���
	static void Unload(void);	// �e�N�X�`�����

	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col);	// �I�u�W�F�N�g�̐���

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_pos;					// �ʒu
	D3DXVECTOR3 m_move;					// �ړ���
	D3DXCOLOR m_col;							// �F
	int m_nLife;
};

#endif