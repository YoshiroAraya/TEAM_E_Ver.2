//=============================================================================
//
// エネミー処理 [player.h]
// Author : 目黒 未来也
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "sceneX.h"

//========================================
// マクロ定義
//========================================
#define MOVE_ENEMY			(0.5f)							//エネミー移動量

//========================================
// クラスの定義
//========================================
//=====================
// エネミークラス
//=====================
class CEnemy : public CSceneX
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

	//向き
	typedef enum
	{
		DIRECTION_RIGHT = 0,
		DIRECTION_LEFT,
	}DIRECTION;

	CEnemy();	// コンストラクタ
	~CEnemy();	// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos);	// エネミー初期化処理
	void Uninit(void);	// エネミー終了処理
	void Update(void);	// エネミー更新処理
	void Draw(void);	// エネミー描画処理

	static HRESULT LoadModel(void);	// モデル読み込み
	static void UnloadModel(void);	// モデルテクスチャ解放

	bool GetLand(void);
	STATE GetState(void);
	void SetState(STATE state);
	DIRECTION GetDirection(void) { return m_Direction; }
	void SetDirection(DIRECTION direction) { m_Direction = direction; }
	bool GetRecovery(void) { return m_bRecovery; }
	void SetRecovery(bool bReco) { m_bRecovery = bReco; }
	int GetRecoveryTime(void) { return m_nRecoveryTime; }
	void SetRecoveryTime(int nReco) { m_nRecoveryTime = nReco; }

	static CEnemy *Create(D3DXVECTOR3 pos);	// オブジェクトの生成

	void CollisonSceneX(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 radius);						// 当たり判定
	void SetMove(D3DXVECTOR3 move);

private:
	static LPD3DXMESH		m_pMesh;		// メッシュ情報（頂点情報）へのポインタ
	static LPD3DXBUFFER		m_pBuffMat;		// マテリアル情報へのポインタ
	static DWORD			m_nNumMat;		// マテリアル情報の数
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ

	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス

	D3DXVECTOR3				m_move;			// ポリゴンの位置
	D3DXVECTOR3				m_posOld;
	float					m_fDestAngle;	// 目的の角度
	float					m_fDiffAngle;	// 角度の差分
	bool					m_bLand;		// モデルに乗っているかどうか
	bool					m_bHit;		// 敵に当たっているかどうか
	bool					m_bRecovery;	// 硬直フラグ
	int						m_nRecoveryTime;// 硬直時間
	STATE					m_State;		// 状態
	DIRECTION				m_Direction;	// 向き(左右)

};
#endif