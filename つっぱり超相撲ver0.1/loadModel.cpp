//=============================================================================
//
// �e�N�X�`������ [texture.cpp]
// Author : ���R���
//
//=============================================================================
#include "loadModel.h"
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "debugProc.h"
#include "manager.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 *CLoadModel::m_pTexture = NULL;
LPD3DXMESH *CLoadModel::m_pMesh = NULL;			// ���b�V�����i���_���j�ւ̃|�C���^
LPD3DXBUFFER *CLoadModel::m_pBuffMat = NULL;	// �}�e���A�����ւ̃|�C���^
DWORD *CLoadModel::m_nNumMat = 0;				// �}�e���A�����̐�

const char *CLoadModel::m_apFilename[] =
{
	"data\\MODEL\\dohyo.x",
	"data\\MODEL\\rikisi.x",
	"data\\MODEL\\rikisi2.x",
	"data\\MODEL\\customer.x",
	"data\\MODEL\\shadow000.x",
	"data\\MODEL\\rikisi.x",
};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLoadModel::CLoadModel()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLoadModel::~CLoadModel()
{
}

//=============================================================================
// �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CLoadModel::Load(void)
{
	// �����_���[���擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	if (m_pTexture != NULL)
	{
		// ���������������
		delete[] m_pTexture;
		m_pTexture = NULL;
	}

	int nModelData = (sizeof m_apFilename);
	int nModelSize = (sizeof m_apFilename[0]);
	int nNumModel = nModelData / nModelSize;

	// �e�N�X�`���̐��𓮓I�Ɋm��
	//m_pTexture = new LPDIRECT3DTEXTURE9[nNumTex];
	//m_pTexture = (LPDIRECT3DTEXTURE9)malloc(sizeof(int) * nNumTex);

	m_pMesh = new LPD3DXMESH[nNumModel];
	m_pBuffMat = new LPD3DXBUFFER[nNumModel];
	m_nNumMat = new DWORD[nNumModel];

	for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
	{
		D3DXLoadMeshFromX(m_apFilename[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_pBuffMat[nCntModel],
			NULL,
			&m_nNumMat[nCntModel],
			&m_pMesh[nCntModel]);
	}

	return S_OK;
}

//=============================================================================
// �e�N�X�`���������
//=============================================================================
void CLoadModel::Unload(void)
{
	if (m_pBuffMat != NULL)
	{
		// ���������������
		delete[] m_pBuffMat;
		m_pBuffMat = NULL;
	}

	if (m_nNumMat != NULL)
	{
		// ���������������
		delete[] m_nNumMat;
		m_nNumMat = NULL;
	}

	if (m_pMesh != NULL)
	{
		// ���������������
		delete[] m_pMesh;
		m_pMesh = NULL;
	}
}

//=============================================================================
// ���b�V���擾
//=============================================================================
LPD3DXMESH CLoadModel::GetMesh(int nIdx)
{
	return m_pMesh[nIdx];
}

//=============================================================================
// �}�e���A�����擾
//=============================================================================
LPD3DXBUFFER CLoadModel::GetBuffMat(int nIdx)
{
	return m_pBuffMat[nIdx];
}

//=============================================================================
// �}�e���A�����̐��擾
//=============================================================================
DWORD CLoadModel::GetNumMat(int nIdx)
{
	return m_nNumMat[nIdx];
}