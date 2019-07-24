//=============================================================================
//
// タイム処理 [UITime.h]
// Author : Mikiya Meguro
//
//=============================================================================
#ifndef _UITIME_H_
#define _UITIME_H_

#include "main.h"
#include "scene.h"
//*****************************************************************************
//　前方宣言
//*****************************************************************************
class CNumber;
class CScene2D;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TIME	(2)		// タイムの桁数

//*********************************************************************
//タイムクラスの定義
//*********************************************************************
class CUITime : public CScene //派生クラス
{
public:
	CUITime(int nPriority = 6);
	~CUITime();
	static CUITime *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);		//タイム生成
	virtual HRESULT  Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void SetTime(int nTime);
	void AddTime(int nValue);
	int GetTime();
	void SetColor(D3DXCOLOR col);
	bool GetTimeStop(void) { return m_bStopTime; };
	void SetTimeStop(bool bStop) { m_bStopTime = bStop; };

private:
	//メンバ変数
	CNumber			*m_apNumber[MAX_TIME];
	CScene2D		*m_p2DSensu;
	D3DXVECTOR3		m_pos;
	float			m_fLength;
	float			m_fWidth, m_fHeight;	// 幅高さ
	int				m_nTime;				// 制限時間
	int				m_nTimerCnt;			// タイマー用カウント
	int				m_nColorFlash;			// 点滅カウント
	int				m_nFadeCnt;				// フェードまでのカウント
	bool			m_bStopTime;			// タイムを止めるかどうか
};

#endif