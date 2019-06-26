//=============================================================================
//
// �j���[�X�̔w�i���� [newsBG.h]
// Author : ���R���
//
//=============================================================================
#ifndef _NEWSBG_H_
#define _NEWSBG_H_

#include "scene3D.h"

//========================================
// �O���錾
//========================================
class CScene;

//========================================
// �N���X�̒�`
//========================================

//=====================
// �I�u�W�F�N�g�N���X
//=====================


//�e�̃N���X�i�V�[��3D�̔h���N���X�j
class CNewsBG : public CScene3D
{
public:
	CNewsBG();										//�R���X�g���N�^
	~CNewsBG();										//�f�X�g���N�^

	HRESULT Init(void);								//����������
	void Uninit(void);								//�I������
	void Update(void);								//�X�V����
	void Draw(void);								//�`�揈��

													//�ÓI�����o�֐�
	static CNewsBG *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth);

	//void SetLifeBar(int nLife);
private:
	LPDIRECT3DVERTEXBUFFER9         m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3				m_pos;					// �|���S���̈ʒu
	D3DXVECTOR3				m_rot;					// �|���S���̉�]
	D3DXVECTOR3				m_move;					// �|���S���̈ړ���
	D3DXCOLOR				m_col;
	LPDIRECT3DTEXTURE9		m_pTexture;
	D3DXMATRIX				m_mtxWorld;				//���[���h�}�g���b�N�X

	int m_nType;
	float m_fCol;
	int m_nCnt;
	float m_fWidth;
	float m_fHeight;
	float m_fSize;
	bool m_bDestroy;
};
#endif