//=============================================================================
//
// リザルト処理 [result.h]
// Author : 目黒未来也
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "scene.h"
#include "numPlayer.h"

//前方宣言
class CPlayer;
class CEnemy;

//クラス（シーン2Dの派生クラス）
class CResult
{
public:
	CResult();
	~CResult();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CResult *Create(void);
	bool GetTurn(void) { return m_bTurn; }
	void SetTurn(bool turn) { m_bTurn = turn; }
	void LoadWinner(void);
	int Get1P(void) { return m_n1P; };
	int Get2P(void) { return m_n2P; };

private:
	bool m_bSetDohyo;
	bool m_bTurn;		// 回転するかしないか
	int m_nCntTurn;
	int m_nCntReturn;
	int m_nCntDrawTimer;	//表示するまでの時間
	bool m_bDrawSelect;
	static CPlayer *m_pPlayer;
	static CEnemy *m_pEnemy;
	int m_nWinner;
	int m_n1P;					//選んだモデルNo
	int m_n2P;					//選んだモデルNo
	int m_nTime;
	CScene2D	*m_pWinner2D;		// メニューのポリゴン
};
#endif