//=============================================================================
//
// �|�[�Y�̏��� [jankenUI.h]
// Author : ���R���
//
//=============================================================================
#ifndef _JANKENUI_H_
#define _JANKENUI_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// �O���錾
//=============================================================================
class CScene2D;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_JANKENUI_TEXTURE	(6)
#define MAX_JANKENUI			(3)

//=========================
// �|�[�Y�N���X
//=========================
class CJankenUI : public CScene
{
public:
	typedef enum
	{
		MODE_GU = 0,		// �O�[
		MODE_CHOKI,		// �`���L
		MODE_PA,		// �p�[
		MODE_MAX
	} MODE;

	typedef enum
	{
		SELECT_NONE = 0,	// �I�΂�Ă��Ȃ����
		SELECT_SELECT,		// �I�΂�Ă�����
		SELECT_MAX
	} SELECT;

	CJankenUI(int nPriority = 2, OBJTYPE objType = OBJTYPE_UI);									// �R���X�g���N�^
	~CJankenUI();													// �f�X�g���N�^

	static CJankenUI *Create(D3DXVECTOR3 pos);								// �|�[�Y�𐶐�

	HRESULT Init(D3DXVECTOR3 pos);	// �|�[�Y����������
	void Uninit(void);											// �|�[�Y�I������
	void Update(void);											// �|�[�Y�X�V����
	void Draw(void);											// �|�[�Y�`�揈��
	static	MODE GetMode(void);									// �|�[�Y���擾
	int GetSelect(void);

private:
	CScene2D *m_apScene2D[MAX_JANKENUI_TEXTURE];
	SELECT m_aSelect[MAX_JANKENUI];
	D3DXCOLOR m_aCol[MAX_JANKENUI];
	static MODE m_Mode;
	int	m_nSelect;
};
#endif