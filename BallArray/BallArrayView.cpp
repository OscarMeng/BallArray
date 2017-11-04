// BallArrayView.cpp : 实现文件
//

#include "stdafx.h"
#include "BallArray.h"
#include "BallArrayView.h"
#include "BallCurve.h"
#include "CurveData.h"


// CBallArrayView

IMPLEMENT_DYNCREATE(CBallArrayView, CScrollView)

CBallArrayView::CBallArrayView()
{
	m_bDragMode = false;

	m_pBallCurve = new CBallCurve();
	m_pCurveData = new CCurveData();
}

CBallArrayView::~CBallArrayView()
{
	delete m_pBallCurve;
	delete m_pCurveData;
}


BEGIN_MESSAGE_MAP(CBallArrayView, CScrollView)
END_MESSAGE_MAP()


// CBallArrayView 绘图

void CBallArrayView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO:  计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


BOOL CBallArrayView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此添加专用代码和/或调用基类
	CSize size;
	size.cx = m_pBallCurve->GetWidth();
	size.cy = m_pBallCurve->GetHeight();
	SetScrollSizes(MM_TEXT, size);

	return CScrollView::PreCreateWindow(cs);
}


void CBallArrayView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO:  在此添加专用代码和/或调用基类

	CScrollView::OnPrepareDC(pDC, pInfo);
}


void CBallArrayView::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码
	CRect rcClient;
	GetClientRect(rcClient);

	CDC	memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bBmp;
	bBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	CBitmap *pOldBitmap = memDC.SelectObject(&bBmp);

	m_pBallCurve->Draw(&memDC, &rcClient);
	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	bBmp.DeleteObject();
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
}


BOOL CBallArrayView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	return CScrollView::OnEraseBkgnd(pDC);
}

// CBallArrayView 诊断

#ifdef _DEBUG
void CBallArrayView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CBallArrayView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBallArrayView 消息处理程序



