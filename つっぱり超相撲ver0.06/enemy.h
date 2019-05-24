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
#include "tuppari.h"

//========================================
// �}�N����`
//========================================
#define MOVE_ENEMY			(0.5f)							//�G�l�~�[�ړ���
#define ENEMY_COLLISION		(D3DXVECTOR3(7.0f, 60.0f, 7.0f))		//�G�l�~�[�̓����蔻��

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
		STATE_JANKEN,
		STATE_NOKOTTA,
		STATE_DOWN,
	}STATE;

	//�y�U�[���
	typedef enum
	{
		DOHYO_NORMAL = 0,
		DOHYO_HAZI,
	}DOHYO;

	//����
	typedef enum
	{
		DIRECTION_RIGHT = 0,
		DIRECTION_LEFT,
	}DIRECTION;

	CEnemy();	// �R���X�g���N�^
	~CEnemy();	// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos);	// �G�l�~�[����������
	void Uninit(void);	// �G�l�~�[�I������
	void Update(void);	// �G�l�~�[�X�V����
	void Draw(void);	// �G�l�~�[�`�揈��

	static HRESULT LoadModel(void);	// ���f���ǂݍ���
	static void UnloadModel(void);	// ���f���e�N�X�`�����

	bool GetLand(void);
	STATE GetState(void);
	void SetState(STATE state);
	DIRECTION GetDirection(void) { return m_Direction; }
	void SetDirection(DIRECTION direction) { m_Direction = direction; }
	bool GetRecovery(void) { return m_bRecovery; }
	void SetRecovery(bool bReco) { m_bRecovery = bReco; }
	int GetRecoveryTime(void) { return m_nRecoveryTime; }
	void SetRecoveryTime(int nReco) { m_nRecoveryTime = nReco; }
	bool GetDying(void) { return m_bDying; }
	void SetDying(bool bDying) { m_bDying = bDying; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }

	static CEnemy *Create(D3DXVECTOR3 pos);	// �I�u�W�F�N�g�̐���

	void CollisonSceneX(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 radius);						// �����蔻��
	void SetMove(D3DXVECTOR3 move);
	CTuppari GetTuppari(void) { return *m_pTuppari; }
	DOHYO GetDohyo(void) { return m_DohyoState; }
	void SetDohyo(DOHYO dohyostate) { m_DohyoState = dohyostate; }

private:
	static LPD3DXMESH		m_pMesh;		// ���b�V�����i���_���j�ւ̃|�C���^
	static LPD3DXBUFFER		m_pBuffMat;		// �}�e���A�����ւ̃|�C���^
	static DWORD			m_nNumMat;		// �}�e���A�����̐�
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X

	D3DXVECTOR3				m_move;			// �|���S���̈ʒu
	D3DXVECTOR3				m_posOld;
	float					m_fDestAngle;	// �ړI�̊p�x
	float					m_fDiffAngle;	// �p�x�̍���
	bool					m_bLand;		// ���f���ɏ���Ă��邩�ǂ���
	bool					m_bHit;			// �G�ɓ������Ă��邩�ǂ���
	bool					m_bRecovery;	// �d���t���O
	int						m_nRecoveryTime;// �d������
	STATE					m_State;		// ���
	DIRECTION				m_Direction;	// ����(���E)
	int						m_nLife;		// �̗�
	bool					m_bDying;		// �m�����ǂ���

	CTuppari				*m_pTuppari;
	DOHYO					m_DohyoState;
};
#endif