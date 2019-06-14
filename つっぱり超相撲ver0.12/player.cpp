//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 長山拓実
//
//=============================================================================
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
#include "enemy.h"
#include "characterMove.h"
#include "load.h"
#include "model.h"
#include "title.h"
#include "Banimation.h"
//=============================================================================
// マクロ定義
//=============================================================================
#define PLAYER_COLLISION		(D3DXVECTOR3(7.0f, 60.0f, 7.0f))		//プレイヤーの当たり判定
#define DOHYO_HAZI_MAX			(135.0f)
#define DOHYO_HAZI_MIN			(110.0f)
#define DASH_MOVE				(0.9f)
#define FILE_NAME_0				("data\\TEXT\\motion_Wrestler_down.txt")
#define FILE_NAME_1				("data\\TEXT\\motion_Wrestler_up.txt")



//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPD3DXMESH					CPlayer::m_pMeshModel[MAX_PARTS][MODEL_PARENT] = {};
LPD3DXBUFFER				CPlayer::m_pBuffMatModel[MAX_PARTS][MODEL_PARENT] = {};
LPDIRECT3DTEXTURE9			CPlayer::m_pTextureModel[MAX_PARTS][MODEL_PARENT] = {};
DWORD						CPlayer::m_nNumMatModel[MAX_PARTS][MODEL_PARENT] = {};

//=============================================================================
// プレイヤークラスのコンストラクタ
//=============================================================================
CPlayer::CPlayer() : CSceneX(PLAYER_PRIORITY)
{
	// 値をクリア
	m_pTexture = NULL;						// テクスチャへのポインタ
	m_pVtxBuff = NULL;						// 頂点バッファへのポインタ
	m_bLand = false;					// 右にいるかどうか
	m_bHit = false;					// 右にいるかどうか
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_State = STATE_NEUTRAL;
	m_Direction = DIRECTION_RIGHT;
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

	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
// オブジェクトの生成処理
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{
		// オブジェクトクラスの生成
		pPlayer = new CPlayer;

		if (pPlayer != NULL)
		{
			pPlayer->BindModel(CLoad::GetBuffMat(CLoad::MODEL_PLAYER), CLoad::GetNumMat(CLoad::MODEL_PLAYER), CLoad::GetMesh(CLoad::MODEL_PLAYER));
			pPlayer->Init(pos);
		}
	}

	return pPlayer;
}

//=============================================================================
// プレイヤー初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	// 2Dオブジェクト初期化処理
	CSceneX::Init(pos);

	// オブジェクトの種類の設定
	SetObjType(CScene::OBJTYPE_PLAYER);

	m_move = D3DXVECTOR3(0, 0, 0);
	m_fDestAngle = 0;
	m_fDiffAngle = 0;
	m_bLand = false;					// 右にいるかどうか
	m_bHit = false;					// 右にいるかどうか
	m_State = STATE_JANKEN;
	m_Direction = DIRECTION_RIGHT;
	m_bRecovery = false;	// 硬直フラグ
	m_nRecoveryTime = 0;	// 硬直時間
	m_bJanken = false;
	m_nLife = 100;
	m_bDying = false;
	m_pTuppari = CTuppari::Create(pos);
	m_DohyoState = DOHYO_NORMAL;
	//m_Touzai = HIGASHI;
	m_nCounterTime = 0;
	m_bCounter = false;
	m_DohyoHaziLR = HAZI_NORMAL;
	m_fLength = sqrtf((pos.x - 0.0f) * (pos.x - 0.0f) + (pos.z - 0.0f) * (pos.z - 0.0f));
	//m_turnRot = D3DXVECTOR3(0, 0, 0);
	m_fRot = 0.0f;
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode != NULL)
	{
		if (mode == CManager::MODE_GAME)
		{
			CSceneX::SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		}
		else
		{
			CSceneX::SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		}
	}


	//モーション用変数
	for (int nCntParent = 0; nCntParent < MODEL_PARENT; nCntParent++)
	{
		m_nKey[nCntParent] = 0;			//現在のキー
		m_nCountFlame[nCntParent] = 0;		//現在のフレーム
		m_nMotionType[nCntParent] = 0;	//現在のモーションタイプ
	}

	m_nOldMotion = 0;	//前のモーション
						//プレイヤー情報の読み込み
	FileLoad(FILE_NAME_0, 0);
	FileLoad(FILE_NAME_1, 1);

	//モデルの親を指定
	m_apModel[0][1]->SetParent(m_apModel[0][0]);


	if (mode == CManager::MODE_GAME)
	{//ゲームモードだったら処理に入る
	//オーラ
		//CBAnimation::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f),
			//50.0f, 100.0f, 0.0625f, 1.0f, 1.5f, 16, 0, 0, 0);
	}

	return S_OK;
}

//=============================================================================
// プレイヤー終了処理
//=============================================================================
void CPlayer::Uninit(void)
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

	// 3Dオブジェクト終了処理
	CSceneX::Uninit();
}

