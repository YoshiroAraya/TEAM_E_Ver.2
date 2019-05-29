//=============================================================================
//
// �j���[�X�L���X�^�[���� [newsCaster.h]
// Author : ���R���
//
//=============================================================================
#ifndef _NEWSCASTER_H_
#define _NEWSCASTER_H_

#include "main.h"
#include "sceneX.h"

//========================================
// �}�N����`
//========================================

//========================================
// �N���X�̒�`
//========================================
//=====================
// �j���[�X�L���X�^�[�N���X
//=====================
class CNewsCaster : public CSceneX
{
public:
	CNewsCaster();	// �R���X�g���N�^
	~CNewsCaster();	// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos);	// �j���[�X�L���X�^�[����������
	void Uninit(void);	// �j���[�X�L���X�^�[�I������
	void Update(void);	// �j���[�X�L���X�^�[�X�V����
	void Draw(void);	// �j���[�X�L���X�^�[�`�揈��

	static HRESULT LoadModel(void);	// ���f���ǂݍ���
	static void UnloadModel(void);	// ���f���e�N�X�`�����
	static HRESULT LoadMat(void);	// �}�e���A���ǂݍ���
	static void UnloadMat(void);	// �}�e���A�����

	static CNewsCaster *Create(D3DXVECTOR3 pos);	// �I�u�W�F�N�g�̐���

private:
	static LPD3DXMESH		m_pMesh;		// ���b�V�����i���_���j�ւ̃|�C���^
	static LPD3DXBUFFER		m_pBuffMat;		// �}�e���A�����ւ̃|�C���^
	static DWORD			m_nNumMat;		// �}�e���A�����̐�
	static LPDIRECT3DTEXTURE9		*m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X
	int m_nCntTimer;
};
#endif
