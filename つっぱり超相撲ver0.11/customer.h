//=============================================================================
//
// 土俵処理 [customer.h]
// Author : 山下敦史
//
//=============================================================================
#ifndef _CUSTOMER_H_
#define _CUSTOMER_H_

#include "main.h"
#include "sceneX.h"

//========================================
// マクロ定義
//========================================
#define MAX_CUSTOMERTYPE (4) //客の種類

//========================================
// クラスの定義
//========================================
//=====================
// 土俵クラス
//=====================
class CCustomer : public CSceneX
{
public:
	typedef enum
	{//客の種類
		CUSTOMERTYPE_00 = 0,
		CUSTOMERTYPE_01,
		CUSTOMERTYPE_02,
		CUSTOMERTYPE_03,
	}CUSTOMER;

	CCustomer();	// コンストラクタ
	~CCustomer();	// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// 客初期化処理
	void Uninit(void);				// 客終了処理
	void Update(void);				// 客更新処理
	void Draw(void);				// 客描画処理

	static CCustomer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);	// オブジェクトの生成

private:
	static LPDIRECT3DTEXTURE9		*m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ

	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	CUSTOMER m_nType;
};
#endif
