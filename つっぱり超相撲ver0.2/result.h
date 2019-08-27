//=============================================================================
//
// ���U���g���� [result.h]
// Author : �ڍ�������
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "scene.h"
#include "numPlayer.h"

//�O���錾
class CPlayer;
class CEnemy;

//�N���X�i�V�[��2D�̔h���N���X�j
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
	bool m_bTurn;		// ��]���邩���Ȃ���
	int m_nCntTurn;
	int m_nCntReturn;
	int m_nCntDrawTimer;	//�\������܂ł̎���
	bool m_bDrawSelect;
	static CPlayer *m_pPlayer;
	static CEnemy *m_pEnemy;
	int m_nWinner;
	int m_n1P;					//�I�񂾃��f��No
	int m_n2P;					//�I�񂾃��f��No
	int m_nTime;
	CScene2D	*m_pWinner2D;		// ���j���[�̃|���S��
};
#endif