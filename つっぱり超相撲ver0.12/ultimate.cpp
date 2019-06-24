//=============================================================================
//
// 必殺技処理 [ultimate.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "ultimate.h"
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
#include "debugProc.h"
#include "scene.h"
#include "fade.h"
#include "input.h"
#include "BattleSystem.h"
#include "customer.h"
#include "dohyocircle.h"
#include <time.h>
#include "gauge.h"
#include "SansoGauge.h"
#include "effect.h"
#include "jankenUI.h"
#include "field.h"
#include "wall.h"
#include "touzai.h"
#include "animation.h"
#include "load.h"
#include "effect3D.h"
#include "particleX.h"

//============================================================================
//	マクロ定義
//============================================================================

//============================================================================
//静的メンバ変数宣言
//============================================================================
CPlayer *CUltimate::m_pPlayer = NULL;
CEnemy *CUltimate::m_pEnemy = NULL;

//=============================================================================
//	コンストラクタ
//=============================================================================
CUltimate::CUltimate()
{
	m_n1P = 0;
	m_n2P = 0;
}

//=============================================================================
//デストラクタ
//=============================================================================
CUltimate::~CUltimate()
{

}

//=============================================================================
// 必殺技の初期化処理
//=============================================================================
void CUltimate::Init(void)
{
	LoadChara();

	CDohyo::LoadMat();
	CParticleX::LoadMat();
	CDohyo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

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

	if (m_pPlayer == NULL)
	{// プレイヤー
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(-150.0f, 40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (m_pEnemy == NULL)
	{// エネミー
		m_pEnemy = CEnemy::Create(D3DXVECTOR3(100.0f, 30.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	CDohyoCircle::Create(D3DXVECTOR3(0, 25.0f, 0), 0.0f, 0.0f);

	CField::Create(D3DXVECTOR3(0, -10.0f, 0), 700.0f);

	CWall::Create(D3DXVECTOR3(0, 200.0f, 500), D3DXVECTOR3(300.0f, 0.0f, 0.0f), 200.0f, 700.0f);
	CWall::Create(D3DXVECTOR3(-550, 200.0f, 0), D3DXVECTOR3(300.0f, 300.0f, 0.0f), 200.0f, 700.0f);

	CWall::Create(D3DXVECTOR3(0, 200.0f, -500), D3DXVECTOR3(300.0f, 600.0f, 0.0f), 200.0f, 700.0f);
	CWall::Create(D3DXVECTOR3(550, 200.0f, 0), D3DXVECTOR3(300.0f, 900.0f, 0.0f), 200.0f, 700.0f);

	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != NULL)
	{
		pCamera->Init();
	}
}

//=============================================================================
// 必殺技の終了処理
//=============================================================================
void CUltimate::Uninit(void)
{
	m_pPlayer = NULL;
	m_pEnemy = NULL;
	CDohyo::UnloadMat();
	CParticleX::UnloadMat();

	//全ての終了処理
	CScene::ReleseAll();
}

//=============================================================================
// 必殺技の更新処理
//=============================================================================
void CUltimate::Update(void)
{
}

//=============================================================================
// 必殺技の描画処理
//=============================================================================
void CUltimate::Draw(void)
{

}

//=============================================================================
// 使われているモデルのロード
//=============================================================================
void CUltimate::LoadChara(void)
{
	FILE *pFile = NULL;

	CManager::MODE mode;
	mode = CManager::GetMode();

	int nCharType = 0;

	pFile = fopen("data\\TEXT\\charaSave.txt", "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%d", &m_n1P);
		fscanf(pFile, "%d", &m_n2P);

		fclose(pFile);
	}
}