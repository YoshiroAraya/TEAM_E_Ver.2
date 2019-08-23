//---------------------------------------------------------------------
// ポーズ処理 [pause.cpp]
// Author : Mikiya Meguro
//---------------------------------------------------------------------
#include "modeSelect.h"
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
LPDIRECT3DTEXTURE9 CModeSelect::m_pTexture[MAX_MODESELECTMENU] = {};
LPDIRECT3DTEXTURE9 CModeSelect::m_pTextureBG = NULL;
CModeSelect::SELECTMODE CModeSelect::m_SelectMode = CModeSelect::SELECTMODE_NONE;
bool CModeSelect::m_bModeSelect = false;

//--------------------------------------------
//ポーズクラス コンストラクタ
//--------------------------------------------
CModeSelect::CModeSelect(int nPriority) : CScene(7)
{
}

//--------------------------------------------
//ポーズクラス デストラクタ
//--------------------------------------------
CModeSelect::~CModeSelect()
{
}

//--------------------------------------------
//オブジェクトの生成
//--------------------------------------------
CModeSelect *CModeSelect::Create(D3DXVECTOR3 pos, float m_fWidth)
{
	//ポーズのポインタ
	CModeSelect *pModeSelect;
	pModeSelect = new CModeSelect;

	pModeSelect->m_InitPos = pos;
	pModeSelect->m_fWidth = m_fWidth;
	pModeSelect->m_fHeight = m_fWidth;
	//ポーズの初期化
	pModeSelect->Init(pos);
	//ポーズの情報を返す
	return pModeSelect;
}


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModeSelect::Init(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < MAX_MODESELECTMENU; nCnt++)
	{
		m_apPolygon[nCnt] = NULL;
	}

	m_pTextureBG = CLoad::GetTexture(CLoad::TEXTURE_MODESELECT_BG);
	m_pTexture[0] = CLoad::GetTexture(CLoad::TEXTURE_MODESELECT_CONTINUE);
	m_pTexture[1] = CLoad::GetTexture(CLoad::TEXTURE_MODESELECT_RETRY);
	m_pTexture[2] = CLoad::GetTexture(CLoad::TEXTURE_MODESELECT_QUIT);

	//BGの初期化
	m_apPolygonBG = CScene2D::Create(D3DXVECTOR3(m_InitPos.x, m_InitPos.y, m_InitPos.z));
	m_apPolygonBG->BindTexture(m_pTextureBG);
	m_apPolygonBG->SetWidthHeight(m_fWidth * 4, m_fHeight * 3);
	m_apPolygonBG->SetbDraw(true);



	for (int nCnt = 0; nCnt < MAX_MODESELECTMENU; nCnt++)
	{
		if (m_apPolygon[nCnt] == NULL)
		{	//ポリゴンの初期化
			m_Pos[nCnt] = m_InitPos;
			m_Pos[nCnt].y = m_InitPos.y - m_fWidth - 60;
			//位置をずらす
			m_Pos[nCnt].y = m_Pos[nCnt].y + ((m_fWidth + 60) * nCnt);
			m_apPolygon[nCnt] = CScene2D::Create(D3DXVECTOR3(m_Pos[nCnt].x, m_Pos[nCnt].y, m_Pos[nCnt].z));
			m_apPolygon[nCnt]->SetWidthHeight(m_fWidth * 2.5f, m_fHeight * 1.0f);
			m_apPolygon[nCnt]->BindTexture(m_pTexture[nCnt]);
			m_apPolygon[nCnt]->SetbDraw(true);
		}
	}

	////頂点情報へのポインタ
	//VERTEX_2D *pVtx;
	////頂点バッファを取得
	//m_pVtxBuff = m_apPolygonBG[1]->GetBuff();

	////頂点バッファをロックし頂点データのポインタを取得
	//m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	////頂点カラー
	//pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
	//pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
	//pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
	//pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
	//// 頂点バッファをアンロックする
	//m_pVtxBuff->Unlock();

	//値の初期化
	m_nSelect = 0;
	m_TexMove = D3DXVECTOR3(0, 0, 0);
	m_aModeSelectMenu[m_nSelect].select = SELECTTYPE_SELECT;
	m_aModeSelectMenu[1].select = SELECTTYPE_NONE;
	m_aModeSelectMenu[2].select = SELECTTYPE_NONE;
	m_bModeSelect = true;
	//オブジェクト種類の設定
	CScene::SetObjType(CScene::OBJTYPE_MODESELECT);

	return S_OK;
}

//=============================================================================
// ポーズクラス 終了処理
//=============================================================================
void CModeSelect::Uninit(void)
{

	//自分を破棄
	Release();
}

