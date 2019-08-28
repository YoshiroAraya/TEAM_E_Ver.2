//=============================================================================
//
// �G�l�~�[���� [CPU.cpp]
// Author : �ڍ� ������
//
//=============================================================================
#include "CPU.h"
#include "player.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "camera.h"
#include "scene3D.h"
#include "meshField.h"
#include "shadow.h"
#include "game.h"
#include "characterMove.h"
#include "load.h"
#include "model.h"
#include "title.h"
#include "Banimation.h"
#include "ultimate.h"
#include "particleX.h"
#include "gauge.h"
#include "SansoGauge.h"
#include "UltimateGauge.h"
#include "BattleSystem.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define CPU_COLLISION			(D3DXVECTOR3(20.0f, 60.0f, 20.0f))		//�G�l�~�[�̓����蔻��
#define DOHYO_HAZI_MAX			(175.0f)
#define DOHYO_HAZI_MIN			(150.0f)
#define DASH_MOVE				(1.2f)
#define PARTICLE_ROT			((rand() % 628) / 100.0f)		//�S����
#define PARTICLE_NUM			(20)							// �ǂɌ��˂����Ƃ��̃p�[�e�B�N���̐�
#define PARTICLE_TIME			(35)							// �ǂɌ��˂����Ƃ��̃p�[�e�B�N���o������
#define FILE_NAME_0				("data\\TEXT\\motion_Wrestler_down.txt")
#define FILE_NAME_1				("data\\TEXT\\motion_Wrestler_up.txt")

#define DOHYO_COLLISION			(20.0f)
#define GUARD_NOW_SANSO			(-2.5f)
#define GUARD_SANSO				(-20.0f)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPD3DXMESH					CCPU::m_pMeshModel[MAX_PARTS][MODEL_PARENT] = {};
LPD3DXBUFFER				CCPU::m_pBuffMatModel[MAX_PARTS][MODEL_PARENT] = {};
LPDIRECT3DTEXTURE9			CCPU::m_pTextureModel[MAX_PARTS][MODEL_PARENT] = {};
DWORD						CCPU::m_nNumMatModel[MAX_PARTS][MODEL_PARENT] = {};
CBAnimation *CCPU::m_pAnimation = NULL;

//=============================================================================
// �G�l�~�[�N���X�̃R���X�g���N�^
//=============================================================================
CCPU::CCPU() : CSceneX(CPU_PRIORITY)
{
	// �l���N���A
	m_pTexture = NULL;							// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;							// ���_�o�b�t�@�ւ̃|�C���^
	m_bLand = false;							// �E�ɂ��邩�ǂ���
	m_bHit = false;									// �E�ɂ��邩�ǂ���
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_State = STATE_NEUTRAL;
	m_Direction = DIRECTION_LEFT;
	m_bRecovery = false;	// �d���t���O
	m_nRecoveryTime = 0;	// �d������
	m_nLife = 0;
	m_bDying = false;
	m_pTuppari = NULL;
	m_DohyoState = DOHYO_NORMAL;
	m_nCounterTime = 0;
	m_bCounter = false;
	m_DohyoHaziLR = HAZI_NORMAL;
	m_fLength = 0.0f;
	m_nOldMotion = 0;	//�O�̃��[�V����
	//m_turnRot = D3DXVECTOR3(0, 0, 0);
	m_fRot = 0.0f;
	m_nSiomakiCnt = 0;
	m_bDash = false;

	for (int nCntParent = 0; nCntParent < MODEL_PARENT; nCntParent++)
	{
		for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
		{
			m_pMeshModel[nCnt][nCntParent] = NULL;
			m_pBuffMatModel[nCnt][nCntParent] = NULL;
			m_pTextureModel[nCnt][nCntParent] = NULL;
			m_nNumMatModel[nCnt][nCntParent] = NULL;
			m_apModel[nCnt][nCntParent] = NULL;
		}
		m_nMotionType[nCntParent] = 0;	//���݂̃��[�V�����^�C�v
		m_nNumParts[nCntParent] = 0;
		m_nKey[nCntParent] = 0;			//���݂̃L�[
		m_nCountFlame[nCntParent] = 0;	//���݂̃t���[��
		m_bMotionEnd[nCntParent] = false;
	}

#ifdef _DEBUG
	m_bColBlockDraw = false;
#endif
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCPU::~CCPU()
{
}

//=============================================================================
// �I�u�W�F�N�g�̐�������
//=============================================================================
CCPU *CCPU::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CCPU *pCPU = NULL;

	if (pCPU == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pCPU = new CCPU;

		if (pCPU != NULL)
		{
			//pCPU->BindModel(CLoad::GetBuffMat(CLoad::MODEL_CPU), CLoad::GetNumMat(CLoad::MODEL_CPU), CLoad::GetMesh(CLoad::MODEL_CPU));
			pCPU->Init(pos, rot);
		}
	}

	return pCPU;
}

//=============================================================================
// �G�l�~�[����������
//=============================================================================
HRESULT CCPU::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �^�C�g���擾
	CGame *pGame;
	pGame = CManager::GetGame();
	CUltimate *pUltimate;
	pUltimate = CManager::GetUltimate();
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{
		if (pGame != NULL)
		{
			// �I�΂ꂽ�L�����N�^�[�̃��f�������蓖��
			if (pGame->Get2P() == 0)
			{// �v���C���[
				BindModel(CLoad::GetBuffMat(CLoad::MODEL_PLAYER), CLoad::GetNumMat(CLoad::MODEL_PLAYER), CLoad::GetMesh(CLoad::MODEL_PLAYER));
			}
			else if (pGame->Get2P() == 1)
			{// �G�l�~�[
				BindModel(CLoad::GetBuffMat(CLoad::MODEL_ENEMY), CLoad::GetNumMat(CLoad::MODEL_ENEMY), CLoad::GetMesh(CLoad::MODEL_ENEMY));
			}
		}
	}
	else if (mode == CManager::MODE_ULTIMATE)
	{
		if (pUltimate != NULL)
		{
			// �I�΂ꂽ�L�����N�^�[�̃��f�������蓖��
			if (pUltimate->Get2P() == 0)
			{// �v���C���[
				BindModel(CLoad::GetBuffMat(CLoad::MODEL_PLAYER), CLoad::GetNumMat(CLoad::MODEL_PLAYER), CLoad::GetMesh(CLoad::MODEL_PLAYER));
			}
			else if (pUltimate->Get2P() == 1)
			{// �G�l�~�[
				BindModel(CLoad::GetBuffMat(CLoad::MODEL_ENEMY), CLoad::GetNumMat(CLoad::MODEL_ENEMY), CLoad::GetMesh(CLoad::MODEL_ENEMY));
			}
		}
	}
	else
	{
		BindModel(CLoad::GetBuffMat(CLoad::MODEL_ENEMY), CLoad::GetNumMat(CLoad::MODEL_ENEMY), CLoad::GetMesh(CLoad::MODEL_ENEMY));
	}

	// 2D�I�u�W�F�N�g����������
	CSceneX::Init(pos);

	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(CScene::OBJTYPE_ENEMY);

	m_move = D3DXVECTOR3(0, 0, 0);
	m_fDestAngle = 0;
	m_fDiffAngle = 0;
	m_bLand = false;					// �E�ɂ��邩�ǂ���
	m_bHit = false;					// �E�ɂ��邩�ǂ���
	m_State = STATE_JANKEN;
	m_Direction = DIRECTION_LEFT;
	m_bDying = false;
	m_bRecovery = false;	// �d���t���O
	m_nRecoveryTime = 0;	// �d������
	m_DohyoState = DOHYO_NORMAL;
	m_nCounterTime = 0;
	m_bCounter = false;
	m_DohyoHaziLR = HAZI_NORMAL;
	m_fLength = sqrtf((pos.x - 0.0f) * (pos.x - 0.0f) + (pos.z - 0.0f) * (pos.z - 0.0f));
	m_bSelect = false;
	m_nSiomakiCnt = 0;
	m_bDash = false;
	m_bWallHit = false;
	m_bUltDis = false;
	m_fRot = 0.0f;

	//���ς萶��
	m_pTuppari = CTuppari::Create(pos);

	if (mode != NULL)
	{
		if (mode == CManager::MODE_GAME || mode == CManager::MODE_ULTIMATE)
		{
			CSceneX::SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		}
		else
		{
			CSceneX::SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		}
	}

	CSceneX::SetRot(rot);

	//���[�V�����p�ϐ�
	for (int nCntParent = 0; nCntParent < MODEL_PARENT; nCntParent++)
	{
		m_nKey[nCntParent] = 0;			//���݂̃L�[
		m_nCountFlame[nCntParent] = 0;		//���݂̃t���[��
		m_nMotionType[nCntParent] = 0;	//���݂̃��[�V�����^�C�v
	}

	m_nOldMotion = 0;	//�O�̃��[�V����
						//�v���C���[���̓ǂݍ���
	FileLoad(FILE_NAME_0, 0);
	FileLoad(FILE_NAME_1, 1);

	//���f���̐e���w��
	m_apModel[0][1]->SetParent(m_apModel[0][0]);

	if (mode == CManager::MODE_GAME)
	{//�Q�[�����[�h�������珈���ɓ���
	}
	return S_OK;
}

