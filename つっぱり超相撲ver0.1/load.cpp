//=============================================================================
//
// �e�N�X�`������ [texture.cpp]
// Author : ���R���
//
//=============================================================================
#include "load.h"
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "debugProc.h"
#include "manager.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 *CLoad::m_pTexture = NULL;
LPD3DXMESH *CLoad::m_pMesh = NULL;			// ���b�V�����i���_���j�ւ̃|�C���^
LPD3DXBUFFER *CLoad::m_pBuffMat = NULL;	// �}�e���A�����ւ̃|�C���^
DWORD *CLoad::m_nNumMat = 0;				// �}�e���A�����̐�

const char *CLoad::m_apModelFilename[] =
{// ���f��
	"data\\MODEL\\dohyo.x",
	"data\\MODEL\\rikisi.x",
	"data\\MODEL\\rikisi2.x",
	"data\\MODEL\\customer.x",
	"data\\MODEL\\shadow000.x",
	"data\\MODEL\\rikisi.x",
};

const char *CLoad::m_apTexFilename[] =
{// �e�N�X�`��
	"data/TEXTURE/dohyo.png",
	"data/TEXTURE/TITLE/newBG.png",
	"data/TEXTURE/sand2.jpg",
	"data/TEXTURE/wall.png",
	"data\\TEXTURE\\TITLE\\title.png",
	"data/TEXTURE/time.png",
	"data/TEXTURE/.png",						// �Q�[�W
	"data/TEXTURE/JANKEN/Buchikamashi.png",
	"data/TEXTURE/JANKEN/Henka.png",
	"data/TEXTURE/JANKEN/Stuppari.png",
	"data/TEXTURE/touzai.png",
	"data\\TEXTURE\\EFFECT\\particle000.jpg",
	"data\\TEXTURE\\EFFECT\\�R���| _0.png",
	"data/TEXTURE/EFFECT/�R���| 10.png",
};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLoad::CLoad()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLoad::~CLoad()
{
}

//=============================================================================
// �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CLoad::LoadModel(void)
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

	int nModelData = (sizeof m_apModelFilename);
	int nModelSize = (sizeof m_apModelFilename[0]);
	int nNumModel = nModelData / nModelSize;

	// ���I�Ɋm��
	m_pMesh = new LPD3DXMESH[nNumModel];
	m_pBuffMat = new LPD3DXBUFFER[nNumModel];
	m_nNumMat = new DWORD[nNumModel];

	for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
	{
		D3DXLoadMeshFromX(m_apModelFilename[nCntModel],
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
void CLoad::UnloadModel(void)
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
// �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CLoad::LoadTex(void)
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

	int nTexData = (sizeof m_apTexFilename);
	int nTexSize = (sizeof m_apTexFilename[0]);
	int nNumTex = nTexData / nTexSize;

	// �e�N�X�`���̐��𓮓I�Ɋm��
	m_pTexture = new LPDIRECT3DTEXTURE9[nNumTex];

	for (int nCntTex = 0; nCntTex < nNumTex; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, m_apTexFilename[nCntTex], &m_pTexture[nCntTex]);
	}

	return S_OK;
}

//=============================================================================
// �e�N�X�`���������
//=============================================================================
void CLoad::UnloadTex(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		// ���������������
		delete[] m_pTexture;
		m_pTexture = NULL;
	}
}

//=============================================================================
// �e�N�X�`���擾
//=============================================================================
LPDIRECT3DTEXTURE9 CLoad::GetTexture(int nIdx)
{
	return m_pTexture[nIdx];
}

//=============================================================================
// ���b�V���擾
//=============================================================================
LPD3DXMESH CLoad::GetMesh(int nIdx)
{
	return m_pMesh[nIdx];
}

//=============================================================================
// �}�e���A�����擾
//=============================================================================
LPD3DXBUFFER CLoad::GetBuffMat(int nIdx)
{
	return m_pBuffMat[nIdx];
}

//=============================================================================
// �}�e���A�����̐��擾
//=============================================================================
DWORD CLoad::GetNumMat(int nIdx)
{
	return m_nNumMat[nIdx];
}