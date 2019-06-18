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


//*****************************************************************************
//�@�O���錾
//*****************************************************************************
class CModel;


//========================================
// �}�N����`
//========================================
#define MOVE_PLAYER			(0.5f)							//�v���C���[�ړ���
#define MAX_PARTS		(30)
#define MAX_MOTION		(30)
#define MODEL_PARENT	(2)
#define TSUPPARI_COLLISION		(D3DXVECTOR3(50.0f, 60.0f, 50.0f))		//���ς�̓����蔻��

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

	typedef enum
	{
		MOTION_NEUTRAL = 0,
		MOTION_WALK,
		MOTION_SIOMAKI,
		MOTION_SYAGAMI,
		MOTION_BATTLE_NEUTRAL,
		MOTION_TSUPPARI,
		MOTION_BUTIKAMASI,
		MOTION_TUKAMI,
		MOTION_TUKAMI_NEUTRAL,
		MOTION_TUKAMI_AGERU,
		MOTION_TUKAMI_AGERARERU,
		MOTION_MAWASI,
		MOTION_NAGE,
		MOTION_KAWASI,
		MOTION_SURIASI,
		MOTION_ULTIMATE,
	}MOTION_TYPE;


	//�y�U�[���
	typedef enum
	{
		DOHYO_NORMAL = 0,
		DOHYO_HAZI,
	}DOHYO;

	//�y�U�[���E���
	typedef enum
	{
		HAZI_NORMAL = 0,
		HAZI_LEFT,
		HAZI_RIGHT,
	}HAZI_LR;

	//����
	typedef enum
	{
		DIRECTION_RIGHT = 0,
		DIRECTION_LEFT,
	}DIRECTION;

	//�L�[�v�f
	typedef struct
	{
		float fposX;
		float fposY;
		float fposZ;
		float frotX;
		float frotY;
		float frotZ;
	}KEY;
	//�L�[���
	typedef struct
	{
		int nFrame;
		KEY aKey[MAX_PARTS];
	}KEY_INFO;

	//���[�V�������
	typedef struct
	{
		bool bLoop;
		int nNumKey;
		KEY_INFO aKayInfo[MAX_MOTION];
	}MOTION_INFO;




	CPlayer();	// �R���X�g���N�^
	~CPlayer();	// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos);	// �v���C���[����������
	void Uninit(void);	// �v���C���[�I������
	void Update(void);	// �v���C���[�X�V����
	void Draw(void);	// �v���C���[�`�揈��

	void CollisonDohyo(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 radius);						// �����蔻��
	void SetMove(D3DXVECTOR3 move);

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
	bool GetJanken(void) { return m_bJanken; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	CTuppari GetTuppari(void) { return *m_pTuppari; }
	DOHYO GetDohyo(void) { return m_DohyoState; }
	void SetDohyo(DOHYO dohyostate) { m_DohyoState = dohyostate; }
	static CPlayer *Create(D3DXVECTOR3 pos);	// �I�u�W�F�N�g�̐���
	bool GetCounter(void) { return m_bCounter; }
	void SetCounter(bool bCounter) { m_bCounter = bCounter; }
	int GetCounterTime(void) { return m_nCounterTime; }
	void SetCounterTime(int nCounter) { m_nCounterTime = nCounter; }
	HAZI_LR GetDohyoHaziLR(void) { return m_DohyoHaziLR; }
	void SetDohyoHaziLR(HAZI_LR DohyoHaziLR) { m_DohyoHaziLR = DohyoHaziLR; }
	bool GetSelect(void) { return m_bSelect; }
	void SetSelect(bool bSelect) { m_bSelect = bSelect; }

	MOTION_TYPE GetMotionType(int nParent) { return m_MotionType[nParent]; }
	void SetMotionType(int nParent, MOTION_TYPE MotionType) { m_MotionType[nParent] = MotionType;
															m_nMotionType[nParent] = m_MotionType[nParent]; }

	void SetbMotionEnd(int nParent, bool bend) { m_bMotionEnd[nParent] = bend; }

	//���[�V�����̍X�V�֐�
	void UpdateMotion(int nParent);
	//�t�@�C���ǂݍ��݊֐�
	void FileLoad(char FileName[256], int nParent);					//�t�@�C���ǂݍ���
	char *ReadLine(FILE *pFile, char *pDst);	//1�s�ǂݍ���
	char *GetLineTop(char *pStr);				//�s�̐擪���擾
	int  PopString(char *pStr, char *pDest);	//�s�̍Ō��؂�̂�

private:
	static LPD3DXMESH		m_pMesh;		// ���b�V�����i���_���j�ւ̃|�C���^
	static LPD3DXBUFFER		m_pBuffMat;		// �}�e���A�����ւ̃|�C���^
	static DWORD			m_nNumMat;		// �}�e���A�����̐�
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X

	D3DXVECTOR3				m_move;
	D3DXVECTOR3				m_posOld;
	float					m_fDestAngle;	// �ړI�̊p�x
	float					m_fDiffAngle;	// �p�x�̍���
	float					m_fLength;		// ��]����Ƃ��̔��a
	float					m_fRot;
	bool					m_bLand;		// ���f���ɏ���Ă��邩�ǂ���
	bool					m_bHit;			// �G�ɓ������Ă��邩�ǂ���
	bool					m_bRecovery;	// �d���t���O
	bool					m_bCounter;		// �J�E���^�[�t���O
	bool					m_bJanken;		// ����񂯂�
	bool					m_bSelect;		// �I�΂�Ă��邩�ǂ���
	int						m_nRecoveryTime;// �d������
	int						m_nCounterTime;	// �J�E���^�[����
	STATE					m_State;		// ���
	DIRECTION				m_Direction;	// ����(���E)
	bool					m_bDying;		// �m�����ǂ���

	int						m_nLife;		// �̗�

	CTuppari				*m_pTuppari;
	DOHYO					m_DohyoState;
	HAZI_LR					m_DohyoHaziLR;
	//TOUZAI					m_Touzai;


	// ���[�V�����֐�
	static LPD3DXMESH			m_pMeshModel[MAX_PARTS][MODEL_PARENT];		//���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMatModel[MAX_PARTS][MODEL_PARENT];	//�}�e���A�����ւ̃|�C���^
	static LPDIRECT3DTEXTURE9	m_pTextureModel[MAX_PARTS][MODEL_PARENT];	//�e�N�X�`���ւ̃|�C���^
	static DWORD				m_nNumMatModel[MAX_PARTS][MODEL_PARENT];	//�}�e���A�����̐�

	KEY_INFO					*m_pKeyInfo[MAX_MOTION][MODEL_PARENT];		//�L�[���ւ̃|�C���^
	int							m_nKey[MODEL_PARENT];						//���݂̃L�[�i���o�[
	int							m_nCountFlame[MODEL_PARENT];				//�t���[����

	int							m_nNumParts[MODEL_PARENT];					//�p�[�c��
	int							m_aIndexParent[MAX_PARTS][MODEL_PARENT];	//�e�̃C���f�b�N�X
	KEY							m_aKayOffset[MAX_PARTS][MODEL_PARENT];		//�I�t�Z�b�g���
	MOTION_INFO					m_aMotionInfo[MAX_MOTION][MODEL_PARENT];	//���[�V�������
	int							m_nMotionType[MODEL_PARENT];				//���[�V�����̃^�C�v(int�^)
	bool						m_bMotionEnd[MODEL_PARENT];								//���[�V�����̏I���
	int							m_nOldMotion;								//�O��̃��[�V����
	D3DXVECTOR3					m_OffSetPos[MAX_PARTS][MODEL_PARENT];		//�I�t�Z�b�g���(���[�V����)
																			//MOTIONSTATE				m_MotionState;
	//DASHSTATE					m_DashState;
	D3DXCOLOR					m_effectCol;
	CModel						*m_apModel[MAX_PARTS][MODEL_PARENT];			//�p�[�c���

	char						m_aFileNameModel[MAX_PARTS][256];
	MOTION_TYPE					m_MotionType[MODEL_PARENT];
#ifdef _DEBUG
	bool						m_bColBlockDraw;
#endif
};
#endif