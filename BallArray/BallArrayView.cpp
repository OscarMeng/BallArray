// BallArrayView.cpp : ʵ���ļ�
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


// CBallArrayView ��ͼ

void CBallArrayView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO:  �������ͼ�ĺϼƴ�С
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


BOOL CBallArrayView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ����ר�ô����/����û���
	CSize size;
	size.cx = m_pBallCurve->GetWidth();
	size.cy = m_pBallCurve->GetHeight();
	SetScrollSizes(MM_TEXT, size);

	return CScrollView::PreCreateWindow(cs);
}


void CBallArrayView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO:  �ڴ����ר�ô����/����û���

	CScrollView::OnPrepareDC(pDC, pInfo);
}


void CBallArrayView::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	// TODO:  �ڴ���ӻ��ƴ���
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

// CBallArrayView ���

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


// CBallArrayView ��Ϣ�������



