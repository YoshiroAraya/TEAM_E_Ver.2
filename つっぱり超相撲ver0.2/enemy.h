//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : 目黒 未来也
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "sceneX.h"
#include "tuppari.h"

//*****************************************************************************
//　前方宣言
//*****************************************************************************
class CModel;
class CBAnimation;

//========================================
// マクロ定義
//========================================
#define MOVE_ENEMY			(0.5f)									//エネミー移動量
#define MAX_PARTS		(30)
#define MAX_MOTION		(30)
#define MODEL_PARENT	(2)


//========================================
// クラスの定義
//========================================

//=====================
// エネミークラス
//=====================
class CEnemy : public CSceneX
{
public:
	typedef enum
	{
		MODE_P2 = 0,
		MODE_CPU,
	}MODE;

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
		STATE_GUARD,
		STATE_DOWN,
		STATE_ULT,
	}STATE;

	//状態
	typedef enum
	{
		CPUACTION_NEUTRAL = 0,	//初期状態
		CPUACTION_WALK,			//歩く
		CPUACTION_NEAR,			//近づく
		CPUACTION_FAR,			//遠ざかる
		CPUACTION_DASHNEAR,		//走って近づく
		CPUACTION_DASHFAR,		//走って遠ざかる
		CPUACTION_GUARD,		//ガード
		CPUACTION_TUPPARI,		//つっぱり
		CPUACTION_RENDA,		//連打
		CPUACTION_YORI,			//寄り
		CPUACTION_OSHI,			//押し
		CPUACTION_NAGE,			//投げ
		CPUACTION_MAX,			//最大数
	}CPUACTION;

	typedef enum
	{
		MOTION_NEUTRAL = 0,
		MOTION_WALK,
		MOTION_SIOMAKI,
		MOTION_SYAGAMI,
		MOTION_BATTLE_NEUTRAL,
		MOTION_TSUPPARI,
		MOTION_BUTIKAMASI,
		MOTION_TUKAMI,
		MOTION_TUKAMI_NEUTRAL,
		MOTION_TUKAMI_AGERU,
		MOTION_TUKAMI_AGERARERU,
		MOTION_MAWASI,
		MOTION_NAGE,
		MOTION_KAWASI,
		MOTION_SURIASI,
		MOTION_ULTIMATE,
	}MOTION_TYPE;

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

	CEnemy();	// コンストラクタ
	~CEnemy();	// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// エネミー初期化処理
	void Uninit(void);	// エネミー終了処理
	void Update(void);	// エネミー更新処理
	void Draw(void);	// エネミー描画処理
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,MODE mode);	// オブジェクトの生成
	static HRESULT LoadModel(void);	// モデル読み込み
	static void UnloadModel(void);	// モデルテクスチャ解放

	void CollisonSceneX(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 radius);						// 当たり判定
	void SetMove(D3DXVECTOR3 move);							//移動を設定

	bool GetLand(void);				//着地しているかどうか
	STATE GetState(void);			//現在の状態
	void SetState(STATE state);		//状態を設定
	DIRECTION GetDirection(void) { return m_Direction; }				//向きを取得
	void SetDirection(DIRECTION direction) { m_Direction = direction; }	//向きを設定
	bool GetRecovery(void) { return m_bRecovery; }					//硬直状態を取得
	void SetRecovery(bool bReco) { m_bRecovery = bReco; }			//硬直を設定
	int GetRecoveryTime(void) { return m_nRecoveryTime; }			//硬直時間を取得
	void SetRecoveryTime(int nReco) { m_nRecoveryTime = nReco; }	//硬直時間を設定
	bool GetDying(void) { return m_bDying; }				//死んでいるかどうか
	void SetDying(bool bDying) { m_bDying = bDying; }		//死を設定
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }		//過去の位置を取得
	CTuppari GetTuppari(void) { return *m_pTuppari; }		//つっぱりのモデルを取得
	DOHYO GetDohyo(void) { return m_DohyoState; }			//土俵の状態を取得
	void SetDohyo(DOHYO dohyostate) { m_DohyoState = dohyostate; }				//土俵端を設定
	bool GetCounter(void) { return m_bCounter; }								//カウンター状態を取得
	void SetCounter(bool bCounter) { m_bCounter = bCounter; }					//カウンター状態を設定
	int GetCounterTime(void) { return m_nCounterTime; }							//カウンターの時間を取得
	void SetCounterTime(int nCounter) { m_nCounterTime = nCounter; }			//カウンターの時間を設定
	HAZI_LR GetDohyoHaziLR(void) { return m_DohyoHaziLR; }						//右左どちらの土俵端にいるか
	void SetDohyoHaziLR(HAZI_LR DohyoHaziLR) { m_DohyoHaziLR = DohyoHaziLR; }	//土俵端を設定
	bool GetSelect(void) { return m_bSelect; }				//キャラ選択を取得
	void SetSelect(bool bSelect) { m_bSelect = bSelect; }	//キャラ選択を設定
	bool GetbDash(void) { return m_bDash; }					//ダッシュ状態を取得
	void SetbDash(bool bDash) { m_bDash = bDash; }			//ダッシュ状態を取得
	bool GetWallHit(void) { return m_bWallHit; }			//壁激突判定を取得
	MOTION_TYPE GetMotionType(int nParent) { return m_MotionType[nParent]; }	//モーション情報を取得
	void SetMotionType(int nParent, MOTION_TYPE MotionType);					//モーションを設定
	void SetbMotionEnd(int nParent, bool bend) { m_bMotionEnd[nParent] = bend; }//モーションの終わりを設定
	void InitStatus(void);
	float EnemyOperation(D3DXVECTOR3 pos, float fMoveEnemy);
	float EnemyCPU(D3DXVECTOR3 pos, float fMoveEnemy);
	float EnemyTutorial(D3DXVECTOR3 pos, float fMoveEnemy);
	void CollisionPlayerAction(void);
	void TimerUpdate(void);
	void TsuppariCollision(D3DXVECTOR3 pos);
	void DohyoHaziWhether(D3DXVECTOR3 pos);
	void EntryEnemy(D3DXVECTOR3 pos, float fMoveEnemy);
	D3DXVECTOR3 DirectionEnemy(D3DXVECTOR3 rot, D3DXVECTOR3 pos);
	bool GetUltDis(void) { return m_bUltDis; }

	static CBAnimation *GetAnimation(void) { return m_pAnimation; }

	//モーションの更新関数
	void UpdateMotion(int nParent);
	//ファイル読み込み関数
	void FileLoad(char FileName[256], int nParent);	//ファイル読み込み
	char *ReadLine(FILE *pFile, char *pDst);		//1行読み込み
	char *GetLineTop(char *pStr);					//行の先頭を取得
	int  PopString(char *pStr, char *pDest);		//行の最後を切り捨て

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
	float					m_fRot;
	bool					m_bLand;		// モデルに乗っているかどうか
	bool					m_bHit;			// 敵に当たっているかどうか
	bool					m_bRecovery;	// 硬直フラグ
	bool					m_bCounter;		// カウンターフラグ
	bool					m_bSelect;		// 選ばれているかどうか
	bool					m_bUltDis;		// 必殺の演出表示
	int						m_nRecoveryTime;// 硬直時間
	int						m_nCounterTime;	// カウンター時間
	STATE					m_State;		// 状態
	DIRECTION				m_Direction;	// 向き(左右)
	int						m_nLife;		// 体力
	bool					m_bDying;		// 瀕死かどうか
	int						m_nSiomakiCnt;	// 塩まきカウンター
	bool					m_bDash;		// 走っているかどうか
	bool					m_bWallHit;		// 壁に激突したかどうか
	CTuppari				*m_pTuppari;
	DOHYO					m_DohyoState;
	HAZI_LR					m_DohyoHaziLR;


	// モーション関数
	static LPD3DXMESH			m_pMeshModel[MAX_PARTS][MODEL_PARENT];		//メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMatModel[MAX_PARTS][MODEL_PARENT];	//マテリアル情報へのポインタ
	static LPDIRECT3DTEXTURE9	m_pTextureModel[MAX_PARTS][MODEL_PARENT];	//テクスチャへのポインタ
	static DWORD				m_nNumMatModel[MAX_PARTS][MODEL_PARENT];	//マテリアル情報の数

	KEY_INFO		*m_pKeyInfo[MAX_MOTION][MODEL_PARENT];		//キー情報へのポインタ
	int				m_nKey[MODEL_PARENT];						//現在のキーナンバー
	int				m_nCountFlame[MODEL_PARENT];				//フレーム数

	int				m_nNumParts[MODEL_PARENT];					//パーツ数
	int				m_aIndexParent[MAX_PARTS][MODEL_PARENT];	//親のインデックス
	KEY				m_aKayOffset[MAX_PARTS][MODEL_PARENT];		//オフセット情報
	MOTION_INFO		m_aMotionInfo[MAX_MOTION][MODEL_PARENT];	//モーション情報
	int				m_nMotionType[MODEL_PARENT];				//モーションのタイプ(int型)
	bool			m_bMotionEnd[MODEL_PARENT];					//モーションの終わり
	int				m_nOldMotion;								//前回のモーション
	D3DXVECTOR3		m_OffSetPos[MAX_PARTS][MODEL_PARENT];		//オフセット情報(モーション)
																	//MOTIONSTATE				m_MotionState;
																			//DASHSTATE					m_DashState;
	D3DXCOLOR		m_effectCol;
	CModel			*m_apModel[MAX_PARTS][MODEL_PARENT];		//パーツ情報

	char			m_aFileNameModel[MAX_PARTS][256];			//モデルパーツの名前
	MOTION_TYPE		m_MotionType[MODEL_PARENT];					//モーションの種類を設定

	static CBAnimation *m_pAnimation;
	MODE			m_Mode;
	//CPU用の変数
	int				m_nThinkingTime;	//考える時間
	int				m_nActionTime;		//行動時間
	CPUACTION		m_CPUAction;		//行動の種類
	bool			m_bAction;			//アクションしたかどうか
	int				m_DamageCnt;		//ダメージを何回受けたか
#ifdef _DEBUG
	bool			m_bColBlockDraw;
#endif
};
#endif