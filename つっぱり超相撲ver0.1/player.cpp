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
#include "bullet.h"
#include "scene3D.h"
#include "meshField.h"
#include "shadow.h"
#include "game.h"
#include "enemy.h"
#include "characterMove.h"
#include "loadModel.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PLAYER_COLLISION		(D3DXVECTOR3(7.0f, 60.0f, 7.0f))		//プレイヤーの当たり判定
#define DOHYO_HAZI_MAX			(135.0f)
#define DOHYO_HAZI_MIN			(110.0f)
#define DASH_MOVE				(0.9f)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

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
			pPlayer->BindModel(CLoadModel::GetBuffMat(CLoadModel::MODEL_PLAYER), CLoadModel::GetNumMat(CLoadModel::MODEL_PLAYER), CLoadModel::GetMesh(CLoadModel::MODEL_PLAYER));
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

	return S_OK;
}

//=============================================================================
// プレイヤー終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	// 2Dオブジェクト終了処理
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

	// 前のフレームの位置代入
	m_posOld = pos;

	float fMovePlayer = MOVE_PLAYER;	// プレイヤーの移動量を設定

	//ダッシュ設定
	if (pInputKeyboard->GetPress(PLAYER_B_BUTTON) == true ||
		pXInput->GetPress(XPLAYER_B_BUTTON, 1) == true)
	{
		fMovePlayer = DASH_MOVE;
	}

	if (CGame::GetState() == CGame::STATE_GAME)
	{
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

	// 目的の角度
	m_fDestAngle = atan2f((pEnemy->GetPosition().x - sinf(rot.y)) - pos.x, (pEnemy->GetPosition().z - cosf(rot.y)) - pos.z);
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

	pShadow->SetPos(pos);

	// モデルとの当たり判定
	CollisonDohyo(&pos, &D3DXVECTOR3(m_posOld.x, m_posOld.y + 1.0f, m_posOld.z), &m_move, PLAYER_COLLISION);
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

	CSceneX::SetPosition(pos);
	CSceneX::SetRot(rot);

	//つっぱり位置更新
	m_pTuppari->SetPosition(pos);

	//土俵際判定
	if (pos.x < -DOHYO_HAZI_MIN && pos.x > -DOHYO_HAZI_MAX || pos.x > DOHYO_HAZI_MIN && pos.x < DOHYO_HAZI_MAX)
	{
		m_DohyoState = DOHYO_HAZI;
	}
	else
	{
		m_DohyoState = DOHYO_NORMAL;
	}

	if (pos.y <= 0)
	{
		pos.y = 0;
		CSceneX::SetPosition(pos);
		CGame::SetWinner(CGame::WINNER_PLAYER2);
	}

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

	/*for (int nCount = 0; nCount < NUM_VTX; nCount++)
	{
	if (abRight[nCount] == true)
	{
	CDebugProc::Print("nc", nCount, " : 右");
	nCnt++;
	}
	else
	{
	CDebugProc::Print("nc", nCount, " : 左");
	}
	}

	if (m_bRTriangle == true)
	{
	CDebugProc::Print("c", "aの範囲内 : false");
	CDebugProc::Print("c", "bの範囲内 : true");
	}
	else
	{
	CDebugProc::Print("c", "aの範囲内 : true");
	CDebugProc::Print("c", "bの範囲内 : false");
	}*/

	//if (bRTriangle == true)
	//{// 三角形で見たとき
	//	CDebugProc::Print("c", "aの範囲内 : true");
	//	CDebugProc::Print("c", "bの範囲内 : false");
	//}
	//else
	//{
	//	CDebugProc::Print("c", "aの範囲内 : false");
	//	CDebugProc::Print("c", "bの範囲内 : true");
	//}

	/*if (nCnt == NUM_VTX)
	{
	CDebugProc::Print("c", "範囲内");
	}
	else
	{
	CDebugProc::Print("c", "範囲外");
	}*/
#endif
}

//=============================================================================
// プレイヤー描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	// 2Dオブジェクト描画処理
	CSceneX::Draw();
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