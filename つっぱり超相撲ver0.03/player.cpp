//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "player.h"
#include "input.h"
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

//=============================================================================
// マクロ定義
//=============================================================================
#define PLAYER_COLLISION		(D3DXVECTOR3(7.0f, 60.0f, 7.0f))		//プレイヤーの当たり判定

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPD3DXMESH CPlayer::m_pMesh = NULL;			// メッシュ情報（頂点情報）へのポインタ
LPD3DXBUFFER CPlayer::m_pBuffMat = NULL;	// マテリアル情報へのポインタ
DWORD CPlayer::m_nNumMat = 0;				// マテリアル情報の数

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
			pPlayer->BindModel(m_pBuffMat, m_nNumMat, m_pMesh);
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
	m_State = STATE_NEUTRAL;

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

	// 前のフレームの位置代入
	m_posOld = pos;

	float fMovePlayer = MOVE_PLAYER;	// プレイヤーの移動量を設定

	if (m_State == STATE_NEUTRAL)
	{
		//任意のキー←
		if (pInputKeyboard->GetPress(DIK_LEFT) == true)
		{
			if (pInputKeyboard->GetPress(DIK_UP) == true)
			{//左上移動
				//モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
				m_move.x -= sinf(cameraRot.y + D3DX_PI * 0.75f) * fMovePlayer;
				m_move.z -= cosf(cameraRot.y + D3DX_PI * 0.75f) * fMovePlayer;
				m_fDestAngle = (cameraRot.y + D3DX_PI * 0.75f);
			}
			else if (pInputKeyboard->GetPress(DIK_DOWN) == true)
			{//左下移動
				//モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
				m_move.x -= sinf(cameraRot.y + D3DX_PI * 0.25f) * fMovePlayer;
				m_move.z -= cosf(cameraRot.y + D3DX_PI * 0.25f) * fMovePlayer;
				m_fDestAngle = (cameraRot.y + D3DX_PI * 0.25f);
			}
			else
			{	//モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
				m_move.x -= sinf(cameraRot.y + D3DX_PI * 0.5f) * fMovePlayer;
				m_move.z -= cosf(cameraRot.y + D3DX_PI * 0.5f) * fMovePlayer;
				m_fDestAngle = (cameraRot.y + D3DX_PI * 0.5f);
			}
		}
		//任意のキー→
		else if (pInputKeyboard->GetPress(DIK_RIGHT) == true)
		{
			if (pInputKeyboard->GetPress(DIK_UP) == true)
			{//右上移動
			 //モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
				m_move.x -= sinf(cameraRot.y - D3DX_PI * 0.75f) * fMovePlayer;
				m_move.z -= cosf(cameraRot.y - D3DX_PI * 0.75f) * fMovePlayer;
				m_fDestAngle = (cameraRot.y - D3DX_PI * 0.75f);
			}
			else if (pInputKeyboard->GetPress(DIK_DOWN) == true)
			{//右下移動
			 //モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
				m_move.x -= sinf(cameraRot.y - D3DX_PI * 0.25f) * fMovePlayer;
				m_move.z -= cosf(cameraRot.y - D3DX_PI * 0.25f) * fMovePlayer;
				m_fDestAngle = (cameraRot.y - D3DX_PI * 0.25f);
			}
			else
			{	//モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
				m_move.x -= sinf(cameraRot.y - D3DX_PI * 0.5f) * fMovePlayer;
				m_move.z -= cosf(cameraRot.y - D3DX_PI * 0.5f) * fMovePlayer;
				m_fDestAngle = (cameraRot.y - D3DX_PI * 0.5f);
			}
		}
		//任意のキー↑
		else if (pInputKeyboard->GetPress(DIK_UP) == true)
		{	//モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
			m_move.x += sinf(cameraRot.y) * fMovePlayer;
			m_move.z += cosf(cameraRot.y) * fMovePlayer;
			m_fDestAngle = (cameraRot.y + D3DX_PI * 1.0f);
		}
		//任意のキー↓
		else if (pInputKeyboard->GetPress(DIK_DOWN) == true)
		{
			//モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
			m_move.x -= sinf(cameraRot.y) * fMovePlayer;
			m_move.z -= cosf(cameraRot.y) * fMovePlayer;
			m_fDestAngle = (cameraRot.y + D3DX_PI * 0.0f);
		}
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

	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
	{// 弾発射
		CBullet::Create(D3DXVECTOR3(pos.x + sinf(rot.y + D3DX_PI) * 10, pos.y, pos.z + cosf(rot.y + D3DX_PI) * 10),
			D3DXVECTOR3(sinf(rot.y + D3DX_PI) * 8.5f, 0, cosf(rot.y + D3DX_PI) * 8.5f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (CGame::GetHit() == true)
	{
		if (m_State == STATE_NEUTRAL)
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
	m_move.y -= cosf(D3DX_PI * 0.0f) * 0.1f;

	//減速
	m_move.x += (0.0f - m_move.x) * 0.5f;
	m_move.z += (0.0f - m_move.z) * 0.5f;
	m_move.y += (0.0f - m_move.y) * 0.01f;

	pShadow->SetPos(pos);

	// モデルとの当たり判定
	CollisonDohyo(&pos, &D3DXVECTOR3(m_posOld.x, m_posOld.y + 1.0f, m_posOld.z), &m_move, PLAYER_COLLISION);

	CSceneX::SetPosition(pos);
	CSceneX::SetRot(rot);

#ifdef _DEBUG

	int nCnt = 0;

	CDebugProc::Print("cfccfccfc", "プレイヤーの位置 : x", pos.x, "f", "   y", pos.y, "f", "  z", pos.z, "f");
	CDebugProc::Print("cfccfccfc", "VtxMax : x", CSceneX::GetVtxMax().x, "f", "   y", CSceneX::GetVtxMax().y, "f", "  z", CSceneX::GetVtxMax().z, "f");
	CDebugProc::Print("cfccfccfc", "VtxMin : x", CSceneX::GetVtxMin().x, "f", "   y", CSceneX::GetVtxMin().y, "f", "  z", CSceneX::GetVtxMin().z, "f");
	CDebugProc::Print("cn", "プレイヤーの状態 : ", m_State);
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
// プレイヤーのモデル読み込み処理
//=============================================================================
HRESULT CPlayer::LoadModel(void)
{
	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}
	// Xファイルの読み込み
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
// プレイヤーのテクスチャ解放処理
//=============================================================================
void CPlayer::UnloadModel(void)
{
	// メッシュの開放
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// マテリアルの開放
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
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