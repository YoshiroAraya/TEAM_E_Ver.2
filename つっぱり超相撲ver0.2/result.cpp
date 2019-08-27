//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : �ڍ�������
//
//=============================================================================
#include "result.h"
#include "debugProc.h"
#include "input.h"
#include "manager.h"
#include "fade.h"
#include "newsBG.h"
#include "newsCaster.h"
#include "dohyo.h"
#include "dohyocircle.h"
#include "field.h"
#include "wall.h"
#include "logo.h"
#include "time.h"
#include "load.h"
#include "numPlayer.h"
#include "player.h"
#include "enemy.h"
#include "camera.h"
#include "modeSelect.h"
#include "Object3DModel.h"
#include "customer.h"
#include "effect2D.h"
#include "effect3D.h"

//============================================================================
//	�}�N����`
//============================================================================

//============================================================================
//�ÓI�����o�ϐ��錾
//============================================================================
CPlayer *CResult::m_pPlayer = NULL;
CEnemy *CResult::m_pEnemy = NULL;

//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CResult::CResult()
{
	m_nCntTurn = 0;
	m_nCntReturn = 0;
	m_nCntDrawTimer = 0;
	m_bDrawSelect = false;
	m_nWinner = 0;
	m_n1P = 0;				//�I�񂾃��f��No
	m_n2P = 0;				//�I�񂾃��f��No

	m_pWinner2D = NULL;
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// �|���S���̏���������
//=============================================================================
void CResult::Init(void)
{
	m_bSetDohyo = true;
	m_bTurn = false;
	m_nCntTurn = 0;
	m_nCntReturn = 0;
	m_bDrawSelect = false;
	CDohyo::LoadMat();

	CDohyo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CDohyoCircle::Create(D3DXVECTOR3(0, 25.0f, 0), 0.0f, 0.0f);
	CField::Create(D3DXVECTOR3(0, -10.0f, 0), 700.0f);
	CWall::Create(D3DXVECTOR3(0, 200.0f, 500), D3DXVECTOR3(300.0f, 0.0f, 0.0f), 200.0f, 700.0f);
	CWall::Create(D3DXVECTOR3(-550, 200.0f, 0), D3DXVECTOR3(300.0f, 300.0f, 0.0f), 200.0f, 700.0f);
	CWall::Create(D3DXVECTOR3(0, 200.0f, -500), D3DXVECTOR3(300.0f, 600.0f, 0.0f), 200.0f, 700.0f);
	CWall::Create(D3DXVECTOR3(550, 200.0f, 0), D3DXVECTOR3(300.0f, 900.0f, 0.0f), 200.0f, 700.0f);
	C3DObject::Create(D3DXVECTOR3(0.0f, 20.0f, -10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CLoad::MODEL_TROPHY);

	int nCntZ, nCntX;
	int nCnt;

	for (nCntZ = 0; nCntZ < 4; nCntZ++)
	{//���q
		for (nCnt = 0; nCnt < 4; nCnt++)
		{//�q

			CCustomer::Create(D3DXVECTOR3(-120.0f + (nCnt * 80.0f), 1.0f, 230.0f + (nCntZ * 120.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f), rand() % 3 + 3, CCustomer::POSITION);
		}
	}
	for (nCntX = 0; nCntX < 3; nCntX++)
	{//����
		for (nCnt = 0; nCnt < 5 - nCntX; nCnt++)
		{//�q

			CCustomer::Create(D3DXVECTOR3(-490.0f + (nCnt * 50.0f) + (nCntX * 100.0f), 1.0f, 150.0f + (nCnt * 70.0f) - (nCntX * 0.0f)), D3DXVECTOR3(0.0f, -1.5f + (nCnt * 0.4f), 0.0f), rand() % 4 + 3, CCustomer::POSITION);
		}
	}
	for (nCntX = 0; nCntX < 3; nCntX++)
	{//���E
		for (nCnt = 0; nCnt < 5 - nCntX; nCnt++)
		{//�q

			CCustomer::Create(D3DXVECTOR3(470.0f - (nCnt * 50.0f) - (nCntX * 100.0f), 1.0f, 150.0f + (nCnt * 70.0f) - (nCntX * 0.0f)), D3DXVECTOR3(0.0f, 1.5f - (nCnt * 0.4f), 0.0f), rand() % 4 + 3, CCustomer::POSITION);
		}
	}

	LoadWinner();

	if (m_pWinner2D == NULL)
	{	//�ʒu�ݒ�
		m_pWinner2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 400, 0));
		//������
		m_pWinner2D->SetWidthHeight(800,300);
		if (m_nWinner == 1)
		{
			m_pWinner2D->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_WINNER_RESULT));
			//���_���ւ̃|�C���^
			VERTEX_2D *pVtx;
			// ���_�o�b�t�@�ւ̃|�C���^
			LPDIRECT3DVERTEXBUFFER9	pVtxBuff;
			//���_�o�b�t�@���擾
			pVtxBuff = m_pWinner2D->GetBuff();
			//���_�o�b�t�@�����b�N�����_�f�[�^�̃|�C���^���擾
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			//���_�J���[
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.3333f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 0.3333f);
			// ���_�o�b�t�@���A�����b�N����
			pVtxBuff->Unlock();

		}
		if (m_nWinner == 2)
		{
			m_pWinner2D->BindTexture(CLoad::GetTexture(CLoad::TEXTURE_WINNER_RESULT));
			//���_���ւ̃|�C���^
			VERTEX_2D *pVtx;
			// ���_�o�b�t�@�ւ̃|�C���^
			LPDIRECT3DVERTEXBUFFER9	pVtxBuff;
			//���_�o�b�t�@���擾
			pVtxBuff = m_pWinner2D->GetBuff();
			//���_�o�b�t�@�����b�N�����_�f�[�^�̃|�C���^���擾
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			//���_�J���[
			if (m_nMode == 0)
			{	//CPU
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.6666f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.6666f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			else if (m_nMode == 1)
			{	//2P
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.3333f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.3333f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 0.6666f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 0.6666f);
			}

			// ���_�o�b�t�@���A�����b�N����
			pVtxBuff->Unlock();
		}
		m_pWinner2D->SetbDraw(true);
	}

	CManager::GetCamera()->Init();
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void CResult::Uninit(void)
{
	m_pPlayer = NULL;
	m_pEnemy = NULL;
	CNewsCaster::UnloadMat();
	CDohyo::UnloadMat();

	//�S�Ă̏I������
	CScene::ReleseAll();
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void CResult::Update(void)
{
	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	CManager *pManager = NULL;
	CFade *pFade = pManager->GetFade();

	CNumPlayer::MODE mode;
	mode = CNumPlayer::GetMode();

	//�C�ӂ̃L�[
	//if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	//{
	////	pFade->SetFade(pManager->MODE_TITLE, pFade->FADE_OUT);
	//}

	m_nCntDrawTimer++;
	if (m_nCntDrawTimer > 240)
	{
		if (m_bDrawSelect == false)
		{
			CModeSelect::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), 150);
			m_bDrawSelect = true;
		}
	}


