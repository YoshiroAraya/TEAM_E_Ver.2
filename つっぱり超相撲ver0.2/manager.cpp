//=============================================================================
//
// マネージャ処理 [manager.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "manager.h"
#include "main.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneX.h"
#include "billboard.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "debugProc.h"
#include "player.h"
#include "enemy.h"
#include "meshField.h"
#include "shadow.h"
#include "mask.h"
#include "dohyo.h"
#include "BattleSystem.h"
#include "fade.h"
#include "game.h"
#include "characterMove.h"
#include "sound.h"
#include "title.h"
#include "result.h"
#include "tutorial.h"
#include "ultimate.h"
#include "load.h"
#include "pause.h"
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CCamera *CManager::m_pCamera = NULL;
CLight *CManager::m_pLight = NULL;
CDebugProc *CManager::m_pDebugProc = NULL;
CMask *CManager::m_pMask = NULL;
CGame *CManager::m_pGame = NULL;
CTitle *CManager::m_pTitle = NULL;
CResult *CManager::m_pResult = NULL;
CTutorial *CManager::m_pTutorial = NULL;
CUltimate *CManager::m_pUltimate = NULL;
CFade *CManager::m_pFade = NULL;
CCharacterMove *CManager::m_pCharacterMove = NULL;
CXInputJoyPad *CManager::m_pXInput = NULL;
CManager::MODE CManager::m_mode = CManager::MODE_TITLE;	//ゲーム起動時のモード
CSound	*CManager::m_pSound[MAX_SOUND] = {};
bool CManager::m_bCharaSelectMode = false;

//=============================================================================
// マネージャクラスのコンストラクタ
//=============================================================================
CManager::CManager()
{
	m_fData = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager()
{
}

//=============================================================================
// マネージャ初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	m_fData = 0.0f;
	m_bCharaSelectMode = false;

	if (m_pRenderer == NULL)
	{
		// レンダリングクラスの生成
		m_pRenderer = new CRenderer;

		if (m_pRenderer != NULL)
		{
			// 初期化処理
			if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
			{
				return -1;
			}
		}
	}

	if (m_pCamera == NULL)
	{
		// カメラクラスの生成
		m_pCamera = new CCamera;

		if (m_pCamera != NULL)
		{
			m_pCamera->Init();
		}
	}

	if (m_pLight == NULL)
	{
		// ライトクラスの生成
		m_pLight = new CLight;

		if (m_pLight != NULL)
		{
			m_pLight->Init();
		}
	}

	if (m_pInputKeyboard == NULL)
	{
		// 入力クラスの生成
		m_pInputKeyboard = new CInputKeyboard;

		if (m_pInputKeyboard != NULL)
		{
			m_pInputKeyboard->Init(hInstance, hWnd);
		}
	}

	if (m_pXInput == NULL)
	{//エックスインプットコントローラーの生成

		m_pXInput = new CXInputJoyPad;

		if (m_pXInput != NULL)
		{
			m_pXInput->Init();
		}
	}

	if (m_pMask == NULL)
	{
		m_pMask = CMask::Create();
	}

	if (m_pCharacterMove == NULL)
	{
		m_pCharacterMove = CCharacterMove::Create();
	}

	//サウンド
	for (int nCnt = 0; nCnt < MAX_SOUND; nCnt++)
	{
		m_pSound[nCnt] = new CSound;
		if (m_pSound[nCnt] != NULL)
		{
			m_pSound[nCnt]->InitSound(hWnd);
		}
	}
#ifdef _DEBUG
	if (m_pDebugProc == NULL)
	{
		// デバック表示クラスの生成
		m_pDebugProc = new CDebugProc;

		if (m_pDebugProc != NULL)
		{
			m_pDebugProc->Init();
		}
	}
#endif

	if (m_pFade == NULL)
	{
		//フェードの生成
		m_pFade = CFade::Create();

		if (m_pFade != NULL)
		{
			m_pFade->SetFade(m_mode, m_pFade->FADE_IN);
		}
	}

	CLoad::LoadModel();
	CLoad::LoadTex();

	// モード切替
	//SetMode(m_mode);

	return S_OK;
}

