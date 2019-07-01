//=============================================================================
//
// �ǂ̏��� [field.cpp]
// Author : Atsushi Yamashita
//
//=============================================================================
#include "newsBG.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "title.h"
#include "load.h"
#include <string>
#include <time.h>

//============================================================================
//	�ÓI�����o�ϐ��錾
//============================================================================

//============================================================================
//	�}�N����`
//============================================================================
#define MAX_HEIGHT (20)   //�i�����j
#define MAX_WITH   (10)   //�i���j

#define MAX_SIZE3D (70)  //3D�|���S���̑傫��
//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CNewsBG::CNewsBG() : CScene3D(4, CScene::OBJTYPE_DOHYOCIRCLE)
{
	//m_pTexture = NULL;					// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;						// ���_�o�b�t�@�ւ̃|�C���^

	m_pos = D3DXVECTOR3(0, 0, 0);			// �|���S���̈ʒu
	m_rot = D3DXVECTOR3(0, 0, 0);			// �|���S���̌���
	m_move = D3DXVECTOR3(0, 0, 0);			// �|���S���̓���

}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CNewsBG::~CNewsBG()
{

}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT CNewsBG::Init(void)
{
	//�e�N�X�`���̓\��t��
	BindTexture(CLoad::GetTexture(CLoad::TEXTURE_NEWS));

	//����������
	CScene3D::SetSize(m_fHeight, m_fWidth);
	CScene3D::Init(m_pos);
	CScene3D::SetRot(m_rot);

	//�F�̐ݒ�
	CScene3D::SetColor(D3DXCOLOR(1.0f * (211.0f / 255.0f), 1.0f * (237.0f / 255.0f), 1.0f * (251.0f / 255.0f), 1.0f));

	CScene3D::SetNor(D3DXVECTOR3(0.0f, 0.0f, 1.0f));

	//CScene3D::SetPosition(D3DXVECTOR3(m_pos), m_fWidth, m_fHeight);

	m_fCol = 0.0f;
	return S_OK;
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void CNewsBG::Uninit(void)
{
	//�~�̏I������
	CScene3D::Uninit();
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void CNewsBG::Update(void)
{
	// �^�C�g���擾
	CTitle *pTitle;
	pTitle = CManager::GetTitle();

	if (pTitle->GetState() == CTitle::STATE_TITLE)
	{
		Uninit();
	}

	//CDebugProc::print(1, "\n\n\nLength %.1f", fLength);
	//CScene3D::SetPosition(D3DXVECTOR3(m_pos), m_fHeight, m_fWidth);
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CNewsBG::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	//�`�揈��
	CScene3D::Draw();
}

//=============================================================================
//�~�̐�������
//=============================================================================
CNewsBG *CNewsBG::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth)
{
	CNewsBG *pNewsBG = {};

	if (pNewsBG == NULL)
	{
		//�������𓮓I�m��
		pNewsBG = new CNewsBG;

		if (pNewsBG != NULL)
		{//NULL�`�F�b�N

			pNewsBG->m_pos = pos;
			pNewsBG->m_rot = rot;
			pNewsBG->m_fHeight = fHeight;
			pNewsBG->m_fWidth = fWidth;


			//����������
			pNewsBG->Init();
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
	return pNewsBG;
}