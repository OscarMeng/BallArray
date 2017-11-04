// TCPServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BallArray.h"
#include "TCPServerDlg.h"
#include "ControlView.h"
#include "afxdialogex.h"


// CTCPServerDlg �Ի���

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


// CTCPServerDlg ��Ϣ�������


BOOL CTCPServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	addr.sin_family = AF_INET;//ָ����ַ����ΪTCP/IP
	addr.sin_port = htons(m_pCView->m_nServerPort);//ָ���˿�
	addr.sin_addr.S_un.S_addr = INADDR_ANY;//inet_addr("127.0.0.1");
	s = ::socket(AF_INET, SOCK_STREAM, 0);
	::bind(s, (sockaddr*)&addr, sizeof(addr));
	::listen(s, 5);
	::WSAAsyncSelect(s, this->m_hWnd, WM_SOCKET, FD_ACCEPT | FD_READ);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
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
			strConnect.Format(_T("��%d���ͻ��Ѿ�������:"), n);
			strConnect += ::inet_ntoa(add1.sin_addr);
			strConnect += _T("\r\n");
			strConnect += _T("��½\r\n");
			m_sShowWnd += strConnect;
			UpdateData(FALSE);
			m_cShowWnd.LineScroll(m_cShowWnd.GetLineCount());//�ÿؼ���������Ϣ������������
		}
			break;

		case FD_READ:
		{
			::recv(s1, cs, 100, 0);
			UpdateData(TRUE);
			m_sShowWnd += _T("\r\n");
			m_sShowWnd += ::inet_ntoa(add1.sin_addr);
			m_sShowWnd += _T("����˵��");
			m_sShowWnd += cs;
			m_sShowWnd += _T("\r\n");
			
			UpdateData(FALSE);
			m_cShowWnd.LineScroll(m_cShowWnd.GetLineCount());//�ÿؼ���������Ϣ������������
	    }
			break;
	}
	return 0;
}


void CTCPServerDlg::OnSend()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	char Buffer[800];
	if (m_sSendWnd.IsEmpty())
	{
		AfxMessageBox(_T("��Ϣ����Ϊ�գ�"));
	}
	else
	{
		WideCharToMultiByte(CP_ACP, 0, m_sSendWnd, -1, Buffer, 800, NULL, NULL);//MultiByteToWideChar(CP_ACP, 0, cs, -1, m_sShowWnd, 100);
		if (::send(s1, Buffer, sizeof(Buffer), 0) != SOCKET_ERROR)
		{
			UpdateData(TRUE);
			m_sShowWnd += _T("\r\n");
			m_sShowWnd += _T("����");
			m_sShowWnd += ::inet_ntoa(add1.sin_addr);
			m_sShowWnd += _T("˵��");
			m_sShowWnd += m_sSendWnd;
			m_sShowWnd += _T("\r\n");
			UpdateData(FALSE);
			m_cShowWnd.LineScroll(m_cShowWnd.GetLineCount());//�ÿؼ���������Ϣ������������
		}
		else
		{
			UpdateData(TRUE);
			m_sShowWnd += _T("\r\n");
			m_sShowWnd += _T("��Ϣ����ʧ�ܣ�\r\n");
			UpdateData(FALSE);
			m_cShowWnd.LineScroll(m_cShowWnd.GetLineCount());//�ÿؼ���������Ϣ������������
		}
	}
}


void CTCPServerDlg::OnClear()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_sSendWnd = _T("");
	UpdateData(FALSE);
	m_cShowWnd.LineScroll(m_cShowWnd.GetLineCount());//�ÿؼ���������Ϣ������������
}
