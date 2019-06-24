//=============================================================================
//
// �K�E�Z���� [ultimate.h]
// Author : ���R���
//
//=============================================================================
#ifndef _ULTIMATE_H_
#define _ULTIMATE_H_

#include "scene.h"

//========================================
//�O���錾
//========================================
class CPlayer;
class CEnemy;

//========================================
// �N���X�̒�`
//========================================
//=========================
// �K�E�Z�N���X
//=========================
class CUltimate
{
public:
	CUltimate();
	~CUltimate();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CUltimate *Create(void);
	int Get1P(void) { return m_n1P; };
	int Get2P(void) { return m_n2P; };
	void LoadChara(void);

private:
	static CPlayer *m_pPlayer;
	static CEnemy *m_pEnemy;
	int m_n1P;
	int m_n2P;
};
#endif