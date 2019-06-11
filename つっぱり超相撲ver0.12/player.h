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
#include "tuppari.h"


//*****************************************************************************
//　前方宣言
//*****************************************************************************
class CModel;


//========================================
// マクロ定義
//========================================
#define MOVE_PLAYER			(0.5f)							//プレイヤー移動量
#define MAX_PARTS		(30)
#define MAX_MOTION		(30)

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
		STATE_JANKEN,
		STATE_NOKOTTA,
		STATE_DOWN,
	}STATE;

	//土俵端状態
	typedef enum
	{
		DOHYO_NORMAL = 0,
		DOHYO_HAZI,
	}DOHYO;

	//土俵端左右状態
	typedef enum
	{
		HAZI_NORMAL = 0,
		HAZI_LEFT,
		HAZI_RIGHT,
	}HAZI_LR;

	//向き
	typedef enum
	{
		DIRECTION_RIGHT = 0,
		DIRECTION_LEFT,
	}DIRECTION;

	/*typedef enum
	{
	HIGASHI = 0,
	NISHI
	} TOUZAI;*/


	//キー要素
	typedef struct
	{
		float fposX;
		float fposY;
		float fposZ;
		float frotX;
		float frotY;
		float frotZ;
	}KEY;
	//キー情報
	typedef struct
	{
		int nFrame;
		KEY aKey[MAX_PARTS];
	}KEY_INFO;

	//モーション情報
	typedef struct
	{
		bool bLoop;
		int nNumKey;
		KEY_INFO aKayInfo[MAX_MOTION];
	}MOTION_INFO;




	CPlayer();	// コンストラクタ
	~CPlayer();	// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos);	// プレイヤー初期化処理
	void Uninit(void);	// プレイヤー終了処理
	void Update(void);	// プレイヤー更新処理
	void Draw(void);	// プレイヤー描画処理

	void CollisonDohyo(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 radius);						// 当たり判定
	void SetMove(D3DXVECTOR3 move);

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
	bool GetDying(void) { return m_bDying; }
	void SetDying(bool bDying) { m_bDying = bDying; }
	bool GetJanken(void) { return m_bJanken; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	CTuppari GetTuppari(void) { return *m_pTuppari; }
	DOHYO GetDohyo(void) { return m_DohyoState; }
	void SetDohyo(DOHYO dohyostate) { m_DohyoState = dohyostate; }
	static CPlayer *Create(D3DXVECTOR3 pos);	// オブジェクトの生成
	bool GetCounter(void) { return m_bCounter; }
	void SetCounter(bool bCounter) { m_bCounter = bCounter; }
	int GetCounterTime(void) { return m_nCounterTime; }
	void SetCounterTime(int nCounter) { m_nCounterTime = nCounter; }
	HAZI_LR GetDohyoHaziLR(void) { return m_DohyoHaziLR; }
	void SetDohyoHaziLR(HAZI_LR DohyoHaziLR) { m_DohyoHaziLR = DohyoHaziLR; }


	//モーションの更新関数
	void UpdateMotion(void);
	//ファイル読み込み関数
	void FileLoad(void);						//ファイル読み込み
	char *ReadLine(FILE *pFile, char *pDst);	//1行読み込み
	char *GetLineTop(char *pStr);				//行の先頭を取得
	int  PopString(char *pStr, char *pDest);	//行の最後を切り捨て




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
	float					m_fLength;		// 回転するときの半径
	bool					m_bLand;		// モデルに乗っているかどうか
	bool					m_bHit;			// 敵に当たっているかどうか
	bool					m_bRecovery;	// 硬直フラグ
	bool					m_bCounter;		// カウンターフラグ
	bool					m_bJanken;		// じゃんけん
	int						m_nRecoveryTime;// 硬直時間
	int						m_nCounterTime;	// カウンター時間
	STATE					m_State;		// 状態
	DIRECTION				m_Direction;	// 向き(左右)
	bool					m_bDying;		// 瀕死かどうか

	int						m_nLife;		// 体力

	CTuppari				*m_pTuppari;
	DOHYO					m_DohyoState;
	HAZI_LR					m_DohyoHaziLR;
	//TOUZAI					m_Touzai;


	// モーション関数
	static LPD3DXMESH			m_pMeshModel[MAX_PARTS];		//メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMatModel[MAX_PARTS];		//マテリアル情報へのポインタ
	static LPDIRECT3DTEXTURE9	m_pTextureModel[MAX_PARTS];		//テクスチャへのポインタ
	static DWORD				m_nNumMatModel[MAX_PARTS];		//マテリアル情報の数

	KEY_INFO					*m_pKeyInfo[MAX_MOTION];		//キー情報へのポインタ
	int							m_nKey;							//現在のキーナンバー
	int							m_nCountFlame;					//フレーム数

	int							m_nNumParts;					//パーツ数
	int							m_aIndexParent[MAX_PARTS];		//親のインデックス
	KEY							m_aKayOffset[MAX_PARTS];		//オフセット情報
	MOTION_INFO					m_aMotionInfo[MAX_MOTION];		//モーション情報
	int							m_nMotionType;					//モーションのタイプ(int型)
	bool						m_bMotionEnd;					//モーションの終わり
	int							m_nOldMotion;					//前回のモーション
	D3DXVECTOR3					m_OffSetPos[MAX_PARTS];			//オフセット情報(モーション)
	//MOTIONSTATE				m_MotionState;
	//DASHSTATE					m_DashState;
	D3DXCOLOR					m_effectCol;
	CModel						*m_apModel[MAX_PARTS];			//パーツ情報

	char						m_aFileNameModel[MAX_PARTS][256];


};
#endif