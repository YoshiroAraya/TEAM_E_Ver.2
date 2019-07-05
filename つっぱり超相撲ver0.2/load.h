//=============================================================================
//
// モデルの読み込み処理 [load.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _LOAD_H_
#define _LOAD_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//========================================
// クラスの定義
//========================================
//=====================
// オブジェクトクラス
//=====================
class CLoad
{
public:
	typedef enum // cppのやつと数と並びを揃える
	{
		MODEL_DOHYO = 0,	// 土俵
		MODEL_PLAYER,		// 力士1
		MODEL_ENEMY,		// 力士2
		MODEL_CUSTMOR,		// 観客
		MODEL_SHADOW,		// 影
		MODEL_NEWSCASTER,	// ニュースキャスター
	} MODEL;

	typedef enum // cppのやつと数と並びを揃える
	{
		TEXTURE_DOHYOCIRCLE = 0,	// 土俵の円
		TEXTURE_NEWS,				// ニュースの背景
		TEXTURE_FIELD,				// 地面
		TEXTURE_WALL,				// 壁
		TEXTURE_TITLE,				// ロゴ
		TEXTURE_TIME,				// 時間
		TEXTURE_GAUGE,				// ゲージ
		TEXTURE_BUCHIKAMASHI,		// ぶちかまし
		TEXTURE_HENKA,				// 変化
		TEXTURE_TSUPPARI,			// 突っ張り
		TEXTURE_TOUZAI,				// 東西
		TEXTURE_1P,					// 1P VS COM
		TEXTURE_2P,					// 1P VS 2P
		TEXTURE_EFFECT_NORMAL000,	// エフェクト000
		TEXTURE_EFFECT_NORMAL001,	// エフェクト001
		TEXTURE_ANIMATION,			// アニメーション
		TEXTURE_EFFECT_NORMAL002,	// エフェクト002
		TEXTURE_WINNER_UI,			// 勝利持のUI
		TEXTURE_NUMBER_UI,			// 数字のUI
		TEXTURE_PAUSE_CONTINUE,			// ポーズのUI
		TEXTURE_PAUSE_RETRY,			// ポーズのUI
		TEXTURE_PAUSE_QUIT,				// ポーズのUI
		TEXTURE_PAUSE_BG,				// ポーズのBG
		TEXTURE_MODESELECT_CONTINUE,	// モードセレクトのUI
		TEXTURE_MODESELECT_RETRY,		// モードセレクトのUI
		TEXTURE_MODESELECT_QUIT,		// モードセレクトのBG
		TEXTURE_MODESELECT_BG,			// モードセレクトのUI
	} TEXTURE;

	CLoad();		// コンストラクタ
	~CLoad();	// デストラクタ

	static HRESULT LoadModel(void);
	static void UnloadModel(void);
	static HRESULT LoadTex(void);
	static void UnloadTex(void);

	static LPD3DXMESH GetMesh(int nIdx);
	static LPD3DXBUFFER GetBuffMat(int nIdx);
	static DWORD GetNumMat(int nIdx);
	static LPDIRECT3DTEXTURE9 GetTexture(int nIdx);

private:
	static LPDIRECT3DTEXTURE9	*m_pTexture;					// テクスチャへのポインタ
	static LPD3DXMESH		*m_pMesh;		// メッシュ情報（頂点情報）へのポインタ
	static LPD3DXBUFFER		*m_pBuffMat;		// マテリアル情報へのポインタ
	static DWORD			*m_nNumMat;		// マテリアル情報の数
	static const char *m_apModelFilename[];
	static const char *m_apTexFilename[];
};

#endif