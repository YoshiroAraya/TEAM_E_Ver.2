//=============================================================================
//
// �y�U�i�~�j�̏��� [dohyocircle.cpp]
// Author : Atsushi Yamashita
//
//=============================================================================
#include "dohyocircle.h"
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

#define MAX_SIZE3D (135)  //3D�|���S���̑傫��
//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CDohyoCircle::CDohyoCircle() : CScene3D(5, CScene::OBJTYPE_DOHYOCIRCLE)
{
	//m_pTexture = NULL;						// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;						// ���_�o�b�t�@�ւ̃|�C���^

	m_pos = D3DXVECTOR3(0, 0, 0);			// �|���S���̈ʒu
	m_rot = D3DXVECTOR3(0, 0, 0);			// �|���S���̌���
	m_move = D3DXVECTOR3(0, 0, 0);			// �|���S���̓���

}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CDohyoCircle::~CDohyoCircle()
{

}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT CDohyoCircle::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	//�e�N�X�`���̓\��t��
	BindTexture(CLoad::GetTexture(CLoad::TEXTURE_DOHYOCIRCLE));

	//�T�C�Y�̐ݒ�
	CScene3D::SetSize(MAX_SIZE3D, MAX_SIZE3D);

	//����������
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
void CDohyoCircle::Uninit(void)
{
	//�~�̏I������
	CScene3D::Uninit();
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void CDohyoCircle::Update(void)
{
	CManager *pManager = NULL;
	int nMode = pManager->GetMode();

	if (nMode == CManager::MODE_GAME)
	{
		CGame *pGame = pManager->GetGame();
		D3DXVECTOR3 PlayerPos = pGame->GetPlayer()->GetPosition();
		D3DXVECTOR3 EnemyPos = pGame->GetEnemy()->GetPosition();

		float fLengthPlayer;
		float fLengthEnemy;

		//���S���狗�����v�Z����
		fLengthPlayer = sqrt((PlayerPos.x - m_pos.x) * (PlayerPos.x - m_pos.x) +
			(PlayerPos.z - m_pos.z) * (PlayerPos.z - m_pos.z));

		if (fLengthPlayer > GROUND_SIZE)
		{//�v���C���[�������𒴂�����

		 //�ړI�̏ꏊ���璆�S�_�̊p�x�����߂�
			float fAnglePlayer = atan2f(PlayerPos.x - m_pos.x, PlayerPos.z - m_pos.z);

			//�v���C���[������̊O�ɍs������͈͓��ɖ߂�
			//PlayerPos.x = m_pos.x + sinf(fAnglePlayer) * GROUND_SIZE;
			//PlayerPos.z = m_pos.z + cosf(fAnglePlayer) * GROUND_SIZE;
		}
		//�v���C���[�̈ʒu��ݒ�
		pGame->GetPlayer()->SetPosition(D3DXVECTOR3(PlayerPos));


		//���S���狗�����v�Z����
		fLengthEnemy = sqrt((EnemyPos.x - m_pos.x) * (EnemyPos.x - m_pos.x) +
			(EnemyPos.z - m_pos.z) * (EnemyPos.z - m_pos.z));

		if (fLengthEnemy > GROUND_SIZE)
		{//�G�l�~�[�������𒴂�����

		 //�ړI�̏ꏊ���璆�S�_�̊p�x�����߂�
			float fAngleEnemy = atan2f(EnemyPos.x - m_pos.x, EnemyPos.z - m_pos.z);

			//�v���C���[������̊O�ɍs������͈͓��ɖ߂�
			//EnemyPos.x = m_pos.x + sinf(fAngleEnemy) * GROUND_SIZE;
			//EnemyPos.z = m_pos.z + cosf(fAngleEnemy) * GROUND_SIZE;
		}
		//�v���C���[�̈ʒu��ݒ�
		pGame->GetEnemy()->SetPosition(D3DXVECTOR3(EnemyPos));


		//CDebugProc::print(1, "\n\n\nLength %.1f", fLength);
		//CScene3D::SetPosition(D3DXVECTOR3(m_pos), m_fHeight, m_fWidth);
	}
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CDohyoCircle::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();


	// ���u�����f�B���O�����Z�����ɐݒ�
	/*pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);*/

	//�`�揈��
	CScene3D::Draw();

	// ���u�����f�B���O�����ɖ߂�
	/*pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);*/

}

//=============================================================================
//�~�̐�������
//=============================================================================
CDohyoCircle *CDohyoCircle::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CDohyoCircle *pDohyoCircle = {};

	if (pDohyoCircle == NULL)
	{
		//�������𓮓I�m��
		pDohyoCircle = new CDohyoCircle;

		if (pDohyoCircle != NULL)
		{//NULL�`�F�b�N

			pDohyoCircle->m_pos = pos;
			pDohyoCircle->m_fWidth = fWidth;
			pDohyoCircle->m_fHeight = fHeight;

			//����������
			pDohyoCircle->Init();
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
	return pDohyoCircle;
}