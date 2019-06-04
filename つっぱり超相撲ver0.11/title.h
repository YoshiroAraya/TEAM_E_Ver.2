//=============================================================================
//
// �^�C�g������ [title.h]
// Author : ���R���
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "scene.h"

//�N���X�i�V�[��2D�̔h���N���X�j
class CTitle
{
public:
	typedef enum
	{
		STATE_NEWS = 0,
		STATE_TITLE,
	}STATE;

	CTitle();
	~CTitle();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CTitle *Create(void);
	STATE GetState(void) { return m_state; }
	void SetState(STATE state) { m_state = state; }

private:
	STATE m_state;
	bool m_bSetDohyo;
};
#endif