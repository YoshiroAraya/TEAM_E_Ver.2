//---------------------------------------------------------------------
// �|�[�Y���� [pause.cpp]
// Author : Mikiya Meguro
//---------------------------------------------------------------------
#include "modeSelect.h"
#include "scene.h"
#include "scene2D.h"
#include "input.h"
#include "manager.h"
#include "fade.h"
#include "renderer.h"
#include "load.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//--------------------------------------------
//�ÓI�����o�ϐ��錾
//--------------------------------------------
LPDIRECT3DTEXTURE9 CModeSelect::m_pTexture[MAX_MODESELECTMENU] = {};
LPDIRECT3DTEXTURE9 CModeSelect::m_pTextureBG = NULL;
CModeSelect::SELECTMODE CModeSelect::m_SelectMode = CModeSelect::SELECTMODE_NONE;
bool CModeSelect::m_bModeSelect = false;

//--------------------------------------------
//�|�[�Y�N���X �R���X�g���N�^
//--------------------------------------------
CModeSelect::CModeSelect(int nPriority) : CScene(7)
{
}

//--------------------------------------------
//�|�[�Y�N���X �f�X�g���N�^
//--------------------------------------------
CModeSelect::~CModeSelect()
{
}

//--------------------------------------------
//�I�u�W�F�N�g�̐���
//--------------------------------------------
CModeSelect *CModeSelect::Create(D3DXVECTOR3 pos, float m_fWidth)
{
	//�|�[�Y�̃|�C���^
	CModeSelect *pModeSelect;
	pModeSelect = new CModeSelect;

	pModeSelect->m_InitPos = pos;
	pModeSelect->m_fWidth = m_fWidth;
	pModeSelect->m_fHeight = m_fWidth;
	//�|�[�Y�̏�����
	pModeSelect->Init(pos);
	//�|�[�Y�̏���Ԃ�
	return pModeSelect;
}


//=============================================================================
// ����������
//=============================================================================
HRESULT CModeSelect::Init(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < MAX_MODESELECTMENU; nCnt++)
	{
		m_apPolygon[nCnt] = NULL;
	}

	m_pTextureBG = CLoad::GetTexture(CLoad::TEXTURE_MODESELECT_BG);
	m_pTexture[0] = CLoad::GetTexture(CLoad::TEXTURE_MODESELECT_CONTINUE);
	m_pTexture[1] = CLoad::GetTexture(CLoad::TEXTURE_MODESELECT_RETRY);
	m_pTexture[2] = CLoad::GetTexture(CLoad::TEXTURE_MODESELECT_QUIT);

	//BG�̏�����
	m_apPolygonBG = CScene2D::Create(D3DXVECTOR3(m_InitPos.x, m_InitPos.y, m_InitPos.z));
	m_apPolygonBG->BindTexture(m_pTextureBG);
	m_apPolygonBG->SetWidthHeight(m_fWidth * 4, m_fHeight * 3);
	m_apPolygonBG->SetbDraw(true);



	for (int nCnt = 0; nCnt < MAX_MODESELECTMENU; nCnt++)
	{
		if (m_apPolygon[nCnt] == NULL)
		{	//�|���S���̏�����
			m_Pos[nCnt] = m_InitPos;
			m_Pos[nCnt].y = m_InitPos.y - m_fWidth - 60;
			//�ʒu�����炷
			m_Pos[nCnt].y = m_Pos[nCnt].y + ((m_fWidth + 60) * nCnt);
			m_apPolygon[nCnt] = CScene2D::Create(D3DXVECTOR3(m_Pos[nCnt].x, m_Pos[nCnt].y, m_Pos[nCnt].z));
			m_apPolygon[nCnt]->SetWidthHeight(m_fWidth * 2.5f, m_fHeight * 1.0f);
			m_apPolygon[nCnt]->BindTexture(m_pTexture[nCnt]);
			m_apPolygon[nCnt]->SetbDraw(true);
		}
	}

	////���_���ւ̃|�C���^
	//VERTEX_2D *pVtx;
	////���_�o�b�t�@���擾
	//m_pVtxBuff = m_apPolygonBG[1]->GetBuff();

	////���_�o�b�t�@�����b�N�����_�f�[�^�̃|�C���^���擾
	//m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	////���_�J���[
	//pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
	//pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
	//pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
	//pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
	//// ���_�o�b�t�@���A�����b�N����
	//m_pVtxBuff->Unlock();

	//�l�̏�����
	m_nSelect = 0;
	m_TexMove = D3DXVECTOR3(0, 0, 0);
	m_aModeSelectMenu[m_nSelect].select = SELECTTYPE_SELECT;
	m_aModeSelectMenu[1].select = SELECTTYPE_NONE;
	m_aModeSelectMenu[2].select = SELECTTYPE_NONE;
	m_bModeSelect = true;
	//�I�u�W�F�N�g��ނ̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_MODESELECT);

	return S_OK;
}

