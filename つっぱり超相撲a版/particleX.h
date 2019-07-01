//=============================================================================
//
// パーティクルの処理 [particleX.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _PARTICLEX_H_
#define _PARTICLEX_H_

#include "main.h"
#include "sceneX.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PARTICLE3D		(256)							// パーティクルの数
#define MAX_TEXTURE			(2)								// パーティクルの数

//========================================
// クラスの定義
//========================================
//=====================
// パーティクルクラス
//=====================
class CParticleX : public CSceneX
{
public:
	typedef enum
	{// キー要素
		TYPE_NORMAL = 0,	// 通常状態
		TYPE_UP,			// 上昇状態
		TYPE_DOWN,			// 下降状態
		PARTICLESTATE_MAX			// プレイヤーの種類の総数
	} PARTICLE_TYPE;

	CParticleX();	// コンストラクタ
	~CParticleX();	// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos);	// プレイヤー初期化処理
	void Uninit(void);	// プレイヤー終了処理
	void Update(void);	// プレイヤー更新処理
	void Draw(void);	// プレイヤー描画処理

	static CParticleX *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, int nLife, PARTICLE_TYPE type);	// オブジェクトの生成

	static HRESULT LoadMat(void);				// マテリアル読み込み
	static void UnloadMat(void);				// マテリアル解放

private:
	LPD3DXMESH		m_pMesh;			// メッシュ情報（頂点情報）へのポインタ
	LPD3DXBUFFER		m_pBuffMat;			// マテリアル情報へのポインタ
	DWORD			m_nNumMat;			// マテリアル情報の数
	static LPDIRECT3DTEXTURE9	*m_pTexture;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファへのポインタ
	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	int m_nLife;
	D3DXVECTOR3 m_move;
	PARTICLE_TYPE m_ParticleType;
};

#endif