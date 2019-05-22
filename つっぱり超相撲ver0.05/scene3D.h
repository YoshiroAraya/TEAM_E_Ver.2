//=============================================================================
//
// 3D�|���S������ [scene3D.h]
// Author : ���R���
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GROUND_TEXTURE_NAME			"data\\TEXTURE\\field.jpg"	// �n�ʂ̃e�N�X�`����
#define GROUND_SIZE					(130.0f)							// �n�ʂ̑傫��
#define NUM_VTX						(4)								// ���_�̐�
//========================================
// �N���X�̒�`
//========================================
//=====================
// �I�u�W�F�N�g�N���X
//=====================
class CScene3D : public CScene
{
public:
	CScene3D(int nPriority = 3, OBJTYPE objType = OBJTYPE_SCENE3D);											// �R���X�g���N�^
	~CScene3D();										// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos);				// 3D�I�u�W�F�N�g����������
	void Uninit(void);							// 3D�I�u�W�F�N�g�I������
	void Update(void);							// 3D�I�u�W�F�N�g�X�V����
	void Draw(void);							// 3D�I�u�W�F�N�g�`�揈��

	D3DXVECTOR3 GetNor(int nIdx);				// �@�����擾

	float GetHeight(D3DXVECTOR3 pos);			// �����̎擾

	static CScene3D *Create(D3DXVECTOR3 pos);			// �I�u�W�F�N�g�̐���
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	void SetSize(float fHeight,float fWidth);
	void SetRot(D3DXVECTOR3 rot);
private:
	LPDIRECT3DTEXTURE9		m_pTexture;					// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3				m_aNor[NUM_VTX];					// �@��
	D3DXVECTOR3				m_aVec[NUM_VTX];					// �x�N�g��
	D3DXVECTOR3				m_pos;						// �|���S���̈ʒu
	D3DXVECTOR3				m_aPos[NUM_VTX];					// ���_�̈ʒu
	D3DXVECTOR3				m_rot;						// ������x�N�g��
	float					m_fSize;					// �傫��
	float					m_fHeight;
	float					m_fWidth;
};

#endif