//=============================================================================
// �G�l�~�[�I������
//=============================================================================
void CCPU::Uninit(void)
{
	for (int nCntParent = 0; nCntParent < MODEL_PARENT; nCntParent++)
	{
		for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
		{
			if (m_apModel[nCnt][nCntParent] != NULL)
			{
				m_apModel[nCnt][nCntParent]->Uninit();
				delete m_apModel[nCnt][nCntParent];
				m_apModel[nCnt][nCntParent] = NULL;
			}
		}
	}

	if (m_pTuppari != NULL)
	{
		m_pTuppari->Uninit();
	}

	m_pAnimation = NULL;

	// 2D�I�u�W�F�N�g�I������
	CSceneX::Uninit();
}

//=============================================================================
// �G�l�~�[�X�V����
//=============================================================================
void CCPU::Update(void)
{
	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();
	// �ʒu�擾
	D3DXVECTOR3 pos;
	pos = CSceneX::GetPosition();
	// �ʒu�擾
	D3DXVECTOR3 rot;
	rot = CSceneX::GetRot();
	// �J�����擾
	CCamera *pCamera;
	pCamera = CManager::GetCamera();
	// �e�̎擾
	CShadow *pShadow;
	pShadow = CGame::GetShadow();
	// �J�����̌������擾
	D3DXVECTOR3 cameraRot;
	cameraRot = pCamera->GetRot();
	// �v���C���[�擾
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	// �ړ������擾
	CCharacterMove *pCharacterMove;
	pCharacterMove = CManager::GetCharacterMove();
	//�Q�[�W�̎擾
	CUltimateGauge *pULTGauge;
	pULTGauge = CGame::GetUltimateGauge();	//���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();

	// �O�̃t���[���̈ʒu���
	m_posOld = pos;

	float fMoveCPU = MOVE_CPU;	// �G�l�~�[�̈ړ��ʂ�ݒ�

	switch (mode)
	{
	case CManager::MODE_GAME:
		if (CGame::GetState() == CGame::STATE_GAME)
		{
			//�v���C���[�̓���
			fMoveCPU = CPUOperation(pos, fMoveCPU);

			//�^�C�}�[�̍X�V
			TimerUpdate();

			//�p�x�̐ݒ�
			rot = DirectionCPU(pos, rot);

			//�G�Ɠ��������Ƃ�
			CollisionPlayerAction();

			if (pPlayer != NULL)
			{
				TsuppariCollision(pos);
			}
			//���ς�ʒu�X�V
			m_pTuppari->SetPosition(pos);

			//�y�U�۔���
			DohyoHaziWhether(pos);

			if (pULTGauge->GetUlt(0) == true && m_bUltDis == false)
			{
				if (m_pAnimation == NULL)
				{
					m_pAnimation = CBAnimation::Create(D3DXVECTOR3(pos), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
						50.0f, 100.0f, 0.0625f, 1.0f, 1.5f, 16, 0, 0, 1, CLoad::TEXTURE_ANIMATION);
				}
				m_bUltDis = true;
			}
			else if (pULTGauge->GetUlt(0) == false)
			{
				if (m_pAnimation != NULL)
				{
					m_pAnimation->SetDestroy(true);
					m_pAnimation = NULL;
				}
				m_bUltDis = false;
			}
		}
		break;

	case CManager::MODE_TITLE:
		// ��]����
		m_fRot = sinf(D3DX_PI + rot.y);
		m_bSelect = pCharacterMove->CharaTurn(&pos, &rot, m_fRot, m_fLength);
		break;
	case CManager::MODE_ULTIMATE:
		if (pos.x < 550.0f)
		{
			m_move = pCharacterMove->MoveRight(m_move, fMoveCPU * 15.0f);
		}
		else if (pos.x > 550.0f)
		{
			m_bWallHit = true;
			pos.x = 550.0f;
			m_move.x = 0.0f;

		}
		break;
	}

	//�L��������
	EntryCPU(pos, fMoveCPU);


	pos += m_move;

	// �d�͉��Z
	if (mode != CManager::MODE_ULTIMATE)
	{
		m_move.y -= cosf(D3DX_PI * 0.0f) * 0.5f;
	}

	//����
	m_move.x += (0.0f - m_move.x) * 0.5f;
	m_move.z += (0.0f - m_move.z) * 0.5f;
	m_move.y += (0.0f - m_move.y) * 0.01f;

	if (pShadow != NULL)
	{
		pShadow->SetPos(pos);
	}

	// ���f���Ƃ̓����蔻��
	CollisonSceneX(&pos, &D3DXVECTOR3(m_posOld.x, m_posOld.y + 1.0f, m_posOld.z), &m_move, CPU_COLLISION);

	CSceneX::SetPosition(pos);
	CSceneX::SetRot(rot);

	if (pos.y <= 0)
	{
		pos.y = 0;
		CSceneX::SetPosition(pos);
		CGame::SetWinner(CGame::WINNER_PLAYER1);
	}

	//���[�V�����X�V
	UpdateMotion(0);
	UpdateMotion(1);

#ifdef _DEBUG

	int nCnt = 0;

	//CDebugProc::Print("cfccfccfc", "�G�l�~�[�̈ʒu : x", pos.x, "f", "   y", pos.y, "f", "  z", pos.z, "f");
	//CDebugProc::Print("cfccfccfc", "�G�l�~�[��VtxMax : x", CSceneX::GetVtxMax().x, "f", "   y", CSceneX::GetVtxMax().y, "f", "  z", CSceneX::GetVtxMax().z, "f");
	//CDebugProc::Print("cfccfccfc", "�G�l�~�[��VtxMin : x", CSceneX::GetVtxMin().x, "f", "   y", CSceneX::GetVtxMin().y, "f", "  z", CSceneX::GetVtxMin().z, "f");
	CDebugProc::Print("cn", "�G�l�~�[�̏�� : ", m_State);
	CDebugProc::Print("cn", "�G�l�~�[�̌��� : ", m_Direction);
	CDebugProc::Print("cf", "�G�l�~�[�̌��� : ", rot.y);
	if (m_bRecovery == true)
	{
		CDebugProc::Print("c", " �G�l�~�[���J�o���[�@ON ");
	}
	else
	{
		CDebugProc::Print("c", " �G�l�~�[���J�o���[�@OFF ");
	}
	if (m_bCounter == true)
	{
		CDebugProc::Print("c", " �G�l�~�[�J�E���^�[�@ON ");
	}
	else
	{
		CDebugProc::Print("c", " �G�l�~�[�J�E���^�[�@OFF ");
	}
	if (m_DohyoState == DOHYO_NORMAL)
	{
		CDebugProc::Print("c", " �y�U�[�@OFF ");
	}
	else
	{
		CDebugProc::Print("c", " �y�U�[�@ON ");
	}
	if (m_bDying == true)
	{
		CDebugProc::Print("c", "�G�l�~�[ �m�� ");
	}
	else
	{
		CDebugProc::Print("c", "�G�l�~�[ ���� ");
	}
	//if (pInputKeyboard->GetTrigger(DIK_Q) == true)
	//{
	//	m_nMotionType[0]--;
	//	m_nMotionType[1]--;
	//	m_nKey[0] = 0;
	//	m_nKey[1] = 0;
	//	m_nCountFlame[0] = 0;
	//	m_nCountFlame[1] = 0;
	//}
	//if (pInputKeyboard->GetTrigger(DIK_E) == true)
	//{
	//	m_nMotionType[0]++;
	//	m_nMotionType[1]++;
	//	m_nKey[0] = 0;
	//	m_nKey[1] = 0;
	//	m_nCountFlame[0] = 0;
	//	m_nCountFlame[1] = 0;
	//}
	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		//3�����Z ���P?���Q:���R  bool == true(��1) �Ȃ� ��2 : false�Ȃ玮3
		m_bColBlockDraw = m_bColBlockDraw == true ? m_bColBlockDraw = false : m_bColBlockDraw = true;
	}

	//CDebugProc::Print("cn", " Num�L�[0  : ", m_nKey[0]);
	//CDebugProc::Print("cn", " �t���[����0  : ", m_nCountFlame[0]);
	//CDebugProc::Print("cn", " Num�L�[1  : ", m_nKey[1]);
	//CDebugProc::Print("cn", " �t���[����1 : ", m_nCountFlame[1]);
