//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : ���R���
//
//=============================================================================
#include "player.h"
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
#include "enemy.h"
#include "characterMove.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define PLAYER_COLLISION		(D3DXVECTOR3(7.0f, 60.0f, 7.0f))		//�v���C���[�̓����蔻��

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPD3DXMESH CPlayer::m_pMesh = NULL;			// ���b�V�����i���_���j�ւ̃|�C���^
LPD3DXBUFFER CPlayer::m_pBuffMat = NULL;	// �}�e���A�����ւ̃|�C���^
DWORD CPlayer::m_nNumMat = 0;				// �}�e���A�����̐�

											//=============================================================================
											// �v���C���[�N���X�̃R���X�g���N�^
											//=============================================================================
CPlayer::CPlayer() : CSceneX(PLAYER_PRIORITY)
{
	// �l���N���A
	m_pTexture = NULL;						// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
	m_bLand = false;					// �E�ɂ��邩�ǂ���
	m_bHit = false;					// �E�ɂ��邩�ǂ���
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_State = STATE_NEUTRAL;
	m_Direction = DIRECTION_RIGHT;
	m_bRecovery = false;	// �d���t���O
	m_nRecoveryTime = 0;	// �d������
	m_nLife = 0;
	m_bDying = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
// �I�u�W�F�N�g�̐�������
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pPlayer = new CPlayer;

		if (pPlayer != NULL)
		{
			pPlayer->BindModel(m_pBuffMat, m_nNumMat, m_pMesh);
			pPlayer->Init(pos);
		}
	}

	return pPlayer;
}

//=============================================================================
// �v���C���[����������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	// 2D�I�u�W�F�N�g����������
	CSceneX::Init(pos);

	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(CScene::OBJTYPE_PLAYER);

	m_move = D3DXVECTOR3(0, 0, 0);
	m_fDestAngle = 0;
	m_fDiffAngle = 0;
	m_bLand = false;					// �E�ɂ��邩�ǂ���
	m_bHit = false;					// �E�ɂ��邩�ǂ���
	m_State = STATE_JANKEN;
	m_Direction = DIRECTION_RIGHT;
	m_bRecovery = false;	// �d���t���O
	m_nRecoveryTime = 0;	// �d������
	m_bJanken = false;
	m_nLife = 100;
	m_bDying = false;

	return S_OK;
}

//=============================================================================
// �v���C���[�I������
//=============================================================================
void CPlayer::Uninit(void)
{
	// 2D�I�u�W�F�N�g�I������
	CSceneX::Uninit();
}

