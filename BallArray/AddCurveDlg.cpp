// AddCurverDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BallArray.h"
#include "AddCurveDlg.h"
#include "MyConstMeth.h"
#include "afxdialogex.h"


// CAddCurverDlg �Ի���

IMPLEMENT_DYNAMIC(CAddCurveDlg, CDialogEx)

CAddCurveDlg::CAddCurveDlg(int nNum,CWnd* pParent /*= NULL*/)
: m_nNum(nNum),m_pModDlg((CModifyDlg*)pParent),CDialogEx(CAddCurveDlg::IDD, pParent)
{
	CMyConstMeth MyMeth;
	m_sPath = MyMeth.ReadModuleFileNm();
}

CAddCurveDlg::~CAddCurveDlg()
{
}

void CAddCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PATH, m_ePath);
	DDX_Control(pDX, IDC_EDIT_TYPE, m_eType);
	DDX_Control(pDX, IDC_EDIT_NAME, m_eName);
	DDX_Control(pDX, IDC_EDIT_RUNTM, m_eRunTime);
	DDX_Control(pDX, IDC_EDIT_PAUSETM, m_ePauseTime);
	DDX_Control(pDX, IDC_EDIT_ROWNB, m_eRowNum);
	DDX_Control(pDX, IDC_EDIT_TIP, m_eRowTip);
}


BEGIN_MESSAGE_MAP(CAddCurveDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CAddCurveDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDOK, &CAddCurveDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddCurverDlg ��Ϣ�������


BOOL CAddCurveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//GetDlgItem(IDC_EDIT_PATH)->EnableWindow(false);
	m_nMaxRowNum =  m_nNum + 1;
	CString ntos,str;
	//���
	if (m_pModDlg->m_nFlag == 1)
	{
		CString strPath;
		CString strTemp;
		strPath = m_sPath + _T(CONFIGURE_FILE) + _T("\\") + _T(CONFIGURE_NAME);

		GetPrivateProfileString(_T("AddPath"), _T("Path"), _T(""), strTemp.GetBuffer(100), 100, strPath);
		strTemp.ReleaseBuffer();
		m_ePath.EnableWindow(false);// ֻ��ʾ
		m_ePath.SetWindowText(strTemp);

		GetPrivateProfileString(_T("AddPath"), _T("Type"), _T(""), strTemp.GetBuffer(100), 100, strPath);
		strTemp.ReleaseBuffer();
		m_eType.EnableWindow(false);
		m_eType.SetWindowText(strTemp);

		GetPrivateProfileString(_T("AddPath"), _T("Name"), _T(""), strTemp.GetBuffer(100), 100, strPath);
		strTemp.ReleaseBuffer();
		m_eName.EnableWindow(false);
		m_eName.SetWindowText(strTemp);

		m_eRowTip.EnableWindow(false);
		ntos.Format(_T("��ѡ1~%d��"), m_nMaxRowNum);
		m_eRowTip.SetWindowText(ntos);
	}
	
	//�޸�
	if (m_pModDlg->m_nFlag==2)
	{
		CString strType, strName;
		CButton *pButton = (CButton*)GetDlgItem(IDC_BUTTON_ADD);//��Ӱ�ťȡ������
		pButton->EnableWindow(FALSE);
		m_ePath.EnableWindow(false);
		m_eType.EnableWindow(false);
		m_eName.EnableWindow(false);

		strType=m_pModDlg->p_tPrjDir[m_pModDlg->m_nItem]->sCurveType;
		strName = m_pModDlg->p_tPrjDir[m_pModDlg->m_nItem]->sCurveName;
		CString strPath;
		strPath = m_sPath + _T(CURVE_DATA) + _T("\\") + strType + _T("\\") + strName + _T(".txt");

		m_ePath.SetWindowText(strPath);
		m_eType.SetWindowText(strType);
		m_eName.SetWindowText(strName);

		str = m_pModDlg->m_clModify.GetItemText(m_pModDlg->m_nItem, 2);
		str = str.Mid(0, str.GetLength() - 1);
		m_eRunTime.SetWindowText(str);
		
		str = m_pModDlg->m_clModify.GetItemText(m_pModDlg->m_nItem, 3);
		str = str.Mid(0, str.GetLength() - 1);
		m_ePauseTime.SetWindowText(str);

		m_eRowNum.EnableWindow(false);
		ntos.Format(_T("%d"), m_pModDlg->m_nItem+1);
		m_eRowNum.SetWindowText(ntos);
		m_eRowTip.EnableWindow(false);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CAddCurveDlg::OnBnClickedButtonAdd()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("TXT Files(*.txt)|*.txt||"));
	CString strPath;
	strPath = m_sPath + _T(CURVE_DATA) + _T("\\");
	fileDialog.m_ofn.lpstrInitialDir = strPath;
	if (fileDialog.DoModal() == IDOK)
	{
		CString path = fileDialog.GetPathName();
		m_sContent = fileDialog.GetFileTitle();
		m_ePath.SetWindowText(path);

		if (path.Find(_T(CURVE_DYNAMIC))!=-1)
		{
			m_eType.SetWindowText(_T(CURVE_DYNAMIC));
			m_pModDlg->m_sType = _T(CURVE_DYNAMIC);
		}
		if (path.Find(_T(CURVE_STATIC)) != -1)
		{
			m_eType.SetWindowText(_T(CURVE_STATIC));
			m_pModDlg->m_sType = _T(CURVE_STATIC);
		}

		m_eName.SetWindowText(m_sContent);
		m_pModDlg->m_sName = m_sContent;
	}
}


