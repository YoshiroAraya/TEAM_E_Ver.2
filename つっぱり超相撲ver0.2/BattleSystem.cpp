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
#include "tutorial.h"
#include "UITime.h"
#include "gauge.h"
#include "SansoGauge.h"
#include "UltimateGauge.h"
#include "Banimation.h"
#include "tutorial.h"

#include "pause.h"
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// マクロ
//=============================================================================
#define YORI_MOVE			(5.0f)
#define NAGE_MOVE			(60.0f)
#define OSI_MOVE			(20.0f)
#define COUNTER_MOVE		(20.0f)
#define TUPARI_MOVE			(10.0f)
#define TUPARI_MOVE_ULT		(5.0f)
#define KNOCKUP_MOVE		(3.0f)
#define KNOCKUP_MOVE_ULT	(2.0f)
#define GUARD_MOVE			(8.0f)

#define NAGE_DIVIDED		(2.0f)
#define NAGE_HINSI			(2.0f)
#define OSI_RECOIL			(1.0f)
#define TUPPARI_RECOIL		(0.5f)
#define HINSI_T_MOVE		(2.8f)
#define HINSI_N_MOVE		(1.4f)
#define HINSI_O_MOVE		(1.9f)

#define JANKEN_TUPARI_MOVE		(5.0f)
#define JANKEN_BUTI_MOVE		(5.0f)
#define JANKEN_KAWASI_MOVE		(30.1f)
#define J_TUPARI_KNOCKUP_MOVE	(15.0f)
#define J_BUTI_KNOCKUP_MOVE		(20.0f)
#define JANKEN_DAMAGE			(100.0f)


#define BATTLE_FLAME		(20)

#define YORI_FLAME			(20)
#define NAGE_FLAME			(15)
#define OSI_FLAME			(20)
#define TUPARI_FLAME		(2)
#define TUPARI_RECOVERY		(30)
#define COUNTER_FLAME		(40)

#define GU_COUNTER			(40)
#define CHOKI_COUNTER		(10)

#define DAMAGE				(30)
#define HEEL				(20)
#define START_SECOND		(2)			// 対戦が始まるまでの秒数
#define DOHYO_HAZI_NUM		(173.0f)

#define SANSO_NEUTRAL		(1.5f)		//組んでいないときの回復量
#define SANSO_KUMI			(0.3f)		//組んでいるときの回復量
#define SANSO_SIKAKE		(20)		//押したときの消費量の倍率

