//=============================================================================
//
// ���b�V���t�B�[���h�̏��� [meshField.cpp]
// Author : ���R���
//
//=============================================================================
#include "meshField.h"
#include "main.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"

//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
float g_aHeight[POLYGON_Z + 1][POLYGON_X + 1];

//=============================================================================
// ���b�V���t�B�[���h�̃R���X�g���N�^
//=============================================================================
CMeshField::CMeshField(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �l���N���A
	m_pTexture = NULL;	// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = NULL;
	m_nNumVerTex = 0;
	m_nNumIndex = 0;
	m_nNumPolygon = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bRand = false;

	for (int nCntNor = 0; nCntNor < NUM_POLYGON; nCntNor++)
	{
		m_aNor[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshField::~CMeshField()
{
}

//=============================================================================
// �I�u�W�F�N�g�̐�������
//=============================================================================
CMeshField *CMeshField::Create(D3DXVECTOR3 pos)
{
	CMeshField *pMeshField = NULL;

	if (pMeshField == NULL)
	{
		// �I�u�W�F�N�g�N���X�̐���
		pMeshField = new CMeshField;

		if (pMeshField != NULL)
		{
			pMeshField->Init(pos);
		}
	}

	return pMeshField;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMeshField::Init(D3DXVECTOR3 pos)
{
	m_pos = pos;	// �ʒu�̐ݒ�

	// �l�̏�����
	m_pTexture = NULL;	// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = NULL;
	m_nNumVerTex = 0;
	m_nNumIndex = 0;
	m_nNumPolygon = 0;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bRand = false;

	for (int nCntZ = 0; nCntZ < POLYGON_Z + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < POLYGON_X + 1; nCntX++)
		{
			g_aHeight[nCntZ][nCntX] = 0.0f;
		}
	}

	// �����_���[���擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{// �f�o�C�X�̎擾
		pDevice = pRenderer->GetDevice();
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, MESHFIELD_TEXTURE_NAME, &m_pTexture);

	// ���_���̍쐬
	int nVtxCounter = 0;
	int nIdxCounter = 0;

	int nCntVtxZ;
	int nCntVtxX;
	int nCntIdxZ;
	int nCntIdxX;

	// ���_��
	m_nNumVerTex = (POLYGON_X + 1) * (POLYGON_Z + 1);

	// �C���f�b�N�X��
	m_nNumIndex = (POLYGON_X + 1) * (POLYGON_Z + 1)
		+ (2 * (POLYGON_Z - 1))
		+ (POLYGON_X + 1) * (POLYGON_Z - 1);

	// �|���S����
	m_nNumPolygon = m_nNumIndex - 2;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVerTex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,	// �m�ۂ���o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,										// 16�r�b�g�̃f�[�^���m��
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntVtxZ = 0; nCntVtxZ < POLYGON_Z + 1; nCntVtxZ++)
	{
		for (nCntVtxX = 0; nCntVtxX < POLYGON_X + 1; nCntVtxX++)
		{
			// ���_���W�̐ݒ�
			pVtx[(nCntVtxZ + nCntVtxX) + nVtxCounter].pos = D3DXVECTOR3(-MESHFIELD_SIZE + (nCntVtxX * MESHFIELD_SIZE), 0.0f, MESHFIELD_SIZE - (nCntVtxZ * MESHFIELD_SIZE));
			// �@���̐ݒ�
			pVtx[(nCntVtxZ + nCntVtxX) + nVtxCounter].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// ���_�J���[�̐ݒ�
			pVtx[(nCntVtxZ + nCntVtxX) + nVtxCounter].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// �e�N�X�`�����W�̐ݒ�
			pVtx[(nCntVtxZ + nCntVtxX) + nVtxCounter].tex = 
				D3DXVECTOR2(0.0f + (nCntVtxX /** (10.0f / POLYGON_X)*/), 0.0f + (nCntVtxZ /** (10.0f / POLYGON_Z)*/));
		}
		nVtxCounter += POLYGON_X;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

#if(1)
	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntPolygon = 0;

	int nData = 0;
	int nData2 = 0;
	int nData3 = 0;

	for (int nCntZ = 0; nCntZ < POLYGON_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX < POLYGON_X; nCntX++)
		{
			D3DXVECTOR3 *pPos0, *pPos1, *pPos2, *pPos3;
			D3DXVECTOR3 vec0, vec1, vec2;
			D3DXVECTOR3 nor;

			// ����̃|���S���̂Q�̃x�N�g������@�����Z�o
			pPos0 = &pVtx[nCntX + nCntZ + nCntPolygon].pos;
			pPos1 = &pVtx[(nCntX + nCntZ + nCntPolygon) + (POLYGON_X + 1)].pos;
			pPos2 = &pVtx[(nCntX + nCntZ + nCntPolygon) + (POLYGON_X + 1) + 1].pos;
			pPos3 = &pVtx[(nCntX + nCntZ + nCntPolygon) + 1].pos;

			vec0 = *pPos1 - *pPos0;
			vec1 = *pPos2 - *pPos0;
			vec2 = *pPos3 - *pPos0;

			// �O�ς��g���Ċe�|���S���̖@�������߂�
			D3DXVec3Cross(&nor, &vec1, &vec0);
			// ���K������
			D3DXVec3Normalize(&nor, &nor);
			// �@����ۑ�
			m_aNor[(nCntZ * POLYGON_Z) + (nCntX * POLYGON_X) + nCntPolygon] = nor;

			// �O�ς��g���Ċe�|���S���̖@�������߂�
			D3DXVec3Cross(&nor, &vec2, &vec1);
			// ���K������
			D3DXVec3Normalize(&nor, &nor);
			// �@����ۑ�
			m_aNor[(nCntZ * POLYGON_Z) + (nCntX * POLYGON_X) + nCntPolygon + 1] = nor;
		}

		nCntPolygon += POLYGON_X;
	}

	for (int nCntPolygonZ = 0; nCntPolygonZ < POLYGON_Z + 1; nCntPolygonZ++)
	{
		for (int nCntPolygonX = 0; nCntPolygonX < POLYGON_X + 1; nCntPolygonX++)
		{
			if (nCntPolygonZ == 0)
			{// ��[
				if (nCntPolygonX == 0)
				{// ���[
					pVtx[0].nor = (m_aNor[0] + m_aNor[1]) / 2;
				}
				else if (nCntPolygonX == POLYGON_X)
				{// �E�[
					pVtx[POLYGON_X].nor = m_aNor[POLYGON_X + (POLYGON_X - 1)];
				}
				else
				{// ��[�̐^��
					pVtx[nCntPolygonX].nor =
						(m_aNor[(nCntPolygonX * 2) - 1] + m_aNor[((nCntPolygonX * 2) - 1) + 1] + m_aNor[((nCntPolygonX * 2) - 1) + 2]) / 3;
				}
			}
			else if (nCntPolygonZ == POLYGON_Z)
			{// ���[
				if (nCntPolygonX == 0)
				{// ���[
					pVtx[POLYGON_Z * (POLYGON_X + 1)].nor = m_aNor[2 * (POLYGON_X * (POLYGON_Z - 1))];
				}
				else if (nCntPolygonX == POLYGON_X)
				{// �E�[
					pVtx[(POLYGON_X * POLYGON_Z) + (POLYGON_X + POLYGON_Z)].nor =
						(m_aNor[(POLYGON_X * (POLYGON_X * (POLYGON_Z - 1))) + (2 * (POLYGON_X - 1))] +
							m_aNor[((2 * (POLYGON_X * (POLYGON_Z - 1))) + (2 * (POLYGON_X - 1))) + 1]) / 2;
				}
				else
				{// ���[�̐^��
					pVtx[(POLYGON_Z * (POLYGON_X + 1)) + nCntPolygonX].nor =
						(m_aNor[(POLYGON_Z - 1) * (POLYGON_X * 2) + (nCntPolygonX * 2) - 2] +
							m_aNor[((POLYGON_Z - 1) * (POLYGON_X * 2) + (nCntPolygonX * 2) - 2) + 1] +
							m_aNor[((POLYGON_Z - 1) * (POLYGON_X * 2) + (nCntPolygonX * 2) - 2) + 2]) / 3;
				}
			}
			else
			{// �^��
				if (nCntPolygonX == 0)
				{// ���[
					pVtx[(POLYGON_X + 1) * nCntPolygonZ].nor =
						(m_aNor[(nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)] +
							m_aNor[(nCntPolygonZ * (2 * POLYGON_X))] +
							m_aNor[(nCntPolygonZ * (2 * POLYGON_X) + 1)]) / 3;
				}
				else if (nCntPolygonX == POLYGON_X)
				{// �E�[
					pVtx[((nCntPolygonZ + 1) * POLYGON_X) + nCntPolygonZ].nor =
						(m_aNor[((POLYGON_X - 1) * 2 + ((POLYGON_X * 2)* (nCntPolygonZ - 1)))]
							+ m_aNor[((POLYGON_X - 1) * 2 + ((POLYGON_X * 2)* (nCntPolygonZ - 1))) + 1]
							+ m_aNor[((POLYGON_X - 1) * 2 + ((POLYGON_X * 2)* (nCntPolygonZ - 1))) + ((POLYGON_X * 2) + 1)]) / 3;
				}
				else
				{
					pVtx[(POLYGON_X + 2) + (nCntPolygonX - 1) + ((nCntPolygonZ * (POLYGON_X + 1)) - (POLYGON_X + 1))].nor =
						(m_aNor[((nCntPolygonX - 1) * 2) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X))] +
							m_aNor[((((nCntPolygonX - 1) * 2) + 1) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)))] +
							m_aNor[((((nCntPolygonX - 1) * 2) + 2) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)))] +
							m_aNor[((2 * POLYGON_X) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X))) + ((nCntPolygonX * 2) - 2)] +
							m_aNor[((2 * POLYGON_X) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)) + 1) + ((nCntPolygonX * 2) - 2)] +
							m_aNor[((2 * POLYGON_X) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)) + 2) + ((nCntPolygonX * 2) - 2)]) / 6;
				}
			}
		}
	}

	//====================================================================
	// �e���_�̖@�����Z�o����
	//====================================================================
	//pVtx[0].nor = (m_aNor[0] + m_aNor[1]) / 2;
	//pVtx[1].nor = (m_aNor[1] + m_aNor[2] + m_aNor[3]) / 3;
	//pVtx[2].nor = m_aNor[3];
	//pVtx[3].nor = (m_aNor[0] + m_aNor[5] + m_aNor[4]) / 3;
	//pVtx[4].nor = (m_aNor[0] + m_aNor[1] + m_aNor[2] + m_aNor[5] + m_aNor[6] + m_aNor[7]) / 6;
	//pVtx[5].nor = (m_aNor[3] + m_aNor[2] + m_aNor[7]) / 3;
	//pVtx[6].nor = m_aNor[4];
	/*pVtx[7].nor = (m_aNor[4] + m_aNor[5] + m_aNor[6]) / 3;*/
	//pVtx[8].nor = (m_aNor[6] + m_aNor[7]) / 2;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

