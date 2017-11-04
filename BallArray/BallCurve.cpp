// BallCurve.cpp : 实现文件
//

#include "stdafx.h"
#include "BallArray.h"
#include "BallCurve.h"
#include "Ball.h"
#include "MyConstMeth.h"
#include <gl/glut.h>


// CBallCurve

CBallCurve::CBallCurve()
:m_dMaxZoom(1),
m_dMinZoom(0.06)
{
	CMyConstMeth MyMeth;
	m_sPath = MyMeth.ReadModuleFileNm();

	ReadPosParam();
	m_nRelStartPos = (m_nPosStart - m_nPosLimit) * 9999.0 / (m_nPosDown3 - m_nPosStart);

	int nCoord[m_nBallNum][2];
	int nMotor[m_nBallNum];
	ReadCoordinate(_T(BALL_COORDINATE), nCoord);
	for (int i = 0; i < m_nBallNum; i++)
		m_pBalls[i] = new CBall(i + 1, nMotor[i], nCoord[i][0], nCoord[i][1], this);

	m_ptStartPoint = CPoint(0, 0);
	m_dZoom = 0.06;
	m_bTextOn = true;

	m_dXRotation = 0;

	m_colBackGround = RGB(5, 9, 50);              //显示界面背景颜色
	m_colCoordinate = RGB(255, 255, 0);        //坐标轴颜色
	m_colBoardNormal = RGB(40, 245, 12);		//普通状态下边框的颜色
	m_colBoardSelected = RGB(244, 39, 11);	//选中时边框颜色
	m_colUpH = 0;				//最高点HSL颜色H值
	m_colUpS = 1;				//最高点HSL颜色S值
	m_colUpV = 1;				//最高点HSL颜色L值
	m_colDownH = 245;	//最低点HSL颜色H值
	m_colDownS = 1;			//最低点HSL颜色S值
	m_colDownV = 1;		//最低点HSL颜色L值

	m_bAutoRotation = false;

	m_textFont.CreateFont(12, 0, 0, 0, 600, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("宋体"));

}

CBallCurve::~CBallCurve()
{
	for (int i = 0; i < m_nBallNum; i++)
		delete m_pBalls[i];
}

void CBallCurve::SetCurrentBall(int n,int nheight)
{
	m_pBalls[n]->SetCurrtPosition(nheight);
}

void CBallCurve::ReadCoordinate(CString filename, int coord[][2])
{
	CString strPath;
	strPath = m_sPath + _T(CONFIGURE_FILE) + _T("\\") +filename;

	FILE* ff;
	CFileStatus status;
	errno_t err;
	if (CFile::GetStatus(strPath, status))//如果指定文件的状态信息成功获取，则为TRUE，否则为FALSE。
	{
		err = _wfopen_s(&ff, strPath, _T("rt"));
		CString str;
		if (err != 0)
		{
			str.Format(_T("文件%s无法打开！"), strPath);
			AfxMessageBox(str);
			return;
		}
		if (filename == _T(BALL_COORDINATE))
		{
			for (int i = 0; i < m_nBallNum; i++)
			{
				fscanf_s(ff, "%d,%d", &coord[i][0], &coord[i][1]);
			}
		}
		if (filename == _T(ROWCOL_COORDINATE))
		{
			for (int i = 0; i < 116; i++)
			{
				fscanf_s(ff, "%d,%d", &coord[i][0], &coord[i][1]);
			}
		}
		fclose(ff);
	}
}


