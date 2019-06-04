//=============================================================================
//
// モデルの読み込み処理 [loadModel.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _LOADMODEL_H_
#define _LOADMODEL_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//========================================
// クラスの定義
//========================================
//=====================
// オブジェクトクラス
//=====================
class CLoadModel
{
public:
	// サウンドファイル
	typedef enum //ラベルと数と並びを揃える
	{
		MODEL_DOHYO = 0,	// 土俵
		MODEL_PLAYER,		// 力士1
		MODEL_ENEMY,		// 力士2
		MODEL_CUSTMOR,		// 観客
		MODEL_SHADOW,		// 影
		MODEL_NEWSCASTER,	// ニュースキャスター
	} MODEL;

	CLoadModel();		// コンストラクタ
	~CLoadModel();	// デストラクタ

	static HRESULT Load(void);
	static void Unload(void);
	static LPD3DXMESH GetMesh(int nIdx);
	static LPD3DXBUFFER GetBuffMat(int nIdx);
	static DWORD GetNumMat(int nIdx);

private:
	static LPDIRECT3DTEXTURE9	*m_pTexture;					// テクスチャへのポインタ
	static LPD3DXMESH		*m_pMesh;		// メッシュ情報（頂点情報）へのポインタ
	static LPD3DXBUFFER		*m_pBuffMat;		// マテリアル情報へのポインタ
	static DWORD			*m_nNumMat;		// マテリアル情報の数
	//static LPDIRECT3DTEXTURE9	m_pTexture;					// テクスチャへのポインタ
	static const char *m_apFilename[];
};

#endif