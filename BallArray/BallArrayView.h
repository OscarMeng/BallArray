#pragma once



// CBallArrayView ��ͼ
class CBallCurve;
class CCurveData;
class CControlView;
class CBallArrayView : public CScrollView
{
	DECLARE_DYNCREATE(CBallArrayView)

protected:
	CBallArrayView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CBallArrayView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnInitialUpdate();     // �����ĵ�һ��
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ

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