#endif
}

//=============================================================================
// �G�l�~�[�`�揈��
//=============================================================================
void CCPU::Draw(void)
{
	//�f�o�C�X���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;

#ifdef _DEBUG
	if (m_bColBlockDraw == true)
	{	// 2D�I�u�W�F�N�g�`�揈��
		CSceneX::Draw();
	}
#endif

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �ʒu�擾
	D3DXVECTOR3 pos;
	pos = CSceneX::GetPosition();
	D3DXVECTOR3 rot;
	rot = CSceneX::GetRot();

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntParent = 0; nCntParent < MODEL_PARENT; nCntParent++)
	{
		for (int nCnt = 0; nCnt < m_nNumParts[nCntParent]; nCnt++)
		{
			if (m_apModel[nCnt][nCntParent] != NULL)
			{
				//m_apModel[nCnt]->m_bShadow = false;
				m_apModel[nCnt][nCntParent]->Draw();
			}
		}
	}
}

//=============================================================================
// ���f���Ƃ̓����蔻��̏���
//=============================================================================
void CCPU::CollisonSceneX(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 radius)
{
	CScene *pScene = NULL;

	// �擪�̃I�u�W�F�N�g���擾
	pScene = CScene::GetTop(DOHYO_PRIORITY);

	while (pScene != NULL)
	{// �D�揇�ʂ�3�̃I�u�W�F�N�g��1��1�m���߂�
	 // �����̍Œ��ɏ�����\�������邩���ɋL�^���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{// ���S�t���O�������Ă��Ȃ�����
			if (pScene->GetObjType() == CScene::OBJTYPE_DOHYO)
			{// �I�u�W�F�N�g�̎�ނ��m���߂�
				m_bLand = ((CSceneX*)pScene)->Collision(pos, posOld, move, radius);
				if (m_bLand == true)
				{// ���f���ɓ�����
				 //m_bJump = false;
				}
			}
		}

		// ���̃V�[���ɐi�߂�
		pScene = pSceneNext;
	}
}

//=============================================================================
// �E���ɂ��邩�ǂ������擾
//=============================================================================
bool CCPU::GetLand(void)
{
	return m_bLand;
}

//=============================================================================
// ��Ԃ��擾
//=============================================================================
CCPU::STATE CCPU::GetState(void)
{
	return m_State;
}

//=============================================================================
// ��Ԃ�ݒ�
//=============================================================================
void CCPU::SetState(STATE state)
{
	m_State = state;
}

//=============================================================================
// �ړ��ʂ�ݒ�
//=============================================================================
void CCPU::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=============================================================================
// ���[�V������ݒ�
//=============================================================================
void CCPU::SetMotionType(int nParent, CCPU::MOTION_TYPE MotionType)
{
	m_MotionType[nParent] = MotionType;
	m_nMotionType[nParent] = m_MotionType[nParent];
	m_nKey[nParent] = 0;
	m_nCountFlame[nParent] = 0;
	m_bDash = false;
}

//=============================================================================
// �G�l�~�[�̃X�e�[�^�X��������
//=============================================================================
void CCPU::InitStatus(void)
{
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDestAngle = 0;
	m_fDiffAngle = 0;
	m_bLand = false;				// �̂��Ă��邩�ǂ���
	m_bHit = false;					// �������Ă��邩�ǂ���
	m_State = STATE_JANKEN;
	m_Direction = DIRECTION_RIGHT;
	m_bRecovery = false;			// �d���t���O
	m_nRecoveryTime = 0;			// �d������
	m_nLife = 100;
	m_bDying = false;
	m_DohyoState = DOHYO_NORMAL;
	m_nCounterTime = 0;
	m_bCounter = false;
	m_DohyoHaziLR = HAZI_NORMAL;
	m_fRot = 0.0f;
	m_bSelect = false;
	m_nSiomakiCnt = 0;
	m_bDash = false;

	for (int nCntParent = 0; nCntParent < MODEL_PARENT; nCntParent++)
	{
		m_nKey[nCntParent] = 0;			//���݂̃L�[
		m_nCountFlame[nCntParent] = 0;	//���݂̃t���[��
		m_bMotionEnd[nCntParent] = false;
	}
}

