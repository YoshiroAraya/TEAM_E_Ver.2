//=============================================================================
//
// ���Ԃ̏��� [time.h]
// Author : ���R���
//
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// �O���錾
//=============================================================================

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TIME				"data/TEXTURE/time.png"						

//=========================
// �|�[�Y�N���X
//=========================
class CTime : public CScene2D
{
public:
	CTime();									// �R���X�g���N�^
	~CTime();													// �f�X�g���N�^

	static HRESULT Load(void);									// �e�N�X�`���ǂݍ���
	static void Unload(void);									// �e�N�X�`�����

	static CTime *Create(D3DXVECTOR3 pos);								// �|�[�Y�𐶐�

	HRESULT Init(D3DXVECTOR3 pos);	// �|�[�Y����������
	void Uninit(void);											// �|�[�Y�I������
	void Update(void);											// �|�[�Y�X�V����
	void Draw(void);											// �|�[�Y�`�揈��

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXCOLOR m_Col;
	int	m_nSelect;
	float m_fScale;
	bool m_bReset;
};
#endif