//=============================================================================
// バトルシステムクラスのコンストラクタ
//=============================================================================
CBattleSys::CBattleSys()
{
	// 値をクリア
	m_nCntAttackFlame = 0;
	m_nImpossibleFlame = 0;
	m_bAttack = false;
	m_aJanken[0] = JANKEN_GU_BUTI;
	m_aJanken[1] = JANKEN_GU_BUTI;
	m_nStartCounter = 0;
	m_nCntPushP1 = 0;
	m_nCntPushP2 = 0;
	m_nFlamePush = 0;
	m_AttackTurn = ATTACK_TURN_NORMAL;
	m_nUltTimer = 0;

	for (int nCntPlayer = 0; nCntPlayer < MAX_CHARACTER; nCntPlayer++)
	{
		m_aGUCounter[nCntPlayer] = 0;
		m_aCHOKICounter[nCntPlayer] = 0;
		m_abPA[nCntPlayer] = false;
		m_fMoveDying[nCntPlayer] = { 0.0f };	//瀕死時の移動量
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
	m_nCntAttackFlame = 0;
	m_nImpossibleFlame = 0;
	m_bAttack = false;
	m_aJanken[0] = JANKEN_GU_BUTI;
	m_aJanken[1] = JANKEN_GU_BUTI;
	m_nStartCounter = 0;
	m_nCntPushP1 = 0;
	m_nCntPushP2 = 0;
	m_nFlamePush = 0;
	m_AttackTurn = ATTACK_TURN_NORMAL;
	m_nUltTimer = 0;
	m_bPlayerUlt = false;
	m_bEnemyUlt = false;
	m_abUlt[0] = false;
	m_abUlt[1] = false;

	for (int nCntPlayer = 0; nCntPlayer < MAX_CHARACTER; nCntPlayer++)
	{
		m_aGUCounter[nCntPlayer] = 0;
		m_aCHOKICounter[nCntPlayer] = 0;
		m_abPA[nCntPlayer] = false;
		m_fMoveDying[nCntPlayer] = { 1.0f };
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

	CPlayer *pPlayer = NULL;
	CEnemy *pEnemy = NULL;

	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{	//
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();
	}
	else if (mode == CManager::MODE_GAME)
	{	//
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();
	}

	//キーボード/コントローラー操作
	Operation();
	//カウンター処理
	//CounterAttack();
	//連打回数判定
	PushJudge();

	if (m_bAttack == true)
	{
		m_nCntAttackFlame--;
		if (m_nCntAttackFlame <= 0)
		{
			if (pPlayer != NULL)
			{
				//状態を通常状態へ
				if (pPlayer->GetState() == CPlayer::STATE_TSUPPARI || pPlayer->GetState() == CPlayer::STATE_NAGE)
				{
					pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				}
			}

			if (pEnemy != NULL)
			{
				if (pEnemy->GetState() == CEnemy::STATE_TSUPPARI || pEnemy->GetState() == CPlayer::STATE_NAGE)
				{
					pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				}
			}
			m_nCntAttackFlame = 0;
			m_bAttack = false;
		}
	}

#ifdef _DEBUG
	if (pPlayer != NULL && pEnemy != NULL)
	{
		if (pPlayer->GetState() == CPlayer::STATE_NEUTRAL
			&& pEnemy->GetState() == CEnemy::STATE_NEUTRAL
			&& m_bAttack == false)
		{
			CDebugProc::Print("c", " 通常 ");
		}
	}
	CDebugProc::Print("cn", " 行動不可フレーム ", m_nCntAttackFlame);

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
	CPlayer *pPlayer = NULL;
	// エネミーの取得
	CEnemy *pEnemy = NULL;
	bool bHit = false;
	// ゲージの取得
	CGauge *pGauge = NULL;
	CUltimateGauge *pULTGauge = NULL;
	CGame::STATE GState = CGame::STATE_GAME;
	CTutorial::STATE TState = CTutorial::STATE_GAME;

	// モード取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{	// プレイヤーの取得
		pPlayer = CTutorial::GetPlayer();
		// エネミーの取得
		pEnemy = CTutorial::GetEnemy();
		//ゲージの取得
		pGauge = CTutorial::GetGauge();
		pULTGauge = CTutorial::GetUltimateGauge();
		TState = CTutorial::GetState();
		bHit = CTutorial::GetHit();
	}
	else if (mode == CManager::MODE_GAME)
	{	// プレイヤーの取得
		pPlayer = CGame::GetPlayer();
		// エネミーの取得
		pEnemy = CGame::GetEnemy();
		//ゲージの取得
		pGauge = CGame::GetGauge();
		pULTGauge = CGame::GetUltimateGauge();
		GState = CGame::GetState();
		bHit = CGame::GetHit();
	}

	//サウンド情報の取得
	CSound *pSound = CManager::GetSound(0);

	D3DXVECTOR3 p1pos, p2pos;

	if (pPlayer != NULL)
	{	//プレイヤー1の位置を取得
		p1pos = pPlayer->GetPosition();
	}
	if (pPlayer != NULL)
	{	//プレイヤー2の位置を取得
		p2pos = pEnemy->GetPosition();
	}

	if (pPlayer != NULL && pEnemy != NULL)
	{
		if (GState == CGame::STATE_GAME
			|| TState == CTutorial::STATE_GAME)
		{
			if (pPlayer->GetState() == CPlayer::STATE_JANKEN
				&& pPlayer->GetbJanken() == true)
			{
#ifdef _DEBUG
				CDebugProc::Print("c", " eじゃんけん ");
#endif
				if (pInputKeyboard->GetPress(DIK_Z) == true ||
					pXInput->GetPress(XPLAYER_B_BUTTON, 0) == true)
				{
					m_aJanken[0] = JANKEN_GU_BUTI;
					pPlayer->SetState(CPlayer::STATE_NOKOTTA);
					pPlayer->SetMotionType(0, CPlayer::MOTION_SYAGAMI);
					pPlayer->SetbMotionEnd(0, true);
					pPlayer->SetMotionType(1, CPlayer::MOTION_SYAGAMI);
					pPlayer->SetbMotionEnd(1, true);
				}
				else if (pInputKeyboard->GetPress(DIK_X) == true ||
					pXInput->GetPress(XPLAYER_Y_BUTTON, 0) == true)
				{
					m_aJanken[0] = JANKEN_CHOKI_TUPPA;
					pPlayer->SetState(CPlayer::STATE_NOKOTTA);
					pPlayer->SetMotionType(0, CPlayer::MOTION_SYAGAMI);
					pPlayer->SetbMotionEnd(0, true);
					pPlayer->SetMotionType(1, CPlayer::MOTION_SYAGAMI);
					pPlayer->SetbMotionEnd(1, true);
				}
				else if (pInputKeyboard->GetPress(DIK_C) == true ||
					pXInput->GetPress(XPLAYER_X_BUTTON, 0) == true)
				{
					m_aJanken[0] = JANKEN_PA_KAWASI;
					pPlayer->SetState(CPlayer::STATE_NOKOTTA);
					pPlayer->SetMotionType(0, CPlayer::MOTION_SYAGAMI);
					pPlayer->SetbMotionEnd(0, true);
					pPlayer->SetMotionType(1, CPlayer::MOTION_SYAGAMI);
					pPlayer->SetbMotionEnd(1, true);
					m_abPA[0] = true;
				}
			}

			if (pEnemy->GetState() == CEnemy::STATE_JANKEN
				&& pEnemy->GetbJanken() == true)
			{
#ifdef _DEBUG
				CDebugProc::Print("c", " eじゃんけん ");
#endif
				//CPUと人間のじゃんけん操作
				if (pEnemy->GetMode() == CEnemy::MODE_CPU)
				{	//じゃんけんをランダムに決定
					int nRandJanken = rand() % 3;

					switch (nRandJanken)
					{
					case 0:
						m_aJanken[1] = JANKEN_GU_BUTI;
						pEnemy->SetState(CEnemy::STATE_NOKOTTA);
						pEnemy->SetMotionType(0, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(1, true);
						break;
					case 1:
						m_aJanken[1] = JANKEN_CHOKI_TUPPA;
						pEnemy->SetState(CEnemy::STATE_NOKOTTA);
						pEnemy->SetMotionType(0, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(1, true);
						break;
					case 2:
						m_aJanken[1] = JANKEN_PA_KAWASI;
						pEnemy->SetState(CEnemy::STATE_NOKOTTA);
						m_abPA[1] = true;
						pEnemy->SetMotionType(0, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(1, true);
						break;
					}

				}
				else
				{	//人間の操作
					if (pInputKeyboard->GetPress(DIK_B) == true ||
						pXInput->GetPress(XENEMY_B_BUTTON, 1) == true)
					{
						m_aJanken[1] = JANKEN_GU_BUTI;
						pEnemy->SetState(CEnemy::STATE_NOKOTTA);
						pEnemy->SetMotionType(0, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(1, true);
					}
					else if (pInputKeyboard->GetPress(DIK_N) == true ||
						pXInput->GetPress(XENEMY_Y_BUTTON, 1) == true)
					{
						m_aJanken[1] = JANKEN_CHOKI_TUPPA;
						pEnemy->SetState(CEnemy::STATE_NOKOTTA);
						pEnemy->SetMotionType(0, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(1, true);
					}
					else if (pInputKeyboard->GetPress(DIK_M) == true ||
						pXInput->GetPress(XENEMY_X_BUTTON, 1) == true)
					{
						m_aJanken[1] = JANKEN_PA_KAWASI;
						pEnemy->SetState(CEnemy::STATE_NOKOTTA);
						m_abPA[1] = true;
						pEnemy->SetMotionType(0, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_SYAGAMI);
						pEnemy->SetbMotionEnd(1, true);
					}
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
				if (m_aJanken[0] == JANKEN_GU_BUTI && m_aJanken[1] == JANKEN_GU_BUTI)
				{// グーとグー
					{
						pPlayer->SetMove(D3DXVECTOR3(JANKEN_BUTI_MOVE, 0.0f, 0.0f));
						pEnemy->SetMove(D3DXVECTOR3(-JANKEN_BUTI_MOVE, 0.0f, 0.0f));
					}
				}
				else if (m_aJanken[0] == JANKEN_GU_BUTI && m_aJanken[1] == JANKEN_CHOKI_TUPPA)
				{// グーとチョキ
					m_aGUCounter[0]++;
					if (m_aGUCounter[0] < GU_COUNTER)
					{
						pPlayer->SetMove(D3DXVECTOR3(JANKEN_BUTI_MOVE, 0.0f, 0.0f));
					}
					else if (m_aGUCounter[0] >= GU_COUNTER)
					{
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						m_aGUCounter[0] = 0;
					}
					m_aCHOKICounter[1]++;
					if (m_aCHOKICounter[1] < CHOKI_COUNTER)
					{
						pEnemy->SetMove(D3DXVECTOR3(-JANKEN_TUPARI_MOVE, 0.0f, 0.0f));
					}
					if (bHit == true)
					{
						pEnemy->SetMove(D3DXVECTOR3(J_BUTI_KNOCKUP_MOVE * 3, 5.0f, 0.0f));
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						if (mode == CManager::MODE_TUTORIAL)
						{
							CTutorial::SetHit(false);
						}
						else if (mode == CManager::MODE_GAME)
						{
							CGame::SetHit(false);
						}
						//ライフゲージ消費
						pGauge->SetGaugeRightLeft(-JANKEN_DAMAGE, 0.0f );
						pULTGauge->SetGaugeRightLeft(0.0f, 30.0f);
					}

				}
				else if (m_aJanken[0] == JANKEN_GU_BUTI && m_aJanken[1] == JANKEN_PA_KAWASI)
				{// グーとパー
					m_aGUCounter[0]++;

					if (m_aGUCounter[0] < GU_COUNTER)
					{
						pPlayer->SetMove(D3DXVECTOR3(JANKEN_BUTI_MOVE, 0.0f, 0.0f));
					}
					else if (m_aGUCounter[0] >= GU_COUNTER)
					{
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						m_aGUCounter[0] = 0;
					}

					if (m_abPA[1] == true)
					{
						pEnemy->SetMove(D3DXVECTOR3(-JANKEN_KAWASI_MOVE, 10.0f, 0.0f));
						m_abPA[1] = false;
						//ライフゲージ消費
						pGauge->SetGaugeRightLeft(0.0f, -JANKEN_DAMAGE );
						pULTGauge->SetGaugeRightLeft(30.0f, 0.0f);
					}
				}
				else if (m_aJanken[0] == JANKEN_CHOKI_TUPPA && m_aJanken[1] == JANKEN_GU_BUTI)
				{// チョキとグー
					if (bHit == true)
					{
						pPlayer->SetMove(D3DXVECTOR3(-J_BUTI_KNOCKUP_MOVE * 3, 5.0f, 0.0f));
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						if (mode == CManager::MODE_TUTORIAL)
						{
							CTutorial::SetHit(false);
						}
						else if (mode == CManager::MODE_GAME)
						{
							CGame::SetHit(false);
						}						//ライフゲージ消費
						pGauge->SetGaugeRightLeft(0.0f, -JANKEN_DAMAGE);
						pULTGauge->SetGaugeRightLeft(30.0f, 0.0f);
					}

					m_aCHOKICounter[0]++;
					if (m_aCHOKICounter[0] < CHOKI_COUNTER)
					{
						pPlayer->SetMove(D3DXVECTOR3(JANKEN_TUPARI_MOVE, 0.0f, 0.0f));
					}
					m_aGUCounter[1]++;

					if (m_aGUCounter[1] < GU_COUNTER)
					{
						pEnemy->SetMove(D3DXVECTOR3(-JANKEN_BUTI_MOVE, 0.0f, 0.0f));
					}

				}
				else if (m_aJanken[0] == JANKEN_CHOKI_TUPPA && m_aJanken[1] == JANKEN_CHOKI_TUPPA)
				{// チョキとチョキ
					m_aCHOKICounter[0]++;

					if (m_aCHOKICounter[0] < CHOKI_COUNTER)
					{
						pEnemy->SetMove(D3DXVECTOR3(-JANKEN_TUPARI_MOVE, 0.0f, 0.0f));
						pPlayer->SetMove(D3DXVECTOR3(JANKEN_TUPARI_MOVE, 0.0f, 0.0f));
					}

					if (m_aCHOKICounter[0] > CHOKI_COUNTER && CHOKI_COUNTER + 5 >= m_aCHOKICounter[0])
					{
						pEnemy->SetMove(D3DXVECTOR3(J_TUPARI_KNOCKUP_MOVE, 3.0f, 0.0f));
					}
					else if (m_aCHOKICounter[0] > CHOKI_COUNTER + 5)
					{
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						m_aCHOKICounter[0] = 0;
					}

					m_aCHOKICounter[1]++;

					if (m_aCHOKICounter[1] > CHOKI_COUNTER && CHOKI_COUNTER + 5 >= m_aCHOKICounter[1])
					{
						pPlayer->SetMove(D3DXVECTOR3(-J_TUPARI_KNOCKUP_MOVE, 3.0f, 0.0f));
					}
					else if (m_aCHOKICounter[1] > CHOKI_COUNTER + 5)
					{
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						m_aCHOKICounter[1] = 0;
					}
				}
				else if (m_aJanken[0] == JANKEN_CHOKI_TUPPA && m_aJanken[1] == JANKEN_PA_KAWASI)
				{// チョキとパー
					m_aCHOKICounter[0]++;

					if (m_aCHOKICounter[0] < CHOKI_COUNTER)
					{
						pPlayer->SetMove(D3DXVECTOR3(JANKEN_TUPARI_MOVE, 0.0f, 0.0f));
					}
					if (m_aCHOKICounter[0] > CHOKI_COUNTER && CHOKI_COUNTER + 5 >= m_aCHOKICounter[0])
					{
						pEnemy->SetMove(D3DXVECTOR3(J_TUPARI_KNOCKUP_MOVE, 3.0f, 0.0f));
					}
					else if (m_aCHOKICounter[0] > CHOKI_COUNTER + 5)
					{
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						m_aCHOKICounter[0] = 0;
						//ライフゲージ消費
						pGauge->SetGaugeRightLeft(-JANKEN_DAMAGE, 0.0f);
						pULTGauge->SetGaugeRightLeft(0.0f, 30.0f);
					}

					if (m_abPA[1] == true)
					{
						pEnemy->SetMove(D3DXVECTOR3(-JANKEN_KAWASI_MOVE, 10.0f, 0.0f));
						m_abPA[1] = false;
					}

				}
				else if (m_aJanken[0] == JANKEN_PA_KAWASI && m_aJanken[1] == JANKEN_GU_BUTI)
				{// パーとグー
					if (m_abPA[0] == true)
					{
						pPlayer->SetMove(D3DXVECTOR3(JANKEN_KAWASI_MOVE, 10.0f, 0.0f));
						m_abPA[0] = false;
						//ライフゲージ消費
						pGauge->SetGaugeRightLeft(-JANKEN_DAMAGE, 0.0f);
						pULTGauge->SetGaugeRightLeft(0.0f, 30.0f);
					}

					m_aGUCounter[1]++;

					if (m_aGUCounter[1] < GU_COUNTER)
					{
						pEnemy->SetMove(D3DXVECTOR3(-JANKEN_BUTI_MOVE, 0.0f, 0.0f));
					}
					else if (m_aGUCounter[1] >= GU_COUNTER)
					{
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						m_aGUCounter[1] = 0;
					}


				}
				else if (m_aJanken[0] == JANKEN_PA_KAWASI && m_aJanken[1] == JANKEN_CHOKI_TUPPA)
				{// パーとチョキ
					if (m_abPA[0] == true)
					{
						pPlayer->SetMove(D3DXVECTOR3(JANKEN_KAWASI_MOVE, 10.0f, 0.0f));
						m_abPA[0] = false;
					}

					m_aCHOKICounter[1]++;
					if (m_aCHOKICounter[1] < CHOKI_COUNTER)
					{
						pEnemy->SetMove(D3DXVECTOR3(-JANKEN_TUPARI_MOVE, 0.0f, 0.0f));
					}

					if (m_aCHOKICounter[1] > CHOKI_COUNTER && CHOKI_COUNTER + 5 >= m_aCHOKICounter[1])
					{
						pPlayer->SetMove(D3DXVECTOR3(-J_TUPARI_KNOCKUP_MOVE, 3.0f, 0.0f));
					}
					else if (m_aCHOKICounter[1] > CHOKI_COUNTER + 5)
					{
						pEnemy->SetState(CEnemy::STATE_NEUTRAL);
						pPlayer->SetState(CPlayer::STATE_NEUTRAL);
						m_aCHOKICounter[1] = 0;
						//ライフゲージ消費
						pGauge->SetGaugeRightLeft(0.0f, -JANKEN_DAMAGE );
						pULTGauge->SetGaugeRightLeft(30.0f, 0.0f);
					}
				}
				else if (m_aJanken[0] == JANKEN_PA_KAWASI && m_aJanken[1] == JANKEN_PA_KAWASI)
				{// パーとパー
					if (m_abPA[0] == true)
					{
						pPlayer->SetMove(D3DXVECTOR3(JANKEN_KAWASI_MOVE, 10.0f, 0.0f));
						m_abPA[0] = false;
					//	pPlayer->SetState(CPlayer::STATE_KUMI);
					}

					if (m_abPA[1] == true)
					{
						pEnemy->SetMove(D3DXVECTOR3(-JANKEN_KAWASI_MOVE, 10.0f, 0.0f));
						m_abPA[1] = false;
					//	pEnemy->SetState(CEnemy::STATE_KUMI);
					}
				}

				//モーションの変化
				if (m_aJanken[0] == JANKEN_GU_BUTI && pPlayer->GetMotionType(0) != CPlayer::MOTION_BUTIKAMASI)
				{
					pPlayer->SetMotionType(0, CPlayer::MOTION_BUTIKAMASI);
					pPlayer->SetbMotionEnd(0, true);
					pPlayer->SetMotionType(1, CPlayer::MOTION_BUTIKAMASI);
					pPlayer->SetbMotionEnd(1, true);
				}
				else if (m_aJanken[0] == JANKEN_CHOKI_TUPPA && pPlayer->GetMotionType(0) != CPlayer::MOTION_TSUPPARI)
				{
					if (pPlayer->GetMotionType(1) != CPlayer::MOTION_TSUPPARI)
					{
						pPlayer->SetMotionType(0, CPlayer::MOTION_TSUPPARI);
						pPlayer->SetbMotionEnd(0, true);
						pPlayer->SetMotionType(1, CPlayer::MOTION_TSUPPARI);
						pPlayer->SetbMotionEnd(1, true);
					}
				}
				else if (m_aJanken[0] == JANKEN_PA_KAWASI && pPlayer->GetMotionType(0) != CPlayer::MOTION_KAWASI)
				{
					pPlayer->SetMotionType(0, CPlayer::MOTION_KAWASI);
					pPlayer->SetbMotionEnd(0, true);
					pPlayer->SetMotionType(1, CPlayer::MOTION_KAWASI);
					pPlayer->SetbMotionEnd(1, true);
				}

				if (m_aJanken[1] == JANKEN_GU_BUTI && pEnemy->GetMotionType(0) != CEnemy::MOTION_BUTIKAMASI)
				{
					pEnemy->SetMotionType(0, CEnemy::MOTION_BUTIKAMASI);
					pEnemy->SetbMotionEnd(0, true);
					pEnemy->SetMotionType(1, CEnemy::MOTION_BUTIKAMASI);
					pEnemy->SetbMotionEnd(1, true);
				}
				else if (m_aJanken[1] == JANKEN_CHOKI_TUPPA && pEnemy->GetMotionType(0) != CEnemy::MOTION_TSUPPARI)
				{
					if (pEnemy->GetMotionType(1) != CEnemy::MOTION_TSUPPARI)
					{
						pEnemy->SetMotionType(0, CEnemy::MOTION_TSUPPARI);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_TSUPPARI);
						pEnemy->SetbMotionEnd(1, true);
					}
				}
				else if (m_aJanken[1] == JANKEN_PA_KAWASI && pEnemy->GetMotionType(0) != CEnemy::MOTION_KAWASI)
				{
					pEnemy->SetMotionType(0, CEnemy::MOTION_KAWASI);
					pEnemy->SetbMotionEnd(0, true);
					pEnemy->SetMotionType(1, CEnemy::MOTION_KAWASI);
					pEnemy->SetbMotionEnd(1, true);
				}
			}
		}

		//瀕死時
		if (pPlayer->GetDying() == true)
		{
			m_fMoveDying[0] = HINSI_T_MOVE;
		}
		else if (pPlayer->GetDying() == false)
		{
			m_fMoveDying[0] = 1.0f;
		}

		if (pEnemy->GetDying() == true)
		{
			m_fMoveDying[1] = HINSI_T_MOVE;
		}
		else if (pEnemy->GetDying() == false)
		{
			m_fMoveDying[1] = 1.0f;
		}

		//重ならないように
		if (pPlayer->GetState() == CPlayer::STATE_KUMI
			&& pEnemy->GetState() == CEnemy::STATE_KUMI)
		{
			//お互いの中心を見る -10 +10間は0  -150 0間は-75  -50 +150間は 50
			float DiffDis = (p1pos.x + p2pos.x) / 2;

			if (pPlayer->GetDirection() == CPlayer::DIRECTION_RIGHT
				&& pEnemy->GetDirection() == CEnemy::DIRECTION_LEFT)
			{
				//POSを代入(固定)
				pPlayer->SetPosition(D3DXVECTOR3(DiffDis - 18, p1pos.y, p1pos.z));
				pEnemy->SetPosition(D3DXVECTOR3(DiffDis + 18, p2pos.y, p2pos.z));
			}
			else if (pPlayer->GetDirection() == CPlayer::DIRECTION_LEFT
				&& pEnemy->GetDirection() == CEnemy::DIRECTION_RIGHT)
			{
				//POSを代入(固定)
				pPlayer->SetPosition(D3DXVECTOR3(DiffDis + 18, p1pos.y, p1pos.z));
				pEnemy->SetPosition(D3DXVECTOR3(DiffDis - 18, p2pos.y, p2pos.z));
			}
		}


		//2pの方が強い(処理が後に入る) 修正
		if (pPlayer->GetState() == CPlayer::STATE_KUMI
			&& pEnemy->GetState() == CEnemy::STATE_KUMI
			&& m_bAttack == false)
		{
			CDebugProc::Print("c", " 組み合い ");

			if (m_AttackTurn == ATTACK_TURN_PLAYER1)
			{
				P1Attack();
				//m_AttackTurn = ATTACK_TURN_NORMAL;
			}
			else if (m_AttackTurn == ATTACK_TURN_PLAYER2)
			{
				P2Attack();
				//m_AttackTurn = ATTACK_TURN_NORMAL;
			}
		}

		//つっぱり
		if (pPlayer->GetState() == CPlayer::STATE_NEUTRAL && m_bAttack == false)
		{
			if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true && pPlayer->GetRecovery() == false ||
				pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true && pPlayer->GetRecovery() == false)
			{
				pPlayer->SetMotionType(0, CPlayer::MOTION_TSUPPARI);
				pPlayer->SetbMotionEnd(0, true);
				pPlayer->SetMotionType(1, CPlayer::MOTION_TSUPPARI);
				pPlayer->SetbMotionEnd(1, true);
				pPlayer->SetbDash(false);
				//向いてる方向 プレイヤー
				switch (pPlayer->GetDirection())
				{
				case CPlayer::DIRECTION_LEFT:
					pPlayer->SetState(CPlayer::STATE_TSUPPARI);
					pPlayer->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x - 10, p1pos.y, p1pos.z));
					m_nCntAttackFlame = TUPARI_FLAME;
					pPlayer->SetRecovery(true);
					pPlayer->SetRecoveryTime(TUPARI_RECOVERY);
					m_bAttack = true;
					break;
				case CPlayer::DIRECTION_RIGHT:
					pPlayer->SetState(CPlayer::STATE_TSUPPARI);
					pPlayer->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x + 10, p1pos.y, p1pos.z));
					m_nCntAttackFlame = TUPARI_FLAME;
					pPlayer->SetRecovery(true);
					pPlayer->SetRecoveryTime(TUPARI_RECOVERY);
					m_bAttack = true;
					break;
				}
			}
		}
		//エネミーのつっぱり
		if (pEnemy->GetState() == CEnemy::STATE_NEUTRAL && m_bAttack == false && pEnemy->GetMode() == CEnemy::MODE_P2)
		{
			if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true && pEnemy->GetRecovery() == false ||
				pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true && pEnemy->GetRecovery() == false)
			{
				pEnemy->SetMotionType(0, CEnemy::MOTION_TSUPPARI);
				pEnemy->SetbMotionEnd(0, true);
				pEnemy->SetMotionType(1, CEnemy::MOTION_TSUPPARI);
				pEnemy->SetbMotionEnd(1, true);
				pEnemy->SetbDash(false);
				//向いてる方向 エネミー
				switch (pEnemy->GetDirection())
				{
				case CEnemy::DIRECTION_LEFT:
					pEnemy->SetState(CEnemy::STATE_TSUPPARI);
					pEnemy->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x - 10, p1pos.y, p1pos.z));
					m_nCntAttackFlame = TUPARI_FLAME;
					pEnemy->SetRecovery(true);
					pEnemy->SetRecoveryTime(TUPARI_RECOVERY);
					m_bAttack = true;
					break;
				case CEnemy::DIRECTION_RIGHT:
					pEnemy->SetState(CEnemy::STATE_TSUPPARI);
					pEnemy->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x + 10, p1pos.y, p1pos.z));
					m_nCntAttackFlame = TUPARI_FLAME;
					pEnemy->SetRecovery(true);
					pEnemy->SetRecoveryTime(TUPARI_RECOVERY);
					m_bAttack = true;
					break;
				}
			}
		}

		if (pEnemy->GetUltDamage() == false)
		{// 通常時
			//ダメージなら吹っ飛ぶ プレイヤーのつっぱり
			if (pEnemy->GetState() == CEnemy::STATE_DAMAGE)
			{
				switch (pPlayer->GetDirection())
				{
				case CPlayer::DIRECTION_LEFT:
					pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
					Battle(0, ATTACK_TYPE_TUPPARI, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3((-TUPARI_MOVE * m_fMoveDying[1]), KNOCKUP_MOVE, 0.0f));
					break;
				case CPlayer::DIRECTION_RIGHT:
					pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
					Battle(0, ATTACK_TYPE_TUPPARI, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3((TUPARI_MOVE * m_fMoveDying[1]), KNOCKUP_MOVE, 0.0f));
					break;
				}
				//状態変化
				pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				//ダメージ
				pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
				pULTGauge->SetGaugeRightLeft(0.0f, 30.0f);
			}
		}
		else
		{// 突っ張りの奥義を受けているとき
			//ダメージなら吹っ飛ぶ プレイヤーのつっぱり
			if (pEnemy->GetState() == CEnemy::STATE_DAMAGE)
			{
				//状態変化
				pEnemy->SetState(CEnemy::STATE_NEUTRAL);
				//ダメージ
				pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
				pULTGauge->SetGaugeRightLeft(0.0f, 30.0f);

				if (pEnemy->GetDying() == true)
				{
					m_abUlt[0] = false;
					m_bPlayerUlt = false;
					switch (pPlayer->GetDirection())
					{
					case CPlayer::DIRECTION_LEFT:
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_TUPPARI, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(((-TUPARI_MOVE * 17.0f)), KNOCKUP_MOVE_ULT * 2.0f, 0.0f));
						break;
					case CPlayer::DIRECTION_RIGHT:
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_TUPPARI, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(((TUPARI_MOVE * 17.0f)), KNOCKUP_MOVE_ULT * 2.0f, 0.0f));
						break;
					}
				}
				else
				{
					switch (pPlayer->GetDirection())
					{
					case CPlayer::DIRECTION_LEFT:
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_TUPPARI, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3((-TUPARI_MOVE * m_fMoveDying[1]), KNOCKUP_MOVE_ULT, 0.0f));
						break;
					case CPlayer::DIRECTION_RIGHT:
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_TUPPARI, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3((TUPARI_MOVE * m_fMoveDying[1]), KNOCKUP_MOVE_ULT, 0.0f));
						break;
					}
				}
			}
			if (pEnemy->GetWallHit() == true)
			{
				pEnemy->SetUltDamage(false);
			}
		}
		
		if (pPlayer->GetUltDamage() == false)
		{
			//ダメージなら吹っ飛ぶ エネミーのつっぱり
			if (pPlayer->GetState() == CPlayer::STATE_DAMAGE)
			{
				switch (pEnemy->GetDirection())
				{
				case CEnemy::DIRECTION_LEFT:
					pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
					Battle(1, ATTACK_TYPE_TUPPARI, D3DXVECTOR3((-TUPARI_MOVE * m_fMoveDying[0]), KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					break;
				case CEnemy::DIRECTION_RIGHT:
					pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
					Battle(1, ATTACK_TYPE_TUPPARI, D3DXVECTOR3((TUPARI_MOVE * m_fMoveDying[0]), KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					break;
				}
				//状態変化
				if (pPlayer != NULL)
				{
					pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				}
				//ダメージ
				pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
				pULTGauge->SetGaugeRightLeft(0.0f, 30.0f);
			}
		}
		else
		{// 突っ張りの奥義を受けているとき
		 //ダメージなら吹っ飛ぶ プレイヤーのつっぱり
			if (pPlayer->GetState() == CPlayer::STATE_DAMAGE)
			{
				//状態変化
				pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				//ダメージ
				pGauge->SetGaugeRightLeft(HEEL, -DAMAGE);
				pULTGauge->SetGaugeRightLeft(30.0f, 0.0f);

				if (pPlayer->GetDying() == true)
				{
					m_abUlt[1] = false;
					m_bEnemyUlt = false;
					switch (pEnemy->GetDirection())
					{
					case CEnemy::DIRECTION_LEFT:
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_TUPPARI, D3DXVECTOR3(((-TUPARI_MOVE * 17.0f)), KNOCKUP_MOVE_ULT * 2.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
						break;
					case CEnemy::DIRECTION_RIGHT:
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_TUPPARI, D3DXVECTOR3(((TUPARI_MOVE * 17.0f)), KNOCKUP_MOVE_ULT * 2.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
						break;
					}
				}
				else
				{
					switch (pEnemy->GetDirection())
					{
					case CEnemy::DIRECTION_LEFT:
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_TUPPARI, D3DXVECTOR3((-TUPARI_MOVE * m_fMoveDying[0]), KNOCKUP_MOVE_ULT, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
						break;
					case CEnemy::DIRECTION_RIGHT:
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_TUPPARI, D3DXVECTOR3((TUPARI_MOVE * m_fMoveDying[0]), KNOCKUP_MOVE_ULT, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
						break;
					}
				}
			}
			if (pPlayer->GetWallHit() == true)
			{
				pPlayer->SetUltDamage(false);
			}
		}

		//もう一度取得
		if (pPlayer != NULL)
		{	//プレイヤー1の位置を取得
			p1pos = pPlayer->GetPosition();
		}
		if (pEnemy != NULL)
		{	//プレイヤー2の位置を取得
			p2pos = pEnemy->GetPosition();
		}

		if (pPlayer != NULL)
		{
			//土俵端で体力があ//つっぱりるなら残る
			if (/*pPlayer->GetDohyo() == CPlayer::DOHYO_HAZI && */pPlayer->GetDying() == false)
			{
				if (pPlayer->GetPosition().x > DOHYO_HAZI_NUM)
				{
					pPlayer->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					pPlayer->SetPosition(D3DXVECTOR3(DOHYO_HAZI_NUM, p1pos.y, p1pos.z));
				}
				else if (pPlayer->GetPosition().x < -DOHYO_HAZI_NUM)
				{
					pPlayer->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					pPlayer->SetPosition(D3DXVECTOR3(-DOHYO_HAZI_NUM, p1pos.y, p1pos.z));
				}
			}
		}

		if (pEnemy != NULL)
		{
			if (pEnemy->GetDying() == false)
			{
				if (pEnemy->GetPosition().x > DOHYO_HAZI_NUM)
				{
					pEnemy->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					pEnemy->SetPosition(D3DXVECTOR3(DOHYO_HAZI_NUM, p2pos.y, p2pos.z));
				}
				else if (pEnemy->GetPosition().x < -DOHYO_HAZI_NUM)
				{
					pEnemy->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					pEnemy->SetPosition(D3DXVECTOR3(-DOHYO_HAZI_NUM, p2pos.y, p2pos.z));
				}
			}
		}
		//リセット
		if (pInputKeyboard->GetTrigger(DIK_R) == true ||
			pXInput->GetTrigger(XINPUT_GAMEPAD_START, 0) == true)
		{
			pPlayer->InitStatus();
			pEnemy->InitStatus();
			pPlayer->SetPosition(D3DXVECTOR3(-80.0f, 20.0f, 0.0f));
			pEnemy->SetPosition(D3DXVECTOR3(80.0f, 20.0f, 0.0f));
			pPlayer->SetState(CPlayer::STATE_JANKEN);
			pEnemy->SetState(CEnemy::STATE_JANKEN);
			m_bAttack = false;
			m_nCntAttackFlame = 0;
			m_nStartCounter = 0;
			pPlayer->SetMotionType(0, CPlayer::MOTION_NEUTRAL);
			pPlayer->SetMotionType(1, CPlayer::MOTION_NEUTRAL);
			pEnemy->SetMotionType(0, CEnemy::MOTION_NEUTRAL);
			pEnemy->SetMotionType(1, CEnemy::MOTION_NEUTRAL);
			m_aJanken[0] = JANKEN_GU_BUTI;
			m_aJanken[1] = JANKEN_GU_BUTI;

			for (int nCntPlayer = 0; nCntPlayer < MAX_CHARACTER; nCntPlayer++)
			{
				m_aGUCounter[nCntPlayer] = 0;
				m_aCHOKICounter[nCntPlayer] = 0;
				m_abPA[nCntPlayer] = false;
			}

			if (mode == CManager::MODE_TUTORIAL)
			{
				CTutorial::SetWinner(CTutorial::WINNER_NONE);
				CTutorial::SetHit(true);
				CManager::GetTutorial()->SetbUI(true);
			}
			else if (mode == CManager::MODE_GAME)
			{
				CGame::SetWinner(CGame::WINNER_NONE);
				CGame::SetHit(true);
				CManager::GetGame()->SetbUI(true);
			}
		}

		if (pPlayer->GetUltDis() == true && pInputKeyboard->GetTrigger(DIK_5) == true)
		{
			m_bPlayerUlt = true;

			pULTGauge->SetGaugeRightLeft(pULTGauge->GetGaugeRight(), -600.0f);
		}
		if (pEnemy->GetUltDis() == true && pInputKeyboard->GetTrigger(DIK_6) == true)
		{
			m_bEnemyUlt = true;

			pULTGauge->SetGaugeRightLeft(0.0f, pULTGauge->GetGaugeLeft());
		}

		if (m_bPlayerUlt == true)
		{// 奥義（プレイヤー）
			if ((float)m_nUltTimer / 60.0f >= 1.5f)
			{// ある程度の秒数がたったら
				m_abUlt[0] = true;

				// 突っ張る
				pPlayer->SetState(CPlayer::STATE_TSUPPARI);
				pPlayer->SetMotionType(0, CPlayer::MOTION_TSUPPARI);
				pPlayer->SetbMotionEnd(0, true);
				pPlayer->SetMotionType(1, CPlayer::MOTION_TSUPPARI);
				pPlayer->SetbMotionEnd(1, true);
				pPlayer->SetbDash(false);
				//向いてる方向 プレイヤー
				switch (pPlayer->GetDirection())
				{
				case CPlayer::DIRECTION_LEFT:
					pPlayer->SetState(CPlayer::STATE_TSUPPARI);
					pPlayer->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x - 10, p1pos.y, p1pos.z));
					m_nCntAttackFlame = TUPARI_FLAME;
					pPlayer->SetRecovery(true);
					pPlayer->SetRecoveryTime(TUPARI_RECOVERY);
					m_bAttack = true;
					break;
				case CPlayer::DIRECTION_RIGHT:
					pPlayer->SetState(CPlayer::STATE_TSUPPARI);
					pPlayer->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x + 10, p1pos.y, p1pos.z));
					m_nCntAttackFlame = TUPARI_FLAME;
					pPlayer->SetRecovery(true);
					pPlayer->SetRecoveryTime(TUPARI_RECOVERY);
					m_bAttack = true;
					break;
				}

				m_nUltTimer = 70;
			}
			else
			{
				m_nUltTimer++;

			}

			// プレイヤーを奥義状態にする
			pPlayer->SetState(CPlayer::STATE_ULT);

			CBAnimation *pAnimation = pPlayer->GetAnimation();

			if (pAnimation != NULL)
			{// オーラを大きくする
				pAnimation->SetBillboard(pPlayer->GetPosition(), 150.0f, 100.0f);
			}
		}
		if (m_bEnemyUlt == true)
		{// 奥義（エネミー）
			if ((float)m_nUltTimer / 60.0f >= 1.5f)
			{// ある程度の秒数がたったら
				m_abUlt[1] = true;

				// 突っ張る
				pEnemy->SetState(CEnemy::STATE_TSUPPARI);
				pEnemy->SetMotionType(0, CEnemy::MOTION_TSUPPARI);
				pEnemy->SetbMotionEnd(0, true);
				pEnemy->SetMotionType(1, CEnemy::MOTION_TSUPPARI);
				pEnemy->SetbMotionEnd(1, true);
				pEnemy->SetbDash(false);
				//向いてる方向 プレイヤー
				switch (pEnemy->GetDirection())
				{
				case CEnemy::DIRECTION_LEFT:
					pEnemy->SetState(CEnemy::STATE_TSUPPARI);
					pEnemy->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x - 10, p1pos.y, p1pos.z));
					m_nCntAttackFlame = TUPARI_FLAME;
					pEnemy->SetRecovery(true);
					pEnemy->SetRecoveryTime(TUPARI_RECOVERY);
					m_bAttack = true;
					break;
				case CEnemy::DIRECTION_RIGHT:
					pEnemy->SetState(CEnemy::STATE_TSUPPARI);
					pEnemy->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x + 10, p1pos.y, p1pos.z));
					m_nCntAttackFlame = TUPARI_FLAME;
					pEnemy->SetRecovery(true);
					pEnemy->SetRecoveryTime(TUPARI_RECOVERY);
					m_bAttack = true;
					break;
				}

				m_nUltTimer = 70;
			}
			else
			{
				m_nUltTimer++;

			}

			// プレイヤーを奥義状態にする
			pEnemy->SetState(CEnemy::STATE_ULT);

			CBAnimation *pAnimation = pEnemy->GetAnimation();

			if (pAnimation != NULL)
			{// オーラを大きくする
				pAnimation->SetBillboard(pEnemy->GetPosition(), 150.0f, 100.0f);
			}
		}
	}
