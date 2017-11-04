// Ball.cpp : 实现文件
//

#include "stdafx.h"
#include "BallCurve.h"
#include "Ball.h"
#include "MyConstMeth.h"
#include "math.h"
#include <gl/glut.h>

// CBall

CBall::CBall(int nID, int nMotor, int nCenX, int nCenY, CBallCurve* pBallCurve)
:m_nID(nID),
m_nMotor(nMotor),
m_nCenX(nCenX),
m_nCenY(nCenY),
m_pBallCurve(pBallCurve),
m_nRelativePosition(pBallCurve->GetAbsolutePosition(MODE_START))
{
	m_dRatio = 0.7;//调整2D小球直接的间隔
	m_nLastPosition = 0;
	m_nCurrentPosition = 0;
	m_nNextPosition = 0;
	//m_nPos = 0;
	m_dSpeed = 0;
	m_bSelected = false;
}

CBall::~CBall()
{
}

void CBall::Draw(CDC* pDC, CRect rRect)
{
	int left = m_nCenX*m_dRatio - m_pBallCurve->m_nRadius;
	int top = m_nCenY*m_dRatio - m_pBallCurve->m_nRadius;
	int right = m_nCenX*m_dRatio + m_pBallCurve->m_nRadius;
	int bottom = m_nCenY*m_dRatio + m_pBallCurve->m_nRadius;

	CPoint leftTop = m_pBallCurve->Lp2Dp(CPoint(left, top));
	CPoint rightBottom = m_pBallCurve->Lp2Dp(CPoint(right, bottom));
	if (leftTop.x > rRect.right)
		return;
	if (leftTop.y > rRect.bottom)
		return;
	if (rightBottom.x < rRect.left)
		return;
	if (rightBottom.y < rRect.top)
		return;
	CRect ballRect(leftTop.x, leftTop.y, rightBottom.x, rightBottom.y);

	CPen pen;
	if (m_bSelected)
		pen.CreatePen(PS_SOLID, 1, m_pBallCurve->GetColBoardSelected());
	else
		pen.CreatePen(PS_SOLID, 1, m_pBallCurve->GetColBoardNormal());

	CBrush brush(m_pBallCurve->CalGradientColor(m_nCurrentPosition, BALL_MAX_POS + m_nRelativePosition));
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	//画圆形
	pDC->Ellipse(&ballRect);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	brush.DeleteObject();
	pen.DeleteObject();

	if (m_pBallCurve->GetTextOn() && m_pBallCurve->GetZoom() >= 0.2)
	{
		COLORREF textColor = RGB(255, 255, 255) - m_pBallCurve->CalGradientColor(m_nCurrentPosition, 9999 + m_nRelativePosition);
		CFont* pOldFont = pDC->SelectObject(&m_pBallCurve->m_textFont);
		pDC->SetTextColor(textColor);
		pDC->SetBkMode(TRANSPARENT);

		CString text;
		text.Format(_T("(%03d)"), m_nID);
		CRect textRect(ballRect);
		textRect.top = ballRect.top + (ballRect.Height() - 20) / 2;
		textRect.bottom = textRect.top + 10;
		pDC->DrawText(text, &textRect, DT_WORDBREAK | DT_VCENTER | DT_CENTER | DT_NOPREFIX);

		/*text.Format("(%02d,%02d)", m_nCanetInd, m_nMotor);
		textRect.top = textRect.bottom;
		textRect.bottom = ballRect.bottom - (ballRect.Height() - 20) / 2;
		pDC->DrawText(text, &textRect, DT_WORDBREAK | DT_VCENTER | DT_CENTER | DT_NOPREFIX);*/

		pDC->SelectObject(pOldFont);
	}
}

