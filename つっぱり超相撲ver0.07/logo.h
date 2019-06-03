//=============================================================================
//
// ���S���� [logo.h]
// Author : ���R���
//
//=============================================================================
#ifndef _LOGO_H_
#define _LOGO_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//========================================
// �N���X�̒�`
//========================================
//=====================
// �I�u�W�F�N�g�N���X
//=====================
class CLogo : public CScene
{
public:
	typedef enum
	{
		TYPE_TITLE = 0,
		TYPE_TEST,
	}TYPE;

	CLogo(int nPriority = 6, OBJTYPE objType = OBJTYPE_SCENE2D);											// �R���X�g���N�^
	~CLogo();										// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos);				// 2D�I�u�W�F�N�g����������
	void Uninit(void);							// 2D�I�u�W�F�N�g�I������
	void Update(void);							// 2D�I�u�W�F�N�g�X�V����
	void Draw(void);							// 2D�I�u�W�F�N�g�`�揈��

	static HRESULT Load(void);
	static void Unload(void);

	static CLogo *Create(D3DXVECTOR3 pos, float fWidth, float fHeight, TYPE type);			// �I�u�W�F�N�g�̐���

private:
	static LPDIRECT3DTEXTURE9	*m_pTexture;					// �e�N�X�`���ւ̃|�C���^
	static const char *m_apFilename[];
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3				m_Pos;						// �|���S���̈ʒu
	float					m_fSpin;				// ��]
	float					m_fWidth, m_fHeight;	// ������
	float					m_fRight, m_fLeft;		// ���E�̒���
	float					m_fLength;				// �傫��
	float					m_fScale;				// �傫���ύX
	D3DXCOLOR				m_Col;
	TYPE m_type;

};

#endif