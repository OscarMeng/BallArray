#pragma once



// CBallArrayView 视图
class CBallCurve;
class CCurveData;
class CControlView;
class CBallArrayView : public CScrollView
{
	DECLARE_DYNCREATE(CBallArrayView)

protected:
	CBallArrayView();           // 动态创建所使用的受保护的构造函数
	virtual ~CBallArrayView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnInitialUpdate();     // 构造后的第一次
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图

private:
	CBallCurve*	        m_pBallCurve;
	CCurveData*		m_pCurveData;
	CControlView*	m_pControlView;

	bool			    m_bDragMode;
	HCURSOR		m_Cursor;
	CPoint			m_ptLast;

public:
	inline CBallCurve*	 GetBallCurve()		    { return m_pBallCurve; }
	inline CCurveData*	 GetCurveData()			{ return m_pCurveData; }
	inline void				 SetControlView(CControlView* pControlView)		{ m_pControlView = pControlView; }

public:
	void	ZoomInOut(double dZoom, CPoint ptCenter);

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

};