//=============================================================================
// �G�l�~�[�̑���
//=============================================================================
float CCPU::CPUOperation(D3DXVECTOR3 pos, float fMoveCPU)
{
	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();
	// �ړ������擾
	CCharacterMove *pCharacterMove;
	pCharacterMove = CManager::GetCharacterMove();
	// �Q�[�W�̎擾
	CSansoGauge *pSansoGauge;
	pSansoGauge = CGame::GetSansoGauge();

	////�ʏ��Ԃōd�����Ă��Ȃ�
	//if (m_State == STATE_NEUTRAL && m_bRecovery == false)
	//{
	//	//�C�ӂ̃L�[��
	//	if (pInputKeyboard->GetPress(CPU_LEFT) == true ||
	//	pXInput->GetPress(XCPU_LEFT, 1) == true)
	//	{
	//		//�_�b�V���ݒ�
	//		if (pInputKeyboard->GetPress(CPU_B_BUTTON) == true ||
	//			pXInput->GetPress(XCPU_B_BUTTON, 1) == true)
	//		{
	//			fMoveCPU = DASH_MOVE;
	//			m_bDash = true;
	//		}
	//		else
	//		{
	//			m_bDash = false;
	//		}
	//		// ���ɐi��
	//		m_move = pCharacterMove->MoveLeft(m_move, fMoveCPU);
	//		m_nMotionType[0] = MOTION_SURIASI;
	//		m_nMotionType[1] = MOTION_SURIASI;
	//	}

	//	//�C�ӂ̃L�[��
	//else if (pInputKeyboard->GetPress(CPU_RIGHT) == true ||
	//		pXInput->GetPress(XCPU_RIGHT, 1) == true)
	//{
	//		//�_�b�V���ݒ�
	//		if (pInputKeyboard->GetPress(CPU_B_BUTTON) == true ||
	//			pXInput->GetPress(XCPU_B_BUTTON, 1) == true)
	//		{
	//			fMoveCPU = DASH_MOVE;
	//			m_bDash = true;
	//		}
	//		else
	//		{
	//			m_bDash = false;
	//		}
	//		// �E�ɐi��
	//		m_move = pCharacterMove->MoveRight(m_move, fMoveCPU);
	//		m_nMotionType[0] = MOTION_SURIASI;
	//		m_nMotionType[1] = MOTION_SURIASI;
	//	}
	//	else
	//	{
	//		if (m_bMotionEnd[0] == true)
	//		{
	//			m_nMotionType[0] = MOTION_BATTLE_NEUTRAL;
	//		}
	//		if (m_bMotionEnd[1] == true)
	//		{
	//			m_nMotionType[1] = MOTION_BATTLE_NEUTRAL;
	//		}
	//	}

	//	if (pInputKeyboard->GetPress(DIK_6) == true && m_bUltDis == true)
	//	{// �K�E
	//		m_State = STATE_ULT;

	//		if (m_pAnimation != NULL)
	//		{
	//			m_pAnimation->SetBillboard(pos, 150.0f, 100.0f);
	//		}
	//	}
	//}

	if (m_State == STATE_NEUTRAL || m_State == STATE_GUARD)
	{
		//�K�[�h���
		//if (pInputKeyboard->GetPress(CPU_C_BUTTON) == true ||
		//	pXInput->GetPress(XCPU_X_BUTTON, 1) == true)
		{
			m_State = STATE_GUARD;
			pSansoGauge->SetSansoGaugeRightLeft(0, GUARD_NOW_SANSO);
		}
	//	if (pInputKeyboard->GetRelese(CPU_C_BUTTON) == true && m_State == STATE_GUARD ||
	//		pXInput->GetRelese(XCPU_X_BUTTON, 1) == true && m_State == STATE_GUARD)
		{
			m_State = STATE_NEUTRAL;
		}
	}

	return fMoveCPU;
}

//=============================================================================
// �G�Ƃ̏Փ�
//=============================================================================
void CCPU::CollisionPlayerAction(void)
{
	if (CGame::GetHit() == true)
	{
		if (m_State == STATE_NEUTRAL || m_State == STATE_NOKOTTA || m_State == STATE_GUARD)
		{	//�g�ݏ�Ԃ�
			m_State = STATE_KUMI;
			if (MOTION_BUTIKAMASI == m_nMotionType[0]
				&& MOTION_BUTIKAMASI == m_nMotionType[1])
			{//�Ԃ����܂����[�V�����̎��͎~�߂�
				m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
			if (MOTION_NAGE != m_nMotionType[0]
				&& MOTION_NAGE != m_nMotionType[1])
			{//�������[�V�����ȊO�̓j���[�g����
				m_nMotionType[0] = MOTION_TUKAMI_NEUTRAL;
				m_nMotionType[1] = MOTION_TUKAMI_NEUTRAL;
			}
		}
		if (m_State == STATE_KUMI)
		{ //�g�ݏ�ԂȂ�
			if (MOTION_TUKAMI_AGERU == m_nMotionType[0]
				&& MOTION_TUKAMI_AGERU == m_nMotionType[1]
				&& m_bMotionEnd[0] == true
				&& m_bMotionEnd[1] == true
				|| MOTION_TUKAMI_AGERARERU == m_nMotionType[0]
				&& MOTION_TUKAMI_AGERARERU == m_nMotionType[1]
				&& m_bMotionEnd[0] == true
				&& m_bMotionEnd[1] == true)
			{//���ݏグ�A�グ��ꃂ�[�V�������I������Ƃ��j���[�g����
				m_nMotionType[0] = MOTION_TUKAMI_NEUTRAL;
				m_nMotionType[1] = MOTION_TUKAMI_NEUTRAL;
			}
		}
	}
	else if (CGame::GetHit() == false && m_State != STATE_JANKEN && m_State != STATE_NOKOTTA && m_State != STATE_TSUPPARI && m_State != STATE_ULT)
	{
		m_State = STATE_NEUTRAL;
	}

}

//=============================================================================
// �^�C�}�[�̍X�V
//=============================================================================
void CCPU::TimerUpdate(void)
{
	//�d�����Ă���Ƃ�
	if (m_bRecovery == true)
	{
		m_nRecoveryTime--;
		if (m_nRecoveryTime <= 0)
		{
			m_bRecovery = false;
			m_nRecoveryTime = 0;
		}
	}
	//�J�E���^�[���Ă���Ƃ�
	if (m_bCounter == true)
	{
		m_bRecovery = true;
		m_nCounterTime--;
		if (m_nCounterTime <= 0)
		{
			m_bCounter = false;
			m_nCounterTime = 0;
			m_nRecoveryTime = 20;
		}
	}
}


//=============================================================================
// ���ς�Ƃ̓����蔻��
//============================================================================
void CCPU::TsuppariCollision(D3DXVECTOR3 pos)
{
	// �v���C���[�擾
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	// �Q�[�W�̎擾
	CSansoGauge *pSansoGauge;
	pSansoGauge = CGame::GetSansoGauge();

	// ���ς�Ƃ̓����蔻��
	if (pPlayer->GetState() == CPlayer::STATE_TSUPPARI)
	{
		bool bHit = pPlayer->GetTuppari().Collision(&pos, &D3DXVECTOR3(m_posOld.x, m_posOld.y + 1.0f, m_posOld.z), &m_move, TSUPPARI_COLLISION);
		//���ς�ɂ�������
		if (bHit == true)
		{
			if (m_State != STATE_GUARD)
			{
				m_State = STATE_DAMAGE;
			}
			else
			{
				CGame::GetBatlteSys()->GuardKnockBack(1);
				pSansoGauge->SetSansoGaugeRightLeft(0, GUARD_SANSO);
				m_State = STATE_GUARD;
			}
			CGame::SetHit(false);
		}
	}

}

//=============================================================================
// �y�U�[���ǂ���
//============================================================================
void CCPU::DohyoHaziWhether(D3DXVECTOR3 pos)
{
	if (pos.x < -DOHYO_HAZI_MIN && pos.x > -DOHYO_HAZI_MAX || pos.x > DOHYO_HAZI_MIN && pos.x < DOHYO_HAZI_MAX)
	{
		m_DohyoState = DOHYO_HAZI;

		if (pos.x < -DOHYO_HAZI_MIN && pos.x > -DOHYO_HAZI_MAX)
		{	//���[
			m_DohyoHaziLR = HAZI_LEFT;
		}
		else if (pos.x > DOHYO_HAZI_MIN && pos.x < DOHYO_HAZI_MAX)
		{	//�E�[
			m_DohyoHaziLR = HAZI_RIGHT;
		}
	}
	else
	{
		m_DohyoState = DOHYO_NORMAL;
		m_DohyoHaziLR = HAZI_NORMAL;
	}

}

//=============================================================================
// �G�l�~�[�̓���V�[��
//=============================================================================
void CCPU::EntryCPU(D3DXVECTOR3 pos, float fMoveCPU)
{
	// �ړ������擾
	CCharacterMove *pCharacterMove;
	pCharacterMove = CManager::GetCharacterMove();

	if (CCamera::GetState() == CCamera::STATE_NISHI)
	{
		if (m_nMotionType[0] != MOTION_SYAGAMI
			&& m_nMotionType[1] != MOTION_SYAGAMI
			&& m_nMotionType[0] != MOTION_SIOMAKI
			&& m_nMotionType[1] != MOTION_SIOMAKI)
		{
			if (m_nSiomakiCnt < 10)
			{
				m_nMotionType[0] = MOTION_WALK;
				m_nMotionType[1] = MOTION_WALK;
			}
		}
		// ���ɐi��
		if (pos.x <= 80.0f)
		{
			m_nSiomakiCnt++;

			if (m_nSiomakiCnt > 60)
			{
				if (m_nMotionType[0] != MOTION_NEUTRAL
					&& m_nMotionType[1] != MOTION_NEUTRAL)
				{
					m_nKey[0] = 0;
					m_nKey[1] = 0;
					m_nMotionType[0] = MOTION_NEUTRAL;
					m_nMotionType[1] = MOTION_NEUTRAL;
				}
			}
			else
			{
				if (m_nMotionType[0] != MOTION_SIOMAKI
					&& m_nMotionType[1] != MOTION_SIOMAKI)
				{
					m_nKey[0] = 0;
					m_nKey[1] = 0;
					m_nMotionType[0] = MOTION_SIOMAKI;
					m_nMotionType[1] = MOTION_SIOMAKI;
				}
			}
			fMoveCPU = 0.0f;
			pos.x = 80.0f;
		}
		m_move = pCharacterMove->MoveLeft(m_move, fMoveCPU * 0.7f);
	}
}

//=============================================================================
// �G�l�~�[�̌���
//=============================================================================
D3DXVECTOR3 CCPU::DirectionCPU(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �v���C���[�擾
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();

	// �ړI�̊p�x
	if (pPlayer != NULL)
	{
		m_fDestAngle = -atan2f((pPlayer->GetPosition().x - sinf(rot.y)) - pos.x, (pPlayer->GetPosition().z - cosf(rot.y)) - pos.z);
	}
	// ����
	m_fDiffAngle = m_fDestAngle - rot.y;

	if (m_fDiffAngle > D3DX_PI)
	{
		m_fDiffAngle -= D3DX_PI * 2.0f;
	}
	if (m_fDiffAngle < -D3DX_PI)
	{
		m_fDiffAngle += D3DX_PI * 2.0f;
	}

	rot.y += m_fDiffAngle * 0.1f;

	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}

	//�����̊���
	m_fDiffAngle = m_fDestAngle - rot.y;

	//�p�x�̐ݒ�
	if (m_fDiffAngle > D3DX_PI)
	{
		m_fDiffAngle -= D3DX_PI* 2.0f;
	}
	if (m_fDiffAngle < -D3DX_PI)
	{
		m_fDiffAngle += D3DX_PI* 2.0f;
	}

	rot.y += m_fDiffAngle * 0.1f;

	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI* 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI* 2.0f;
	}

	if (rot.y < 0.0f)
	{
		m_Direction = DIRECTION_RIGHT;
	}
	else if (rot.y > 0.0f)
	{
		m_Direction = DIRECTION_LEFT;
	}

	return rot;
}



