//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author :  Meguro Mikiya
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "billboard.h"

//*********************************************************************
//�r���{�[�h�N���X�̒�`
//*********************************************************************
class CEffect : public CBillboard //�h���N���X
{
public:
	typedef enum
	{
		EFFECTTYPE_00 = 0,
		EFFECTTYPE_01,
		EFFECTTYPE_MAX
	}EFFECTTYPE;

	CEffect();
	~CEffect();
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move,D3DXCOLOR col,
							float fWidth, float fHeight,int nNumMax,int nLife, int TexType);
	
private:
	//�����o�ϐ�
	D3DXVECTOR3					m_pos;						// �ʒu
	D3DXVECTOR3					m_move;						// �ړ���
	D3DXVECTOR3					m_posold;					// �O��̈ʒu
	float						m_fWidth, m_fHeight;		// �� ����
	D3DXMATRIX					m_mtrxWorld;				// ���[���h�}�g���b�N�X
	D3DXCOLOR					m_Col;						// �J���[
	int							m_nNumMax;					// �G�t�F�N�g�̃|���S����
	int							m_nLife;					// �G�t�F�N�g��������܂ł̎���
	float						m_fAlpha;					// �G�t�F�N�g�̓����x
	int							m_nCntTimer;				// �^�C�}�[
	CBillboard					*m_pBillBoard;

	int					m_nTexType;
	EFFECTTYPE					m_EffectType;
};
#endif
