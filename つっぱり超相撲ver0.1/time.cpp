//=============================================================================
//
// ���Ԃ̏��� [touzai.cpp]
// Author : ���R���
//
//=============================================================================
#include "time.h"
#include "renderer.h"
#include "input.h"
#include "scene2D.h"
#include "manager.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "camera.h"
#include "title.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PAUSE_POS_X		((SCREEN_WIDTH / 2))		// ���Ԃ̍���X���W
#define PAUSE_POS_Y		((SCREEN_HEIGHT / 2))		// ���Ԃ̍���Y���W
#define PAUSE_WIDTH		((SCREEN_WIDTH / 2))		// ���Ԃ̕�
#define PAUSE_HEIGHT	((SCREEN_HEIGHT / 2))		// ���Ԃ̍���
#define TOUZAI_SIZE		(150.0f)						// ���Ԃ̃T�C�Y

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CTime::m_pTexture = NULL;

//=============================================================================
// ���Ԃ̐�������
//=============================================================================
CTime *CTime::Create(D3DXVECTOR3 pos)
{
	CTime *pTouzai = NULL;

	if (pTouzai == NULL)
	{
		// �w�i�N���X�̐���
		pTouzai = new CTime;

		if (pTouzai != NULL)
		{
			pTouzai->Init(pos);
			pTouzai->BindTexture(m_pTexture);
		}
	}
	return pTouzai;
}

//=============================================================================
// ���ԃN���X�̃R���X�g���N�^
//=============================================================================
CTime::CTime() : CScene2D(7)
{
	// �l���N���A
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	m_nSelect = 0;
	m_fScale = 0.0f;
}

//=============================================================================
// ���ԃN���X�̃f�X�g���N�^
//=============================================================================
CTime::~CTime()
{
}

//=============================================================================
// ���ԏ���������
//=============================================================================
HRESULT CTime::Init(D3DXVECTOR3 pos)
{
	m_nSelect = 0;
	m_fScale = 50.0f;
	m_bReset = true;

	CScene2D::Init(pos);

	CScene2D::SetWidthHeight(m_fScale, m_fScale - 30.0f);

	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	CScene2D::SetObjType(CScene::OBJTYPE_UI);

	m_Col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// ���ԏI������
//=============================================================================
void CTime::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// ���ԍX�V����
//=============================================================================
void CTime::Update(void)
{
	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();

	// �v���C���[�̎擾
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	// �G�l�~�[�̎擾
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;
	pVtxBuff = CScene2D::GetBuff();

	// �^�C�g���擾
	CTitle *pTitle;
	pTitle = CManager::GetTitle();

	D3DXVECTOR3 pos = CScene2D::GetPosition();

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

						// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (pTitle != NULL)
	{
		if (pTitle->GetState() == CTitle::STATE_NEWS)
		{
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 0.5f);
		}
		else if(pTitle->GetState() == CTitle::STATE_TITLE)
		{
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	CScene2D::SetBuff(pVtxBuff);
	CScene2D::SetPos(pos, 0.0f, m_fScale, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	if (CCamera::GetState() == CCamera::STATE_NORMAL)
	{
		Uninit();
	}
}

//=============================================================================
// ���ԕ`�揈��
//=============================================================================
void CTime::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// ���Ԃ̃e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CTime::Load(void)
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
	D3DXCreateTextureFromFile(pDevice, TIME, &m_pTexture);

	return S_OK;
}

//=============================================================================
// ���Ԃ̃e�N�X�`���������
//=============================================================================
void CTime::Unload(void)
{
	if (m_pTexture != NULL)
	{// �e�N�X�`���̔j��
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}