//---------------------------------------------------------------------
// ポーズ処理 [pause.cpp]
// Author : Mikiya Meguro
//---------------------------------------------------------------------
#include "pause.h"
#include "scene.h"
#include "scene2D.h"
#include "input.h"
#include "manager.h"
#include "fade.h"
#include "renderer.h"
#include "load.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//--------------------------------------------
//静的メンバ変数宣言
//--------------------------------------------
LPDIRECT3DTEXTURE9 CPause::m_pTexture[MAX_PAUSEMENU] = {};
LPDIRECT3DTEXTURE9 CPause::m_pTextureBG = NULL;
CPause::SELECTMODE CPause::m_SelectMode = CPause::SELECTMODE_NONE;
bool CPause::m_bPause = false;

//--------------------------------------------
//ポーズクラス コンストラクタ
//--------------------------------------------
CPause::CPause(int nPriority) : CScene(6)
{
}

//--------------------------------------------
//ポーズクラス デストラクタ
//--------------------------------------------
CPause::~CPause()
{
}

//--------------------------------------------
//オブジェクトの生成
//--------------------------------------------
CPause *CPause::Create(D3DXVECTOR3 pos, float m_fWidth)
{
	//ポーズのポインタ
	CPause *pPause;
	pPause = new CPause;

	pPause->m_InitPos = pos;
	pPause->m_fWidth = m_fWidth;
	pPause->m_fHeight = m_fWidth;
	//ポーズの初期化
	pPause->Init(pos);
	//ポーズの情報を返す
	return pPause;
}


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPause::Init(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < MAX_PAUSEMENU; nCnt++)
	{
		m_apPolygon[nCnt] = NULL;
	}

	m_pTextureBG = CLoad::GetTexture(CLoad::TEXTURE_PAUSE_BG);
	m_pTexture[0] = CLoad::GetTexture(CLoad::TEXTURE_PAUSE_CONTINUE);
	m_pTexture[1] = CLoad::GetTexture(CLoad::TEXTURE_PAUSE_RETRY);
	m_pTexture[2] = CLoad::GetTexture(CLoad::TEXTURE_PAUSE_QUIT);

	//BGの初期化
	m_apPolygonBG = CScene2D::Create(D3DXVECTOR3(m_InitPos.x, m_InitPos.y, m_InitPos.z), 5);
	m_apPolygonBG->BindTexture(m_pTextureBG);
	m_apPolygonBG->SetWidthHeight(m_fWidth * 4, m_fHeight * 3);
	m_apPolygonBG->SetbDraw(false);

	for (int nCnt = 0; nCnt < MAX_PAUSEMENU; nCnt++)
	{
		if (m_apPolygon[nCnt] == NULL)
		{	//ポリゴンの初期化
			m_Pos[nCnt] = m_InitPos;
			m_Pos[nCnt].y = m_InitPos.y - m_fWidth - 20;
			//位置をずらす
			m_Pos[nCnt].y = m_Pos[nCnt].y + ((m_fWidth + 30) * nCnt);
			m_apPolygon[nCnt] = CScene2D::Create(D3DXVECTOR3(m_Pos[nCnt].x, m_Pos[nCnt].y, m_Pos[nCnt].z), 6);
			m_apPolygon[nCnt]->SetWidthHeight(m_fWidth * 2.5f, m_fHeight * 1.0f);
			m_apPolygon[nCnt]->BindTexture(m_pTexture[nCnt]);
			m_apPolygon[nCnt]->SetbDraw(false);
		}
	}

	//頂点情報へのポインタ
	//VERTEX_2D *pVtx;
	////頂点バッファを取得
	//m_pVtxBuff = m_apPolygonBG->GetBuff();

	////頂点バッファをロックし頂点データのポインタを取得
	//m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	////頂点カラー
	//pVtx[0].col = D3DXCOLOR(1.0f, 0.6f, 0.0f, 0.8f);
	//pVtx[1].col = D3DXCOLOR(1.0f, 0.6f, 0.0f, 0.8f);
	//pVtx[2].col = D3DXCOLOR(1.0f, 0.6f, 0.0f, 0.8f);
	//pVtx[3].col = D3DXCOLOR(1.0f, 0.6f, 0.0f, 0.8f);
	//// 頂点バッファをアンロックする
	//m_pVtxBuff->Unlock();

	//値の初期化
	m_nSelect = 0;
	m_TexMove = D3DXVECTOR3(0, 0, 0);
	m_aPauseMenu[m_nSelect].select = SELECTTYPE_SELECT;
	m_aPauseMenu[1].select = SELECTTYPE_NONE;
	m_aPauseMenu[2].select = SELECTTYPE_NONE;
	m_bPause = false;
	//オブジェクト種類の設定
	CScene::SetObjType(CScene::OBJTYPE_PAUSE);

	return S_OK;
}

