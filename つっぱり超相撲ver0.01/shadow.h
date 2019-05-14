//=============================================================================
//
// 影処理
// Author :  長山拓実
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "scene.h"
#include "sceneX.h"

//*********************************************************************
//シーン3Dクラスの定義
//*********************************************************************
class CShadow : public CSceneX//派生クラス
{
public:
	CShadow();
	~CShadow();
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPos(D3DXVECTOR3 pos);
	static CShadow *Create(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void UnLoad(void);

	//メンバ変数
private:
	LPDIRECT3DVERTEXBUFFER9				m_pVtxBuff = NULL;	//頂点バッファへのポインタ
	static LPDIRECT3DTEXTURE9			m_pTexture;	//テクスチャへのポインタ
	static LPD3DXMESH		m_pMesh;		// メッシュ情報（頂点情報）へのポインタ
	static LPD3DXBUFFER		m_pBuffMat;		// マテリアル情報へのポインタ
	static DWORD			m_nNumMat;		// マテリアル情報の数
	float								m_Height;

	D3DXVECTOR3							m_pos;							//位置
	D3DXVECTOR3							m_rot;							//向き
	D3DXCOLOR							m_col;
	D3DXMATRIX							m_mtxWorld;						//ワールドマトリックス
};

#endif