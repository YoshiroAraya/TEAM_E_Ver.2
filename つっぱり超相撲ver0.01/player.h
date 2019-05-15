//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "sceneX.h"
//========================================
// マクロ定義
//========================================
#define MOVE_PLAYER			(0.1f)							//プレイヤー移動量

//========================================
// クラスの定義
//========================================
//=====================
// プレイヤークラス
//=====================
class CPlayer : public CSceneX
{
public:
	//状態
	typedef enum
	{
		STATE_NEUTRAL = 0,
		STATE_KUMI,
		STATE_NAGE,
		STATE_TSUPPARI,
		STATE_DAMAGE,
		STATE_DOWN,
	}STATE;
	CPlayer();	// コンストラクタ
	~CPlayer();	// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos);	// プレイヤー初期化処理
	void Uninit(void);	// プレイヤー終了処理
	void Update(void);	// プレイヤー更新処理
	void Draw(void);	// プレイヤー描画処理

	void CollisonDohyo(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 radius);						// 当たり判定

	static HRESULT LoadModel(void);	// モデル読み込み
	static void UnloadModel(void);	// モデルテクスチャ解放

	bool GetLand(void);

	static CPlayer *Create(D3DXVECTOR3 pos);	// オブジェクトの生成

private:
	static LPD3DXMESH		m_pMesh;		// メッシュ情報（頂点情報）へのポインタ
	static LPD3DXBUFFER		m_pBuffMat;		// マテリアル情報へのポインタ
	static DWORD			m_nNumMat;		// マテリアル情報の数
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ

	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス

	D3DXVECTOR3				m_move;
	D3DXVECTOR3				m_posOld;
	float					m_fDestAngle;	// 目的の角度
	float					m_fDiffAngle;	// 角度の差分
	bool					m_bLand;		// モデルに乗っているかどうか
	bool					m_bHit;		// 敵に当たっているかどうか	

	STATE				m_State;		// 状態

};
#endif