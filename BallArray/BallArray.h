
// BallArray.h : BallArray Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CBallArrayApp:
// �йش����ʵ�֣������ BallArray.cpp
//

class CBallArrayApp : public CWinApp
{
public:
	CBallArrayApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBallArrayApp theApp;