#endif

#if(0)
	//====================================================================
	// �e�|���S���̖@�������߂�
	//====================================================================
	D3DXVECTOR3 aVec[3] = {};
	D3DXVECTOR3 aNor[8] = {};

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//================================================
	// ����|���S��
	//================================================
	// �x�N�g�������߂�
	aVec[0] = pVtx[3].pos - pVtx[0].pos;
	aVec[1] = pVtx[4].pos - pVtx[0].pos;
	aVec[2] = pVtx[1].pos - pVtx[0].pos;

	// �O�ς��g���Ė@�������߂�
	D3DXVec3Cross(&aNor[0], &aVec[1], &aVec[0]); // �@���`
	D3DXVec3Cross(&aNor[1], &aVec[2], &aVec[1]); // �@���a

	// ���K������
	D3DXVec3Normalize(&aNor[0], &aNor[0]); // �@���`
	D3DXVec3Normalize(&aNor[1], &aNor[1]); // �@���a

	//================================================
	// �E��|���S��
	//================================================
	// �x�N�g�������߂�
	aVec[0] = pVtx[4].pos - pVtx[1].pos;
	aVec[1] = pVtx[5].pos - pVtx[1].pos;
	aVec[2] = pVtx[2].pos - pVtx[1].pos;

	// �O�ς��g���Ė@�������߂�
	D3DXVec3Cross(&aNor[2], &aVec[1], &aVec[0]); // �@���b
	D3DXVec3Cross(&aNor[3], &aVec[2], &aVec[1]); // �@���c

	// ���K������
	D3DXVec3Normalize(&aNor[2], &aNor[2]); // �@���b
	D3DXVec3Normalize(&aNor[3], &aNor[3]); // �@���c

	//================================================
	// �����|���S��
	//================================================
	// �x�N�g�������߂�
	aVec[0] = pVtx[6].pos - pVtx[3].pos;
	aVec[1] = pVtx[7].pos - pVtx[3].pos;
	aVec[2] = pVtx[4].pos - pVtx[3].pos;

	// �O�ς��g���Ė@�������߂�
	D3DXVec3Cross(&aNor[4], &aVec[1], &aVec[0]); // �@���d
	D3DXVec3Cross(&aNor[5], &aVec[2], &aVec[1]); // �@���e

	// ���K������
	D3DXVec3Normalize(&aNor[4], &aNor[4]); // �@���d
	D3DXVec3Normalize(&aNor[5], &aNor[5]); // �@���e

	//================================================
	// �E���|���S��
	//================================================
	// �x�N�g�������߂�
	aVec[0] = pVtx[7].pos - pVtx[4].pos;
	aVec[1] = pVtx[8].pos - pVtx[4].pos;
	aVec[2] = pVtx[5].pos - pVtx[4].pos;

	// �O�ς��g���Ė@�������߂�
	D3DXVec3Cross(&aNor[6], &aVec[1], &aVec[0]); // �@���f
	D3DXVec3Cross(&aNor[7], &aVec[2], &aVec[1]); // �@���g

	// ���K������
	D3DXVec3Normalize(&aNor[6], &aNor[6]); // �@���f
	D3DXVec3Normalize(&aNor[7], &aNor[7]); // �@���g
	
	//====================================================================
	// �e���_�̖@�����Z�o����
	//====================================================================
	pVtx[0].nor = (aNor[0] + aNor[1]) / 2;
	pVtx[1].nor = (aNor[1] + aNor[2] + aNor[3]) / 3;
	pVtx[2].nor = aNor[3];
	pVtx[3].nor = (aNor[0] + aNor[5] + aNor[4]) / 3;
	pVtx[4].nor = (aNor[0] + aNor[1] + aNor[2] + aNor[5] + aNor[6] + aNor[7]) / 6;
	pVtx[5].nor = (aNor[3] + aNor[2] + aNor[7]) / 3;
	pVtx[6].nor = aNor[4];
	pVtx[7].nor = (aNor[4] + aNor[5] + aNor[6]) / 3;
	pVtx[8].nor = (aNor[6] + aNor[7]) / 2;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

