//=============================================================================
//
// 土俵（円）の処理 [dohyocircle.cpp]
// Author : Atsushi Yamashita
//
//=============================================================================
#include "dohyocircle.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "load.h"
#include <string>
#include <time.h>
#include "tutorial.h"

//============================================================================
//	静的メンバ変数宣言
//============================================================================

//============================================================================
//	マクロ定義
//============================================================================
#define MAX_HEIGHT (20)   //（高さ）
#define MAX_WITH   (10)   //（幅）

#define MAX_SIZE3D (175)  //3Dポリゴンの大きさ
//=============================================================================
//	コンストラクタ
//=============================================================================
CDohyoCircle::CDohyoCircle() : CScene3D(4, CScene::OBJTYPE_DOHYOCIRCLE)
{
	//m_pTexture = NULL;						// テクスチャへのポインタ
	m_pVtxBuff = NULL;						// 頂点バッファへのポインタ

	m_pos = D3DXVECTOR3(0, 0, 0);			// ポリゴンの位置
	m_rot = D3DXVECTOR3(0, 0, 0);			// ポリゴンの向き
	m_move = D3DXVECTOR3(0, 0, 0);			// ポリゴンの動き

}

//=============================================================================
//デストラクタ
//=============================================================================
CDohyoCircle::~CDohyoCircle()
{

}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT CDohyoCircle::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	//テクスチャの貼り付け
	BindTexture(CLoad::GetTexture(CLoad::TEXTURE_DOHYOCIRCLE));

	//サイズの設定
	CScene3D::SetSize(MAX_SIZE3D, MAX_SIZE3D);

	//初期化処理
	CScene3D::Init(m_pos);

	//色の設定
	CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));


	//CScene3D::SetPosition(D3DXVECTOR3(m_pos), m_fWidth, m_fHeight);

	m_fCol = 0.0f;
	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CDohyoCircle::Uninit(void)
{
	//円の終了処理
	CScene3D::Uninit();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CDohyoCircle::Update(void)
{
	CManager *pManager = NULL;
	int nMode = pManager->GetMode();

	if (nMode == CManager::MODE_GAME)
	{
		CGame *pGame = pManager->GetGame();

		D3DXVECTOR3 PlayerPos;
		D3DXVECTOR3 EnemyPos;

		if (pGame->GetPlayer() != NULL)
		{
			PlayerPos = pGame->GetPlayer()->GetPosition();
			EnemyPos = pGame->GetEnemy()->GetPosition();

			float fLengthPlayer;
			float fLengthEnemy;
			//中心から距離を計算する
			fLengthPlayer = sqrt((PlayerPos.x - m_pos.x) * (PlayerPos.x - m_pos.x) +
				(PlayerPos.z - m_pos.z) * (PlayerPos.z - m_pos.z));

			if (fLengthPlayer > GROUND_SIZE)
			{//プレイヤーが距離を超えたら

			 //目的の場所から中心点の角度を求める
				float fAnglePlayer = atan2f(PlayerPos.x - m_pos.x, PlayerPos.z - m_pos.z);
			}
			//プレイヤーの位置を設定
			pGame->GetPlayer()->SetPosition(D3DXVECTOR3(PlayerPos));

			//中心から距離を計算する
			fLengthEnemy = sqrt((EnemyPos.x - m_pos.x) * (EnemyPos.x - m_pos.x) +
				(EnemyPos.z - m_pos.z) * (EnemyPos.z - m_pos.z));
			if (fLengthEnemy > GROUND_SIZE)
			{//エネミーが距離を超えたら
			 //目的の場所から中心点の角度を求める
				float fAngleEnemy = atan2f(EnemyPos.x - m_pos.x, EnemyPos.z - m_pos.z);
			}
			//プレイヤーの位置を設定
			pGame->GetEnemy()->SetPosition(D3DXVECTOR3(EnemyPos));
		}
	}
	else if (nMode == CManager::MODE_TUTORIAL)
	{
		CTutorial *pTuto = pManager->GetTutorial();

		D3DXVECTOR3 PlayerPos;
		D3DXVECTOR3 EnemyPos;

		if (pTuto->GetPlayer() != NULL)
		{
			PlayerPos = pTuto->GetPlayer()->GetPosition();
			EnemyPos = pTuto->GetEnemy()->GetPosition();

			float fLengthPlayer;
			float fLengthEnemy;

			//中心から距離を計算する
			fLengthPlayer = sqrt((PlayerPos.x - m_pos.x) * (PlayerPos.x - m_pos.x) +
				(PlayerPos.z - m_pos.z) * (PlayerPos.z - m_pos.z));

			if (fLengthPlayer > GROUND_SIZE)
			{//プレイヤーが距離を超えたら
				 //目的の場所から中心点の角度を求める
				float fAnglePlayer = atan2f(PlayerPos.x - m_pos.x, PlayerPos.z - m_pos.z);
			}
			//プレイヤーの位置を設定
			pTuto->GetPlayer()->SetPosition(D3DXVECTOR3(PlayerPos));
			//中心から距離を計算する
			fLengthEnemy = sqrt((EnemyPos.x - m_pos.x) * (EnemyPos.x - m_pos.x) +
				(EnemyPos.z - m_pos.z) * (EnemyPos.z - m_pos.z));

			if (fLengthEnemy > GROUND_SIZE)
			{//エネミーが距離を超えたら
			 //目的の場所から中心点の角度を求める
				float fAngleEnemy = atan2f(EnemyPos.x - m_pos.x, EnemyPos.z - m_pos.z);
			}
			//プレイヤーの位置を設定
			pTuto->GetEnemy()->SetPosition(D3DXVECTOR3(EnemyPos));
		}
	}
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CDohyoCircle::Draw(void)
{
	//描画処理
	CScene3D::Draw();

}

//=============================================================================
//円の生成処理
//=============================================================================
CDohyoCircle *CDohyoCircle::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CDohyoCircle *pDohyoCircle = {};

	if (pDohyoCircle == NULL)
	{
		//メモリを動的確保
		pDohyoCircle = new CDohyoCircle;

		if (pDohyoCircle != NULL)
		{//NULLチェック

			pDohyoCircle->m_pos = pos;
			pDohyoCircle->m_fWidth = fWidth;
			pDohyoCircle->m_fHeight = fHeight;

			//初期化処理
			pDohyoCircle->Init();
		}
		else
		{
			MessageBox(0, "NULLでした", "警告", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "NULLじゃないです", "警告", MB_OK);
	}
	return pDohyoCircle;
}