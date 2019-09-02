//=============================================================================
//
// ���C������ [game.cpp]
// Author :
//
//=============================================================================
#include "game.h"
#include "manager.h"
#include "light.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "CPU.h"
#include "scene3D.h"
#include "game.h"
#include "meshField.h"
#include "dohyo.h"
#include "shadow.h"
#include "debugProc.h"
#include "scene.h"
#include "fade.h"
#include "input.h"
#include "BattleSystem.h"
#include "customer.h"
#include "dohyocircle.h"
#include <time.h>
#include "gauge.h"
#include "SansoGauge.h"
#include "UltimateGauge.h"
#include "effect.h"
#include "jankenUI.h"
#include "field.h"
#include "wall.h"
#include "touzai.h"
#include "animation.h"
#include "load.h"
#include "effect3D.h"
#include "winnerUI.h"
#include "effect2D.h"
#include "2Danimation.h"
#include "ougiUI.h"
#include "Object3DModel.h"
#include "number.h"
#include "UITime.h"
#include "pause.h"
#include "numPlayer.h"

//============================================================================
//	�}�N����`
//============================================================================
#define SIZE_X (SCREEN_WIDTH)
#define SIZE_Y (SCREEN_HEIGHT)
#define COLISIONSIZE (20.0f)
#define TIME_INI		(60)
//============================================================================
//�ÓI�����o�ϐ��錾
//============================================================================
CScene3D *CGame::m_pScene3D = NULL;
CPlayer *CGame::m_pPlayer = NULL;
CEnemy *CGame::m_pEnemy = NULL;
CShadow *CGame::m_pShadow[MAX_SHADOW] = {};
CMeshField *CGame::m_pMeshField = NULL;
CBattleSys *CGame::m_pBatlteSys = NULL;
CGauge *CGame::m_pGauge = NULL;
CUltimateGauge *CGame::m_pUltimateGauge = NULL;
CSansoGauge *CGame::m_pSansoGauge = NULL;
CUITime *CGame::m_pUITime = NULL;
CWinnerUI *CGame::m_pWinnerUI = NULL;

bool CGame::m_bHit = false;
CGame::STATE CGame::m_State = CGame::STATE_START;
CGame::WINNER CGame::m_Winner = CGame::WINNER_NONE;

