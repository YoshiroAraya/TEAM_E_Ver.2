//=============================================================================
//
// �G�t�F�N�g���� [effect3D.h]
// Author :  �R���֎j
//
//=============================================================================
#ifndef _EFFECT3D_H_
#define _EFFECT_H_

#include "main.h"
#include "scene3D.h"

//*********************************************************************
//�r���{�[�h�N���X�̒�`
//*********************************************************************
class CEffect3D : public CScene3D //�h���N���X
{
public:
	typedef enum
	{
		EFFECTTYPE_SALT = 0,
		EFFECTTYPE_MONEY,
		EFFECTTYPE_MAX
	}EFFECTTYPE;

	CEffect3D();
	~CEffect3D();
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEffect3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
		float fWidth, float fHeight, int nNumMax, int nLife, int TexType);

	void UpdateMoney(void);
	void UpdateSalt(void);
	void UpdateConfetti(void);
	void UpdateUI(void);
private:
	//�����o�ϐ�
	D3DXVECTOR3					m_pos;						// �ʒu
	D3DXVECTOR3					m_move;						// �ړ���
	D3DXVECTOR3					m_rot;						// ����
	D3DXVECTOR3					m_posold;					// �O��̈ʒu
	float						m_fWidth, m_fHeight;		// �� ����
	D3DXMATRIX					m_mtrxWorld;				// ���[���h�}�g���b�N�X
	D3DXCOLOR					m_Col;						// �J���[
	int							m_nNumMax;					// �G�t�F�N�g�̃|���S����
	int							m_nLife;					// �G�t�F�N�g��������܂ł̎���
	float						m_fAlpha;					// �G�t�F�N�g�̓����x
	int							m_nCntTimer;				// �^�C�}�[
	int							m_nTexType;
	EFFECTTYPE					m_EffectType;
	int							m_DrawType;
};


#endif
