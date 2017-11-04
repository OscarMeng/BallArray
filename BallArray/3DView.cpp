// 3DView.cpp : 实现文件
//

#include "stdafx.h"
#include "BallArray.h"
#include "3DView.h"
#include "BallCurve.h"
#include <gl/glut.h>

// C3DView

IMPLEMENT_DYNCREATE(C3DView, CView)

C3DView::C3DView()
{
	m_hGLContext = NULL;
	m_GLPixelIndex = 0;
	m_bLButtonDown = false;
}

C3DView::~C3DView()
{
}

BEGIN_MESSAGE_MAP(C3DView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// C3DView 绘图

void C3DView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码
}


// C3DView 诊断

#ifdef _DEBUG
void C3DView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void C3DView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// C3DView 消息处理程序

// 在程序显示窗口之前改变窗口的样式
BOOL C3DView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此添加专用代码和/或调用基类

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}


int C3DView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	HDC hDC = ::GetDC(GetSafeHwnd());
	if (!SetWindowPixelFormat(hDC))
		return 0;
	if (!CreateViewGLContext(hDC))
		return 0;

	return 0;
}


void C3DView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	GLsizei width = cx;
	GLsizei height = cy;
	GLdouble aspect;
	if (cy == 0)
		aspect = (GLdouble)width;
	else
		aspect = (GLdouble)width / (GLdouble)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, aspect, 1, 10);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDrawBuffer(GL_BACK);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}


void C3DView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CView::OnPaint()

	m_pBallCurve->Draw3D();
	SwapBuffers(dc.m_ps.hdc);
}


void C3DView::OnDestroy()
{
	if (wglGetCurrentContext() != NULL)
	{
		// make the rendering context not current
		wglMakeCurrent(NULL, NULL);
	}
	if (m_hGLContext != NULL)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}

	CView::OnDestroy();
}


void C3DView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_bLButtonDown = true;
	m_ptLButtonDownPos = point;

	CView::OnLButtonDown(nFlags, point);
}


void C3DView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_bLButtonDown = false;

	CView::OnLButtonUp(nFlags, point);
}


void C3DView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_bLButtonDown)
	{
		double xRotation = (m_ptLButtonDownPos.x - point.x) / 10.0;
		m_pBallCurve->SetXRotation(m_pBallCurve->GetXRotation() + xRotation);
		InvalidateRect(NULL, FALSE);

		m_ptLButtonDownPos = point;
	}

	CView::OnMouseMove(nFlags, point);
}


BOOL C3DView::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;
	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_STEREO_DONTCARE;
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;
	m_GLPixelIndex = ChoosePixelFormat(hDC, &pixelDesc);
	if (m_GLPixelIndex == 0) // Let's choose a default index.
	{
		m_GLPixelIndex = 1;
		if (!DescribePixelFormat(hDC, m_GLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc))
		{
			return FALSE;
		}
	}
	if (!SetPixelFormat(hDC, m_GLPixelIndex, &pixelDesc))
	{
		return FALSE;
	}

	return TRUE;
}


BOOL C3DView::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);
	if (m_hGLContext == NULL)
	{
		return FALSE;
	}
	if (!wglMakeCurrent(hDC, m_hGLContext))
	{
		return FALSE;
	}

	return TRUE;
}
