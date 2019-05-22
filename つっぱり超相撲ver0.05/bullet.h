//=============================================================================
//
// 弾の処理 [bullet.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
#include "billboard.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BULLET	(256)	// 弾の数
#define BULLET_TEXTURENAME		 "data\\TEXTURE\\bullet000.png"			//テクスチャのファイル名
#define TEX_POS_X_INIT			(1.0f)									//テクスチャ座標Uの初期位置
#define TEX_POS_Y_INIT			(1.0f)									//テクスチャ座標Vの初期位置
#define TEX_LEFT				(0.0f)									//テクスチャ座標U左
#define TEX_RIGHT				(1.0f)									//テクスチャ座標U右
#define TEX_TOP					(0.0f)									//テクスチャ座標V上
#define TEX_BOT					(1.0f)									//テクスチャ座標V下

//========================================
// クラスの定義
//========================================
//=====================
// プレイヤークラス
//=====================
class CBullet : public CBillboard
{
public:
	CBullet();	// コンストラクタ
	~CBullet();	// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos);	// プレイヤー初期化処理
	void Uninit(void);	// プレイヤー終了処理
	void Update(void);	// プレイヤー更新処理
	void Draw(void);	// プレイヤー描画処理

	static HRESULT Load(void);	// テクスチャ読み込み
	static void Unload(void);	// テクスチャ解放

	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col);	// オブジェクトの生成

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_move;					// 移動量
	D3DXCOLOR m_col;							// 色
	int m_nLife;
};

#endif