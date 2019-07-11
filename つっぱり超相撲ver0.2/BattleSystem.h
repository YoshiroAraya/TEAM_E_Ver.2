//=============================================================================
//
// バトルシステム処理 [battlesystem.h]
// Author : 目黒 未来也
//
//=============================================================================
#ifndef _BATTLESYSTEM_H_
#define _BATTLESYSTEM_H_

#include "main.h"
#include "enemy.h"
//========================================
// マクロ定義
//========================================
#define MAX_CHARACTER (2)

//========================================
// クラスの定義
//========================================
//=====================
// バトルシステムクラス
//=====================
class CBattleSys
{
public:
	//3すくみ
	typedef enum
	{
		JANKEN_GU_BUTI = 0,
		JANKEN_CHOKI_TUPPA,
		JANKEN_PA_KAWASI,
	}JANKEN;

	//攻撃の種類
	typedef enum
	{
		ATTACK_TYPE_YORI = 0,
		ATTACK_TYPE_OSI,
		ATTACK_TYPE_NAGE,
		ATTACK_TYPE_TUPPARI,
		ATTACK_TYPE_COUNTER,
	}ATTACK_TYPE;

	//どっちが攻撃可能か
	typedef enum
	{
		ATTACK_TURN_NORMAL = 0,
		ATTACK_TURN_PLAYER1,
		ATTACK_TURN_PLAYER2,
	}ATTACK_TURN;



	CBattleSys();	// コンストラクタ
	~CBattleSys();	// デストラクタ

	HRESULT Init();		// バトルシステム初期化処理
	void Uninit(void);	// バトルシステム終了処理
	void Update(void);	// バトルシステム更新処理

	void Recovery(void);	//硬直処理まとめ
	void Battle(int nPlayer, ATTACK_TYPE AttackType, D3DXVECTOR3 P1move, D3DXVECTOR3 P2move);
	void PushJudge(void);	//連打判定
	void P1Attack(void);
	void P2Attack(void);
	void CounterAttack(void);
	void ResetBattle(void);
	void GuardKnockBack(int nAttack);
	void MotionSetYORI(int nAttack);
	void PlusCntPushP2(int nCnt) { m_nCntPushP2 += nCnt; };
	void CPUBattle(CEnemy::CPUACTION CpuAction);
	ATTACK_TURN GetAttackTurn(void) { return m_AttackTurn; };

	static CBattleSys *Create();	// オブジェクトの生成

	void Operation(void);
private:
	int		m_nCntAttackFlame;			//攻撃のフレーム数
	int		m_nImpossibleFlame;		//操作不可時間
	int		m_aGUCounter[MAX_CHARACTER];
	int		m_aCHOKICounter[MAX_CHARACTER];
	bool	m_abPA[MAX_CHARACTER];
	bool	m_bAttack;				//攻撃しているかどうか
	int		m_nStartCounter;
	int		m_nCntPushP1, m_nCntPushP2;	//どのくらい連打したか
	int		m_nFlamePush;
	float	m_fMoveDying[2] = { 1.0f,1.0f };	//瀕死時の移動量
	JANKEN	m_aJanken[MAX_CHARACTER];
	ATTACK_TURN m_AttackTurn;
};
#endif