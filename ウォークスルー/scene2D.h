//=============================================================================
//
// シーン処理 [scene2D.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_NAME		"data/TEXTURE/.png"	// プレイヤーのテクスチャ名
#define BULLET_NAME		"data/TEXTURE/bullet000.png"	// 弾のテクスチャ名
#define MAX_TEX_TYPE	(2)								// テクスチャの種類

//========================================
// クラスの定義
//========================================
//=====================
// オブジェクトクラス
//=====================
class CScene2D : public CScene
{
public:
	CScene2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_SCENE2D);											// コンストラクタ
	~CScene2D();										// デストラクタ

	virtual HRESULT Init(D3DXVECTOR3 pos);				// 2Dオブジェクト初期化処理
	virtual void Uninit(void);							// 2Dオブジェクト終了処理
	virtual void Update(void);							// 2Dオブジェクト更新処理
	virtual void Draw(void);							// 2Dオブジェクト描画処理

	D3DXVECTOR3 GetPosition(void);						// 位置を取得
	void SetPosition(D3DXVECTOR3 pos);					// 位置を設定

	static CScene2D *Create(D3DXVECTOR3 pos);			// オブジェクトの生成

private:
	LPDIRECT3DTEXTURE9		m_pTexture;					// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// 頂点バッファへのポインタ

	D3DXVECTOR3				m_Pos;						// ポリゴンの位置
};

#endif