#endif

	WORD *pIdx;	// �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntIdxZ = 0; nCntIdxZ < POLYGON_Z; nCntIdxZ++)
	{
		for (nCntIdxX = 0; nCntIdxX < POLYGON_X + 1; nCntIdxX++, nIdxCounter++)
		{
			pIdx[0] = nIdxCounter + POLYGON_X + 1;
			pIdx[1] = nIdxCounter;
			pIdx += 2;

			if (nCntIdxZ < POLYGON_Z - 1 && nCntIdxX == POLYGON_X)
			{
				pIdx[0] = nIdxCounter;
				pIdx[1] = nIdxCounter + (POLYGON_X + 1) + 1;
				pIdx += 2;
			}
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();

	LoadHeight();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMeshField::Uninit(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̊J��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// �I�u�W�F�N�g�̉��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMeshField::Update(void)
{
	// ���͏����擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

#if(1)
	int nCntPolygon = 0;
	int nCntNorPolygon = 0;

	for (int nCntZ = 0; nCntZ < POLYGON_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX < POLYGON_X; nCntX++)
		{
			D3DXVECTOR3 *pPos0, *pPos1, *pPos2, *pPos3;
			D3DXVECTOR3 vec0, vec1, vec2;
			D3DXVECTOR3 nor;

			// ����̃|���S���̂Q�̃x�N�g������@�����Z�o
			pPos0 = &pVtx[nCntX + nCntZ + nCntPolygon].pos;
			pPos1 = &pVtx[(nCntX + nCntZ + nCntPolygon) + (POLYGON_X + 1)].pos;
			pPos2 = &pVtx[(nCntX + nCntZ + nCntPolygon) + (POLYGON_X + 1) + 1].pos;
			pPos3 = &pVtx[(nCntX + nCntZ + nCntPolygon) + 1].pos;

			vec0 = *pPos1 - *pPos0;
			vec1 = *pPos2 - *pPos0;
			vec2 = *pPos3 - *pPos0;

			// �O�ς��g���Ċe�|���S���̖@�������߂�
			D3DXVec3Cross(&nor, &vec1, &vec0);
			// ���K������
			D3DXVec3Normalize(&nor, &nor);
			// �@����ۑ�
			m_aNor[(nCntZ * 2) + (nCntX * 2) + nCntNorPolygon] = nor;

			// �O�ς��g���Ċe�|���S���̖@�������߂�
			D3DXVec3Cross(&nor, &vec2, &vec1);
			// ���K������
			D3DXVec3Normalize(&nor, &nor);
			// �@����ۑ�
			m_aNor[(nCntZ * 2) + (nCntX * 2) + nCntNorPolygon + 1] = nor;
		}

		nCntPolygon += POLYGON_X;
		nCntNorPolygon += (POLYGON_X * 2) - 2;
	}

	int nData0 = 0;
	int nData1 = 0;

	for (int nCntPolygonZ = 0; nCntPolygonZ < POLYGON_Z + 1; nCntPolygonZ++)
	{
		for (int nCntPolygonX = 0; nCntPolygonX < POLYGON_X + 1; nCntPolygonX++)
		{
			if (nCntPolygonZ == 0)
			{// ��[
				if (nCntPolygonX == 0)
				{// ���[
					pVtx[0].nor = (m_aNor[0] + m_aNor[1]) / 2;
				}
				else if (nCntPolygonX == POLYGON_X)
				{// �E�[
					pVtx[POLYGON_X].nor = m_aNor[POLYGON_X + (POLYGON_X - 1)];
				}
				else
				{// ��[�̐^��
					pVtx[nCntPolygonX].nor =
						(m_aNor[(nCntPolygonX * 2) - 1] + m_aNor[((nCntPolygonX * 2) - 1) + 1] + m_aNor[((nCntPolygonX * 2) - 1) + 2]) / 3;
				}
			}
			else if (nCntPolygonZ == POLYGON_Z)
			{// ���[
				if (nCntPolygonX == 0)
				{// ���[
					pVtx[POLYGON_Z * (POLYGON_X + 1)].nor = m_aNor[2 * (POLYGON_X * (POLYGON_Z - 1))];
				}
				else if (nCntPolygonX == POLYGON_X)
				{// �E�[
					pVtx[(POLYGON_X * POLYGON_Z) + (POLYGON_X + POLYGON_Z)].nor =
						(m_aNor[(2 * (POLYGON_X * (POLYGON_Z - 1))) + (2 * (POLYGON_X - 1))] +
							m_aNor[((2 * (POLYGON_X * (POLYGON_Z - 1))) + (2 * (POLYGON_X - 1))) + 1]) / 2;
				}
				else
				{// ���[�̐^��
					pVtx[(POLYGON_Z * (POLYGON_X + 1)) + nCntPolygonX].nor =
						(m_aNor[(POLYGON_Z - 1) * (POLYGON_X * 2) + (nCntPolygonX * 2) - 2] +
							m_aNor[((POLYGON_Z - 1) * (POLYGON_X * 2) + (nCntPolygonX * 2) - 2) + 1] +
							m_aNor[((POLYGON_Z - 1) * (POLYGON_X * 2) + (nCntPolygonX * 2) - 2) + 2]) / 3;
				}
			}
			else
			{// �^��
				if (nCntPolygonX == 0)
				{// ���[
					pVtx[(POLYGON_X + 1) * nCntPolygonZ].nor =
						(m_aNor[(nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)] +
						m_aNor[(nCntPolygonZ * (2 * POLYGON_X))] +
						m_aNor[(nCntPolygonZ * (2 * POLYGON_X) + 1)]) / 3;
				}
				else if (nCntPolygonX == POLYGON_X)
				{// �E�[
					pVtx[((nCntPolygonZ + 1) * POLYGON_X) + nCntPolygonZ].nor =
						(m_aNor[((POLYGON_X - 1) * 2 + ((POLYGON_X * 2)* (nCntPolygonZ - 1)))]
							+ m_aNor[((POLYGON_X - 1) * 2 + ((POLYGON_X * 2)* (nCntPolygonZ - 1))) + 1]
							+ m_aNor[((POLYGON_X - 1) * 2 + ((POLYGON_X * 2)* (nCntPolygonZ - 1))) + ((POLYGON_X * 2) + 1)]) / 3;
				}
				else
				{
					pVtx[(POLYGON_X + 2) + (nCntPolygonX - 1) + ((nCntPolygonZ * (POLYGON_X + 1)) - (POLYGON_X + 1))].nor =
						(m_aNor[((nCntPolygonX - 1) * 2) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X))] +
						 m_aNor[((((nCntPolygonX - 1) * 2) + 1) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)))] +
						 m_aNor[((((nCntPolygonX - 1) * 2) + 2) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)))] +
						 m_aNor[((2 * POLYGON_X) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X))) + ((nCntPolygonX * 2) - 1)] +
						 m_aNor[((2 * POLYGON_X) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)) + 1) + ((nCntPolygonX * 2) - 1)] +
						 m_aNor[((2 * POLYGON_X) + ((nCntPolygonZ * (2 * POLYGON_X)) - (2 * POLYGON_X)) + 2) + ((nCntPolygonX * 2) - 1)]) / 6;

					nData0 = (POLYGON_X + 2) + (nCntPolygonX - 1) + ((nCntPolygonZ * (POLYGON_X + 1)) - (POLYGON_X + 1));
					/*nData1 = ((2 * (POLYGON_X * (POLYGON_Z - 1))) + (2 * (POLYGON_X - 1))) + 1;*/
				}
			}
		}
	}

	int nVtxCounter = 0;

	if (pInputKeyboard->GetTrigger(DIK_0) == true)
	{// 0�L�[�������ꂽ��
		SaveHeight();
	}

	//if (pInputKeyboard->GetTrigger(DIK_9) == true)
	//{// 9�L�[�������ꂽ��
	//	LoadHeight();
	//}
	
