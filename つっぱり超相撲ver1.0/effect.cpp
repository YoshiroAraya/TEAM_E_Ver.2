//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "billboard.h"
#include "debugProc.h"
#include "load.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURENAME002			 "data\\TEXTURE\\EFFECT\\effect003.png"		//�e�N�X�`���̃t�@�C����
#define TEXTURENAME003			 "data\\TEXTURE\\EFFECT\\effect004.png"		//�e�N�X�`���̃t�@�C����
#define TEXTURENAME004			 "data\\TEXTURE\\EFFECT\\effect005.png"		//�e�N�X�`���̃t�@�C����

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
CEffect::CEffect() :CBillboard(7, CScene::OBJTYPE_EFFECT)
{
	m_pos = D3DXVECTOR3(0,0,0);						// �ʒu
	m_move = D3DXVECTOR3(0, 0, 0);					// �ړ���
	m_posold = D3DXVECTOR3(0, 0, 0);				// �O��̈ʒu


	//m_TexType = EFFECTTEX_NORMAL000;
}

//--------------------------------------------
//�G�t�F�N�g�N���X �f�X�g���N�^
//--------------------------------------------
CEffect::~CEffect()
{
}

//--------------------------------------------
//�G�t�F�N�g�̐���
//--------------------------------------------
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
	float fWidth, float fHeight, int nNumMax, int nLife, int TexType)
{
	CEffect *pEffect = NULL;

	if (pEffect == NULL)
	{
		//�������𓮓I�m��
		pEffect = new CEffect;

		if (pEffect != NULL)
		{
			pEffect->m_pos = pos;
			pEffect->m_move = move;
			pEffect->m_Col = col;
			pEffect->m_fHeight = fHeight;
			pEffect->m_fWidth = fWidth;
			pEffect->m_nNumMax = nNumMax;
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
HRESULT CEffect::Init(void)
{

	CBillboard::BindTexture(CLoad::GetTexture(m_nTexType));
	CBillboard::Init(m_pos);
	CBillboard::SetCol(m_Col);
	CBillboard::SetBillboard(m_pos, m_fHeight, m_fWidth);

	m_fAlpha = 1.0f / (float)m_nLife;
	//m_fAlpha = m_fAlpha / 60;
	m_nCntTimer = 0;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEffect::Uninit(void)
{
	CBillboard::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEffect::Update(void)
{
	if (m_nTexType == CLoad::TEXTURE_EFFECT_NORMAL002)
	{
		//���̃G�t�F�N�g
		UpdateSmoke();
	}

	if (m_nTexType == CLoad::TEXTURE_EFFECT_NORMAL000)
	{
		//���p�[�e�B�N��
		UpdateSalt();
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CEffect::UpdateSmoke(void)
{
	//�����p�̎��S�t���O�ϐ�
	bool bDestroy = false;

	m_nCntTimer++;

	if (m_nLife > 0)
	{
		m_nLife--;

		//�d��
		m_move.y += cosf(D3DX_PI * 0) * 0.05f;

		//�ʒu���X�V
		m_pos += m_move;
		
		//���X�ɓ����ɂ��Ă���
		m_Col.a -= 0.009f;
		//���ȉ��ɂȂ�����0��
		if (m_Col.a < 0.01f)
		{
			m_Col.a = 0;
		}

		//�F��ݒ�
		CBillboard::SetCol(m_Col);
		m_fHeight += 1.5f;
		m_fWidth += 1.5f;
		//�ݒ菈��
		CBillboard::SetBillboard(m_pos, m_fHeight, m_fWidth);
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

		//false�ɖ߂�
		bDestroy = false;
	}
}

//=============================================================================
// �����̍X�V����
//=============================================================================
void CEffect::UpdateSalt(void)
{
	//�����p�̎��S�t���O�ϐ�
	bool bDestroy = false;

	m_nCntTimer++;

	if (m_nLife > 0)
	{
		m_nLife--;

		//�d��
		m_move.y -= cosf(D3DX_PI * 0) * 0.2f;

		//�ʒu���X�V		
		m_pos += m_move;
		//m_rot.x += 0.2f;

		//���X�ɓ����ɂ��Ă���
		m_Col.a = m_Col.a - m_fAlpha;

		//���ȉ��ɂȂ�����0��
		if (m_Col.a < 0.01f)
		{
			m_Col.a = 0;
		}
		//�F��ݒ�
		CBillboard::SetCol(m_Col);
	
		//�ݒ菈��
		CBillboard::SetBillboard(m_pos, m_fHeight, m_fWidth);
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
void CEffect::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//if (m_pBillBoard != NULL)
	{
		CBillboard::Draw();
	}

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}