//=============================================================================
//
// �V�[��2D���� [scene2D.h]
// Author : ���R���
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "input.h"

//=============================================================================
// �V�[���N���X�̃R���X�g���N�^
//=============================================================================
CScene2D::CScene2D(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �l���N���A
	m_pTexture = NULL;
	
	m_pVtxBuff = NULL;

	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �I�u�W�F�N�g�̐�������
//=============================================================================
CScene2D *CScene2D::Create(D3DXVECTOR3 pos)
{
	CScene2D *pScene2D = NULL;

	if (pScene2D == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pScene2D = new CScene2D;

		if (pScene2D != NULL)
		{
			pScene2D->Init(pos);
		}
	}

	return pScene2D;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{
}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos)
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
	D3DXCreateTextureFromFile(pDevice, PLAYER_NAME, &m_pTexture);

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
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x - 50.0f, m_Pos.y - 50.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x + 50.0f, m_Pos.y - 50.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x - 50.0f, m_Pos.y + 50.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x + 50.0f, m_Pos.y + 50.0f, 0.0f);

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
void CScene2D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	

	// �I�u�W�F�N�g�̉��
	Release();
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void CScene2D::Update(void)
{
	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		Uninit();
	}

	//VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	//// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	//m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//// ���_�����X�V
	//pVtx[0].pos = D3DXVECTOR3(m_Pos.x - 50.0f, m_Pos.y - 50.0f, 0.0f);
	//pVtx[1].pos = D3DXVECTOR3(m_Pos.x + 50.0f, m_Pos.y - 50.0f, 0.0f);
	//pVtx[2].pos = D3DXVECTOR3(m_Pos.x - 50.0f, m_Pos.y + 50.0f, 0.0f);
	//pVtx[3].pos = D3DXVECTOR3(m_Pos.x + 50.0f, m_Pos.y + 50.0f, 0.0f);

	//// ���_�o�b�t�@���A�����b�N
	//m_pVtxBuff->Unlock();

#ifdef _DEBUG
	//CDebugProc::Print("cfccfccfc", "2DPos    : x", m_Pos.x, "f", " y", m_Pos.y, "f", " z", m_Pos.z, "f");
#endif
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CScene2D::Draw(void)
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
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// �I�u�W�F�N�g�̎擾
//=============================================================================
D3DXVECTOR3 CScene2D::GetPosition(void)
{
	return m_Pos;
}

//=============================================================================
// �I�u�W�F�N�g�̐ݒ�
//=============================================================================
void CScene2D::SetPosition(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}