#endif

#ifdef _DEBUG
	/*for (int nCntNor = 0; nCntNor < 25; nCntNor++)
	{
		CDebugProc::Print("cncfcfcfc", "�@��[", nCntNor, "] : (", pVtx[nCntNor].nor.x, ", ", pVtx[nCntNor].nor.y, ", ", pVtx[nCntNor].nor.z, ")");
	}*/

	/*for (int nCntNor = 0; nCntNor < 25; nCntNor++)
	{
		CDebugProc::Print("cncfcfcfc", "�@��[", nCntNor, "] : (", m_aNor[nCntNor].x, ", ", m_aNor[nCntNor].y, ", ", m_aNor[nCntNor].z, ")");
	}*/

	//for (int nCntNor = 0; nCntNor < 16; nCntNor++)
	//{
	//	CDebugProc::Print("cncfcfcfc", "pos[", nCntNor, "] : (", pVtx[nCntNor].pos.x, ", ", pVtx[nCntNor].pos.y, ", ", pVtx[nCntNor].pos.z, ")");
	//}

#endif

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMeshField::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �����_���[���擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{// �f�o�C�X�̎擾
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

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVerTex, 0, m_nNumPolygon);
}

//=============================================================================
// �������擾
//============================================================================
float CMeshField::GetHeight(D3DXVECTOR3 pos)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	bool bX = false;
	bool bZ = false;
	bool abX[POLYGON_X];
	bool abZ[POLYGON_Z];
	bool bRand = false;

	for (int nCount = 0; nCount < POLYGON_X; nCount++)
	{
		abX[nCount] = false;
	}

	for (int nCount = 0; nCount < POLYGON_Z; nCount++)
	{
		abZ[nCount] = false;
	}

	// ���������ǂ̃|���S���ɏ���Ă��邩
	for (int nCntZ = 0; nCntZ < POLYGON_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX < POLYGON_X; nCntX++)
		{
			if (pos.x >= -MESHFIELD_SIZE + (nCntX * MESHFIELD_SIZE) && pos.x < (nCntX * MESHFIELD_SIZE))
			{
				bX = true;
				abX[nCntX] = true;
			}

			if (pos.z <= MESHFIELD_SIZE - (nCntZ * MESHFIELD_SIZE) && pos.z > (nCntZ * -MESHFIELD_SIZE))
			{
				bZ = true;
				abZ[nCntZ] = true;
			}
		}
	}

	int nCount = 0;

	for (int nCntZ = 0; nCntZ < POLYGON_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX < POLYGON_X; nCntX++)
		{
			if (abX[nCntX] == true && abZ[nCntZ] == true)
			{
				//CDebugProc::Print("nc", nCntX + nCntZ + nCount, ": ��");
				bRand = true;
			}
			else
			{
				//CDebugProc::Print("nc", nCntX + nCntZ + nCount, ": �O");

				bRand = false;
			}
		}
		nCount+= 2;
	}


	int nCntPolygon = 0;
	int nCntNorPolygon = 0;

	for (int nCntZ = 0; nCntZ < POLYGON_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX < POLYGON_X; nCntX++)
		{
			if (abX[nCntX] == true && abZ[nCntZ] == true)
			{
				D3DXVECTOR3 *pPos0, *pPos1, *pPos2, *pPos3;
				D3DXVECTOR3 vec0, vec1, vec2, vec3;
				D3DXVECTOR3 nor0, nor1;
				float fData = 0.0f;

				// ����̃|���S���̂Q�̃x�N�g������@�����Z�o
				pPos0 = &pVtx[nCntX + nCntZ + nCntPolygon].pos;							// ����
				pPos1 = &pVtx[(nCntX + nCntZ + nCntPolygon) + (POLYGON_X + 1)].pos;		// ����
				pPos2 = &pVtx[(nCntX + nCntZ + nCntPolygon) + (POLYGON_X + 1) + 1].pos;	// �E��
				pPos3 = &pVtx[(nCntX + nCntZ + nCntPolygon) + 1].pos;					// �E��

				vec0 = *pPos1 - *pPos0;	// ���̕ӂ̃x�N�g��
				vec1 = *pPos2 - *pPos0;	// ���ォ��E���̃x�N�g��
				vec2 = *pPos3 - *pPos0;	// ��̕ӂ̃x�N�g��
				vec3 = pos - *pPos0;

				// �O�ς��g���Ċe�|���S���̖@�������߂�
				D3DXVec3Cross(&nor0, &vec1, &vec0);
				// ���K������
				D3DXVec3Normalize(&nor0, &nor0);

				// �O�ς��g���Ċe�|���S���̖@�������߂�
				D3DXVec3Cross(&nor1, &vec2, &vec1);
				// ���K������
				D3DXVec3Normalize(&nor1, &nor1);

				fData = ((vec1.z * vec3.x) - (vec1.x * vec3.z));

				if (fData > 0)
				{
					pos.y = (((nor0.x * (pos.x - pPos0->x) + nor0.z * (pos.z - pPos0->z)) / -nor0.y) + pPos0->y);
				}
				else
				{
					pos.y = (((nor1.x * (pos.x - pPos2->x) + nor1.z * (pos.z - pPos2->z)) / -nor1.y) + pPos2->y);
				}

				break;
			}
		}

		nCntPolygon += POLYGON_X;
		nCntNorPolygon += (POLYGON_X * 2) - 2;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return pos.y;
}

