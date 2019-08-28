//=============================================================================
//
// �^�C������ [UITime.h]
// Author : Mikiya Meguro
//
//=============================================================================
#ifndef _UITIME_H_
#define _UITIME_H_

#include "main.h"
#include "scene.h"
//*****************************************************************************
//�@�O���錾
//*****************************************************************************
class CNumber;
class CScene2D;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TIME	(2)		// �^�C���̌���

//*********************************************************************
//�^�C���N���X�̒�`
//*********************************************************************
class CUITime : public CScene //�h���N���X
{
public:
	CUITime(int nPriority = 6);
	~CUITime();
	static CUITime *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);		//�^�C������
	virtual HRESULT  Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void SetTime(int nTime);
	void AddTime(int nValue);
	int GetTime();
	void SetColor(D3DXCOLOR col);
	bool GetTimeStop(void) { return m_bStopTime; };
	void SetTimeStop(bool bStop) { m_bStopTime = bStop; };

private:
	//�����o�ϐ�
	CNumber			*m_apNumber[MAX_TIME];
	CScene2D		*m_p2DSensu;
	D3DXVECTOR3		m_pos;
	float			m_fLength;
	float			m_fWidth, m_fHeight;	// ������
	int				m_nTime;				// ��������
	int				m_nTimerCnt;			// �^�C�}�[�p�J�E���g
	int				m_nColorFlash;			// �_�ŃJ�E���g
	int				m_nFadeCnt;				// �t�F�[�h�܂ł̃J�E���g
	bool			m_bStopTime;			// �^�C�����~�߂邩�ǂ���
};

#endif