void CBall::Draw3D()
{
	double h, s, v;
	double r, g, b;
	h = m_pBallCurve->GetColUpH() + m_nCurrentPosition / (BALL_MAX_POS + m_nRelativePosition)* (m_pBallCurve->GetColDownH() - m_pBallCurve->GetColUpH());
	s = m_pBallCurve->GetColUpS() + m_nCurrentPosition / (BALL_MAX_POS + m_nRelativePosition) * (m_pBallCurve->GetColDownS() - m_pBallCurve->GetColUpS());
	v = m_pBallCurve->GetColUpV() + m_nCurrentPosition / (BALL_MAX_POS + m_nRelativePosition) * (m_pBallCurve->GetColDownV() - m_pBallCurve->GetColUpV());

	int hi = int(h / 60.0) % 6;
	double f = h / 60.0 - hi;
	double p = v*(1 - s);
	double q = v*(1 - f*s);
	double t = v*(1 - (1 - f)*s);
	switch (hi)
	{
	case 0:
		r = v; g = t; b = p;
		break;
	case 1:
		r = q; g = v; b = p;
		break;
	case 2:
		r = p; g = v; b = t;
		break;
	case 3:
		r = p; g = q; b = v;
		break;
	case 4:
		r = t; g = p; b = v;
		break;
	case 5:
		r = v; g = p; b = q;
		break;;
	default:
		r = 1; g = 1; b = 1;
		break;
	}//3D小球随高度变色
	
	GLfloat no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat mat_diffusse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat low_shininess[] = { 15.0f };
	GLfloat high_shininess[] = { 100.0f };
	GLfloat mat_emission[] = { 0.0f+r, 0.0f+g, 0.0f+b, 1.0f };
	glEnable(GL_NORMALIZE);
	glPushMatrix();
	glTranslated(m_nCenX / 5000.0, m_nCenY / 5000.0, -1.0);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffusse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	GLUquadricObj* pObj;
	pObj = gluNewQuadric();
	glPushMatrix();
	double dFixedPos = 1000;//电机安装面到机械限位面的位置
	double dPosRatio = 6000;//Z轴位置比率
	glTranslated(0.0, 0.0, -dFixedPos / dPosRatio);//电机安装面到机械限位面的距离
	gluCylinder(pObj, 0.01, 0.01, dFixedPos / dPosRatio, 100, 50);//固定电机线长度
	glPopMatrix();
	gluCylinder(pObj, 0.01, 0.01, m_nCurrentPosition / dPosRatio, 100, 50);//初始点在机械限位面
	gluDeleteQuadric(pObj);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.0, -1.0);
	glTranslated(m_nCenX / 5000.0, m_nCenY / 5000.0, m_nCurrentPosition / dPosRatio);//初始点在机械限位面
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffusse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

	glutSolidSphere(0.03, 50, 50);//画小球
	glPopMatrix();
}

double CBall::CalTime(double speed)
{
	double dLastPos = m_nLastPosition / (BALL_MAX_POS + m_nRelativePosition)*(m_pBallCurve->GetPosDown3() - m_pBallCurve->GetPosLimit());
	double dNextPos = m_nNextPosition / (BALL_MAX_POS + m_nRelativePosition)*(m_pBallCurve->GetPosDown3() - m_pBallCurve->GetPosLimit());
	double dis = fabs(dNextPos - dLastPos);
	double circle = dis / (PI*PAN_D);
	double time = circle / speed*60.0;
	return time;
}

double CBall::CalSpeed(double time)
{
	double dLastPos = m_nLastPosition / (BALL_MAX_POS + m_nRelativePosition)*(m_pBallCurve->GetPosDown3() - m_pBallCurve->GetPosLimit());
	double dNextPos = m_nNextPosition / (BALL_MAX_POS + m_nRelativePosition)*(m_pBallCurve->GetPosDown3() - m_pBallCurve->GetPosLimit());
	double dis = fabs(dNextPos - dLastPos);
	double circle = dis / (PI*PAN_D);
	//m_nPos = int(dNextPos*LINENUM / (PI*PAN_D));
	m_dSpeed = circle / (time / 60.0);

	//if (m_dSpeed == 0 && circle != 0)
	//	m_dSpeed = 1;
	//if (m_dSpeed > MAX_SPEED)
	//{
	//	time = circle*1.5 / MAX_SPEED * 60;
	//	//m_dSpeed = circle / (time / 60.0);
	//}
	return time;
}

void CBall::SetCurrentPosition(DWORD nStartTime)
{
	double circle = m_dSpeed*((GetTickCount() - nStartTime) / 1000.0 / 60.0);//转了多少圈
	double dis = circle*PI*PAN_D;//转了多少圈的长度
	double pos = dis / (m_pBallCurve->GetPosDown3() - m_pBallCurve->GetPosStart()) * BALL_MAX_POS;//把长度转换为小球的位置0~9999,加上相对位置
	if (m_nLastPosition <= m_nNextPosition)//往下运行
	{
		m_nCurrentPosition = m_nLastPosition + pos;
		if (m_nCurrentPosition > m_nNextPosition)
			m_nCurrentPosition = m_nNextPosition;
	}
	else//往上运行
	{
		m_nCurrentPosition = m_nLastPosition - pos;
		if (m_nCurrentPosition < m_nNextPosition)
			m_nCurrentPosition = m_nNextPosition;
		if (m_nCurrentPosition < m_nRelativePosition)
		{
			m_nCurrentPosition = m_nRelativePosition;
		}
	}
	
}


// CBall 成员函数