//=============================================================================
// ������ݒ�
//============================================================================
void CMeshField::SetHeight(D3DXVECTOR3 pos, float fValue, float fRange)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < POLYGON_Z + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < POLYGON_X + 1; nCntX++)
		{
			// pos����Ώۂ̒��_�܂ł̋���
			float fLength = sqrtf
				// x�̋���
				((pos.x - pVtx->pos.x) * (pos.x - pVtx->pos.x) +
				// z�̋���
				(pos.z - pVtx->pos.z) * (pos.z - pVtx->pos.z));

			if (fLength < fRange)
			{// �Ώۂ̒��_���͈͓�
				// �͈͓��ł̋����̔䗦�ɉ���������
				float fHeight = cosf((fLength / fRange) * (D3DX_PI * 0.5f)) * fValue;

				pVtx->pos.y += fHeight;
			}

			// ��������
			g_aHeight[nCntZ][nCntX] = pVtx->pos.y;

			pVtx++;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �������Z�[�u
//=============================================================================
void CMeshField::SaveHeight(void)
{
	FILE *pFile = NULL;

	// �t�@�C���ɏ�������
	pFile = fopen(TOUL_FILENAME, "wb");

	if (pFile != NULL)
	{
		fwrite(&g_aHeight[0][0], sizeof(float), POLYGON_Z * POLYGON_X, pFile);	// �f�[�^�̃A�h���X,�f�[�^�̃T�C�Y,�f�[�^�̌�,�t�@�C���|�C���^

		fclose(pFile);
	}
	else
	{
		printf("number.txt���J���܂���ł����B\n");
	}

}

//=============================================================================
// ���������[�h
//=============================================================================
void CMeshField::LoadHeight(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	FILE *pFile = NULL;

	pFile = fopen(TOUL_FILENAME, "rb");

	if (pFile != NULL)
	{
		fread(&g_aHeight[0][0], sizeof(float), POLYGON_Z * POLYGON_X, pFile);	// �f�[�^�̃A�h���X,�f�[�^�̃T�C�Y,�f�[�^�̌�,�t�@�C���|�C���^

		fclose(pFile);
	}
	else
	{
		printf("number.txt���J���܂���ł����B\n");
	}

	for (int nCntZ = 0; nCntZ < POLYGON_Z + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < POLYGON_X + 1; nCntX++)
		{// pos����
			pVtx->pos.y = g_aHeight[nCntZ][nCntX];

			pVtx++;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}