#pragma once

#include "MyConstMeth.h"

// CCurveData ����Ŀ��

class CCurveData : public CObject
{
public:
	CCurveData();
	virtual ~CCurveData();

public:
	const static int m_nMaxFrame = 50;			//���֡��	
	const static int m_nBallNum = 35 * 23;		//С����
	const static int m_nMaxRow = 1 + m_nMaxFrame + 35 * m_nMaxFrame;		//��������

	int	m_nFrameNum;							                             //��֡��
	int	m_nCurrentFrame;							                         //��ǰ֡��
	int   m_nCurveData[m_nMaxFrame][m_nBallNum];	    //С��λ��
	ProjectData* p_stData[m_nMaxRow];

public:
	bool ReadCurveData(CString path);
};


