#pragma once
//=============================================================================
//
// ���C������ [dohyocircle.h]
// Author : 
//
//=============================================================================
#ifndef _DOHYOCIRCLE_H_
#define _DOHYOCIRCLE_H_

#include "scene3D.h"

class CScene;
//�e�̃N���X�i�V�[��3D�̔h���N���X�j
class CDohyoCircle : public CScene3D
{
public:

	CDohyoCircle();										//�R���X�g���N�^
	~CDohyoCircle();										//�f�X�g���N�^

	HRESULT Init(void);								//����������
	void Uninit(void);								//�I������
	void Update(void);								//�X�V����
	void Draw(void);								//�`�揈��

	//�ÓI�����o�֐�
	static CDohyoCircle *Create(D3DXVECTOR3 pos, float fWhidth, float fHeight);

	//void SetLifeBar(int nLife);
private:
	LPDIRECT3DVERTEXBUFFER9         m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^

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

	bool m_bDestroy;
};
#endif