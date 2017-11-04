
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "BallArray.h"

#include "MainFrm.h"
#include "3DView.h"
#include "BallArrayView.h"
#include "ControlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO:  在此添加成员初始化代码
	m_wndSplitter1.SetHorVer(1);
	m_wndSplitter2.SetHorVer(2);

	m_bInitSplitter = false;
	m_nWidth = 0;
	m_nHeight = 0;
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, LPCTSTR lpszMenuName, DWORD dwExStyle, CCreateContext* pContext)
{
	// TODO:  在此添加专用代码和/或调用基类
	HMENU nIDorHMenu = NULL;
	return CFrameWnd::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle,
		rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
		pParentWnd->GetSafeHwnd(),
		nIDorHMenu,
		(LPVOID)pContext);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	cs.lpszName = _T("小球阵列");
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// TODO:  在此添加专用代码和/或调用基类

	return CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

//分屏
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO:  在此添加专用代码和/或调用基类
	m_wndSplitter1.CreateStatic(this, 1, 2);
	m_wndSplitter1.CreateView(0, 0, RUNTIME_CLASS(C3DView), CSize(0, 0), pContext);
	m_wndSplitter2.CreateStatic(&m_wndSplitter1, 2, 1, WS_CHILD | WS_VISIBLE, m_wndSplitter1.IdFromRowCol(0, 1));
	m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(CBallArrayView), CSize(0, 0), pContext);
	m_wndSplitter2.CreateView(1, 0, RUNTIME_CLASS(CControlView), CSize(0, 0), pContext);

	m_p3DView = (C3DView*)m_wndSplitter1.GetPane(0, 0);
	m_pBallArrayView = (CBallArrayView*)m_wndSplitter2.GetPane(0, 0);
	m_pControlView = (CControlView*)m_wndSplitter2.GetPane(1, 0);
	m_p3DView->SetBallCurve(m_pBallArrayView->GetBallCurve());
	m_pBallArrayView->SetControlView(m_pControlView);
	m_pControlView->SetMainFrame(this);
	m_pControlView->Set3DView(m_p3DView);
	m_pControlView->SetBallArrayView(m_pBallArrayView);
	m_pControlView->SetBallCurve(m_pBallArrayView->GetBallCurve());
	m_pControlView->SetCurveData(m_pBallArrayView->GetCurveData());

	m_bInitSplitter = true;
	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

//显示最小最大尺寸
void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x = 1300;
	lpMMI->ptMinTrackSize.y = 750;

	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

//设置分屏大小
void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_bInitSplitter)
	{
		CRect rect;
		GetClientRect(&rect);
		if (rect.Width() <= 0 || rect.Height() <= 0)
			return;
		if (cx == m_nWidth)
			return;
		m_nWidth = cx;
		int x = max(500, (int)(rect.Width()*0.4));
		m_wndSplitter1.SetColumnInfo(0, x, 0);
		m_wndSplitter1.SetColumnInfo(1, rect.Width() - x, 0);
		m_wndSplitter1.RecalcLayout();

		if (cy == m_nHeight)
			return;
		m_nHeight = cy;
		int y = 300;
		m_wndSplitter2.SetRowInfo(0, rect.Height() - y, 0);
		m_wndSplitter2.SetRowInfo(1, y, 0);
		m_wndSplitter2.RecalcLayout();
	}
}


BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CFrameWnd::OnEraseBkgnd(pDC);
}