#ifdef _DEBUG
#endif
}

//=============================================================================
// 硬直処理まとめ
//=============================================================================
void CBattleSys::Recovery(void)
{
	// プレイヤーの取得
	CPlayer *pPlayer = NULL;
	// エネミーの取得
	CEnemy *pEnemy = NULL;
	// モード取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();

		CTutorial::SetWinner(CTutorial::WINNER_NONE);
		CTutorial::SetHit(true);
		CTutorial::SetHit(false);
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();

		CGame::SetWinner(CGame::WINNER_NONE);
		CGame::SetHit(true);
		CGame::SetHit(false);
	}

	if (pPlayer != NULL)
	{
		pPlayer->SetState(CPlayer::STATE_NEUTRAL);
		pPlayer->SetRecovery(true);
		pPlayer->SetRecoveryTime(20);
	}
	if (pEnemy != NULL)
	{
		pEnemy->SetState(CEnemy::STATE_NEUTRAL);
		pEnemy->SetRecovery(true);
		pEnemy->SetRecoveryTime(20);
	}
}

//=============================================================================
// バトル処理まとめ
//=============================================================================
void CBattleSys::Battle(int nPlayer, ATTACK_TYPE AttackType, D3DXVECTOR3 P1move, D3DXVECTOR3 P2move)
{
	// プレイヤーの取得
	CPlayer *pPlayer = NULL;
	// エネミー
	CEnemy *pEnemy = NULL;
	// ゲージ
	CGauge *pGauge = NULL;
	CUltimateGauge *pULTGauge = NULL;

	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();
		pGauge = CTutorial::GetGauge();
		pULTGauge = CTutorial::GetUltimateGauge();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();
		pGauge = CGame::GetGauge();
		pULTGauge = CGame::GetUltimateGauge();
	}

	//プレイヤー1の位置を取得
	D3DXVECTOR3 p1pos;
	if (pPlayer != NULL)
	{
		p1pos = pPlayer->GetPosition();
	}
	//プレイヤー2の位置を取得
	D3DXVECTOR3 p2pos;
	if (pPlayer != NULL)
	{
		p2pos = pEnemy->GetPosition();
	}
	//ダメージ判定
	if (nPlayer == 0)
	{
		if (m_bPlayerUlt == false)
		{
			pGauge->SetGaugeRightLeft(-DAMAGE, HEEL);
			pULTGauge->SetGaugeRightLeft(0.0f, 30.0f);
		}
	}
	else
	{
		pGauge->SetGaugeRightLeft(HEEL, -DAMAGE);
		pULTGauge->SetGaugeRightLeft(30.0f, 0.0f);
	}

	//攻撃の種類
	switch (AttackType)
	{
	case ATTACK_TYPE_YORI:
		if (pEnemy->GetDohyo() == CEnemy::DOHYO_HAZI && pEnemy->GetDying() == false
			|| pPlayer->GetDohyo() == CPlayer::DOHYO_HAZI && pPlayer->GetDying() == false)
		{//体力がある場合で土俵端の寄り攻撃
		 //向いてる方向 プレイヤー
			switch (pPlayer->GetDirection())
			{
			case CPlayer::DIRECTION_LEFT:
				if (pPlayer->GetDohyoHaziLR() == CPlayer::HAZI_RIGHT && pPlayer->GetDying() == false && nPlayer == 1)
				{
					pPlayer->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
				}
				else if (pEnemy->GetDohyoHaziLR() == CEnemy::HAZI_LEFT && pEnemy->GetDying() == false && nPlayer == 0)
				{
					pPlayer->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
				}
				else
				{//通常
					pPlayer->SetMove(P1move);
					pEnemy->SetMove(P2move);
				}
				break;
			case CPlayer::DIRECTION_RIGHT:
				if (pPlayer->GetDohyoHaziLR() == CPlayer::HAZI_LEFT && pPlayer->GetDying() == false && nPlayer == 1)
				{
					pPlayer->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
				}
				else if (pEnemy->GetDohyoHaziLR() == CEnemy::HAZI_RIGHT && pEnemy->GetDying() == false && nPlayer == 0)
				{
					pPlayer->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
				}
				else
				{//通常
					pPlayer->SetMove(P1move);
					pEnemy->SetMove(P2move);
				}
				break;
			}
		}
		else
		{
			pPlayer->SetMove(P1move);
			pEnemy->SetMove(P2move);
		}

		//フレーム設定
		m_nCntAttackFlame = YORI_FLAME;
		break;
	case ATTACK_TYPE_OSI:
		if (pEnemy->GetDohyo() == CEnemy::DOHYO_HAZI && pEnemy->GetDying() == false
			|| pPlayer->GetDohyo() == CPlayer::DOHYO_HAZI && pPlayer->GetDying() == false)
		{//体力がある場合で土俵端の押し攻撃
			//向いてる方向 プレイヤー
			switch (pPlayer->GetDirection())
			{
			case CPlayer::DIRECTION_LEFT:
				if (pPlayer->GetDohyoHaziLR() == CPlayer::HAZI_RIGHT && pPlayer->GetDying() == false && nPlayer == 1)
				{
					pPlayer->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(-P1move.x / OSI_RECOIL, 0.0f, 0.0f));
				}
				else if (pEnemy->GetDohyoHaziLR() == CEnemy::HAZI_LEFT && pEnemy->GetDying() == false && nPlayer == 0)
				{
					pPlayer->SetMove(D3DXVECTOR3(-P2move.x / OSI_RECOIL, 0.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
				}
				else
				{//通常
					pPlayer->SetMove(P1move);
					pEnemy->SetMove(P2move);
				}
				break;
			case CPlayer::DIRECTION_RIGHT:
				if (pPlayer->GetDohyoHaziLR() == CPlayer::HAZI_LEFT && pPlayer->GetDying() == false && nPlayer == 1)
				{
					pPlayer->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(-P1move.x / OSI_RECOIL, 0.0f, 0.0f));
				}
				else if (pEnemy->GetDohyoHaziLR() == CEnemy::HAZI_RIGHT && pEnemy->GetDying() == false && nPlayer == 0)
				{
					pPlayer->SetMove(D3DXVECTOR3(-P2move.x / OSI_RECOIL, 0.0f, 0.0f));
					pEnemy->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
				}
				else
				{//通常
					pPlayer->SetMove(P1move);
					pEnemy->SetMove(P2move);
				}
				break;
			}
		}
		else
		{
			pPlayer->SetMove(P1move);
			pEnemy->SetMove(P2move);
		}
		//硬直
		Recovery();
		//フレーム設定
		m_nCntAttackFlame = OSI_FLAME;
		break;
	case ATTACK_TYPE_NAGE:

		if (pEnemy->GetDohyo() == CEnemy::DOHYO_HAZI && pEnemy->GetDying() == false
			|| pPlayer->GetDohyo() == CPlayer::DOHYO_HAZI && pPlayer->GetDying() == false)
		{//体力がある場合で土俵端の投げ攻撃
			if (pPlayer->GetDohyo() == CPlayer::DOHYO_HAZI && nPlayer == 0)
			{//プレイヤー(投げ)が端で投げ側の攻撃
				pPlayer->SetMove(D3DXVECTOR3(-P2move.x / NAGE_DIVIDED, 0.0f, 0.0f));
				pEnemy->SetMove(D3DXVECTOR3(P2move.x / NAGE_DIVIDED, KNOCKUP_MOVE, 0.0f));
			}
			else if (pEnemy->GetDohyo() == CEnemy::DOHYO_HAZI && nPlayer == 1)
			{//エネミー(投げ)が端で投げ側の攻撃
				pPlayer->SetMove(D3DXVECTOR3(P1move.x / NAGE_DIVIDED, KNOCKUP_MOVE, 0.0f));
				pEnemy->SetMove(D3DXVECTOR3(-P1move.x / NAGE_DIVIDED, 0.0f, 0.0f));
			}
			else
			{//投げられる側が端で投げ側の攻撃
				pPlayer->SetMove(P1move);
				pEnemy->SetMove(P2move);
			}
		}
		else
		{//土俵端以外の投げ攻撃
			pPlayer->SetMove(P1move);
			pEnemy->SetMove(P2move);
		}
		//フレーム設定
		m_nCntAttackFlame = NAGE_FLAME;
		//硬直
		Recovery();
		pEnemy->SetState(CEnemy::STATE_NAGE);
		pPlayer->SetState(CPlayer::STATE_NAGE);

		break;
	case ATTACK_TYPE_TUPPARI:
		if (pPlayer->GetDohyo() == CPlayer::DOHYO_HAZI && pPlayer->GetDying() == false)
		{//プレイヤーの体力がある場合で土俵端のつっぱり攻撃
			if (nPlayer == 0)
			{	//プレイヤーのつっぱり
				if (pPlayer->GetUltDamage() == true)
				{
					pEnemy->SetMove(D3DXVECTOR3(P2move.x / 1.5f, 0.0f, 0.0f));
				}
				else
				{
					pEnemy->SetMove(D3DXVECTOR3(P2move.x / TUPPARI_RECOIL, 0.0f, 0.0f));
				}
				pPlayer->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
			}
			else
			{	//エネミーのつっぱり
				if (pPlayer->GetUltDamage() == true)
				{
					pEnemy->SetMove(D3DXVECTOR3(-P1move.x / 1.5f, 0.0f, 0.0f));
				}
				else
				{
					pEnemy->SetMove(D3DXVECTOR3(-P1move.x / TUPPARI_RECOIL, 0.0f, 0.0f));
				}
				pPlayer->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
			}
		}
		else if (pEnemy->GetDohyo() == CEnemy::DOHYO_HAZI && pEnemy->GetDying() == false)
		{//エネミーの体力がある場合で土俵端のつっぱり攻撃
			if (nPlayer == 0)
			{	//プレイヤーのつっぱり
				if (pEnemy->GetUltDamage() == true)
				{
					pPlayer->SetMove(D3DXVECTOR3(-P2move.x / 1.5f, 0.0f, 0.0f));
				}
				else
				{
					pPlayer->SetMove(D3DXVECTOR3(-P2move.x / TUPPARI_RECOIL, 0.0f, 0.0f));
				}
				
				pEnemy->SetMove(D3DXVECTOR3(0.0f, KNOCKUP_MOVE, 0.0f));
			}
			else
			{	//エネミーのつっぱり
				if (pEnemy->GetUltDamage() == true)
				{
					pPlayer->SetMove(D3DXVECTOR3(P1move.x / 1.5f, KNOCKUP_MOVE, 0.0f));
				}
				else 
				{
					pPlayer->SetMove(D3DXVECTOR3(P1move.x / TUPPARI_RECOIL, KNOCKUP_MOVE, 0.0f));
				}
				
				pEnemy->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
		}
		else
		{
			pPlayer->SetMove(P1move);
			pEnemy->SetMove(P2move);
		}


		break;
	case ATTACK_TYPE_COUNTER:

		if (pEnemy->GetDohyo() == CEnemy::DOHYO_HAZI && pEnemy->GetDying() == false
			|| pPlayer->GetDohyo() == CPlayer::DOHYO_HAZI && pPlayer->GetDying() == false)
		{//体力がある場合で土俵端のカウンター攻撃
			if (pPlayer->GetDohyo() == CPlayer::DOHYO_HAZI && nPlayer == 0)
			{//プレイヤーが端で投げ側の攻撃
				pPlayer->SetMove(D3DXVECTOR3(-P2move.x, 0.0f, 0.0f));
				pEnemy->SetMove(D3DXVECTOR3(P2move.x, 0.0f, 0.0f));
			}
			else if (pEnemy->GetDohyo() == CEnemy::DOHYO_HAZI && nPlayer == 1)
			{//エネミーが端で投げ側の攻撃
				pPlayer->SetMove(D3DXVECTOR3(P1move.x, 0.0f, 0.0f));
				pEnemy->SetMove(D3DXVECTOR3(-P1move.x, 0.0f, 0.0f));
			}
			else
			{//投げられる側が端で投げ側の攻撃
				pPlayer->SetMove(D3DXVECTOR3(P1move.x, P1move.y, P1move.z));
				pEnemy->SetMove(D3DXVECTOR3(P2move.x, P2move.y, P2move.z));
			}
		}
		else
		{
			pPlayer->SetMove(P1move);
			pEnemy->SetMove(P2move);
		}

		//追加回復
		if (nPlayer == 0)
		{
			pGauge->SetGaugeRightLeft(0, HEEL);
		}
		else
		{
			pGauge->SetGaugeRightLeft(HEEL, 0);
		}

		break;
	}
	m_bAttack = true;
	m_AttackTurn = ATTACK_TURN_NORMAL;
}


