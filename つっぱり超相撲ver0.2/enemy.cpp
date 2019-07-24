//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author : 目黒 未来也
//
//=============================================================================
#include "enemy.h"
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
#include "effect.h"
#include "tutorial.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define ENEMY_COLLISION			(D3DXVECTOR3(20.0f, 60.0f, 20.0f))		//エネミーの当たり判定
#define DOHYO_HAZI_MAX			(175.0f)
#define DOHYO_HAZI_MIN			(150.0f)
#define DASH_MOVE				(1.2f)
#define PARTICLE_ROT			((rand() % 628) / 100.0f)		//全方向
#define PARTICLE_NUM			(20)							// 壁に激突したときのパーティクルの数
#define PARTICLE_TIME			(35)							// 壁に激突したときのパーティクル出現時間
#define FILE_NAME_0				("data\\TEXT\\motion_Wrestler_down.txt")
#define FILE_NAME_1				("data\\TEXT\\motion_Wrestler_up.txt")
#define FILE_NAME_2				("data\\TEXT\\motion_resura_down.txt")
#define FILE_NAME_3				("data\\TEXT\\motion_resura_up.txt")

#define DOHYO_COLLISION			(20.0f)
#define GUARD_NOW_SANSO			(-2.5f)
#define GUARD_SANSO				(-20.0f)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPD3DXMESH					CEnemy::m_pMeshModel[MAX_PARTS][MODEL_PARENT] = {};
LPD3DXBUFFER				CEnemy::m_pBuffMatModel[MAX_PARTS][MODEL_PARENT] = {};
LPDIRECT3DTEXTURE9			CEnemy::m_pTextureModel[MAX_PARTS][MODEL_PARENT] = {};
DWORD						CEnemy::m_nNumMatModel[MAX_PARTS][MODEL_PARENT] = {};
CBAnimation *CEnemy::m_pAnimation = NULL;

//=============================================================================
// エネミークラスのコンストラクタ
//=============================================================================
CEnemy::CEnemy() : CSceneX(ENEMY_PRIORITY)
{
	// 値をクリア
	m_pTexture = NULL;							// テクスチャへのポインタ
	m_pVtxBuff = NULL;							// 頂点バッファへのポインタ
	m_bLand = false;							// 右にいるかどうか
	m_bHit = false;								// 右にいるかどうか
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_State = STATE_NEUTRAL;
	m_Direction = DIRECTION_LEFT;
	m_bRecovery = false;	// 硬直フラグ
	m_nRecoveryTime = 0;	// 硬直時間
	m_nLife = 0;
	m_bDying = false;
	m_pTuppari = NULL;
	m_DohyoState = DOHYO_NORMAL;
	m_nCounterTime = 0;
	m_bCounter = false;
	m_DohyoHaziLR = HAZI_NORMAL;
	m_fLength = 0.0f;
	m_nOldMotion = 0;	//前のモーション
	//m_turnRot = D3DXVECTOR3(0, 0, 0);
	m_fRot = 0.0f;
	m_nSiomakiCnt = 0;
	m_bDash = false;
	//CPU用の変数
	m_nThinkingTime = 0;
	m_nActionTime = 0;
	m_CPUAction = CPUACTION_NEUTRAL;
	m_bAction = false;
	m_DamageCnt = 0;
	m_bUse = false;
	m_bJanken = false;

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
		m_nMotionType[nCntParent] = 0;	//現在のモーションタイプ
		m_nNumParts[nCntParent] = 0;
		m_nKey[nCntParent] = 0;			//現在のキー
		m_nCountFlame[nCntParent] = 0;	//現在のフレーム
		m_bMotionEnd[nCntParent] = false;
	}

#ifdef _DEBUG
	m_bColBlockDraw = false;
#endif
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{
}

//=============================================================================
// オブジェクトの生成処理
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODE mode)
{
	CEnemy *pEnemy = NULL;

	if (pEnemy == NULL)
	{
		// オブジェクトクラスの生成
		pEnemy = new CEnemy;

		if (pEnemy != NULL)
		{
			//pEnemy->BindModel(CLoad::GetBuffMat(CLoad::MODEL_ENEMY), CLoad::GetNumMat(CLoad::MODEL_ENEMY), CLoad::GetMesh(CLoad::MODEL_ENEMY));
			pEnemy->Init(pos, rot);
			pEnemy->m_Mode = mode;
		}
	}

	return pEnemy;
}

//=============================================================================
// エネミー初期化処理
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// タイトル取得
	CGame *pGame;
	pGame = CManager::GetGame();
	CTutorial *pTuto;
	pTuto = CManager::GetTutorial();
	CUltimate *pUltimate;
	pUltimate = CManager::GetUltimate();
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{
		if (pGame != NULL)
		{
			// 選ばれたキャラクターのモデルを割り当て
			if (pGame->Get2P() == 0)
			{// プレイヤー
				BindModel(CLoad::GetBuffMat(CLoad::MODEL_PLAYER), CLoad::GetNumMat(CLoad::MODEL_PLAYER), CLoad::GetMesh(CLoad::MODEL_PLAYER));
			}
			else if (pGame->Get2P() == 1)
			{// エネミー
				BindModel(CLoad::GetBuffMat(CLoad::MODEL_ENEMY), CLoad::GetNumMat(CLoad::MODEL_ENEMY), CLoad::GetMesh(CLoad::MODEL_ENEMY));
			}
		}
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{
		if (pTuto != NULL)
		{
			// 選ばれたキャラクターのモデルを割り当て
			if (pTuto->Get2P() == 0)
			{// プレイヤー
				BindModel(CLoad::GetBuffMat(CLoad::MODEL_PLAYER), CLoad::GetNumMat(CLoad::MODEL_PLAYER), CLoad::GetMesh(CLoad::MODEL_PLAYER));
			}
			else if (pTuto->Get2P() == 1)
			{// エネミー
				BindModel(CLoad::GetBuffMat(CLoad::MODEL_ENEMY), CLoad::GetNumMat(CLoad::MODEL_ENEMY), CLoad::GetMesh(CLoad::MODEL_ENEMY));
			}
		}
	}
	else if (mode == CManager::MODE_ULTIMATE)
	{
		if (pUltimate != NULL)
		{
			// 選ばれたキャラクターのモデルを割り当て
			if (pUltimate->Get2P() == 0)
			{// プレイヤー
				BindModel(CLoad::GetBuffMat(CLoad::MODEL_PLAYER), CLoad::GetNumMat(CLoad::MODEL_PLAYER), CLoad::GetMesh(CLoad::MODEL_PLAYER));
			}
			else if (pUltimate->Get2P() == 1)
			{// エネミー
				BindModel(CLoad::GetBuffMat(CLoad::MODEL_ENEMY), CLoad::GetNumMat(CLoad::MODEL_ENEMY), CLoad::GetMesh(CLoad::MODEL_ENEMY));
			}
		}
	}
	else
	{
		BindModel(CLoad::GetBuffMat(CLoad::MODEL_ENEMY), CLoad::GetNumMat(CLoad::MODEL_ENEMY), CLoad::GetMesh(CLoad::MODEL_ENEMY));
	}

	// 2Dオブジェクト初期化処理
	CSceneX::Init(pos);

	// オブジェクトの種類の設定
	SetObjType(CScene::OBJTYPE_ENEMY);

	m_move = D3DXVECTOR3(0, 0, 0);
	m_fDestAngle = 0;
	m_fDiffAngle = 0;
	m_bLand = false;					// 右にいるかどうか
	m_bHit = false;					// 右にいるかどうか
	m_State = STATE_JANKEN;
	m_Direction = DIRECTION_LEFT;
	m_bDying = false;
	m_bRecovery = false;	// 硬直フラグ
	m_nRecoveryTime = 0;	// 硬直時間
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
	m_nThinkingTime = 0;
	m_nActionTime = 0;
	m_CPUAction = CPUACTION_NEUTRAL;
	m_bAction = false;
	m_bPlayerDamage = false;
	m_DamageCnt = 0;
	m_bUltDamage = false;

	//つっぱり生成
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

	//モーション用変数
	for (int nCntParent = 0; nCntParent < MODEL_PARENT; nCntParent++)
	{
		m_nKey[nCntParent] = 0;			//現在のキー
		m_nCountFlame[nCntParent] = 0;		//現在のフレーム
		m_nMotionType[nCntParent] = 0;	//現在のモーションタイプ
	}
	//前のモーション
	m_nOldMotion = 0;
	//エネミー情報の読み込み
	if (mode == CManager::MODE_GAME)
	{
		if (pGame != NULL)
		{
			// 選ばれたキャラクターのモデルを割り当て
			if (pGame->Get2P() == 0)
			{// 力士
				FileLoad(FILE_NAME_0, 0);
				FileLoad(FILE_NAME_1, 1);
			}
			else if (pGame->Get2P() == 1)
			{// レスラー
				FileLoad(FILE_NAME_2, 0);
				FileLoad(FILE_NAME_3, 1);
			}
		}
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{
		if (pTuto != NULL)
		{
			// 選ばれたキャラクターのモデルを割り当て
			if (pTuto->Get2P() == 0)
			{// 力士
				FileLoad(FILE_NAME_0, 0);
				FileLoad(FILE_NAME_1, 1);
			}
			else if (pTuto->Get2P() == 1)
			{// レスラー
				FileLoad(FILE_NAME_2, 0);
				FileLoad(FILE_NAME_3, 1);
			}
		}
	}
	else
	{
		FileLoad(FILE_NAME_2, 0);
		FileLoad(FILE_NAME_3, 1);
	}
	//モデルの親を指定
	m_apModel[0][1]->SetParent(m_apModel[0][0]);

	if (mode == CManager::MODE_GAME)
	{//ゲームモードだったら処理に入る
	}
	return S_OK;
}

//=============================================================================
// エネミー終了処理
//=============================================================================
void CEnemy::Uninit(void)
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

	// 2Dオブジェクト終了処理
	CSceneX::Uninit();
}

