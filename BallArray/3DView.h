#pragma once


// C3DView 视图

class CBallCurve;
class C3DView : public CView
{
	DECLARE_DYNCREATE(C3DView)

protected:
	C3DView();           // 动态创建所使用的受保护的构造函数
	virtual ~C3DView();


#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	DECLARE_MESSAGE_MAP()

private:
	int		m_GLPixelIndex;
	HGLRC	m_hGLContext;
	bool	    m_bLButtonDown;
	CPoint	m_ptLButtonDownPos;
	CBallCurve* 	m_pBallCurve;
public:
	inline void SetBallCurve(CBallCurve*	pBallCurve)		{ m_pBallCurve = pBallCurve; }
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

private:
	BOOL SetWindowPixelFormat(HDC hDC);
	BOOL CreateViewGLContext(HDC hDC);
};


