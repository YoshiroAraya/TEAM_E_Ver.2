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
	"data\\MODEL\\dohyo2.x",
	"data\\MODEL\\rikisi.x",
	"data\\MODEL\\rikisi2.x",
	"data\\MODEL\\customer.x",
	"data\\MODEL\\shadow000.x",
	"data\\MODEL\\newscaster.x",
};

const char *CLoad::m_apTexFilename[] =
{// �e�N�X�`��
	"data/TEXTURE/dohyo.png",					//0
	"data/TEXTURE/TITLE/newBG.png",				//1
	"data/TEXTURE/sand2.jpg",					//2
	"data/TEXTURE/wall.png",					//3
	"data\\TEXTURE\\TITLE\\title.png",			//4
	"data/TEXTURE/time.png",					//5
	"data/TEXTURE/.png",						//6 �Q�[�W
	"data/TEXTURE/JANKEN/Buchikamashi.png",		//7
	"data/TEXTURE/JANKEN/Henka.png",			//8
	"data/TEXTURE/JANKEN/Stuppari.png",			//9
	"data/TEXTURE/touzai.png",					//10
	"data/TEXTURE/TITLE/1P.png",				//11
	"data/TEXTURE/TITLE/2P.png",				//12
	"data/TEXTURE/TITLE/tutorial01.png",		//13
	"data\\TEXTURE\\EFFECT\\particle000.jpg",	//14
	"data\\TEXTURE\\EFFECT\\money.png",			//15
	"data/TEXTURE/EFFECT/16.png",				//16
	"data/TEXTURE/EFFECT/somke.png",			//17
	"data/TEXTURE/circle.png",					//18
	"data/TEXTURE/UI/Tyanco.png",				//19
	"data/TEXTURE/UI/number002.png"	,			//20
	"data/TEXTURE/UI/Select00.png",				//21
	"data/TEXTURE/UI/Select01.png",				//22
	"data/TEXTURE/UI/Select02.png",				//23
	"data/TEXTURE/UI/pause100.png",				//24
	"data/TEXTURE/UI/Select03.png",				//25
	"data/TEXTURE/UI/Select04.png",				//26
	"data/TEXTURE/UI/Select02.png",				//27
	"data/TEXTURE/UI/pause100.png",				//28
	"data/TEXTURE/UI/select.png",				//29
	"data/TEXTURE/EFFECT/all.png",				//30
	"data/TEXTURE/UI/tutorial00.jpg",			//31
	"data/TEXTURE/UI/tutorial01.jpg",			//32
	"data/TEXTURE/UI/tutorial02.jpg",			//33
	"data/TEXTURE/UI/sensu.png",				//34
	"data/TEXTURE/UI/line.png",					//35
	"data/TEXTURE/UI/ippon.png",				//36
	"data/TEXTURE/UI/HPgauge.png",				//37
	"data/TEXTURE/UI/EXgauge.png",				//38
	"data/TEXTURE/UI/ResultWinner.png",			//39
	"data/TEXTURE/EFFECT/10.png",				//40
	"data/TEXTURE/EFFECT/19.png",				//41
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

	if (m_pMesh != NULL)
	{
		// ���������������
		delete[] m_pMesh;
		m_pMesh = NULL;
	}

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