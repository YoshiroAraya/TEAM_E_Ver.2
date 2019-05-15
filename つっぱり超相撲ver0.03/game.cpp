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
#include "scene3D.h"
#include "game.h"
#include "meshField.h"
#include "dohyo.h"
#include "shadow.h"
#include "bullet.h"
#include "debugProc.h"
#include "scene.h"
#include "fade.h"
#include "input.h"
#include "BattleSystem.h"
//============================================================================
//	�}�N����`
//============================================================================
#define SIZE_X (SCREEN_WIDTH)
#define SIZE_Y (SCREEN_HEIGHT)

//============================================================================
//�ÓI�����o�ϐ��錾
//============================================================================
CScene3D *CGame::m_pScene3D = NULL;
CPlayer *CGame::m_pPlayer = NULL;
CEnemy *CGame::m_pEnemy = NULL;
CShadow *CGame::m_pShadow = NULL;
CMeshField *CGame::m_pMeshField = NULL;
CBattleSys *CGame::m_pBatlteSys = NULL;
//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CGame::CGame()
{

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

	//�C���X�^���X
	CManager *pManager = NULL;

	

	CDohyo::LoadModel();
	CDohyo::LoadMat();
	CDohyo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (m_pScene3D == NULL)
	{
		//m_pScene3D = CScene3D::Create(D3DXVECTOR3(200.0f, 0.0f, 0.0f));
	}

	if (m_pPlayer == NULL)
	{// �v���C���[
		CPlayer::LoadModel();

		m_pPlayer = CPlayer::Create(D3DXVECTOR3(-20.0f, 50.0f, 0.0f));
	}

	if (m_pEnemy == NULL)
	{// �G�l�~�[
		CEnemy::LoadModel();

		m_pEnemy = CEnemy::Create(D3DXVECTOR3(20.0f, 50.0f, 0.0f));
	}
	if (m_pMeshField == NULL)
	{
		// ���b�V���t�B�[���h�̐���
		//	m_pMeshField = CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	// 2D�|���S���̐���
	//CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH -50.0f, 50.0f, 0.0f));

	// 3D�|���S���̐���
	//CScene3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 3D���f��
	//CSceneX::Create(D3DXVECTOR3(0.0f, 25.0f, 0.0f));

	// �e�̃e�N�X�`����ǂݍ���
	CBullet::Load();

	CShadow::Load();

	if (m_pShadow == NULL)
	{
		m_pShadow = CShadow::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (m_pBatlteSys == NULL)
	{
		m_pBatlteSys = CBattleSys::Create();
	}
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void CGame::Uninit(void)
{
	CBullet::Unload();

	CPlayer::UnloadModel();
	CDohyo::UnloadModel();
	CDohyo::UnloadMat();
	CShadow::UnLoad();

	//m_pScene3D = NULL;
	m_pPlayer = NULL;
	m_pEnemy = NULL;
	m_pMeshField = NULL;
	m_pShadow = NULL;

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


	bool bHit = Collision(&m_pPlayer->GetPosition(), 10.0f, &m_pEnemy->GetPosition(), 10.0f);


	CDebugProc::Print("c", "�Q�[�����[�h");

	//�C�ӂ̃L�[��
	if (pInputKeyboard->GetPress(DIK_RETURN) == true)
	{
		pFade->SetFade(pManager->MODE_GAME, pFade->FADE_OUT);
	}

	if (m_pBatlteSys != NULL)
	{
		m_pBatlteSys->Update();
	}


#ifdef _DEBUG
	if (bHit == true)
	{
		CDebugProc::Print("c", "�������Ă���");
	}
	else
	{
		CDebugProc::Print("c", "�������Ă��Ȃ�");
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
CShadow *CGame::GetShadow(void)
{
	return m_pShadow;
}


//=============================================================================
// ���b�V���t�B�[���h�̎擾
//=============================================================================
CMeshField *CGame::GetMeshField(void)
{
	return m_pMeshField;
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

	if (fLength < fRadius0 + fRadius1 && fLength < fRadius0 + fRadius1 && fLength < fRadius0 + fRadius1)
	{// ���������a�̘a��菬������Γ������Ă���
		bHit = true;
	}

	return bHit;	// �u���b�N�ɓ������Ă��邩�ǂ�����Ԃ�
}
