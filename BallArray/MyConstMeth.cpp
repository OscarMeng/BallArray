#include "stdafx.h"
#include "MyConstMeth.h"


CMyConstMeth::CMyConstMeth()
{
}


CMyConstMeth::~CMyConstMeth()
{
}

//��ȡģ���ļ�·��
CString CMyConstMeth::ReadModuleFileNm()
{
	CString strTemp;
	TCHAR lpFileName[200 + 1];

	GetModuleFileName(NULL, lpFileName, 200);
	PathRemoveFileSpec(lpFileName);            //ȥ��.exe��Ϊ"*\\BallArray\\Debug\\"
	strTemp.Format(_T("%s"), lpFileName);
	
	strTemp = strTemp.Left(strTemp.GetLength() - 5);    //����ȥ��5���ַ���Ϊ"*\\BallArray\\"
	return strTemp;
}
