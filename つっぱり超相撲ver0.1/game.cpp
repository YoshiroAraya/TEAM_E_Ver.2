//=============================================================================
//
// メイン処理 [game.cpp]
// Author :
//
//=============================================================================
#include "game.h"
#include "manager.h"
#include "light.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "scene3D.h"
#include "game.h"
#include "meshField.h"
#include "dohyo.h"
#include "shadow.h"
#include "bullet.h"
#include "debugProc.h"
#include "scene.h"
#include "fade.h"
#include "input.h"
#include "BattleSystem.h"
#include "customer.h"
#include "dohyocircle.h"
#include <time.h>
#include "gauge.h"
#include "effect.h"
#include "jankenUI.h"
#include "field.h"
#include "wall.h"
#include "touzai.h"
#include "animation.h"
#include "loadModel.h"

//============================================================================
//	マクロ定義
//============================================================================
#define SIZE_X (SCREEN_WIDTH)
#define SIZE_Y (SCREEN_HEIGHT)
#define COLISIONSIZE (10.0f)

//============================================================================
//静的メンバ変数宣言
//============================================================================
CScene3D *CGame::m_pScene3D = NULL;
CPlayer *CGame::m_pPlayer = NULL;
CEnemy *CGame::m_pEnemy = NULL;
CShadow *CGame::m_pShadow = NULL;
CMeshField *CGame::m_pMeshField = NULL;
CBattleSys *CGame::m_pBatlteSys = NULL;
CGauge *CGame::m_pGauge = NULL;

bool CGame::m_bHit = false;
CGame::STATE CGame::m_State = CGame::STATE_START;
CGame::WINNER CGame::m_Winner = CGame::WINNER_NONE;