void CBallCurve::ReadPosParam()
{
	CString strPath;
	CString strTemp;
	strPath = m_sPath + _T(CONFIGURE_FILE) + _T("\\") + _T(CONFIGURE_NAME);

	GetPrivateProfileString(_T("PosParam"), _T("PosLimit"), _T("1"), strTemp.GetBuffer(100), 100, strPath);
	strTemp.ReleaseBuffer();
	strTemp.Trim();
	m_nPosLimit = _ttoi(strTemp);

	GetPrivateProfileString(_T("PosParam"), _T("PosZero"), _T("2"), strTemp.GetBuffer(100), 100, strPath);
	strTemp.ReleaseBuffer();
	strTemp.Trim();
	m_nPosZero = _ttoi(strTemp);

	GetPrivateProfileString(_T("PosParam"), _T("PosCeil"), _T("3"), strTemp.GetBuffer(100), 100, strPath);
	strTemp.ReleaseBuffer();
	strTemp.Trim();
	m_nPosCeil = _ttoi(strTemp);

	GetPrivateProfileString(_T("PosParam"), _T("PosStart"), _T("4"), strTemp.GetBuffer(100), 100, strPath);
	strTemp.ReleaseBuffer();
	strTemp.Trim();
	m_nPosStart = _ttoi(strTemp);

	GetPrivateProfileString(_T("PosParam"), _T("PosDown1"), _T("8"), strTemp.GetBuffer(100), 100, strPath);
	strTemp.ReleaseBuffer();
	strTemp.Trim();
	m_nPosDown1 = _ttoi(strTemp);

	GetPrivateProfileString(_T("PosParam"), _T("PosDown2"), _T("8"), strTemp.GetBuffer(100), 100, strPath);
	strTemp.ReleaseBuffer();
	strTemp.Trim();
	m_nPosDown2 = _ttoi(strTemp);

	GetPrivateProfileString(_T("PosParam"), _T("PosDown3"), _T("8"), strTemp.GetBuffer(100), 100, strPath);
	strTemp.ReleaseBuffer();
	strTemp.Trim();
	m_nPosDown3 = _ttoi(strTemp);
}

int CBallCurve::GetWidth()
{
	return int(m_nMargin * 2 * m_dZoom);
}

int CBallCurve::GetHeight()
{
	return int(m_nMargin  * 2 * m_dZoom);
}

//坐标原点换算到2D中心位置
CPoint CBallCurve::Lp2Dp(const CPoint& point)
{
	double x = point.x * m_dZoom + m_ptStartPoint.x + m_pt2DCenterPoint.x;
	double y = point.y * m_dZoom + m_ptStartPoint.y + m_pt2DCenterPoint.y;
	int nx, ny;
	if (x - floor(x) < 0.5)
		nx = (int)floor(x);
	else
		nx = (int)floor(x) + 1;
	if (y - floor(y) < 0.5)
		ny = (int)floor(y);
	else
		ny = (int)floor(y) + 1;

	return CPoint(nx, ny);
}

CPoint CBallCurve::Dp2Lp(const CPoint& point)
{
	double x = (point.x - m_pt2DCenterPoint.x - m_ptStartPoint.x) / m_dZoom;
	double y = (point.y - m_pt2DCenterPoint.y - m_ptStartPoint.y) / m_dZoom;

	int nx, ny;
	if (x - floor(x) < 0.5)
		nx = (int)floor(x);
	else
		nx = (int)floor(x) + 1;
	if (y - floor(y) < 0.5)
		ny = (int)floor(y);
	else
		ny = (int)floor(y) + 1;

	return CPoint(nx, ny);
}

