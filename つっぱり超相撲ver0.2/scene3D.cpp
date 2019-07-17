//=============================================================================
//
// 3D�|���S������ [scene3D.cpp]
// Author : ���R���
//
//=============================================================================
#include "scene3D.h"
#include "main.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "player.h"
#include "game.h"

//=============================================================================
// 3D�|���S���N���X�̃R���X�g���N�^
//=============================================================================
CScene3D::CScene3D(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �l���N���A
	m_pTexture = NULL;						// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����

	//for (int nCntNor = 0; nCntNor < 2; nCntNor++)
	//{
	//	m_aNor[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �@��
	//}

	for (int nCntNor = 0; nCntNor < NUM_VTX; nCntNor++)
	{
		m_aVec[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �@��
		m_aPos[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �@��
	}
	m_Nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �@��

	m_fSize = 0.0f;							// �傫��
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene3D::~CScene3D()
{
}

//=============================================================================
// �I�u�W�F�N�g�̐�������
//=============================================================================
CScene3D *CScene3D::Create(D3DXVECTOR3 pos)
{
	CScene3D *pScene3D = NULL;

	if (pScene3D == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pScene3D = new CScene3D;

		if (pScene3D != NULL)
		{
			pScene3D->Init(pos);
		}
	}

	return pScene3D;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScene3D::Init(D3DXVECTOR3 pos)
{
	// �|���S���̏���ݒ�
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//for (int nCntNor = 0; nCntNor < 2; nCntNor++)
	//{// �@��
	//	m_aNor[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//}

	for (int nCntNor = 0; nCntNor < NUM_VTX; nCntNor++)
	{
		m_aVec[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �@��
		m_aPos[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �@��
	}

	m_Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �|���S���̈ʒu��ݒ�
	m_pos = pos;

	//m_fSize = GROUND_SIZE ;

	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	//SetObjType(CScene::OBJTYPE_SCENE3D);

	// �����_���[���擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// �e�N�X�`���̐���
	//D3DXCreateTextureFromFile(pDevice, GROUND_TEXTURE_NAME, &m_pTexture);

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, 0.0f, m_fHeight);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, 0.0f, m_fHeight);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, 0.0f, -m_fHeight);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, 0.0f, -m_fHeight);

	//pVtx[0].pos = D3DXVECTOR3(-m_fSize, pVtx[0].pos.y, m_fSize);
	//pVtx[1].pos = D3DXVECTOR3(pVtx[1].pos.x, pVtx[1].pos.y, m_fSize);
	//pVtx[2].pos = D3DXVECTOR3(-m_fSize, pVtx[2].pos.y, pVtx[2].pos.z);
	//pVtx[3].pos = D3DXVECTOR3(pVtx[3].pos.x, pVtx[3].pos.y, pVtx[3].pos.z);

	// �@���̐ݒ�
	// �x�N�g�������߂�
	//m_aVec[0] = pVtx[2].pos - pVtx[3].pos;
	//m_aVec[1] = pVtx[1].pos - pVtx[3].pos;
	//m_aVec[2] = pVtx[1].pos - pVtx[0].pos;
	//m_aVec[3] = pVtx[2].pos - pVtx[0].pos;

	// �O�ς��g���Ė@�������߂�
	//D3DXVec3Cross(&m_aNor[0], &m_aVec[0], &m_aVec[1]);
	//D3DXVec3Cross(&m_aNor[1], &m_aVec[2], &m_aVec[3]);

	//// ���K������
	//D3DXVec3Normalize(&m_aNor[0], &m_aNor[0]);
	//D3DXVec3Normalize(&m_aNor[1], &m_aNor[1]);

	// �@���̐ݒ�
	/*pVtx[0].nor = (m_aNor[0] + m_aNor[1]) / 2;
	pVtx[1].nor = m_aNor[1];
	pVtx[2].nor = m_aNor[0];
	pVtx[3].nor = (m_aNor[0] + m_aNor[1]) / 2;*/

	//pVtx[0].nor = m_aNor[1];
	//pVtx[1].nor = (m_aNor[0] + m_aNor[1]) / 2;
	//pVtx[2].nor = (m_aNor[0] + m_aNor[1]) / 2;
	//pVtx[3].nor = m_aNor[0];

	pVtx[0].nor = m_Nor;
	pVtx[1].nor = m_Nor;
	pVtx[2].nor = m_Nor;
	pVtx[3].nor = m_Nor;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	for (int nCntPos = 0; nCntPos < NUM_VTX; nCntPos++)
	{
		m_aPos[nCntPos] = pVtx[nCntPos].pos;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScene3D::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �I�u�W�F�N�g�̉��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScene3D::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CScene3D::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �f�o�C�X���擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

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

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// �@�����擾
//=============================================================================
D3DXVECTOR3 CScene3D::GetNor(int nIdx)
{
	D3DXVECTOR3 vecA[2];
	return m_Nor;
}

//=============================================================================
// �@����ݒ�
//=============================================================================
void CScene3D::SetNor(D3DXVECTOR3 nor)
{
	m_Nor = nor;
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].nor = m_Nor;
	pVtx[1].nor = m_Nor;
	pVtx[2].nor = m_Nor;
	pVtx[3].nor = m_Nor;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �������擾
//============================================================================
float CScene3D::GetHeight(D3DXVECTOR3 pos)
{
	return pos.y;
}

//=============================================================================
// �e�N�X�`���̐ݒ�
//=============================================================================
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	//�����Ă���������
	m_pTexture = pTexture;
}

//=============================================================================
// �T�C�Y�̐ݒ�
//=============================================================================
void CScene3D::SetSize(float fHeight,float fWidth)
{
	//�擾�����T�C�Y����
	m_fHeight = fHeight;
	m_fWidth = fWidth;
}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void CScene3D::SetRot(D3DXVECTOR3 rot)
{
	//�擾�����T�C�Y����
	m_rot = rot;
}

//=============================================================================
// �A�j���[�V�����̐ݒ菈��
//=============================================================================
void CScene3D::SetAnimation(int m_PatternAnim, float fUV_U, float fUV_V)
{
	VERTEX_3D*pVtx;	//���_���ւ̃|�C���^

					//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���̍��W
	pVtx[0].tex = D3DXVECTOR2(fUV_U * m_PatternAnim, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fUV_U * m_PatternAnim + fUV_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(fUV_U * m_PatternAnim, fUV_V);
	pVtx[3].tex = D3DXVECTOR2(fUV_U * m_PatternAnim + fUV_U, fUV_V);

	////�e�N�X�`���̍��W
	//pVtx[0].tex = D3DXVECTOR2(EXPLOSION_UV_U, EXPLOSION_UV_V * m_PatternAnim);
	//pVtx[1].tex = D3DXVECTOR2(0.0f, EXPLOSION_UV_V * m_PatternAnim);
	//pVtx[2].tex = D3DXVECTOR2(EXPLOSION_UV_U, EXPLOSION_UV_V * m_PatternAnim + EXPLOSION_UV_V);
	//pVtx[3].tex = D3DXVECTOR2(0.0f, EXPLOSION_UV_V * m_PatternAnim + EXPLOSION_UV_V);

	m_PatternAnim = 0;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void CScene3D::SetColor(D3DXCOLOR col)
{
	m_col = col;

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

						// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}