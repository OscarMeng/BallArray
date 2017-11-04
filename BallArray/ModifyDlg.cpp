// ModifyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BallArray.h"
#include "ModifyDlg.h"
#include "AddCurveDlg.h"
#include "afxdialogex.h"
#include <locale.h>

// CModifyDlg �Ի���

IMPLEMENT_DYNAMIC(CModifyDlg, CDialogEx)

CModifyDlg::CModifyDlg(CWnd* pParent /*=NULL*/)
: m_pCtrlView((CControlView*)pParent),
	CDialogEx(CModifyDlg::IDD, pParent)
{
	//m_pCtrlView = (CControlView*)pParent;
	for (int i = 0; i < CURVE_NUM; ++i)
	{
		p_tPrjDir[i] = new ProjectDir;
	}

	for (int i = 0; i< m_pCtrlView->m_nCurveNum; i++)
	{
		strcpy(p_tPrjDir[i]->sCurveType, m_pCtrlView->p_tPrjDir[i]->sCurveType);//������и���
		strcpy(p_tPrjDir[i]->sCurveName, m_pCtrlView->p_tPrjDir[i]->sCurveName);

		p_tPrjDir[i]->nCurveTime = m_pCtrlView->p_tPrjDir[i]->nCurveTime;
		p_tPrjDir[i]->nPauseTime = m_pCtrlView->p_tPrjDir[i]->nPauseTime;

		strcpy(p_tPrjDir[i]->sCurveUndet2, m_pCtrlView->p_tPrjDir[i]->sCurveUndet2);
		strcpy(p_tPrjDir[i]->sCurveUndet3, m_pCtrlView->p_tPrjDir[i]->sCurveUndet3);
		//memcpy(p_stPrjDir[i]->sCurveName, m_pCtrlView->PrjDir[i]->sCurveName, sizeof(m_pCtrlView->PrjDir[i]->sCurveName));
	}
}

CModifyDlg::~CModifyDlg()
{
	for (int i = 0; i < CURVE_NUM; ++i)
	{
		delete p_tPrjDir[i];
	}
}

void CModifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODIFY_LIST, m_clModify);
	DDX_Control(pDX, IDC_MBT_ADD, m_btAdd);
	DDX_Control(pDX, IDC_MBT_UP, m_btUp);
	DDX_Control(pDX, IDC_MBT_DOWN, m_btDown);
	DDX_Control(pDX, IDC_MBT_SAVE, m_btSave);
	DDX_Control(pDX, IDC_MBT_DELETE, m_btDelete);
}


BEGIN_MESSAGE_MAP(CModifyDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MBT_ADD, &CModifyDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_MBT_UP, &CModifyDlg::OnBnClickedBtnUp)
	ON_BN_CLICKED(IDC_MBT_DOWN, &CModifyDlg::OnBnClickedBtnDown)
	ON_BN_CLICKED(IDC_MBT_SAVE, &CModifyDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_MBT_DELETE, &CModifyDlg::OnBnClickedBtnDelete)
	ON_NOTIFY(NM_CLICK, IDC_MODIFY_LIST, &CModifyDlg::OnNMClickListItem)
	ON_BN_CLICKED(IDC_MBT_RESIVE, &CModifyDlg::OnBnClickedMbtResive)
END_MESSAGE_MAP()


// CModifyDlg ��Ϣ�������


BOOL CModifyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rect;
	// ��ȡ�б���ͼ�ؼ���λ�úʹ�С   
	m_clModify.GetClientRect(&rect);
	//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��| �����ߣ�ֻ������report����listctrl��| ��������
	m_clModify.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_ONECLICKACTIVATE);
	CImageList   m_imageList;
	m_imageList.Create(1, rect.Height() / 10, ILC_COLOR24, 1, 1);   //1��ͼƬ��30�Ǹ߶� 
	m_clModify.SetImageList(&m_imageList, LVSIL_SMALL);
	m_clModify.InsertColumn(0, NULL, LVCFMT_CENTER, 0);
	m_clModify.InsertColumn(1, _T("���"), LVCFMT_CENTER, rect.Width() / 7, 0);
	m_clModify.InsertColumn(2, _T("��������"), LVCFMT_CENTER, rect.Width() * 3 / 7, 1);
	m_clModify.InsertColumn(3, _T("����ʱ��"), LVCFMT_CENTER, rect.Width() * 3 / 14, 2);
	m_clModify.InsertColumn(4, _T("ͣ��ʱ��"), LVCFMT_CENTER, rect.Width() * 3 / 14, 3);
	m_clModify.DeleteColumn(0);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	for (int index = 0; index < m_pCtrlView->m_nCurveNum; index++)
	{
		CString rowNum;
		CString curverName;
		CString runTime;
		CString pauseTime;

		curverName = m_pCtrlView->p_tPrjDir[index]->sCurveName;//char����ֱ�Ӹ���CString
		runTime.Format(_T("%d"), m_pCtrlView->p_tPrjDir[index]->nCurveTime);
		pauseTime.Format(_T("%d"), m_pCtrlView->p_tPrjDir[index]->nPauseTime);

		rowNum.Format(_T("%d"), index + 1);
		m_clModify.InsertItem(index, rowNum);
		m_clModify.SetItemText(index, 1, curverName);
		m_clModify.SetItemText(index, 2, runTime + _T("s"));
		m_clModify.SetItemText(index, 3, pauseTime + _T("s"));
	}

	m_nItem = -1;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//��������
