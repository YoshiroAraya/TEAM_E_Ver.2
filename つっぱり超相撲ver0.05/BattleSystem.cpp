//=============================================================================
//
// バトルシステム処理 [BattleSystem.cpp]
// Author : 目黒 未来也 & 長山 拓実 & 山下 敦史
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

#include "gauge.h"
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// マクロ
//=============================================================================
#define YORI_MOVE	(5.0f)
#define NAGE_MOVE	(35.0f)
#define OSI_MOVE	(8.0f)
#define TUPARI_MOVE	(10.0f)

#define YORI_FLAME			(20)
#define NAGE_FLAME			(30)
#define OSI_FLAME			(20)
#define TUPARI_FLAME		(10)
#define TUPARI_RECOVERY		(40)

#define GU_COUNTER			(40)
#define CHOKI_COUNTER		(10)

#define DAMAGE				(30)
#define HEEL				(20)
#define START_SECOND		(2)	// 対戦が始まるまでの秒数

//=============================================================================
// バトルシステムクラスのコンストラクタ
//=============================================================================
CBattleSys::CBattleSys()
{
	// 値をクリア
	m_nCntFlame = 0;
	m_nImpossibleFlame = 0;
	m_bAttack = false;
	m_nStartCounter = 0;

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
	m_nStartCounter = 0;

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
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	// エネミーの取得
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();
	//ゲージの取得
	CGauge *pGauge;
	pGauge = CGame::GetGauge();

#ifdef _DEBUG

	//キーボード/コントローラー操作
	Operation();



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
			//状態を通常状態へ
			if (pPlayer->GetState() == CPlayer::STATE_TSUPPARI || pPlayer->GetState() == CPlayer::STATE_NAGE)
			{
				pPlayer->SetState(CPlayer::STATE_NEUTRAL);
			}
			if (pEnemy->GetState() == CEnemy::STATE_TSUPPARI || pEnemy->GetState() == CPlayer::STATE_NAGE)
			{
				pEnemy->SetState(CEnemy::STATE_NEUTRAL);
			}

			m_nCntFlame = 0;
			m_bAttack = false;
		}
	}

	CDebugProc::Print("cn", " 行動不可フレーム ", m_nCntFlame);

#endif
}

