//=============================================================================
//
// �A�j���[�V�����̏��� [animation.h]
// Author : Yamashita
//
//=============================================================================
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "main.h"
#include "scene3D.h"

//�}�N����`
#define MAX_COUNTER (5)
#define MAX_PATTERN (4)
//#define EXPLOSION_UV_U (1.0f)
//#define EXPLOSION_UV_V (0.25f)

class CScene;

//�G�N�X�v���[�W�����N���X�i�V�[��2D�h���j
class CAnimation : public CScene3D
{
public:
	CAnimation();
	~CAnimation();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXCOLOR col, float fHeight, float fWidth,float fUV_U, float fUV_V, int nCntSpeed, int nTotalAnim,int nRoop,int nDrawType);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void UpdateAnim();

	//�ÓI�����o�֐�
	static CAnimation *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fWidth,float fUV_U,float fUV_V,int nCntSpeed,int nTotalAnim,int nRoop,int nDrawType);

private:
	//	D3DXVECTOR3 m_pos;							//�ʒu
	D3DXCOLOR m_col;								//�F
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^

	int m_nCounterAnim;								//�A�j���[�V�����J�E���^�[
	int m_nPatternAnim;								//�A�j���[�V�����p�^�[��NO
	D3DXVECTOR3 m_pos;								//�ʒu
	int m_nCntSpeed;								//�A�j���[�V�����Đ��X�s�[�h
	int m_nTotalAnim;								//�A�j���[�V�����̍��v����
	float m_fUV_U;									//�e�N�X�`��UV(U)
	float m_fUV_V;									//�e�N�X�`��UV(V)
	bool m_bUse;									//���[�v�Đ������Ă��邩���Ă��Ȃ���
	int  m_nRoop;									//���[�v�Đ����邩���Ȃ���
	int m_nLife;									//�e�N�X�`���̎���
	int m_nDrawType;								//�`��^�C�v

};
#endif
