//=============================================================================
//
// ���C������ [main.h]
// Author : �R���֎j
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "scene.h"

//�O���錾
class CScene3D;
class CPlayer;
class CEnemy;
class CMeshField;
class CShadow;
class CBattleSys;
class CGauge;
class CSansoGauge;
class CUITime;
//�N���X�i�V�[��2D�̔h���N���X�j
class CGame
{
public:
	typedef enum
	{
		WINNER_NONE = 0,
		WINNER_PLAYER1,
		WINNER_PLAYER2,
	}WINNER;

	typedef enum
	{
		STATE_START = 0,
		STATE_GAME,
	}STATE;

	CGame();
	~CGame();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void LoadChara(void);
	static CGame *Create(void);

	static CScene3D *GetScene3D(void);
	static CPlayer *GetPlayer(void);
	static CEnemy *GetEnemy(void);
	static CShadow *GetShadow(void);
	static CMeshField *GetMeshField(void);
	bool Collision(D3DXVECTOR3 *pos0, float fRadius0, D3DXVECTOR3 *pos1, float fRadius1);	// �����蔻��
	static CBattleSys *GetBatlteSys(void) { return m_pBatlteSys; };
	static bool GetHit(void) { return m_bHit; };
	static void SetHit(bool bHit) { m_bHit = bHit; };
	bool GetbUI(void) { return m_bUI; };
	void SetbUI(bool bUI) { m_bUI = bUI; };
	static CGauge *GetGauge(void);
	static CSansoGauge *GetSansoGauge(void) { return m_pSansoGauge; };
	static STATE GetState(void) { return m_State; };
	static void SetState(STATE state) { m_State = state; };
	static WINNER GetWinner(void) { return m_Winner; };
	static void SetWinner(WINNER Winner) { m_Winner = Winner; };
	int Get1P(void) { return m_n1P; };
	int Get2P(void) { return m_n2P; };
	void SaveWinner(void);

private:
	static CScene3D *m_pScene3D;
	static CPlayer *m_pPlayer;
	static CEnemy *m_pEnemy;
	static CShadow *m_pShadow;
	static CUITime *m_pUITime;

	static CMeshField *m_pMeshField;
	static CBattleSys *m_pBatlteSys;
	static bool m_bHit;
	static CGauge *m_pGauge;
	static CSansoGauge *m_pSansoGauge;
	static STATE m_State;
	static WINNER m_Winner;

	bool m_bUI;				//
	int m_n1P;				//�I�񂾃��f��No
	int m_n2P;				//�I�񂾃��f��No
	int m_nWin1P;			//�����������L��
	int m_nWin2P;			//�����������L��
	int m_WinerNum;			//�������̂͂ǂ��炩
	int m_nTime = 0;
};
#endif