void CBallCurve::Draw(CDC* pDC, CRect rRect)
{
	m_pt2DCenterPoint = rRect.CenterPoint();

	pDC->FillSolidRect(rRect, m_colBackGround);

	int left, top, right, bottom;
	CPoint leftTop, rightBottom;

	CPen pen;
	CPen* pOldPen;
	CBrush* pOldBrush;

	pen.CreatePen(PS_DASHDOT, 1, m_colCoordinate);
	pOldPen = pDC->SelectObject(&pen);
	pOldBrush = (CBrush*)pDC->SelectStockObject(HOLLOW_BRUSH);

	//画出XY坐标轴
	left = -m_nBallsX * 0.7 - 200;
	top = 0;
	right = -left;
	bottom = 0;
	leftTop = Lp2Dp(CPoint(left, top));
	rightBottom = Lp2Dp(CPoint(right, bottom));
	pDC->MoveTo(leftTop);
	pDC->LineTo(rightBottom);

	left = 0;
	top = -m_nBallsY*0.7 - 200;
	right = 0;
	bottom = -top;
	leftTop = Lp2Dp(CPoint(left, top));
	rightBottom = Lp2Dp(CPoint(right, bottom));
	pDC->MoveTo(leftTop);
	pDC->LineTo(rightBottom);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pen.DeleteObject();

	//画出小球
	for (int i = 0; i < m_nBallNum; i++)
		m_pBalls[i]->Draw(pDC, rRect);

	//写行列数
	int nCoord[116][2];
	int nNum = 2 * (ROW_NUM + COL_NUM);
	ReadCoordinate(_T(ROWCOL_COORDINATE), nCoord);
	for (int i = 0; i < nNum;i++)
	{
		CString itos;
		COLORREF color = RGB(255, 255, 255) - m_colBackGround;
		pDC->SetTextColor(color);
		pDC->SetBkMode(TRANSPARENT);
		CFont	textFont;
		textFont.CreateFont(10, 0, 0, 0, 600, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("宋体"));
		CFont* pFont = pDC->SelectObject(&textFont);
		if (i < COL_NUM)
		{
			itos.Format(_T("%d"), i+1);
			CString text = itos;
			//CString text = _T("C") + itos;

			int nleft = nCoord[i][0]*0.7-120;
			int ntop = nCoord[i][1] * 0.7 - 100;
			int nright = nCoord[i][0] * 0.7 + 100;
			int nbottom = nCoord[i][1] * 0.7 + 100;

			CPoint leftTop = Lp2Dp(CPoint(nleft, ntop));
			CPoint rightBottom = Lp2Dp(CPoint(nright, nbottom));
			CRect textRect(leftTop, rightBottom);

			pDC->DrawText(text, &textRect, DT_WORDBREAK | DT_CENTER | DT_VCENTER | DT_NOPREFIX);
			pDC->SelectObject(pFont);
		}
		//if (i >= COL_NUM && i < (COL_NUM + ROW_NUM))//右上角开始
		if (i >= 2*COL_NUM + ROW_NUM && i < 2 * (COL_NUM + ROW_NUM))//左下角开始
		{
			itos.Format(_T("%d"), i - 2 * COL_NUM- ROW_NUM + 1);
			CString text = itos;
			//CString text = _T("C") + itos;

			//右上角开始
			/*int nleft = nCoord[i][0] * 0.7 - 80;
			int ntop = nCoord[i][1] * 0.7 - 80;
			int nright = nCoord[i][0] * 0.7 + 140;
			int nbottom = nCoord[i][1] * 0.7 + 120;*/
			int nleft = nCoord[i][0] * 0.7 - 120;
			int ntop = nCoord[i][1] * 0.7 - 80;
			int nright = nCoord[i][0] * 0.7 + 100;
			int nbottom = nCoord[i][1] * 0.7 + 120;

			CPoint leftTop = Lp2Dp(CPoint(nleft, ntop));
			CPoint rightBottom = Lp2Dp(CPoint(nright, nbottom));
			CRect textRect(leftTop, rightBottom);

			pDC->DrawText(text, &textRect, DT_WORDBREAK | DT_CENTER | DT_VCENTER | DT_NOPREFIX);
			pDC->SelectObject(pFont);
		}
	}

	//画出color bar
	CRect barRect(0, rRect.Height() - 20 - 300, 60, rRect.Height());  //color bar背景大小
	pDC->FillSolidRect(barRect, m_colBackGround);
	for (int i = 0; i < 300; i++)
	{
		pen.CreatePen(PS_SOLID, 1, CalGradientColor(i, 300));
		pOldPen = pDC->SelectObject(&pen);
		pDC->MoveTo(10, i + rRect.Height() - 10 - 300);
		pDC->LineTo(40, i + rRect.Height() - 10 - 300);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
	}
	COLORREF textColor = RGB(255, 255, 255) - m_colBackGround;
	pDC->SetTextColor(textColor);
	pDC->SetBkMode(TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject(&m_textFont);
	CString text = _T("上");
	CRect textRect(45, rRect.Height() - 10 - 300, 60, rRect.Height() + 10 - 300);
	pDC->DrawText(text, &textRect, DT_WORDBREAK | DT_VCENTER | DT_NOPREFIX);
	text = _T("下");
	textRect = CRect(45, rRect.Height() - 25, 60, rRect.Height()-5);
	pDC->DrawText(text, &textRect, DT_WORDBREAK | DT_VCENTER | DT_NOPREFIX);
	pDC->SelectObject(pOldFont);
}

void CBallCurve::Draw3D()
{
	GLfloat LightAmbient[] = { 0.1f, 0.1f, 0.1f, 0.1f };
	GLfloat LightDiffuse[] = { 0.7f, 0.7f, 0.7f, 0.7f };
	GLfloat LightSpecular[] = { 0.0f, 0.0f, 0.0f, 0.1f };
	GLfloat LightPosition[] = { -5.0f, -5.0f, 5.0f, 0.0f };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT0);

	glPushMatrix();
	glEnable(GL_NORMALIZE);

	glTranslated(0.0, 0.0, -3);
	glRotated(90, 1.0, 0.0, 0.0);
	glTranslated(0, -3.0, -1);

	if (m_bAutoRotation)
		m_dXRotation += 5;
	glRotated(m_dXRotation, 0.0, 0.0, 1.0);

	for (int i = 0; i < m_nBallNum; i++)
	{
		m_pBalls[i]->Draw3D();
	}
	glPopMatrix();
}

