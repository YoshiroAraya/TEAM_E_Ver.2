//=============================================================================
//
// �o�g���V�X�e������ [BattleSystem.cpp]
// Author : �ڍ� ������
//
//=============================================================================
#include "BattleSystem.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "camera.h"

#include "player.h"
#include "enemy.h"
#include "game.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �}�N��
//=============================================================================
#define OSIAI_MOVE (5.0f)

//=============================================================================
// �o�g���V�X�e���N���X�̃R���X�g���N�^
//=============================================================================
CBattleSys::CBattleSys()
{
	// �l���N���A
	m_nCntFlame = 0;
	m_nImpossibleFlame = 0;
	m_bAttack = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBattleSys::~CBattleSys()
{
}

//=============================================================================
// �I�u�W�F�N�g�̐�������
//=============================================================================
CBattleSys *CBattleSys::Create()
{
	CBattleSys *pBattleSys = NULL;

	if (pBattleSys == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pBattleSys = new CBattleSys;

		if (pBattleSys != NULL)
		{
			pBattleSys->Init();
		}
	}

	return pBattleSys;
}

//=============================================================================
// �o�g���V�X�e������������
//=============================================================================
HRESULT CBattleSys::Init()
{
	// 2D�I�u�W�F�N�g����������
	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	//SetObjType(CScene::OBJTYPE_PLAYER);

	// �l���N���A
	m_nCntFlame = 0;
	m_nImpossibleFlame = 0;
	m_bAttack = false;

	return S_OK;
}

//=============================================================================
// �o�g���V�X�e���I������
//=============================================================================
void CBattleSys::Uninit(void)
{
}

//=============================================================================
// �o�g���V�X�e���X�V����
//=============================================================================
void CBattleSys::Update(void)
{
	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// �v���C���[�̎擾
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer ();
	// �G�l�~�[�̎擾
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();


#ifdef _DEBUG
	if (pPlayer->GetState() == CPlayer::STATE_KUMI
		&& pEnemy->GetState() == CEnemy::STATE_KUMI
		&& m_bAttack == false)
	{
		CDebugProc::Print("c", " �g�ݍ��� ");
		//�C�ӂ̃L�[��
		if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
		{
			CDebugProc::Print("c", " �E�� ");
			pPlayer->SetMove(D3DXVECTOR3(OSIAI_MOVE, 3.0f, 0.0f));
			pEnemy->SetMove(D3DXVECTOR3(OSIAI_MOVE, 3.0f, 0.0f));
			m_bAttack = true;
		}
		//�C�ӂ̃L�[4
		if (pInputKeyboard->GetTrigger(DIK_NUMPAD4) == true)
		{
			CDebugProc::Print("c", " ���� ");
			pPlayer->SetMove(D3DXVECTOR3(-OSIAI_MOVE, 3.0f, 0.0f));
			pEnemy->SetMove(D3DXVECTOR3(-OSIAI_MOVE, 3.0f, 0.0f));

			m_bAttack = true;
		}
	}
	//���Z�b�g
	if (pInputKeyboard->GetTrigger(DIK_R) == true)
	{
		pPlayer->SetPosition(D3DXVECTOR3(-20.0f, 50.0f, 0.0f));
		pEnemy->SetPosition(D3DXVECTOR3(20.0f, 50.0f, 0.0f));
		pPlayer->SetState(CPlayer::STATE_NEUTRAL);
		pEnemy->SetState(CEnemy::STATE_NEUTRAL);
		m_bAttack = false;
		m_nCntFlame = 0;
		CGame::SetHit(false);
	}

	if (pPlayer->GetState() == CPlayer::STATE_NEUTRAL
		&& pEnemy->GetState() == CEnemy::STATE_NEUTRAL
		&& m_bAttack == false)
	{
		CDebugProc::Print("c", " �ʏ� ");
	}

	if (m_bAttack == true)
	{
		m_nCntFlame++;
		if (pPlayer->GetState() == CPlayer::STATE_KUMI
			&& pEnemy->GetState() == CEnemy::STATE_KUMI)
		{
			if (m_nCntFlame > 20)
			{
				m_nCntFlame = 0;
				m_bAttack = false;
			}
		}
	}

	CDebugProc::Print("cn", " �s���s�t���[�� ",m_nCntFlame);

#endif
}

