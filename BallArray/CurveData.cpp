// CurveData.cpp : 实现文件
//

#include "stdafx.h"
#include "CurveData.h"


// CCurveData

CCurveData::CCurveData()
{
	m_nFrameNum = 0;
	for (int i = 0; i < m_nMaxFrame; i++)
	{
		for (int j = 0; j < m_nBallNum; j++)
			m_nCurveData[i][j] = 0;
	}
	for (int j = 0; j < m_nMaxRow;j++)
	{
		p_stData[j] = new ProjectData;
	}
}

CCurveData::~CCurveData()
{
	for (int j = 0; j < m_nMaxRow; j++)
	{
		delete p_stData[j];
	}
}

// CCurveData 成员函数

bool CCurveData::ReadCurveData(CString path)
{
	FILE* ff;
	CFileStatus status;
	errno_t err;
	if (CFile::GetStatus(path, status))
	{
		err = _wfopen_s(&ff, path, _T("rt"));
		CString str;
		CString sFlag;
		CString sNum;
		CString sPos;

		int index = 0;

		if (err != 0)
		{
			str.Format(_T("文件%s无法打开！"), path);
			AfxMessageBox(str);
			return false;
		}

		while (fscanf(ff, "%s %s", p_stData[index]->sTitle, p_stData[index]->sData)>0)
		{
			sFlag = p_stData[index]->sTitle;
			sNum = p_stData[index]->sData;
			if (sFlag==_T("FrameNum"))//总帧数
			{
				m_nFrameNum = _ttoi(sNum);
			}
			if (sFlag == _T("Frame"))//当前帧
			{
				m_nCurrentFrame = _ttoi(sNum);
			}
			for (int j = 0; j < 35; j++)
			{
				str.Format(_T("Ball%03d-%03d"), j * 23 + 1, (j + 1) * 23);
				if (sFlag ==str)
				{
					for (int k = 0; k < ((sNum.GetLength() / 4>23) ? 23 : sNum.GetLength() / 4); k++)
					{
						sPos = sNum.Mid(k * 4, 4);
						m_nCurveData[m_nCurrentFrame-1][j * 23 + k] = _ttoi(sPos);
					}
				}
			}
			index++;
		}
	}
	return true;
}
