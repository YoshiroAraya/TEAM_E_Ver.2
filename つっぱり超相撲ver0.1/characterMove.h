//=============================================================================
//
// キャラクターの移動処理 [characterMove.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _CHARACTERMOVE_H_
#define _CHARACTERMOVE_H_

#include "main.h"

//========================================
// マクロ定義
//========================================

//========================================
// クラスの定義
//========================================
//=====================
// バトルシステムクラス
//=====================
class CCharacterMove
{
public:
	typedef enum
	{
		MOVESTATE_RIGHT = 0,
		MOVESTATE_LEFT,
	}MOVESTATE;

	CCharacterMove();	// コンストラクタ
	~CCharacterMove();	// デストラクタ

	static CCharacterMove *Create();	// オブジェクトの生成

	HRESULT Init();		// バトルシステム初期化処理
	void Uninit(void);	// バトルシステム終了処理

	D3DXVECTOR3 MoveLeft(D3DXVECTOR3 move, float fSpeed);	// 右に進む
	D3DXVECTOR3 MoveRight(D3DXVECTOR3 move, float fSpeed);	// 左に進む

private:
	D3DXVECTOR3				m_move;			// 移動量
	D3DXVECTOR3				m_pos;			// 移動量
};
#endif