//=============================================================================
//
// �l���I������ [numPlayer.h]
// Author : ���R���
//
//=============================================================================
#ifndef _NUMPLAYER_H_
#define _NUMPLAYER_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// �O���錾
//=============================================================================
class CScene2D;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_NUMPLAYER			(3)

//=========================
// �l���I���N���X
//=========================
class CNumPlayer : public CScene
{
public:
	typedef enum
	{
		MODE_1P = 0,
		MODE_2P,
		MODE_TUTORIAL,
		MODE_MAX
	} MODE;

	typedef enum
	{
		SELECT_NONE = 0,	// �I�΂�Ă��Ȃ����
		SELECT_SELECT,		// �I�΂�Ă�����
		SELECT_MAX
	} SELECT;

	CNumPlayer(int nPriority = 7, OBJTYPE objType = OBJTYPE_UI);									// �R���X�g���N�^
	~CNumPlayer();													// �f�X�g���N�^

	static CNumPlayer *Create(D3DXVECTOR3 pos);								// �l���I���𐶐�

	HRESULT Init(D3DXVECTOR3 pos);	// �l���I������������
	void Uninit(void);											// �l���I���I������
	void Update(void);											// �l���I���X�V����
	void Draw(void);											// �l���I��`�揈��
	static	MODE GetMode(void);									// �l���I�����擾
	int GetSelect(void);
	static bool GetDecision(void) { return m_bDecision; }

private:
	CScene2D *m_apScene2D[MAX_NUMPLAYER];
	SELECT m_aSelect[MAX_NUMPLAYER];
	D3DXCOLOR m_aCol[MAX_NUMPLAYER];
	static MODE m_Mode;
	int	m_nSelect;
	static bool m_bDecision;
};
#endif