//=============================================================================
// プレイヤー更新処理
//=============================================================================
void CPlayer::Update(void)
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

	// 影の取得
	CShadow *pShadow;
	pShadow = CGame::GetShadow();

	// カメラの向きを取得
	D3DXVECTOR3 cameraRot;
	cameraRot = pCamera->GetRot();

	// 敵取得
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

	// 移動処理取得
	CCharacterMove *pCharacterMove;
	pCharacterMove = CManager::GetCharacterMove();

	CManager::MODE mode;
	mode = CManager::GetMode();

	// 前のフレームの位置代入
	m_posOld = pos;

	float fMovePlayer = MOVE_PLAYER;	// プレイヤーの移動量を設定

	if (mode == CManager::MODE_GAME)
	{
		if (CGame::GetState() == CGame::STATE_GAME)
		{
			//ダッシュ設定
			if (pInputKeyboard->GetPress(PLAYER_B_BUTTON) == true ||
				pXInput->GetPress(XPLAYER_B_BUTTON, 1) == true)
			{
				fMovePlayer = DASH_MOVE;
			}

			//通常状態で硬直していない
			if (m_State == STATE_NEUTRAL && m_bRecovery == false)
			{
				//任意のキー←
				if (pInputKeyboard->GetPress(PLAYER_LEFT) == true ||
					pXInput->GetPress(XPLAYER_LEFT, 1) == true)
				{
					// 左に進む
					m_move = pCharacterMove->MoveLeft(m_move, fMovePlayer);
				}

				//任意のキー→
				else if (pInputKeyboard->GetPress(PLAYER_RIGHT) == true ||
					pXInput->GetPress(XPLAYER_RIGHT, 1) == true)
				{
					// 右に進む
					m_move = pCharacterMove->MoveRight(m_move, fMovePlayer);
				}
			}

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
					m_nRecoveryTime = 30;
				}
			}
			// 目的の角度
			m_fDestAngle = -atan2f((pEnemy->GetPosition().x - sinf(rot.y)) - pos.x, (pEnemy->GetPosition().z - cosf(rot.y)) - pos.z);
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
				if (m_State == STATE_NEUTRAL || m_State == STATE_NOKOTTA)
				{
					m_State = STATE_KUMI;
				}
				/*else if (m_State == STATE_KUMI)
				{
				m_State = STATE_NEUTRAL;
				}*/
			}
			else if (CGame::GetHit() == false && m_State != STATE_JANKEN && m_State != STATE_NOKOTTA && m_State != STATE_TSUPPARI)
			{
				m_State = STATE_NEUTRAL;
			}

			// つっぱりとの当たり判定
			if (pEnemy->GetState() == CPlayer::STATE_TSUPPARI)
			{
				bool bHit = pEnemy->GetTuppari().Collision(&pos, &D3DXVECTOR3(m_posOld.x, m_posOld.y + 1.0f, m_posOld.z), &m_move, ENEMY_COLLISION);
				//つっぱりにあたった
				if (bHit == true)
				{
					m_State = STATE_DAMAGE;
					CGame::SetHit(false);
				}
			}

			//つっぱり位置更新
			m_pTuppari->SetPosition(pos);

			//土俵際判定
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
	}
	else if (mode == CManager::MODE_TITLE)
	{
		m_fRot = sinf(D3DX_PI + rot.y);
		pCharacterMove->CharaTurn(&pos, &rot, m_fRot, m_fLength);
	}

	//if (m_Touzai == HIGASHI)
	//{
		if (CCamera::GetState() == CCamera::STATE_HIGASHI)
		{
			// 右に進む
			if (pos.x >= -20.0f)
			{
				fMovePlayer = 0.0f;
				pos.x = -20.0f;
			}

			m_move = pCharacterMove->MoveRight(m_move, fMovePlayer * 0.7f);
		}
	//}
	//else if(m_Touzai == NISHI)
	//{
	//	if (CCamera::GetState() == CCamera::STATE_NISHI)
	//	{
	//		// 左に進む
	//		if (pos.x <= 20.0f)
	//		{
	//			fMovePlayer = 0.0f;
	//			pos.x = 20.0f;
	//		}

	//		m_move = pCharacterMove->MoveLeft(m_move, fMovePlayer * 0.7f);
	//	}
	//}


	if (pInputKeyboard->GetPress(DIK_I) == true)
	{
		// ジャンプ力
		pos.y += 1.0f;
	}
	if (pInputKeyboard->GetPress(DIK_K) == true)
	{
		// ジャンプ力
		pos.y -= 1.0f;
	}

	pos += m_move;

	// 重力加算
	m_move.y -= cosf(D3DX_PI * 0.0f) * 0.5f;

	//減速
	m_move.x += (0.0f - m_move.x) * 0.5f;
	m_move.z += (0.0f - m_move.z) * 0.5f;
	m_move.y += (0.0f - m_move.y) * 0.01f;

	if (pShadow != NULL)
	{
		pShadow->SetPos(pos);
	}

	// モデルとの当たり判定
	CollisonDohyo(&pos, &D3DXVECTOR3(m_posOld.x, m_posOld.y + 1.0f, m_posOld.z), &m_move, PLAYER_COLLISION);

	CSceneX::SetPosition(pos);
	CSceneX::SetRot(rot);

	if (pos.y <= 0)
	{
		pos.y = 0;
		CSceneX::SetPosition(pos);
		CGame::SetWinner(CGame::WINNER_PLAYER2);
	}

	//モーション更新
	UpdateMotion(0);
	UpdateMotion(1);

