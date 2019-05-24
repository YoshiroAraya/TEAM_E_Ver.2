//=============================================================================
//
// �����̏��� [touzai.h]
// Author : ���R���
//
//=============================================================================
#ifndef _TOUZAI_H_
#define _TOUZAI_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// �O���錾
//=============================================================================

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TOUZAI				"data/TEXTURE/touzai.png"						

//=========================
// �|�[�Y�N���X
//=========================
class CTouzai : public CScene2D
{
public:
	CTouzai();									// �R���X�g���N�^
	~CTouzai();													// �f�X�g���N�^

	static HRESULT Load(void);									// �e�N�X�`���ǂݍ���
	static void Unload(void);									// �e�N�X�`�����

	static CTouzai *Create(D3DXVECTOR3 pos);								// �|�[�Y�𐶐�

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