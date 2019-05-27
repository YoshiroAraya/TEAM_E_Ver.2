//=============================================================================
//
// �J�������� [camera.h]
// Author : ���R���
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//========================================
// �N���X�̒�`
//========================================
//=====================
// �J�����N���X
//=====================
class CCamera
{
public:
	typedef enum
	{
		STATE_NONE = 0,
		STATE_NORMAL,
		STATE_HIGASHI,
		STATE_NISHI
	}STATE;

	CCamera();		// �R���X�g���N�^
	~CCamera();		// �f�X�g���N�^

	void Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

	D3DXVECTOR3 GetRot(void);						// �ʒu�̎擾
	static STATE GetState(void) { return m_State; }

private:
	D3DXVECTOR3 m_posV;				// ���_
	D3DXVECTOR3 m_posR;				// �����_
	D3DXVECTOR3 m_recU;				// ������x�N�g��
	D3DXVECTOR3 m_rot;				// ������x�N�g��
	float m_fLength;				// �����_����̋���
	//float fAngle;				
	D3DXMATRIX	m_mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	m_mtxView;			// �r���[�}�g���b�N�X
	int m_nStartCounter;
	static STATE m_State;
};

#endif