//=============================================================================
// �G�l�~�[�̃��[�V����
//=============================================================================
void CCPU::UpdateMotion(int nParent)
{
	//�T�E���h�����擾
	//CSound *pSound = CManager::GetSound();
	CSound *pSound = CManager::GetSound(2);

	//���[�V����
	KEY *pKey, *pNextKey;
	float fRateMotion;
	float fDiffMotion;
	float fPlusData;
	float fMinusData;
	//float fPlusPos;
	//float fMinusPos;
	D3DXVECTOR3 rotmotion;
	D3DXVECTOR3 posmotion;
	D3DXVECTOR3 BodyRot;
	KEY			NowKey;
	float fDivideMotion = 1;

	//�L�[���ő吔������Ȃ��悤��
	if (m_aMotionInfo[m_nMotionType[nParent]][nParent].nNumKey <= m_nKey[nParent])
	{
		m_nKey[nParent] = 0;
	}

	if (m_bDash == true)
	{
		fDivideMotion = 2.0f;
	}

	//���[�V�����X�V
	for (int nCntParts = 0; nCntParts < m_nNumParts[nParent]; nCntParts++)
	{
		if (m_apModel[nCntParts][nParent] != NULL)
		{
			//���݂̃L�[���擾
			pKey = &m_pKeyInfo[m_nMotionType[nParent]][nParent][m_nKey[nParent]].aKey[nCntParts];

			NowKey = *pKey;

			//���̃L�[���擾
			pNextKey = &m_pKeyInfo[m_nMotionType[nParent]][nParent][(m_nKey[nParent] + 1) % m_aMotionInfo[m_nMotionType[nParent]][nParent].nNumKey].aKey[nCntParts];
			//���݂̃L�[���玟�̃L�[�ւ̍Đ��t���[�����ɂ����郂�[�V�����J�E���^�[�̑��Βl���Z�o
			fRateMotion = (float)m_nCountFlame[nParent] / (float)(m_pKeyInfo[m_nMotionType[nParent]][nParent][m_nKey[nParent]].nFrame / fDivideMotion);

#if 1
			fPlusData = pNextKey->frotX + NowKey.frotX;
			fMinusData = pNextKey->frotX - NowKey.frotX;
			//�l���o��(��Βl)
			fPlusData = abs(fPlusData);
			fMinusData = abs(fMinusData);
			//�����������ق�
			if (fPlusData < fMinusData)
			{	//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
				fDiffMotion = pNextKey->frotX - NowKey.frotX;
				//���Βl���������g���Ċe�v�f�̒l���Z�o
				rotmotion.x = NowKey.frotX + (fDiffMotion * fRateMotion);
			}
			else
			{	//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
				fDiffMotion = pNextKey->frotX - NowKey.frotX;
				//���Βl���������g���Ċe�v�f�̒l���Z�o
				rotmotion.x = NowKey.frotX + (fDiffMotion * fRateMotion);
			}

			fPlusData = pNextKey->frotY + NowKey.frotY;
			fMinusData = pNextKey->frotY - NowKey.frotY;
			//�l���o��(��Βl)
			fPlusData = abs(fPlusData);
			fMinusData = abs(fMinusData);
			//�����������ق�
			if (fPlusData < fMinusData)
			{	//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
				fDiffMotion = pNextKey->frotY - NowKey.frotY;
				//���Βl���������g���Ċe�v�f�̒l���Z�o
				rotmotion.y = NowKey.frotY + (fDiffMotion * fRateMotion);
			}
			else
			{	//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
				fDiffMotion = pNextKey->frotY - NowKey.frotY;
				//���Βl���������g���Ċe�v�f�̒l���Z�o
				rotmotion.y = NowKey.frotY + (fDiffMotion * fRateMotion);
			}

			fPlusData = pNextKey->frotZ + NowKey.frotZ;
			fMinusData = pNextKey->frotZ - NowKey.frotZ;
			//�l���o��(��Βl)
			fPlusData = abs(fPlusData);
			fMinusData = abs(fMinusData);
			//�����������ق�
			if (fPlusData < fMinusData)
			{	//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
				fDiffMotion = pNextKey->frotZ - NowKey.frotZ;
				//���Βl���������g���Ċe�v�f�̒l���Z�o
				rotmotion.z = NowKey.frotZ + (fDiffMotion * fRateMotion);
			}
			else
			{	//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
				fDiffMotion = pNextKey->frotZ - NowKey.frotZ;
				//���Βl���������g���Ċe�v�f�̒l���Z�o
				rotmotion.z = NowKey.frotZ + (fDiffMotion * fRateMotion);
			}

			//POS
			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->fposX - NowKey.fposX;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			posmotion.x = NowKey.fposX + (fDiffMotion * fRateMotion);
			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->fposY - NowKey.fposY;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			posmotion.y = NowKey.fposY + (fDiffMotion * fRateMotion);
			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->fposZ - NowKey.fposZ;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			posmotion.z = NowKey.fposZ + (fDiffMotion * fRateMotion);

#endif
			//�p�[�c�𓮂���
			m_apModel[nCntParts][nParent]->Setrot(rotmotion);
			//�I�t�Z�b�g�̈ʒu��ݒ�
			m_apModel[nCntParts][nParent]->Setpos(D3DXVECTOR3(m_OffSetPos[nCntParts][nParent].x + posmotion.x,
				m_OffSetPos[nCntParts][nParent].y + posmotion.y,
				m_OffSetPos[nCntParts][nParent].z + posmotion.z));
		}
	}

	//���[�v�̔���
	switch (m_aMotionInfo[m_nMotionType[nParent]][nParent].bLoop)
	{
	case true:
		//���[�v����
		//�t���[����i�߂�
		m_nCountFlame[nParent]++;
		//�L�[�̍X�V
		if (m_nCountFlame[nParent] >= m_pKeyInfo[m_nMotionType[nParent]][nParent][m_nKey[nParent]].nFrame / fDivideMotion)
		{
			if (m_aMotionInfo[m_nMotionType[nParent]][nParent].nNumKey - 1 == m_nKey[nParent])
			{//�L�[�̏�����
				m_nKey[nParent] = 0;
			}
			else
			{//�L�[�̍X�V
				m_nKey[nParent] += 1;
			}
			m_nCountFlame[nParent] = 0;
		}

		break;
	case false:
		//���[�v���Ȃ�
		if (m_aMotionInfo[m_nMotionType[nParent]][nParent].nNumKey - 1 > m_nKey[nParent])
		{//�t���[����i�߂�
			m_nCountFlame[nParent]++;
		}
		else if (m_aMotionInfo[m_nMotionType[nParent]][nParent].nNumKey - 1 == m_nKey[nParent])
		{
			m_bMotionEnd[nParent] = true;
		}
		//�L�[�̍X�V
		if (m_nCountFlame[nParent] >= m_pKeyInfo[m_nMotionType[nParent]][nParent][m_nKey[nParent]].nFrame)
		{
			if (m_aMotionInfo[m_nMotionType[nParent]][nParent].nNumKey > m_nKey[nParent])
			{
				m_nKey[nParent] += 1;
			}
			m_nCountFlame[nParent] = 0;
		}

		break;
	}

}