//=============================================================================
// ポーズクラス 更新処理
//=============================================================================
void CModeSelect::Update(void)
{
	//キーボード情報を取得
	CInputKeyboard *pInput = CManager::GetInputKeyboard();
	//ジョイパッド情報を取得
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();
	//フェードを取得
	CManager *pManager = NULL;
	CFade *pFade = pManager->GetFade();


	//サウンド情報を取得
	CSound *pSound = CManager::GetSound(0);

	//表示切替
	for (int nCnt = 0; nCnt < MAX_MODESELECTMENU; nCnt++)
	{
		m_apPolygon[nCnt]->SetbDraw(m_bModeSelect);
	}
	m_apPolygonBG->SetbDraw(m_bModeSelect);


#ifdef  _DEBUG
	if (pInput->GetTrigger(DIK_O) == true)
	{//TRUEならFALSE FALSEならTRUE
		m_bModeSelect = m_bModeSelect ? false : true;
	}
#endif

	//ポーズ中
	if (m_bModeSelect == true)
	{
		//選択処理
		if (pInput->GetTrigger(DIK_DOWN) == true || pInput->GetTrigger(DIK_S) == true
			|| pXInput->GetTrigger(XINPUT_GAMEPAD_DPAD_DOWN,0) == true
			|| pXInput->GetTrigger(XINPUT_GAMEPAD_DPAD_DOWN,1) == true)
		{
			//pSound->PlaySound(pSound->SOUND_LABEL_SE_SELECT);
			m_aModeSelectMenu[m_nSelect].select = SELECTTYPE_NONE;
			m_nSelect = (m_nSelect + 1) % 3;
			m_aModeSelectMenu[m_nSelect].select = SELECTTYPE_SELECT;
		}
		else if (pInput->GetTrigger(DIK_UP) == true || pInput->GetTrigger(DIK_W) == true
			|| pXInput->GetTrigger(XINPUT_GAMEPAD_DPAD_UP, 0) == true
			|| pXInput->GetTrigger(XINPUT_GAMEPAD_DPAD_UP, 1) == true)
		{
			//pSound->PlaySound(pSound->SOUND_LABEL_SE_SELECT);
			m_aModeSelectMenu[m_nSelect].select = SELECTTYPE_NONE;
			m_nSelect = (m_nSelect + 2) % 3;
			m_aModeSelectMenu[m_nSelect].select = SELECTTYPE_SELECT;
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
			}

			switch (m_SelectMode)
			{
			case SELECTMODE_CONTINUE:
				m_bModeSelect = false;
				CFade::SetFade(CManager::MODE_GAME, pFade->FADE_OUT);
				//SetbModeSelect(m_bModeSelect);
				break;
			case SELECTMODE_RETRY:
				m_bModeSelect = false;
				pManager->SetCharaSelectMode(true);
				pManager->SetNumPlayerMode(1);
				CFade::SetFade(CManager::MODE_TITLE, pFade->FADE_OUT);
				//SetbModeSelect(m_bModeSelect);
				break;
			case SELECTMODE_QUIT:
				m_bModeSelect = false;
				pManager->SetCharaSelectMode(false);
				CFade::SetFade(CManager::MODE_TITLE, pFade->FADE_OUT);
				//SetbModeSelect(m_bModeSelect);
				break;
			}
		}

		//Pキーでキャンセル
		if (pInput->GetTrigger(DIK_P) == true
			|| pXInput->GetTrigger(XINPUT_GAMEPAD_START, 0) == true
			|| pXInput->GetTrigger(XINPUT_GAMEPAD_START, 1) == true)
		{
			m_nSelect = 0;
			m_aModeSelectMenu[m_nSelect].select = SELECTTYPE_SELECT;
			m_aModeSelectMenu[1].select = SELECTTYPE_NONE;
			m_aModeSelectMenu[2].select = SELECTTYPE_NONE;
		}

		//色変え
		for (int nCnt = 0; nCnt < MAX_MODESELECTMENU; nCnt++)
		{
			if (m_aModeSelectMenu[nCnt].select == SELECTTYPE_SELECT)
			{//選択中の色
				m_aModeSelectMenu[nCnt].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			}
			else
			{//未選択の色
				m_aModeSelectMenu[nCnt].col = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
			}

			//頂点情報へのポインタ
			VERTEX_2D *pVtx;
			//頂点バッファを取得
			m_pVtxBuff = m_apPolygon[nCnt]->GetBuff();
			//頂点バッファをロックし頂点データのポインタを取得
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			//頂点カラー
			pVtx[0].col = m_aModeSelectMenu[nCnt].col;
			pVtx[1].col = m_aModeSelectMenu[nCnt].col;
			pVtx[2].col = m_aModeSelectMenu[nCnt].col;
			pVtx[3].col = m_aModeSelectMenu[nCnt].col;
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
void CModeSelect::Draw(void)
{
}

//=============================================================================
// ポーズクラス　選択したモードを取得
//=============================================================================
CModeSelect::SELECTMODE * CModeSelect::GetModeSelectMode(void)
{
	return &m_SelectMode;
}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
HRESULT CModeSelect::Load(void)
{

	return S_OK;
}

//=============================================================================
// テクスチャの破棄処理
//=============================================================================
void CModeSelect::UnLoad(void)
{
	for (int nCnt = 0; nCnt < MAX_MODESELECTMENU; nCnt++)
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
bool CModeSelect::GetModeSelectBool(void)
{
	return m_bModeSelect;
}

//=============================================================================
// ポーズの状態を設定
//=============================================================================
void CModeSelect::SetModeSelectBool(bool ModeSelectBool)
{
	m_bModeSelect = ModeSelectBool;
}

