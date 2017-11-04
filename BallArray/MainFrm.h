
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "ChildView.h"
#include "MySplitterWnd.h"

class C3DView;
class CBallArrayView;
class CControlView;

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext = NULL);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CToolBar           m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	CChildView       m_wndView;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

private:
	// �ָ��1
	CMySplitterWnd      m_wndSplitter1;
	// �ָ��2
	CMySplitterWnd      m_wndSplitter2;
	C3DView*			    m_p3DView;
	CBallArrayView*	    m_pBallArrayView;
	CControlView*		m_pControlView;
	
private:
	bool 	m_bInitSplitter;
	int		m_nWidth;
	int		m_nHeight;

};


