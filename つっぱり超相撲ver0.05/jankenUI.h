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
#define PAUSE_NAME				"data/TEXTURE/PAUSE/pause100.png"		// �|�[�Y�̃e�N�X�`����
#define PAUSET_TEXTURE_NAME0	"data/TEXTURE/PAUSE/pause200.png"	// �|�[�Y�Z���N�g0�̃e�N�X�`����
#define PAUSET_TEXTURE_NAME1	"data/TEXTURE/PAUSE/pause201.png"	// �|�[�Y�Z���N�g1�̃e�N�X�`����
#define PAUSET_TEXTURE_NAME2	"data/TEXTURE/PAUSE/pause202.png"	// �|�[�Y�Z���N�g2�̃e�N�X�`����
#define MAX_PAUSE_TEXTURE		(6)								// �|�[�Y�Z���N�g�̃e�N�X�`����
#define MAX_PAUSE				(3)								// �|�[�Y�Z���N�g�̐�

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

	CJankenUI(int nPriority = 7, OBJTYPE objType = OBJTYPE_UI);									// �R���X�g���N�^
	~CJankenUI();													// �f�X�g���N�^

	static HRESULT Load(void);									// �e�N�X�`���ǂݍ���
	static void Unload(void);									// �e�N�X�`�����

	static CJankenUI *Create(D3DXVECTOR3 pos);								// �|�[�Y�𐶐�

	HRESULT Init(D3DXVECTOR3 pos);	// �|�[�Y����������
	void Uninit(void);											// �|�[�Y�I������
	void Update(void);											// �|�[�Y�X�V����
	void Draw(void);											// �|�[�Y�`�揈��
	static	MODE GetMode(void);									// �|�[�Y���擾
	int GetSelect(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_PAUSE];
	CScene2D *m_apScene2D[MAX_PAUSE_TEXTURE];
	SELECT m_aSelect[MAX_PAUSE];
	D3DXCOLOR m_aCol[MAX_PAUSE];
	static MODE m_Mode;
	int	m_nSelect;
};
#endif