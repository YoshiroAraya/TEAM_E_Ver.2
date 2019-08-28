//=============================================================================
//
// �A�j���[�V�����̏��� [animation.cpp]
// Author : Yamashita
//
//=============================================================================
#include "main.h"
#include "2Danimation.h"
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "load.h"
#include "player.h"
#include "game.h"
#include "tutorial.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=============================================================================
//	�R���X�g���N�^
//=============================================================================
C2DAnimation::C2DAnimation() : CScene2D(7, CScene2D::OBJTYPE_EFFECT)
{
	m_fUV_U = 0.0f;
	m_fUV_V = 0.0f;
	m_nLife = 0;
	m_nDrawType = 0;
	m_nCounterAnim = 0;
	m_fCntSpeed = 0.0f;
	m_nPatternAnim = 0;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
C2DAnimation::~C2DAnimation()
{

}

//=============================================================================
// �A�j���[�V�����̏���������
//=============================================================================
HRESULT C2DAnimation::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, float fUV_U, float fUV_V, float fCntSpeed, int nTotalAnim, int nRoop,
	int nDrawType)
{
	//�F����
	m_col = col;


	//������
	CScene2D::Init(pos);
	//�e�N�X�`���̓\��t��
	CScene2D::BindTexture(CLoad::GetTexture(CLoad::TEXTURE_ANIMATION_OUGI));
	CScene2D::SetPos(pos,0.0f,fScale,m_col);
	//UV�̒l����
	m_fUV_U = fUV_U;
	m_fUV_V = fUV_V;

	//�A�j���[�V�����̐ݒ�
	SetAnimation(0, m_fUV_U, m_fUV_V);

	//�A�j���[�V�����̑���
	m_fCntSpeed = fCntSpeed;

	//�A�j���[�V�����̍��v����
	m_nTotalAnim = nTotalAnim;

	//�A�j���[�V�������[�v���邩���Ȃ���
	m_nRoop = nRoop;

	//�`��^�C�v
	m_nDrawType = nDrawType;


	m_bUse = true;
	return S_OK;
}

//=============================================================================
// �A�j���[�V�����̏I������
//=============================================================================
void C2DAnimation::Uninit(void)
{
	//�I������
	CScene2D::Uninit();
}

//=============================================================================
// �A�j���[�V�����̍X�V����
//=============================================================================
void C2DAnimation::Update(void)
{
	CGame *pGame = NULL;
	CPlayer *pPlayer = NULL;
	D3DXVECTOR3 pos;


	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
	}

	if (pPlayer != NULL)
	{
		pos = pPlayer->GetPosition();
	}
	//�e�N�X�`���̔j���t���O
	bool bDestroy = false;

	if (m_nRoop == 0)
	{//���[�v����

	 //�A�j���[�V������i�߂�X�V����
		UpdateAnim();
	}
	else if (m_nRoop == 1)
	{//���[�v���Ȃ�

	 //���������炷
		m_nLife--;

		//�����x�����炷
		//m_col.a -= 0.05f;

		if (m_bUse == true)
		{
			//�A�j���[�V������i�߂�X�V����
			UpdateAnim();
		}

		if (m_nTotalAnim - 1 == m_nPatternAnim)
		{//�A�j���[�V�������~�߂�
			m_bUse = false;
		}
		//�F�̒l���X�V����
		SetCol(m_col);
	}

	//�F��0.0f�ɂȂ�����
	if (m_col.a <= 0.0f)
	{
		//�j���t���O��������
		bDestroy = true;
	}

	if (bDestroy == true)
	{
		//�e�N�X�`����j��
		Uninit();
	}

}

//=============================================================================
// �A�j���[�V������i�߂�X�V����
//=============================================================================
void C2DAnimation::UpdateAnim(void)
{
	//�A�j���[�V�����̃J�E���^�[��i�߂�
	m_nCounterAnim++;

	if ((m_nCounterAnim % (int)m_fCntSpeed) == 0)
	{
		//�p�^�[���X�V
		m_nPatternAnim = (m_nPatternAnim + 1) % m_nTotalAnim;

		//�A�j���[�V�����̐ݒ�
		SetAnimation(m_nPatternAnim, m_fUV_U, m_fUV_V);
	}
}

//=============================================================================
//�A�j���[�V�����̕`�揈��
//=============================================================================
void C2DAnimation::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	if (m_nDrawType == 0)
	{
		//�A���t�@�e�X�g(�����F��`�悵�Ȃ��悤��)
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 500);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		// ���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		//�`�揈��
		CScene2D::Draw();

		// ���u�����f�B���O�����ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//�����_�[�X�e�[�g�̐ݒ�����ɖ߂�
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
	else
	{
		//�`�揈��
		CScene2D::Draw();
	}
}

//=============================================================================
// �A�j���[�V�����̐�������
//=============================================================================
C2DAnimation *C2DAnimation::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, float fUV_U, float fUV_V, float fCntSpeed, int nTotalAnim,
	int nRoop,int nDrawType)
{
	C2DAnimation *pAnimation = {};

	if (pAnimation == NULL)
	{//NULL��������

	 //�������̓��I�m��
		pAnimation = new C2DAnimation;

		if (pAnimation != NULL)
		{
			// �|���S���̏���������
			pAnimation->Init(pos, col, fScale, fUV_U, fUV_V, fCntSpeed, nTotalAnim, nRoop, nDrawType);
		}
		else
		{
			MessageBox(0, "NULL�ł���", "�x��", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "NULL����Ȃ��ł�", "�x��", MB_OK);
	}

	return pAnimation;
}