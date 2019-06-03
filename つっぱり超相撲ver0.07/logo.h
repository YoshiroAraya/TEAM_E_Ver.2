//=============================================================================
//
// ロゴ処理 [logo.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _LOGO_H_
#define _LOGO_H_

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
class CLogo : public CScene
{
public:
	typedef enum
	{
		TYPE_TITLE = 0,
		TYPE_TEST,
	}TYPE;

	CLogo(int nPriority = 6, OBJTYPE objType = OBJTYPE_SCENE2D);											// コンストラクタ
	~CLogo();										// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos);				// 2Dオブジェクト初期化処理
	void Uninit(void);							// 2Dオブジェクト終了処理
	void Update(void);							// 2Dオブジェクト更新処理
	void Draw(void);							// 2Dオブジェクト描画処理

	static HRESULT Load(void);
	static void Unload(void);

	static CLogo *Create(D3DXVECTOR3 pos, float fWidth, float fHeight, TYPE type);			// オブジェクトの生成

private:
	static LPDIRECT3DTEXTURE9	*m_pTexture;					// テクスチャへのポインタ
	static const char *m_apFilename[];
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// 頂点バッファへのポインタ

	D3DXVECTOR3				m_Pos;						// ポリゴンの位置
	float					m_fSpin;				// 回転
	float					m_fWidth, m_fHeight;	// 幅高さ
	float					m_fRight, m_fLeft;		// 左右の長さ
	float					m_fLength;				// 大きさ
	float					m_fScale;				// 大きさ変更
	D3DXCOLOR				m_Col;
	TYPE m_type;

};

#endif