//=============================================================================
//
// �Q�[�W���� [scene2D.h]
// Author : ���R���
//
//=============================================================================
#ifndef _OUGIUI_H_
#define _OUGIUI_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TEX_TYPE	(2)								// �e�N�X�`���̎��
#define MAX_OUGIUI (1)


//========================================
// �N���X�̒�`
//========================================
//=====================
// �I�u�W�F�N�g�N���X
//=====================
class COugiUI : public CScene2D
{
public:
	COugiUI(int nPriority = 3, OBJTYPE objType = OBJTYPE_SCENE2D);											// �R���X�g���N�^
	~COugiUI();										// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos);	// 2D�I�u�W�F�N�g����������
	void Uninit(void);				// 2D�I�u�W�F�N�g�I������
	void Update(void);				// 2D�I�u�W�F�N�g�X�V����
	void Draw(void);				// 2D�I�u�W�F�N�g�`�揈��

	D3DXVECTOR3 GetPosition(void);		// �ʒu���擾
	static COugiUI *Create(D3DXVECTOR3 pos);		// �I�u�W�F�N�g�̐���

private:
	CScene2D		*m_pScene2D[MAX_OUGIUI];		// 2d�ւ̃|�C���^

	D3DXVECTOR3		m_Pos;						// �|���S���̈ʒu
	float			m_fRight, m_fLeft;			// ���E�̒l
};

#endif