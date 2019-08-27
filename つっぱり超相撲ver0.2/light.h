//=============================================================================
//
// ライト処理 [light.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LIGHT	(5)

//========================================
// クラスの定義
//========================================
//=====================
// ライトクラス
//=====================
class CLight
{
public:
	CLight();		// コンストラクタ
	~CLight();		// デストラクタ

	void Init(void);
	void Uninit(void);
	void Update(void);

private:
	static D3DLIGHT9 m_light[MAX_LIGHT];	// ライトの情報
};

#endif