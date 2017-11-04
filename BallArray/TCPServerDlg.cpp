// TCPServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BallArray.h"
#include "TCPServerDlg.h"
#include "ControlView.h"
#include "afxdialogex.h"


// CTCPServerDlg 对话框

IMPLEMENT_DYNAMIC(CTCPServerDlg, CDialogEx)

CTCPServerDlg::CTCPServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTCPServerDlg::IDD, pParent)
	, n(0)
	, m_sShowWnd(_T(""))
	, m_sSendWnd(_T(""))
	, m_pCView((CControlView*)pParent)
{	

}

CTCPServerDlg::~CTCPServerDlg()
{
}

void CTCPServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SHOW_WND, m_sShowWnd);
	DDX_Text(pDX, IDC_SEND_WND, m_sSendWnd);
	DDX_Control(pDX, IDC_SHOW_WND, m_cShowWnd);
}


BEGIN_MESSAGE_MAP(CTCPServerDlg, CDialog)
	ON_MESSAGE(WM_SOCKET, &CTCPServerDlg::OnSocket)
	ON_BN_CLICKED(IDC_SERVER_SEND, &CTCPServerDlg::OnSend)
	ON_BN_CLICKED(IDC_SERVER_CLEAR, &CTCPServerDlg::OnClear)
END_MESSAGE_MAP()


// CTCPServerDlg 消息处理程序


BOOL CTCPServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	addr.sin_family = AF_INET;//指定地址家族为TCP/IP
	addr.sin_port = htons(m_pCView->m_nServerPort);//指定端口
	addr.sin_addr.S_un.S_addr = INADDR_ANY;//inet_addr("127.0.0.1");
	s = ::socket(AF_INET, SOCK_STREAM, 0);
	::bind(s, (sockaddr*)&addr, sizeof(addr));
	::listen(s, 5);
	::WSAAsyncSelect(s, this->m_hWnd, WM_SOCKET, FD_ACCEPT | FD_READ);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


afx_msg LRESULT CTCPServerDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	CString strConnect;
	char cs[800] = { 0 };
	switch (lParam)
	{
		case FD_ACCEPT:
		{
			int lenth = sizeof(add1);
			s1 = ::accept(s, (sockaddr*)&add1, &lenth);
			UpdateData(TRUE);
			m_sShowWnd += _T("\r\n");
			n = n + 1;
			strConnect.Format(_T("有%d个客户已经连接上:"), n);
			strConnect += ::inet_ntoa(add1.sin_addr);
			strConnect += _T("\r\n");
			strConnect += _T("登陆\r\n");
			m_sShowWnd += strConnect;
			UpdateData(FALSE);
			m_cShowWnd.LineScroll(m_cShowWnd.GetLineCount());//用控件变量让信息滚动条在最下
		}
			break;

		case FD_READ:
		{
			::recv(s1, cs, 100, 0);
			UpdateData(TRUE);
			m_sShowWnd += _T("\r\n");
			m_sShowWnd += ::inet_ntoa(add1.sin_addr);
			m_sShowWnd += _T("对您说：");
			m_sShowWnd += cs;
			m_sShowWnd += _T("\r\n");
			
			UpdateData(FALSE);
			m_cShowWnd.LineScroll(m_cShowWnd.GetLineCount());//用控件变量让信息滚动条在最下
	    }
			break;
	}
	return 0;
}


void CTCPServerDlg::OnSend()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	char Buffer[800];
	if (m_sSendWnd.IsEmpty())
	{
		AfxMessageBox(_T("消息不能为空！"));
	}
	else
	{
		WideCharToMultiByte(CP_ACP, 0, m_sSendWnd, -1, Buffer, 800, NULL, NULL);//MultiByteToWideChar(CP_ACP, 0, cs, -1, m_sShowWnd, 100);
		if (::send(s1, Buffer, sizeof(Buffer), 0) != SOCKET_ERROR)
		{
			UpdateData(TRUE);
			m_sShowWnd += _T("\r\n");
			m_sShowWnd += _T("您对");
			m_sShowWnd += ::inet_ntoa(add1.sin_addr);
			m_sShowWnd += _T("说：");
			m_sShowWnd += m_sSendWnd;
			m_sShowWnd += _T("\r\n");
			UpdateData(FALSE);
			m_cShowWnd.LineScroll(m_cShowWnd.GetLineCount());//用控件变量让信息滚动条在最下
		}
		else
		{
			UpdateData(TRUE);
			m_sShowWnd += _T("\r\n");
			m_sShowWnd += _T("消息发送失败！\r\n");
			UpdateData(FALSE);
			m_cShowWnd.LineScroll(m_cShowWnd.GetLineCount());//用控件变量让信息滚动条在最下
		}
	}
}


void CTCPServerDlg::OnClear()
{
	// TODO:  在此添加控件通知处理程序代码
	m_sSendWnd = _T("");
	UpdateData(FALSE);
	m_cShowWnd.LineScroll(m_cShowWnd.GetLineCount());//用控件变量让信息滚动条在最下
}