//=============================================================================
// エネミー更新処理
//=============================================================================
void CEnemy::Update(void)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();
	// 位置取得
	D3DXVECTOR3 pos;
	pos = CSceneX::GetPosition();
	// 位置取得
	D3DXVECTOR3 rot;
	rot = CSceneX::GetRot();
	// カメラ取得
	CCamera *pCamera;
	pCamera = CManager::GetCamera();
	// カメラの向きを取得
	D3DXVECTOR3 cameraRot;
	cameraRot = pCamera->GetRot();
	// 移動処理取得
	CCharacterMove *pCharacterMove;
	pCharacterMove = CManager::GetCharacterMove();
	// 影の取得
	CShadow *pShadow = NULL;
	// プレイヤー取得
	CPlayer *pPlayer = NULL;
	//ゲージの取得
	CUltimateGauge *pULTGauge = NULL;
	CManager::MODE mode;
	mode = CManager::GetMode();
	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pULTGauge = CTutorial::GetUltimateGauge();
		pShadow = CTutorial::GetShadow();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pULTGauge = CGame::GetUltimateGauge();
		pShadow = CGame::GetShadow();
	}
	// 前のフレームの位置代入
	m_posOld = pos;

	float fMoveEnemy = MOVE_ENEMY;	// エネミーの移動量を設定

	switch (mode)
	{
	case CManager::MODE_GAME:
		if (CGame::GetState() == CGame::STATE_GAME)
		{
			//エネミーの操作
			if (m_Mode == MODE_P2)
			{
				fMoveEnemy = EnemyOperation(pos, fMoveEnemy);
			}
			else if(m_Mode == MODE_CPU)
			{
				fMoveEnemy = EnemyCPU(pos, fMoveEnemy);
			}

			//タイマーの更新
			TimerUpdate();

			//角度の設定
			rot = DirectionEnemy(pos, rot);

			//敵と当たったとき
			CollisionPlayerAction();

			if (pPlayer != NULL)
			{
				TsuppariCollision(pos);
			}
			//つっぱり位置更新
			m_pTuppari->SetPosition(pos);

			//土俵際判定
			DohyoHaziWhether(pos);

			if (pULTGauge->GetUlt(0) == true && m_bUltDis == false)
			{
				if (m_pAnimation == NULL)
				{
					m_pAnimation = CBAnimation::Create(D3DXVECTOR3(pos), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
						50.0f, 100.0f, 0.0625f, 1.0f, 1.5f, 16, 0, 0, 1);
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

			if (pos.x > 550.0f)
			{
				m_bWallHit = true;
				pos.x = 550.0f;
				m_move.x = 0.0f;

			}
		}
		break;
	case CManager::MODE_TUTORIAL:

		if (CTutorial::GetState() == CTutorial::STATE_GAME)
		{
			//エネミーの処理
			EnemyTutorial(pos, fMoveEnemy);
			//タイマーの更新
			TimerUpdate();

			//角度の設定
			rot = DirectionEnemy(pos, rot);

			//敵と当たったとき
			CollisionPlayerAction();

			if (pPlayer != NULL)
			{
				TsuppariCollision(pos);
			}
			//つっぱり位置更新
			m_pTuppari->SetPosition(pos);

			//土俵際判定
			DohyoHaziWhether(pos);

			if (pULTGauge->GetUlt(0) == true && m_bUltDis == false)
			{
				if (m_pAnimation == NULL)
				{
					m_pAnimation = CBAnimation::Create(D3DXVECTOR3(pos), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
						50.0f, 100.0f, 0.0625f, 1.0f, 1.5f, 16, 0, 0, 1);
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
		// 回転処理
		m_fRot = sinf(D3DX_PI + rot.y);
		m_bSelect = pCharacterMove->CharaTurn(&pos, &rot, m_fRot, m_fLength);
		break;
	case CManager::MODE_ULTIMATE:
		if (pos.x < 550.0f)
		{
			m_move = pCharacterMove->MoveRight(m_move, fMoveEnemy * 15.0f);
		}
		break;
	}

	//キャラ入場
	EntryEnemy(pos, fMoveEnemy);


	pos += m_move;

	// 重力加算
	if (mode != CManager::MODE_ULTIMATE)
	{
		m_move.y -= cosf(D3DX_PI * 0.0f) * 0.5f;
	}

	//減速
	m_move.x += (0.0f - m_move.x) * 0.5f;
	m_move.z += (0.0f - m_move.z) * 0.5f;
	m_move.y += (0.0f - m_move.y) * 0.01f;

	if (pShadow != NULL)
	{
		pShadow->SetPos(pos);
	}

	// モデルとの当たり判定
	CollisonSceneX(&pos, &D3DXVECTOR3(m_posOld.x, m_posOld.y + 1.0f, m_posOld.z), &m_move, ENEMY_COLLISION);

	CSceneX::SetPosition(pos);
	CSceneX::SetRot(rot);

	if (pos.y <= 0)
	{
		pos.y = 0;
		CSceneX::SetPosition(pos);
		if (m_bUltDamage == false)
		{
			if (mode == CManager::MODE_TUTORIAL)
			{
				CTutorial::SetWinner(CTutorial::WINNER_PLAYER1);
			}
			else if (mode == CManager::MODE_GAME)
			{
				CGame::SetWinner(CGame::WINNER_PLAYER1);
			}
		}
	}

	//モーション更新
	UpdateMotion(0);
	UpdateMotion(1);

#ifdef _DEBUG

	int nCnt = 0;

	//CDebugProc::Print("cfccfccfc", "エネミーの位置 : x", pos.x, "f", "   y", pos.y, "f", "  z", pos.z, "f");
	//CDebugProc::Print("cfccfccfc", "エネミーのVtxMax : x", CSceneX::GetVtxMax().x, "f", "   y", CSceneX::GetVtxMax().y, "f", "  z", CSceneX::GetVtxMax().z, "f");
	//CDebugProc::Print("cfccfccfc", "エネミーのVtxMin : x", CSceneX::GetVtxMin().x, "f", "   y", CSceneX::GetVtxMin().y, "f", "  z", CSceneX::GetVtxMin().z, "f");
	CDebugProc::Print("cn", "エネミーの状態 : ", m_State);
	CDebugProc::Print("cn", "エネミーの向き : ", m_Direction);
	CDebugProc::Print("cf", "エネミーの向き : ", rot.y);
	if (m_bRecovery == true)
	{
		CDebugProc::Print("c", " エネミーリカバリー　ON ");
	}
	else
	{
		CDebugProc::Print("c", " エネミーリカバリー　OFF ");
	}
	if (m_bCounter == true)
	{
		CDebugProc::Print("c", " エネミーカウンター　ON ");
	}
	else
	{
		CDebugProc::Print("c", " エネミーカウンター　OFF ");
	}
	if (m_DohyoState == DOHYO_NORMAL)
	{
		CDebugProc::Print("c", " 土俵端　OFF ");
	}
	else
	{
		CDebugProc::Print("c", " 土俵端　ON ");
	}
	if (m_bDying == true)
	{
		CDebugProc::Print("c", "エネミー 瀕死 ");
	}
	else
	{
		CDebugProc::Print("c", "エネミー 生存 ");
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
		//3項演算 式１?式２:式３  bool == true(式1) なら 式2 : falseなら式3
		m_bColBlockDraw = m_bColBlockDraw == true ? m_bColBlockDraw = false : m_bColBlockDraw = true;
	}

	if (m_bUltDamage == true)
	{
		CDebugProc::Print("c", "敵が奥義受けている");
	}
	else if (m_bUltDamage == false)
	{
		CDebugProc::Print("c", "敵が奥義受けていない");
	}

	//CDebugProc::Print("cn", " Numキー0  : ", m_nKey[0]);
	//CDebugProc::Print("cn", " フレーム数0  : ", m_nCountFlame[0]);
	//CDebugProc::Print("cn", " Numキー1  : ", m_nKey[1]);
	//CDebugProc::Print("cn", " フレーム数1 : ", m_nCountFlame[1]);
#endif
}

//=============================================================================
// エネミー描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	//デバイスを取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;

#ifdef _DEBUG
	if (m_bColBlockDraw == true)
	{	// 2Dオブジェクト描画処理
		CSceneX::Draw();
	}
#endif

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 位置取得
	D3DXVECTOR3 pos;
	pos = CSceneX::GetPosition();
	D3DXVECTOR3 rot;
	rot = CSceneX::GetRot();

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	// ワールドマトリックスの設定
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
// モデルとの当たり判定の処理
//=============================================================================
void CEnemy::CollisonSceneX(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 radius)
{
	CScene *pScene = NULL;

	// 先頭のオブジェクトを取得
	pScene = CScene::GetTop(DOHYO_PRIORITY);

	while (pScene != NULL)
	{// 優先順位が3のオブジェクトを1つ1つ確かめる
	 // 処理の最中に消える可能性があるから先に記録しておく
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{// 死亡フラグが立っていないもの
			if (pScene->GetObjType() == CScene::OBJTYPE_DOHYO)
			{// オブジェクトの種類を確かめる
				m_bLand = ((CSceneX*)pScene)->Collision(pos, posOld, move, radius);
				if (m_bLand == true)
				{// モデルに当たる
				 //m_bJump = false;
				}
			}
		}

		// 次のシーンに進める
		pScene = pSceneNext;
	}
}

//=============================================================================
// 右側にいるかどうかを取得
//=============================================================================
bool CEnemy::GetLand(void)
{
	return m_bLand;
}

//=============================================================================
// 状態を取得
//=============================================================================
CEnemy::STATE CEnemy::GetState(void)
{
	return m_State;
}

//=============================================================================
// 状態を設定
//=============================================================================
void CEnemy::SetState(STATE state)
{
	m_State = state;
}

//=============================================================================
// 移動量を設定
//=============================================================================
void CEnemy::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=============================================================================
// モーションを設定
//=============================================================================
void CEnemy::SetMotionType(int nParent, CEnemy::MOTION_TYPE MotionType)
{
	m_MotionType[nParent] = MotionType;
	m_nMotionType[nParent] = m_MotionType[nParent];
	m_nKey[nParent] = 0;
	m_nCountFlame[nParent] = 0;
	m_bDash = false;
}

//=============================================================================
// エネミーのステータスを初期化
//=============================================================================
void CEnemy::InitStatus(void)
{
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDestAngle = 0;
	m_fDiffAngle = 0;
	m_bLand = false;				// のっているかどうか
	m_bHit = false;					// 当たっているかどうか
	m_State = STATE_JANKEN;
	m_Direction = DIRECTION_RIGHT;
	m_bRecovery = false;			// 硬直フラグ
	m_nRecoveryTime = 0;			// 硬直時間
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
	//CPU用の変数
	m_nThinkingTime = 0;
	m_nActionTime = 0;
	m_CPUAction = CPUACTION_NEUTRAL;
	m_bAction = false;
	m_DamageCnt = 0;
	m_bUltDamage = false;

	for (int nCntParent = 0; nCntParent < MODEL_PARENT; nCntParent++)
	{
		m_nKey[nCntParent] = 0;			//現在のキー
		m_nCountFlame[nCntParent] = 0;	//現在のフレーム
		m_bMotionEnd[nCntParent] = false;
	}
}

//=============================================================================
// エネミーの操作
//=============================================================================
float CEnemy::EnemyOperation(D3DXVECTOR3 pos, float fMoveEnemy)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();
	// 移動処理取得
	CCharacterMove *pCharacterMove;
	pCharacterMove = CManager::GetCharacterMove();
	// ゲージの取得
	CSansoGauge *pSansoGauge = NULL;
	CBattleSys *pBattleSys = NULL;
	CPlayer *pPlayer = NULL;

	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{
		pSansoGauge = CTutorial::GetSansoGauge();
		pPlayer = CTutorial::GetPlayer();
		pBattleSys = CTutorial::GetBatlteSys();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pSansoGauge = CGame::GetSansoGauge();
		pPlayer = CGame::GetPlayer();
		pBattleSys = CGame::GetBatlteSys();
	}
	if (pBattleSys->GetUlt(0) == false)
	{
		//通常状態で硬直していない
		if (m_State == STATE_NEUTRAL && m_bRecovery == false)
		{
			//任意のキー←
			if (pInputKeyboard->GetPress(ENEMY_LEFT) == true ||
				pXInput->GetPress(XENEMY_LEFT, 1) == true)
			{
				//ダッシュ設定
				if (pInputKeyboard->GetPress(ENEMY_B_BUTTON) == true ||
					pXInput->GetPress(XENEMY_B_BUTTON, 1) == true)
				{
					fMoveEnemy = DASH_MOVE;
					m_bDash = true;
				}
				else
				{
					m_bDash = false;
				}
				// 左に進む
				m_move = pCharacterMove->MoveLeft(m_move, fMoveEnemy);
				m_nMotionType[0] = MOTION_SURIASI;
				m_nMotionType[1] = MOTION_SURIASI;
			}

			//任意のキー→
			else if (pInputKeyboard->GetPress(ENEMY_RIGHT) == true ||
				pXInput->GetPress(XENEMY_RIGHT, 1) == true)
			{
				//ダッシュ設定
				if (pInputKeyboard->GetPress(ENEMY_B_BUTTON) == true ||
					pXInput->GetPress(XENEMY_B_BUTTON, 1) == true)
				{
					fMoveEnemy = DASH_MOVE;
					m_bDash = true;
				}
				else
				{
					m_bDash = false;
				}
				// 右に進む
				m_move = pCharacterMove->MoveRight(m_move, fMoveEnemy);
				m_nMotionType[0] = MOTION_SURIASI;
				m_nMotionType[1] = MOTION_SURIASI;
			}
			else
			{
				if (m_bMotionEnd[0] == true)
				{
					m_nMotionType[0] = MOTION_BATTLE_NEUTRAL;
				}
				if (m_bMotionEnd[1] == true)
				{
					m_nMotionType[1] = MOTION_BATTLE_NEUTRAL;
				}
			}
		}

		if (m_State == STATE_NEUTRAL || m_State == STATE_GUARD)
		{
			//ガード状態
			if (pInputKeyboard->GetPress(ENEMY_C_BUTTON) == true ||
				pXInput->GetPress(XENEMY_X_BUTTON, 1) == true)
			{
				m_State = STATE_GUARD;
				pSansoGauge->SetSansoGaugeRightLeft(0, GUARD_NOW_SANSO);
				if (m_nMotionType[0] != MOTION_GUARD
					&& m_nMotionType[1] != MOTION_GUARD)
				{
					m_nKey[0] = 0;
					m_nKey[1] = 0;
					m_nMotionType[0] = MOTION_GUARD;
					m_nMotionType[1] = MOTION_GUARD;
				}
			}
			if (pInputKeyboard->GetRelese(ENEMY_C_BUTTON) == true && m_State == STATE_GUARD ||
				pXInput->GetRelese(XENEMY_X_BUTTON, 1) == true && m_State == STATE_GUARD)
			{
				m_State = STATE_NEUTRAL;
			}
		}
	}

	if (pBattleSys != NULL)
	{
		if (pBattleSys->GetUlt(1) == true)
		{
			if (pPlayer != NULL)
			{
				pPlayer->SetUltDamage(true);
				if (m_Direction == DIRECTION_LEFT)
				{// 左向き
				 // 左に進む
					m_move.x -= sinf(D3DX_PI * 0.5f) * fMoveEnemy;
				}
				else if (m_Direction == DIRECTION_RIGHT)
				{// 右向き
				 // 右に進む
					m_move.x += sinf(D3DX_PI * 0.5f) * fMoveEnemy;
				}
			}
		}
	}

	//ダメージを受けた回数を初期化
	m_DamageCnt = 0;

	return fMoveEnemy;
}

//=============================================================================
// CPUの動作
//=============================================================================
float CEnemy::EnemyCPU(D3DXVECTOR3 pos, float fMoveEnemy)
{
	// 移動処理取得
	CCharacterMove *pCharacterMove;
	pCharacterMove = CManager::GetCharacterMove();
	// プレイヤー取得
	CPlayer *pPlayer = NULL;
	CSansoGauge *pSansoGauge = NULL;
	CBattleSys::ATTACK_TURN Attackturn;

	CManager::MODE mode;
	mode = CManager::GetMode();
	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pSansoGauge = CTutorial::GetSansoGauge();
		Attackturn = CTutorial::GetBatlteSys()->GetAttackTurn();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pSansoGauge = CGame::GetSansoGauge();
		Attackturn = CGame::GetBatlteSys()->GetAttackTurn();
	}

	D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();
	float PosDiff = 0.0f;		//プレイヤーとの距離


	if (m_nActionTime > 0)
	{	//行動時間
		m_nActionTime--;

		if (m_State != STATE_JANKEN && m_State != STATE_NOKOTTA && m_State != STATE_ULT)
		{
			switch (m_CPUAction)
			{
			case CPUACTION_WALK:
				if (m_State == STATE_NEUTRAL && m_bRecovery == false)
				{//通常状態で硬直していない
					if (PlayerPos.x < pos.x)
					{	// 左に進む
						m_move = pCharacterMove->MoveLeft(m_move, fMoveEnemy);
						m_nMotionType[0] = MOTION_SURIASI;
						m_nMotionType[1] = MOTION_SURIASI;
					}
					if (PlayerPos.x > pos.x)
					{	// 右に進む
						m_move = pCharacterMove->MoveRight(m_move, fMoveEnemy);
						m_nMotionType[0] = MOTION_SURIASI;
						m_nMotionType[1] = MOTION_SURIASI;
					}
				}
				break;
			case CPUACTION_TUPPARI:
				//アクションをしているかどうか
				if (m_bAction == false)
				{	//アクションをしている
					m_bAction = true;
					if (mode == CManager::MODE_TUTORIAL)
					{
						CTutorial::GetBatlteSys()->CPUBattle(CPUACTION_TUPPARI);
					}
					else if (mode == CManager::MODE_GAME)
					{
						CGame::GetBatlteSys()->CPUBattle(CPUACTION_TUPPARI);
					}
				}
				break;
			case CPUACTION_GUARD:
				//ガード状態へ
				m_State = STATE_GUARD;
				pSansoGauge->SetSansoGaugeRightLeft(0, GUARD_NOW_SANSO);
				if (m_nMotionType[0] != MOTION_GUARD
					&& m_nMotionType[1] != MOTION_GUARD)
				{
					m_nKey[0] = 0;
					m_nKey[1] = 0;
					m_nMotionType[0] = MOTION_GUARD;
					m_nMotionType[1] = MOTION_GUARD;
				}
				break;
			case CPUACTION_DASHFAR:
				if (m_State == STATE_NEUTRAL && m_bRecovery == false)
				{//通常状態で硬直していない
					if (PlayerPos.x < pos.x)
					{	// 右に進む
						fMoveEnemy = DASH_MOVE;
						m_move = pCharacterMove->MoveRight(m_move, fMoveEnemy);
						m_nMotionType[0] = MOTION_SURIASI;
						m_nMotionType[1] = MOTION_SURIASI;
					}
					if (PlayerPos.x > pos.x)
					{	// 左に進む
						fMoveEnemy = DASH_MOVE;
						m_move = pCharacterMove->MoveLeft(m_move, fMoveEnemy);
						m_nMotionType[0] = MOTION_SURIASI;
						m_nMotionType[1] = MOTION_SURIASI;
					}
				}
				break;
			case CPUACTION_DASHNEAR:
				if (m_State == STATE_NEUTRAL && m_bRecovery == false)
				{//通常状態で硬直していない
					if (PlayerPos.x < pos.x)
					{	// 右に進む
						fMoveEnemy = DASH_MOVE;
						m_move = pCharacterMove->MoveLeft(m_move, fMoveEnemy);
						m_nMotionType[0] = MOTION_SURIASI;
						m_nMotionType[1] = MOTION_SURIASI;
					}
					if (PlayerPos.x > pos.x)
					{	// 左に進む
						fMoveEnemy = DASH_MOVE;
						m_move = pCharacterMove->MoveRight(m_move, fMoveEnemy);
						m_nMotionType[0] = MOTION_SURIASI;
						m_nMotionType[1] = MOTION_SURIASI;
					}
				}
				break;
			case CPUACTION_RENDA:
				if (m_bAction == false)
				{	//アクションをしている
					m_bAction = true;
					if (mode == CManager::MODE_TUTORIAL)
					{
						CTutorial::GetBatlteSys()->PlusCntPushP2(1);
					}
					else if (mode == CManager::MODE_GAME)
					{
						CGame::GetBatlteSys()->PlusCntPushP2(1);
					}
					pSansoGauge->SetSansoGaugeRightLeft(0, -20);
				}
				break;
			case CPUACTION_YORI:
				if (m_bAction == false)
				{	//アクションをしている
					m_bAction = true;
					if (mode == CManager::MODE_TUTORIAL)
					{
						CTutorial::GetBatlteSys()->PlusCntPushP2(1);
						pSansoGauge->SetSansoGaugeRightLeft(0, -20);
						CTutorial::GetBatlteSys()->CPUBattle(CPUACTION_YORI);
					}
					else if (mode == CManager::MODE_GAME)
					{
						CGame::GetBatlteSys()->PlusCntPushP2(1);
						pSansoGauge->SetSansoGaugeRightLeft(0, -20);
						CGame::GetBatlteSys()->CPUBattle(CPUACTION_YORI);
					}
				}
				break;
			case CPUACTION_NAGE:
				if (m_bAction == false)
				{	//アクションをしている
					m_bAction = true;
					if (mode == CManager::MODE_TUTORIAL)
					{
						CTutorial::GetBatlteSys()->PlusCntPushP2(1);
						pSansoGauge->SetSansoGaugeRightLeft(0, -20);
						CTutorial::GetBatlteSys()->CPUBattle(CPUACTION_NAGE);
					}
					else if (mode == CManager::MODE_GAME)
					{
						CGame::GetBatlteSys()->PlusCntPushP2(1);
						pSansoGauge->SetSansoGaugeRightLeft(0, -20);
						CGame::GetBatlteSys()->CPUBattle(CPUACTION_NAGE);
					}
				}
				break;
			case CPUACTION_OSHI:
				if (m_bAction == false)
				{	//アクションをしている
					m_bAction = true;
					if (mode == CManager::MODE_TUTORIAL)
					{
						CTutorial::GetBatlteSys()->PlusCntPushP2(1);
						pSansoGauge->SetSansoGaugeRightLeft(0, -20);
						CTutorial::GetBatlteSys()->CPUBattle(CPUACTION_OSHI);
					}
					else if (mode == CManager::MODE_GAME)
					{
						CGame::GetBatlteSys()->PlusCntPushP2(1);
						pSansoGauge->SetSansoGaugeRightLeft(0, -20);
						CGame::GetBatlteSys()->CPUBattle(CPUACTION_OSHI);
					}
				}
				break;
			}
		}
	}
	else if (m_nActionTime == 0)
	{	//考える時間(フレーム)
		m_nThinkingTime++;
		m_bAction = false;
		if (m_State != STATE_JANKEN && m_State != STATE_NOKOTTA && m_State != STATE_ULT && m_State != STATE_KUMI)
		{
			m_State = STATE_NEUTRAL;
		}
	}

	//タイムが一定になったら何をするか決める
	if (m_nThinkingTime > 1)
	{	//タイムを初期化
		m_nThinkingTime = 0;
		//距離を測る
		PosDiff = PlayerPos.x - pos.x;
		//正規化
		PosDiff = abs(PosDiff);

		//組み状態の時
		if (m_State == STATE_KUMI && m_bRecovery == false)
		{
			if (Attackturn == CBattleSys::ATTACK_TURN_NORMAL)
			{//攻撃のターンが決まっていないときに連打
				m_CPUAction = CPUACTION_RENDA;
				m_nActionTime = 10;
			}
			else if (Attackturn == CBattleSys::ATTACK_TURN_PLAYER2)
			{//自分の攻撃ターンの時に技を仕掛ける
				//if (pSansoGauge->GetSansoRight() > 200)
				{
					int nKumiPattern = rand() % 5;

					if (nKumiPattern < 3)
					{//3以下の時寄りを実行
						m_CPUAction = CPUACTION_YORI;
						m_nActionTime = 20;
					}
					else if(nKumiPattern == 3)
					{//3の時投げを実行
						m_CPUAction = CPUACTION_NAGE;
						m_nActionTime = 10;
					}
					else if (nKumiPattern == 4)
					{//4の時押しを実行
						m_CPUAction = CPUACTION_OSHI;
						m_nActionTime = 10;
					}
				}
			}
		}
		else
		{
			if (PosDiff < 80)
			{//距離が近い時
				m_CPUAction = CPUACTION_TUPPARI;
				m_nActionTime = 30;
				int nNearPattern = rand() % 5;

				if (nNearPattern == 0)
				{
					m_CPUAction = CPUACTION_DASHNEAR;
					m_nActionTime = 30;
				}
			}
			else if (PosDiff > 80)
			{//距離が遠い時
				int nNearPattern = rand() % 2;

				if (nNearPattern == 0)
				{
					m_CPUAction = CPUACTION_WALK;
					m_nActionTime = 30;
				}
				else if (nNearPattern == 1)
				{
					m_CPUAction = CPUACTION_DASHNEAR;
					m_nActionTime = 30;
				}
			}
			//ガードのタイミング
			if (m_DamageCnt > 1)
			{//攻撃を2回受けたら
				if (PosDiff < 120)
				{//相手が近い
					int nGuardPattern = rand() % 3;

					if (nGuardPattern == 0)
					{
						m_CPUAction = CPUACTION_GUARD;
						m_nActionTime = 120;
					}
					else if (nGuardPattern == 1)
					{
						m_CPUAction = CPUACTION_DASHFAR;
						m_nActionTime = 30;
					}
					else
					{
						m_CPUAction = CPUACTION_TUPPARI;
						m_nActionTime = 20;
					}
				}
				//カウントを初期化
				m_DamageCnt = 0;
			}
		}
	}

	PosDiff = PlayerPos.x - pos.x;

	return fMoveEnemy;
}

//=============================================================================
// チュートリアルのエネミー処理
//=============================================================================
float CEnemy::EnemyTutorial(D3DXVECTOR3 pos, float fMoveEnemy)
{
	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();
	// 移動処理取得
	CCharacterMove *pCharacterMove;
	pCharacterMove = CManager::GetCharacterMove();
	// ゲージの取得
	CSansoGauge *pSansoGauge = NULL;

	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{
		pSansoGauge = CTutorial::GetSansoGauge();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pSansoGauge = CGame::GetSansoGauge();
	}

	//通常状態で硬直していない
	if (m_State == STATE_NEUTRAL && m_bRecovery == false)
	{
		if (m_bMotionEnd[0] == true)
		{
			m_nMotionType[0] = MOTION_BATTLE_NEUTRAL;
		}
		if (m_bMotionEnd[1] == true)
		{
			m_nMotionType[1] = MOTION_BATTLE_NEUTRAL;
		}
	}

	//ダメージを受けた回数を初期化
	m_DamageCnt = 0;

	return fMoveEnemy;
}

//=============================================================================
// 敵との衝突
//=============================================================================
void CEnemy::CollisionPlayerAction(void)
{
	bool Hit;

	CManager::MODE mode;
	mode = CManager::GetMode();
	if (mode == CManager::MODE_TUTORIAL)
	{
		Hit = CTutorial::GetHit();
	}
	else if (mode == CManager::MODE_GAME)
	{
		Hit = CGame::GetHit();
	}

	if (Hit == true)
	{
		if (m_State == STATE_NEUTRAL || m_State == STATE_NOKOTTA || m_State == STATE_GUARD)
		{	//組み状態へ
			m_State = STATE_KUMI;
			if (MOTION_BUTIKAMASI == m_nMotionType[0]
				&& MOTION_BUTIKAMASI == m_nMotionType[1])
			{//ぶちかましモーションの時は止める
				m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
			if (MOTION_NAGE != m_nMotionType[0]
				&& MOTION_NAGE != m_nMotionType[1])
			{//投げモーション以外はニュートラル
				m_nMotionType[0] = MOTION_TUKAMI_NEUTRAL;
				m_nMotionType[1] = MOTION_TUKAMI_NEUTRAL;
			}
		}
		if (m_State == STATE_KUMI)
		{ //組み状態なら
			if (MOTION_TUKAMI_AGERU == m_nMotionType[0]
				&& MOTION_TUKAMI_AGERU == m_nMotionType[1]
				&& m_bMotionEnd[0] == true
				&& m_bMotionEnd[1] == true
				|| MOTION_TUKAMI_AGERARERU == m_nMotionType[0]
				&& MOTION_TUKAMI_AGERARERU == m_nMotionType[1]
				&& m_bMotionEnd[0] == true
				&& m_bMotionEnd[1] == true)
			{//つかみ上げ、上げられモーションが終わったときニュートラル
				m_nMotionType[0] = MOTION_TUKAMI_NEUTRAL;
				m_nMotionType[1] = MOTION_TUKAMI_NEUTRAL;
			}
		}
	}
	else if (Hit == false && m_State != STATE_JANKEN && m_State != STATE_NOKOTTA && m_State != STATE_TSUPPARI
			&& m_State != STATE_NAGE && m_State != STATE_ULT && m_State != STATE_GUARD)
	{
		m_State = STATE_NEUTRAL;
	}

}

//=============================================================================
// タイマーの更新
//=============================================================================
void CEnemy::TimerUpdate(void)
{
	//硬直しているとき
	if (m_bRecovery == true)
	{
		m_nRecoveryTime--;
		if (m_nRecoveryTime <= 0)
		{
			m_bRecovery = false;
			m_nRecoveryTime = 0;
		}
	}
	//カウンターしているとき
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
// つっぱりとの当たり判定
//============================================================================
void CEnemy::TsuppariCollision(D3DXVECTOR3 pos)
{
	// プレイヤー取得
	CPlayer *pPlayer = NULL;
	CSansoGauge *pSansoGauge = NULL;
	CBattleSys *pBattleSys = NULL;

	CManager::MODE mode;
	mode = CManager::GetMode();
	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pSansoGauge = CTutorial::GetSansoGauge();
		pBattleSys = CTutorial::GetBatlteSys();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pSansoGauge = CGame::GetSansoGauge();
		pBattleSys = CGame::GetBatlteSys();
	}

	// つっぱりとの当たり判定
	if (pPlayer->GetState() == CPlayer::STATE_TSUPPARI || pPlayer->GetState() == CPlayer::STATE_ULT)
	{
		bool bHit = pPlayer->GetTuppari().Collision(&pos, &D3DXVECTOR3(m_posOld.x, m_posOld.y + 1.0f, m_posOld.z), &m_move, TSUPPARI_COLLISION);
		//つっぱりにあたった
		if (bHit == true)
		{
			if (m_State != STATE_GUARD)
			{
				m_State = STATE_DAMAGE;
				m_DamageCnt++;
			}
			else
			{
				pBattleSys->GuardKnockBack(1);
				pSansoGauge->SetSansoGaugeRightLeft(0, GUARD_SANSO);
				m_State = STATE_GUARD;
			}

			if (mode == CManager::MODE_TUTORIAL)
			{
				CTutorial::SetHit(false);
			}
			else if (mode == CManager::MODE_GAME)
			{
				CGame::SetHit(false);
			}
		}
	}

}

//=============================================================================
// 土俵端かどうか
//============================================================================
void CEnemy::DohyoHaziWhether(D3DXVECTOR3 pos)
{
	if (pos.x < -DOHYO_HAZI_MIN && pos.x > -DOHYO_HAZI_MAX || pos.x > DOHYO_HAZI_MIN && pos.x < DOHYO_HAZI_MAX)
	{
		m_DohyoState = DOHYO_HAZI;

		if (pos.x < -DOHYO_HAZI_MIN && pos.x > -DOHYO_HAZI_MAX)
		{	//左端
			m_DohyoHaziLR = HAZI_LEFT;
		}
		else if (pos.x > DOHYO_HAZI_MIN && pos.x < DOHYO_HAZI_MAX)
		{	//右端
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
// エネミーの入場シーン
//=============================================================================
void CEnemy::EntryEnemy(D3DXVECTOR3 pos, float fMoveEnemy)
{
	// 移動処理取得
	CCharacterMove *pCharacterMove;
	pCharacterMove = CManager::GetCharacterMove();
	D3DXVECTOR3 moveRand = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
		// 左に進む
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

			//塩を投げる
			if (m_nSiomakiCnt > 20)
			{
				if (m_bUse == false)
				{
					for (int nCnt = 0; nCnt < 30; nCnt++)
					{
						//塩
						moveRand.x = sinf((rand() % 628) / 100.0f) * ((rand() % 3 + 1));
						moveRand.y = cosf((rand() % 628) / 20.0f) * ((rand() % 6 + 3));
						moveRand.z = (float)((rand() % 7 + 3));

						CEffect::Create(D3DXVECTOR3(80.0f, 100.0f, 0.0f), D3DXVECTOR3(moveRand.x, moveRand.y, moveRand.z),
							D3DXCOLOR(1, 1, 1, 1), 6, 6, 1, 200, CLoad::TEXTURE_EFFECT_NORMAL000);
					}
					m_bUse = true;

				}
			}

			fMoveEnemy = 0.0f;
			pos.x = 80.0f;
		}
		m_move = pCharacterMove->MoveLeft(m_move, fMoveEnemy * 0.7f);
	}
}

//=============================================================================
// エネミーの向き
//=============================================================================
D3DXVECTOR3 CEnemy::DirectionEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// プレイヤー取得
	CPlayer *pPlayer = NULL;

	CManager::MODE mode;
	mode = CManager::GetMode();
	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
	}

	// 目的の角度
	if (pPlayer != NULL)
	{
		m_fDestAngle = -atan2f((pPlayer->GetPosition().x - sinf(rot.y)) - pos.x, (pPlayer->GetPosition().z - cosf(rot.y)) - pos.z);
	}
	// 差分
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

	//向きの慣性
	m_fDiffAngle = m_fDestAngle - rot.y;

	//角度の設定
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
// エネミーのモーション
//=============================================================================
void CEnemy::UpdateMotion(int nParent)
{
	//サウンド情報を取得
	//CSound *pSound = CManager::GetSound();
	CSound *pSound = CManager::GetSound(2);

	//モーション
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

	//キーが最大数を上回らないように
	if (m_aMotionInfo[m_nMotionType[nParent]][nParent].nNumKey <= m_nKey[nParent])
	{
		m_nKey[nParent] = 0;
	}

	if (m_bDash == true)
	{
		fDivideMotion = 2.0f;
	}

	//モーション更新
	for (int nCntParts = 0; nCntParts < m_nNumParts[nParent]; nCntParts++)
	{
		if (m_apModel[nCntParts][nParent] != NULL)
		{
			//現在のキーを取得
			pKey = &m_pKeyInfo[m_nMotionType[nParent]][nParent][m_nKey[nParent]].aKey[nCntParts];

			NowKey = *pKey;

			//次のキーを取得
			pNextKey = &m_pKeyInfo[m_nMotionType[nParent]][nParent][(m_nKey[nParent] + 1) % m_aMotionInfo[m_nMotionType[nParent]][nParent].nNumKey].aKey[nCntParts];
			//現在のキーから次のキーへの再生フレーム数におけるモーションカウンターの相対値を算出
			fRateMotion = (float)m_nCountFlame[nParent] / (float)(m_pKeyInfo[m_nMotionType[nParent]][nParent][m_nKey[nParent]].nFrame / fDivideMotion);

#if 1
			fPlusData = pNextKey->frotX + NowKey.frotX;
			fMinusData = pNextKey->frotX - NowKey.frotX;
			//値を出す(絶対値)
			fPlusData = abs(fPlusData);
			fMinusData = abs(fMinusData);
			//差分小さいほう
			if (fPlusData < fMinusData)
			{	//現在のキーと次のキーの各要素の差分を算出
				fDiffMotion = pNextKey->frotX - NowKey.frotX;
				//相対値を差分を使って各要素の値を算出
				rotmotion.x = NowKey.frotX + (fDiffMotion * fRateMotion);
			}
			else
			{	//現在のキーと次のキーの各要素の差分を算出
				fDiffMotion = pNextKey->frotX - NowKey.frotX;
				//相対値を差分を使って各要素の値を算出
				rotmotion.x = NowKey.frotX + (fDiffMotion * fRateMotion);
			}

			fPlusData = pNextKey->frotY + NowKey.frotY;
			fMinusData = pNextKey->frotY - NowKey.frotY;
			//値を出す(絶対値)
			fPlusData = abs(fPlusData);
			fMinusData = abs(fMinusData);
			//差分小さいほう
			if (fPlusData < fMinusData)
			{	//現在のキーと次のキーの各要素の差分を算出
				fDiffMotion = pNextKey->frotY - NowKey.frotY;
				//相対値を差分を使って各要素の値を算出
				rotmotion.y = NowKey.frotY + (fDiffMotion * fRateMotion);
			}
			else
			{	//現在のキーと次のキーの各要素の差分を算出
				fDiffMotion = pNextKey->frotY - NowKey.frotY;
				//相対値を差分を使って各要素の値を算出
				rotmotion.y = NowKey.frotY + (fDiffMotion * fRateMotion);
			}

			fPlusData = pNextKey->frotZ + NowKey.frotZ;
			fMinusData = pNextKey->frotZ - NowKey.frotZ;
			//値を出す(絶対値)
			fPlusData = abs(fPlusData);
			fMinusData = abs(fMinusData);
			//差分小さいほう
			if (fPlusData < fMinusData)
			{	//現在のキーと次のキーの各要素の差分を算出
				fDiffMotion = pNextKey->frotZ - NowKey.frotZ;
				//相対値を差分を使って各要素の値を算出
				rotmotion.z = NowKey.frotZ + (fDiffMotion * fRateMotion);
			}
			else
			{	//現在のキーと次のキーの各要素の差分を算出
				fDiffMotion = pNextKey->frotZ - NowKey.frotZ;
				//相対値を差分を使って各要素の値を算出
				rotmotion.z = NowKey.frotZ + (fDiffMotion * fRateMotion);
			}

			//POS
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->fposX - NowKey.fposX;
			//相対値を差分を使って各要素の値を算出
			posmotion.x = NowKey.fposX + (fDiffMotion * fRateMotion);
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->fposY - NowKey.fposY;
			//相対値を差分を使って各要素の値を算出
			posmotion.y = NowKey.fposY + (fDiffMotion * fRateMotion);
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->fposZ - NowKey.fposZ;
			//相対値を差分を使って各要素の値を算出
			posmotion.z = NowKey.fposZ + (fDiffMotion * fRateMotion);

#endif
			//パーツを動かす
			m_apModel[nCntParts][nParent]->Setrot(rotmotion);
			//オフセットの位置を設定
			m_apModel[nCntParts][nParent]->Setpos(D3DXVECTOR3(m_OffSetPos[nCntParts][nParent].x + posmotion.x,
				m_OffSetPos[nCntParts][nParent].y + posmotion.y,
				m_OffSetPos[nCntParts][nParent].z + posmotion.z));
		}
	}

	//ループの判定
	switch (m_aMotionInfo[m_nMotionType[nParent]][nParent].bLoop)
	{
	case true:
		//ループする
		//フレームを進める
		m_nCountFlame[nParent]++;
		//キーの更新
		if (m_nCountFlame[nParent] >= m_pKeyInfo[m_nMotionType[nParent]][nParent][m_nKey[nParent]].nFrame / fDivideMotion)
		{
			if (m_aMotionInfo[m_nMotionType[nParent]][nParent].nNumKey - 1 == m_nKey[nParent])
			{//キーの初期化
				m_nKey[nParent] = 0;
			}
			else
			{//キーの更新
				m_nKey[nParent] += 1;
			}
			m_nCountFlame[nParent] = 0;
		}

		break;
	case false:
		//ループしない
		if (m_aMotionInfo[m_nMotionType[nParent]][nParent].nNumKey - 1 > m_nKey[nParent])
		{//フレームを進める
			m_nCountFlame[nParent]++;
		}
		else if (m_aMotionInfo[m_nMotionType[nParent]][nParent].nNumKey - 1 == m_nKey[nParent])
		{
			m_bMotionEnd[nParent] = true;
		}
		//キーの更新
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
// ファイル読み込み
//=============================================================================
void CEnemy::FileLoad(char FileName[256], int nParent)
{
	//デバイスを取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 位置取得

	D3DXVECTOR3 pos;
	pos = CSceneX::GetPosition();
	D3DXVECTOR3 rot;
	rot = CSceneX::GetRot();

	//ファイル読み込み用変数
	FILE *pFile;		//ファイルポインタ
	char *pStrcur;		//現在の先頭の文字列
	char aLine[256];	//文字列
	char aStr[256];		//一時保存文字列
	int nIndex = 0;		//現在のインデックス
	int nWord = 0;		//ポップで返された値を保持

	D3DXVECTOR3 ParentPos;	//親の位置情報を取得

	char aTestname[256];
	strcpy(aTestname, FileName);

#if 1
	//ファイルを開く 読み込み
	pFile = fopen(FileName, "r");
	//NULLチェック
	if (pFile != NULL)
	{
		//文字列の先頭を設定
		pStrcur = ReadLine(pFile, &aLine[0]);
		if (memcmp(pStrcur, "SCRIPT", strlen("SCRIPT")) == 0)
		{
			for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
			{
				//文字列の先頭を設定
				pStrcur = ReadLine(pFile, &aLine[0]);
				//文字列を取り出す
				strcpy(aStr, pStrcur);

				//文字列のデータ 比較する文字列 比較する文字数
				if (memcmp(pStrcur, "NUM_MODEL = ", strlen("NUM_MODEL = ")) == 0)
				{
					//頭出し
					pStrcur += strlen("NUM_MODEL = ");
					//文字列の先頭を設定
					strcpy(aStr, pStrcur);
					//文字列抜き出し
					int nNumModel = atoi(pStrcur);

					for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
					{
						//文字列の先頭を設定
						pStrcur = ReadLine(pFile, &aLine[0]);
						//文字列を取り戻す
						strcpy(aStr, pStrcur);
						if (memcmp(pStrcur, "MODEL_FILENAME = ", strlen("MODEL_FILENAME = ")) == 0)
						{
							//頭出し
							pStrcur += strlen("MODEL_FILENAME = ");

							//文字列を設定
							strcpy(aStr, pStrcur);

							//必要な文字列の最後の文字までの文字数を数える
							int nNullNum = PopString(pStrcur, &aStr[0]);

							//文字列を取り戻す
							strcpy(aStr, pStrcur);

							//最後の文字にNULL文字を入れる
							aStr[nNullNum - 1] = '\0';

							//対象の文字列から抜き出し
							strcpy(&m_aFileNameModel[nCntModel][0], aStr);

							// Xファイルの読み込み
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
					//文字列の先頭を設定
					pStrcur = ReadLine(pFile, &aLine[0]);
					//文字列を取り出す
					strcpy(aStr, pStrcur);
				}


				//文字列のデータ 比較する文字列 比較する文字数
				if (memcmp(pStrcur, "CHARACTERSET", 12) == 0)
				{
					while (1)
					{
						//文字列の先頭を設定
						pStrcur = ReadLine(pFile, &aLine[0]);
						//文字列を取り出す
						strcpy(aStr, pStrcur);
						if (memcmp(pStrcur, "NUM_PARTS = ", strlen("NUM_PARTS = ")) == 0)
						{
							//頭出し
							pStrcur += strlen("NUM_PARTS = ");
							//文字列の先頭を設定
							strcpy(aStr, pStrcur);
							//文字列抜き出し
							m_nNumParts[nParent] = atoi(pStrcur);
						}
						if (memcmp(pStrcur, "PARTSSET", strlen("PARTSSET")) == 0)
						{
							while (1)
							{
								//文字列の先頭を設定
								pStrcur = ReadLine(pFile, &aLine[0]);

								//INDEXを読み込み
								if (memcmp(pStrcur, "INDEX = ", strlen("INDEX = ")) == 0)
								{
									//頭出し
									pStrcur += strlen("INDEX = ");
									//文字列の先頭を設定
									strcpy(aStr, pStrcur);
									//文字列抜き出し
									nIndex = atoi(pStrcur);
								}
								//PARENTを読み込み
								if (memcmp(pStrcur, "PARENT = ", strlen("PARENT = ")) == 0)
								{
									//頭出し
									pStrcur += strlen("PARENT = ");
									//文字列の先頭を設定
									strcpy(aStr, pStrcur);
									//文字列抜き出し
									m_aIndexParent[nIndex][nParent] = atoi(pStrcur);
								}
								//POSを読み込み
								if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
								{
									//頭出し
									pStrcur += strlen("POS = ");
									//文字列の先頭を設定
									strcpy(aStr, pStrcur);

									//文字数を返してもらう
									nWord = PopString(pStrcur, &aStr[0]);
									//文字列変換
									m_aKayOffset[nIndex][nParent].fposX = (float)atof(pStrcur);
									//文字数分進める
									pStrcur += nWord;

									//文字数を返してもらう
									nWord = PopString(pStrcur, &aStr[0]);
									//文字列変換
									m_aKayOffset[nIndex][nParent].fposY = (float)atof(pStrcur);
									//文字数分進める
									pStrcur += nWord;

									//文字数を返してもらう
									nWord = PopString(pStrcur, &aStr[0]);
									//文字列変換
									m_aKayOffset[nIndex][nParent].fposZ = (float)atof(pStrcur);

								}
								//ROTを読み込み
								if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
								{
									//頭出し
									pStrcur += strlen("ROT = ");
									//文字列の先頭を設定
									strcpy(aStr, pStrcur);

									//文字数を返してもらう
									nWord = PopString(pStrcur, &aStr[0]);
									//文字列変換
									m_aKayOffset[nIndex][nParent].frotX = (float)atof(pStrcur);

									//文字数分進める
									pStrcur += nWord;
									//文字数を返してもらう
									nWord = PopString(pStrcur, &aStr[0]);
									//文字列変換
									m_aKayOffset[nIndex][nParent].frotY = (float)atof(pStrcur);

									//文字数分進める
									pStrcur += nWord;
									//文字数を返してもらう
									nWord = PopString(pStrcur, &aStr[0]);
									//文字列変換
									//rotFile[nIndex].z = (float)atof(pStrcur);
									m_aKayOffset[nIndex][nParent].frotZ = (float)atof(pStrcur);


								}
								//パーツセット終了
								else if (memcmp(pStrcur, "END_PARTSSET", strlen("END_PARTSSET")) == 0)
								{

									//モデルを生成	オフセット設定
									m_apModel[nIndex][nParent] = CModel::Create(
										D3DXVECTOR3(m_aKayOffset[nIndex][nParent].fposX,
											m_aKayOffset[nIndex][nParent].fposY,
											m_aKayOffset[nIndex][nParent].fposZ),
										D3DXVECTOR3(m_aKayOffset[nIndex][nParent].frotX,
											m_aKayOffset[nIndex][nParent].frotY,
											m_aKayOffset[nIndex][nParent].frotZ));

									//posを代入
									m_OffSetPos[nIndex][nParent] = m_apModel[nIndex][nParent]->GetPos();

									//モデルを割り当て
									m_apModel[nIndex][nParent]->BindModel(m_nNumMatModel[nIndex][nParent], m_pMeshModel[nIndex][nParent], m_pBuffMatModel[nIndex][nParent]);

									if (m_aIndexParent[nIndex][nParent] == -1)
									{
										//モデルの親を指定
										m_apModel[nIndex][nParent]->SetParent(NULL);
									}
									else
									{
										//モデルの親を指定
										m_apModel[nIndex][nParent]->SetParent(m_apModel[m_aIndexParent[nIndex][nParent]][nParent]);
									}

									break;
								}
							}
						}
						//キャラクターセット終了
						else if (memcmp(pStrcur, "END_CHARACTERSET", strlen("END_CHARACTERSET")) == 0)
						{
							break;
						}
					}
					//文字列の先頭を設定
					pStrcur = ReadLine(pFile, &aLine[0]);
					//文字列を取り出す
					strcpy(aStr, pStrcur);
				}

				//モーション読み込み
				if (memcmp(pStrcur, "MOTIONSET", strlen("MOTIONSET")) == 0)
				{
					//頭出し
					pStrcur += strlen("MOTIONSET");

					while (1)
					{
						//文字列の先頭を設定
						pStrcur = ReadLine(pFile, &aLine[0]);
						//文字列を取り出す
						strcpy(aStr, pStrcur);

						if (memcmp(pStrcur, "LOOP = ", strlen("LOOP = ")) == 0)
						{
							//頭出し
							pStrcur += strlen("LOOP = ");
							//文字列の先頭を設定
							strcpy(aStr, pStrcur);

							switch (atoi(pStrcur))
							{
							case 0:
								//文字列抜き出し
								m_aMotionInfo[nCntMotion][nParent].bLoop = false;
								break;
							case 1:
								//文字列抜き出し
								m_aMotionInfo[nCntMotion][nParent].bLoop = true;
								break;
							}
							//文字列の先頭を設定
							pStrcur = ReadLine(pFile, &aLine[0]);
						}

						if (memcmp(pStrcur, "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
						{
							//頭出し
							pStrcur += strlen("NUM_KEY = ");
							//文字列の先頭を設定
							strcpy(aStr, pStrcur);
							//文字列抜き出し
							m_aMotionInfo[nCntMotion][nParent].nNumKey = atoi(pStrcur);

							//文字列の先頭を設定
							pStrcur = ReadLine(pFile, &aLine[0]);
							//文字列を取り出す
							strcpy(aStr, pStrcur);
						}

						//キーの設定
						for (int nCntKey = 0; nCntKey < m_aMotionInfo[nCntMotion][nParent].nNumKey;)
						{
							if (memcmp(pStrcur, "KEYSET", strlen("KEYSET")) == 0)
							{
								//頭出し
								pStrcur += strlen("KEYSET");
								//文字列の先頭を設定
								strcpy(aStr, pStrcur);
								//文字列の先頭を設定
								pStrcur = ReadLine(pFile, &aLine[0]);

								if (memcmp(pStrcur, "FRAME = ", strlen("FRAME = ")) == 0)
								{
									//頭出し
									pStrcur += strlen("FRAME = ");

									m_aMotionInfo[nCntMotion][nParent].aKayInfo[nCntKey].nFrame = atoi(pStrcur);

									//文字列の先頭を設定
									strcpy(aStr, pStrcur);
									//文字列の先頭を設定
									pStrcur = ReadLine(pFile, &aLine[0]);
								}

								//パーツ分回す
								for (int nCntParts = 0; nCntParts < m_nNumParts[nParent];)
								{
									if (memcmp(pStrcur, "KEY", strlen("KEY")) == 0)
									{
										//文字列の先頭を設定
										pStrcur = ReadLine(pFile, &aLine[0]);

										if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
										{
											//頭出し
											pStrcur += strlen("POS = ");
											//文字列の先頭を設定
											strcpy(aStr, pStrcur);

											//文字数を返してもらう
											nWord = PopString(pStrcur, &aStr[0]);
											//POS.X代入
											m_aMotionInfo[nCntMotion][nParent].aKayInfo[nCntKey].aKey[nCntParts].fposX = (float)atof(pStrcur);
											//文字数分進める
											pStrcur += nWord;

											//文字数を返してもらう
											nWord = PopString(pStrcur, &aStr[0]);
											//POS.Y代入
											m_aMotionInfo[nCntMotion][nParent].aKayInfo[nCntKey].aKey[nCntParts].fposY = (float)atof(pStrcur);
											//文字数分進める
											pStrcur += nWord;

											//文字数を返してもらう
											nWord = PopString(pStrcur, &aStr[0]);
											//POS.Z代入
											m_aMotionInfo[nCntMotion][nParent].aKayInfo[nCntKey].aKey[nCntParts].fposZ = (float)atof(pStrcur);
											//文字列の先頭を設定
											pStrcur = ReadLine(pFile, &aLine[0]);
										}
										//ROTを読み込み
										if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
										{
											//頭出し
											pStrcur += strlen("ROT = ");
											//文字列の先頭を設定
											strcpy(aStr, pStrcur);

											//文字数を返してもらう
											nWord = PopString(pStrcur, &aStr[0]);
											//RotX
											m_aMotionInfo[nCntMotion][nParent].aKayInfo[nCntKey].aKey[nCntParts].frotX = (float)atof(pStrcur);
											//文字数分進める
											pStrcur += nWord;

											//文字数を返してもらう
											nWord = PopString(pStrcur, &aStr[0]);
											//RotY
											m_aMotionInfo[nCntMotion][nParent].aKayInfo[nCntKey].aKey[nCntParts].frotY = (float)atof(pStrcur);
											//文字数分進める
											pStrcur += nWord;

											//文字数を返してもらう
											nWord = PopString(pStrcur, &aStr[0]);
											//RotZ
											m_aMotionInfo[nCntMotion][nParent].aKayInfo[nCntKey].aKey[nCntParts].frotZ = (float)atof(pStrcur);

											//文字列の先頭を設定
											pStrcur = ReadLine(pFile, &aLine[0]);
										}
										if (memcmp(pStrcur, "END_KEY", strlen("END_KEY")) == 0)
										{
											//頭出し
											pStrcur += strlen("END_KEY");
											//文字列の先頭を設定
											strcpy(aStr, pStrcur);
											//文字列の先頭を設定
											pStrcur = ReadLine(pFile, &aLine[0]);
											//パーツのカウントを進める
											nCntParts++;
										}
									}
									else
									{
										//文字列の先頭を設定
										pStrcur = ReadLine(pFile, &aLine[0]);
									}
								}
								if (memcmp(pStrcur, "END_KEYSET", strlen("END_KEYSET")) == 0)
								{
									//文字列の先頭を設定
									pStrcur = ReadLine(pFile, &aLine[0]);
									//カウントを進める
									nCntKey++;
								}
							}
							else
							{
								//文字列の先頭を設定
								pStrcur = ReadLine(pFile, &aLine[0]);
							}

						}
						if (memcmp(pStrcur, "END_MOTIONSET", strlen("END_MOTIONSET")) == 0)
						{
							//モーションの情報をセット
							m_pKeyInfo[nCntMotion][nParent] = &m_aMotionInfo[nCntMotion][nParent].aKayInfo[0];
							break;
						}
					}
				}
				//スクリプトの終わり
				if (memcmp(pStrcur, "END_SCRIPT	", strlen("END_SCRIPT")) == 0)
				{
					break;
				}
			}
		}
	}
	else
	{	//ファイルが開けなかった
		MessageBox(0, "NULLでした", "警告", MB_OK);
	}
	//ファイルを閉じる
	fclose(pFile);
#endif

}

//=============================================================================
//　ファイル読み込み無効文を排除
//=============================================================================
char *CEnemy::ReadLine(FILE *pFile, char *pDst)
{
	while (1)
	{
		//１行分読み込み
		fgets(&pDst[0], 256, pFile);

		//文字列のデータ 比較する文字列 比較する文字数
		if (memcmp(pDst, "#", strlen("#")) == 0)
		{
			pDst += strlen("\n");
		}
		//文字列のデータ 比較する文字列 比較する文字数
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
		//文字列のデータ 比較する文字列 比較する文字数
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
		//文字列のデータ 比較する文字列 比較する文字数
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
//　ファイル読み込み先頭を排除
//=============================================================================
char * CEnemy::GetLineTop(char * pStr)
{
	while (1)
	{
		//文字列のデータ 比較する文字列 比較する文字数
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
		//文字列のデータ 比較する文字列 比較する文字数
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
//　文字数を返す
//=============================================================================
int CEnemy::PopString(char * pStr, char * pDest)
{
	int nWord = 0;

	while (1)
	{	//頭出し
		pStr += 1;
		nWord += 1;
		if (memcmp(pStr, " ", strlen(" ")) == 0)
		{	//頭出し
			pStr = "\0";
			nWord += 1;
			break;
		}
		if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{	//頭出し
			pStr = "\0";
			nWord += strlen("\t");
			break;
		}
		//文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pStr, "\n", strlen("\n")) == 0)
		{
			//頭出し
			nWord += strlen("\n");
			break;
		}
	}
	strcpy(pDest, pStr);
	//文字列の数を返す
	return nWord;
}
