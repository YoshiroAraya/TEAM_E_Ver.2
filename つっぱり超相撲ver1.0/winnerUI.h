//=============================================================================
//
// ����UI�̏��� [winnerUI.h]
// Author : �R���֎j
//
//=============================================================================
#ifndef _WINNERUI_H_
#define _WINNERUI_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TEX_TYPE	(2)								// �e�N�X�`���̎��
#define MAX_UI (6)

//========================================
// �N���X�̒�`
//========================================
//=====================
// �I�u�W�F�N�g�N���X
//=====================
class CWinnerUI : public CScene2D
{
public:
	CWinnerUI();											// �R���X�g���N�^
	~CWinnerUI();										// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos);				// 2D�I�u�W�F�N�g����������
	void Uninit(void);							// 2D�I�u�W�F�N�g�I������
	void Update(void);							// 2D�I�u�W�F�N�g�X�V����
	void Draw(void);							// 2D�I�u�W�F�N�g�`�揈��

	D3DXVECTOR3 GetPosition(void);						// �ʒu���擾
	void SetPosition(D3DXVECTOR3 pos);					// �ʒu��ݒ�

	static CWinnerUI *Create(D3DXVECTOR3 pos);			// �I�u�W�F�N�g�̐���
	void SetDrawSyouhai(bool bDraw) { m_pSyouhai2D->SetbDraw(bDraw); };
	void SetbWinner(bool bWinner) { m_bWinner = bWinner; };
private:
	CScene2D						*m_pScene2D[MAX_UI];		// 2d�ւ̃|�C���^
	CScene2D						*m_pTyanko2D[MAX_UI];		// 2d�ւ̃|�C���^
	CScene2D						*m_pSyouhai2D;				// 2d�ւ̃|�C���^

	D3DXVECTOR3						m_Pos;						// �|���S���̈ʒu
	float							m_fRight, m_fLeft;			// ���E�̒l

	int								m_n1player;
	int								m_n2player;
	bool							m_bWinner;
};

#endif
