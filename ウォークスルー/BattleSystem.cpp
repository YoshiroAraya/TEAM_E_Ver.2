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
#define OSIAI_MOVE (5.0f)
#define NAGE_MOVE (8.0f)

#define OSIAI_FLAME (20)
#define NAGE_FLAME (30)

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
	if (pPlayer->GetState() == CPlayer::STATE_KUMI
		&& pEnemy->GetState() == CEnemy::STATE_KUMI
		&& m_bAttack == false)
	{
		CDebugProc::Print("c", " 組み合い ");

		//向いてる方向に押す プレイヤー
		switch (pPlayer->GetDirection())
		{
		case CPlayer::DIRECTION_LEFT:
			if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
			{
				CDebugProc::Print("c", " 押す ");
				pPlayer->SetMove(D3DXVECTOR3(-OSIAI_MOVE, 3.0f, 0.0f));
				pEnemy->SetMove(D3DXVECTOR3(-OSIAI_MOVE, 3.0f, 0.0f));
				m_nCntFlame = OSIAI_FLAME;
				m_bAttack = true;
			}

			if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
			{
				CDebugProc::Print("c", " 投げる ");
				pPlayer->SetMove(D3DXVECTOR3(-NAGE_MOVE, 3.0f, 0.0f));
				pEnemy->SetMove(D3DXVECTOR3(NAGE_MOVE, 3.0f, 0.0f));
				m_bAttack = true;
				m_nCntFlame = NAGE_FLAME;
				CGame::SetHit(false);
				pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				pEnemy->SetState(CEnemy::STATE_NEUTRAL);
			}
			break;
		case CPlayer::DIRECTION_RIGHT:
			if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
			{
				CDebugProc::Print("c", " 押す ");
				pPlayer->SetMove(D3DXVECTOR3(OSIAI_MOVE, 3.0f, 0.0f));
				pEnemy->SetMove(D3DXVECTOR3(OSIAI_MOVE, 3.0f, 0.0f));
				m_nCntFlame = OSIAI_FLAME;
				m_bAttack = true;
			}

			if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
			{
				CDebugProc::Print("c", " 投げる ");
				pPlayer->SetMove(D3DXVECTOR3(NAGE_MOVE, 3.0f, 0.0f));
				pEnemy->SetMove(D3DXVECTOR3(-NAGE_MOVE, 3.0f, 0.0f));
				m_bAttack = true;
				m_nCntFlame = NAGE_FLAME;
				CGame::SetHit(false);
				pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				pEnemy->SetState(CEnemy::STATE_NEUTRAL);
			}
			break;
		}

		//向いてる方向に押す エネミー
		switch (pEnemy->GetDirection())
		{
		case CEnemy::DIRECTION_LEFT:
			if (pInputKeyboard->GetTrigger(DIK_NUMPAD4) == true)
			{
				CDebugProc::Print("c", " 押す ");
				pPlayer->SetMove(D3DXVECTOR3(-OSIAI_MOVE, 3.0f, 0.0f));
				pEnemy->SetMove(D3DXVECTOR3(-OSIAI_MOVE, 3.0f, 0.0f));
				m_bAttack = true;
				m_nCntFlame = OSIAI_FLAME;
			}
			if (pInputKeyboard->GetTrigger(DIK_NUMPAD6) == true)
			{
				CDebugProc::Print("c", " 投げる ");
				pPlayer->SetMove(D3DXVECTOR3(NAGE_MOVE, 3.0f, 0.0f));
				pEnemy->SetMove(D3DXVECTOR3(-NAGE_MOVE, 3.0f, 0.0f));
				m_bAttack = true;
				m_nCntFlame = NAGE_FLAME;
				CGame::SetHit(false);
				pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				pEnemy->SetState(CEnemy::STATE_NEUTRAL);
			}

			break;
		case CEnemy::DIRECTION_RIGHT:
			if (pInputKeyboard->GetTrigger(DIK_NUMPAD6) == true)
			{
				CDebugProc::Print("c", " 押す ");
				pPlayer->SetMove(D3DXVECTOR3(OSIAI_MOVE, 3.0f, 0.0f));
				pEnemy->SetMove(D3DXVECTOR3(OSIAI_MOVE, 3.0f, 0.0f));
				m_bAttack = true;
				m_nCntFlame = OSIAI_FLAME;
			}

			if (pInputKeyboard->GetTrigger(DIK_NUMPAD4) == true)
			{
					CDebugProc::Print("c", " 投げる ");
					pPlayer->SetMove(D3DXVECTOR3(-NAGE_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(NAGE_MOVE, 3.0f, 0.0f));
					m_bAttack = true;
					m_nCntFlame = NAGE_FLAME;
					CGame::SetHit(false);
					pPlayer->SetState(CPlayer::STATE_NEUTRAL);
					pEnemy->SetState(CEnemy::STATE_NEUTRAL);
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
		if (pPlayer->GetState() == CPlayer::STATE_KUMI
			&& pEnemy->GetState() == CEnemy::STATE_KUMI)
		{
			if (m_nCntFlame <= 0)
			{
				m_nCntFlame = 0;
				m_bAttack = false;
			}
		}
	}

	CDebugProc::Print("cn", " 行動不可フレーム ",m_nCntFlame);

#endif
}

