//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author : 目黒 未来也
//
//=============================================================================
#include "enemy.h"
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

//=============================================================================
// マクロ定義
//=============================================================================
#define ENEMY_COLLISION		(D3DXVECTOR3(7.0f, 60.0f, 7.0f))		//エネミーの当たり判定

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPD3DXMESH CEnemy::m_pMesh = NULL;			// メッシュ情報（頂点情報）へのポインタ
LPD3DXBUFFER CEnemy::m_pBuffMat = NULL;	// マテリアル情報へのポインタ
DWORD CEnemy::m_nNumMat = 0;				// マテリアル情報の数

//=============================================================================
// エネミークラスのコンストラクタ
//=============================================================================
CEnemy::CEnemy() : CSceneX(2)
{
	// 値をクリア
	m_pTexture = NULL;						// テクスチャへのポインタ
	m_pVtxBuff = NULL;						// 頂点バッファへのポインタ
	m_bLand = false;					// 右にいるかどうか
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
CEnemy *CEnemy::Create(D3DXVECTOR3 pos)
{
	CEnemy *pEnemy = NULL;

	if (pEnemy == NULL)
	{
		// オブジェクトクラスの生成
		pEnemy = new CEnemy;

		if (pEnemy != NULL)
		{
			pEnemy->BindModel(m_pBuffMat, m_nNumMat, m_pMesh);
			pEnemy->Init(pos);
		}
	}

	return pEnemy;
}

//=============================================================================
// エネミー初期化処理
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos)
{
	// 2Dオブジェクト初期化処理
	CSceneX::Init(pos);

	// オブジェクトの種類の設定
	SetObjType(CScene::OBJTYPE_ENEMY);

	m_move = D3DXVECTOR3(0, 0, 0);
	m_fDestAngle = 0;
	m_fDiffAngle = 0;
	m_bLand = false;					// 右にいるかどうか

	return S_OK;
}

//=============================================================================
// エネミー終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
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
	pShadow = CManager::GetShadow();

	// カメラの向きを取得
	D3DXVECTOR3 cameraRot;
	cameraRot = pCamera->GetRot();

	// 前のフレームの位置代入
	m_posOld = pos;

	float fMoveEnemy = MOVE_ENEMY;	// エネミーの移動量を設定

	if (m_State == STATE_NEUTRAL)
	{
		//任意のキー←
		if (pInputKeyboard->GetPress(DIK_NUMPAD4) == true)
		{
			if (pInputKeyboard->GetPress(DIK_NUMPAD8) == true)
			{//左上移動
				//モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
				m_move.x -= sinf(cameraRot.y + D3DX_PI * 0.75f) * fMoveEnemy;
				m_move.z -= cosf(cameraRot.y + D3DX_PI * 0.75f) * fMoveEnemy;
				m_fDestAngle = (cameraRot.y + D3DX_PI * 0.75f);
			}
			else if (pInputKeyboard->GetPress(DIK_NUMPAD2) == true)
			{//左下移動
				//モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
				m_move.x -= sinf(cameraRot.y + D3DX_PI * 0.25f) * fMoveEnemy;
				m_move.z -= cosf(cameraRot.y + D3DX_PI * 0.25f) * fMoveEnemy;
				m_fDestAngle = (cameraRot.y + D3DX_PI * 0.25f);
			}
			else
			{	//モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
				m_move.x -= sinf(cameraRot.y + D3DX_PI * 0.5f) * fMoveEnemy;
				m_move.z -= cosf(cameraRot.y + D3DX_PI * 0.5f) * fMoveEnemy;
				m_fDestAngle = (cameraRot.y + D3DX_PI * 0.5f);
			}
		}
		//任意のキー→
		else if (pInputKeyboard->GetPress(DIK_NUMPAD6) == true)
		{
			if (pInputKeyboard->GetPress(DIK_NUMPAD8) == true)
			{//右上移動
			 //モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
				m_move.x -= sinf(cameraRot.y - D3DX_PI * 0.75f) * fMoveEnemy;
				m_move.z -= cosf(cameraRot.y - D3DX_PI * 0.75f) * fMoveEnemy;
				m_fDestAngle = (cameraRot.y - D3DX_PI * 0.75f);
			}
			else if (pInputKeyboard->GetPress(DIK_NUMPAD2) == true)
			{//右下移動
			 //モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
				m_move.x -= sinf(cameraRot.y - D3DX_PI * 0.25f) * fMoveEnemy;
				m_move.z -= cosf(cameraRot.y - D3DX_PI * 0.25f) * fMoveEnemy;
				m_fDestAngle = (cameraRot.y - D3DX_PI * 0.25f);
			}
			else
			{	//モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
				m_move.x -= sinf(cameraRot.y - D3DX_PI * 0.5f) * fMoveEnemy;
				m_move.z -= cosf(cameraRot.y - D3DX_PI * 0.5f) * fMoveEnemy;
				m_fDestAngle = (cameraRot.y - D3DX_PI * 0.5f);
			}
		}
		//任意のキー↑
		else if (pInputKeyboard->GetPress(DIK_NUMPAD8) == true)
		{	//モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
			m_move.x += sinf(cameraRot.y) * fMoveEnemy;
			m_move.z += cosf(cameraRot.y) * fMoveEnemy;
			m_fDestAngle = (cameraRot.y + D3DX_PI * 1.0f);
		}
		//任意のキー↓
		else if (pInputKeyboard->GetPress(DIK_NUMPAD2) == true)
		{
			//モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
			m_move.x -= sinf(cameraRot.y) * fMoveEnemy;
			m_move.z -= cosf(cameraRot.y) * fMoveEnemy;
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

	if (pInputKeyboard->GetTrigger(DIK_PERIOD) == true)
	{
		if (m_State == STATE_NEUTRAL)
		{
			m_State = STATE_KUMI;
		}
		else if (m_State == STATE_KUMI)
		{
			m_State = STATE_NEUTRAL;
		}
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
	m_move.y += (0.0f - m_move.y) * 0.5f;

	// メッシュフィールド取得
	CMeshField *pMeshField;
	pMeshField = CManager::GetMeshField();

	pShadow->SetPos(pos);

	// モデルとの当たり判定
	CollisonSceneX(&pos, &D3DXVECTOR3(m_posOld.x, m_posOld.y + 1.0f, m_posOld.z), &m_move, ENEMY_COLLISION);

	CSceneX::SetPosition(pos);
	CSceneX::SetRot(rot);

#ifdef _DEBUG

	int nCnt = 0;

	CDebugProc::Print("cfccfccfc", "エネミーの位置 : x", pos.x, "f", "   y", pos.y, "f", "  z", pos.z, "f");
	CDebugProc::Print("cn", "エネミーの状態 : ", m_State);

#endif
}

//=============================================================================
// エネミー描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	// 2Dオブジェクト描画処理
	CSceneX::Draw();
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
// エネミーのモデル読み込み処理
//=============================================================================
HRESULT CEnemy::LoadModel(void)
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
	D3DXLoadMeshFromX(ENEMY_MODEL_NAME,
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
// エネミーのテクスチャ解放処理
//=============================================================================
void CEnemy::UnloadModel(void)
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
bool CEnemy::GetLand(void)
{
	return m_bLand;
}