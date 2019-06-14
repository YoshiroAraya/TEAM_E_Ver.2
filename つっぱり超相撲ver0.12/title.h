//=============================================================================
//
// タイトル処理 [title.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "scene.h"
#include "numPlayer.h"

//前方宣言
class CPlayer;
class CEnemy;

//クラス（シーン2Dの派生クラス）
class CTitle
{
public:
	typedef enum
	{
		STATE_NEWS = 0,
		STATE_TITLE,
		STATE_CHARASELECT,
	}STATE;

	typedef enum
	{
		CHARACTER_PLAYER = 0,
		CHARACTER_ENEMY,
	}CHARACTER;

	CTitle();
	~CTitle();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CTitle *Create(void);
	STATE GetState(void) { return m_state; }
	void SetState(STATE state) { m_state = state; }
	bool GetTurn(void) { return m_bTurn; }
	void SetTurn(bool turn) { m_bTurn = turn; }
	void SaveCharacter(void);

private:
	STATE m_state;
	CHARACTER m_Character[MAX_NUMPLAYER];
	bool m_bSetDohyo;
	bool m_bTurn;		// 回転するかしないか
	int m_nCntTurn;
	int m_nCntReturn;
	static CPlayer *m_pPlayer;
	static CEnemy *m_pEnemy;
};
#endif