//=============================================================================
//	コンストラクタ
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
//デストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
void CGame::Init(void)
{
	m_bHit = false;
	m_bUI = true;
	m_State = STATE_START;
	m_Winner = WINNER_NONE;
	//インスタンス
	CManager *pManager = NULL;

	CLoadModel::Load();

	CEffect::Load();
	//CDohyo::LoadModel();
	CDohyo::LoadMat();
	CDohyo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));



	CGauge::Load();
	m_pGauge = CGauge::Create(D3DXVECTOR3(100, 80, 0));

	int nCntZ;
	int nCnt;

	for (nCntZ = 0; nCntZ < 2; nCntZ++)
	{//奥客
		for (nCnt = 0; nCnt < 3; nCnt++)
		{//客

			CCustomer::Create(D3DXVECTOR3(-140.0f + (nCnt * 160.0f), 1.0f, 270.0f + (nCntZ * 160.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3);
		}
	}

	for (nCntZ = 0; nCntZ < 2; nCntZ++)
	{//右客
		for (nCnt = 0; nCnt < 3; nCnt++)
		{//客

			CCustomer::Create(D3DXVECTOR3(270.0f + (nCnt * 150.0f), 1.0f, -80.0f + (nCntZ * 160.0f)), D3DXVECTOR3(0.0f, -300.0f, 0.0f), 3);
		}
	}

	for (nCntZ = 0; nCntZ < 2; nCntZ++)
	{//左客
		for (nCnt = 0; nCnt < 3; nCnt++)
		{//客

			CCustomer::Create(D3DXVECTOR3(-580.0f + (nCnt * 150.0f), 1.0f, -80.0f + (nCntZ * 160.0f)), D3DXVECTOR3(0.0f, 300.0f, 0.0f), 3);
		}
	}

	if (m_pScene3D == NULL)
	{
		//m_pScene3D = CScene3D::Create(D3DXVECTOR3(200.0f, 0.0f, 0.0f));
	}

	if (m_pPlayer == NULL)
	{// プレイヤー
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(-100.0f, 50.0f, 0.0f));
	}

	if (m_pEnemy == NULL)
	{// エネミー
		m_pEnemy = CEnemy::Create(D3DXVECTOR3(100.0f, 50.0f, 0.0f));
	}
	if (m_pMeshField == NULL)
	{
		// メッシュフィールドの生成
		//	m_pMeshField = CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	CDohyoCircle::Load();
	CDohyoCircle::Create(D3DXVECTOR3(0, 25.0f, 0), 0.0f, 0.0f);

	CField::Load();
	CField::Create(D3DXVECTOR3(0, -10.0f, 0), 700.0f);

	CWall::Load();
	CWall::Create(D3DXVECTOR3(0, 200.0f, 500), D3DXVECTOR3(300.0f, 0.0f, 0.0f), 200.0f, 700.0f);
	CWall::Create(D3DXVECTOR3(-550, 200.0f, 0), D3DXVECTOR3(300.0f, 300.0f, 0.0f), 200.0f, 700.0f);

	CWall::Create(D3DXVECTOR3(0, 200.0f, -500), D3DXVECTOR3(300.0f, 600.0f, 0.0f), 200.0f, 700.0f);
	CWall::Create(D3DXVECTOR3(550, 200.0f, 0), D3DXVECTOR3(300.0f, 900.0f, 0.0f), 200.0f, 700.0f);

	CAnimation::Load();
	// 2Dポリゴンの生成
	//CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH -50.0f, 50.0f, 0.0f));

	// 3Dポリゴンの生成
	//CScene3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 3Dモデル
	//CSceneX::Create(D3DXVECTOR3(0.0f, 25.0f, 0.0f));

	// 弾のテクスチャを読み込む
	CBullet::Load();

	if (m_pShadow == NULL)
	{
		m_pShadow = CShadow::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (m_pBatlteSys == NULL)
	{
		m_pBatlteSys = CBattleSys::Create();
	}

	CJankenUI::Load();

	CTouzai::Load();
	CTouzai::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != NULL)
	{
		pCamera->Init();
	}
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CGame::Uninit(void)
{
	CBullet::Unload();
	CDohyoCircle::Unload();

	//CDohyo::UnloadModel();
	CDohyo::UnloadMat();
	CGauge::UnLoad();
	CJankenUI::Unload();
	CField::Unload();
	CWall::Unload();
	CTouzai::Unload();
	CAnimation::Unload();
	//m_pScene3D = NULL;
	m_pPlayer = NULL;
	m_pEnemy = NULL;
	m_pMeshField = NULL;
	m_pShadow = NULL;

	if (m_pBatlteSys != NULL)
	{
		//メモリの開放
		delete m_pBatlteSys;
		m_pBatlteSys = NULL;
	}
	if (m_pBatlteSys != NULL)
	{
		//メモリの開放
		delete m_pGauge;
		m_pGauge = NULL;
	}

	CLoadModel::Unload();

		//全ての終了処理
	CScene::ReleseAll();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CGame::Update(void)
{
	CManager *pManager = NULL;
	CFade *pFade = pManager->GetFade();

	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	m_bHit = Collision(&m_pPlayer->GetPosition(), COLISIONSIZE, &m_pEnemy->GetPosition(), COLISIONSIZE);

	CDebugProc::Print("c", "ゲームモード");

	//任意のキー←
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		pFade->SetFade(pManager->MODE_GAME, pFade->FADE_OUT);
	}

	if (m_pBatlteSys != NULL)
	{
		m_pBatlteSys->Update();
	}

	if (m_State == STATE_GAME)
	{
		if (m_bUI == true)
		{// じゃんけんのUIを出す
			CJankenUI::Create(D3DXVECTOR3(200.0f, 400.0f, 0.0f));
			m_bUI = false;
		}
	}

#ifdef _DEBUG
	if (m_bHit == true)
	{
		CDebugProc::Print("c", "当たっている");
	}
	else
	{
		CDebugProc::Print("c", "当たっていない");
	}

	if (m_Winner == WINNER_PLAYER1)
	{
		CDebugProc::Print("c", "プレイヤー1の勝利");
	}
	else if(m_Winner == WINNER_PLAYER2)
	{
		CDebugProc::Print("c", "プレイヤー2の勝利");
	}

	//エフェクト用関数
	D3DXVECTOR3 RotRand;
	D3DXVECTOR3 PosRand;
	D3DXVECTOR3 effectmove =D3DXVECTOR3(0.0f,0.0f,0.0f);

	//任意のキー←
	if (pInputKeyboard->GetTrigger(DIK_J) == true)
	{
		//アニメーションテクスチャの生成
		//(Pos / Rot / Col / Height / Width / UV_U / UV_V / アニメーションスピード / アニメーションの数 /
		//ループするかしないか(0:する/ 1:しない) / 加算合成するかしないか(0:する/ 1:しない))

		CAnimation::Create(D3DXVECTOR3(0, 100.0f, 0), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			100.0f, 100.0f, 0.1666666666666667f, 1.0f, 3, 5, 1, 0);

		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			PosRand.x = (float)(rand() % 100 - 50);
			PosRand.y = (float)(rand() % 100 - 50);


			effectmove.x = sinf(RotRand.y + 1) * 5.5f;
			effectmove.y = sinf(RotRand.y + 1) * 0.1f;
			effectmove.z = RotRand.x * -0.05f;

			//m_effectCol = D3DXCOLOR(1, 1, 1, 1);

			CEffect::Create(D3DXVECTOR3(0 + PosRand.x, 100.0f, 0.0f), effectmove, D3DXCOLOR(1, 1, 1, 1),
				10, 10, 1, 50, CEffect::EFFECTTEX_NORMAL000);
		}
	}

#endif
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// 3Dポリゴンの取得
//=============================================================================
CScene3D *CGame::GetScene3D(void)
{
	return m_pScene3D;
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
CEnemy * CGame::GetEnemy(void)
{
	return m_pEnemy;
}

//=============================================================================
// 影の取得
//=============================================================================
CShadow *CGame::GetShadow(void)
{
	return m_pShadow;
}

//=============================================================================
// メッシュフィールドの取得
//=============================================================================
CMeshField *CGame::GetMeshField(void)
{
	return m_pMeshField;
}

//=============================================================================
// ゲージの取得
//=============================================================================
CGauge *CGame::GetGauge(void)
{
	return m_pGauge;
}

//=============================================================================
// ブロックとの当たり判定処理
//=============================================================================
bool CGame::Collision(D3DXVECTOR3 *pos0, float fRadius0, D3DXVECTOR3 *pos1, float fRadius1)
{
	bool bHit = false;	// 当たっていない状態

	// 中心と中心の差を求める
	D3DXVECTOR3 DiffLength = D3DXVECTOR3(pos0->x - pos1->x, pos0->y - pos1->y, pos0->z - pos1->z);

	// 中心から中心のベクトルの長さを算出
	float fLength = sqrtf((DiffLength.x * DiffLength.x) + (DiffLength.y * DiffLength.y) + (DiffLength.z * DiffLength.z));

	if (fLength < fRadius0 + fRadius1)
	{// 長さが半径の和より小さければ当たっている
		bHit = true;
	}

	return bHit;	// ブロックに当たっているかどうかを返す
}


