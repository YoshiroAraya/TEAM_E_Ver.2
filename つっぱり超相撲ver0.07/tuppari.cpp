//=============================================================================
//
// ���ς菈�� [tuppari.cpp]
// Author : ���R���
//
//=============================================================================
#include "tuppari.h"
#include "input.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TUPPARI_SIZE	(22.0f)	// �T�C�Y�̒���

//=============================================================================
// ���ς�N���X�̃R���X�g���N�^
//=============================================================================
CTuppari::CTuppari(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �l���N���A
	m_nNumMat = 0;							// �}�e���A�����̐�
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTuppari::~CTuppari()
{
}

//=============================================================================
// �I�u�W�F�N�g�̐�������
//=============================================================================
CTuppari *CTuppari::Create(D3DXVECTOR3 pos)
{
	CTuppari *pSceneX = NULL;

	if (pSceneX == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pSceneX = new CTuppari;

		if (pSceneX != NULL)
		{
			pSceneX->Init(pos);
		}
	}

	return pSceneX;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTuppari::Init(D3DXVECTOR3 pos)
{
	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	//SetObjType(CScene::OBJTYPE_SCENEX);

	// �����_���[���擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// �ʒu�E�����̏����ݒ�
	m_pos = pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���_���W�̐ݒ�
	SetVtx();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTuppari::Uninit(void)
{
	// �I�u�W�F�N�g�̉��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTuppari::Update(void)
{


#ifdef _DEBUG
	//CDebugProc::Print("cfccfccfc", "ModelPos : x", m_pos.x, "f", "   y", m_pos.y, "f", "  z", m_pos.z, "f");
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTuppari::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;				// �v�Z�p�}�g���b�N�X

	// �����_���[���擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

}

//=============================================================================
// ���_���W�̐ݒ菈��
//=============================================================================
void CTuppari::SetVtx(void)
{
	// ���_�����擾

	// ���_�̍ŏ��l�ƍő�l����
	//m_VtxMin = D3DXVECTOR3(10000, 10000, 10000);
	//m_VtxMax = D3DXVECTOR3(-10000, -10000, -10000);

}

//=============================================================================
// �u���b�N�Ƃ̓����蔻�菈��
//=============================================================================
bool CTuppari::Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 radius)
{
	bool bLand = false;	// ����Ă��Ȃ����

	D3DXVECTOR3 ScaleVtxMax;
	D3DXVECTOR3 ScaleVtxMin;



	if (pos->z - radius.z <= m_pos.z + TUPPARI_SIZE && pos->z + radius.z >= m_pos.z - TUPPARI_SIZE)
	{// z�͈̔͂̒�
		if (pos->x - radius.x <= m_pos.x + TUPPARI_SIZE && pos->x + radius.x >= m_pos.x - TUPPARI_SIZE)
		{// x�͈̔͂̒�
			bLand = true;
		}
	}


	return bLand;	// �u���b�N�ɏ���Ă��邩�ǂ�����Ԃ�
}

//=============================================================================
// �ʒu�̎擾
//=============================================================================
D3DXVECTOR3 CTuppari::GetPosition(void)
{
	return m_pos;
}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void CTuppari::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// �ʒu�̎擾
//=============================================================================
D3DXVECTOR3 CTuppari::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void CTuppari::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// ���_���̍ő�l�̎擾
//=============================================================================
D3DXVECTOR3 CTuppari::GetVtxMax(void)
{
	return m_VtxMax;
}

//=============================================================================
// ���_���̍ő�l�̐ݒ�
//=============================================================================
void CTuppari::SetVtxMax(D3DXVECTOR3 VtxMax)
{
	m_VtxMax = VtxMax;
}

//=============================================================================
// ���_���̍ŏ��l�̎擾
//=============================================================================
D3DXVECTOR3 CTuppari::GetVtxMin(void)
{
	return m_VtxMin;
}

//=============================================================================
// ���_���̍ŏ��l�̐ݒ�
//=============================================================================
void CTuppari::SetVtxMin(D3DXVECTOR3 VtxMin)
{
	m_VtxMin = VtxMin;
}