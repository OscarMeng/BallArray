// NewProjectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BallArray.h"
#include "NewProjectDlg.h"
#include "MyConstMeth.h"
#include "afxdialogex.h"


// CNewProjectDlg 对话框

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


// CNewProjectDlg 消息处理程序


BOOL CNewProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString strPath;
	CString strTemp;
	strPath = m_sPath + _T(CONFIGURE_FILE) + _T("\\") + _T(CONFIGURE_NAME);

	GetPrivateProfileString(_T("OpenProject"), _T("Path"), _T(""), strTemp.GetBuffer(100), 100, strPath);
	strTemp.ReleaseBuffer();
	m_eNewPath.EnableWindow(false);// 只显示
	m_eNewPath.SetWindowText(strTemp);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CNewProjectDlg::OnBnClickedNewOpen()
{
	// TODO:  在此添加控件通知处理程序代码
	BROWSEINFO   bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	TCHAR tchPath[255];

	bInfo.hwndOwner = m_hWnd;
	bInfo.lpszTitle = _T("所选择路径:   ");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	LPITEMIDLIST lpDlist;//用来保存返回信息的IpDList，使用SHGetPathFromIDList函数转换为字符串 
	lpDlist = SHBrowseForFolder(&bInfo);   //显示选择对话框 
	if (lpDlist != NULL)
	{
		SHGetPathFromIDList(lpDlist, tchPath);//把项目标识列表转化成目录 
		m_eNewPath.SetWindowText(tchPath);
	}
}


void CNewProjectDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CString sName;
	CString sPath;
	CString strPath;
	CString sPrjPath;
	m_eNewPath.GetWindowText(strPath);
	m_eNewName.GetWindowText(sName);
	if (sName.IsEmpty())
	{
		AfxMessageBox(_T("文件名不能为空！"));
		return;
	}
	sPrjPath = strPath + _T("\\") + sName + _T(".txt");
	if (PathFileExists(sPrjPath))//文件存在
	{
		if (MessageBox(_T("文件已存在，确定覆盖吗？"), _T("文件创建提示"), MB_OKCANCEL) == IDCANCEL)
			return;
	}
	FILE *fp = NULL;
	_wfopen_s(&fp, sPrjPath, _T("w"));//创建文件
	fclose(fp);

	CString str = m_sPath + _T(CONFIGURE_FILE) + _T("\\") + _T(CONFIGURE_NAME);
	WritePrivateProfileString(_T("ProjectPath"), _T("Path"), sPrjPath, str);
	WritePrivateProfileString(_T("ProjectPath"), _T("Name"), sName, str);
	WritePrivateProfileString(_T("OpenProject"), _T("Path"), strPath, str);
	CDialogEx::OnOK();
}
