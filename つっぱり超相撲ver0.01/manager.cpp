//=============================================================================
//
// マネージャ処理 [manager.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "manager.h"
#include "main.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneX.h"
#include "billboard.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "debugProc.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "meshField.h"
#include "shadow.h"
#include "mask.h"
#include "dohyo.h"
#include "BattleSystem.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CCamera *CManager::m_pCamera = NULL;
CLight *CManager::m_pLight = NULL;
CDebugProc *CManager::m_pDebugProc = NULL;
CScene3D *CManager::m_pScene3D = NULL;
CPlayer *CManager::m_pPlayer = NULL;
CEnemy *CManager::m_pEnemy = NULL;
CShadow *CManager::m_pShadow = NULL;
CMeshField *CManager::m_pMeshField = NULL;
CMask *CManager::m_pMask = NULL;
CBattleSys *CManager::m_pBatlteSys = NULL;

//=============================================================================
// マネージャクラスのコンストラクタ
//=============================================================================
CManager::CManager()
{
	m_fData = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager()
{
}

//=============================================================================
// マネージャ初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	m_fData = 0.0f;

	if (m_pRenderer == NULL)
	{
		// レンダリングクラスの生成
		m_pRenderer = new CRenderer;

		if (m_pRenderer != NULL)
		{
			// 初期化処理
			if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
			{
				return -1;
			}
		}
	}

	if (m_pCamera == NULL)
	{
		// カメラクラスの生成
		m_pCamera = new CCamera;

		if (m_pCamera != NULL)
		{
			m_pCamera->Init();
		}
	}

	if (m_pLight == NULL)
	{
		// ライトクラスの生成
		m_pLight = new CLight;

		if (m_pLight != NULL)
		{
			m_pLight->Init();
		}
	}

	if (m_pInputKeyboard == NULL)
	{
		// 入力クラスの生成
		m_pInputKeyboard = new CInputKeyboard;

		if (m_pInputKeyboard != NULL)
		{
			m_pInputKeyboard->Init(hInstance, hWnd);
		}
	}

	if (m_pMask == NULL)
	{
		m_pMask = CMask::Create();
	}

	if (m_pScene3D == NULL)
	{
		//m_pScene3D = CScene3D::Create(D3DXVECTOR3(200.0f, 0.0f, 0.0f));
	}

	CDohyo::LoadModel();
	CDohyo::LoadMat();
	CDohyo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (m_pPlayer == NULL)
	{// プレイヤー
		CPlayer::LoadModel();

		m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 50.0f, 0.0f));
	}

	if (m_pEnemy == NULL)
	{// エネミー
		CEnemy::LoadModel();

		m_pEnemy = CEnemy::Create(D3DXVECTOR3(0.0f, 50.0f, 0.0f));
	}

#ifdef _DEBUG
	if (m_pDebugProc == NULL)
	{
		// デバック表示クラスの生成
		m_pDebugProc = new CDebugProc;

		if (m_pDebugProc != NULL)
		{
			m_pDebugProc->Init();
		}
	}
