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
	CTitle();
	~CTitle();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CTitle *Create(void);

private:
	
};
#endif