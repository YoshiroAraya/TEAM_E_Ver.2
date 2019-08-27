//=============================================================================
//
// オブジェクト処理 [Object3DModel.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "Object3DModel.h"
#include "input.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "camera.h"
#include "scene3D.h"
#include "meshField.h"
#include "shadow.h"
#include "load.h"
#include "enemy.h"
#include "ultimate.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 *C3DObject::m_pTexture = NULL;				// テクスチャ

#define CUSTOMER_ROT			((rand() % 628) / 100.0f)		//全方向

//=============================================================================
// オブジェクトクラスのコンストラクタ
//=============================================================================
C3DObject::C3DObject() : CSceneX(DOHYO_PRIORITY)
{
	// 値をクリア
	m_pVtxBuff = NULL;						// 頂点バッファへのポインタ
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
C3DObject::~C3DObject()
{

}

//=============================================================================
// オブジェクトの生成処理
//=============================================================================
C3DObject *C3DObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	C3DObject *pCustomer = NULL;

	if (pCustomer == NULL)
	{
		// オブジェクトクラスの生成
		pCustomer = new C3DObject;

		if (pCustomer != NULL)
		{
			pCustomer->BindModel(CLoad::GetBuffMat(nType), CLoad::GetNumMat(nType), CLoad::GetMesh(nType));
			pCustomer->Init(pos, rot);
		}
	}

	return pCustomer;
}

//=============================================================================
// オブジェクト初期化処理
//=============================================================================
HRESULT C3DObject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// オブジェクト初期化処理
	CSceneX::Init(pos);

	//オブジェクトの向き
	CSceneX::SetRot(rot);

	// オブジェクトの種類の設定
	SetObjType(CScene::OBJTYPE_DOHYO);
	m_move = D3DXVECTOR3(sinf((rand() % 628) / 100.0f) * ((rand() % 7 + 1)), -1.0f, cosf((rand() % 628) / 100.0f) * ((rand() % 7 + 1)));
	m_rot = D3DXVECTOR3(sinf(D3DX_PI * CUSTOMER_ROT), cosf(D3DX_PI * CUSTOMER_ROT), cosf(D3DX_PI * CUSTOMER_ROT));

	return S_OK;
}

//=============================================================================
// オブジェクト終了処理
//=============================================================================
void C3DObject::Uninit(void)
{
	// 2Dオブジェクト終了処理
	CSceneX::Uninit();
}

//=============================================================================
// オブジェクト更新処理
//=============================================================================
void C3DObject::Update(void)
{
	CManager::MODE mode;
	mode = CManager::GetMode();

	// 敵取得
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

	// 位置を取得
	D3DXVECTOR3 pos;
	pos = CSceneX::GetPosition();

	switch (mode)
	{
	case CManager::MODE_GAME:
		break;
	}

	// 位置の設定
	CSceneX::SetPosition(pos);
}

//=============================================================================
// オブジェクト描画処理
//=============================================================================
void C3DObject::Draw(void)
{
	// 2Dオブジェクト描画処理
	CSceneX::Draw();
}