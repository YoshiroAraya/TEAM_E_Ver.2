//=============================================================================
//
// アニメーションの処理 [motion.cpp]
// Author : 荒谷 由朗
//
//=============================================================================
#include "motion.h"
#include "txt.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MOVE_PARTS (15)			// アニメーションで動くパーツ数

//=============================================================================
// グローバル変数宣言
//=============================================================================
Tex		*g_aTex;				// テキストデータの取得
Player	*pPlayer;				// プレイヤーデータの取得
Motion	g_Motion;
int		nNumMotion = 0;

//=============================================================================
// 初期化処理
//=============================================================================
void InitMotion(void)
{
	// ファイルデータの取得
	g_aTex = GetTex();

	// プレイヤーの取得
	pPlayer = GetPlayer();

	// 初期設定
	g_Motion.nNumPlayermotion	= 0;
	g_Motion.nCntFlame			= 0;
	g_Motion.KeyFlame			= 0;
	g_Motion.MAX_FLAME			= g_aTex->aMotionTex[g_Motion.nNumPlayermotion][g_Motion.KeyFlame][0].MAX_FLAME;
	g_Motion.state				= MOTIONSTATE_MOVE;

	for (int nCntParts = 0; nCntParts < MOVE_PARTS; nCntParts++)
	{// アニメーションするパーツの情報を取得
		g_Motion.aParts[nCntParts].Pos			= D3DXVECTOR3(pPlayer->aModel[nCntParts].pos.x, pPlayer->aModel[nCntParts].pos.y, pPlayer->aModel[nCntParts].pos.z);
		g_Motion.aParts[nCntParts].fPosAngle	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_Motion.aParts[nCntParts].Rot			= D3DXVECTOR3(pPlayer->aModel[nCntParts].rot.x, pPlayer->aModel[nCntParts].rot.y, pPlayer->aModel[nCntParts].rot.z);
		g_Motion.aParts[nCntParts].fRotAngle	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMotion(void)
{// いらない
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMotion(void)
{
	// ファイルデータの取得
	g_aTex = GetTex();

	// プレイヤーの取得
	pPlayer = GetPlayer();

	if (g_Motion.state == MOTIONSTATE_MOVE)
	{
		for (int nCntMotion = 0; nCntMotion < MOVE_PARTS; nCntMotion++)
		{// 回転の差分をだして移動する
			// テキストデータから回転情報を取得する
			g_Motion.aParts[nCntMotion].RotDest = D3DXVECTOR3(g_aTex->aMotionTex[g_Motion.nNumPlayermotion][g_Motion.KeyFlame][nCntMotion].MotionRotX,
				g_aTex->aMotionTex[g_Motion.nNumPlayermotion][g_Motion.KeyFlame][nCntMotion].MotionRotY,
				g_aTex->aMotionTex[g_Motion.nNumPlayermotion][g_Motion.KeyFlame][nCntMotion].MotionRotZ);
			// 差分を計算する
			g_Motion.aParts[nCntMotion].fRotAngle = g_Motion.aParts[nCntMotion].RotDest - g_Motion.aParts[nCntMotion].Rot;		// 差分 = 目的 - 現在

			if (g_Motion.aParts[nCntMotion].fRotAngle.x > D3DX_PI)
			{
				g_Motion.aParts[nCntMotion].fRotAngle.x -= D3DX_PI * 2;
			}
			if (g_Motion.aParts[nCntMotion].fRotAngle.x < -D3DX_PI)
			{
				g_Motion.aParts[nCntMotion].fRotAngle.x += D3DX_PI * 2;
			}

			if (g_Motion.aParts[nCntMotion].fRotAngle.y > D3DX_PI)
			{
				g_Motion.aParts[nCntMotion].fRotAngle.y -= D3DX_PI * 2;
			}
			if (g_Motion.aParts[nCntMotion].fRotAngle.y < -D3DX_PI)
			{
				g_Motion.aParts[nCntMotion].fRotAngle.y += D3DX_PI * 2;
			}

			if (g_Motion.aParts[nCntMotion].fRotAngle.z > D3DX_PI)
			{
				g_Motion.aParts[nCntMotion].fRotAngle.z -= D3DX_PI * 2;
			}
			if (g_Motion.aParts[nCntMotion].fRotAngle.z < -D3DX_PI)
			{
				g_Motion.aParts[nCntMotion].fRotAngle.z += D3DX_PI * 2;
			}

			pPlayer->aModel[nCntMotion].rot += g_Motion.aParts[nCntMotion].fRotAngle / (g_Motion.MAX_FLAME * 1.0f);

			// 更新
			if (pPlayer->aModel[nCntMotion].rot >= g_Motion.aParts[nCntMotion].fRotAngle)
			{
				pPlayer->aModel[nCntMotion].rot += g_Motion.aParts[nCntMotion].fRotAngle / (g_Motion.MAX_FLAME * 1.0f);
			}
			if (pPlayer->aModel[nCntMotion].rot <= g_Motion.aParts[nCntMotion].fRotAngle)
			{
				pPlayer->aModel[nCntMotion].rot += g_Motion.aParts[nCntMotion].fRotAngle / (g_Motion.MAX_FLAME * 1.0f);
			}

			g_Motion.aParts[nCntMotion].Rot = pPlayer->aModel[nCntMotion].rot;		// 現在の更新
		}

		for (int nCntMotion = 0; nCntMotion < MOVE_PARTS; nCntMotion++)
		{// 座標の差分をだして移動する
			// テキストデータから座標情報を取得する
			g_Motion.aParts[nCntMotion].PosDest = D3DXVECTOR3(g_aTex->aMotionTex[g_Motion.nNumPlayermotion][g_Motion.KeyFlame][nCntMotion].MotionPosX,
				g_aTex->aMotionTex[g_Motion.nNumPlayermotion][g_Motion.KeyFlame][nCntMotion].MotionPosY,
				g_aTex->aMotionTex[g_Motion.nNumPlayermotion][g_Motion.KeyFlame][nCntMotion].MotionPosZ);
			// 差分を計算する
			g_Motion.aParts[nCntMotion].fPosAngle.x = g_Motion.aParts[nCntMotion].PosDest.x - g_Motion.aParts[nCntMotion].Pos.x + g_aTex->aModelTex[nCntMotion].PosX;		// 差分 = 目的 - 現在
			g_Motion.aParts[nCntMotion].fPosAngle.y = g_Motion.aParts[nCntMotion].PosDest.y - g_Motion.aParts[nCntMotion].Pos.y + g_aTex->aModelTex[nCntMotion].PosY;
			g_Motion.aParts[nCntMotion].fPosAngle.z = g_Motion.aParts[nCntMotion].PosDest.z - g_Motion.aParts[nCntMotion].Pos.z + g_aTex->aModelTex[nCntMotion].PosZ;

			pPlayer->aModel[nCntMotion].pos += g_Motion.aParts[nCntMotion].fPosAngle / (g_Motion.MAX_FLAME * 1.0f);

			// 更新
			if (pPlayer->aModel[nCntMotion].pos >= g_Motion.aParts[nCntMotion].fPosAngle)
			{
				pPlayer->aModel[nCntMotion].pos += g_Motion.aParts[nCntMotion].fPosAngle / (g_Motion.MAX_FLAME * 1.0f);
			}
			if (pPlayer->aModel[nCntMotion].pos <= g_Motion.aParts[nCntMotion].fPosAngle)
			{
				pPlayer->aModel[nCntMotion].pos += g_Motion.aParts[nCntMotion].fPosAngle / (g_Motion.MAX_FLAME * 1.0f);
			}

			g_Motion.aParts[nCntMotion].Pos = pPlayer->aModel[nCntMotion].pos;		// 現在の更新
		}

		// キーフレームの更新
		if (g_Motion.MAX_FLAME > 0)
		{// !!!	↑これ消したらエラーで落ちます	!!!
			if (g_Motion.nCntFlame % g_Motion.MAX_FLAME == 0)
			{// フレームが指定した分、進んだらキーフレームを次に進める
				if (g_Motion.KeyFlame == g_aTex->KEYFLAME[g_Motion.nNumPlayermotion] - 1)
				{// 最後のキーになったら0に戻す
					g_Motion.KeyFlame = 0;
				}
				else
				{// キーを次に進めてテキストデータから次のフレームを取得する
					g_Motion.KeyFlame++;
					g_Motion.MAX_FLAME = g_aTex->aMotionTex[g_Motion.nNumPlayermotion][g_Motion.KeyFlame][0].MAX_FLAME;
				}
			}
		}
	}
	else if(g_Motion.state == MOTIONSTATE_STOP)
	{
		for (int nCntMotion = 0; nCntMotion < MOVE_PARTS; nCntMotion++)
		{// 回転の差分をだして移動する
		 // テキストデータから回転情報を取得する
			g_Motion.aParts[nCntMotion].RotDest = D3DXVECTOR3(g_aTex->aMotionTex[g_Motion.nNumPlayermotion][g_Motion.KeyFlame][nCntMotion].MotionRotX,
				g_aTex->aMotionTex[g_Motion.nNumPlayermotion][g_Motion.KeyFlame][nCntMotion].MotionRotY,
				g_aTex->aMotionTex[g_Motion.nNumPlayermotion][g_Motion.KeyFlame][nCntMotion].MotionRotZ);
			// 差分を計算する
			g_Motion.aParts[nCntMotion].fRotAngle = g_Motion.aParts[nCntMotion].RotDest - g_Motion.aParts[nCntMotion].Rot;		// 差分 = 目的 - 現在

			if (g_Motion.aParts[nCntMotion].fRotAngle.x > D3DX_PI)
			{
				g_Motion.aParts[nCntMotion].fRotAngle.x -= D3DX_PI * 2;
			}
			if (g_Motion.aParts[nCntMotion].fRotAngle.x < -D3DX_PI)
			{
				g_Motion.aParts[nCntMotion].fRotAngle.x += D3DX_PI * 2;
			}

			if (g_Motion.aParts[nCntMotion].fRotAngle.y > D3DX_PI)
			{
				g_Motion.aParts[nCntMotion].fRotAngle.y -= D3DX_PI * 2;
			}
			if (g_Motion.aParts[nCntMotion].fRotAngle.y < -D3DX_PI)
			{
				g_Motion.aParts[nCntMotion].fRotAngle.y += D3DX_PI * 2;
			}

			if (g_Motion.aParts[nCntMotion].fRotAngle.z > D3DX_PI)
			{
				g_Motion.aParts[nCntMotion].fRotAngle.z -= D3DX_PI * 2;
			}
			if (g_Motion.aParts[nCntMotion].fRotAngle.z < -D3DX_PI)
			{
				g_Motion.aParts[nCntMotion].fRotAngle.z += D3DX_PI * 2;
			}

			pPlayer->aModel[nCntMotion].rot += g_Motion.aParts[nCntMotion].fRotAngle / (g_Motion.MAX_FLAME * 1.0f);

			// 更新
			if (pPlayer->aModel[nCntMotion].rot >= g_Motion.aParts[nCntMotion].fRotAngle)
			{
				pPlayer->aModel[nCntMotion].rot += g_Motion.aParts[nCntMotion].fRotAngle / (g_Motion.MAX_FLAME * 1.0f);
			}
			if (pPlayer->aModel[nCntMotion].rot <= g_Motion.aParts[nCntMotion].fRotAngle)
			{
				pPlayer->aModel[nCntMotion].rot += g_Motion.aParts[nCntMotion].fRotAngle / (g_Motion.MAX_FLAME * 1.0f);
			}

			g_Motion.aParts[nCntMotion].Rot = pPlayer->aModel[nCntMotion].rot;		// 現在の更新
		}

		for (int nCntMotion = 0; nCntMotion < MOVE_PARTS; nCntMotion++)
		{// 座標の差分をだして移動する
		 // テキストデータから座標情報を取得する
			g_Motion.aParts[nCntMotion].PosDest = D3DXVECTOR3(g_aTex->aMotionTex[g_Motion.nNumPlayermotion][g_Motion.KeyFlame][nCntMotion].MotionPosX,
				g_aTex->aMotionTex[g_Motion.nNumPlayermotion][g_Motion.KeyFlame][nCntMotion].MotionPosY,
				g_aTex->aMotionTex[g_Motion.nNumPlayermotion][g_Motion.KeyFlame][nCntMotion].MotionPosZ);
			// 差分を計算する
			g_Motion.aParts[nCntMotion].fPosAngle.x = g_Motion.aParts[nCntMotion].PosDest.x - g_Motion.aParts[nCntMotion].Pos.x + g_aTex->aModelTex[nCntMotion].PosX;		// 差分 = 目的 - 現在
			g_Motion.aParts[nCntMotion].fPosAngle.y = g_Motion.aParts[nCntMotion].PosDest.y - g_Motion.aParts[nCntMotion].Pos.y + g_aTex->aModelTex[nCntMotion].PosY;
			g_Motion.aParts[nCntMotion].fPosAngle.z = g_Motion.aParts[nCntMotion].PosDest.z - g_Motion.aParts[nCntMotion].Pos.z + g_aTex->aModelTex[nCntMotion].PosZ;

			pPlayer->aModel[nCntMotion].pos += g_Motion.aParts[nCntMotion].fPosAngle / (g_Motion.MAX_FLAME * 1.0f);

			// 更新
			if (pPlayer->aModel[nCntMotion].pos >= g_Motion.aParts[nCntMotion].fPosAngle)
			{
				pPlayer->aModel[nCntMotion].pos += g_Motion.aParts[nCntMotion].fPosAngle / (g_Motion.MAX_FLAME * 1.0f);
			}
			if (pPlayer->aModel[nCntMotion].pos <= g_Motion.aParts[nCntMotion].fPosAngle)
			{
				pPlayer->aModel[nCntMotion].pos += g_Motion.aParts[nCntMotion].fPosAngle / (g_Motion.MAX_FLAME * 1.0f);
			}

			g_Motion.aParts[nCntMotion].Pos = pPlayer->aModel[nCntMotion].pos;		// 現在の更新

			// キーフレームの更新
			if (g_Motion.MAX_FLAME > 0)
			{// !!!	↑これ消したらエラーで落ちます	!!!
				if (g_Motion.nCntFlame % g_Motion.MAX_FLAME == 0)
				{// フレームが指定した分、進んだらキーフレームを次に進める
					if (g_Motion.KeyFlame == g_aTex->KEYFLAME[g_Motion.nNumPlayermotion] - 1)
					{// 最後のキーになったら0に戻す
						g_Motion.KeyFlame = g_aTex->KEYFLAME[g_Motion.nNumPlayermotion] - 1;
						//g_Motion.MAX_FLAME = g_aTex->aMotionTex[g_Motion.nNumPlayermotion][g_Motion.KeyFlame][0].MAX_FLAME;
					}
					else
					{// キーを次に進めてテキストデータから次のフレームを取得する
						g_Motion.KeyFlame++;
						g_Motion.MAX_FLAME = g_aTex->aMotionTex[g_Motion.nNumPlayermotion][g_Motion.KeyFlame][0].MAX_FLAME;
					}
				}
			}
		}
	}
	// フレームの加算
	g_Motion.nCntFlame++;

	switch (g_Motion.state)
	{
	case MOTIONSTATE_NEUTRAL:
		break;
	case MOTIONSTATE_MOVE:
		break;
	case MOTIONSTATE_STOP:
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMotion(void)
{// いらない
}

//=============================================================================
// モーションの状態の設定
//=============================================================================
void SetMotion_STATE(MOTIONSTATE state)
{
	g_Motion.state = state;
}

//=============================================================================
// モーションの設定
//=============================================================================
void SetMotion(int nNumPlayermotion)
{
	// モーションの番号
	g_Motion.nNumPlayermotion = nNumPlayermotion;
	// テキストデータから次のフレームを取得する
	g_Motion.MAX_FLAME = g_aTex->aMotionTex[g_Motion.nNumPlayermotion][0][0].MAX_FLAME;

	if (nNumPlayermotion > 0)
	{// 0以外だったらキーフレームを初期化する
		g_Motion.KeyFlame = 0;
	}
}

//=============================================================================
// モーションの情報の取得
//=============================================================================
Motion *GetMotion(void)
{
	return &g_Motion;
}