//���ȷ��ʱ��������ʾ�����������·����Ϣ.ini
void CAddCurveDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strPath,strType,strName,strRun,strPause,strRow,str;

	if (m_pModDlg->m_nFlag == 1)
	{
		m_ePath.GetWindowText(strPath);
		if (strPath.IsEmpty())
		{
			MessageBox(_T("\"�ļ�·��\"����Ϊ��!"), _T("���/�޸��ļ���ʾ"), MB_ICONWARNING);
			return;
		}

		m_eRunTime.GetWindowText(strRun);
		if (strRun.IsEmpty())
		{
			MessageBox(_T("\"����ʱ��\"����Ϊ��!"), _T("���/�޸��ļ���ʾ"), MB_ICONWARNING);
			return;
		}
		m_pModDlg->m_nRunTm = _ttoi(strRun);

		m_ePauseTime.GetWindowText(strPause);
		if (strPause.IsEmpty())
		{
			MessageBox(_T("\"��ͣʱ��\"����Ϊ��!"), _T("���/�޸��ļ���ʾ"), MB_ICONWARNING);
			return;
		}
		m_pModDlg->m_nPauseTm = _ttoi(strPause);

		m_eRowNum.GetWindowText(strRow);
		if (strRow.IsEmpty())
		{
			MessageBox(_T("\"��������\"����Ϊ��!"), _T("���/�޸��ļ���ʾ"), MB_ICONWARNING);
			return;
		}
		if ( _ttoi(strRow) > m_nMaxRowNum)
		{
			MessageBox(_T("\"��������\"���ڿ�ѡ��Χ!"), _T("���/�޸��ļ���ʾ"), MB_ICONWARNING);
			return;
		}
		m_pModDlg->m_nRowNm = _ttoi(strRow);

	}
	if (m_pModDlg->m_nFlag == 2)
	{
		m_eRunTime.GetWindowText(strRun);
		if (strRun.IsEmpty())
		{
			MessageBox(_T("\"����ʱ��\"����Ϊ��!"), _T("���/�޸��ļ���ʾ"), MB_ICONWARNING);
			return;
		}
		m_pModDlg->m_nRunTm = _ttoi(strRun);

		m_ePauseTime.GetWindowText(strPause);
		if (strPause.IsEmpty())
		{
			MessageBox(_T("\"��ͣʱ��\"����Ϊ��!"), _T("���/�޸��ļ���ʾ"), MB_ICONWARNING);
			return;
		}
		m_pModDlg->m_nPauseTm = _ttoi(strPause);
	}

	m_ePath.GetWindowText(strPath);
	m_eType.GetWindowText(strType);
	m_eName.GetWindowText(strName);
	m_pModDlg->m_sType = strType;
	m_pModDlg->m_sName = strName;

	str= m_sPath + _T(CONFIGURE_FILE) + _T("\\") + _T(CONFIGURE_NAME);

	WritePrivateProfileString(_T("AddPath"), _T("Path"), strPath, str);
	WritePrivateProfileString(_T("AddPath"), _T("Type"), strType, str);
	WritePrivateProfileString(_T("AddPath"), _T("Name"), strName, str);

	CDialogEx::OnOK();
}
