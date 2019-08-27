//=============================================================================
//
// �G�t�F�N�g���� [effect3D.cpp]
// Author : �R���֎j
//
//=============================================================================
#include "effect3D.h"
#include "manager.h"
#include "renderer.h"
#include "billboard.h"
#include "debugProc.h"
#include "load.h"
#include "scene3D.h"

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
CEffect3D::CEffect3D() : CScene3D(7, CScene::OBJTYPE_EFFECT)
{
	m_pos = D3DXVECTOR3(0, 0, 0);						// �ʒu
	m_move = D3DXVECTOR3(0, 0, 0);					// �ړ���
	m_posold = D3DXVECTOR3(0, 0, 0);				// �O��̈ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------
//�G�t�F�N�g�N���X �f�X�g���N�^
//--------------------------------------------
CEffect3D::~CEffect3D()
{
}

//--------------------------------------------
//�G�t�F�N�g�̐���
//--------------------------------------------
CEffect3D *CEffect3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
	float fWidth, float fHeight, int nNumMax, int nLife, int TexType)
{
	CEffect3D *pEffect = NULL;

	if (pEffect == NULL)
	{
		//�������𓮓I�m��
		pEffect = new CEffect3D;

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
HRESULT CEffect3D::Init(void)
{
	
	//�e�N�X�`���̐ݒ�
	CEffect3D::BindTexture(CLoad::GetTexture(m_nTexType));

	//�T�C�Y�̐ݒ�
	CEffect3D::SetSize(m_fHeight, m_fWidth);

	//����������
	CScene3D::Init(m_pos);

	m_fAlpha = 1.0f / (float)m_nLife;
	//m_fAlpha = m_fAlpha / 60;
	m_nCntTimer = 0;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEffect3D::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEffect3D::Update(void)
{
	if (m_nTexType == CLoad::TEXTURE_EFFECT_NORMAL001)
	{
		//�����p�[�e�B�N��
		UpdateMoney();
	}

	if(m_nTexType == CLoad::TEXTURE_EFFECT_NORMAL000)
	{
		//���p�[�e�B�N��
		UpdateSalt();
	}

	if (m_nTexType == CLoad::TEXTURE_EFFECT_CONFETTI)
	{
		//���p�[�e�B�N��
		UpdateConfetti();
	}
}

//=============================================================================
// �����̍X�V����
//=============================================================================
void CEffect3D::UpdateMoney(void)
{
	//�����p�̎��S�t���O�ϐ�
	bool bDestroy = false;

	m_nCntTimer++;
	m_DrawType = 0;

	/*D3DXVECTOR3 Parpos1 = D3DXVECTOR3(sinf(fAngle) * fLength1, 0.0f, cosf(fAngle) * fLength1);
	D3DXVECTOR3 Parpos2 = D3DXVECTOR3(sinf(fAngle) * fLength2, 0.0f, cosf(fAngle) * fLength2);*/

	if (m_nLife > 0)
	{
		m_nLife--;

		//�d��
		m_move.y -= cosf(D3DX_PI * 0) * 0.005f;

		//�ʒu���X�V		
		m_pos += m_move;
		m_rot.x += 0.06f;

		//���X�ɓ����ɂ��Ă���
		//m_Col.a = m_Col.a - m_fAlpha;

		//���ȉ��ɂȂ�����0��
		if (m_Col.a < 0.01f)
		{
			m_Col.a = 0;
		}
		//�F��ݒ�
		CScene3D::SetColor(m_Col);

		//�ݒ菈��
		CScene3D::SetSize(m_fHeight, m_fWidth);
		CScene3D::SetPos(m_pos);
		CScene3D::SetRot(m_rot);
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
// �����̍X�V����
//=============================================================================
void CEffect3D::UpdateSalt(void)
{
	//�����p�̎��S�t���O�ϐ�
	bool bDestroy = false;

	m_nCntTimer++;
	m_DrawType = 0;

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
		CScene3D::SetColor(m_Col);

		//�ݒ菈��
		CScene3D::SetSize(m_fHeight, m_fWidth);
		CScene3D::SetPos(m_pos);
		CScene3D::SetRot(m_rot);
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
// �����̍X�V����
//=============================================================================
void CEffect3D::UpdateUI(void)
{
	//�����p�̎��S�t���O�ϐ�
	bool bDestroy = false;

	m_nCntTimer++;
	m_DrawType = 0;

	/*D3DXVECTOR3 Parpos1 = D3DXVECTOR3(sinf(fAngle) * fLength1, 0.0f, cosf(fAngle) * fLength1);
	D3DXVECTOR3 Parpos2 = D3DXVECTOR3(sinf(fAngle) * fLength2, 0.0f, cosf(fAngle) * fLength2);*/

	if (m_nLife > 0)
	{
		m_nLife--;

		//�ʒu���X�V		
		m_pos += m_move;
		m_rot.x += 0.2f;

		//���X�ɓ����ɂ��Ă���
		m_Col.a = m_Col.a - m_fAlpha;

		//���ȉ��ɂȂ�����0��
		if (m_Col.a < 0.01f)
		{
			m_Col.a = 0;
		}
		//�F��ݒ�
		CScene3D::SetColor(m_Col);

		//�ݒ菈��
		CScene3D::SetSize(m_fHeight, m_fWidth);
		CScene3D::SetPos(m_pos);
		CScene3D::SetRot(m_rot);
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
// �����̍X�V����
//=============================================================================
void CEffect3D::UpdateConfetti(void)
{
	//�����p�̎��S�t���O�ϐ�
	bool bDestroy = false;

	m_nCntTimer++;
	m_DrawType = 1;

	/*D3DXVECTOR3 Parpos1 = D3DXVECTOR3(sinf(fAngle) * fLength1, 0.0f, cosf(fAngle) * fLength1);
	D3DXVECTOR3 Parpos2 = D3DXVECTOR3(sinf(fAngle) * fLength2, 0.0f, cosf(fAngle) * fLength2);*/

	if (m_nLife > 0)
	{
		m_nLife--;

		//�d��
		m_move.y -= cosf(D3DX_PI * 0) * 0.000009f;

		//�ʒu���X�V		
		m_pos += m_move;
		m_rot.x += 0.06f;

		//���X�ɓ����ɂ��Ă���
		//m_Col.a = m_Col.a - m_fAlpha;

		//���ȉ��ɂȂ�����0��
		if (m_Col.a < 0.01f)
		{
			m_Col.a = 0;
		}
		//�F��ݒ�
		CScene3D::SetColor(m_Col);

		//�ݒ菈��
		CScene3D::SetSize(m_fHeight, m_fWidth);
		CScene3D::SetPos(m_pos);
		CScene3D::SetRot(m_rot);
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
void CEffect3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	if (m_DrawType == 0)
	{

		// ���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		CScene3D::Draw();

		// ���u�����f�B���O�����ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	else if (m_DrawType == 1)
	{
		CScene3D::Draw();
	}

	//���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}