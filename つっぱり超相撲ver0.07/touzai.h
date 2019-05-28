//=============================================================================
//
// 東西の処理 [touzai.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _TOUZAI_H_
#define _TOUZAI_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// 前方宣言
//=============================================================================

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TOUZAI				"data/TEXTURE/touzai.png"						

//=========================
// ポーズクラス
//=========================
class CTouzai : public CScene2D
{
public:
	CTouzai();									// コンストラクタ
	~CTouzai();													// デストラクタ

	static HRESULT Load(void);									// テクスチャ読み込み
	static void Unload(void);									// テクスチャ解放

	static CTouzai *Create(D3DXVECTOR3 pos);								// ポーズを生成

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