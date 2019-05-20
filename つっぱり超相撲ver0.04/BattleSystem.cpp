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
#define GU_COUNTER	(70)
#define CHOKI_COUNTER	(10)
//=============================================================================
// バトルシステムクラスのコンストラクタ
//=============================================================================
CBattleSys::CBattleSys()
{
	// 値をクリア
	m_nCntFlame = 0;
	m_nImpossibleFlame = 0;
	m_bAttack = false;

	for (int nCntPlayer = 0; nCntPlayer < MAX_CHARACTER; nCntPlayer++)
	{
		m_aGUCounter[nCntPlayer] = 0;
		m_aCHOKICounter[nCntPlayer] = 0;
		m_abPA[nCntPlayer] = false;
	}
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
	m_aJanken[0] = JANKEN_GU;
	m_aJanken[1] = JANKEN_GU;
	for (int nCntPlayer = 0; nCntPlayer < MAX_CHARACTER; nCntPlayer++)
	{
		m_aGUCounter[nCntPlayer] = 0;
		m_aCHOKICounter[nCntPlayer] = 0;
		m_abPA[nCntPlayer] = false;
	}

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

	// プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	// エネミーの取得
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

	//キーボード操作処理
	KeyboardOperation();

	//コントローラー操作処理
	ControllerOperation();

#ifdef _DEBUG


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

	CDebugProc::Print("cn", " 行動不可フレーム ", m_nCntFlame);

#endif
}

