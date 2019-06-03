//=============================================================================
//
// �V�[��2D���� [scene2D.h]
// Author : ���R���
//
//=============================================================================
#include "scene.h"
#include "logo.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "input.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 *CLogo::m_pTexture = NULL;

const char *CLogo::m_apFilename[] =
{
	"data\\TEXTURE\\TITLE\\title.png",
	"data\\TEXTURE\\mikiya005.jpg",
};

//=============================================================================
// �V�[���N���X�̃R���X�g���N�^
//=============================================================================
CLogo::CLogo(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �l���N���A

	//m_pVtxBuff = NULL;

	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fSpin = 0.0f;	//��]
	m_Col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//�F
	m_fScale = 0.0f;	//�g��
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fRight = 0.0f;
	m_fLeft = 0.0f;
}

//=============================================================================
// �I�u�W�F�N�g�̐�������
//=============================================================================
CLogo *CLogo::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, TYPE type)
{
	CLogo *pLogo = NULL;

	if (pLogo == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pLogo = new CLogo;

		if (pLogo != NULL)
		{
			pLogo->m_fWidth = fWidth;
			pLogo->m_fHeight = fHeight;
			pLogo->Init(pos);
			pLogo->m_type = type;
		}
	}

	return pLogo;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLogo::~CLogo()
{
}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT CLogo::Init(D3DXVECTOR3 pos)
{
	// �|���S���̈ʒu��ݒ�
	m_Pos = pos;

	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	//SetObjType(CScene::OBJTYPE_SCENE2D);

	// �����_���[���擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// �e�N�X�`���̐���
	//D3DXCreateTextureFromFile(pDevice, PLAYER_NAME, &m_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	// �m�ۂ���o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

						// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x - m_fWidth, m_Pos.y - m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x + m_fWidth, m_Pos.y - m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x - m_fWidth, m_Pos.y + m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x + m_fWidth, m_Pos.y + m_fHeight, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void CLogo::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �I�u�W�F�N�g�̉��
	Release();
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void CLogo::Update(void)
{
	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

#ifdef _DEBUG
	//CDebugProc::Print("cfccfccfc", "2DPos    : x", m_Pos.x, "f", " y", m_Pos.y, "f", " z", m_Pos.z, "f");
#endif
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CLogo::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture[m_type]);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CLogo::Load(void)
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

	int nTexData = (sizeof m_apFilename);
	int nTexSize = (sizeof m_apFilename[0]);
	int nNumTex = nTexData / nTexSize;

	// �e�N�X�`���̐��𓮓I�Ɋm��
	m_pTexture = new LPDIRECT3DTEXTURE9[nNumTex];

	for (int nCntTex = 0; nCntTex < nNumTex; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, m_apFilename[nCntTex], &m_pTexture[nCntTex]);
	}

	return S_OK;
}

//=============================================================================
// �e�N�X�`���������
//=============================================================================
void CLogo::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		// ���������������
		delete[] m_pTexture;
		m_pTexture = NULL;
	}
}