//=============================================================================
// 連打回数判定処理
//=============================================================================
void CBattleSys::PushJudge(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();
	// プレイヤーの取得
	CPlayer *pPlayer = NULL;
	// エネミー
	CEnemy *pEnemy = NULL;
	// ゲージ
	CSansoGauge *pSansoGauge = NULL;
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();
		pSansoGauge = CTutorial::GetSansoGauge();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();
		pSansoGauge = CGame::GetSansoGauge();
	}

	float fPushCntP1 = 0, fPushCntP2 = 0;

	if (pPlayer != NULL && pEnemy != NULL)
	{
		//連打判定
		if (pPlayer->GetState() == CPlayer::STATE_KUMI
			&& pEnemy->GetState() == CEnemy::STATE_KUMI)
		{
			if (pPlayer->GetRecovery() == false)
			{
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					fPushCntP1++;
					m_nCntPushP1++;
				}
				else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
				{
					fPushCntP1++;
					m_nCntPushP1++;
				}
				else
				{
					//酸素回復
					pSansoGauge->SetSansoGaugeRightLeft(SANSO_KUMI, 0);
				}
			}
			else
			{
				//酸素回復
				pSansoGauge->SetSansoGaugeRightLeft(SANSO_NEUTRAL, 0);
			}

			if (pEnemy->GetRecovery() == false)
			{
				if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true ||
					pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
				{
					fPushCntP2++;
					m_nCntPushP2++;
				}
				else if (pInputKeyboard->GetTrigger(ENEMY_B_BUTTON) == true ||
					pXInput->GetTrigger(XENEMY_B_BUTTON, 1) == true)
				{
					fPushCntP2++;
					m_nCntPushP2++;
				}
				else
				{
					//酸素回復
					pSansoGauge->SetSansoGaugeRightLeft(0, SANSO_KUMI);
				}
			}
			else
			{
				//酸素回復
				pSansoGauge->SetSansoGaugeRightLeft(0, SANSO_NEUTRAL);
			}

			m_nFlamePush++;
			//酸素消費
			fPushCntP1 *= SANSO_SIKAKE;
			fPushCntP2 *= SANSO_SIKAKE;
			pSansoGauge->SetSansoGaugeRightLeft(-fPushCntP1, -fPushCntP2);
		}
		else
		{
			m_nFlamePush = 0;
			m_nCntPushP1 = 0;
			m_nCntPushP2 = 0;
			//酸素回復
			pSansoGauge->SetSansoGaugeRightLeft(SANSO_NEUTRAL, SANSO_NEUTRAL);
		}
	}

	if (m_nFlamePush > BATTLE_FLAME)
	{
		//m_nCntPushP1 *= 7;
		//m_nCntPushP2 *= 7;
		//酸素消費
		//pGauge->SetSansoGaugeRightLeft(-(float)m_nCntPushP1, -(float)m_nCntPushP2);

		//勝敗判定
		if (m_nCntPushP1 > m_nCntPushP2)
		{
			m_AttackTurn = ATTACK_TURN_PLAYER1;
		}
		else if(m_nCntPushP1 < m_nCntPushP2)
		{
			m_AttackTurn = ATTACK_TURN_PLAYER2;
		}
		else
		{
			m_AttackTurn = ATTACK_TURN_NORMAL;
		}

		m_nFlamePush = 0;
		m_nCntPushP1 = 0;
		m_nCntPushP2 = 0;
	}

