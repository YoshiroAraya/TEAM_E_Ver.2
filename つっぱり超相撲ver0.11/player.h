//=============================================================================
//
// �v���C���[���� [player.h]
// Author : ���R���
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "sceneX.h"
#include "tuppari.h"

//========================================
// �}�N����`
//========================================
#define MOVE_PLAYER			(0.5f)							//�v���C���[�ړ���

//========================================
// �N���X�̒�`
//========================================
//=====================
// �v���C���[�N���X
//=====================
class CPlayer : public CSceneX
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

	/*typedef enum
	{
		HIGASHI = 0,
		NISHI
	} TOUZAI;*/

	CPlayer();	// �R���X�g���N�^
	~CPlayer();	// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos);	// �v���C���[����������
	void Uninit(void);	// �v���C���[�I������
	void Update(void);	// �v���C���[�X�V����
	void Draw(void);	// �v���C���[�`�揈��

	void CollisonDohyo(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 radius);						// �����蔻��
	void SetMove(D3DXVECTOR3 move);

	bool GetLand(void);
	STATE GetState(void);
	void SetState(STATE state);
	DIRECTION GetDirection(void) { return m_Direction; }
	void SetDirection(DIRECTION direction) { m_Direction = direction; }
	bool GetRecovery(void) { return m_bRecovery; }
	void SetRecovery(bool bReco) { m_bRecovery = bReco; }
	int GetRecoveryTime(void) { return m_nRecoveryTime; }
	bool GetJanken(void) { return m_bJanken; }
	void SetRecoveryTime(int nReco) { m_nRecoveryTime = nReco; }
	bool GetDying(void) { return m_bDying; }
	void SetDying(bool bDying) { m_bDying = bDying; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	CTuppari GetTuppari(void) { return *m_pTuppari; }
	DOHYO GetDohyo(void) { return m_DohyoState; }
	void SetDohyo(DOHYO dohyostate) { m_DohyoState = dohyostate; }
	static CPlayer *Create(D3DXVECTOR3 pos);	// �I�u�W�F�N�g�̐���

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X

	D3DXVECTOR3				m_move;
	D3DXVECTOR3				m_posOld;
	float					m_fDestAngle;	// �ړI�̊p�x
	float					m_fDiffAngle;	// �p�x�̍���
	bool					m_bLand;		// ���f���ɏ���Ă��邩�ǂ���
	bool					m_bHit;			// �G�ɓ������Ă��邩�ǂ���
	bool					m_bRecovery;	// �d���t���O
	bool					m_bJanken;		// ����񂯂�
	int						m_nRecoveryTime;// �d������
	STATE					m_State;		// ���
	DIRECTION				m_Direction;	// ����(���E)
	bool					m_bDying;		// �m�����ǂ���

	int						m_nLife;		// �̗�

	CTuppari				*m_pTuppari;
	DOHYO					m_DohyoState;
	//TOUZAI					m_Touzai;
};
#endif