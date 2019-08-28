//=============================================================================
//
// ナンバー処理 [number.h]
// Author : Mikiya Meguro
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"
//*********************************************************************
//ナンバークラスの定義
//*********************************************************************
class CNumber : public CScene2D //派生クラス
{
public:
	CNumber();
	~CNumber();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CNumber *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight);	//オブジェクト生成
	void SetNumber(int nNumber);
	int GetNumber(void);

	static HRESULT Load(void);
	static void UnLoad(void);
	void SetColor(D3DXCOLOR col);
	void SetScale(float scale);

private:
	//メンバ変数
	D3DXVECTOR3						m_pos;
	D3DXCOLOR						m_col;
	float							m_fWidth, m_fHeight;	// 幅高さ
	float							m_fScale;
	int								m_nNumber;
	static LPDIRECT3DTEXTURE9		m_pTexture;				// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9			m_pVtxBuff;				// 頂点バッファへのポインタ
};

#endif