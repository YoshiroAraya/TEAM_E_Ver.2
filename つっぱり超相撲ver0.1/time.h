//=============================================================================
//
// 時間の処理 [time.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// 前方宣言
//=============================================================================

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TIME				"data/TEXTURE/time.png"						

//=========================
// ポーズクラス
//=========================
class CTime : public CScene2D
{
public:
	CTime();									// コンストラクタ
	~CTime();													// デストラクタ

	static HRESULT Load(void);									// テクスチャ読み込み
	static void Unload(void);									// テクスチャ解放

	static CTime *Create(D3DXVECTOR3 pos);								// ポーズを生成

	HRESULT Init(D3DXVECTOR3 pos);	// ポーズ初期化処理
	void Uninit(void);											// ポーズ終了処理
	void Update(void);											// ポーズ更新処理
	void Draw(void);											// ポーズ描画処理

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXCOLOR m_Col;
	int	m_nSelect;
	float m_fScale;
	bool m_bReset;
};
#endif