//=============================================================================
//
// 3Dオブジェクト処理 [customer.h]
// Author : 目黒　未来也
//
//=============================================================================
#ifndef _3DOBJECT_H_
#define _3DOBJECT_H_

#include "main.h"
#include "sceneX.h"

//========================================
// マクロ定義
//========================================
#define MAX_3DOBJECTTYPE (1) //オブジェクトの種類

//========================================
// クラスの定義
//========================================
//=====================
// オブジェクトクラス
//=====================
class C3DObject : public CSceneX
{
public:
	typedef enum
	{//オブジェクトの種類
		OBJECTTYPE_TROPHY = 0,
	}OBJECTTYPE;

	C3DObject();	// コンストラクタ
	~C3DObject();	// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// オブジェクト初期化処理
	void Uninit(void);				// オブジェクト終了処理
	void Update(void);				// オブジェクト更新処理
	void Draw(void);				// オブジェクト描画処理

	static C3DObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);	// オブジェクトの生成

private:
	LPDIRECT3DTEXTURE9		*m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ

	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	OBJECTTYPE m_nType;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rot;
	int m_nTypeTex;
	int m_nLife;
};
#endif
