//=============================================================================
//
// モデル処理 [model.h]
// Author : Meguro Mikiya
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
#include "scene.h"

//*********************************************************************
//モデルクラスの定義
//*********************************************************************
class CModel
{
public:
	typedef struct
	{
		DWORD nNumMat = 0;						//マテリアル情報の数
		D3DXVECTOR3 pos;						//位置
		D3DXVECTOR3 rot;						//向き
		float fShadow;							//影の大きさ
		int nIdxShadow;							//影のインデックス
		int nType;								//モデルの種類
		bool bUse;								//使用しているか
	} MODEL;

	CModel();
	~CModel();
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Setpos(D3DXVECTOR3 pos);
	void Setrot(D3DXVECTOR3 rot);
	MODEL *GetModel(void);
	bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, MODEL **pModel*/);
	void SetModel(D3DXVECTOR3 pos, int nType);
	static CModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void BindModel(DWORD nNumMat, LPD3DXMESH Mesh, LPD3DXBUFFER Buff);	//TEXTUREを割り当てる
	void SetParent(CModel *pModel);
	D3DXMATRIX GetMtxWorld(void);
	void SetMtxWorld(D3DXMATRIX MtxWorld);

	D3DXVECTOR3 VtxMin(void) { return m_VtxMinModel; }
	D3DXVECTOR3 VtxMax(void) { return m_VtxMaxModel; }

	D3DXVECTOR3 Getrot(void);
	D3DXVECTOR3 GetPos(void) { return m_aModel.pos; }

	//メンバ変数
private:
	LPD3DXMESH				m_pMeshModel;			//メッシュ情報へのポインタ
	LPD3DXBUFFER			m_pBuffMatModel;		//マテリアル情報へのポインタ
	LPDIRECT3DTEXTURE9		*m_pTextureModel;		//テクスチャへのポインタ
	DWORD					m_nNumMatModel = 0;		//マテリアル情報の数
	MODEL					m_aModel;				//モデル情報
	D3DXVECTOR3				m_VtxMinModel, m_VtxMaxModel;		//モデルの最小値、最大値
	D3DXMATRIX				m_mtxWorld;				//ワールドマトリックス
	CModel					*m_pParent;				//親モデルへのポインタ
public:
	bool					m_bShadow;

};


#endif