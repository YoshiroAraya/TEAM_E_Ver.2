//=============================================================================
//
// プレイヤー処理 [dohyo.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "customer.h"
#include "input.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "camera.h"
#include "scene3D.h"
#include "meshField.h"
#include "shadow.h"
#include "load.h"
#include "enemy.h"
#include "ultimate.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 *CCustomer::m_pTexture = NULL;				// テクスチャ

#define CUSTOMER_ROT			((rand() % 628) / 100.0f)		//全方向

//=============================================================================
// プレイヤークラスのコンストラクタ
//=============================================================================
CCustomer::CCustomer() : CSceneX(DOHYO_PRIORITY)
{
	// 値をクリア
	m_pVtxBuff = NULL;						// 頂点バッファへのポインタ
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CCustomer::~CCustomer()
{

}

//=============================================================================
// オブジェクトの生成処理
//=============================================================================
CCustomer *CCustomer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, CUSTOMER_POS customerpos)
{
	CCustomer *pCustomer = NULL;

	if (pCustomer == NULL)
	{
		// オブジェクトクラスの生成
		pCustomer = new CCustomer;

		if (pCustomer != NULL)
		{
			pCustomer->BindModel(CLoad::GetBuffMat(nType), CLoad::GetNumMat(nType), CLoad::GetMesh(nType));
			//pCustomer->BindMat(m_pTexture);
			pCustomer->Init(pos, rot);
			pCustomer->m_CustomerPos = customerpos;
		}
	}

	return pCustomer;
}

//=============================================================================
// プレイヤー初期化処理
//=============================================================================
HRESULT CCustomer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// オブジェクト初期化処理
	CSceneX::Init(pos);

	//オブジェクトの向き
	CSceneX::SetRot(rot);

	// オブジェクトの種類の設定
	SetObjType(CScene::OBJTYPE_CUSTOMER);
	m_move = D3DXVECTOR3(sinf((rand() % 628) / 100.0f) * ((rand() % 7 + 1)), -1.0f, cosf((rand() % 628) / 100.0f) * ((rand() % 7 + 1)));
	m_rot = D3DXVECTOR3(sinf(D3DX_PI * CUSTOMER_ROT), cosf(D3DX_PI * CUSTOMER_ROT), cosf(D3DX_PI * CUSTOMER_ROT));

	return S_OK;
}

//=============================================================================
// プレイヤー終了処理
//=============================================================================
void CCustomer::Uninit(void)
{
	// 2Dオブジェクト終了処理
	CSceneX::Uninit();
}

//=============================================================================
// プレイヤー更新処理
//=============================================================================
void CCustomer::Update(void)
{
	CManager::MODE mode;
	mode = CManager::GetMode();

	// 敵取得
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();
	// プレイヤー取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();

	// 位置を取得
	D3DXVECTOR3 pos;
	pos = CSceneX::GetPosition();

	switch (mode)
	{
	case CManager::MODE_GAME:
	/*	if (m_CustomerPos == POSITION_RIGHT)
		{
			if (pEnemy != NULL)
			{
				if (pEnemy->GetWallHit() == true)
				{
					pos -= m_move * 3.5f;

					CSceneX::SetRot(m_rot);
				}
			}
		}*/

		if (pEnemy->GetDying() == true)
		{// 敵が死んだとき
			if (pEnemy->GetDirection() == CEnemy::DIRECTION_LEFT)
			{// 左を向いていた時
				if (m_CustomerPos == POSITION_RIGHT)
				{
					pos -= m_move * 3.5f;

					CSceneX::SetRot(m_rot);
				}
			}
			else if(pEnemy->GetDirection() == CEnemy::DIRECTION_RIGHT)
			{
				if (m_CustomerPos == POSITION_LEFT)
				{
					pos -= m_move * 3.5f;

					CSceneX::SetRot(m_rot);
				}
			}
		}
		break;
	}

	// 位置の設定
	CSceneX::SetPosition(pos);
}

//=============================================================================
// プレイヤー描画処理
//=============================================================================
void CCustomer::Draw(void)
{
	// 2Dオブジェクト描画処理
	CSceneX::Draw();
}