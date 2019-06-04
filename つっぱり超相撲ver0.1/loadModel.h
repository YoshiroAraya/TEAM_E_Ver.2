//=============================================================================
//
// ���f���̓ǂݍ��ݏ��� [loadModel.h]
// Author : ���R���
//
//=============================================================================
#ifndef _LOADMODEL_H_
#define _LOADMODEL_H_

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
class CLoadModel
{
public:
	// �T�E���h�t�@�C��
	typedef enum //���x���Ɛ��ƕ��т𑵂���
	{
		MODEL_DOHYO = 0,	// �y�U
		MODEL_PLAYER,		// �͎m1
		MODEL_ENEMY,		// �͎m2
		MODEL_CUSTMOR,		// �ϋq
		MODEL_SHADOW,		// �e
		MODEL_NEWSCASTER,	// �j���[�X�L���X�^�[
	} MODEL;

	CLoadModel();		// �R���X�g���N�^
	~CLoadModel();	// �f�X�g���N�^

	static HRESULT Load(void);
	static void Unload(void);
	static LPD3DXMESH GetMesh(int nIdx);
	static LPD3DXBUFFER GetBuffMat(int nIdx);
	static DWORD GetNumMat(int nIdx);

private:
	static LPDIRECT3DTEXTURE9	*m_pTexture;					// �e�N�X�`���ւ̃|�C���^
	static LPD3DXMESH		*m_pMesh;		// ���b�V�����i���_���j�ւ̃|�C���^
	static LPD3DXBUFFER		*m_pBuffMat;		// �}�e���A�����ւ̃|�C���^
	static DWORD			*m_nNumMat;		// �}�e���A�����̐�
	//static LPDIRECT3DTEXTURE9	m_pTexture;					// �e�N�X�`���ւ̃|�C���^
	static const char *m_apFilename[];
};

#endif