void CModifyDlg::Swap(ProjectDir* lp, ProjectDir* rp)
{
	ProjectDir pd;
	if (lp == rp)  return;

	strcpy(pd.sCurveType, lp->sCurveType);
	strcpy(lp->sCurveType, rp->sCurveType);
	strcpy(rp->sCurveType, pd.sCurveType);

	strcpy(pd.sCurveName, lp->sCurveName);
	strcpy(lp->sCurveName, rp->sCurveName);
	strcpy(rp->sCurveName, pd.sCurveName);

	pd.nCurveTime = lp->nCurveTime;
	lp->nCurveTime = rp->nCurveTime;
	rp->nCurveTime = pd.nCurveTime;

	pd.nPauseTime = lp->nPauseTime;
	lp->nPauseTime = rp->nPauseTime;
	rp->nPauseTime = pd.nPauseTime;

	strcpy(pd.sCurveUndet2, lp->sCurveUndet2);
	strcpy(lp->sCurveUndet2, rp->sCurveUndet2);
	strcpy(rp->sCurveUndet2, pd.sCurveUndet2);

	strcpy(pd.sCurveUndet3, lp->sCurveUndet3);
	strcpy(lp->sCurveUndet3, rp->sCurveUndet3);
	strcpy(rp->sCurveUndet3, pd.sCurveUndet3);
}

//�����б���Ӧ����
void CModifyDlg::OnNMClickListItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_nItem = pNMItemActivate->iItem;//ѡ�е���
	m_clModify.Invalidate();//�ػ�
	m_nNum = m_clModify.GetItemCount();
	for (int i = 0; i < m_nNum; i++)
	{
		if (i != m_nItem)
		{
			m_clModify.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED | LVS_SHOWSELALWAYS | LVS_SINGLESEL);
		}
	}
	*pResult = 0;
}

void CModifyDlg::OnBnClickedBtnUp()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_nItem < 0)
	{
		AfxMessageBox(_T("��û��ѡ���κ����ݣ�"));
		return;         //��ֹ�������˳�
	}

	int nNum = m_clModify.GetItemCount();
	int nIndex = -1;

	for (int i = 0; i < nNum;i++)
	{
		if (m_clModify.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			CString currentName, upName;
			CString currentTime, upTime;
			CString currentPauseTm, upPauseTm;
			if (i  > 0)
			{
				currentName = m_clModify.GetItemText(i, 1);
				upName = m_clModify.GetItemText(i - 1, 1);
				currentTime = m_clModify.GetItemText(i, 2);
				upTime = m_clModify.GetItemText(i - 1, 2);
				currentPauseTm = m_clModify.GetItemText(i, 3);
				upPauseTm= m_clModify.GetItemText(i - 1, 3);
				m_clModify.SetItemText(i, 1, upName);
				m_clModify.SetItemText(i - 1, 1, currentName);
				m_clModify.SetItemText(i, 2, upTime);
				m_clModify.SetItemText(i - 1, 2, currentTime);
				m_clModify.SetItemText(i, 3, upPauseTm);
				m_clModify.SetItemText(i - 1, 3, currentPauseTm);

				Swap(p_tPrjDir[i], p_tPrjDir[i - 1]);//��������
				nIndex = i;
			}
		}
	}
	m_clModify.SetItemState(nIndex - 1, LVIS_SELECTED | LVIS_FOCUSED | LVS_SHOWSELALWAYS | LVS_SINGLESEL, LVIS_SELECTED | LVIS_FOCUSED | LVS_SHOWSELALWAYS | LVS_SINGLESEL);
	m_clModify.SetItemState(nIndex, 0, LVIS_SELECTED | LVIS_FOCUSED | LVS_SHOWSELALWAYS | LVS_SINGLESEL);
}