#ifdef _DEBUG
	CDebugProc::Print("cn", "連打回数カウントフレーム : ", m_nFlamePush);
	CDebugProc::Print("cn", "プレイヤーの連打回数 : ", m_nCntPushP1);
	CDebugProc::Print("cn", "エネミーの連打回数 : ", m_nCntPushP2);
	if (m_AttackTurn == ATTACK_TURN_NORMAL)
	{
		CDebugProc::Print("c", "通常勝敗判定");
	}
	else if (m_AttackTurn == ATTACK_TURN_PLAYER1)
	{
		CDebugProc::Print("c", "プレイヤー1の勝利");
	}
	else if (m_AttackTurn == ATTACK_TURN_PLAYER2)
	{
		CDebugProc::Print("c", "プレイヤー2の勝利");
	}

#endif
}

//=============================================================================
// P1攻撃処理
//=============================================================================
void CBattleSys::P1Attack(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();

	// プレイヤーの取得
	CPlayer *pPlayer = NULL;
	// エネミー
	CEnemy *pEnemy = NULL;

	CManager::MODE mode;
	mode = CManager::GetMode();
	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();
	}

	//サウンド情報の取得
	CSound *pSound = CManager::GetSound(0);
	//瀕死時の移動量
	float fHinshiOshi = 1.0f;
	float fHinshiNage = 1.0f;

	if (pEnemy != NULL)
	{
		if (pEnemy->GetDying() == true)
		{
			fHinshiNage = HINSI_N_MOVE;
			fHinshiOshi = HINSI_O_MOVE;
		}
		if (pEnemy->GetCounter() == false)
		{	//向いてる方向 プレイヤー
			switch (pPlayer->GetDirection())
			{
			case CPlayer::DIRECTION_LEFT:
				if (pInputKeyboard->GetPress(PLAYER_LEFT) == true ||
					pXInput->GetPress(XPLAYER_LEFT, 0) == true)
				{
					if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
					{
						//寄り
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_YORI, D3DXVECTOR3(-YORI_MOVE, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(-YORI_MOVE, KNOCKUP_MOVE, 0.0f));
						MotionSetYORI(0);
					}
					else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
					{	//押し
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_OSI, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-OSI_MOVE * fHinshiOshi, KNOCKUP_MOVE, 0.0f));
						pPlayer->SetMotionType(0, CPlayer::MOTION_TSUPPARI);
						pPlayer->SetbMotionEnd(0, true);
						pPlayer->SetMotionType(1, CPlayer::MOTION_TSUPPARI);
						pPlayer->SetbMotionEnd(1, true);
						pPlayer->SetbDash(false);
					}
				}
				else if (pInputKeyboard->GetPress(PLAYER_RIGHT) == true ||
					pXInput->GetPress(XPLAYER_RIGHT, 0) == true)
				{
					if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
					{	//投げ
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_NAGE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3((NAGE_MOVE * fHinshiNage), KNOCKUP_MOVE, 0.0f));
						pPlayer->SetMotionType(0, CPlayer::MOTION_NAGE);
						//pPlayer->SetbMotionEnd(0, true);
						pPlayer->SetMotionType(1, CPlayer::MOTION_NAGE);
						//pPlayer->SetbMotionEnd(1, true);
					}
				}
				break;

			case CPlayer::DIRECTION_RIGHT:
				if (pInputKeyboard->GetPress(PLAYER_RIGHT) == true ||
					pXInput->GetPress(XPLAYER_RIGHT, 0) == true)
				{
					if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
					{	//寄り
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_YORI, D3DXVECTOR3(YORI_MOVE, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(YORI_MOVE, KNOCKUP_MOVE, 0.0f));
						MotionSetYORI(0);
					}
					else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
					{	//押し
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_OSI, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(OSI_MOVE * fHinshiOshi, KNOCKUP_MOVE, 0.0f));
						pPlayer->SetMotionType(0, CPlayer::MOTION_TSUPPARI);
						pPlayer->SetbMotionEnd(0, true);
						pPlayer->SetMotionType(1, CPlayer::MOTION_TSUPPARI);
						pPlayer->SetbMotionEnd(1, true);
						pPlayer->SetbDash(false);
					}
				}
				else if (pInputKeyboard->GetPress(PLAYER_LEFT) == true ||
					pXInput->GetPress(XPLAYER_LEFT, 0) == true)
				{
					if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
					{	//投げ
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(0, ATTACK_TYPE_NAGE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3((-NAGE_MOVE * fHinshiNage), KNOCKUP_MOVE, 0.0f));
						pPlayer->SetMotionType(0, CPlayer::MOTION_NAGE);
						pPlayer->SetbMotionEnd(0, true);
						pPlayer->SetMotionType(1, CPlayer::MOTION_NAGE);
						pPlayer->SetbMotionEnd(1, true);
					}
				}
				break;
			}
		}
		else
		{//カウンター中にボタンを押した

			switch (pPlayer->GetDirection())
			{
			case CPlayer::DIRECTION_LEFT:
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					Battle(1, ATTACK_TYPE_COUNTER, D3DXVECTOR3(COUNTER_MOVE, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				}
				else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
				{
					Battle(1, ATTACK_TYPE_COUNTER, D3DXVECTOR3(COUNTER_MOVE, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				}
				break;

			case CPlayer::DIRECTION_RIGHT:
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					Battle(1, ATTACK_TYPE_COUNTER, D3DXVECTOR3(-COUNTER_MOVE, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				}
				else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
				{
					Battle(1, ATTACK_TYPE_COUNTER, D3DXVECTOR3(-COUNTER_MOVE, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				}
				break;
			}
		}
	}
}

//=============================================================================
// P2攻撃処理
//=============================================================================
void CBattleSys::P2Attack(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();

	CManager::MODE mode;
	mode = CManager::GetMode();

	// プレイヤーの取得
	CPlayer *pPlayer = NULL;
	// エネミー
	CEnemy *pEnemy = NULL;

	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();
	}
	//サウンド情報の取得
	CSound *pSound = CManager::GetSound(0);

	//瀕死時の移動量
	float fHinshiOshi = 1.0f;
	float fHinshiNage = 1.0f;

	if (pPlayer != NULL)
	{
		if (pPlayer->GetDying() == true)
		{
			fHinshiNage = HINSI_N_MOVE;
			fHinshiOshi = HINSI_O_MOVE;
		}

		if (pPlayer->GetCounter() == false)
		{
			//向いてる方向 エネミー
			switch (pEnemy->GetDirection())
			{
			case CEnemy::DIRECTION_LEFT:
				if (pInputKeyboard->GetPress(ENEMY_LEFT) == true ||
					pXInput->GetPress(XENEMY_LEFT, 1) == true)
				{
					if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
					{	//寄り
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_YORI, D3DXVECTOR3(-YORI_MOVE, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(-YORI_MOVE, KNOCKUP_MOVE, 0.0f));
						MotionSetYORI(1);
					}
					else if (pInputKeyboard->GetTrigger(ENEMY_B_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_B_BUTTON, 1) == true)
					{	//押し
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_OSI, D3DXVECTOR3(-OSI_MOVE * fHinshiOshi, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
						pEnemy->SetMotionType(0, CEnemy::MOTION_TSUPPARI);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_TSUPPARI);
						pEnemy->SetbMotionEnd(1, true);
						pEnemy->SetbDash(false);
					}
				}
				else if (pInputKeyboard->GetPress(ENEMY_RIGHT) == true ||
					pXInput->GetPress(XENEMY_RIGHT, 1) == true)
				{
					if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
					{	//投げ
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_NAGE, D3DXVECTOR3(NAGE_MOVE * fHinshiNage, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
						pEnemy->SetMotionType(0, CEnemy::MOTION_NAGE);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_NAGE);
						pEnemy->SetbMotionEnd(1, true);
					}
				}
				break;
			case CEnemy::DIRECTION_RIGHT:
				if (pInputKeyboard->GetPress(ENEMY_RIGHT) == true ||
					pXInput->GetPress(XENEMY_RIGHT, 1) == true)
				{
					if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
					{	//寄り
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_YORI, D3DXVECTOR3(YORI_MOVE, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(YORI_MOVE, KNOCKUP_MOVE, 0.0f));
						MotionSetYORI(1);
					}
					else if (pInputKeyboard->GetTrigger(ENEMY_B_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_B_BUTTON, 1) == true)
					{	//押し
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_OSI, D3DXVECTOR3(OSI_MOVE * fHinshiOshi, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
						pEnemy->SetMotionType(0, CEnemy::MOTION_TSUPPARI);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_TSUPPARI);
						pEnemy->SetbMotionEnd(1, true);
						pEnemy->SetbDash(false);
					}
				}
				else if (pInputKeyboard->GetPress(ENEMY_LEFT) == true ||
					pXInput->GetPress(XENEMY_LEFT, 1) == true)
				{
					if (pInputKeyboard->GetTrigger(ENEMY_A_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_A_BUTTON, 1) == true)
					{	//投げ
						pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
						Battle(1, ATTACK_TYPE_NAGE, D3DXVECTOR3(-NAGE_MOVE * fHinshiNage, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
						pEnemy->SetMotionType(0, CEnemy::MOTION_NAGE);
						pEnemy->SetbMotionEnd(0, true);
						pEnemy->SetMotionType(1, CEnemy::MOTION_NAGE);
						pEnemy->SetbMotionEnd(1, true);
					}
				}
				break;
			}
		}
		else
		{//カウンター中にボタンを押した
			switch (pEnemy->GetDirection())
			{
			case CEnemy::DIRECTION_LEFT:
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					Battle(0, ATTACK_TYPE_COUNTER, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(COUNTER_MOVE, 0.0f, 0.0f));
				}
				else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
				{
					Battle(0, ATTACK_TYPE_COUNTER, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(COUNTER_MOVE, 0.0f, 0.0f));
				}
				break;

			case CEnemy::DIRECTION_RIGHT:
				if (pInputKeyboard->GetTrigger(PLAYER_A_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_A_BUTTON, 0) == true)
				{
					Battle(0, ATTACK_TYPE_COUNTER, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-COUNTER_MOVE, 0.0f, 0.0f));
				}
				else if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
					pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
				{
					Battle(0, ATTACK_TYPE_COUNTER, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-COUNTER_MOVE, 0.0f, 0.0f));
				}
				break;
			}
		}
	}
}

//=============================================================================
// カウンター処理
//=============================================================================
void CBattleSys::CounterAttack(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();

	// プレイヤーの取得
	CPlayer *pPlayer = NULL;
	// エネミー
	CEnemy *pEnemy = NULL;
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();
	}

	if (pPlayer != NULL && pEnemy != NULL)
	{
		if (pPlayer->GetCounter() == false && pPlayer->GetRecovery() == false)
		{
			//向いてる方向 プレイヤー
			switch (pPlayer->GetDirection())
			{
			case CPlayer::DIRECTION_LEFT:
				if (pInputKeyboard->GetPress(PLAYER_RIGHT) == true ||
					pXInput->GetPress(XPLAYER_RIGHT, 0) == true)
				{
					if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
					{	//カウンター
						pPlayer->SetCounter(true);
						pPlayer->SetCounterTime(COUNTER_FLAME);
					}
				}
				break;
			case CPlayer::DIRECTION_RIGHT:
				if (pInputKeyboard->GetPress(PLAYER_LEFT) == true ||
					pXInput->GetPress(XPLAYER_LEFT, 0) == true)
				{
					if (pInputKeyboard->GetTrigger(PLAYER_B_BUTTON) == true ||
						pXInput->GetTrigger(XPLAYER_B_BUTTON, 0) == true)
					{	//カウンター
						pPlayer->SetCounter(true);
						pPlayer->SetCounterTime(COUNTER_FLAME);
					}
				}
				break;
			}
		}

		if (pEnemy->GetCounter() == false && pEnemy->GetRecovery() == false)
		{
			//向いてる方向 エネミー
			switch (pEnemy->GetDirection())
			{
			case CEnemy::DIRECTION_LEFT:
				if (pInputKeyboard->GetPress(ENEMY_RIGHT) == true ||
					pXInput->GetPress(XENEMY_RIGHT, 1) == true)
				{
					if (pInputKeyboard->GetTrigger(ENEMY_B_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_B_BUTTON, 1) == true)
					{	//カウンター
						pEnemy->SetCounter(true);
						pEnemy->SetCounterTime(COUNTER_FLAME);
					}
				}
				break;
			case CEnemy::DIRECTION_RIGHT:
				if (pInputKeyboard->GetPress(ENEMY_LEFT) == true ||
					pXInput->GetPress(XENEMY_LEFT, 1) == true)
				{
					if (pInputKeyboard->GetTrigger(ENEMY_B_BUTTON) == true ||
						pXInput->GetTrigger(XENEMY_B_BUTTON, 1) == true)
					{	//カウンター
						pEnemy->SetCounter(true);
						pEnemy->SetCounterTime(COUNTER_FLAME);
					}
				}
				break;
			}
		}
	}
}

//=============================================================================
// バトルのリセット処理
//=============================================================================
void CBattleSys::ResetBattle(void)
{
	// プレイヤーの取得
	CPlayer *pPlayer = NULL;
	// エネミー
	CEnemy *pEnemy = NULL;
	// ゲージの取得
	CGauge *pGauge = NULL;
	CSansoGauge *pSansoGauge = NULL;
	CUltimateGauge *pULTGauge = NULL;
	// タイムの取得
	CUITime *pTime = NULL;
	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();
		pGauge = CTutorial::GetGauge();
		pSansoGauge = CTutorial::GetSansoGauge();
		pULTGauge = CTutorial::GetUltimateGauge();
		pTime = CTutorial::GetTime();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();
		pGauge = CGame::GetGauge();
		pSansoGauge = CGame::GetSansoGauge();
		pULTGauge = CGame::GetUltimateGauge();
		pTime = CGame::GetTime();
	}

	pPlayer->InitStatus();
	pEnemy->InitStatus();
	pPlayer->SetPosition(D3DXVECTOR3(-80.0f, 20.0f, 0.0f));
	pEnemy->SetPosition(D3DXVECTOR3(80.0f, 20.0f, 0.0f));
	pPlayer->SetState(CPlayer::STATE_JANKEN);
	pEnemy->SetState(CEnemy::STATE_JANKEN);
	m_bAttack = false;
	m_nCntAttackFlame = 0;
	m_nStartCounter = 0;
	pPlayer->SetMotionType(0, CPlayer::MOTION_NEUTRAL);
	pPlayer->SetMotionType(1, CPlayer::MOTION_NEUTRAL);
	pEnemy->SetMotionType(0, CEnemy::MOTION_NEUTRAL);
	pEnemy->SetMotionType(1, CEnemy::MOTION_NEUTRAL);
	m_aJanken[0] = JANKEN_GU_BUTI;
	m_aJanken[1] = JANKEN_GU_BUTI;
	m_bPlayerUlt = false;
	m_bEnemyUlt = false;
	m_abUlt[0] = false;
	m_abUlt[1] = false;
	m_nUltTimer = 0;
	pPlayer->SetUltDis(false);
	pEnemy->SetUltDis(false);
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != NULL)
	{
		pCamera->Init();
	}

	for (int nCntPlayer = 0; nCntPlayer < MAX_CHARACTER; nCntPlayer++)
	{
		m_aGUCounter[nCntPlayer] = 0;
		m_aCHOKICounter[nCntPlayer] = 0;
		m_abPA[nCntPlayer] = false;
	}

	pGauge->SetGaugeRightLeft(600, 600);
	pSansoGauge->SetSansoGaugeRightLeft(600, 600);
	pULTGauge->SetGaugeRightLeft(-600, -600);

	if (mode == CManager::MODE_TUTORIAL)
	{
		CTutorial::SetWinner(CTutorial::WINNER_NONE);
		CTutorial::SetHit(true);
		CManager::GetTutorial()->SetbUI(true);
	}
	else if (mode == CManager::MODE_GAME)
	{
		CGame::SetWinner(CGame::WINNER_NONE);
		CGame::SetHit(true);
		CManager::GetGame()->SetbUI(true);
	}

	pTime->SetTime(60);
}

