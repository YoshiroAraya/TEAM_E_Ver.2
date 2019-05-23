//=============================================================================
//
// バトルシステム処理 [battlesystem.h]
// Author : 目黒 未来也
//
//=============================================================================
#ifndef _BATTLESYSTEM_H_
#define _BATTLESYSTEM_H_

#include "main.h"

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
		JANKEN_GU = 0,
		JANKEN_CHOKI,
		JANKEN_PA,
	}JANKEN;

	//攻撃の種類
	typedef enum
	{
		ATTACK_TYPE_YORI = 0,
		ATTACK_TYPE_OSI,
		ATTACK_TYPE_NAGE,
	}ATTACK_TYPE;

	CBattleSys();	// コンストラクタ
	~CBattleSys();	// デストラクタ

	HRESULT Init();		// バトルシステム初期化処理
	void Uninit(void);	// バトルシステム終了処理
	void Update(void);	// バトルシステム更新処理

	void Recovery(void);	//硬直処理まとめ
	void Battle(int nPlayer, ATTACK_TYPE AttackType);

	static CBattleSys *Create();	// オブジェクトの生成

	void Operation(void);
private:
	int		m_nCntFlame;			//攻撃のフレーム数
	int		m_nImpossibleFlame;		//操作不可時間
	int		m_aGUCounter[MAX_CHARACTER];
	int		m_aCHOKICounter[MAX_CHARACTER];
	bool	m_abPA[MAX_CHARACTER];
	bool	m_bAttack;				//攻撃しているかどうか
	int		m_nStartCounter;
	JANKEN	m_aJanken[MAX_CHARACTER];
};
#endif