#pragma once
//=============================================================================
//
// �ǂ̏��� [wall.h]
// Author : �R���֎j
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "scene3D.h"

class CScene;
//�e�̃N���X�i�V�[��3D�̔h���N���X�j
class CWall : public CScene3D
{
public:

	CWall();										//�R���X�g���N�^
	~CWall();										//�f�X�g���N�^

	HRESULT Init(void);								//����������
	void Uninit(void);								//�I������
	void Update(void);								//�X�V����
	void Draw(void);								//�`�揈��

	//�ÓI�����o�֐�
	static CWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth);

	static HRESULT Load(void);						//�e�N�X�`���ǂݍ���
	static void Unload(void);

	//void SetLifeBar(int nLife);
private:
	static LPDIRECT3DTEXTURE9		m_pTextures;	// �e�N�X�`���ւ̃|�C���^
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