//=============================================================================
//
// ����e�N�X�`���̏��� [charaSelect.cpp]
// Author : ���R���
//
//=============================================================================
#include "charaSelect.h"
#include "renderer.h"
#include "input.h"
#include "scene2D.h"
#include "manager.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "camera.h"
#include "load.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PAUSE_POS_X		((SCREEN_WIDTH / 2))		// �����̍���X���W
#define PAUSE_POS_Y		((SCREEN_HEIGHT / 2))		// �����̍���Y���W
#define PAUSE_WIDTH		((SCREEN_WIDTH / 2))		// �����̕�
#define PAUSE_HEIGHT	((SCREEN_HEIGHT / 2))		// �����̍���
#define TOUZAI_SIZE		(150.0f)						// �����̃T�C�Y

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// ����e�N�X�`���̐�������
//=============================================================================
CCharaSelect *CCharaSelect::Create(D3DXVECTOR3 pos)
{
	CCharaSelect *pCharaSelect = NULL;

	if (pCharaSelect == NULL)
	{
		// �w�i�N���X�̐���
		pCharaSelect = new CCharaSelect;

		if (pCharaSelect != NULL)
		{
			pCharaSelect->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_CHARASELECT));
			pCharaSelect->Init(pos);
		}
	}
	return pCharaSelect;
}

//=============================================================================
// ����e�N�X�`���N���X�̃R���X�g���N�^
//=============================================================================
CCharaSelect::CCharaSelect() : CScene2D(3)
{
	// �l���N���A
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	m_nSelect = 0;
	m_fScale = 0.0f;
}

//=============================================================================
// ����e�N�X�`���N���X�̃f�X�g���N�^
//=============================================================================
CCharaSelect::~CCharaSelect()
{
}

//=============================================================================
// ����e�N�X�`������������
//=============================================================================
HRESULT CCharaSelect::Init(D3DXVECTOR3 pos)
{
	m_nSelect = 0;
	m_fScale = 150.0f;
	m_bReset = true;

	CScene2D::Init(pos);

	CScene2D::SetWidthHeight(m_fScale, m_fScale);

	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	CScene2D::SetObjType(CScene::OBJTYPE_UI);

	return S_OK;
}

//=============================================================================
// ����e�N�X�`���I������
//=============================================================================
void CCharaSelect::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// ����e�N�X�`���X�V����
//=============================================================================
void CCharaSelect::Update(void)
{
	//// ���͏����擾
	//CInputKeyboard *pInputKeyboard;
	//pInputKeyboard = CManager::GetInputKeyboard();
	//CXInputJoyPad *pXInput = NULL;
	//pXInput = CManager::GetXInput();

	//LPDIRECT3DVERTEXBUFFER9 pVtxBuff;
	//pVtxBuff = CScene2D::GetBuff();

	//D3DXVECTOR3 pos = CScene2D::GetPosition();

	//VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	//					// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	//pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//if (CCamera::GetState() == CCamera::STATE_HIGASHI)
	//{
	//	// �e�N�X�`�����W�̐ݒ�
	//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	//	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

	//	if (m_fScale <= TOUZAI_SIZE)
	//	{
	//		m_fScale += 5.0f;

	//		if (m_fScale > TOUZAI_SIZE)
	//		{
	//			m_fScale = TOUZAI_SIZE;
	//		}
	//	}

	//	pos = D3DXVECTOR3(200.0f, 150.0f, 0.0f);
	//}
	//else if (CCamera::GetState() == CCamera::STATE_NISHI)
	//{
	//	if (m_bReset == true)
	//	{
	//		m_fScale = 10.0f;

	//		m_bReset = false;
	//	}

	//	// �e�N�X�`�����W�̐ݒ�
	//	pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
	//	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//	pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
	//	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//	if (m_fScale <= TOUZAI_SIZE)
	//	{
	//		m_fScale += 5.0f;

	//		if (m_fScale > TOUZAI_SIZE)
	//		{
	//			m_fScale = TOUZAI_SIZE;
	//		}
	//	}

	//	pos = D3DXVECTOR3(SCREEN_WIDTH - 200.0f, 150.0f, 0.0f);
	//}

	//CScene2D::SetBuff(pVtxBuff);
	//CScene2D::SetPos(pos, 0.0f, m_fScale, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//=============================================================================
// ����e�N�X�`���`�揈��
//=============================================================================
void CCharaSelect::Draw(void)
{
	CScene2D::Draw();
}