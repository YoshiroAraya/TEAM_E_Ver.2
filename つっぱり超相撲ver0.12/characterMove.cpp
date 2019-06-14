//=============================================================================
//
// キャラクターの移動処理 [characterMove.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "characterMove.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "camera.h"
#include "title.h"

//=============================================================================
// キャラクターの移動のコンストラクタ
//=============================================================================
CCharacterMove::CCharacterMove()
{
	// 値をクリア
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0.0f;
	//m_fRot = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCharacterMove::~CCharacterMove()
{
}

//=============================================================================
// オブジェクトの生成処理
//=============================================================================
CCharacterMove *CCharacterMove::Create()
{
	CCharacterMove *pCharacterMove = NULL;

	if (pCharacterMove == NULL)
	{
		// オブジェクトクラスの生成
		pCharacterMove = new CCharacterMove;

		if (pCharacterMove != NULL)
		{
			pCharacterMove->Init();
		}
	}

	return pCharacterMove;
}

//=============================================================================
// バトルシステム初期化処理
//=============================================================================
HRESULT CCharacterMove::Init()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0.0f;
	//m_fRot = 0.0f;

	return S_OK;
}

//=============================================================================
// バトルシステム終了処理
//=============================================================================
void CCharacterMove::Uninit(void)
{
}

//=============================================================================
// 右に進む
//=============================================================================
D3DXVECTOR3 CCharacterMove::MoveRight(D3DXVECTOR3 move, float fSpeed)
{
	// カメラ取得
	CCamera *pCamera;
	pCamera = CManager::GetCamera();
	// カメラの向きを取得
	D3DXVECTOR3 cameraRot;
	cameraRot = pCamera->GetRot();

	move.x -= sinf(cameraRot.y - D3DX_PI * 0.5f) * fSpeed;

	return move;
}

//=============================================================================
// 左に進む
//=============================================================================
D3DXVECTOR3 CCharacterMove::MoveLeft(D3DXVECTOR3 move, float fSpeed)
{
	// カメラ取得
	CCamera *pCamera;
	pCamera = CManager::GetCamera();
	// カメラの向きを取得
	D3DXVECTOR3 cameraRot;
	cameraRot = pCamera->GetRot();

	move.x -= sinf(cameraRot.y + D3DX_PI * 0.5f) * fSpeed;

	return move;
}

//=============================================================================
// キャラ回転
//=============================================================================
void CCharacterMove::CharaTurn(D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, float fRot, float fLength)
{
	// タイトル取得
	CTitle *pTitle;
	pTitle = CManager::GetTitle();

	if (pTitle != NULL)
	{
		if (pTitle->GetState() == CTitle::STATE_CHARASELECT && pTitle->GetTurn() == true)
		{
			fRot = sinf(D3DX_PI + rot->y);
			rot->y -= 0.1f;

			if (rot->y > D3DX_PI)
			{
				rot->y -= D3DX_PI* 2.0f;
			}
			if (rot->y < -D3DX_PI)
			{
				rot->y += D3DX_PI* 2.0f;
			}

			if (fRot >= 0.999f && fRot < 1.0f)
			{
				pTitle->SetTurn(false);
				fRot = 1.0f;
				rot->y = D3DX_PI * -0.5f;
			}
			if (fRot <= -0.999f && fRot > -1.0f)
			{
				pTitle->SetTurn(false);
				fRot = -1.0f;
				rot->y = D3DX_PI * 0.5f;
			}

			pos->x = 0.0f + fRot * fLength;
			pos->z = 0.0f + cosf(D3DX_PI + rot->y) * fLength;
		}
		else if (pTitle->GetState() == CTitle::STATE_CHARASELECT && pTitle->GetTurn() == false)
		{
			if (fRot >= 0.999f && fRot < 1.0f)
			{
				pTitle->SetTurn(false);
				fRot = 1.0f;
				rot->y = D3DX_PI * -0.5f;
			}
			if (fRot <= -0.999f && fRot > -1.0f)
			{
				pTitle->SetTurn(false);
				fRot = -1.0f;
				rot->y = D3DX_PI * 0.5f;
			}

			pos->x = 0.0f + fRot * fLength;
			pos->z = 0.0f + cosf(D3DX_PI + rot->y) * fLength;
		}
	}
}