//=============================================================================
// �t�@�C���ǂݍ���
//=============================================================================
void CCPU::FileLoad(char FileName[256], int nParent)
{
	//�f�o�C�X���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �ʒu�擾

	D3DXVECTOR3 pos;
	pos = CSceneX::GetPosition();
	D3DXVECTOR3 rot;
	rot = CSceneX::GetRot();

	//�t�@�C���ǂݍ��ݗp�ϐ�
	FILE *pFile;		//�t�@�C���|�C���^
	char *pStrcur;		//���݂̐擪�̕�����
	char aLine[256];	//������
	char aStr[256];		//�ꎞ�ۑ�������
	int nIndex = 0;		//���݂̃C���f�b�N�X
	int nWord = 0;		//�|�b�v�ŕԂ��ꂽ�l��ێ�

	D3DXVECTOR3 ParentPos;	//�e�̈ʒu�����擾

	char aTestname[256];
	strcpy(aTestname, FileName);

#if 1
	//�t�@�C�����J�� �ǂݍ���
	pFile = fopen(FileName, "r");
	//NULL�`�F�b�N
	if (pFile != NULL)
	{
		//������̐擪��ݒ�
		pStrcur = ReadLine(pFile, &aLine[0]);
		if (memcmp(pStrcur, "SCRIPT", strlen("SCRIPT")) == 0)
		{
			for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
			{
				//������̐擪��ݒ�
				pStrcur = ReadLine(pFile, &aLine[0]);
				//����������o��
				strcpy(aStr, pStrcur);

				//������̃f�[�^ ��r���镶���� ��r���镶����
				if (memcmp(pStrcur, "NUM_MODEL = ", strlen("NUM_MODEL = ")) == 0)
				{
					//���o��
					pStrcur += strlen("NUM_MODEL = ");
					//������̐擪��ݒ�
					strcpy(aStr, pStrcur);
					//�����񔲂��o��
					int nNumModel = atoi(pStrcur);

					for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
					{
						//������̐擪��ݒ�
						pStrcur = ReadLine(pFile, &aLine[0]);
						//����������߂�
						strcpy(aStr, pStrcur);
						if (memcmp(pStrcur, "MODEL_FILENAME = ", strlen("MODEL_FILENAME = ")) == 0)
						{
							//���o��
							pStrcur += strlen("MODEL_FILENAME = ");

							//�������ݒ�
							strcpy(aStr, pStrcur);

							//�K�v�ȕ�����̍Ō�̕����܂ł̕������𐔂���
							int nNullNum = PopString(pStrcur, &aStr[0]);

							//����������߂�
							strcpy(aStr, pStrcur);

							//�Ō�̕�����NULL����������
							aStr[nNullNum - 1] = '\0';

							//�Ώۂ̕����񂩂甲���o��
							strcpy(&m_aFileNameModel[nCntModel][0], aStr);

							// X�t�@�C���̓ǂݍ���
							D3DXLoadMeshFromX(&m_aFileNameModel[nCntModel][0],
								D3DXMESH_SYSTEMMEM,
								pDevice,
								NULL,
								&m_pBuffMatModel[nCntModel][nParent],
								NULL,
								&m_nNumMatModel[nCntModel][nParent],
								&m_pMeshModel[nCntModel][nParent]);
						}
					}
					//������̐擪��ݒ�
					pStrcur = ReadLine(pFile, &aLine[0]);
					//����������o��
					strcpy(aStr, pStrcur);
				}


				//������̃f�[�^ ��r���镶���� ��r���镶����
				if (memcmp(pStrcur, "CHARACTERSET", 12) == 0)
				{
					while (1)
					{
						//������̐擪��ݒ�
						pStrcur = ReadLine(pFile, &aLine[0]);
						//����������o��
						strcpy(aStr, pStrcur);
						if (memcmp(pStrcur, "NUM_PARTS = ", strlen("NUM_PARTS = ")) == 0)
						{
							//���o��
							pStrcur += strlen("NUM_PARTS = ");
							//������̐擪��ݒ�
							strcpy(aStr, pStrcur);
							//�����񔲂��o��
							m_nNumParts[nParent] = atoi(pStrcur);
						}
						if (memcmp(pStrcur, "PARTSSET", strlen("PARTSSET")) == 0)
						{
							while (1)
							{
								//������̐擪��ݒ�
								pStrcur = ReadLine(pFile, &aLine[0]);

								//INDEX��ǂݍ���
								if (memcmp(pStrcur, "INDEX = ", strlen("INDEX = ")) == 0)
								{
									//���o��
									pStrcur += strlen("INDEX = ");
									//������̐擪��ݒ�
									strcpy(aStr, pStrcur);
									//�����񔲂��o��
									nIndex = atoi(pStrcur);
								}
								//PARENT��ǂݍ���
								if (memcmp(pStrcur, "PARENT = ", strlen("PARENT = ")) == 0)
								{
									//���o��
									pStrcur += strlen("PARENT = ");
									//������̐擪��ݒ�
									strcpy(aStr, pStrcur);
									//�����񔲂��o��
									m_aIndexParent[nIndex][nParent] = atoi(pStrcur);
								}
								//POS��ǂݍ���
								if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
								{
									//���o��
									pStrcur += strlen("POS = ");
									//������̐擪��ݒ�
									strcpy(aStr, pStrcur);

									//��������Ԃ��Ă��炤
									nWord = PopString(pStrcur, &aStr[0]);
									//������ϊ�
									m_aKayOffset[nIndex][nParent].fposX = (float)atof(pStrcur);
									//���������i�߂�
									pStrcur += nWord;

									//��������Ԃ��Ă��炤
									nWord = PopString(pStrcur, &aStr[0]);
									//������ϊ�
									m_aKayOffset[nIndex][nParent].fposY = (float)atof(pStrcur);
									//���������i�߂�
									pStrcur += nWord;

									//��������Ԃ��Ă��炤
									nWord = PopString(pStrcur, &aStr[0]);
									//������ϊ�
									m_aKayOffset[nIndex][nParent].fposZ = (float)atof(pStrcur);

								}
								//ROT��ǂݍ���
								if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
								{
									//���o��
									pStrcur += strlen("ROT = ");
									//������̐擪��ݒ�
									strcpy(aStr, pStrcur);

									//��������Ԃ��Ă��炤
									nWord = PopString(pStrcur, &aStr[0]);
									//������ϊ�
									m_aKayOffset[nIndex][nParent].frotX = (float)atof(pStrcur);

									//���������i�߂�
									pStrcur += nWord;
									//��������Ԃ��Ă��炤
									nWord = PopString(pStrcur, &aStr[0]);
									//������ϊ�
									m_aKayOffset[nIndex][nParent].frotY = (float)atof(pStrcur);

									//���������i�߂�
									pStrcur += nWord;
									//��������Ԃ��Ă��炤
									nWord = PopString(pStrcur, &aStr[0]);
									//������ϊ�
									//rotFile[nIndex].z = (float)atof(pStrcur);
									m_aKayOffset[nIndex][nParent].frotZ = (float)atof(pStrcur);


								}
								//�p�[�c�Z�b�g�I��
								else if (memcmp(pStrcur, "END_PARTSSET", strlen("END_PARTSSET")) == 0)
								{

									//���f���𐶐�	�I�t�Z�b�g�ݒ�
									m_apModel[nIndex][nParent] = CModel::Create(
										D3DXVECTOR3(m_aKayOffset[nIndex][nParent].fposX,
											m_aKayOffset[nIndex][nParent].fposY,
											m_aKayOffset[nIndex][nParent].fposZ),
										D3DXVECTOR3(m_aKayOffset[nIndex][nParent].frotX,
											m_aKayOffset[nIndex][nParent].frotY,
											m_aKayOffset[nIndex][nParent].frotZ));

									//pos����
									m_OffSetPos[nIndex][nParent] = m_apModel[nIndex][nParent]->GetPos();

									//���f�������蓖��
									m_apModel[nIndex][nParent]->BindModel(m_nNumMatModel[nIndex][nParent], m_pMeshModel[nIndex][nParent], m_pBuffMatModel[nIndex][nParent]);

									if (m_aIndexParent[nIndex][nParent] == -1)
									{
										//���f���̐e���w��
										m_apModel[nIndex][nParent]->SetParent(NULL);
									}
									else
									{
										//���f���̐e���w��
										m_apModel[nIndex][nParent]->SetParent(m_apModel[m_aIndexParent[nIndex][nParent]][nParent]);
									}

									break;
								}
							}
						}
						//�L�����N�^�[�Z�b�g�I��
						else if (memcmp(pStrcur, "END_CHARACTERSET", strlen("END_CHARACTERSET")) == 0)
						{
							break;
						}
					}
					//������̐擪��ݒ�
					pStrcur = ReadLine(pFile, &aLine[0]);
					//����������o��
					strcpy(aStr, pStrcur);
				}

				//���[�V�����ǂݍ���
				if (memcmp(pStrcur, "MOTIONSET", strlen("MOTIONSET")) == 0)
				{
					//���o��
					pStrcur += strlen("MOTIONSET");

					while (1)
					{
						//������̐擪��ݒ�
						pStrcur = ReadLine(pFile, &aLine[0]);
						//����������o��
						strcpy(aStr, pStrcur);

						if (memcmp(pStrcur, "LOOP = ", strlen("LOOP = ")) == 0)
						{
							//���o��
							pStrcur += strlen("LOOP = ");
							//������̐擪��ݒ�
							strcpy(aStr, pStrcur);

							switch (atoi(pStrcur))
							{
							case 0:
								//�����񔲂��o��
								m_aMotionInfo[nCntMotion][nParent].bLoop = false;
								break;
							case 1:
								//�����񔲂��o��
								m_aMotionInfo[nCntMotion][nParent].bLoop = true;
								break;
							}
							//������̐擪��ݒ�
							pStrcur = ReadLine(pFile, &aLine[0]);
						}

						if (memcmp(pStrcur, "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
						{
							//���o��
							pStrcur += strlen("NUM_KEY = ");
							//������̐擪��ݒ�
							strcpy(aStr, pStrcur);
							//�����񔲂��o��
							m_aMotionInfo[nCntMotion][nParent].nNumKey = atoi(pStrcur);

							//������̐擪��ݒ�
							pStrcur = ReadLine(pFile, &aLine[0]);
							//����������o��
							strcpy(aStr, pStrcur);
						}

						//�L�[�̐ݒ�
						for (int nCntKey = 0; nCntKey < m_aMotionInfo[nCntMotion][nParent].nNumKey;)
						{
							if (memcmp(pStrcur, "KEYSET", strlen("KEYSET")) == 0)
							{
								//���o��
								pStrcur += strlen("KEYSET");
								//������̐擪��ݒ�
								strcpy(aStr, pStrcur);
								//������̐擪��ݒ�
								pStrcur = ReadLine(pFile, &aLine[0]);

								if (memcmp(pStrcur, "FRAME = ", strlen("FRAME = ")) == 0)
								{
									//���o��
									pStrcur += strlen("FRAME = ");

									m_aMotionInfo[nCntMotion][nParent].aKayInfo[nCntKey].nFrame = atoi(pStrcur);

									//������̐擪��ݒ�
									strcpy(aStr, pStrcur);
									//������̐擪��ݒ�
									pStrcur = ReadLine(pFile, &aLine[0]);
								}

								//�p�[�c����
								for (int nCntParts = 0; nCntParts < m_nNumParts[nParent];)
								{
									if (memcmp(pStrcur, "KEY", strlen("KEY")) == 0)
									{
										//������̐擪��ݒ�
										pStrcur = ReadLine(pFile, &aLine[0]);

										if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
										{
											//���o��
											pStrcur += strlen("POS = ");
											//������̐擪��ݒ�
											strcpy(aStr, pStrcur);

											//��������Ԃ��Ă��炤
											nWord = PopString(pStrcur, &aStr[0]);
											//POS.X���
											m_aMotionInfo[nCntMotion][nParent].aKayInfo[nCntKey].aKey[nCntParts].fposX = (float)atof(pStrcur);
											//���������i�߂�
											pStrcur += nWord;

											//��������Ԃ��Ă��炤
											nWord = PopString(pStrcur, &aStr[0]);
											//POS.Y���
											m_aMotionInfo[nCntMotion][nParent].aKayInfo[nCntKey].aKey[nCntParts].fposY = (float)atof(pStrcur);
											//���������i�߂�
											pStrcur += nWord;

											//��������Ԃ��Ă��炤
											nWord = PopString(pStrcur, &aStr[0]);
											//POS.Z���
											m_aMotionInfo[nCntMotion][nParent].aKayInfo[nCntKey].aKey[nCntParts].fposZ = (float)atof(pStrcur);
											//������̐擪��ݒ�
											pStrcur = ReadLine(pFile, &aLine[0]);
										}
										//ROT��ǂݍ���
										if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
										{
											//���o��
											pStrcur += strlen("ROT = ");
											//������̐擪��ݒ�
											strcpy(aStr, pStrcur);

											//��������Ԃ��Ă��炤
											nWord = PopString(pStrcur, &aStr[0]);
											//RotX
											m_aMotionInfo[nCntMotion][nParent].aKayInfo[nCntKey].aKey[nCntParts].frotX = (float)atof(pStrcur);
											//���������i�߂�
											pStrcur += nWord;

											//��������Ԃ��Ă��炤
											nWord = PopString(pStrcur, &aStr[0]);
											//RotY
											m_aMotionInfo[nCntMotion][nParent].aKayInfo[nCntKey].aKey[nCntParts].frotY = (float)atof(pStrcur);
											//���������i�߂�
											pStrcur += nWord;

											//��������Ԃ��Ă��炤
											nWord = PopString(pStrcur, &aStr[0]);
											//RotZ
											m_aMotionInfo[nCntMotion][nParent].aKayInfo[nCntKey].aKey[nCntParts].frotZ = (float)atof(pStrcur);

											//������̐擪��ݒ�
											pStrcur = ReadLine(pFile, &aLine[0]);
										}
										if (memcmp(pStrcur, "END_KEY", strlen("END_KEY")) == 0)
										{
											//���o��
											pStrcur += strlen("END_KEY");
											//������̐擪��ݒ�
											strcpy(aStr, pStrcur);
											//������̐擪��ݒ�
											pStrcur = ReadLine(pFile, &aLine[0]);
											//�p�[�c�̃J�E���g��i�߂�
											nCntParts++;
										}
									}
									else
									{
										//������̐擪��ݒ�
										pStrcur = ReadLine(pFile, &aLine[0]);
									}
								}
								if (memcmp(pStrcur, "END_KEYSET", strlen("END_KEYSET")) == 0)
								{
									//������̐擪��ݒ�
									pStrcur = ReadLine(pFile, &aLine[0]);
									//�J�E���g��i�߂�
									nCntKey++;
								}
							}
							else
							{
								//������̐擪��ݒ�
								pStrcur = ReadLine(pFile, &aLine[0]);
							}

						}
						if (memcmp(pStrcur, "END_MOTIONSET", strlen("END_MOTIONSET")) == 0)
						{
							//���[�V�����̏����Z�b�g
							m_pKeyInfo[nCntMotion][nParent] = &m_aMotionInfo[nCntMotion][nParent].aKayInfo[0];
							break;
						}
					}
				}
				//�X�N���v�g�̏I���
				if (memcmp(pStrcur, "END_SCRIPT	", strlen("END_SCRIPT")) == 0)
				{
					break;
				}
			}
		}
	}
	else
	{	//�t�@�C�����J���Ȃ�����
		MessageBox(0, "NULL�ł���", "�x��", MB_OK);
	}
	//�t�@�C�������
	fclose(pFile);
#endif

}

//=============================================================================
//�@�t�@�C���ǂݍ��ݖ�������r��
//=============================================================================
char *CCPU::ReadLine(FILE *pFile, char *pDst)
{
	while (1)
	{
		//�P�s���ǂݍ���
		fgets(&pDst[0], 256, pFile);

		//������̃f�[�^ ��r���镶���� ��r���镶����
		if (memcmp(pDst, "#", strlen("#")) == 0)
		{
			pDst += strlen("\n");
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pDst, "\t", strlen("\t")) == 0)
		{
			pDst += strlen("\t");
			while (1)
			{
				if (memcmp(pDst, "\t", strlen("\t")) == 0)
				{
					pDst += strlen("\t");
				}
				else
				{
					break;
				}
			}
			break;
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pDst, " ", strlen(" ")) == 0)
		{
			pDst += strlen(" ");
			while (1)
			{
				if (memcmp(pDst, " ", strlen(" ")) == 0)
				{
					pDst += strlen(" ");
				}
				else
				{
					break;
				}
			}
			break;
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pDst, "\n", strlen("\n")) == 0)
		{
			pDst += strlen("\n");
		}
		else
		{
			break;
		}
	}
	return pDst;
}

