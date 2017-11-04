#pragma once
#include "afxwin.h"


// CTCPServerDlg 对话框
class CControlView;
class CTCPServerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTCPServerDlg)

public:
	CTCPServerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTCPServerDlg();

// 对话框数据
	enum { IDD = IDD_TCP_SERVER_DLG };

public:
	CControlView* m_pCView;
	SOCKET s, s1,sock[5];
	sockaddr_in addr, add1;//套接字地址结构
	int n;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_sShowWnd;
	virtual BOOL OnInitDialog();
protected:
	afx_msg LRESULT OnSocket(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSend();
	CString m_sSendWnd;
	CEdit m_cShowWnd;
	afx_msg void OnClear();
};