//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CGame::CGame()
{
	m_n1P = 0;
	m_n2P = 0;
	m_nWin1P = 0;
	m_nWin2P = 0;
	m_WinerNum = 0;
	m_bDetermine = false;
	m_nBattleResetTimer = 0;
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// �|���S���̏���������
//=============================================================================
void CGame::Init(void)
{
	m_bHit = false;
	m_bUI = true;
	m_State = STATE_START;
	m_Winner = WINNER_NONE;
	m_bDetermine = false;

	//�C���X�^���X
	CManager *pManager = NULL;

	LoadChara();

	CDohyo::LoadMat();
	CDohyo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_pGauge = CGauge::Create(D3DXVECTOR3(100, 100, 0));
	m_pWinnerUI = CWinnerUI::Create(D3DXVECTOR3(300, 50, 0));
	m_pUltimateGauge = CUltimateGauge::Create(D3DXVECTOR3(100, 150, 0));
	m_pSansoGauge = CSansoGauge::Create(D3DXVECTOR3(100, 680, 0));

	int nCntZ,nCntX;
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

	for (nCntZ = 0; nCntZ < 3; nCntZ++)
	{//�E�q
		for (nCnt = 0; nCnt < 3; nCnt++)
		{//�q

			CCustomer::Create(D3DXVECTOR3(300.0f + (nCnt * 80.0f), 1.0f, -80.0f + (nCntZ * 70.0f)), D3DXVECTOR3(0.0f, -300.0f, 0.0f), rand() % 3 + 3, CCustomer::POSITION_RIGHT);
		}
	}

	for (nCntZ = 0; nCntZ < 3; nCntZ++)
	{//���q
		for (nCnt = 0; nCnt < 3; nCnt++)
		{//�q

			CCustomer::Create(D3DXVECTOR3(-450.0f + (nCnt * 80.0f), 1.0f, -80.0f + (nCntZ * 70.0f)), D3DXVECTOR3(0.0f, 300.0f, 0.0f), rand() % 3 + 3, CCustomer::POSITION_LEFT);
		}
	}

	if (m_pScene3D == NULL)
	{
		//m_pScene3D = CScene3D::Create(D3DXVECTOR3(200.0f, 0.0f, 0.0f));
	}

	if (m_pPlayer == NULL)
	{// �v���C���[
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(-170.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	}

	if (m_pEnemy == NULL)
	{// �G�l�~�[
		if (CNumPlayer::GetMode() == CNumPlayer::MODE_1P)
		{
			m_pEnemy = CEnemy::Create(D3DXVECTOR3(170.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), CEnemy::MODE_CPU);
		}
		else if (CNumPlayer::GetMode() == CNumPlayer::MODE_2P)
		{
			m_pEnemy = CEnemy::Create(D3DXVECTOR3(170.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), CEnemy::MODE_P2);
		}
	}

	if (m_pMeshField == NULL)
	{
		// ���b�V���t�B�[���h�̐���
		//	m_pMeshField = CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	CDohyoCircle::Create(D3DXVECTOR3(0, 25.0f, 0), 0.0f, 0.0f);

	CField::Create(D3DXVECTOR3(0, -10.0f, 0), 700.0f);

	CWall::Create(D3DXVECTOR3(0, 200.0f, 500), D3DXVECTOR3(300.0f, 0.0f, 0.0f), 200.0f, 700.0f);
	CWall::Create(D3DXVECTOR3(-550, 200.0f, 0), D3DXVECTOR3(300.0f, 300.0f, 0.0f), 200.0f, 700.0f);

	CWall::Create(D3DXVECTOR3(0, 200.0f, -500), D3DXVECTOR3(300.0f, 600.0f, 0.0f), 200.0f, 700.0f);
	CWall::Create(D3DXVECTOR3(550, 200.0f, 0), D3DXVECTOR3(300.0f, 900.0f, 0.0f), 200.0f, 700.0f);

	// 2D�|���S���̐���
	//CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH -50.0f, 50.0f, 0.0f));

	// 3D�|���S���̐���
	//CScene3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 3D���f��
	//CSceneX::Create(D3DXVECTOR3(0.0f, 25.0f, 0.0f));

	if (m_pShadow[0] == NULL)
	{
		m_pShadow[0] = CShadow::Create(D3DXVECTOR3(-80.0f, 20.0f, 0.0f), 20.0f, 20.0f);
	}
	if (m_pShadow[1] == NULL)
	{
		m_pShadow[1] = CShadow::Create(D3DXVECTOR3(170.0f, 20.0f, 0.0f), 20.0f, 20.0f);
	}

	if (m_pBatlteSys == NULL)
	{
		m_pBatlteSys = CBattleSys::Create();
	}

	CTouzai::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != NULL)
	{
		pCamera->Init();
	}

	//��������
	m_pUITime = CUITime::Create(D3DXVECTOR3(680, 70, 0), 70, 70);
	//�|�[�Y����
	CPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), 150);

	//C3DObject::Create(D3DXVECTOR3(0.0f, 20.0f, 90.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CLoad::MODE_GYOUZI);


	m_nTime = 0;
	m_nWin1P = 0;
	m_nWin2P = 0;
	m_WinerNum = 0;
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void CGame::Uninit(void)
{
	CDohyo::UnloadMat();
	m_pPlayer = NULL;
	m_pEnemy = NULL;
	m_pMeshField = NULL;
	m_pGauge = NULL;

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		m_pShadow[nCntShadow] = NULL;
	}

	if (m_pBatlteSys != NULL)
	{
		//�������̊J��
		delete m_pBatlteSys;
		m_pBatlteSys = NULL;
	}


	//�S�Ă̏I������
	CScene::ReleseAll();
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void CGame::Update(void)
{
	CManager *pManager = NULL;
	CFade *pFade = pManager->GetFade();

	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	// �����_���Ȓl���X�V
	srand((unsigned int)time(NULL));


	if (m_pPlayer != NULL && m_pEnemy != NULL)
	{
		m_bHit = Collision(&m_pPlayer->GetPosition(), COLISIONSIZE, &m_pEnemy->GetPosition(), COLISIONSIZE);
	}

	CDebugProc::Print("c", "�Q�[�����[�h");

	m_nTime++;

	//�C�ӂ̃L�[��
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
	//	pFade->SetFade(pManager->MODE_RESULT, pFade->FADE_OUT);
	}
	if (pInputKeyboard->GetTrigger(DIK_BACKSPACE) == true)
	{
		pFade->SetFade(pManager->MODE_GAME, pFade->FADE_OUT);

	}
	if (pInputKeyboard->GetTrigger(DIK_9) == true)
	{
		pFade->SetFade(pManager->MODE_ULTIMATE, pFade->FADE_OUT);
	}

	if (CScene::GetbPause() == false)
	{
		if (m_pBatlteSys != NULL)
		{
			m_pBatlteSys->Update();
		}
	}

	if (m_pUITime != NULL)
	{
	//	m_pUITime->Update();
	}

	if (m_State == STATE_GAME)
	{
		if (m_bUI == true)
		{// ����񂯂��UI���o��
			CJankenUI::Create(D3DXVECTOR3(200.0f, 500.0f, 0.0f));
			m_bUI = false;
			m_pPlayer->SetbJanken(true);
			m_pEnemy->SetbJanken(true);
		}
	}

	//���s����
	if (m_Winner == WINNER_PLAYER1)
	{
		m_pUITime->SetTimeStop(true);
		m_bDetermine = false;
		m_nBattleResetTimer++;
		m_pWinnerUI->SetDrawSyouhai(true);
		if (m_nBattleResetTimer > 120)
		{
			if (m_bDetermine == false)
			{
				m_nWin1P++;
				m_pWinnerUI->SetbWinner(true);
				m_pWinnerUI->Update();
				m_Winner = WINNER_NONE;
				if (m_nWin1P != 3)
				{
					m_pBatlteSys->ResetBattle();
					m_pUITime->SetTime(TIME_INI);
					m_pUITime->SetTimeStop(false);
					m_pWinnerUI->SetDrawSyouhai(false);
				}
				m_bDetermine = true;
				//�^�C�}�[������
				m_nBattleResetTimer = 0;
			}
		}
	}
	else if (m_Winner == WINNER_PLAYER2)
	{
		m_pUITime->SetTimeStop(true);
		m_bDetermine = false;
		m_nBattleResetTimer++;
		m_pWinnerUI->SetDrawSyouhai(true);

		if (m_nBattleResetTimer > 120)
		{
			if (m_bDetermine == false)
			{
				m_nWin2P++;
				m_pWinnerUI->SetbWinner(true);
				m_pWinnerUI->Update();
				m_Winner = WINNER_NONE;
				if (m_nWin2P != 3)
				{
					m_pBatlteSys->ResetBattle();
					m_pUITime->SetTime(TIME_INI);
					m_pUITime->SetTimeStop(false);
					m_pWinnerUI->SetDrawSyouhai(false);
				}
				m_bDetermine = true;
				//�^�C�}�[������
				m_nBattleResetTimer = 0;
			}
		}
	}

	//�����̐���3���
	if (m_nWin1P == 3 || m_nWin2P == 3)
	{
		if (m_nWin1P == 3)
		{
			m_WinerNum = 1;
		}
		else if (m_nWin2P == 3)
		{
			m_WinerNum = 2;
		}
		SaveWinner();
		pFade->SetFade(pManager->MODE_RESULT, pFade->FADE_OUT);
	}
#ifdef _DEBUG

	if (pInputKeyboard->GetTrigger(DIK_7) == true)
	{
		m_Winner = WINNER_PLAYER1;
	}
	if (pInputKeyboard->GetTrigger(DIK_8) == true)
	{
		m_Winner = WINNER_PLAYER2;
	}

	if (m_bHit == true)
	{
		CDebugProc::Print("c", "�������Ă���");
	}
	else
	{
		CDebugProc::Print("c", "�������Ă��Ȃ�");
	}


	CDebugProc::Print("cn", " �v���C���[�̏�����  : ", m_nWin1P);
	CDebugProc::Print("cn", " �G�l�~�[�̏�����  : ", m_nWin2P);

	//�G�t�F�N�g�p�֐�
	D3DXVECTOR3 moveRand;
	D3DXVECTOR3 PosRand;
	D3DXVECTOR3 effectmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�C�ӂ̃L�[��
	if (pInputKeyboard->GetTrigger(DIK_J) == true)
	{
		//�A�j���[�V�����e�N�X�`���̐���
		//(Pos / Rot / Col / Height / Width / UV_U / UV_V / �A�j���[�V�����X�s�[�h / �A�j���[�V�����̐� /
		//���[�v���邩���Ȃ���(0:����/ 1:���Ȃ�) / ���Z�������邩���Ȃ���(0:����/ 1:���Ȃ�))

		/*CAnimation::Create(D3DXVECTOR3(0, 100.0f, 0), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		100.0f, 100.0f, 0.1666666666666667f, 1.0f, 3, 5, 1, 0);*/

		//�I�[��
		//CAnimation::Create(D3DXVECTOR3(0, 100, 0), D3DXVECTOR3(-1.57f, 0, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			//50.0f, 50.0f, 0.166666666666667f, 1.0f, 3,6, 1, 0,CLoad::TEXTURE_EFFECT_WAVE);

		////�Ή�
		//CAnimation::Create(D3DXVECTOR3(0, 100, 0), D3DXVECTOR3(-1.57f, 0, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		//	50.0f, 50.0f, 0.0526315789473684f, 1.0f, 1, 19, 0, 1, CLoad::TEXTURE_EFFECT_SPARK);

		////�Ռ��g
		//CAnimation::Create(D3DXVECTOR3(0, 100, 0), D3DXVECTOR3(-1.57f, 0, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		//	50.0f, 50.0f, 0.1f, 1.0f, 2, 10, 1, 0, CLoad::TEXTURE_EFFECT_WAVE);

		////��
		//CAnimation::Create(D3DXVECTOR3(0, 100, 0), D3DXVECTOR3(-1.57f, 0, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		//	50.0f, 50.0f, 0.0714285714285714f, 1.0f, 2, 14, 1, 0, CLoad::TEXTURE_EFFECT_SMOKE);

		//0.066666666666667f
		/*CAnimation::Create(D3DXVECTOR3(0, 100, 0), D3DXVECTOR3(-1.57f, 0, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			50.0f, 50.0f, 0.0526315789473684f, 1.0f, 1, 19, 1, 0, CLoad::TEXTURE_EFFECT_SPARK);
		CAnimation::Create(D3DXVECTOR3(0, 100, 0), D3DXVECTOR3(-1.57f, 0, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			50.0f, 50.0f, 0.0526315789473684f, 1.0f, 1, 19, 1, 0, CLoad::TEXTURE_EFFECT_SPARK);*/
		// 0.0588235294117647f

		//�A�j���[�V�����e�N�X�`���̐���
		//(Pos / Col / Height / Width / UV_U / UV_V / �A�j���[�V�����X�s�[�h / �A�j���[�V�����̐� /
		//���[�v���邩���Ȃ���(0:����/ 1:���Ȃ�) / ���Z�������邩���Ȃ���(0:����/ 1:���Ȃ�))
		//���`�A�j���[�V����
	/*	C2DAnimation::Create(D3DXVECTOR3(200, 300, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			200.0f, 0.0333333333333333f, 1.0f, 1, 30, 0, 1);

		COugiUI::Create(D3DXVECTOR3(200, 300, 0));*/

		/*CEffect2D::Create(D3DXVECTOR3(200.0f, 400.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1, 1, 1, 0.4f),
		10, 10, 50, CLoad::TEXTURE_EFFECT_WAVE);*/

			for (int nCnt = 0; nCnt < 20; nCnt++)
			{
				//CEffect2D::Create(D3DXVECTOR3(300.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1, 1, 1, 0.5f),
				//100, 100, 200, CLoad::TEXTURE_EFFECT_NORMAL001);

				//����
				moveRand.x = sinf((rand() % 628) / 100.0f) * ((rand() % 6 + 1));
				moveRand.y = cosf((rand() % 628) / 20.0f) * ((rand() % 5 + 2));
				moveRand.z = cosf((rand() % 628) / 100.0f) * ((rand() % 4 + 1));


				CEffect3D::Create(D3DXVECTOR3(0.0f, 400.0f, 0.0f), D3DXVECTOR3(moveRand.x, moveRand.y, moveRand.z), D3DXCOLOR(1, 1, 1, 1),
					20, 20, 1, 200, CLoad::TEXTURE_EFFECT_NORMAL001);

				CDebugProc::Print("%.1f,%.1f,%.1f", moveRand.x, moveRand.y, moveRand.z);
			}
		}
#endif
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// 3D�|���S���̎擾
//=============================================================================
CScene3D *CGame::GetScene3D(void)
{
	return m_pScene3D;
}

//=============================================================================
// �v���C���[�̎擾
//=============================================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// �v���C���[�̎擾
//=============================================================================
CEnemy * CGame::GetEnemy(void)
{
	return m_pEnemy;
}

//=============================================================================
// �e�̎擾
//=============================================================================
CShadow *CGame::GetShadow(int nIdx)
{
	return m_pShadow[nIdx];
}

//=============================================================================
// ���b�V���t�B�[���h�̎擾
//=============================================================================
CMeshField *CGame::GetMeshField(void)
{
	return m_pMeshField;
}

//=============================================================================
// �Q�[�W�̎擾
//=============================================================================
CGauge *CGame::GetGauge(void)
{
	return m_pGauge;
}

//=============================================================================
// ���Ԑ؂�
//=============================================================================
void CGame::TimeOver(void)
{
	float Life1P, Life2P;	// ���E�̒l

	Life1P = m_pGauge->GetGaugeRight();
	Life2P = m_pGauge->GetGaugeLeft();

	if (Life1P < Life2P)
	{
		m_Winner = WINNER_PLAYER1;
	}
	else if(Life2P < Life1P)
	{
		m_Winner = WINNER_PLAYER2;
	}
}

//=============================================================================
// ���҂��Z�[�u
//=============================================================================
void CGame::SaveWinner(void)
{
	FILE *pFileW;

	// �t�@�C�����J��
	pFileW = fopen("data\\TEXT\\Winner.txt", "w");

	if (pFileW != NULL)
	{// �t�@�C�����J������
		//���҂�No
		fprintf(pFileW, "%d\n", m_WinerNum);

		if (m_WinerNum == 1)
		{	//�ǂ̃L������
			fprintf(pFileW, "%d\n", m_n1P);
		}
		else
		{	//�ǂ̃L������
			fprintf(pFileW, "%d\n", m_n2P);
		}

		if (CNumPlayer::GetMode() == CNumPlayer::MODE_1P)
		{	//���[�h
			fprintf(pFileW, "%d\n", 0);
		}
		else if (CNumPlayer::GetMode() == CNumPlayer::MODE_2P)
		{	//���[�h
			fprintf(pFileW, "%d\n", 1);
		}
		//�t�@�C�������
		fclose(pFileW);
	}
	else
	{// �t�@�C�����J���Ȃ�������
		MessageBox(0, "NULL�ł���", "�x��", MB_OK);
	}
}

//=============================================================================
// �u���b�N�Ƃ̓����蔻�菈��
//=============================================================================
bool CGame::Collision(D3DXVECTOR3 *pos0, float fRadius0, D3DXVECTOR3 *pos1, float fRadius1)
{
	bool bHit = false;	// �������Ă��Ȃ����

	// ���S�ƒ��S�̍������߂�
	D3DXVECTOR3 DiffLength = D3DXVECTOR3(pos0->x - pos1->x, pos0->y - pos1->y, pos0->z - pos1->z);

	// ���S���璆�S�̃x�N�g���̒������Z�o
	float fLength = sqrtf((DiffLength.x * DiffLength.x) + (DiffLength.y * DiffLength.y) + (DiffLength.z * DiffLength.z));

	if (fLength < fRadius0 + fRadius1)
	{// ���������a�̘a��菬������Γ������Ă���
		bHit = true;
	}

	return bHit;	// �u���b�N�ɓ������Ă��邩�ǂ�����Ԃ�
}

//=============================================================================
// �g���Ă��郂�f���̃��[�h
//=============================================================================
void CGame::LoadChara(void)
{
	FILE *pFile = NULL;

	CManager::MODE mode;
	mode = CManager::GetMode();

	int nCharType = 0;

	pFile = fopen("data\\TEXT\\charaSave.txt", "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%d", &m_n1P);
		fscanf(pFile, "%d", &m_n2P);

		fclose(pFile);
	}
}

