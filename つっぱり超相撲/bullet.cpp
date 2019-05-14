//=============================================================================
//
// �|���S������ [Bullet.cpp]
// Author : ���R���
//
//=============================================================================
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

//=============================================================================
// �v���C���[�N���X�̃R���X�g���N�^
//=============================================================================
CBullet::CBullet() : CBillboard(3)
{
	// �l���N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nLife = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBullet::~CBullet()
{
}

//=============================================================================
// �e�̐�������
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col)
{
	CBullet *pBullet = NULL;

	if (pBullet == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pBullet = new CBullet;

		if (pBullet != NULL)
		{
			pBullet->Init(pos);
			pBullet->m_move = move;
			pBullet->m_col = col;
			pBullet->BindTexture(m_pTexture);
		}
	}
	return pBullet;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos)
{
	CBillboard::Init(pos);

	// �����_���[���擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nLife = 50;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBullet::Uninit(void)
{	
	CBillboard::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBullet::Update(void)
{
	// ���_�o�b�t�@���擾
	LPDIRECT3DVERTEXBUFFER9 VtxBuff;
	VtxBuff = CBillboard::GetVtxBuff();

	// �ʒu�擾
	D3DXVECTOR3 pos;
	pos = CBillboard::GetPosition();

	VERTEX_3D *pVtx;//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N�����_�f�[�^�̃|�C���^���擾
	VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�𓮂���
	pos.x += m_move.x;
	pos.y += m_move.y;
	pos.z += m_move.z;

	// �e�̃��C�t�����炷
	m_nLife--;

	if (m_nLife <= 0)
	{//	�e�폜
		Uninit();
	}

	//���_�o�b�t�@���A�����b�N����
	VtxBuff->Unlock();

	CBillboard::SetPosition(pos);

#ifdef _DEBUG
	//CDebugProc::Print("cfccfccfc", "BulletPos : x", pos.x, "f", "   y", pos.y, "f", "  z", pos.z, "f");
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBullet::Draw(void)
{
	CBillboard::Draw();
}

//=============================================================================
// �v���C���[�̃e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CBullet::Load(void)
{
	// �����_���[���擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, BULLET_TEXTURENAME, &m_pTexture);

	return S_OK;
}

//=============================================================================
// �v���C���[�̃e�N�X�`���������
//=============================================================================
void CBullet::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}