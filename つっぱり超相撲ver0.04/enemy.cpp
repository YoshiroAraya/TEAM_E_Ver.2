//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author : �ڍ� ������
//
//=============================================================================
#include "enemy.h"
#include "input.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "camera.h"
#include "bullet.h"
#include "scene3D.h"
#include "meshField.h"
#include "shadow.h"
#include "game.h"
#include "player.h"
#include "characterMove.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define ENEMY_COLLISION		(D3DXVECTOR3(7.0f, 60.0f, 7.0f))		//�G�l�~�[�̓����蔻��

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPD3DXMESH CEnemy::m_pMesh = NULL;			// ���b�V�����i���_���j�ւ̃|�C���^
LPD3DXBUFFER CEnemy::m_pBuffMat = NULL;	// �}�e���A�����ւ̃|�C���^
DWORD CEnemy::m_nNumMat = 0;				// �}�e���A�����̐�

//=============================================================================
// �G�l�~�[�N���X�̃R���X�g���N�^
//=============================================================================
CEnemy::CEnemy() : CSceneX(ENEMY_PRIORITY)
{
	// �l���N���A
	m_pTexture = NULL;							// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;							// ���_�o�b�t�@�ւ̃|�C���^
	m_bLand = false;							// �E�ɂ��邩�ǂ���
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Direction = DIRECTION_LEFT;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{
}

//=============================================================================
// �I�u�W�F�N�g�̐�������
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos)
{
	CEnemy *pEnemy = NULL;

	if (pEnemy == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pEnemy = new CEnemy;

		if (pEnemy != NULL)
		{
			pEnemy->BindModel(m_pBuffMat, m_nNumMat, m_pMesh);
			pEnemy->Init(pos);
		}
	}

	return pEnemy;
}

//=============================================================================
// �G�l�~�[����������
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos)
{
	// 2D�I�u�W�F�N�g����������
	CSceneX::Init(pos);

	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(CScene::OBJTYPE_ENEMY);

	m_move = D3DXVECTOR3(0, 0, 0);
	m_fDestAngle = 0;
	m_fDiffAngle = 0;
	m_bLand = false;					// �E�ɂ��邩�ǂ���
	m_Direction = DIRECTION_LEFT;

	return S_OK;
}

//=============================================================================
// �G�l�~�[�I������
//=============================================================================
void CEnemy::Uninit(void)
{
	// 2D�I�u�W�F�N�g�I������
	CSceneX::Uninit();
}

