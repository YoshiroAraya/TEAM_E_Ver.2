//=============================================================================
//
// 必殺技処理 [ultimate.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _ULTIMATE_H_
#define _ULTIMATE_H_

#include "scene.h"

//========================================
//前方宣言
//========================================
class CPlayer;
class CEnemy;

//========================================
// クラスの定義
//========================================
//=========================
// 必殺技クラス
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