//=============================================================================
//�@�t�@�C���ǂݍ��ݐ擪��r��
//=============================================================================
char * CCPU::GetLineTop(char * pStr)
{
	while (1)
	{
		//������̃f�[�^ ��r���镶���� ��r���镶����
		if (memcmp(pStr, " ", strlen(" ")) == 0)
		{
			pStr += strlen(" ");
			while (1)
			{
				if (memcmp(pStr, " ", strlen(" ")) == 0)
				{
					pStr += strlen(" ");
				}
				else
				{
					break;
				}
				break;
			}
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{
			pStr += strlen("\t");
			while (1)
			{
				if (memcmp(pStr, "\t", strlen("\t")) == 0)
				{
					pStr += strlen("\t");
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			break;
		}
	}
	return pStr;
}

//=============================================================================
//�@��������Ԃ�
//=============================================================================
int CCPU::PopString(char * pStr, char * pDest)
{
	int nWord = 0;

	while (1)
	{	//���o��
		pStr += 1;
		nWord += 1;
		if (memcmp(pStr, " ", strlen(" ")) == 0)
		{	//���o��
			pStr = "\0";
			nWord += 1;
			break;
		}
		if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{	//���o��
			pStr = "\0";
			nWord += strlen("\t");
			break;
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pStr, "\n", strlen("\n")) == 0)
		{
			//���o��
			nWord += strlen("\n");
			break;
		}
	}
	strcpy(pDest, pStr);
	//������̐���Ԃ�
	return nWord;
}
