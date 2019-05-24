//=============================================================================
//
// ���̏��� [field.cpp]
// Author : Atsushi Yamashita
//
//=============================================================================
#include "field.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "bullet.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include <string>
#include <time.h>

//============================================================================
//	�ÓI�����o�ϐ��錾
//============================================================================
LPDIRECT3DTEXTURE9 CField::m_pTextures = NULL;

//============================================================================
//	�}�N����`
//============================================================================
#define MAX_HEIGHT (20)   //�i�����j
#define MAX_WITH   (10)   //�i���j

#define MAX_SIZE3D (70)  //3D�|���S���̑傫��
//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CField::CField() : CScene3D(4, CScene::OBJTYPE_DOHYOCIRCLE)
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
CField::~CField()
{

}

//=============================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CField::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/sand2.jpg", &m_pTextures);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̊J������
//=============================================================================
void CField::Unload(void)
{
	//�e�N�X�`���̔j��
	if (m_pTextures != NULL)
	{
		m_pTextures->Release();
		m_pTextures = NULL;
	}
}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT CField::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	//�e�N�X�`���̓\��t��
	BindTexture(m_pTextures);

	//����������
	CScene3D::SetSize(m_fSize, m_fSize);
	CScene3D::Init(m_pos);

	//�F�̐ݒ�
	CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//CScene3D::SetPosition(D3DXVECTOR3(m_pos), m_fWidth, m_fHeight);

	m_fCol = 0.0f;
	return S_OK;
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void CField::Uninit(void)
{
	//�~�̏I������
	CScene3D::Uninit();
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void CField::Update(void)
{
	


	//CDebugProc::print(1, "\n\n\nLength %.1f", fLength);
	//CScene3D::SetPosition(D3DXVECTOR3(m_pos), m_fHeight, m_fWidth);
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CField::Draw(void)
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
CField *CField::Create(D3DXVECTOR3 pos, float fSize)
{
	CField *pField = {};

	if (pField == NULL)
	{
		//�������𓮓I�m��
		pField = new CField;

		if (pField != NULL)
		{//NULL�`�F�b�N

			pField->m_pos = pos;
			pField->m_fSize = fSize;
			

			//����������
			pField->Init();
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
	return pField;
}