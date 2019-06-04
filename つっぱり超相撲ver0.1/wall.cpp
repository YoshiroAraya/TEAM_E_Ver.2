//=============================================================================
//
// �ǂ̏��� [field.cpp]
// Author : Atsushi Yamashita
//
//=============================================================================
#include "wall.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
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
CWall::CWall() : CScene3D(4, CScene::OBJTYPE_DOHYOCIRCLE)
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
CWall::~CWall()
{

}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT CWall::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	//�e�N�X�`���̓\��t��
	BindTexture(CLoad::GetTexture(CLoad::TEXTURE_WALL));

	//����������
	CScene3D::SetSize(m_fHeight,m_fWidth);
	CScene3D::Init(m_pos);
	CScene3D::SetRot(m_rot);

	//�F�̐ݒ�
	CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//CScene3D::SetPosition(D3DXVECTOR3(m_pos), m_fWidth, m_fHeight);

	m_fCol = 0.0f;
	return S_OK;
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void CWall::Uninit(void)
{
	//�~�̏I������
	CScene3D::Uninit();
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void CWall::Update(void)
{



	//CDebugProc::print(1, "\n\n\nLength %.1f", fLength);
	//CScene3D::SetPosition(D3DXVECTOR3(m_pos), m_fHeight, m_fWidth);
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CWall::Draw(void)
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
CWall *CWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot ,float fHeight,float fWidth)
{
	CWall *pWall = {};

	if (pWall == NULL)
	{
		//�������𓮓I�m��
		pWall = new CWall;

		if (pWall != NULL)
		{//NULL�`�F�b�N

			pWall->m_pos = pos;
			pWall->m_rot = rot;
			pWall->m_fHeight = fHeight;
			pWall->m_fWidth = fWidth;


			//����������
			pWall->Init();
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
	return pWall;
}