//=============================================================================
// ガード中に攻撃したときのノックバック処理
//=============================================================================
void CBattleSys::GuardKnockBack(int nAttack)
{
	// プレイヤーの取得
	CPlayer *pPlayer = NULL;
	// エネミー
	CEnemy *pEnemy = NULL;
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();
	}

	if (nAttack == 0)
	{
		//向いてる方向 プレイヤー
		switch (pPlayer->GetDirection())
		{
		case CPlayer::DIRECTION_LEFT:
			pEnemy->SetMove(D3DXVECTOR3(-GUARD_MOVE, 0.0f, 0.0f));
			break;
		case CPlayer::DIRECTION_RIGHT:
			pEnemy->SetMove(D3DXVECTOR3(GUARD_MOVE, 0.0f, 0.0f));
			break;
		}
	}
	else if (nAttack == 1)
	{
		//向いてる方向 エネミー
		switch (pEnemy->GetDirection())
		{
		case CEnemy::DIRECTION_LEFT:
			pPlayer->SetMove(D3DXVECTOR3(-GUARD_MOVE, 0.0f, 0.0f));
			break;
		case CEnemy::DIRECTION_RIGHT:
			pPlayer->SetMove(D3DXVECTOR3(GUARD_MOVE, 0.0f, 0.0f));
			break;
		}
	}
}

