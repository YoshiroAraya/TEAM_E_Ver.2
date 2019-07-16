//---------------------------------------------------------------------
//	タイム処理(UITime.cpp)
//	Author : Mikiya Meguro
//---------------------------------------------------------------------
#include "UITime.h"
#include "number.h"
#include "manager.h"
#include "fade.h"
#include "game.h"
#include "tutorial.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define INIT_TIME		(60)

//--------------------------------------------
//タイムクラス コンストラクタ
//--------------------------------------------
CUITime::CUITime(int nPriority) : CScene(nPriority)
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_fLength = 0;
	m_nTime = 0;
	m_nTimerCnt = 0;
	m_nFadeCnt = 0;
	m_nColorFlash = 0;

	for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
	{
		m_apNumber[nCnt] = NULL;
	}
}

//--------------------------------------------
//タイムクラス デストラクタ
//--------------------------------------------
CUITime::~CUITime()
{
}

//--------------------------------------------
//オブジェクトの生成
//--------------------------------------------
CUITime *CUITime::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CUITime *pTime;
	pTime = new CUITime;
	pTime->m_pos = pos;
	pTime->m_fWidth = fWidth;
	pTime->m_fHeight = fHeight;
	pTime->Init(pos);
	return pTime;
}


//=============================================================================
// タイムの初期化処理
//=============================================================================
HRESULT CUITime::Init(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
	{
		m_apNumber[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
	{
		if (m_apNumber[nCnt] == NULL)
		{	//数字の初期化
			m_apNumber[nCnt] = CNumber::Create(D3DXVECTOR3((m_pos.x - (m_fWidth * 1.2f) * nCnt), m_pos.y, m_pos.z),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), m_fWidth, m_fHeight);
		}
	}
	//タイム初期化
	m_nTime = INIT_TIME;
	m_nTimerCnt = 0;
	m_nFadeCnt = 0;
	m_nColorFlash = 0;

	AddTime(0);
	//オブジェクトタイプの設定
	CScene::SetObjType(OBJTYPE_SCENE2D);
	return S_OK;
}

//=============================================================================
// タイムの終了処理
//=============================================================================
void CUITime::Uninit(void)
{
	//自分を破棄
	Release();
}

//=============================================================================
// タイムの更新処理
//=============================================================================
void CUITime::Update(void)
{
	CManager *pManager = NULL;
	CFade *pFade = pManager->GetFade();

	// プレイヤーの取得
	CPlayer *pPlayer = NULL;
	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();
	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
	}
	else if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
	}

	float AlphaCol = 1.0f;

	if (CFade::GetFade() == CFade::FADE_NONE)
	{
		//10秒以下
		if (m_nTime <= 10)
		{
			SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, AlphaCol));
		}
#if 1
		if (pPlayer->GetState() != CPlayer::STATE_JANKEN
			&& pPlayer->GetState() != CPlayer::STATE_NOKOTTA
			&& mode == CManager::MODE_GAME)
		{
			if (m_nTime > 0)
			{	//カウント加算
				m_nTimerCnt++;
				if (m_nTimerCnt >= 60)
				{	//カウント初期化
					m_nTimerCnt = 0;
					//実際の値を引く
					m_nTime -= 1;
					//更新するだけ
					AddTime(0);
				}
			}
			else if (m_nTime <= 0)
			{
				//制限時間を0に
				m_nTime = 0;
				//フェードまでのカウント
				m_nFadeCnt++;
				if (m_nFadeCnt >= 60)
				{
					CManager::GetGame()->TimeOver();
				}
			}
		}
#endif
	}
}

//=============================================================================
// タイムの描画処理
//=============================================================================
void CUITime::Draw(void)
{

}

//=============================================================================
// 数字の設定処理
//=============================================================================
void CUITime::SetTime(int nTime)
{
	m_nTime = nTime;
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	AddTime(0);
}

//=============================================================================
// 数字の加算処理
//=============================================================================
void CUITime::AddTime(int nValue)
{
	int nAnswer = 0;
	int nDigit = 1;
	//タイム加算
	m_nTime += nValue;

	for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
	{
		//桁数の計算
		nAnswer = m_nTime % (nDigit * 10) / nDigit;
		//その桁の数字をセット
		if (m_apNumber[nCnt] != NULL)
		{
			m_apNumber[nCnt]->SetNumber(nAnswer);
		}
		//次の桁へ
		nDigit *= 10;
	}
}
//=============================================================================
// 現在のタイムを返す
//=============================================================================
int CUITime::GetTime()
{
	return m_nTime;
}
//=============================================================================
// 色を設定
//=============================================================================
void CUITime::SetColor(D3DXCOLOR col)
{
	for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
	{
		m_apNumber[nCnt]->SetColor(col);
	}
}

