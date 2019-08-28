//=============================================================================
//
// ����e�N�X�`���̏��� [charaSelect.h]
// Author : ���R���
//
//=============================================================================
#ifndef _CHARASELECT_H_
#define _CHARASELECT_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// �O���錾
//=============================================================================
class CScene2D;

//*****************************************************************************
// �}�N����`
//*****************************************************************************	
#define MAX_SELECT (2)

//=========================
// �|�[�Y�N���X
//=========================
class CCharaSelect : public CScene
{
public:
	CCharaSelect(int nPriority = SELECT_PRIORITY, OBJTYPE objType = OBJTYPE_UI);									// �R���X�g���N�^
	~CCharaSelect();								// �f�X�g���N�^

	static CCharaSelect *Create(D3DXVECTOR3 pos);	// ����e�N�X�`���𐶐�

	HRESULT Init(D3DXVECTOR3 pos);					// ����e�N�X�`������������
	void Uninit(void);								// ����e�N�X�`���I������
	void Update(void);								// ����e�N�X�`���X�V����
	void Draw(void);								// ����e�N�X�`���`�揈��

private:
	CScene2D *m_apScene2D[MAX_SELECT];
	D3DXCOLOR m_Col;
	int	m_nSelect;
	float m_fScale;
	bool m_bReset;
};
#endif