//=============================================================================
// キーボード/コントローラーの操作処理
//=============================================================================
void CBattleSys::Operation(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();

	// プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	// エネミーの取得
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

	CGauge *pGauge;
	pGauge = CGame::GetGauge();

#ifdef _DEBUG
	if (CGame::GetState() == CGame::STATE_GAME)
	{
		if (pPlayer->GetState() == CPlayer::STATE_JANKEN)
		{
			CDebugProc::Print("c", " pじゃんけん ");

			if (pInputKeyboard->GetPress(DIK_Z) == true ||
				pXInput->GetPress(XPLAYER_B_BUTTON, 0) == true)
			{
				m_aJanken[0] = JANKEN_GU;
				pPlayer->SetState(CPlayer::STATE_NOKOTTA);
			}
			else if (pInputKeyboard->GetPress(DIK_X) == true ||
				pXInput->GetPress(XPLAYER_Y_BUTTON, 0) == true)
			{
				m_aJanken[0] = JANKEN_CHOKI;
				pPlayer->SetState(CPlayer::STATE_NOKOTTA);
			}
			else if (pInputKeyboard->GetPress(DIK_C) == true ||
				pXInput->GetPress(XPLAYER_X_BUTTON, 0) == true)
			{
				m_aJanken[0] = JANKEN_PA;
				pPlayer->SetState(CPlayer::STATE_NOKOTTA);
				m_abPA[0] = true;
			}
		}

		if (pEnemy->GetState() == CEnemy::STATE_JANKEN)
		{
			CDebugProc::Print("c", " eじゃんけん ");

			if (pInputKeyboard->GetPress(DIK_B) == true ||
				pXInput->GetPress(XENEMY_B_BUTTON, 1) == true)
			{
				m_aJanken[1] = JANKEN_GU;
				pEnemy->SetState(CEnemy::STATE_NOKOTTA);
			}
			else if (pInputKeyboard->GetPress(DIK_N) == true ||
				pXInput->GetPress(XENEMY_Y_BUTTON, 1) == true)
			{
				m_aJanken[1] = JANKEN_CHOKI;
				pEnemy->SetState(CEnemy::STATE_NOKOTTA);
			}
			else if (pInputKeyboard->GetPress(DIK_M) == true ||
				pXInput->GetPress(XENEMY_X_BUTTON, 1) == true)
			{
				m_aJanken[1] = JANKEN_PA;
				pEnemy->SetState(CEnemy::STATE_NOKOTTA);
				m_abPA[1] = true;
			}
		}
	}

	if (pPlayer->GetState() == CPlayer::STATE_NOKOTTA && pEnemy->GetState() == CEnemy::STATE_NOKOTTA)
	{// 2人のじゃんけんが決まったら
		int nTime = (int)(m_nStartCounter / 60);

		if (nTime < START_SECOND)
		{
			m_nStartCounter++;
		}

		if (nTime >= START_SECOND)
		{
			if (m_aJanken[0] == JANKEN_GU && m_aJanken[1] == JANKEN_GU)
			{// グーとグー
				if (CGame::GetHit() == false)
				{
					pPlayer->SetMove(D3DXVECTOR3(3.0f, 0.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(-3.0f, 0.0f, 0.0f));
				}
			}
			else if (m_aJanken[0] == JANKEN_GU && m_aJanken[1] == JANKEN_CHOKI)
			{// グーとチョキ
				m_aGUCounter[0]++;

				if (m_aGUCounter[0] < GU_COUNTER)
				{
					pPlayer->SetMove(D3DXVECTOR3(3.0f, 0.0f, 0.0f));
				}
				else if (m_aGUCounter[0] >= GU_COUNTER)
				{
					pPlayer->SetState(CPlayer::STATE_NEUTRAL);
					m_aGUCounter[0] = 0;
				}

				if (CGame::GetHit() == true)
				{
					pEnemy->SetMove(D3DXVECTOR3(OSI_MOVE * 3, 5.0f, 0.0f));
					pPlayer->SetState(CPlayer::STATE_NEUTRAL);
					pEnemy->SetState(CEnemy::STATE_NEUTRAL);
					CGame::SetHit(false);
				}
			}
			else if (m_aJanken[0] == JANKEN_GU && m_aJanken[1] == JANKEN_PA)
			{// グーとパー
				m_aGUCounter[0]++;

				if (m_aGUCounter[0] < GU_COUNTER)
				{
					pPlayer->SetMove(D3DXVECTOR3(3.0f, 0.0f, 0.0f));
				}
				else if (m_aGUCounter[0] >= GU_COUNTER)
				{
					pPlayer->SetState(CPlayer::STATE_NEUTRAL);
					m_aGUCounter[0] = 0;
				}

				if (m_abPA[1] == true)
				{
					pEnemy->SetMove(D3DXVECTOR3(0.0, 10.0f, 0.0f));
					m_abPA[1] = false;
				}
			}
			else if (m_aJanken[0] == JANKEN_CHOKI && m_aJanken[1] == JANKEN_GU)
			{// チョキとグー
				if (CGame::GetHit() == true)
				{
					pPlayer->SetMove(D3DXVECTOR3(-OSI_MOVE * 3, 5.0f, 0.0f));
					pPlayer->SetState(CPlayer::STATE_NEUTRAL);
					pEnemy->SetState(CEnemy::STATE_NEUTRAL);
					CGame::SetHit(false);
				}

				m_aGUCounter[1]++;

				if (m_aGUCounter[1] < GU_COUNTER)
				{
					pEnemy->SetMove(D3DXVECTOR3(-3.0f, 0.0f, 0.0f));
				}
				else if (m_aGUCounter[1] >= GU_COUNTER)
				{
					pEnemy->SetState(CEnemy::STATE_NEUTRAL);
					m_aGUCounter[1] = 0;
				}
			}
			else if (m_aJanken[0] == JANKEN_CHOKI && m_aJanken[1] == JANKEN_CHOKI)
			{// チョキとチョキ
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
			else if (m_aJanken[0] == JANKEN_CHOKI && m_aJanken[1] == JANKEN_PA)
			{// チョキとパー
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
			else if (m_aJanken[0] == JANKEN_PA && m_aJanken[1] == JANKEN_GU)
			{// パーとグー
				if (m_abPA[0] == true)
				{
					pPlayer->SetMove(D3DXVECTOR3(0.0, 10.0f, 0.0f));
					m_abPA[0] = false;
				}
				if (pEnemy->GetState() == CEnemy::STATE_NEUTRAL)
				{
					pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				}

				m_aGUCounter[1]++;

				if (m_aGUCounter[1] < GU_COUNTER)
				{
					pEnemy->SetMove(D3DXVECTOR3(-3.0f, 0.0f, 0.0f));
				}
				else if (m_aGUCounter[1] >= GU_COUNTER)
				{
					pEnemy->SetState(CEnemy::STATE_NEUTRAL);
					m_aGUCounter[1] = 0;
				}
			}
			else if (m_aJanken[0] == JANKEN_PA && m_aJanken[1] == JANKEN_CHOKI)
			{// パーとチョキ
				if (m_abPA[0] == true)
				{
					pPlayer->SetMove(D3DXVECTOR3(0.0, 10.0f, 0.0f));
					m_abPA[0] = false;
				}
				if (pEnemy->GetState() == CEnemy::STATE_NEUTRAL)
				{
					pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				}

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
			else if (m_aJanken[0] == JANKEN_PA && m_aJanken[1] == JANKEN_PA)
			{// パーとパー
				if (m_abPA[0] == true)
				{
					pPlayer->SetMove(D3DXVECTOR3(0.0, 10.0f, 0.0f));
					m_abPA[0] = false;
					pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				}

				if (m_abPA[1] == true)
				{
					pEnemy->SetMove(D3DXVECTOR3(0.0, 10.0f, 0.0f));
					m_abPA[1] = false;
					pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				}
			}
		}
	}

	//瀕死時の移動量
	float fMoveDying[2] = { 1.0f,1.0f };
	//瀕死時
	if (pPlayer->GetDying() == true)
	{
		fMoveDying[0] = 2.0f;
	}
	if (pEnemy->GetDying() == true)
	{
		fMoveDying[1] = 2.0f;
	}
	//プレイヤー1の位置を取得
	D3DXVECTOR3 p1pos = pPlayer->GetPosition();




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
			if (pInputKeyboard->GetPress(PLAYER_LEFT) == true ||
				pXInput->GetPress(XPLAYER_LEFT, 0) == true)
			{
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					CDebugProc::Print("c", " 寄り ");
					pPlayer->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					m_nCntFlame = YORI_FLAME;
					pGauge->SetGaugeRightLeft(HEEL, -DAMAGE);
					m_bAttack = true;
				}
				else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
				{
					CDebugProc::Print("c", " 押す ");

					pEnemy->SetMove(D3DXVECTOR3(-OSI_MOVE * fMoveDying[1], 3.0f, 0.0f));
					m_nCntFlame = OSI_FLAME;
					pGauge->SetGaugeRightLeft(HEEL, -DAMAGE);
					m_bAttack = true;
					//硬直
					Recovery();
				}
			}
			else if (pInputKeyboard->GetPress(PLAYER_RIGHT) == true ||
				pXInput->GetPress(XPLAYER_RIGHT, 0) == true)
			{
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					CDebugProc::Print("c", " 投げる ");

					pEnemy->SetMove(D3DXVECTOR3((NAGE_MOVE * fMoveDying[1]) / 1.5f, 3.0f, 0.0f));


					m_bAttack = true;
					m_nCntFlame = NAGE_FLAME;
					pGauge->SetGaugeRightLeft(HEEL, -DAMAGE);
					//硬直
					Recovery();
					CGame::SetHit(false);
					pEnemy->SetState(CEnemy::STATE_NAGE);
					pPlayer->SetState(CPlayer::STATE_NAGE);
				}
			}
			break;
		case CPlayer::DIRECTION_RIGHT:
			if (pInputKeyboard->GetPress(PLAYER_RIGHT) == true ||
				pXInput->GetPress(XPLAYER_RIGHT, 0) == true)
			{
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					CDebugProc::Print("c", " 寄り ");
					pPlayer->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					pGauge->SetGaugeRightLeft(HEEL, -DAMAGE);
					m_nCntFlame = YORI_FLAME;
					m_bAttack = true;
				}
				else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
				{
					CDebugProc::Print("c", " 押す ");

					pEnemy->SetMove(D3DXVECTOR3(OSI_MOVE * fMoveDying[1], 3.0f, 0.0f));

					pGauge->SetGaugeRightLeft(HEEL, -DAMAGE);
					m_nCntFlame = OSI_FLAME;
					m_bAttack = true;
					//硬直
					Recovery();
				}
			}
			else if (pInputKeyboard->GetPress(PLAYER_LEFT) == true ||
				pXInput->GetPress(XPLAYER_LEFT, 0) == true)
			{
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					CDebugProc::Print("c", " 投げる ");
					pEnemy->SetMove(D3DXVECTOR3((-NAGE_MOVE * fMoveDying[1]) / 1.5f, 3.0f, 0.0f));

					pGauge->SetGaugeRightLeft(HEEL, -DAMAGE);
					m_bAttack = true;
					m_nCntFlame = NAGE_FLAME;
					//硬直
					Recovery();
					CGame::SetHit(false);
					pEnemy->SetState(CEnemy::STATE_NAGE);
					pPlayer->SetState(CPlayer::STATE_NAGE);
				}
			}
			break;
		}

		//向いてる方向 エネミー
		switch (pEnemy->GetDirection())
		{
		case CEnemy::DIRECTION_LEFT:
			if (pInputKeyboard->GetPress(ENEMY_LEFT) == true ||
				pXInput->GetPress(XENEMY_LEFT, 1) == true)
			{
				if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true ||
					pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
				{
					CDebugProc::Print("c", " 寄り ");
					pPlayer->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(-YORI_MOVE, 3.0f, 0.0f));
					pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
					m_bAttack = true;
					m_nCntFlame = YORI_FLAME;
				}
				else if (pInputKeyboard->GetTrigger(ENEMY_B_BUTTON) == true ||
					pXInput->GetTrigger(XENEMY_B_BUTTON, 1) == true)
				{
					CDebugProc::Print("c", " 押す ");

					pPlayer->SetMove(D3DXVECTOR3(-OSI_MOVE * fMoveDying[0], 3.0f, 0.0f));

					pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
					m_nCntFlame = OSI_FLAME;
					m_bAttack = true;
					//硬直
					Recovery();
				}
			}
			else if (pInputKeyboard->GetPress(ENEMY_RIGHT) == true ||
				pXInput->GetPress(XENEMY_RIGHT, 1) == true)
			{
				if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true ||
					pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
				{
					CDebugProc::Print("c", " 投げる ");

					pPlayer->SetMove(D3DXVECTOR3((NAGE_MOVE * fMoveDying[0]) / 1.5f, 3.0f, 0.0f));

					m_bAttack = true;
					pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
					m_nCntFlame = NAGE_FLAME;
					//硬直
					Recovery();
					CGame::SetHit(false);
					pEnemy->SetState(CEnemy::STATE_NAGE);
					pPlayer->SetState(CPlayer::STATE_NAGE);
				}
			}

			break;
		case CEnemy::DIRECTION_RIGHT:
			if (pInputKeyboard->GetPress(ENEMY_RIGHT) == true ||
				pXInput->GetPress(XENEMY_RIGHT, 1) == true)
			{
				if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true ||
					pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
				{
					CDebugProc::Print("c", " 寄り ");
					pPlayer->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(YORI_MOVE, 3.0f, 0.0f));
					m_bAttack = true;
					pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
					m_nCntFlame = YORI_FLAME;
				}
				else if (pInputKeyboard->GetTrigger(ENEMY_B_BUTTON) == true ||
					pXInput->GetTrigger(XENEMY_B_BUTTON, 1) == true)
				{
					CDebugProc::Print("c", " 押す ");

					pPlayer->SetMove(D3DXVECTOR3(OSI_MOVE * fMoveDying[0], 3.0f, 0.0f));

					pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
					m_nCntFlame = OSI_FLAME;
					m_bAttack = true;
					//硬直
					Recovery();
				}
			}
			else if (pInputKeyboard->GetPress(ENEMY_LEFT) == true ||
				pXInput->GetPress(XENEMY_LEFT, 1) == true)
			{
				if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true ||
					pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
				{
					CDebugProc::Print("c", " 投げる ");

					pPlayer->SetMove(D3DXVECTOR3((-NAGE_MOVE * fMoveDying[0]) / 1.5f, 3.0f, 0.0f));

					m_bAttack = true;
					pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
					m_nCntFlame = NAGE_FLAME;
					//硬直
					Recovery();
				}
			}
			break;
		}
	}
	//つっぱり
	else if (pPlayer->GetState() == CPlayer::STATE_NEUTRAL
		&& pEnemy->GetState() == CEnemy::STATE_NEUTRAL
		&& m_bAttack == false)
	{
		if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true && pPlayer->GetRecovery() == false ||
			pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true && pPlayer->GetRecovery() == false)
		{
			//向いてる方向 プレイヤー
			switch (pPlayer->GetDirection())
			{
			case CPlayer::DIRECTION_LEFT:
				pPlayer->SetState(CPlayer::STATE_TSUPPARI);
				pPlayer->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x - 10, p1pos.y, p1pos.z));
				m_nCntFlame = TUPARI_FLAME;
				pPlayer->SetRecovery(true);
				pPlayer->SetRecoveryTime(TUPARI_RECOVERY);
				m_bAttack = true;
				break;
			case CPlayer::DIRECTION_RIGHT:
				pPlayer->SetState(CPlayer::STATE_TSUPPARI);
				pPlayer->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x + 10, p1pos.y, p1pos.z));
				m_nCntFlame = TUPARI_FLAME;
				pPlayer->SetRecovery(true);
				pPlayer->SetRecoveryTime(TUPARI_RECOVERY);
				m_bAttack = true;
				break;
			}
		}
		if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true && pEnemy->GetRecovery() == false ||
			pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true && pEnemy->GetRecovery() == false)
		{
			//向いてる方向 エネミー
			switch (pEnemy->GetDirection())
			{
			case CEnemy::DIRECTION_LEFT:
				pEnemy->SetState(CEnemy::STATE_TSUPPARI);
				pEnemy->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x - 10, p1pos.y, p1pos.z));
				m_nCntFlame = TUPARI_FLAME;
				pEnemy->SetRecovery(true);
				pEnemy->SetRecoveryTime(TUPARI_RECOVERY);
				m_bAttack = true;
				break;
			case CEnemy::DIRECTION_RIGHT:
				pEnemy->SetState(CEnemy::STATE_TSUPPARI);
				pEnemy->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x + 10, p1pos.y, p1pos.z));
				m_nCntFlame = TUPARI_FLAME;
				pEnemy->SetRecovery(true);
				pEnemy->SetRecoveryTime(TUPARI_RECOVERY);
				m_bAttack = true;
				break;
			}
		}
	}

	//ダメージなら吹っ飛ぶ プレイヤーのつっぱり
	if (pEnemy->GetState() == CEnemy::STATE_DAMAGE)
	{
		switch (pPlayer->GetDirection())
		{
		case CPlayer::DIRECTION_LEFT:
			pEnemy->SetMove(D3DXVECTOR3(-TUPARI_MOVE, 3.0f, 0.0f));
			pEnemy->SetState(CEnemy::STATE_NEUTRAL);
			break;
		case CPlayer::DIRECTION_RIGHT:
			pEnemy->SetMove(D3DXVECTOR3(TUPARI_MOVE, 3.0f, 0.0f));
			pEnemy->SetState(CEnemy::STATE_NEUTRAL);
			break;
		}
		//ダメージ
		pGauge->SetGaugeRightLeft(HEEL, -DAMAGE);
	}

	//ダメージなら吹っ飛ぶ エネミーのつっぱり
	if (pPlayer->GetState() == CPlayer::STATE_DAMAGE)
	{
		switch (pEnemy->GetDirection())
		{
		case CEnemy::DIRECTION_LEFT:
			pPlayer->SetMove(D3DXVECTOR3(-TUPARI_MOVE, 3.0f, 0.0f));
			pPlayer->SetState(CPlayer::STATE_NEUTRAL);
			break;
		case CEnemy::DIRECTION_RIGHT:
			pPlayer->SetMove(D3DXVECTOR3(TUPARI_MOVE, 3.0f, 0.0f));
			pPlayer->SetState(CPlayer::STATE_NEUTRAL);
			break;
		}
		//ダメージ
		pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
	}
	//リセット
	if (pInputKeyboard->GetTrigger(DIK_R) == true ||
		pXInput->GetTrigger(XINPUT_GAMEPAD_START, 0) == true)
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

//=============================================================================
// バトル処理まとめ
//=============================================================================
void CBattleSys::Battle(int nPlayer, ATTACK_TYPE AttackType)
{

}
