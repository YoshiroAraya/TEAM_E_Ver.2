//=============================================================================
//
// �G�l�~�[���� [enemy.h]
// Author : �ڍ� ������
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "sceneX.h"
#include "tuppari.h"

//*****************************************************************************
//�@�O���錾
//*****************************************************************************
class CModel;
class CBAnimation;

//========================================
// �}�N����`
//========================================
#define MOVE_ENEMY			(0.5f)									//�G�l�~�[�ړ���
#define MAX_PARTS		(30)
#define MAX_MOTION		(30)
#define MODEL_PARENT	(2)


//========================================
// �N���X�̒�`
//========================================

//=====================
// �G�l�~�[�N���X
//=====================
class CEnemy : public CSceneX
{
public:
	typedef enum
	{
		MODE_P2 = 0,
		MODE_CPU,
	}MODE;

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
		STATE_GUARD,
		STATE_DOWN,
		STATE_ULT,
	}STATE;

	//���
	typedef enum
	{
		CPUACTION_NEUTRAL = 0,	//�������
		CPUACTION_WALK,			//����
		CPUACTION_NEAR,			//�߂Â�
		CPUACTION_FAR,			//��������
		CPUACTION_DASHNEAR,		//�����ċ߂Â�
		CPUACTION_DASHFAR,		//�����ĉ�������
		CPUACTION_GUARD,		//�K�[�h
		CPUACTION_TUPPARI,		//���ς�
		CPUACTION_RENDA,		//�A��
		CPUACTION_YORI,			//���
		CPUACTION_OSHI,			//����
		CPUACTION_NAGE,			//����
		CPUACTION_MAX,			//�ő吔
	}CPUACTION;

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

	CEnemy();	// �R���X�g���N�^
	~CEnemy();	// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// �G�l�~�[����������
	void Uninit(void);	// �G�l�~�[�I������
	void Update(void);	// �G�l�~�[�X�V����
	void Draw(void);	// �G�l�~�[�`�揈��
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,MODE mode);	// �I�u�W�F�N�g�̐���
	static HRESULT LoadModel(void);	// ���f���ǂݍ���
	static void UnloadModel(void);	// ���f���e�N�X�`�����

	void CollisonSceneX(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 radius);						// �����蔻��
	void SetMove(D3DXVECTOR3 move);							//�ړ���ݒ�

	bool GetLand(void);				//���n���Ă��邩�ǂ���
	STATE GetState(void);			//���݂̏��
	void SetState(STATE state);		//��Ԃ�ݒ�
	DIRECTION GetDirection(void) { return m_Direction; }				//�������擾
	void SetDirection(DIRECTION direction) { m_Direction = direction; }	//������ݒ�
	bool GetRecovery(void) { return m_bRecovery; }					//�d����Ԃ��擾
	void SetRecovery(bool bReco) { m_bRecovery = bReco; }			//�d����ݒ�
	int GetRecoveryTime(void) { return m_nRecoveryTime; }			//�d�����Ԃ��擾
	void SetRecoveryTime(int nReco) { m_nRecoveryTime = nReco; }	//�d�����Ԃ�ݒ�
	bool GetDying(void) { return m_bDying; }				//����ł��邩�ǂ���
	void SetDying(bool bDying) { m_bDying = bDying; }		//����ݒ�
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }		//�ߋ��̈ʒu���擾
	CTuppari GetTuppari(void) { return *m_pTuppari; }		//���ς�̃��f�����擾
	DOHYO GetDohyo(void) { return m_DohyoState; }			//�y�U�̏�Ԃ��擾
	void SetDohyo(DOHYO dohyostate) { m_DohyoState = dohyostate; }				//�y�U�[��ݒ�
	bool GetCounter(void) { return m_bCounter; }								//�J�E���^�[��Ԃ��擾
	void SetCounter(bool bCounter) { m_bCounter = bCounter; }					//�J�E���^�[��Ԃ�ݒ�
	int GetCounterTime(void) { return m_nCounterTime; }							//�J�E���^�[�̎��Ԃ��擾
	void SetCounterTime(int nCounter) { m_nCounterTime = nCounter; }			//�J�E���^�[�̎��Ԃ�ݒ�
	HAZI_LR GetDohyoHaziLR(void) { return m_DohyoHaziLR; }						//�E���ǂ���̓y�U�[�ɂ��邩
	void SetDohyoHaziLR(HAZI_LR DohyoHaziLR) { m_DohyoHaziLR = DohyoHaziLR; }	//�y�U�[��ݒ�
	bool GetSelect(void) { return m_bSelect; }				//�L�����I�����擾
	void SetSelect(bool bSelect) { m_bSelect = bSelect; }	//�L�����I����ݒ�
	bool GetbDash(void) { return m_bDash; }					//�_�b�V����Ԃ��擾
	void SetbDash(bool bDash) { m_bDash = bDash; }			//�_�b�V����Ԃ��擾
	bool GetWallHit(void) { return m_bWallHit; }			//�ǌ��˔�����擾
	MOTION_TYPE GetMotionType(int nParent) { return m_MotionType[nParent]; }	//���[�V���������擾
	void SetMotionType(int nParent, MOTION_TYPE MotionType);					//���[�V������ݒ�
	void SetbMotionEnd(int nParent, bool bend) { m_bMotionEnd[nParent] = bend; }//���[�V�����̏I����ݒ�
	void InitStatus(void);
	float EnemyOperation(D3DXVECTOR3 pos, float fMoveEnemy);
	float EnemyCPU(D3DXVECTOR3 pos, float fMoveEnemy);
	float EnemyTutorial(D3DXVECTOR3 pos, float fMoveEnemy);
	void CollisionPlayerAction(void);
	void TimerUpdate(void);
	void TsuppariCollision(D3DXVECTOR3 pos);
	void DohyoHaziWhether(D3DXVECTOR3 pos);
	void EntryEnemy(D3DXVECTOR3 pos, float fMoveEnemy);
	D3DXVECTOR3 DirectionEnemy(D3DXVECTOR3 rot, D3DXVECTOR3 pos);
	bool GetUltDis(void) { return m_bUltDis; }

	static CBAnimation *GetAnimation(void) { return m_pAnimation; }

	//���[�V�����̍X�V�֐�
	void UpdateMotion(int nParent);
	//�t�@�C���ǂݍ��݊֐�
	void FileLoad(char FileName[256], int nParent);	//�t�@�C���ǂݍ���
	char *ReadLine(FILE *pFile, char *pDst);		//1�s�ǂݍ���
	char *GetLineTop(char *pStr);					//�s�̐擪���擾
	int  PopString(char *pStr, char *pDest);		//�s�̍Ō��؂�̂�

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
	bool					m_bSelect;		// �I�΂�Ă��邩�ǂ���
	bool					m_bUltDis;		// �K�E�̉��o�\��
	int						m_nRecoveryTime;// �d������
	int						m_nCounterTime;	// �J�E���^�[����
	STATE					m_State;		// ���
	DIRECTION				m_Direction;	// ����(���E)
	int						m_nLife;		// �̗�
	bool					m_bDying;		// �m�����ǂ���
	int						m_nSiomakiCnt;	// ���܂��J�E���^�[
	bool					m_bDash;		// �����Ă��邩�ǂ���
	bool					m_bWallHit;		// �ǂɌ��˂������ǂ���
	CTuppari				*m_pTuppari;
	DOHYO					m_DohyoState;
	HAZI_LR					m_DohyoHaziLR;


	// ���[�V�����֐�
	static LPD3DXMESH			m_pMeshModel[MAX_PARTS][MODEL_PARENT];		//���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMatModel[MAX_PARTS][MODEL_PARENT];	//�}�e���A�����ւ̃|�C���^
	static LPDIRECT3DTEXTURE9	m_pTextureModel[MAX_PARTS][MODEL_PARENT];	//�e�N�X�`���ւ̃|�C���^
	static DWORD				m_nNumMatModel[MAX_PARTS][MODEL_PARENT];	//�}�e���A�����̐�

	KEY_INFO		*m_pKeyInfo[MAX_MOTION][MODEL_PARENT];		//�L�[���ւ̃|�C���^
	int				m_nKey[MODEL_PARENT];						//���݂̃L�[�i���o�[
	int				m_nCountFlame[MODEL_PARENT];				//�t���[����

	int				m_nNumParts[MODEL_PARENT];					//�p�[�c��
	int				m_aIndexParent[MAX_PARTS][MODEL_PARENT];	//�e�̃C���f�b�N�X
	KEY				m_aKayOffset[MAX_PARTS][MODEL_PARENT];		//�I�t�Z�b�g���
	MOTION_INFO		m_aMotionInfo[MAX_MOTION][MODEL_PARENT];	//���[�V�������
	int				m_nMotionType[MODEL_PARENT];				//���[�V�����̃^�C�v(int�^)
	bool			m_bMotionEnd[MODEL_PARENT];					//���[�V�����̏I���
	int				m_nOldMotion;								//�O��̃��[�V����
	D3DXVECTOR3		m_OffSetPos[MAX_PARTS][MODEL_PARENT];		//�I�t�Z�b�g���(���[�V����)
																	//MOTIONSTATE				m_MotionState;
																			//DASHSTATE					m_DashState;
	D3DXCOLOR		m_effectCol;
	CModel			*m_apModel[MAX_PARTS][MODEL_PARENT];		//�p�[�c���

	char			m_aFileNameModel[MAX_PARTS][256];			//���f���p�[�c�̖��O
	MOTION_TYPE		m_MotionType[MODEL_PARENT];					//���[�V�����̎�ނ�ݒ�

	static CBAnimation *m_pAnimation;
	MODE			m_Mode;
	//CPU�p�̕ϐ�
	int				m_nThinkingTime;	//�l���鎞��
	int				m_nActionTime;		//�s������
	CPUACTION		m_CPUAction;		//�s���̎��
	bool			m_bAction;			//�A�N�V�����������ǂ���
	int				m_DamageCnt;		//�_���[�W������󂯂���
#ifdef _DEBUG
	bool			m_bColBlockDraw;
#endif
};
#endif