//=============================================================================
// コントローラーの移動処理
//=============================================================================
void CBattleSys::ControllerOperation(void)
{
	// 入力情報を取得
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();

	// プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	// エネミーの取得
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

#ifdef _DEBUG

	if (pPlayer->GetState() == CPlayer::STATE_JANKEN)
	{
		CDebugProc::Print("c", " pじゃんけん ");

		if (pXInput->GetPress(XPLAYER_B_BUTTON, 0) == true)
		{
			m_aJanken[0] = JANKEN_GU;
			pPlayer->SetState(CPlayer::STATE_NOKOTTA);
		}
		else if (pXInput->GetPress(XPLAYER_Y_BUTTON, 0) == true)
		{
			m_aJanken[0] = JANKEN_CHOKI;
			pPlayer->SetState(CPlayer::STATE_NOKOTTA);
		}
		else if (pXInput->GetPress(XPLAYER_X_BUTTON, 0) == true)
		{
			m_aJanken[0] = JANKEN_PA;
			pPlayer->SetState(CPlayer::STATE_NOKOTTA);
			m_abPA[0] = true;
		}
	}

	if (pEnemy->GetState() == CEnemy::STATE_JANKEN)
	{
		CDebugProc::Print("c", " eじゃんけん ");

		if (pXInput->GetPress(XENEMY_B_BUTTON, 1) == true)
		{
			m_aJanken[1] = JANKEN_GU;
			pEnemy->SetState(CEnemy::STATE_NOKOTTA);
		}
		else if (pXInput->GetPress(XENEMY_Y_BUTTON, 1) == true)
		{
			m_aJanken[1] = JANKEN_CHOKI;
			pEnemy->SetState(CEnemy::STATE_NOKOTTA);
		}
		else if (pXInput->GetPress(XENEMY_X_BUTTON, 1) == true)
		{
			m_aJanken[1] = JANKEN_PA;
			pEnemy->SetState(CEnemy::STATE_NOKOTTA);
			m_abPA[1] = true;
		}
	}

	if (pPlayer->GetState() == CPlayer::STATE_NOKOTTA && pEnemy->GetState() == CEnemy::STATE_NOKOTTA)
	{// 2人のじゃんけんが決まったら
	 //===================================
	 // キャラ1
	 //===================================
		if (m_aJanken[0] == JANKEN_GU)
		{
			m_aGUCounter[0]++;

			if (m_aGUCounter[0] < GU_COUNTER)
			{
				pPlayer->SetMove(D3DXVECTOR3(1.5f, 0.0f, 0.0f));
			}
			else if (m_aGUCounter[0] >= GU_COUNTER)
			{
				pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				m_aGUCounter[0] = 0;
			}
		}
		else if (m_aJanken[0] == JANKEN_CHOKI)
		{
			m_aCHOKICounter[0]++;

			if (m_aCHOKICounter[0] > CHOKI_COUNTER && CHOKI_COUNTER + 5 >= m_aCHOKICounter[0])
			{
				pEnemy->SetMove(D3DXVECTOR3(OSI_MOVE, 3.0f, 0.0f));
			}
			else if (m_aCHOKICounter[0] > CHOKI_COUNTER + 5)
			{
				pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				m_aCHOKICounter[0] = 0;
			}
		}
		else if (m_aJanken[0] == JANKEN_PA)
		{
			if (m_abPA[0] == true)
			{
				pPlayer->SetMove(D3DXVECTOR3(0.0, 10.0f, 0.0f));
				m_abPA[0] = false;
			}

			if (pEnemy->GetState() == CEnemy::STATE_NEUTRAL)
			{
				pPlayer->SetState(CPlayer::STATE_NEUTRAL);
			}
		}

		//===================================
		// キャラ2
		//===================================
		if (m_aJanken[1] == JANKEN_GU)
		{
			m_aGUCounter[1]++;

			if (m_aGUCounter[1] < GU_COUNTER)
			{
				pEnemy->SetMove(D3DXVECTOR3(-1.5f, 0.0f, 0.0f));
			}
			else if (m_aGUCounter[1] >= GU_COUNTER)
			{
				pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				m_aGUCounter[1] = 0;
			}
		}
		else if (m_aJanken[1] == JANKEN_CHOKI)
		{
			m_aCHOKICounter[1]++;

			if (m_aCHOKICounter[1] > CHOKI_COUNTER && CHOKI_COUNTER + 5 >= m_aCHOKICounter[1])
			{
				pPlayer->SetMove(D3DXVECTOR3(-OSI_MOVE, 3.0f, 0.0f));
			}
			else if (m_aCHOKICounter[1] > CHOKI_COUNTER + 5)
			{
				pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				m_aCHOKICounter[1] = 0;
			}
		}
		else if (m_aJanken[1] == JANKEN_PA)
		{
			if (m_abPA[1] == true)
			{
				pEnemy->SetMove(D3DXVECTOR3(0.0, 10.0f, 0.0f));
				m_abPA[1] = false;
			}

			if (pPlayer->GetState() == CPlayer::STATE_NEUTRAL)
			{
				pEnemy->SetState(CEnemy::STATE_NEUTRAL);
			}
		}
	}

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
			if (pXInput->GetPress(XPLAYER_LEFT, 0) == true)
			{
				if (pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					CDebugProc::Print("c", " 寄り ");
					pPlayer->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = YORI_FLAME;
					m_bAttack = true;
				}
				else if (pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
				{
					CDebugProc::Print("c", " 押す ");
					pEnemy->SetMove(D3DXVECTOR3(-OSI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = OSI_FLAME;
					m_bAttack = true;
					//硬直
					Recovery();
				}
			}
			else if (pXInput->GetPress(XPLAYER_RIGHT, 0) == true)
			{
				if (pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
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
			if (pXInput->GetPress(XPLAYER_RIGHT, 0) == true)
			{
				if (pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					CDebugProc::Print("c", " 寄り ");
					pPlayer->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = YORI_FLAME;
					m_bAttack = true;
				}
				else if (pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
				{
					CDebugProc::Print("c", " 押す ");
					pEnemy->SetMove(D3DXVECTOR3(OSI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = OSI_FLAME;
					m_bAttack = true;
					//硬直
					Recovery();
				}
			}
			else if (pXInput->GetPress(XPLAYER_LEFT, 0) == true)
			{
				if (pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
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
			if (pXInput->GetPress(XENEMY_LEFT, 1) == true)
			{
				if (pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
				{
					CDebugProc::Print("c", " 寄り ");
					pPlayer->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					m_bAttack = true;
					m_nCntFlame = YORI_FLAME;
				}
				else if (pXInput->GetTrigger(XENEMY_B_BUTTON, 1) == true)
				{
					CDebugProc::Print("c", " 押す ");
					pPlayer->SetMove(D3DXVECTOR3(-OSI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = OSI_FLAME;
					m_bAttack = true;
					//硬直
					Recovery();
				}
			}
			else if (pXInput->GetPress(XENEMY_RIGHT, 1) == true)
			{
				if (pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
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
			if (pXInput->GetPress(XENEMY_RIGHT, 1) == true)
			{
				if (pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
				{
					CDebugProc::Print("c", " 寄り ");
					pPlayer->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					m_bAttack = true;
					m_nCntFlame = YORI_FLAME;
				}
				else if (pXInput->GetTrigger(XENEMY_B_BUTTON, 1) == true)
				{
					CDebugProc::Print("c", " 押す ");
					pPlayer->SetMove(D3DXVECTOR3(OSI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = OSI_FLAME;
					m_bAttack = true;
					//硬直
					Recovery();
				}
			}
			else if (pXInput->GetPress(XENEMY_LEFT, 1) == true)
			{
				if (pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
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
	if (pXInput->GetTrigger(XINPUT_GAMEPAD_START, 0) == true)
	{
		pPlayer->SetPosition(D3DXVECTOR3(-20.0f, 50.0f, 0.0f));
		pEnemy->SetPosition(D3DXVECTOR3(20.0f, 50.0f, 0.0f));
		pPlayer->SetState(CPlayer::STATE_NEUTRAL);
		pEnemy->SetState(CEnemy::STATE_NEUTRAL);
		m_bAttack = false;
		m_nCntFlame = 0;
		CGame::SetHit(false);
	}
#endif
}

//=============================================================================
// キーボードの操作処理
//=============================================================================
void CBattleSys::KeyboardOperation(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	// エネミーの取得
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

#ifdef _DEBUG

	if (pPlayer->GetState() == CPlayer::STATE_JANKEN)
	{
		CDebugProc::Print("c", " pじゃんけん ");

		if (pInputKeyboard->GetPress(DIK_Z) == true)
		{
			m_aJanken[0] = JANKEN_GU;
			pPlayer->SetState(CPlayer::STATE_NOKOTTA);
		}
		else if (pInputKeyboard->GetPress(DIK_X) == true)
		{
			m_aJanken[0] = JANKEN_CHOKI;
			pPlayer->SetState(CPlayer::STATE_NOKOTTA);
		}
		else if (pInputKeyboard->GetPress(DIK_C) == true)
		{
			m_aJanken[0] = JANKEN_PA;
			pPlayer->SetState(CPlayer::STATE_NOKOTTA);
			m_abPA[0] = true;
		}
	}

	if (pEnemy->GetState() == CEnemy::STATE_JANKEN)
	{
		CDebugProc::Print("c", " eじゃんけん ");

		if (pInputKeyboard->GetPress(DIK_B) == true)
		{
			m_aJanken[1] = JANKEN_GU;
			pEnemy->SetState(CEnemy::STATE_NOKOTTA);
		}
		else if (pInputKeyboard->GetPress(DIK_N) == true)
		{
			m_aJanken[1] = JANKEN_CHOKI;
			pEnemy->SetState(CEnemy::STATE_NOKOTTA);
		}
		else if (pInputKeyboard->GetPress(DIK_M) == true)
		{
			m_aJanken[1] = JANKEN_PA;
			pEnemy->SetState(CEnemy::STATE_NOKOTTA);
			m_abPA[1] = true;
		}
	}

	if (pPlayer->GetState() == CPlayer::STATE_NOKOTTA && pEnemy->GetState() == CEnemy::STATE_NOKOTTA)
	{// 2人のじゃんけんが決まったら
	 //===================================
	 // キャラ1
	 //===================================
		if (m_aJanken[0] == JANKEN_GU)
		{
			m_aGUCounter[0]++;

			if (m_aGUCounter[0] < GU_COUNTER)
			{
				pPlayer->SetMove(D3DXVECTOR3(1.5f, 0.0f, 0.0f));
			}
			else if (m_aGUCounter[0] >= GU_COUNTER)
			{
				pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				m_aGUCounter[0] = 0;
			}
		}
		else if (m_aJanken[0] == JANKEN_CHOKI)
		{
			m_aCHOKICounter[0]++;

			if (m_aCHOKICounter[0] > CHOKI_COUNTER && CHOKI_COUNTER + 5 >= m_aCHOKICounter[0])
			{
				pEnemy->SetMove(D3DXVECTOR3(OSI_MOVE, 3.0f, 0.0f));
			}
			else if (m_aCHOKICounter[0] > CHOKI_COUNTER + 5)
			{
				pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				m_aCHOKICounter[0] = 0;
			}
		}
		else if (m_aJanken[0] == JANKEN_PA)
		{
			if (m_abPA[0] == true)
			{
				pPlayer->SetMove(D3DXVECTOR3(0.0, 10.0f, 0.0f));
				m_abPA[0] = false;
			}

			if (pEnemy->GetState() == CEnemy::STATE_NEUTRAL)
			{
				pPlayer->SetState(CPlayer::STATE_NEUTRAL);
			}
		}

		//===================================
		// キャラ2
		//===================================
		if (m_aJanken[1] == JANKEN_GU)
		{
			m_aGUCounter[1]++;

			if (m_aGUCounter[1] < GU_COUNTER)
			{
				pEnemy->SetMove(D3DXVECTOR3(-1.5f, 0.0f, 0.0f));
			}
			else if (m_aGUCounter[1] >= GU_COUNTER)
			{
				pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				m_aGUCounter[1] = 0;
			}
		}
		else if (m_aJanken[1] == JANKEN_CHOKI)
		{
			m_aCHOKICounter[1]++;

			if (m_aCHOKICounter[1] > CHOKI_COUNTER && CHOKI_COUNTER + 5 >= m_aCHOKICounter[1])
			{
				pPlayer->SetMove(D3DXVECTOR3(-OSI_MOVE, 3.0f, 0.0f));
			}
			else if (m_aCHOKICounter[1] > CHOKI_COUNTER + 5)
			{
				pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				m_aCHOKICounter[1] = 0;
			}
		}
		else if (m_aJanken[1] == JANKEN_PA)
		{
			if (m_abPA[1] == true)
			{
				pEnemy->SetMove(D3DXVECTOR3(0.0, 10.0f, 0.0f));
				m_abPA[1] = false;
			}

			if (pPlayer->GetState() == CPlayer::STATE_NEUTRAL)
			{
				pEnemy->SetState(CEnemy::STATE_NEUTRAL);
			}
		}
	}

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