void CModifyDlg::OnBnClickedBtnDown()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_nItem < 0)
	{
		AfxMessageBox(_T("��û��ѡ���κ����ݣ�"));
		return;         //��ֹ�������˳�
	}

	int nNum = m_clModify.GetItemCount();
	int nIndex = -1;

	for (int i = 0; i < nNum; i++)
	{
		if (m_clModify.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			CString currentName, downName;
			CString currentTime, downTime;
			CString currentPauseTm, downPauseTm;
			if (i + 1 < nNum)
			{
				currentName = m_clModify.GetItemText(i, 1);
				downName = m_clModify.GetItemText(i + 1, 1);
				currentTime = m_clModify.GetItemText(i, 2);
				downTime = m_clModify.GetItemText(i + 1, 2);
				currentPauseTm = m_clModify.GetItemText(i, 3);
				downPauseTm = m_clModify.GetItemText(i + 1, 3);
				m_clModify.SetItemText(i, 1, downName);
				m_clModify.SetItemText(i + 1, 1, currentName);
				m_clModify.SetItemText(i, 2, downTime);
				m_clModify.SetItemText(i + 1, 2, currentTime);
				m_clModify.SetItemText(i, 3, downPauseTm);
				m_clModify.SetItemText(i + 1, 3, currentPauseTm);

				Swap(p_tPrjDir[i], p_tPrjDir[i + 1]);//��������
				nIndex = i;
			}
		}
	}
	m_clModify.SetItemState(nIndex + 1, LVIS_SELECTED | LVIS_FOCUSED | LVS_SHOWSELALWAYS | LVS_SINGLESEL, LVIS_SELECTED | LVIS_FOCUSED | LVS_SHOWSELALWAYS | LVS_SINGLESEL);
	m_clModify.SetItemState(nIndex, 0, LVIS_SELECTED | LVIS_FOCUSED | LVS_SHOWSELALWAYS | LVS_SINGLESEL);
}


//���
void CModifyDlg::OnBnClickedBtnAdd()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_nFlag = 1;
	int nNum = m_clModify.GetItemCount();
	CAddCurveDlg addCurveDlg(nNum, this);
	if (addCurveDlg.DoModal() == IDOK)
	{
		CString itos;
		
		itos.Format(_T("%d"), m_nRowNm);//����
		m_clModify.InsertItem(m_nRowNm-1, itos);
		m_clModify.SetItemText(m_nRowNm-1, 1, m_sName);
		itos.Format(_T("%d"), m_nRunTm);
		m_clModify.SetItemText(m_nRowNm-1, 2, itos + _T("s"));
		itos.Format(_T("%d"), m_nPauseTm);
		m_clModify.SetItemText(m_nRowNm-1, 3, itos + _T("s"));

		//���������Ϊ���һ��
		if (m_nRowNm == nNum + 1)
		{
			USES_CONVERSION;
			char * pch = T2A(m_sType);
			strcpy(p_tPrjDir[m_nRowNm - 1]->sCurveType, pch);
			pch = T2A(m_sName);
			strcpy(p_tPrjDir[m_nRowNm - 1]->sCurveName, pch);
			p_tPrjDir[m_nRowNm - 1]->nCurveTime = m_nRunTm;
			p_tPrjDir[m_nRowNm - 1]->nPauseTime = m_nPauseTm;
			pch = T2A(_T("ab"));
			strcpy(p_tPrjDir[m_nRowNm - 1]->sCurveUndet2, pch);
			pch = T2A(_T("cd"));
			strcpy(p_tPrjDir[m_nRowNm - 1]->sCurveUndet3, pch);
		}
		else	//�����������Ϊ���һ��
		{
			for (int i = m_nRowNm; i < nNum + 1; i++)//�����������µ����
			{
				CString currentName;
				CString currentRunTm;
				CString currentPauseTm;

				itos.Format(_T("%d"), i + 1);
				m_clModify.SetItemText(i, 0, itos);
			}
			USES_CONVERSION;
			char * pch = T2A(m_sType);
			strcpy(p_tPrjDir[nNum]->sCurveType, pch);
			pch = T2A(m_sName);
			strcpy(p_tPrjDir[nNum]->sCurveName, pch);
			p_tPrjDir[nNum]->nCurveTime = m_nRunTm;
			p_tPrjDir[nNum]->nPauseTime = m_nPauseTm;
			pch = T2A(_T("ab"));
			strcpy(p_tPrjDir[nNum]->sCurveUndet2, pch);
			pch = T2A(_T("cd"));
			strcpy(p_tPrjDir[nNum]->sCurveUndet3, pch);

			for (int j = nNum; j >= m_nRowNm; j--)
			{
				Swap(p_tPrjDir[j - 1], p_tPrjDir[j]);
			}
		}
	}
}

