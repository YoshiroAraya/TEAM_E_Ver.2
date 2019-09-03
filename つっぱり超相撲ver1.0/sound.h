//=============================================================================
//
// サウンド処理 [sound.h]
// Author : Meguro Mikiya
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*********************************************************************
//サウンドクラスの定義
//*********************************************************************
//基本クラス
class CSound
{
public:
	// サウンドファイル
	typedef enum //ラベルと数と並びを揃える
	{
		SOUND_LABEL_BGM_TITLE = 0,	// BGMタイトル
		SOUND_LABEL_BGM_TUTORIAL,	// BGMチュートリアル
		SOUND_LABEL_BGM_GAME,		// BGMゲーム
		SOUND_LABEL_BGM_RESULT,		// BGMリザルト
		SOUND_LABEL_BGM_GAMEOVER,	// BGMゲームオーバー
		SOUND_LABEL_BGM_RANKING,	// BGMランキング
		SOUND_LABEL_SE_HIT00,		// 衝突
		SOUND_LABEL_SE_NOISE,		// 雑音
		SOUND_LABEL_SE_HAKKEYOI,	// はっけよい
		SOUND_LABEL_SE_NOKOTTA,		// のこった
		SOUND_LABEL_SE_DOHYO,		// 土俵


		SOUND_LABEL_MAX,
	} SOUND_LABEL;
	// パラメータ構造体定義
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
	} SOUNDPARAM;

	CSound();
	~CSound();
	HRESULT InitSound(HWND hWnd);
	void UninitSound(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

protected:
	IXAudio2				*m_pXAudio2 = NULL;								// XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice	*m_pMasteringVoice = NULL;						// マスターボイスへのポインタ
	IXAudio2SourceVoice		*m_apSourceVoice[SOUND_LABEL_MAX] = {};			// ソースボイスへのポインタ
	BYTE					*m_apDataAudio[SOUND_LABEL_MAX] = {};			// オーディオデータへのポインタ
	DWORD					m_aSizeAudio[SOUND_LABEL_MAX] = {};				// オーディオデータサイズ
    // 各音素材のパラメータ ラベルと数と並びを揃える
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/title1.wav", -1 },		// タイトル
		{ "data/BGM/tutorial.wav", -1 },	// チュートリアル
		{ "data/BGM/game.wav", -1 },		// ゲーム
		{ "data/BGM/result1.wav", -1 },		// リザルト
		{ "data/BGM/gameover.wav", -1 },	// ゲームオーバー
		{ "data/BGM/ranking.wav", -1 },		// ランキング
		{ "data/SE/hit.wav", 0 },			// 衝撃音001
		{ "data/SE/noise.wav", 0 },			// 雑音
		{ "data/SE/hakkeyoi.wav", 0 },		// はっけよい
		{ "data/SE/nokotta.wav", 0 },		// のこった
		{ "data/SE/dohyo.wav", 0 },			// 土俵入り
	};
};
#endif
