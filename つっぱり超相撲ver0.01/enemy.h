//=============================================================================
//
// �G�l�~�[���� [player.h]
// Author : �ڍ� ������
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "sceneX.h"

//========================================
// �}�N����`
//========================================
#define MOVE_ENEMY			(0.5f)							//�G�l�~�[�ړ���

//========================================
// �N���X�̒�`
//========================================
//=====================
// �G�l�~�[�N���X
//=====================
class CEnemy : public CSceneX
{
public:
	//���
	typedef enum
	{
		STATE_NEUTRAL = 0,
		STATE_KUMI,
		STATE_NAGE,
		STATE_TSUPPARI,
		STATE_DAMAGE,
		STATE_DOWN,
	}STATE;

	CEnemy();	// �R���X�g���N�^
	~CEnemy();	// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos);	// �G�l�~�[����������
	void Uninit(void);	// �G�l�~�[�I������
	void Update(void);	// �G�l�~�[�X�V����
	void Draw(void);	// �G�l�~�[�`�揈��

	static HRESULT LoadModel(void);	// ���f���ǂݍ���
	static void UnloadModel(void);	// ���f���e�N�X�`�����

	bool GetRTriangle(void);

	static CEnemy *Create(D3DXVECTOR3 pos);	// �I�u�W�F�N�g�̐���

private:
	static LPD3DXMESH		m_pMesh;		// ���b�V�����i���_���j�ւ̃|�C���^
	static LPD3DXBUFFER		m_pBuffMat;		// �}�e���A�����ւ̃|�C���^
	static DWORD			m_nNumMat;		// �}�e���A�����̐�
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X

	D3DXVECTOR3				m_move;			// �|���S���̈ʒu
	float					m_fDestAngle;	// �ړI�̊p�x
	float					m_fDiffAngle;	// �p�x�̍���
	bool					m_bRTriangle;	// �E���ɂ��邩�ǂ���
	STATE				m_State;		// ���

};
#endif