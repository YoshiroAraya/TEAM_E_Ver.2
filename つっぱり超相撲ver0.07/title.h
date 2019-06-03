//=============================================================================
//
// タイトル処理 [title.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "scene.h"

//クラス（シーン2Dの派生クラス）
class CTitle
{
public:
	typedef enum
	{
		STATE_NEWS = 0,
		STATE_TITLE,
	}STATE;

	CTitle();
	~CTitle();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CTitle *Create(void);
	STATE GetState(void) { return m_state; }
	void SetState(STATE state) { m_state = state; }

private:
	STATE m_state;
	bool m_bSetDohyo;
};
#endif