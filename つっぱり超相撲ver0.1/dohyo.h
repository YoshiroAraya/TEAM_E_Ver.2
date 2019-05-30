//=============================================================================
//
// 土俵処理 [dohyo.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _DOHYO_H_
#define _DOHYO_H_

#include "main.h"
#include "sceneX.h"

//========================================
// マクロ定義
//========================================

//========================================
// クラスの定義
//========================================
//=====================
// 土俵クラス
//=====================
class CDohyo : public CSceneX
{
public:
	CDohyo();	// コンストラクタ
	~CDohyo();	// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos);	// 土俵初期化処理
	void Uninit(void);	// 土俵終了処理
	void Update(void);	// 土俵更新処理
	void Draw(void);	// 土俵描画処理

	static HRESULT LoadModel(void);	// モデル読み込み
	static void UnloadModel(void);	// モデルテクスチャ解放
	static HRESULT LoadMat(void);	// マテリアル読み込み
	static void UnloadMat(void);	// マテリアル解放

	static CDohyo *Create(D3DXVECTOR3 pos);	// オブジェクトの生成

private:
	static LPD3DXMESH		m_pMesh;		// メッシュ情報（頂点情報）へのポインタ
	static LPD3DXBUFFER		m_pBuffMat;		// マテリアル情報へのポインタ
	static DWORD			m_nNumMat;		// マテリアル情報の数
	static LPDIRECT3DTEXTURE9		*m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ

	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
};
#endif