//=============================================================================
// 寄りのモーション処理
//=============================================================================
void CBattleSys::MotionSetYORI(int nAttack)
{
	// プレイヤーの取得
	CPlayer *pPlayer = NULL;
	// エネミー
	CEnemy *pEnemy = NULL;
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();
	}

	if (nAttack == 0)
	{
		pPlayer->SetMotionType(0, CPlayer::MOTION_TUKAMI_AGERU);
		pPlayer->SetbMotionEnd(0, false);
		pPlayer->SetMotionType(1, CPlayer::MOTION_TUKAMI_AGERU);
		pPlayer->SetbMotionEnd(1, false);
		pEnemy->SetMotionType(0, CEnemy::MOTION_TUKAMI_AGERARERU);
		pEnemy->SetbMotionEnd(0, false);
		pEnemy->SetMotionType(1, CEnemy::MOTION_TUKAMI_AGERARERU);
		pEnemy->SetbMotionEnd(1, false);
	}
	else if(nAttack == 1)
	{
		pEnemy->SetMotionType(0, CEnemy::MOTION_TUKAMI_AGERU);
		pEnemy->SetbMotionEnd(0, false);
		pEnemy->SetMotionType(1, CEnemy::MOTION_TUKAMI_AGERU);
		pEnemy->SetbMotionEnd(1, false);
		pPlayer->SetMotionType(0, CPlayer::MOTION_TUKAMI_AGERARERU);
		pPlayer->SetbMotionEnd(0, false);
		pPlayer->SetMotionType(1, CPlayer::MOTION_TUKAMI_AGERARERU);
		pPlayer->SetbMotionEnd(1, false);
	}
}