#ifdef _DEBUG

	int nCnt = 0;

	//CDebugProc::Print("cfccfccfc", "プレイヤーの位置 : x", pos.x, "f", "   y", pos.y, "f", "  z", pos.z, "f");
	//CDebugProc::Print("cfccfccfc", "VtxMax : x", CSceneX::GetVtxMax().x, "f", "   y", CSceneX::GetVtxMax().y, "f", "  z", CSceneX::GetVtxMax().z, "f");
	//CDebugProc::Print("cfccfccfc", "VtxMin : x", CSceneX::GetVtxMin().x, "f", "   y", CSceneX::GetVtxMin().y, "f", "  z", CSceneX::GetVtxMin().z, "f");
	CDebugProc::Print("cn", "プレイヤーの状態 : ", m_State);
	CDebugProc::Print("cn", "プレイヤーの向き : ", m_Direction);
	CDebugProc::Print("cf", "プレイヤーの向き : ", rot.y);
	if (m_bRecovery == true)
	{
		CDebugProc::Print("c", " プレイヤーリカバリー　ON ");
	}
	else
	{
		CDebugProc::Print("c", " プレイヤーリカバリー　OFF ");
	}
	if (m_bCounter == true)
	{
		CDebugProc::Print("c", " プレイヤーカウンター　ON ");
	}
	else
	{
		CDebugProc::Print("c", " プレイヤーカウンター　OFF ");
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
		CDebugProc::Print("c", "プレイヤー 瀕死 ");
	}
	else
	{
		CDebugProc::Print("c", "プレイヤー 生存 ");
	}

	if (pInputKeyboard->GetTrigger(DIK_Q) == true)
	{
		m_nMotionType[0]--;
		m_nMotionType[1]--;
		m_nKey[0] = 0;
		m_nKey[1] = 0;
		m_nCountFlame[0] = 0;
		m_nCountFlame[1] = 0;
	}
	if (pInputKeyboard->GetTrigger(DIK_E) == true)
	{
		m_nMotionType[0]++;
		m_nMotionType[1]++;
		m_nKey[0] = 0;
		m_nKey[1] = 0;
		m_nCountFlame[0] = 0;
		m_nCountFlame[1] = 0;
	}
	CDebugProc::Print("cn", " Numキー0  : ", m_nKey[0]);
	CDebugProc::Print("cn", " フレーム数0  : ", m_nCountFlame[0]);
	CDebugProc::Print("cn", " Numキー1  : ", m_nKey[1]);
	CDebugProc::Print("cn", " フレーム数1 : ", m_nCountFlame[1]);
#endif
}

//=============================================================================
// プレイヤー描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	//デバイスを取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;				// 計算用マトリックス

	// 2Dオブジェクト描画処理
	//CSceneX::Draw();

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
// 土俵との当たり判定の処理
//=============================================================================
void CPlayer::CollisonDohyo(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 radius)
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
bool CPlayer::GetLand(void)
{
	return m_bLand;
}

//=============================================================================
// 状態を取得
//=============================================================================
CPlayer::STATE CPlayer::GetState(void)
{
	return m_State;
}

//=============================================================================
// 状態を設定
//=============================================================================
void CPlayer::SetState(STATE state)
{
	m_State = state;
}

//=============================================================================
// 移動量を設定
//=============================================================================
void CPlayer::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}


//=============================================================================
// プレイヤーのモーション
//=============================================================================
void CPlayer::UpdateMotion(int nParent)
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
			fRateMotion = (float)m_nCountFlame[nParent] / (float)m_pKeyInfo[m_nMotionType[nParent]][nParent][m_nKey[nParent]].nFrame;

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
		if (m_nCountFlame[nParent] >= m_pKeyInfo[m_nMotionType[nParent]][nParent][m_nKey[nParent]].nFrame)
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
			m_bMotionEnd = true;
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
void CPlayer::FileLoad(char FileName[256], int nParent)
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
		printf("ファイルが開けませんでした\n");
	}
	//ファイルを閉じる
	fclose(pFile);
#endif

}

//=============================================================================
//　ファイル読み込み無効文を排除
//=============================================================================
char *CPlayer::ReadLine(FILE *pFile, char *pDst)
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
char * CPlayer::GetLineTop(char * pStr)
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
int CPlayer::PopString(char * pStr, char * pDest)
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