COLORREF CBallCurve::CalGradientColor(double index, double range)
{
	double h, s, v;
	double r, g, b;
	h = m_colUpH + index / range*(m_colDownH - m_colUpH);
	s = m_colUpS + index / range*(m_colDownS - m_colUpS);
	v = m_colUpV + index / range*(m_colDownV - m_colUpV);

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
	}

	return RGB(int(r * 255), int(g * 255), int(b * 255));
}

void CBallCurve::SetColorContents(DWORD nStartTime)
{
	for (int i = 0; i < m_nBallNum; i++)
		m_pBalls[i]->SetCurrentPosition(nStartTime);
}

double CBallCurve::SetPosSpeedContents(int pos[], double time)
{
	double dProperTime = time;
	int nPos;

	for (int i = 0; i < m_nBallNum; i++)
	{
		nPos = GetAbsolutePosition(pos[i]);
		m_pBalls[i]->SetNextPosition(nPos);
		double dCalTime = m_pBalls[i]->CalSpeed(time);
		if (dProperTime < dCalTime)
			dProperTime = dCalTime;//从每个小球得到最长时间
	}
	if (dProperTime > time)
	{
		for (int i = 0; i < m_nBallNum; i++)
			m_pBalls[i]->CalSpeed(dProperTime);//根据时间得到每个小球的速度，从而得到每个小球的当前位置
	}

	return dProperTime;
}

int CBallCurve::SetPosSpecialContents(int mode)
{
	int nProperTime = 0;
	if (mode == MODE_START)
	{
		int time;
		int pos;
		for (int i = 0; i < m_nBallNum; i++)
		{
			pos = GetAbsolutePosition(MODE_START);
			m_pBalls[i]->SetNextPosition(pos);
			time = m_pBalls[i]->CalTime(UNIT_SPEED);
			if (nProperTime < time)
				nProperTime = time;
		}//从每个小球得到最长时间
		if (nProperTime > 0)
		{
			for (int i = 0; i < m_nBallNum; i++)
				m_pBalls[i]->CalSpeed(nProperTime);//根据时间得到每个小球的速度，从而得到每个小球的当前位置
		}
	}
	
	return nProperTime;
}

int CBallCurve::GetAbsolutePosition(int mode)
{
	int absPos;
	if (mode==MODE_LIMIT)
	{
		absPos = 0;
	}
	else if (mode==MODE_ZERO)
	{
		absPos = (m_nPosZero - m_nPosLimit) * 9999 / (m_nPosDown3 - m_nPosStart);
	}
	else if (mode == MODE_CEIL)
	{
		absPos = (m_nPosCeil - m_nPosLimit) * 9999 / (m_nPosDown3 - m_nPosStart);
	}
	else if (mode == MODE_START)
	{
		absPos = (m_nPosStart - m_nPosLimit) * 9999 / (m_nPosDown3 - m_nPosStart);
	}
	else if (mode == MODE_DOWN1)
	{
		absPos = (m_nPosDown1 - m_nPosStart) * 9999 / (m_nPosDown3 - m_nPosStart) + m_nRelStartPos;
	}
	else if (mode == MODE_DOWN2)
	{
		absPos = (m_nPosDown2 - m_nPosStart) * 9999 / (m_nPosDown3 - m_nPosStart) + m_nRelStartPos;
	}
	else if (mode == MODE_DOWN3)
	{
		absPos = (m_nPosDown3 - m_nPosStart) * 9999 / (m_nPosDown3 - m_nPosStart) + m_nRelStartPos;
	}
	else
	{
		absPos = mode + m_nRelStartPos;//小球的
	}
	return absPos;
}

// CBallCurve 成员函数
