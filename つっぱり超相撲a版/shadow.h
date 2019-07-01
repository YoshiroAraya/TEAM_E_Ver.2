//=============================================================================
//
// �e����
// Author :  ���R���
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "scene.h"
#include "sceneX.h"

//*********************************************************************
//�V�[��3D�N���X�̒�`
//*********************************************************************
class CShadow : public CSceneX//�h���N���X
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

	//�����o�ϐ�
private:
	LPDIRECT3DVERTEXBUFFER9				m_pVtxBuff = NULL;	//���_�o�b�t�@�ւ̃|�C���^
	static LPDIRECT3DTEXTURE9			m_pTexture;	//�e�N�X�`���ւ̃|�C���^
	float								m_Height;

	D3DXVECTOR3							m_pos;							//�ʒu
	D3DXVECTOR3							m_rot;							//����
	D3DXCOLOR							m_col;
	D3DXMATRIX							m_mtxWorld;						//���[���h�}�g���b�N�X
};

#endif