//=============================================================================
//
// ���f���̓ǂݍ��ݏ��� [load.h]
// Author : ���R���
//
//=============================================================================
#ifndef _LOAD_H_
#define _LOAD_H_

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
class CLoad
{
public:
	typedef enum // cpp�̂�Ɛ��ƕ��т𑵂���
	{
		MODEL_DOHYO = 0,	// �y�U
		MODEL_PLAYER,		// �͎m1
		MODEL_ENEMY,		// �͎m2
		MODEL_CUSTMOR0,		// �ϋq0
		MODEL_CUSTMOR1,		// �ϋq1
		MODEL_CUSTMOR2,		// �ϋq2
		MODEL_CUSTMOR3,		// �ϋq3
		MODEL_SHADOW,		// �e
		MODEL_NEWSCASTER,	// �j���[�X�L���X�^�[
		MODEL_TROPHY,		// �g���t�B�[
		MODEL_GYOUZI,		// �s�i
		MODEL_MAWASI,		// �܂킵
		MODEL_SYMBOL,		// �V���{��
	} MODEL;

	typedef enum // cpp�̂�Ɛ��ƕ��т𑵂���
	{
		TEXTURE_DOHYOCIRCLE = 0,	// �y�U�̉~
		TEXTURE_NEWS,				// �j���[�X�̔w�i
		TEXTURE_FIELD,				// �n��
		TEXTURE_WALL,				// ��
		TEXTURE_TITLE,				// ���S
		TEXTURE_TIME,				// ����
		TEXTURE_GAUGE,				// �Q�[�W
		TEXTURE_BUCHIKAMASHI,		// �Ԃ����܂�
		TEXTURE_HENKA,				// �ω�
		TEXTURE_TSUPPARI,			// �˂�����
		TEXTURE_TOUZAI,				// ����
		TEXTURE_1P,					// 1P VS COM
		TEXTURE_2P,					// 1P VS 2P
		TEXTURE_TUTORIAL,			// �`���[�g���A��
		TEXTURE_EFFECT_NORMAL000,	// �G�t�F�N�g000
		TEXTURE_EFFECT_NORMAL001,	// �G�t�F�N�g001
		TEXTURE_ANIMATION,			// �A�j���[�V����
		TEXTURE_EFFECT_NORMAL002,	// �G�t�F�N�g002
		TEXTURE_WINNER_UI,			// ��������UI
		TEXTURE_TYANKO_UI,			// ��������UI
		TEXTURE_NUMBER_UI,			// ������UI
		TEXTURE_PAUSE_CONTINUE,			// �|�[�Y��UI
		TEXTURE_PAUSE_RETRY,			// �|�[�Y��UI
		TEXTURE_PAUSE_QUIT,				// �|�[�Y��UI
		TEXTURE_PAUSE_BG,				// �|�[�Y��BG
		TEXTURE_MODESELECT_CONTINUE,	// ���[�h�Z���N�g��UI
		TEXTURE_MODESELECT_RETRY,		// ���[�h�Z���N�g��UI
		TEXTURE_MODESELECT_QUIT,		// ���[�h�Z���N�g��BG
		TEXTURE_MODESELECT_BG,			// ���[�h�Z���N�g��UI
		TEXTURE_CHARASELECT,			// ����e�N�X�`��
		TEXTURE_ANIMATION_OUGI,			// ���`�̃A�j���[�V����
		TEXTURE_TUTORIAL00,				// �`���[�g���A���̉摜
		TEXTURE_TUTORIAL01,				// �`���[�g���A���̉摜
		TEXTURE_TUTORIAL02,				// �`���[�g���A���̉摜
		TEXTURE_SENSU,					// �^�C���̔w�i�p
		TEXTURE_OUGIUI00,				// ���`����UI�摜
		TEXTURE_IPPON,					// ��{!��UI�摜
		TEXTURE_GAUGEBG,				// �Q�[�W��UI�摜
		TEXTURE_EXGAUGEBG,				// �K�E�Q�[�W��UI�摜
		TEXTURE_WINNER_RESULT,			// ���҂̉摜
		TEXTURE_EFFECT_WAVE,			// �Ռ��g�̉摜
		TEXTURE_EFFECT_SPARK,			// �ΉԂ̉摜
		TEXTURE_TAG,					// �v���C���[�^�O��UI�摜
		TEXTURE_EFFECT_SMOKE,			// ���̉摜
		TEXTURE_EFFECT_CONFETTI,		// ������̉摜
		TEXTURE_EFFECT_WAVE2,			// �Ռ��g�̉摜
		TEXTURE_SHADOW,					// �e�̉摜
	} TEXTURE;

	CLoad();		// �R���X�g���N�^
	~CLoad();	// �f�X�g���N�^

	static HRESULT LoadModel(void);
	static void UnloadModel(void);
	static HRESULT LoadTex(void);
	static void UnloadTex(void);

	static LPD3DXMESH GetMesh(int nIdx);
	static LPD3DXBUFFER GetBuffMat(int nIdx);
	static DWORD GetNumMat(int nIdx);
	static LPDIRECT3DTEXTURE9 GetTexture(int nIdx);

private:
	static LPDIRECT3DTEXTURE9	*m_pTexture;					// �e�N�X�`���ւ̃|�C���^
	static LPD3DXMESH		*m_pMesh;		// ���b�V�����i���_���j�ւ̃|�C���^
	static LPD3DXBUFFER		*m_pBuffMat;		// �}�e���A�����ւ̃|�C���^
	static DWORD			*m_nNumMat;		// �}�e���A�����̐�
	static const char *m_apModelFilename[];
	static const char *m_apTexFilename[];
};

#endif