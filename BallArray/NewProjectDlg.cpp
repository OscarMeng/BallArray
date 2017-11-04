// NewProjectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BallArray.h"
#include "NewProjectDlg.h"
#include "MyConstMeth.h"
#include "afxdialogex.h"


// CNewProjectDlg �Ի���

IMPLEMENT_DYNAMIC(CNewProjectDlg, CDialogEx)

CNewProjectDlg::CNewProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewProjectDlg::IDD, pParent)
{
	CMyConstMeth MyMeth;
	m_sPath = MyMeth.ReadModuleFileNm();
}

CNewProjectDlg::~CNewProjectDlg()
{
}

void CNewProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NEW_OPEN, m_btnNew);
	DDX_Control(pDX, IDC_NEW_PATH, m_eNewPath);
	DDX_Control(pDX, IDC_NEW_NAME, m_eNewName);
}


BEGIN_MESSAGE_MAP(CNewProjectDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNewProjectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_NEW_OPEN, &CNewProjectDlg::OnBnClickedNewOpen)
END_MESSAGE_MAP()


// CNewProjectDlg ��Ϣ�������


BOOL CNewProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString strPath;
	CString strTemp;
	strPath = m_sPath + _T(CONFIGURE_FILE) + _T("\\") + _T(CONFIGURE_NAME);

	GetPrivateProfileString(_T("OpenProject"), _T("Path"), _T(""), strTemp.GetBuffer(100), 100, strPath);
	strTemp.ReleaseBuffer();
	m_eNewPath.EnableWindow(false);// ֻ��ʾ
	m_eNewPath.SetWindowText(strTemp);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CNewProjectDlg::OnBnClickedNewOpen()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	BROWSEINFO   bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	TCHAR tchPath[255];

	bInfo.hwndOwner = m_hWnd;
	bInfo.lpszTitle = _T("��ѡ��·��:   ");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	LPITEMIDLIST lpDlist;//�������淵����Ϣ��IpDList��ʹ��SHGetPathFromIDList����ת��Ϊ�ַ��� 
	lpDlist = SHBrowseForFolder(&bInfo);   //��ʾѡ��Ի��� 
	if (lpDlist != NULL)
	{
		SHGetPathFromIDList(lpDlist, tchPath);//����Ŀ��ʶ�б�ת����Ŀ¼ 
		m_eNewPath.SetWindowText(tchPath);
	}
}


void CNewProjectDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString sName;
	CString sPath;
	CString strPath;
	CString sPrjPath;
	m_eNewPath.GetWindowText(strPath);
	m_eNewName.GetWindowText(sName);
	if (sName.IsEmpty())
	{
		AfxMessageBox(_T("�ļ�������Ϊ�գ�"));
		return;
	}
	sPrjPath = strPath + _T("\\") + sName + _T(".txt");
	if (PathFileExists(sPrjPath))//�ļ�����
	{
		if (MessageBox(_T("�ļ��Ѵ��ڣ�ȷ��������"), _T("�ļ�������ʾ"), MB_OKCANCEL) == IDCANCEL)
			return;
	}
	FILE *fp = NULL;
	_wfopen_s(&fp, sPrjPath, _T("w"));//�����ļ�
	fclose(fp);

	CString str = m_sPath + _T(CONFIGURE_FILE) + _T("\\") + _T(CONFIGURE_NAME);
	WritePrivateProfileString(_T("ProjectPath"), _T("Path"), sPrjPath, str);
	WritePrivateProfileString(_T("ProjectPath"), _T("Name"), sName, str);
	WritePrivateProfileString(_T("OpenProject"), _T("Path"), strPath, str);
	CDialogEx::OnOK();
}