//=============================================================================
// �G�l�~�[�X�V����
//=============================================================================
void CEnemy::Update(void)
{
	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

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

	// �G�擾
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();

	// �ړ������擾
	CCharacterMove *pCharacterMove;
	pCharacterMove = CManager::GetCharacterMove();

	// �O�̃t���[���̈ʒu���
	m_posOld = pos;

	float fMoveEnemy = MOVE_ENEMY;	// �G�l�~�[�̈ړ��ʂ�ݒ�

	//�ʏ��Ԃōd�����Ă��Ȃ�
	if (m_State == STATE_NEUTRAL && m_bRecovery == false)
	{
		//�C�ӂ̃L�[��
		if (pInputKeyboard->GetPress(ENEMY_LEFT) == true)
		{
			// ���ɐi��
			m_move = pCharacterMove->MoveLeft(m_move, fMoveEnemy);
		}

		//�C�ӂ̃L�[��
		else if (pInputKeyboard->GetPress(ENEMY_RIGHT) == true)
		{
			// �E�ɐi��
			m_move = pCharacterMove->MoveRight(m_move, fMoveEnemy);
		}
	}

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
	// �ړI�̊p�x
	m_fDestAngle = atan2f((pPlayer->GetPosition().x - sinf(rot.y)) - pos.x, (pPlayer->GetPosition().z - cosf(rot.y)) - pos.z);
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

	if (rot.y > 0.0f)
	{
		m_Direction = DIRECTION_RIGHT;
	}
	else if (rot.y < 0.0f)
	{
		m_Direction = DIRECTION_LEFT;
	}

	if (CGame::GetHit() == true)
	{
		if (m_State == STATE_NEUTRAL)
		{
			m_State = STATE_KUMI;
		}
		/*else if (m_State == STATE_KUMI)
		{
		m_State = STATE_NEUTRAL;
		}*/
	}

	if (pInputKeyboard->GetPress(DIK_I) == true)
	{
		// �W�����v��
		pos.y += 1.0f;
	}
	if (pInputKeyboard->GetPress(DIK_K) == true)
	{
		// �W�����v��
		pos.y -= 1.0f;
	}

	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();

	if (pXInput->GetPress(XINPUT_GAMEPAD_A, 1) == true)
	{
		// �W�����v��
		pos.y += 1.0f;
		CDebugProc::Print("c", "������");
	}

	pos += m_move;

	// �d�͉��Z
	m_move.y -= cosf(D3DX_PI * 0.0f) * 0.5f;

	//����
	m_move.x += (0.0f - m_move.x) * 0.5f;
	m_move.z += (0.0f - m_move.z) * 0.5f;
	m_move.y += (0.0f - m_move.y) * 0.01f;

	// ���b�V���t�B�[���h�擾
	CMeshField *pMeshField;
	pMeshField = CGame::GetMeshField();

	pShadow->SetPos(pos);

	// ���f���Ƃ̓����蔻��
	CollisonSceneX(&pos, &D3DXVECTOR3(m_posOld.x, m_posOld.y + 1.0f, m_posOld.z), &m_move, ENEMY_COLLISION);

	CSceneX::SetPosition(pos);
	CSceneX::SetRot(rot);

#ifdef _DEBUG

	int nCnt = 0;

	CDebugProc::Print("cfccfccfc", "�G�l�~�[�̈ʒu : x", pos.x, "f", "   y", pos.y, "f", "  z", pos.z, "f");
	CDebugProc::Print("cfccfccfc", "�G�l�~�[��VtxMax : x", CSceneX::GetVtxMax().x, "f", "   y", CSceneX::GetVtxMax().y, "f", "  z", CSceneX::GetVtxMax().z, "f");
	CDebugProc::Print("cfccfccfc", "�G�l�~�[��VtxMin : x", CSceneX::GetVtxMin().x, "f", "   y", CSceneX::GetVtxMin().y, "f", "  z", CSceneX::GetVtxMin().z, "f");
	CDebugProc::Print("cn", "�G�l�~�[�̏�� : ", m_State);
	CDebugProc::Print("cn", "�G�l�~�[�̌��� : ", m_Direction);
	CDebugProc::Print("cf", "�G�l�~�[�̌��� : ", rot.y);

#endif
}

//=============================================================================
// �G�l�~�[�`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	// 2D�I�u�W�F�N�g�`�揈��
	CSceneX::Draw();
}

//=============================================================================
// ���f���Ƃ̓����蔻��̏���
//=============================================================================
void CEnemy::CollisonSceneX(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 radius)
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
// �G�l�~�[�̃��f���ǂݍ��ݏ���
//=============================================================================
HRESULT CEnemy::LoadModel(void)
{
	// �����_���[���擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(ENEMY_MODEL_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	return S_OK;
}

//=============================================================================
// �G�l�~�[�̃e�N�X�`���������
//=============================================================================
void CEnemy::UnloadModel(void)
{
	// ���b�V���̊J��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// �}�e���A���̊J��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
}

//=============================================================================
// �E���ɂ��邩�ǂ������擾
//=============================================================================
bool CEnemy::GetLand(void)
{
	return m_bLand;
}

//=============================================================================
// ��Ԃ��擾
//=============================================================================
CEnemy::STATE CEnemy::GetState(void)
{
	return m_State;
}

//=============================================================================
// ��Ԃ�ݒ�
//=============================================================================
void CEnemy::SetState(STATE state)
{
	m_State = state;
}

//=============================================================================
// �ړ��ʂ�ݒ�
//=============================================================================
void CEnemy::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}