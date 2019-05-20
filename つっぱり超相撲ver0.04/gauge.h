//=============================================================================
//
// �Q�[�W���� [scene2D.h]
// Author : ���R���
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GAUGE_NAME		"data/TEXTURE/player000.png"	// �v���C���[�̃e�N�X�`����
#define BULLET_NAME		"data/TEXTURE/bullet000.png"	// �e�̃e�N�X�`����
#define MAX_TEX_TYPE	(2)								// �e�N�X�`���̎��
#define MAX_GAUGE (2)

//========================================
// �N���X�̒�`
//========================================
//=====================
// �I�u�W�F�N�g�N���X
//=====================
class CGauge : public CScene2D
{
public:
	CGauge(int nPriority = 3, OBJTYPE objType = OBJTYPE_SCENE2D);											// �R���X�g���N�^
	~CGauge();										// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos);				// 2D�I�u�W�F�N�g����������
	void Uninit(void);							// 2D�I�u�W�F�N�g�I������
	void Update(void);							// 2D�I�u�W�F�N�g�X�V����
	void Draw(void);							// 2D�I�u�W�F�N�g�`�揈��

	D3DXVECTOR3 GetPosition(void);						// �ʒu���擾
	void SetPosition(D3DXVECTOR3 pos);					// �ʒu��ݒ�

	static CGauge *Create(D3DXVECTOR3 pos);			// �I�u�W�F�N�g�̐���
	static HRESULT Load(void);
	static void UnLoad(void);

	void SetGaugeRightLeft(float fRight, float fLeft);

private:
	static LPDIRECT3DTEXTURE9		m_pTexture;					// �e�N�X�`���ւ̃|�C���^

	CScene2D						*m_pScene2D[MAX_GAUGE];		// 2d�ւ̃|�C���^

	D3DXVECTOR3						m_Pos;						// �|���S���̈ʒu
	float							m_fRight, m_fLeft;			// ���E�̒l
};

#endif