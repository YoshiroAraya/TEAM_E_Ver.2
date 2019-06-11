//=============================================================================
//
// ���ς菈�� [tuppari.h]
// Author : �ڍ� ������
//
//=============================================================================
#ifndef _TUPPARI_H_
#define _TUPPARI_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//========================================
// �N���X�̒�`
//========================================

//=====================
// �I�u�W�F�N�g�N���X
//=====================
class CTuppari : public CScene
{
public:
	CTuppari(int nPriority = SCENEX_PRIORITY, OBJTYPE objType = OBJTYPE_TUPPARI);					// �R���X�g���N�^
	~CTuppari();									// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos);				// 3D�I�u�W�F�N�g����������
	void Uninit(void);							// 3D�I�u�W�F�N�g�I������
	void Update(void);							// 3D�I�u�W�F�N�g�X�V����
	void Draw(void);							// 3D�I�u�W�F�N�g�`�揈��

	static CTuppari *Create(D3DXVECTOR3 pos);	// �I�u�W�F�N�g�̐���

	D3DXVECTOR3 GetPosition(void);						// �ʒu�̎擾
	void SetPosition(D3DXVECTOR3 pos);					// �ʒu�̐ݒ�

	D3DXVECTOR3 GetRot(void);						// �����̎擾
	void SetRot(D3DXVECTOR3 rot);					// �����̐ݒ�

	D3DXVECTOR3 GetVtxMax(void);						// �ő�l�̎擾
	void SetVtxMax(D3DXVECTOR3 VtxMax);					// �ő�l�̐ݒ�

	D3DXVECTOR3 GetVtxMin(void);						// �ŏ��l�̎擾
	void SetVtxMin(D3DXVECTOR3 VtxMin);					// �ŏ��l�̐ݒ�

	void SetVtx(void);																// ���_���W�̐ݒ�
	bool Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 radius);	// �����蔻��

private:
	D3DXMATRIX				m_mtxWorld;			// ���[���h�}�g���b�N�X
	DWORD					m_nNumMat;			// �}�e���A�����̐�
	D3DXVECTOR3				m_VtxMin, m_VtxMax;	// ���f���̍ŏ��l�A�ő�l
	D3DXVECTOR3				m_pos;				// �|���S���̈ʒu
	D3DXVECTOR3				m_rot;				// ������x�N�g��
};

#endif