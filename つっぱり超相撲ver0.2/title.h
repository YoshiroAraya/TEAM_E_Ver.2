//=============================================================================
//
// �^�C�g������ [title.h]
// Author : ���R���
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "scene.h"
#include "numPlayer.h"

//�O���錾
class CPlayer;
class CEnemy;

//�N���X�i�V�[��2D�̔h���N���X�j
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
	bool GetTurnR(void) { return m_bTurnRight; }
	void SetTurnR(bool turn) { m_bTurnRight = turn; }
	bool GetTurnL(void) { return m_bTurnLeft; }
	void SetTurnL(bool turn) { m_bTurnLeft = turn; }
	void SaveCharacter(void);

private:
	STATE m_state;
	CHARACTER m_Character[MAX_NUMPLAYER];
	bool m_bSetDohyo;
	bool m_bSetUI;
	bool m_bTurnRight;		// ��]���邩���Ȃ���
	bool m_bTurnLeft;		// ��]���邩���Ȃ���
	int m_nCntTurn;
	int m_nCntReturn;
	static CPlayer *m_pPlayer;
	static CEnemy *m_pEnemy;
};
#endif