//=============================================================================
//
// メイン処理 [main.h]
// Author : 山下敦史
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "scene.h"

//前方宣言
class CScene3D;
class CPlayer;
class CEnemy;
class CMeshField;
class CShadow;
class CBattleSys;

//クラス（シーン2Dの派生クラス）
class CGame
{
public:
	CGame();
	~CGame();

	void Init(void);
	void Uninit(void);
	void Update(void);		
	void Draw(void);
	static CGame *Create(void);

	static CScene3D *GetScene3D(void);
	static CPlayer *GetPlayer(void);
	static CEnemy *GetEnemy(void);
	static CShadow *GetShadow(void);
	static CMeshField *GetMeshField(void);
	bool Collision(D3DXVECTOR3 *pos0, float fRadius0, D3DXVECTOR3 *pos1, float fRadius1);	// 当たり判定

private:
	static CScene3D *m_pScene3D;
	static CPlayer *m_pPlayer;
	static CEnemy *m_pEnemy;
	static CShadow *m_pShadow;
	static CMeshField *m_pMeshField;
	static CBattleSys *m_pBatlteSys;

};
#endif