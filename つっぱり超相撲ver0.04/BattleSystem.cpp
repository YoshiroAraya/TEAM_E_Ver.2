//=============================================================================
//
// バトルシステム処理 [BattleSystem.cpp]
// Author : 目黒 未来也
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
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// マクロ
//=============================================================================
#define YORI_MOVE	(5.0f)
#define NAGE_MOVE	(8.0f)
#define OSI_MOVE	(8.0f)

#define YORI_FLAME	(20)
#define NAGE_FLAME	(30)
#define OSI_FLAME	(20)
//=============================================================================
// バトルシステムクラスのコンストラクタ
//=============================================================================
CBattleSys::CBattleSys()
{
	// 値をクリア
	m_nCntFlame = 0;
	m_nImpossibleFlame = 0;
	m_bAttack = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBattleSys::~CBattleSys()
{
}


//=============================================================================
// オブジェクトの生成処理
//=============================================================================
CBattleSys *CBattleSys::Create()
{
	CBattleSys *pBattleSys = NULL;

	if (pBattleSys == NULL)
	{
		// オブジェクトクラスの生成
		pBattleSys = new CBattleSys;

		if (pBattleSys != NULL)
		{
			pBattleSys->Init();
		}
	}

	return pBattleSys;
}

//=============================================================================
// バトルシステム初期化処理
//=============================================================================
HRESULT CBattleSys::Init()
{
	// 2Dオブジェクト初期化処理
	// オブジェクトの種類の設定
	//SetObjType(CScene::OBJTYPE_PLAYER);

	// 値をクリア
	m_nCntFlame = 0;
	m_nImpossibleFlame = 0;
	m_bAttack = false;

	return S_OK;
}

//=============================================================================
// バトルシステム終了処理
//=============================================================================
void CBattleSys::Uninit(void)
{
}

//=============================================================================
// バトルシステム更新処理
//=============================================================================
void CBattleSys::Update(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer ();
	// エネミーの取得
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();


#ifdef _DEBUG

	//2pの方が強い(処理が後に入る)
	if (pPlayer->GetState() == CPlayer::STATE_KUMI
		&& pEnemy->GetState() == CEnemy::STATE_KUMI
		&& m_bAttack == false)
	{
		CDebugProc::Print("c", " 組み合い ");

		//向いてる方向 プレイヤー
		switch (pPlayer->GetDirection())
		{
		case CPlayer::DIRECTION_LEFT:
			if (pInputKeyboard->GetPress(PLAYER_LEFT) == true)
			{
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true)
				{
					CDebugProc::Print("c", " 寄り ");
					pPlayer->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = YORI_FLAME;
					m_bAttack = true;
				}
				else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true)
				{
					CDebugProc::Print("c", " 押す ");
					pEnemy->SetMove(D3DXVECTOR3(-OSI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = OSI_FLAME;
					m_bAttack = true;
					//硬直
					Recovery();
				}
			}
			else if (pInputKeyboard->GetPress(PLAYER_RIGHT) == true)
			{
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true)
				{
					CDebugProc::Print("c", " 投げる ");
					pPlayer->SetMove(D3DXVECTOR3(-NAGE_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(NAGE_MOVE, 3.0f, 0.0f));
					m_bAttack = true;
					m_nCntFlame = NAGE_FLAME;
					//CGame::SetHit(false);
					//pPlayer->SetState(CPlayer::STATE_NEUTRAL);
					//pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				}
			}
			break;
		case CPlayer::DIRECTION_RIGHT:
			if (pInputKeyboard->GetPress(PLAYER_RIGHT) == true)
			{
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true)
				{
					CDebugProc::Print("c", " 寄り ");
					pPlayer->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = YORI_FLAME;
					m_bAttack = true;
				}
				else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true)
				{
					CDebugProc::Print("c", " 押す ");
					pEnemy->SetMove(D3DXVECTOR3(OSI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = OSI_FLAME;
					m_bAttack = true;
					//硬直
					Recovery();
				}
			}
			else if (pInputKeyboard->GetPress(PLAYER_LEFT) == true)
			{
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true)
				{
					CDebugProc::Print("c", " 投げる ");
					pPlayer->SetMove(D3DXVECTOR3(NAGE_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(-NAGE_MOVE, 3.0f, 0.0f));
					m_bAttack = true;
					m_nCntFlame = NAGE_FLAME;
					//CGame::SetHit(false);
					//pPlayer->SetState(CPlayer::STATE_NEUTRAL);
					//pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				}
			}
			break;
		}

		//向いてる方向 エネミー
		switch (pEnemy->GetDirection())
		{
		case CEnemy::DIRECTION_LEFT:
			if (pInputKeyboard->GetPress(ENEMY_LEFT) == true)
			{
				if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true)
				{
					CDebugProc::Print("c", " 寄り ");
					pPlayer->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					m_bAttack = true;
					m_nCntFlame = YORI_FLAME;
				}
				else if (pInputKeyboard->GetTrigger(ENEMY_B_BUTTON) == true)
				{
					CDebugProc::Print("c", " 押す ");
					pPlayer->SetMove(D3DXVECTOR3(-OSI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = OSI_FLAME;
					m_bAttack = true;
					//硬直
					Recovery();
				}
			}
			else if (pInputKeyboard->GetPress(ENEMY_RIGHT) == true)
			{
				if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true)
				{
					CDebugProc::Print("c", " 投げる ");
					pPlayer->SetMove(D3DXVECTOR3(NAGE_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(-NAGE_MOVE, 3.0f, 0.0f));
					m_bAttack = true;
					m_nCntFlame = NAGE_FLAME;
					//CGame::SetHit(false);
					//pPlayer->SetState(CPlayer::STATE_NEUTRAL);
					//pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				}
			}

			break;
		case CEnemy::DIRECTION_RIGHT:
			if (pInputKeyboard->GetPress(ENEMY_RIGHT) == true)
			{
				if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true)
				{
					CDebugProc::Print("c", " 寄り ");
					pPlayer->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					m_bAttack = true;
					m_nCntFlame = YORI_FLAME;
				}
				else if (pInputKeyboard->GetTrigger(ENEMY_B_BUTTON) == true)
				{
					CDebugProc::Print("c", " 押す ");
					pPlayer->SetMove(D3DXVECTOR3(OSI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = OSI_FLAME;
					m_bAttack = true;
					//硬直
					Recovery();
				}
			}
			else if (pInputKeyboard->GetPress(ENEMY_LEFT) == true)
			{
				if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true)
				{
					CDebugProc::Print("c", " 投げる ");
					pPlayer->SetMove(D3DXVECTOR3(-NAGE_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(NAGE_MOVE, 3.0f, 0.0f));
					m_bAttack = true;
					m_nCntFlame = NAGE_FLAME;
					//CGame::SetHit(false);
					//pPlayer->SetState(CPlayer::STATE_NEUTRAL);
					//pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				}
			}
			break;
		}
	}
	//リセット
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
		CDebugProc::Print("c", " 通常 ");
	}

	if (m_bAttack == true)
	{
		m_nCntFlame--;
		if (m_nCntFlame <= 0)
		{
			m_nCntFlame = 0;
			m_bAttack = false;
		}
	}

	CDebugProc::Print("cn", " 行動不可フレーム ",m_nCntFlame);

#endif
}


//=============================================================================
// 硬直処理まとめ
//=============================================================================
void CBattleSys::Recovery(void)
{
	// プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	// エネミーの取得
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

	CGame::SetHit(false);
	pPlayer->SetState(CPlayer::STATE_NEUTRAL);
	pEnemy->SetState(CEnemy::STATE_NEUTRAL);
	pPlayer->SetRecovery(true);
	pPlayer->SetRecoveryTime(20);
	pEnemy->SetRecovery(true);
	pEnemy->SetRecoveryTime(20);
}