//=============================================================================
// マネージャ終了処理
//=============================================================================
void CManager::Uninit(void)
{
	if (m_pMask != NULL)
	{// フェードの終了
		m_pMask->Uninit();
		delete m_pMask;
		m_pMask = NULL;
	}

	if (m_pCharacterMove != NULL)
	{// 移動処理の終了
		m_pCharacterMove->Uninit();
		delete m_pCharacterMove;
		m_pCharacterMove = NULL;
	}

	if (m_pRenderer != NULL)
	{// レンダリングクラスの破棄
		// 終了処理
		m_pRenderer->Uninit();

		// メモリを開放
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	if (m_pCamera != NULL)
	{// カメラクラスの破棄
	 // 終了処理
		m_pCamera->Uninit();

		// メモリを開放
		delete m_pCamera;
		m_pCamera = NULL;
	}

	if (m_pLight != NULL)
	{// ライトクラスの破棄
	 // 終了処理
		m_pLight->Uninit();

		// メモリを開放
		delete m_pLight;
		m_pLight = NULL;
	}

	if (m_pInputKeyboard != NULL)
	{// レンダリングクラスの破棄
		 // 終了処理
		m_pInputKeyboard->Uninit();

		// メモリを開放
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	if (m_pXInput != NULL)
	{// レンダリングクラスの破棄
	 // 終了処理

	 // メモリを開放
		delete m_pXInput;
		m_pXInput = NULL;
	}

	//フェードクラスの破棄
	if (m_pFade != NULL)
	{
		// 終了処理
		m_pFade->Uninit();

		//メモリの開放
		delete m_pFade;

		//NULLにする
		m_pFade = NULL;
	}

	//サウンド
	for (int nCnt = 0; nCnt < MAX_SOUND; nCnt++)
	{
		if (m_pSound[nCnt] != NULL)
		{
			m_pSound[nCnt]->UninitSound();
			delete m_pSound[nCnt];
			m_pSound[nCnt] = NULL;
		}
	}

#ifdef _DEBUG
	if (m_pDebugProc != NULL)
	{// デバック表示クラスの破棄
		// 終了処理
		m_pDebugProc->Uninit();

		// メモリを開放
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}
#endif

	switch (m_mode)
	{
		//タイトルモードの終了処理
	case CManager::MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		break;

	case CManager::MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Uninit();
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		break;

		//ゲームモードの終了処理
	case CManager::MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		break;

		//リザルトモードの終了処理
	case CManager::MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}
		break;

		//必殺技モードの終了処理
	case CManager::MODE_ULTIMATE:
		if (m_pUltimate != NULL)
		{
			m_pUltimate->Uninit();
			delete m_pUltimate;
			m_pUltimate = NULL;
		}
		break;
	}

	CLoad::UnloadModel();
	CLoad::UnloadTex();

	// 全てのオブジェクトを解放
	CScene::ReleseAll();
}

//=============================================================================
// マネージャ更新処理
//=============================================================================
void CManager::Update(void)
{
#ifdef _DEBUG
	// デバック表示を消す
	CDebugProc::ReleseStr();
#endif

	// 入力情報を取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//if (pInputKeyboard->GetTrigger(DIK_G) == true)
	//{
	//	// 2Dポリゴンの生成
	//	CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + m_fData, SCREEN_HEIGHT / 2, 0.0f));

	//	m_fData += 80.0f;
	//}

	bool bPause = CPause::GetPauseBool();


	if (m_pRenderer != NULL)
	{// レンダラー更新処理
		m_pRenderer->Update();
	}

	if (bPause == false)
	{
		if (m_pCamera != NULL)
		{// カメラ更新処理
			m_pCamera->Update();
		}

		if (m_pLight != NULL)
		{// ライト更新処理
			m_pLight->Update();
		}
	}


	if (m_pInputKeyboard != NULL)
	{// キーボード入力更新処理
		m_pInputKeyboard->Update();
	}

	if (m_pXInput != NULL)
	{
		m_pXInput->Update();
	}

	if (m_pMask != NULL)
	{// フェード更新処理
		m_pMask->Update();
	}

	if (m_pFade != NULL)
	{//フェードの更新処理
		m_pFade->Update();
	}

	switch (m_mode)
	{
		//タイトルモードの更新処理
	case CManager::MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Update();
		}
		break;

	case CManager::MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Update();
		}
		break;

		//ゲームモードの更新処理
	case CManager::MODE_GAME:
		if (m_pGame != NULL)
		{
			if (bPause == false)
			{
				m_pGame->Update();
			}
		}
		break;

		//リザルトモードの更新処理
	case CManager::MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Update();
		}
		break;

		//必殺技モードの更新処理
	case CManager::MODE_ULTIMATE:
		if (m_pUltimate != NULL)
		{
			m_pUltimate->Update();
		}
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	if (m_pCamera != NULL)
	{// カメラを設定
		m_pCamera->SetCamera();
	}

	if (m_pRenderer != NULL)
	{// レンダラー描画処理
		m_pRenderer->Draw();
	}
}

//=============================================================================
// レンダラーの取得
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
// 入力情報の取得
//=============================================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

//=============================================================================
// カメラの取得
//=============================================================================
CCamera *CManager::GetCamera(void)
{
	return m_pCamera;
}

//=============================================================================
// マスクの取得
//=============================================================================
CCharacterMove *CManager::GetCharacterMove(void)
{
	return m_pCharacterMove;
}

//=============================================================================
// マスクの取得
//=============================================================================
CMask *CManager::GetMask(void)
{
	return m_pMask;
}

//=============================================================================
// マネージャークラス サウンドを取得
//=============================================================================
CSound *CManager::GetSound(int nNum)
{
	return m_pSound[nNum];
}