//=============================================================================
// CPUのバトル処理
//=============================================================================
void CBattleSys::CPUBattle(CEnemy::CPUACTION CpuAction)
{
	// プレイヤーの取得
	CPlayer *pPlayer = NULL;
	// エネミー
	CEnemy *pEnemy = NULL;
	// ゲージの取得
	CGauge *pGauge = NULL;
	CUltimateGauge *pULTGauge = NULL;

	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pEnemy = CTutorial::GetEnemy();
		pGauge = CTutorial::GetGauge();
		pULTGauge = CTutorial::GetUltimateGauge();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pEnemy = CGame::GetEnemy();
		pGauge = CGame::GetGauge();
		pULTGauge = CGame::GetUltimateGauge();
	}

	//サウンド情報の取得
	CSound *pSound = CManager::GetSound(0);
	D3DXVECTOR3 p1pos;
	//瀕死時の移動量
	float fHinshiOshi = 1.0f;
	float fHinshiNage = 1.0f;

	if (pPlayer != NULL)
	{	//プレイヤー1の位置を取得
		p1pos = pPlayer->GetPosition();

		if (pPlayer->GetDying() == true)
		{
			fHinshiNage = HINSI_N_MOVE;
			fHinshiOshi = HINSI_O_MOVE;
		}
	}

	//向いてる方向 エネミー
	switch (pEnemy->GetDirection())
	{
	case CEnemy::DIRECTION_LEFT:
		switch (CpuAction)
		{
		case CEnemy::CPUACTION_TUPPARI:
			if (pEnemy->GetState() == CEnemy::STATE_NEUTRAL && m_bAttack == false)
			{
				pEnemy->SetMotionType(0, CEnemy::MOTION_TSUPPARI);
				pEnemy->SetbMotionEnd(0, true);
				pEnemy->SetMotionType(1, CEnemy::MOTION_TSUPPARI);
				pEnemy->SetbMotionEnd(1, true);
				pEnemy->SetbDash(false);
				pEnemy->SetState(CEnemy::STATE_TSUPPARI);
				pEnemy->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x - 10, p1pos.y, p1pos.z));
				m_nCntAttackFlame = TUPARI_FLAME;
				pEnemy->SetRecovery(true);
				pEnemy->SetRecoveryTime(TUPARI_RECOVERY);
				m_bAttack = true;
			}
			break;
		case CEnemy::CPUACTION_YORI:
			//寄り
			pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
			Battle(1, ATTACK_TYPE_YORI, D3DXVECTOR3(-YORI_MOVE, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(-YORI_MOVE, KNOCKUP_MOVE, 0.0f));
			MotionSetYORI(1);
			break;
		case CEnemy::CPUACTION_NAGE:
			//投げ
			pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
			Battle(1, ATTACK_TYPE_NAGE, D3DXVECTOR3(NAGE_MOVE * fHinshiNage, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pEnemy->SetMotionType(0, CEnemy::MOTION_NAGE);
			pEnemy->SetbMotionEnd(0, true);
			pEnemy->SetMotionType(1, CEnemy::MOTION_NAGE);
			pEnemy->SetbMotionEnd(1, true);
			break;
		case CEnemy::CPUACTION_OSHI:
			//押し
			pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
			Battle(1, ATTACK_TYPE_OSI, D3DXVECTOR3(-OSI_MOVE * fHinshiOshi, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pEnemy->SetMotionType(0, CEnemy::MOTION_TSUPPARI);
			pEnemy->SetbMotionEnd(0, true);
			pEnemy->SetMotionType(1, CEnemy::MOTION_TSUPPARI);
			pEnemy->SetbMotionEnd(1, true);
			pEnemy->SetbDash(false);
			break;
		}
		break;
	case CEnemy::DIRECTION_RIGHT:
		switch (CpuAction)
		{
		case CEnemy::CPUACTION_TUPPARI:
			if (pEnemy->GetState() == CEnemy::STATE_NEUTRAL && m_bAttack == false)
			{
				pEnemy->SetMotionType(0, CEnemy::MOTION_TSUPPARI);
				pEnemy->SetbMotionEnd(0, true);
				pEnemy->SetMotionType(1, CEnemy::MOTION_TSUPPARI);
				pEnemy->SetbMotionEnd(1, true);
				pEnemy->SetbDash(false);

				pEnemy->SetState(CEnemy::STATE_TSUPPARI);
				pEnemy->GetTuppari().SetPosition(D3DXVECTOR3(p1pos.x + 10, p1pos.y, p1pos.z));
				m_nCntAttackFlame = TUPARI_FLAME;
				pEnemy->SetRecovery(true);
				pEnemy->SetRecoveryTime(TUPARI_RECOVERY);
				m_bAttack = true;
			}
			break;
		case CEnemy::CPUACTION_YORI:
			//寄り
			pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
			Battle(1, ATTACK_TYPE_YORI, D3DXVECTOR3(YORI_MOVE, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(YORI_MOVE, KNOCKUP_MOVE, 0.0f));
			MotionSetYORI(1);
			break;
		case CEnemy::CPUACTION_NAGE:
			//投げ
			pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
			Battle(1, ATTACK_TYPE_NAGE, D3DXVECTOR3(-NAGE_MOVE * fHinshiNage, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pEnemy->SetMotionType(0, CEnemy::MOTION_NAGE);
			pEnemy->SetbMotionEnd(0, true);
			pEnemy->SetMotionType(1, CEnemy::MOTION_NAGE);
			pEnemy->SetbMotionEnd(1, true);
			break;
		case CEnemy::CPUACTION_OSHI:
			//押し
			pSound->PlaySound(pSound->SOUND_LABEL_SE_HIT00);
			Battle(1, ATTACK_TYPE_OSI, D3DXVECTOR3(OSI_MOVE * fHinshiOshi, KNOCKUP_MOVE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pEnemy->SetMotionType(0, CEnemy::MOTION_TSUPPARI);
			pEnemy->SetbMotionEnd(0, true);
			pEnemy->SetMotionType(1, CEnemy::MOTION_TSUPPARI);
			pEnemy->SetbMotionEnd(1, true);
			pEnemy->SetbDash(false);
			break;
		}
	}
}
