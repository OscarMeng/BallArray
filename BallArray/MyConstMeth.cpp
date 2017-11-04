#include "stdafx.h"
#include "MyConstMeth.h"


CMyConstMeth::CMyConstMeth()
{
}


CMyConstMeth::~CMyConstMeth()
{
}

//读取模块文件路径
CString CMyConstMeth::ReadModuleFileNm()
{
	CString strTemp;
	TCHAR lpFileName[200 + 1];

	GetModuleFileName(NULL, lpFileName, 200);
	PathRemoveFileSpec(lpFileName);            //去掉.exe成为"*\\BallArray\\Debug\\"
	strTemp.Format(_T("%s"), lpFileName);
	
	strTemp = strTemp.Left(strTemp.GetLength() - 5);    //向左去掉5个字符成为"*\\BallArray\\"
	return strTemp;
}
