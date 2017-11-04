#pragma once
#include "afxwin.h"


// CNewProjectDlg 对话框

class CNewProjectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewProjectDlg)

public:
	CNewProjectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewProjectDlg();

// 对话框数据
	enum { IDD = IDD_NEW_PROJECT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString  m_sPath;
	CButton m_btnNew;
	CEdit m_eNewPath;
	CEdit m_eNewName;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedNewOpen();
};