#endif

	if (m_pMeshField == NULL)
	{
		// メッシュフィールドの生成
	//	m_pMeshField = CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	// 2Dポリゴンの生成
	//CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH -50.0f, 50.0f, 0.0f));

	// 3Dポリゴンの生成
	//CScene3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 3Dモデル
	//CSceneX::Create(D3DXVECTOR3(0.0f, 25.0f, 0.0f));

	// 弾のテクスチャを読み込む
	CBullet::Load();

	CShadow::Load();

	if (m_pShadow == NULL)
	{
		m_pShadow = CShadow::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (m_pBatlteSys == NULL)
	{
		m_pBatlteSys = CBattleSys::Create();
	}
	return S_OK;
}

//=============================================================================
// マネージャ終了処理
//=============================================================================
void CManager::Uninit(void)
{
	CBullet::Unload();

	CPlayer::UnloadModel();
	CDohyo::UnloadModel();
	CDohyo::UnloadMat();
	CShadow::UnLoad();

	//m_pScene3D = NULL;
	m_pPlayer = NULL;
	m_pEnemy = NULL;
	m_pMeshField = NULL;
	m_pShadow = NULL;
	m_pBatlteSys = NULL;

	if (m_pMask != NULL)
	{// フェードの終了
		m_pMask->Uninit();
		delete m_pMask;
		m_pMask = NULL;
	}

	if (m_pRenderer != NULL)
	{// レンダリングクラスの破棄
		// 終了処理
		m_pRenderer->Uninit();

		// メモリを開放
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	if (m_pCamera != NULL)
	{// カメラクラスの破棄
	 // 終了処理
		m_pCamera->Uninit();

		// メモリを開放
		delete m_pCamera;
		m_pCamera = NULL;
	}

	if (m_pLight != NULL)
	{// ライトクラスの破棄
	 // 終了処理
		m_pLight->Uninit();

		// メモリを開放
		delete m_pLight;
		m_pLight = NULL;
	}

	if (m_pInputKeyboard != NULL)
	{// レンダリングクラスの破棄
		 // 終了処理
		m_pInputKeyboard->Uninit();

		// メモリを開放
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

#ifdef _DEBUG
	if (m_pDebugProc != NULL)
	{// デバック表示クラスの破棄
		// 終了処理
		m_pDebugProc->Uninit();

		// メモリを開放
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}
#endif

	// 全てのオブジェクトを解放
	CScene::ReleseAll();
}

//=============================================================================
// マネージャ更新処理
//=============================================================================
void CManager::Update(void)
{
#ifdef _DEBUG
	// デバック表示を消す
	CDebugProc::ReleseStr();
#endif

	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//if (pInputKeyboard->GetTrigger(DIK_G) == true)
	//{
	//	// 2Dポリゴンの生成
	//	CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + m_fData, SCREEN_HEIGHT / 2, 0.0f));

	//	m_fData += 80.0f;
	//}

	bool bHit = Collision(&m_pPlayer->GetPosition(), 10.0f, &m_pEnemy->GetPosition(), 10.0f);

	if (m_pRenderer != NULL)
	{// レンダラー更新処理
		m_pRenderer->Update();
	}

	if (m_pCamera != NULL)
	{// カメラ更新処理
		m_pCamera->Update();
	}

	if (m_pLight != NULL)
	{// カメラ更新処理
		m_pLight->Update();
	}

	if (m_pInputKeyboard != NULL)
	{// キーボード入力更新処理
		m_pInputKeyboard->Update();
	}

	if (m_pMask != NULL)
	{// フェード更新処理
		m_pMask->Update();
	}

	if (m_pBatlteSys != NULL)
	{
		m_pBatlteSys->Update();
	}
#ifdef _DEBUG
	if (bHit == true)
	{
		CDebugProc::Print("c", "当たっている");
	}
	else
	{
		CDebugProc::Print("c", "当たっていない");
	}
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	if (m_pCamera != NULL)
	{// カメラを設定
		m_pCamera->SetCamera();
	}

	if (m_pRenderer != NULL)
	{// レンダラー描画処理
		m_pRenderer->Draw();
	}
}

//=============================================================================
// ブロックとの当たり判定処理
//=============================================================================
bool CManager::Collision(D3DXVECTOR3 *pos0, float fRadius0, D3DXVECTOR3 *pos1, float fRadius1)
{
	bool bHit = false;	// 当たっていない状態

						// 中心と中心の差を求める
	D3DXVECTOR3 DiffLength = D3DXVECTOR3(pos0->x - pos1->x, pos0->y - pos1->y, pos0->z - pos1->z);

	// 中心から中心のベクトルの長さを算出
	float fLength = sqrtf((DiffLength.x * DiffLength.x) + (DiffLength.y * DiffLength.y) + (DiffLength.z * DiffLength.z));

	if (fLength < fRadius0 + fRadius1 && fLength < fRadius0 + fRadius1 && fLength < fRadius0 + fRadius1)
	{// 長さが半径の和より小さければ当たっている
		bHit = true;
	}

	return bHit;	// ブロックに当たっているかどうかを返す
}

//=============================================================================
// レンダラーの取得
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
// 入力情報の取得
//=============================================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

//=============================================================================
// カメラの取得
//=============================================================================
CCamera *CManager::GetCamera(void)
{
	return m_pCamera;
}

//=============================================================================
// 3Dポリゴンの取得
//=============================================================================
CScene3D *CManager::GetScene3D(void)
{
	return m_pScene3D;
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
CPlayer *CManager::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
CEnemy * CManager::GetEnemy(void)
{
	return m_pEnemy;
}

//=============================================================================
// 影の取得
//=============================================================================
CShadow *CManager::GetShadow(void)
{
	return m_pShadow;
}


//=============================================================================
// メッシュフィールドの取得
//=============================================================================
CMeshField *CManager::GetMeshField(void)
{
	return m_pMeshField;
}

//=============================================================================
// マスクの取得
//=============================================================================
CMask *CManager::GetMask(void)
{
	return m_pMask;
}