//=============================================================================
// モードの設定
//=============================================================================
void CManager::SetMode(MODE mode)
{
	switch (m_mode)
	{
	case CManager::MODE_TITLE:
		//タイトルクラスの破棄
		if (m_pTitle != NULL)
		{
			// 終了処理
			m_pTitle->Uninit();

			//メモリの開放
			delete m_pTitle;

			//NULLにする
			m_pTitle = NULL;
		}
		break;

	case CManager::MODE_TUTORIAL:
		//チュートリアルクラスの破棄
		if (m_pTutorial != NULL)
		{
			// 終了処理
			m_pTutorial->Uninit();
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		break;

	case CManager::MODE_GAME:
		//ゲームクラスの破棄
		if (m_pGame != NULL)
		{
			// 終了処理
			m_pGame->Uninit();

			delete m_pGame;

			m_pGame = NULL;
		}
		break;

	case CManager::MODE_RESULT:
		//リザルトクラスの破棄
		if (m_pResult != NULL)
		{
			// 終了処理
			m_pResult->Uninit();

			//メモリの開放
			delete m_pResult;

			//NULLにする
			m_pResult = NULL;
		}
		break;

	case CManager::MODE_ULTIMATE:
		//リザルトクラスの破棄
		if (m_pUltimate != NULL)
		{
			// 終了処理
			m_pUltimate->Uninit();

			//メモリの開放
			delete m_pUltimate;

			//NULLにする
			m_pUltimate = NULL;
		}
		break;
	}
	m_mode = mode;

	switch (mode)
	{
	case CManager::MODE_TITLE:
		//タイトルの初期化
		if (m_pTitle == NULL)
		{
			//キーボードのメモリを動的確保
			m_pTitle = new CTitle;

			if (m_pTitle != NULL)
			{
				// 初期化処理
				m_pTitle->Init();

				if (m_bCharaSelectMode == true)
				{
					m_pTitle->SetState(CTitle::STATE_CHARASELECT);
				}
			}
			else
			{
				MessageBox(0, "NULLじゃないです", "警告", MB_OK);
			}
		}
		else
		{
			MessageBox(0, "aaaNULLでした", "警告", MB_OK);
		}

		break;
	case CManager::MODE_TUTORIAL:
		//ゲームの初期化
		if (m_pTutorial == NULL)
		{
			//ゲームのメモリを動的確保
			m_pTutorial = new CTutorial;

			if (m_pTutorial != NULL)
			{	// 初期化処理
				m_pTutorial->Init();
			}
			else
			{
				MessageBox(0, "NULLじゃないです", "警告", MB_OK);
			}
		}
		else
		{
			MessageBox(0, "NULLでした", "警告", MB_OK);
		}
		break;
	case CManager::MODE_GAME:
		//ゲームの初期化
		if (m_pGame == NULL)
		{
			//ゲームのメモリを動的確保
			m_pGame = new CGame;

			if (m_pGame != NULL)
			{
				//m_pSound[0]->PlaySound(m_pSound[0]->SOUND_LABEL_BGM_GAME);
				// 初期化処理
				m_pGame->Init();
			}
			else
			{
				MessageBox(0, "NULLじゃないです", "警告", MB_OK);
			}
		}
		else
		{
			MessageBox(0, "NULLでした", "警告", MB_OK);
		}
		break;

	case CManager::MODE_RESULT:
		//リザルトの初期化
		if (m_pResult == NULL)
		{
			//リザルトのメモリを動的確保
			m_pResult = new CResult;

			if (m_pResult != NULL)
			{
				// 初期化処理
				m_pResult->Init();
			}
			else
			{
				MessageBox(0, "NULLじゃないです", "警告", MB_OK);
			}
		}
		else
		{
			MessageBox(0, "NULLでした", "警告", MB_OK);
		}
		break;

	case CManager::MODE_ULTIMATE:
		//リザルトの初期化
		if (m_pUltimate == NULL)
		{
			//リザルトのメモリを動的確保
			m_pUltimate = new CUltimate;

			if (m_pUltimate != NULL)
			{
				// 初期化処理
				m_pUltimate->Init();
			}
			else
			{
				MessageBox(0, "NULLじゃないです", "警告", MB_OK);
			}
		}
		else
		{
			MessageBox(0, "NULLでした", "警告", MB_OK);
		}
		break;

		//case CManager::MODE_TUTORIAL:
		//	//チュートーリアルの初期化
		//	if (m_pTutorial == NULL)
		//	{
		//		//キーボードのメモリを動的確保
		//		m_pTutorial = new CTutorial;

		//		if (m_pTutorial != NULL)
		//		{
		//			// 初期化処理
		//			m_pTutorial->Init();
		//		}
		//		else
		//		{
		//			MessageBox(0, "NULLじゃないです", "警告", MB_OK);
		//		}
		//	}
		//	else
		//	{
		//		MessageBox(0, "NULLでした", "警告", MB_OK);
		//	}
		//	break;

	}
}