#ifdef _DEBUG
	CDebugProc::Print("c", "���U���g");

	//�G�t�F�N�g�p�֐�
	D3DXVECTOR3 moveRand;
	////�C�ӂ̃L�[��
	//if (pInputKeyboard->GetTrigger(DIK_J) == true)
	//{
	//	for (int nCnt = 0; nCnt < 1; nCnt++)
	//	{
	//		//CEffect2D::Create(D3DXVECTOR3(300.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1, 1, 1, 0.5f),
	//		//100, 100, 200, CLoad::TEXTURE_EFFECT_NORMAL001);
	//		//����
	//		moveRand.x = sinf((rand() % 628) / 100.0f) * ((rand() % 1 + 1));
	//		moveRand.y = cosf((rand() % 628) / 20.0f) * ((rand() % 8 + 5));
	//		moveRand.z = cosf((rand() % 628) / 100.0f) * ((rand() % 1 + 1));

	//		CEffect3D::Create(D3DXVECTOR3(0.0f, 400.0f, 0.0f), D3DXVECTOR3(moveRand.x, moveRand.y, moveRand.z), D3DXCOLOR(1, 1, 1, 1),
	//			20, 20, 1, 200, CLoad::TEXTURE_EFFECT_NORMAL001);

	//		//��(�G���[)
	//		//CEffect::Create(D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(moveRand.x, 0.5f, moveRand.z), D3DXCOLOR(1, 1, 1, 1),
	//		//10, 10, 1, 60, CLoad::TEXTURE_EFFECT_NORMAL002);
	//	}
	//}
#endif
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CResult::Draw(void)
{

}

//=============================================================================
// ���҂̓ǂݍ��ݏ���
//=============================================================================
void CResult::LoadWinner(void)
{
	FILE *pFile = NULL;

	pFile = fopen("data\\TEXT\\Winner.txt", "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%d\n", &m_nWinner);

		if (m_nWinner == 1)
		{	//�������̂�1P�Ȃ�1P�̃��f����ǂݍ���
			fscanf(pFile, "%d\n", &m_n1P);
		}
		else
		{	//�������̂�2P�Ȃ�2P�̃��f����ǂݍ���
			fscanf(pFile, "%d\n", &m_n2P);
		}

		fscanf(pFile, "%d\n", &m_nMode);

		fclose(pFile);
	}

	if (m_nWinner == 1)
	{
		if (m_pPlayer == NULL)
		{// �v���C���[
			m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 20.0f, 10.0f), D3DXVECTOR3(0.0f, D3DX_PI * 2.0f, 0.0f));
		}
	}
	else if(m_nWinner == 2)
	{
		if (m_pEnemy == NULL)
		{// �G�l�~�[
			m_pEnemy = CEnemy::Create(D3DXVECTOR3(0.0f, 20.0f, 10.0f), D3DXVECTOR3(0.0f,D3DX_PI * 2.0f, 0.0f), CEnemy::MODE_P2);
		}
	}
}
