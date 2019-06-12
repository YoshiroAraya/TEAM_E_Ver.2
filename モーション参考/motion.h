//=============================================================================
//
// アニメーションの処理 [motion.h]
// Author : 荒谷 由朗
//
//=============================================================================
#ifndef _MOTION_H_
#define _MOTION_H_

#include "main.h"

//=============================================================================
// モーションの状態
//=============================================================================
typedef enum
{
	MOTIONSTATE_NEUTRAL = 0,
	MOTIONSTATE_MOVE,
	MOTIONSTATE_STOP,
	MOTIONSTATE_MAX
}MOTIONSTATE;

//=============================================================================
// パーツの構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3 Rot;			// 現在の向き
	D3DXVECTOR3 RotDest;		// 目標の向き
	D3DXVECTOR3 fRotAngle;		// 向きの差分
	D3DXVECTOR3 Pos;			// 現在の座標
	D3DXVECTOR3 PosDest;		// 目標の座標
	D3DXVECTOR3 fPosAngle;		// 座標の差分
}PartsMotion;

//=============================================================================
// モーションの構造体
//=============================================================================
typedef struct
{
	int			MAX_FLAME;			// フレーム数
	int			KeyFlame;			// キーフレーム
	bool		Loop;				// ループするかどうか
	int			nNumPlayermotion;	// モーションの番号
	int			nCntFlame;			// フレームカウンター
	PartsMotion aParts[256];
	MOTIONSTATE state;
}Motion;

//------------------------------------------------------------------------
// プロトタイプ宣言
//------------------------------------------------------------------------
void	InitMotion		(void);
void	UninitMotion	(void);
void	UpdateMotion	(void);
void	DrawMotion		(void);
void	SetMotion_STATE	(MOTIONSTATE state);
void	SetMotion		(int nNumPlayermotion);
Motion	*GetMotion		(void);
#endif