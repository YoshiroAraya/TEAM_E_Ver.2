//=============================================================================
//
// つっぱり処理 [tuppari.h]
// Author : 目黒 未来也
//
//=============================================================================
#ifndef _TUPPARI_H_
#define _TUPPARI_H_

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
class CTuppari : public CScene
{
public:
	CTuppari(int nPriority = SCENEX_PRIORITY, OBJTYPE objType = OBJTYPE_TUPPARI);					// コンストラクタ
	~CTuppari();									// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos);				// 3Dオブジェクト初期化処理
	void Uninit(void);							// 3Dオブジェクト終了処理
	void Update(void);							// 3Dオブジェクト更新処理
	void Draw(void);							// 3Dオブジェクト描画処理

	static CTuppari *Create(D3DXVECTOR3 pos);	// オブジェクトの生成

	D3DXVECTOR3 GetPosition(void);						// 位置の取得
	void SetPosition(D3DXVECTOR3 pos);					// 位置の設定

	D3DXVECTOR3 GetRot(void);						// 向きの取得
	void SetRot(D3DXVECTOR3 rot);					// 向きの設定

	D3DXVECTOR3 GetVtxMax(void);						// 最大値の取得
	void SetVtxMax(D3DXVECTOR3 VtxMax);					// 最大値の設定

	D3DXVECTOR3 GetVtxMin(void);						// 最小値の取得
	void SetVtxMin(D3DXVECTOR3 VtxMin);					// 最小値の設定

	void SetVtx(void);																// 頂点座標の設定
	bool Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 radius);	// 当たり判定

private:
	D3DXMATRIX				m_mtxWorld;			// ワールドマトリックス
	DWORD					m_nNumMat;			// マテリアル情報の数
	D3DXVECTOR3				m_VtxMin, m_VtxMax;	// モデルの最小値、最大値
	D3DXVECTOR3				m_pos;				// ポリゴンの位置
	D3DXVECTOR3				m_rot;				// 上方向ベクトル
};

#endif