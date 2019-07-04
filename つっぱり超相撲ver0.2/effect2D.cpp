//=============================================================================
//
// �G�t�F�N�g���� [effect3D.cpp]
// Author : �R���֎j
//
//=============================================================================
#include "effect2D.h"
#include "manager.h"
#include "renderer.h"
#include "billboard.h"
#include "debugProc.h"
#include "load.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEX_POS_X_INIT			(1.0f)									//�e�N�X�`�����WU�̏����ʒu
#define TEX_POS_Y_INIT			(1.0f)									//�e�N�X�`�����WV�̏����ʒu
#define TEX_LEFT				(0.0f)									//�e�N�X�`�����WU��
#define TEX_RIGHT				(1.0f)									//�e�N�X�`�����WU�E
#define TEX_TOP					(0.0f)									//�e�N�X�`�����WV��
#define TEX_BOT					(1.0f)									//�e�N�X�`�����WV��

//--------------------------------------------
//�ÓI�����o�ϐ��錾
//--------------------------------------------

//--------------------------------------------
//�G�t�F�N�g�N���X �R���X�g���N�^
//--------------------------------------------
CEffect2D::CEffect2D() : CScene2D(7, CScene::OBJTYPE_EFFECT)
{
	m_pos = D3DXVECTOR3(0, 0, 0);						// �ʒu
	m_move = D3DXVECTOR3(0, 0, 0);					// �ړ���
	m_posold = D3DXVECTOR3(0, 0, 0);				// �O��̈ʒu
}

//--------------------------------------------
//�G�t�F�N�g�N���X �f�X�g���N�^
//--------------------------------------------
CEffect2D::~CEffect2D()
{
}

//--------------------------------------------
//�G�t�F�N�g�̐���
//--------------------------------------------
CEffect2D *CEffect2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
	float fWidth, float fHeight, int nLife, int TexType)
{
	CEffect2D *pEffect = NULL;

	if (pEffect == NULL)
	{
		//�������𓮓I�m��
		pEffect = new CEffect2D;

		if (pEffect != NULL)
		{
			pEffect->m_pos = pos;
			pEffect->m_move = move;
			pEffect->m_Col = col;
			pEffect->m_fHeight = fHeight;
			pEffect->m_fWidth = fWidth;
			pEffect->m_nLife = nLife;
			pEffect->m_nTexType = TexType;
			pEffect->Init();
		}
	}
	return pEffect;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEffect2D::Init(void)
{
	//����������
	CScene2D::Init(m_pos);

	//�e�N�X�`���̐ݒ�
	CEffect2D::BindTexture(CLoad::GetTexture(m_nTexType));

	//�T�C�Y�̐ݒ�
	CEffect2D::SetWidthHeight(m_fWidth, m_fHeight);

	m_fAlpha = 1.0f / (float)m_nLife;
	
	m_nCntTimer = 0;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEffect2D::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEffect2D::Update(void)
{
	if (m_nTexType == CLoad::TEXTURE_EFFECT_NORMAL001)
	{
		//�p�[�e�B�N��
		UpdateUI();
	}
}

//=============================================================================
// �����̍X�V����
//=============================================================================
void CEffect2D::UpdateUI(void)
{
	//�����p�̎��S�t���O�ϐ�
	bool bDestroy = false;

	m_nCntTimer++;


	/*D3DXVECTOR3 Parpos1 = D3DXVECTOR3(sinf(fAngle) * fLength1, 0.0f, cosf(fAngle) * fLength1);
	D3DXVECTOR3 Parpos2 = D3DXVECTOR3(sinf(fAngle) * fLength2, 0.0f, cosf(fAngle) * fLength2);*/

	if (m_nLife > 0)
	{
		m_nLife--;

		//�ʒu���X�V		
		m_pos += m_move;
		
		//���X�ɓ����ɂ��Ă���
		//m_Col.a = m_Col.a - m_fAlpha;
		m_Col.a -= 0.02f;

		//���ȉ��ɂȂ�����0��
		if (m_Col.a < 0.01f)
		{
			m_Col.a = 0;
		}
		m_fWidth += 3.0f;
		m_fHeight += 3.0f;
		//�F��ݒ�
		CScene2D::SetCol(m_Col);

		//�ݒ菈��
		CScene2D::SetWidthHeight(m_fWidth, m_fHeight);
		CScene2D::SetPosition(m_pos);
	}
	else if (m_nLife <= 0)
	{
		//�����������t���O�𗧂Ă�
		bDestroy = true;
	}

	if (bDestroy == true)
	{
		//����������(�j��)
		Uninit();
	}
	/*CDebugProc::Print("c", "�G�t�F�N�g");*/
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEffect2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	// ���u�����f�B���O�����Z�����ɐݒ�
	/*pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);*/

	CScene2D::Draw();

	// ���u�����f�B���O�����ɖ߂�
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	////���C�g��L���ɂ���
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}