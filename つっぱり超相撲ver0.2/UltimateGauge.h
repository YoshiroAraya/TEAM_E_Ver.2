//=============================================================================
//
// �Q�[�W���� [UltimateGauge.h]
// Author : �ڍ�������
//
//=============================================================================
#ifndef _ULTIMATEGAUGE_H_
#define _ULTIMATEGAUGE_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TEX_TYPE	(2)								// �e�N�X�`���̎��
#define MAX_GAUGE (2)
#define NUM_PLAYER (2)

//========================================
// �N���X�̒�`
//========================================
//=====================
// �I�u�W�F�N�g�N���X
//=====================
class CUltimateGauge : public CScene2D
{
public:
	CUltimateGauge(int nPriority = 3, OBJTYPE objType = OBJTYPE_SCENE2D);											// �R���X�g���N�^
	~CUltimateGauge();										// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos);	// 2D�I�u�W�F�N�g����������
	void Uninit(void);				// 2D�I�u�W�F�N�g�I������
	void Update(void);				// 2D�I�u�W�F�N�g�X�V����
	void Draw(void);				// 2D�I�u�W�F�N�g�`�揈��

	D3DXVECTOR3 GetPosition(void);		// �ʒu���擾
	void SetPosition(D3DXVECTOR3 pos);	// �ʒu��ݒ�

	bool GetUlt(int nPlayer) { return m_bUlt[nPlayer]; }	// �K�E�t���O���擾

	static CUltimateGauge *Create(D3DXVECTOR3 pos);		// �I�u�W�F�N�g�̐���

	void SetGaugeRightLeft(float fRight, float fLeft);	// �Q�[�W�̒l��ݒ�
	float GetGaugeRight(void) { return m_fRight; };	// �Q�[�W�̒l���擾
	float GetGaugeLeft(void) { return m_fLeft; };	// �Q�[�W�̒l���擾

private:
	CScene2D		*m_pScene2D[MAX_GAUGE];		// 2d�ւ̃|�C���^

	D3DXVECTOR3		m_Pos;						// �|���S���̈ʒu
	float			m_fRight, m_fLeft;			// ���E�̒l
	bool			m_bUlt[NUM_PLAYER];			// �K�E�t���O
};

#endif