//=============================================================================
// �|�[�Y�N���X �I������
//=============================================================================
void CModeSelect::Uninit(void)
{

	//������j��
	Release();
}

//=============================================================================
// �|�[�Y�N���X �X�V����
//=============================================================================
void CModeSelect::Update(void)
{
	//�L�[�{�[�h�����擾
	CInputKeyboard *pInput = CManager::GetInputKeyboard();
	//�W���C�p�b�h�����擾
	CXInputJoyPad *pXInput = NULL;
	pXInput = CManager::GetXInput();
	//�t�F�[�h���擾
	CManager *pManager = NULL;
	CFade *pFade = pManager->GetFade();


	//�T�E���h�����擾
	CSound *pSound = CManager::GetSound(0);

	//�\���ؑ�
	for (int nCnt = 0; nCnt < MAX_MODESELECTMENU; nCnt++)
	{
		m_apPolygon[nCnt]->SetbDraw(m_bModeSelect);
	}
	m_apPolygonBG->SetbDraw(m_bModeSelect);


#ifdef  _DEBUG
	if (pInput->GetTrigger(DIK_O) == true)
	{//TRUE�Ȃ�FALSE FALSE�Ȃ�TRUE
		m_bModeSelect = m_bModeSelect ? false : true;
	}
#endif

	//�|�[�Y��
	if (m_bModeSelect == true)
	{
		//�I������
		if (pInput->GetTrigger(DIK_DOWN) == true || pInput->GetTrigger(DIK_S) == true
			|| pXInput->GetTrigger(XINPUT_GAMEPAD_DPAD_DOWN,0) == true
			|| pXInput->GetTrigger(XINPUT_GAMEPAD_DPAD_DOWN,1) == true)
		{
			//pSound->PlaySound(pSound->SOUND_LABEL_SE_SELECT);
			m_aModeSelectMenu[m_nSelect].select = SELECTTYPE_NONE;
			m_nSelect = (m_nSelect + 1) % 3;
			m_aModeSelectMenu[m_nSelect].select = SELECTTYPE_SELECT;
		}
		else if (pInput->GetTrigger(DIK_UP) == true || pInput->GetTrigger(DIK_W) == true
			|| pXInput->GetTrigger(XINPUT_GAMEPAD_DPAD_UP, 0) == true
			|| pXInput->GetTrigger(XINPUT_GAMEPAD_DPAD_UP, 1) == true)
		{
			//pSound->PlaySound(pSound->SOUND_LABEL_SE_SELECT);
			m_aModeSelectMenu[m_nSelect].select = SELECTTYPE_NONE;
			m_nSelect = (m_nSelect + 2) % 3;
			m_aModeSelectMenu[m_nSelect].select = SELECTTYPE_SELECT;
		}
		//�G���^�[�L�[
		if (pInput->GetTrigger(DIK_RETURN) == true
			|| pXInput->GetPress(XINPUT_GAMEPAD_A, 0) == true
			|| pXInput->GetPress(XINPUT_GAMEPAD_A, 1) == true)
		{
			//pSound->PlaySound(pSound->SOUND_LABEL_SE_CANCEL);
			if (m_nSelect == 0)
			{
				m_SelectMode = SELECTMODE_CONTINUE;
			}
			else if (m_nSelect == 1)
			{
				m_SelectMode = SELECTMODE_RETRY;
			}
			else if (m_nSelect == 2)
			{
				m_SelectMode = SELECTMODE_QUIT;
			}

			switch (m_SelectMode)
			{
			case SELECTMODE_CONTINUE:
				m_bModeSelect = false;
				CFade::SetFade(CManager::MODE_GAME, pFade->FADE_OUT);
				//SetbModeSelect(m_bModeSelect);
				break;
			case SELECTMODE_RETRY:
				m_bModeSelect = false;
				pManager->SetCharaSelectMode(true);
				pManager->SetNumPlayerMode(1);
				CFade::SetFade(CManager::MODE_TITLE, pFade->FADE_OUT);
				//SetbModeSelect(m_bModeSelect);
				break;
			case SELECTMODE_QUIT:
				m_bModeSelect = false;
				pManager->SetCharaSelectMode(false);
				CFade::SetFade(CManager::MODE_TITLE, pFade->FADE_OUT);
				//SetbModeSelect(m_bModeSelect);
				break;
			}
		}

		//P�L�[�ŃL�����Z��
		if (pInput->GetTrigger(DIK_P) == true
			|| pXInput->GetTrigger(XINPUT_GAMEPAD_START, 0) == true
			|| pXInput->GetTrigger(XINPUT_GAMEPAD_START, 1) == true)
		{
			m_nSelect = 0;
			m_aModeSelectMenu[m_nSelect].select = SELECTTYPE_SELECT;
			m_aModeSelectMenu[1].select = SELECTTYPE_NONE;
			m_aModeSelectMenu[2].select = SELECTTYPE_NONE;
		}

		//�F�ς�
		for (int nCnt = 0; nCnt < MAX_MODESELECTMENU; nCnt++)
		{
			if (m_aModeSelectMenu[nCnt].select == SELECTTYPE_SELECT)
			{//�I�𒆂̐F
				m_aModeSelectMenu[nCnt].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			}
			else
			{//���I���̐F
				m_aModeSelectMenu[nCnt].col = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
			}

			//���_���ւ̃|�C���^
			VERTEX_2D *pVtx;
			//���_�o�b�t�@���擾
			m_pVtxBuff = m_apPolygon[nCnt]->GetBuff();
			//���_�o�b�t�@�����b�N�����_�f�[�^�̃|�C���^���擾
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			//���_�J���[
			pVtx[0].col = m_aModeSelectMenu[nCnt].col;
			pVtx[1].col = m_aModeSelectMenu[nCnt].col;
			pVtx[2].col = m_aModeSelectMenu[nCnt].col;
			pVtx[3].col = m_aModeSelectMenu[nCnt].col;
			// ���_�o�b�t�@���A�����b�N����
			m_pVtxBuff->Unlock();
		}

		//���_���ւ̃|�C���^
		VERTEX_2D *pVtx;
		//���_�o�b�t�@���擾
		m_pVtxBuff = m_apPolygonBG->GetBuff();
		//���_�o�b�t�@�����b�N�����_�f�[�^�̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		//���_�e�N�X�`��
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// �|�[�Y�N���X �`�揈��
//=============================================================================
void CModeSelect::Draw(void)
{
}

//=============================================================================
// �|�[�Y�N���X�@�I���������[�h���擾
//=============================================================================
CModeSelect::SELECTMODE * CModeSelect::GetModeSelectMode(void)
{
	return &m_SelectMode;
}

//=============================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CModeSelect::Load(void)
{

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j������
//=============================================================================
void CModeSelect::UnLoad(void)
{
	for (int nCnt = 0; nCnt < MAX_MODESELECTMENU; nCnt++)
	{
		// �e�N�X�`���̔j��
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}

	if (m_pTextureBG != NULL)
	{
		m_pTextureBG->Release();
		m_pTextureBG = NULL;
	}
}
//=============================================================================
// �|�[�Y�̏�Ԃ��擾
//=============================================================================
bool CModeSelect::GetModeSelectBool(void)
{
	return m_bModeSelect;
}

//=============================================================================
// �|�[�Y�̏�Ԃ�ݒ�
//=============================================================================
void CModeSelect::SetModeSelectBool(bool ModeSelectBool)
{
	m_bModeSelect = ModeSelectBool;
}

