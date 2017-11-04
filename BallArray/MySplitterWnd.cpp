#include "stdafx.h"
#include "MySplitterWnd.h"

IMPLEMENT_DYNCREATE(CMySplitterWnd, CSplitterWnd)

CMySplitterWnd::CMySplitterWnd()
{
}

CMySplitterWnd::~CMySplitterWnd(void)
{
}

BEGIN_MESSAGE_MAP(CMySplitterWnd, CSplitterWnd)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void CMySplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CRect rect;                                 //创建一个矩形区域
	GetClientRect(&rect);                //指定窗口的用户区域矩形，得到rect（即以所在窗口左上角为原点，仅仅是个大小，返回值的左上角x，y为0，0）
	if (m_nHorVer == 1)
	{
		if (point.x < 500 || point.x >(rect.Width() - 800))
			CWnd::OnMouseMove(nFlags, point);
		else
			CSplitterWnd::OnMouseMove(nFlags, point);
	}
	else
	{
		if (point.y < 550 || point.y >(rect.Height() - 200))
			CWnd::OnMouseMove(nFlags, point);
		else
			CSplitterWnd::OnMouseMove(nFlags, point);
	}
	CWnd::OnMouseMove(nFlags, point);
}