void CModifyDlg::OnBnClickedBtnSave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strPath;
	CString strTemp;
	CMyConstMeth MyMeth;

	strPath = MyMeth.ReadModuleFileNm();
	strPath = strPath + _T(CONFIGURE_FILE) + _T("\\") + _T(CONFIGURE_NAME);

	GetPrivateProfileString(_T("ProjectPath"), _T("Path"), _T(""), strTemp.GetBuffer(100), 100, strPath);
	strTemp.ReleaseBuffer();
	int a =p_tPrjDir[2]->nCurveTime;
	FILE* ff;
	errno_t err;
	CString str;

	err = _wfopen_s(&ff, strTemp, _T("wt"));
	if (err != 0)
	{
		str.Format(_T("�ļ�%s�޷��򿪣�"), strTemp);
		AfxMessageBox(str);
		return;
	}

	int nNum = m_clModify.GetItemCount();
	int nIndex = -1;
	for (int i = 0; i < nNum; i++)
	{
		CString currentName,str;
		currentName = m_clModify.GetItemText(i, 1);
		if (currentName == p_tPrjDir[i]->sCurveName)
		{
			fprintf(ff, "%-10s%-20s%-10d%-10d%-10s%-10s\n", p_tPrjDir[i]->sCurveType, p_tPrjDir[i]->sCurveName, p_tPrjDir[i]->nCurveTime, p_tPrjDir[i]->nPauseTime, p_tPrjDir[i]->sCurveUndet2, p_tPrjDir[i]->sCurveUndet3);
		}
		//	setlocale(LC_ALL, "chs");//�ַ������ĵ�д��
		//	_ftprintf(ff, _T("%-10s%-20s%-10d%-10d%-10s%-10s\n"), (LPCTSTR)m_sType, (LPCTSTR)m_sName, m_nRunTm, m_nPauseTm, _T("b"), _T("c"));

	}
	fclose(ff);
	AfxMessageBox(_T("����ɹ���"));
	return;
}

//�޸�
void CModifyDlg::OnBnClickedMbtResive()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_nItem < 0)
	{
		AfxMessageBox(_T("��û��ѡ���κ����ݣ�"));
		return;         //��ֹ�������˳�
	}
	m_nFlag = 2;
	int nNum = m_clModify.GetItemCount();
	CAddCurveDlg addCurveDlg(nNum, this);
	if (addCurveDlg.DoModal() == IDOK)
	{
		CString itos;
		itos.Format(_T("%d"), m_nRunTm);
		m_clModify.SetItemText(m_nItem, 2, itos + _T("s"));
		itos.Format(_T("%d"), m_nPauseTm);
		m_clModify.SetItemText(m_nItem, 3, itos + _T("s"));

		p_tPrjDir[m_nItem]->nCurveTime = m_nRunTm;
		p_tPrjDir[m_nItem]->nPauseTime = m_nPauseTm;
	}
}

void CModifyDlg::OnBnClickedBtnDelete()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_nItem < 0)
	{
		AfxMessageBox(_T("��û��ѡ���κ����ݣ�"));
		return;         //��ֹ�������˳�
	}

	int nNum = m_clModify.GetItemCount();
	for (int i = 0; i < nNum; i++)
	{
		if (m_clModify.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			m_clModify.DeleteItem(i);
			nNum = m_clModify.GetItemCount();//ɾ��֮�������
			for (int j = 0; j < nNum; j++)
			{
				CString rowNum;
				rowNum.Format(_T("%d"), j + 1);
				m_clModify.SetItemText(j, 0, rowNum);
			}
			for (int k = i; k < nNum;k++)
			{
				strcpy(p_tPrjDir[k]->sCurveType, p_tPrjDir[k + 1]->sCurveType);
				strcpy(p_tPrjDir[k]->sCurveName, p_tPrjDir[k + 1]->sCurveName);
				p_tPrjDir[k]->nCurveTime = p_tPrjDir[k + 1]->nCurveTime;
				p_tPrjDir[k]->nPauseTime = p_tPrjDir[k + 1]->nCurveTime;
				strcpy(p_tPrjDir[k]->sCurveUndet2, p_tPrjDir[k + 1]->sCurveUndet2);
				strcpy(p_tPrjDir[k]->sCurveUndet3, p_tPrjDir[k + 1]->sCurveUndet3);
			}
		}
	}
}
