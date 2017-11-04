#pragma once

#include "MyConstMeth.h"

// CCurveData 命令目标

class CCurveData : public CObject
{
public:
	CCurveData();
	virtual ~CCurveData();

public:
	const static int m_nMaxFrame = 50;			//最大帧数	
	const static int m_nBallNum = 35 * 23;		//小球数
	const static int m_nMaxRow = 1 + m_nMaxFrame + 35 * m_nMaxFrame;		//数据行数

	int	m_nFrameNum;							                             //总帧数
	int	m_nCurrentFrame;							                         //当前帧数
	int   m_nCurveData[m_nMaxFrame][m_nBallNum];	    //小球位置
	ProjectData* p_stData[m_nMaxRow];

public:
	bool ReadCurveData(CString path);
};


