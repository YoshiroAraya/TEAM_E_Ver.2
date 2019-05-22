//=============================================================================
//
// �y�U���� [dohyo.h]
// Author : ���R���
//
//=============================================================================
#ifndef _DOHYO_H_
#define _DOHYO_H_

#include "main.h"
#include "sceneX.h"

//========================================
// �}�N����`
//========================================

//========================================
// �N���X�̒�`
//========================================
//=====================
// �y�U�N���X
//=====================
class CDohyo : public CSceneX
{
public:
	CDohyo();	// �R���X�g���N�^
	~CDohyo();	// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos);	// �y�U����������
	void Uninit(void);	// �y�U�I������
	void Update(void);	// �y�U�X�V����
	void Draw(void);	// �y�U�`�揈��

	static HRESULT LoadModel(void);	// ���f���ǂݍ���
	static void UnloadModel(void);	// ���f���e�N�X�`�����
	static HRESULT LoadMat(void);	// �}�e���A���ǂݍ���
	static void UnloadMat(void);	// �}�e���A�����

	static CDohyo *Create(D3DXVECTOR3 pos);	// �I�u�W�F�N�g�̐���

private:
	static LPD3DXMESH		m_pMesh;		// ���b�V�����i���_���j�ւ̃|�C���^
	static LPD3DXBUFFER		m_pBuffMat;		// �}�e���A�����ւ̃|�C���^
	static DWORD			m_nNumMat;		// �}�e���A�����̐�
	static LPDIRECT3DTEXTURE9		*m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X
};
#endif
