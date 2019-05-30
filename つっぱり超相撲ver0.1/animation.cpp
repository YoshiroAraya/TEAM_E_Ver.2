//=============================================================================
//
// �A�j���[�V�����̏��� [animation.cpp]
// Author : Yamashita
//
//=============================================================================
#include "main.h"
#include "animation.h"
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME	"data/TEXTURE/EFFECT/�R���| 10.png"		//�ǂݍ��ރe�N�X�`���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@
LPDIRECT3DTEXTURE9 CAnimation::m_pTexture = {};

//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CAnimation::CAnimation() : CScene3D(7,CScene3D::OBJTYPE_EFFECT)
{
	m_fUV_U = 0.0f;
	m_fUV_V = 0.0f;
	m_nLife = 0;
	m_nDrawType = 0;
	m_nCounterAnim = 0;
	m_nCntSpeed = 0;
	m_nPatternAnim = 0;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CAnimation::~CAnimation() 
{

}

//=============================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CAnimation::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̊J������
//=============================================================================
void CAnimation::Unload(void)
{
	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
// �A�j���[�V�����̏���������
//=============================================================================
HRESULT CAnimation::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXCOLOR col, float fHeight, float fWidth, float fUV_U, float fUV_V, int nCntSpeed, int nTotalAnim,int nRoop,
	int nDrawType)
{
	//�F����
	m_col = col;

	

	CScene3D::SetSize(fHeight, fWidth);
	CScene3D::SetRot(rot);


	//������
	CScene3D::Init(pos);
	//�e�N�X�`���̓\��t��
	CScene3D::BindTexture(m_pTexture);
	CScene3D::SetColor(m_col);
	//UV�̒l����
	m_fUV_U = fUV_U;
	m_fUV_V = fUV_V;

	//�A�j���[�V�����̐ݒ�
	SetAnimation(0, m_fUV_U, m_fUV_V);

	//�A�j���[�V�����̑���
	m_nCntSpeed = nCntSpeed;

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
void CAnimation::Uninit(void)
{
	//�I������
	CScene3D::Uninit();
}

//=============================================================================
// �A�j���[�V�����̍X�V����
//=============================================================================
void CAnimation::Update(void)
{
	//�e�N�X�`���̔j���t���O
	bool bDestroy = false;

	if (m_nRoop == 0)
	{//���[�v����

		//�A�j���[�V������i�߂�X�V����
		UpdateAnim();
	}
	else if(m_nRoop == 1)
	{//���[�v���Ȃ�

		//���������炷
		m_nLife--;

		//�����x�����炷
		m_col.a -= 0.05f;

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
		SetColor(m_col);
	}


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
void CAnimation::UpdateAnim(void)
{
	//�A�j���[�V�����̃J�E���^�[��i�߂�
	m_nCounterAnim++;

	if ((m_nCounterAnim % m_nCntSpeed) == 0)
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
void CAnimation::Draw(void)
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
		CScene3D::Draw();

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
		CScene3D::Draw();
	}
}

//=============================================================================
// �A�j���[�V�����̐�������
//=============================================================================
CAnimation *CAnimation::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXCOLOR col,float fHeight,float fWidth, float fUV_U, float fUV_V, int nCntSpeed, int nTotalAnim,int nRoop,
	int nDrawType)
{
	CAnimation *pAnimation = {};

	if (pAnimation == NULL)
	{//NULL��������

	 //�������̓��I�m��
		pAnimation = new CAnimation;

		if (pAnimation != NULL)
		{
			// �|���S���̏���������
			pAnimation->Init(pos,rot, col,fWidth,fHeight,fUV_U,fUV_V,nCntSpeed,nTotalAnim,nRoop,nDrawType);
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