//=============================================================================
// �v���C���[�X�V����
//=============================================================================
void CPlayer::Update(void)
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
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

	// �ړ������擾
	CCharacterMove *pCharacterMove;
	pCharacterMove = CManager::GetCharacterMove();

	// �O�̃t���[���̈ʒu���
	m_posOld = pos;

	float fMovePlayer = MOVE_PLAYER;	// �v���C���[�̈ړ��ʂ�ݒ�

										//�ʏ��Ԃōd�����Ă��Ȃ�
	if (m_State == STATE_NEUTRAL && m_bRecovery == false)
	{
		//�C�ӂ̃L�[��
		if (pInputKeyboard->GetPress(PLAYER_LEFT) == true)
		{
			// ���ɐi��
			m_move = pCharacterMove->MoveLeft(m_move, fMovePlayer);
		}

		//�C�ӂ̃L�[��
		else if (pInputKeyboard->GetPress(PLAYER_RIGHT) == true)
		{
			// �E�ɐi��
			m_move = pCharacterMove->MoveRight(m_move, fMovePlayer);
		}
	}

	//�d�����Ă���Ƃ�
	if (m_bRecovery == true)
	{
		CGame::SetHit(false);
		m_nRecoveryTime--;
		if (m_nRecoveryTime <= 0)
		{
			m_bRecovery = false;
			m_nRecoveryTime = 0;
		}
	}

	// �ړI�̊p�x
	m_fDestAngle = atan2f((pEnemy->GetPosition().x - sinf(rot.y)) - pos.x, (pEnemy->GetPosition().z - cosf(rot.y)) - pos.z);
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

	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
	{// �e����
		CBullet::Create(D3DXVECTOR3(pos.x + sinf(rot.y + D3DX_PI) * 10, pos.y, pos.z + cosf(rot.y + D3DX_PI) * 10),
			D3DXVECTOR3(sinf(rot.y + D3DX_PI) * 8.5f, 0, cosf(rot.y + D3DX_PI) * 8.5f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (CGame::GetHit() == true)
	{
		if (m_State == STATE_NEUTRAL || m_State == STATE_NOKOTTA)
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

	pos += m_move;

	// �d�͉��Z
	m_move.y -= cosf(D3DX_PI * 0.0f) * 0.5f;

	//����
	m_move.x += (0.0f - m_move.x) * 0.5f;
	m_move.z += (0.0f - m_move.z) * 0.5f;
	m_move.y += (0.0f - m_move.y) * 0.01f;

	pShadow->SetPos(pos);

	// ���f���Ƃ̓����蔻��
	CollisonDohyo(&pos, &D3DXVECTOR3(m_posOld.x, m_posOld.y + 1.0f, m_posOld.z), &m_move, PLAYER_COLLISION);

	CSceneX::SetPosition(pos);
	CSceneX::SetRot(rot);

#ifdef _DEBUG

	int nCnt = 0;

	CDebugProc::Print("cfccfccfc", "�v���C���[�̈ʒu : x", pos.x, "f", "   y", pos.y, "f", "  z", pos.z, "f");
	CDebugProc::Print("cfccfccfc", "VtxMax : x", CSceneX::GetVtxMax().x, "f", "   y", CSceneX::GetVtxMax().y, "f", "  z", CSceneX::GetVtxMax().z, "f");
	CDebugProc::Print("cfccfccfc", "VtxMin : x", CSceneX::GetVtxMin().x, "f", "   y", CSceneX::GetVtxMin().y, "f", "  z", CSceneX::GetVtxMin().z, "f");
	CDebugProc::Print("cn", "�v���C���[�̏�� : ", m_State);
	CDebugProc::Print("cn", "�v���C���[�̌��� : ", m_Direction);
	CDebugProc::Print("cf", "�v���C���[�̌��� : ", rot.y);

	/*for (int nCount = 0; nCount < NUM_VTX; nCount++)
	{
	if (abRight[nCount] == true)
	{
	CDebugProc::Print("nc", nCount, " : �E");
	nCnt++;
	}
	else
	{
	CDebugProc::Print("nc", nCount, " : ��");
	}
	}

	if (m_bRTriangle == true)
	{
	CDebugProc::Print("c", "a�͈͓̔� : false");
	CDebugProc::Print("c", "b�͈͓̔� : true");
	}
	else
	{
	CDebugProc::Print("c", "a�͈͓̔� : true");
	CDebugProc::Print("c", "b�͈͓̔� : false");
	}*/

	//if (bRTriangle == true)
	//{// �O�p�`�Ō����Ƃ�
	//	CDebugProc::Print("c", "a�͈͓̔� : true");
	//	CDebugProc::Print("c", "b�͈͓̔� : false");
	//}
	//else
	//{
	//	CDebugProc::Print("c", "a�͈͓̔� : false");
	//	CDebugProc::Print("c", "b�͈͓̔� : true");
	//}

	/*if (nCnt == NUM_VTX)
	{
	CDebugProc::Print("c", "�͈͓�");
	}
	else
	{
	CDebugProc::Print("c", "�͈͊O");
	}*/
#endif
}

//=============================================================================
// �v���C���[�`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	// 2D�I�u�W�F�N�g�`�揈��
	CSceneX::Draw();
}

//=============================================================================
// �y�U�Ƃ̓����蔻��̏���
//=============================================================================
void CPlayer::CollisonDohyo(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 radius)
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
// �v���C���[�̃��f���ǂݍ��ݏ���
//=============================================================================
HRESULT CPlayer::LoadModel(void)
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
	D3DXLoadMeshFromX(PLAYER_MODEL_NAME,
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
// �v���C���[�̃e�N�X�`���������
//=============================================================================
void CPlayer::UnloadModel(void)
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
bool CPlayer::GetLand(void)
{
	return m_bLand;
}

//=============================================================================
// ��Ԃ��擾
//=============================================================================
CPlayer::STATE CPlayer::GetState(void)
{
	return m_State;
}

//=============================================================================
// ��Ԃ�ݒ�
//=============================================================================
void CPlayer::SetState(STATE state)
{
	m_State = state;
}

//=============================================================================
// �ړ��ʂ�ݒ�
//=============================================================================
void CPlayer::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}