//=============================================================================
// ポーズクラス 終了処理
//=============================================================================
void CPause::Uninit(void)
{

	//自分を破棄
	Release();
}

//=============================================================================
// ポーズクラス 更新処理
//=============================================================================
void CPause::Update(void)
{
	//キーボード情報を取得
	CInputKeyboard *pInput = CManager::GetInputKeyboard();
	//ジョイパッド情報を取得
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();
	//フェードを取得
	CManager *pManager = NULL;
	CFade *pFade = pManager->GetFade();
	// モード取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	//サウンド情報を取得
	CSound *pSound = CManager::GetSound(0);

	//表示切替
	for (int nCnt = 0; nCnt < MAX_PAUSEMENU; nCnt++)
	{
		m_apPolygon[nCnt]->SetbDraw(m_bPause);
	}
	m_apPolygonBG->SetbDraw(m_bPause);

#ifdef  _DEBUG
	if (pInput->GetTrigger(DIK_O) == true)
	{//TRUEならFALSE FALSEならTRUE
		m_bPause = m_bPause ? false : true;
	}
#endif

	//ポーズ中
	if (m_bPause == true)
	{
		//選択処理
		if (pInput->GetTrigger(DIK_DOWN) == true || pInput->GetTrigger(DIK_S) == true
			|| pXInput->GetTrigger(XINPUT_GAMEPAD_DPAD_DOWN,0) == true
			|| pXInput->GetTrigger(XINPUT_GAMEPAD_DPAD_DOWN,1) == true)
		{
			//pSound->PlaySound(pSound->SOUND_LABEL_SE_SELECT);
			m_aPauseMenu[m_nSelect].select = SELECTTYPE_NONE;
			m_nSelect = (m_nSelect + 1) % 3;
			m_aPauseMenu[m_nSelect].select = SELECTTYPE_SELECT;
		}
		else if (pInput->GetTrigger(DIK_UP) == true || pInput->GetTrigger(DIK_W) == true
			|| pXInput->GetTrigger(XINPUT_GAMEPAD_DPAD_UP, 0) == true
			|| pXInput->GetTrigger(XINPUT_GAMEPAD_DPAD_UP, 1) == true)
		{
			//pSound->PlaySound(pSound->SOUND_LABEL_SE_SELECT);
			m_aPauseMenu[m_nSelect].select = SELECTTYPE_NONE;
			m_nSelect = (m_nSelect + 2) % 3;
			m_aPauseMenu[m_nSelect].select = SELECTTYPE_SELECT;
		}
		//エンターキー
		if (pInput->GetTrigger(DIK_RETURN) == true
			|| pXInput->GetPress(XINPUT_GAMEPAD_A, 0) == true
			|| pXInput->GetPress(XINPUT_GAMEPAD_A, 1) == true)
		{
			//pSound->PlaySound(pSound->SOUND_LABEL_SE_CANCEL);
			if (m_nSelect == 0)
			{
				m_SelectMode = SELECTMODE_CONTINUE;
			}
			else if (m_nSelect == 1)
			{
				m_SelectMode = SELECTMODE_RETRY;
			}
			else if (m_nSelect == 2)
			{
				m_SelectMode = SELECTMODE_QUIT;
				pManager->SetCharaSelectMode(false);
			}

			switch (m_SelectMode)
			{
			case SELECTMODE_CONTINUE:
				m_bPause = false;
				SetbPause(m_bPause);
				break;
			case SELECTMODE_RETRY:
				m_bPause = false;

				if (mode == CManager::MODE_TUTORIAL)
				{
					CFade::SetFade(CManager::MODE_TUTORIAL, pFade->FADE_OUT);
				}
				else if (mode == CManager::MODE_GAME)
				{
					CFade::SetFade(CManager::MODE_GAME, pFade->FADE_OUT);
				}
				SetbPause(m_bPause);
				break;
			case SELECTMODE_QUIT:
				m_bPause = false;
				CFade::SetFade(CManager::MODE_TITLE, pFade->FADE_OUT);
				SetbPause(m_bPause);
				break;
			}
		}

		//Pキーでキャンセル
		if (pInput->GetTrigger(DIK_P) == true
			|| pXInput->GetTrigger(XINPUT_GAMEPAD_START, 0) == true
			|| pXInput->GetTrigger(XINPUT_GAMEPAD_START, 1) == true)
		{
			m_nSelect = 0;
			m_aPauseMenu[m_nSelect].select = SELECTTYPE_SELECT;
			m_aPauseMenu[1].select = SELECTTYPE_NONE;
			m_aPauseMenu[2].select = SELECTTYPE_NONE;
		}

		//色変え
		for (int nCnt = 0; nCnt < MAX_PAUSEMENU; nCnt++)
		{
			if (m_aPauseMenu[nCnt].select == SELECTTYPE_SELECT)
			{//選択中の色
				m_aPauseMenu[nCnt].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			}
			else
			{//未選択の色
				m_aPauseMenu[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			//頂点情報へのポインタ
			VERTEX_2D *pVtx;
			//頂点バッファを取得
			m_pVtxBuff = m_apPolygon[nCnt]->GetBuff();
			//頂点バッファをロックし頂点データのポインタを取得
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			//頂点カラー
			pVtx[0].col = m_aPauseMenu[nCnt].col;
			pVtx[1].col = m_aPauseMenu[nCnt].col;
			pVtx[2].col = m_aPauseMenu[nCnt].col;
			pVtx[3].col = m_aPauseMenu[nCnt].col;
			// 頂点バッファをアンロックする
			m_pVtxBuff->Unlock();
		}

		//頂点情報へのポインタ
		VERTEX_2D *pVtx;
		//頂点バッファを取得
		m_pVtxBuff = m_apPolygonBG->GetBuff();
		//頂点バッファをロックし頂点データのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		//頂点テクスチャ
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// ポーズクラス 描画処理
//=============================================================================
void CPause::Draw(void)
{
}

//=============================================================================
// ポーズクラス　選択したモードを取得
//=============================================================================
CPause::SELECTMODE * CPause::GetPauseMode(void)
{
	return &m_SelectMode;
}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
HRESULT CPause::Load(void)
{

	return S_OK;
}

//=============================================================================
// テクスチャの破棄処理
//=============================================================================
void CPause::UnLoad(void)
{
	for (int nCnt = 0; nCnt < MAX_PAUSEMENU; nCnt++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}

	if (m_pTextureBG != NULL)
	{
		m_pTextureBG->Release();
		m_pTextureBG = NULL;
	}
}
//=============================================================================
// ポーズの状態を取得
//=============================================================================
bool CPause::GetPauseBool(void)
{
	return m_bPause;
}

//=============================================================================
// ポーズの状態を設定
//=============================================================================
void CPause::SetPauseBool(bool PauseBool)
{
	m_bPause = PauseBool;
}

