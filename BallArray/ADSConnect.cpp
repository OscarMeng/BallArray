// ADSConnect.cpp : 实现文件
//

#include "stdafx.h"
#include "BallArray.h"
#include "ADSConnect.h"

// CADSConnect
char szVarName[5][50] =
{
	"MAIN.bNewFile_down",  //fasle是文件下载
	"MAIN.iFileNume_Ftp",  //下载文件数量 
	"MAIN.iNoFile",        //文件数组编号     

};
CADSConnect::CADSConnect()
{
	m_lPort = AdsPortOpen(); //打开ADS通讯端口
	m_lErr = AdsGetLocalAddress(m_pAddr); //得到本地端口地址
	if (m_lErr)
	{
		AfxMessageBox(_T("本地端口地址获取失败！"));
		return;
	}
	m_pAddr->port = AMSPORT_R0_PLC_RTS1; //将指针指向801端口
	//初始化 创建读写句柄
	if (m_lErr == 0)
	{
		m_lErr = AdsSyncReadWriteReq(m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(_bHandle), &_bHandle, sizeof(szVarName[0]), szVarName[0]);
		m_lErr = AdsSyncReadWriteReq(m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(_iHandle), &_iHandle, sizeof(szVarName[1]), szVarName[1]);
		m_lErr = AdsSyncReadWriteReq(m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(_aHandle), &_aHandle, sizeof(szVarName[2]), szVarName[2]);
	}

}

CADSConnect::~CADSConnect()
{
	m_lErr = AdsPortClose(); //关闭ADS通讯端口
	//if (nErr) cerr << "Error: AdsPortClose: " << nErr << '\n'; //检查关闭通讯端口的操作是否执行成功
	if (m_lErr)
	{
		AfxMessageBox(_T("通讯端口关闭失败！"));
		return;
	}
}

bool CADSConnect::ReadWriteBoolValue(int i,bool b)
{
	if (i==1)//读bool值
	{
		while (1); //用while语句来实现循环读取
		{
			//从ADS服务器同步读取数据，pAddr：ADS设备的地址，0x4020：段地址，0x0偏移地址，0x1：数据长度， &m_bBool：接收数据的缓存
			m_lErr = AdsSyncReadReq(m_pAddr, 0x4020, 0x0, 0x1, &m_bBool); 
			//if (nErr) cerr << "Error: AdsSyncReadReq: " << nErr << '\n'; //检查获取地址的操作是否执行成功
			return m_bBool;
		}
	}
	if (i == 2)//写bool值
	{
		//同步写数据到ADS设备，pAddr：ADS设备的地址，0x4020：段地址，0x0偏移地址，0x1：数据长度，@BOOL1:接收数据的缓存
		m_lErr = AdsSyncWriteReq(m_pAddr, 0x4020, 0x0, 0x1, &b); 
		if (m_lErr)
			AfxMessageBox(_T("写入布尔型失败！"));
		return m_lErr;
	}
}

short CADSConnect::ReadWriteIntValue(int i, short n)//short两个字节
{
	if (i == 1)//读bool值
	{
		while (1); //用while语句来实现循环读取
		{
			m_lErr = AdsSyncReadReq(m_pAddr, 0x4020, 0x2, 0x4, &m_nsInt); 
			return m_bBool;
		}
		if (m_lErr)
		{
			AfxMessageBox(_T("读取整型失败！"));
			return m_lErr;
		}
	}
	if (i == 2)//写bool值
	{
		m_lErr = AdsSyncWriteReq(m_pAddr, 0x4020, 0x0, 0x1, &n); 
		if (m_lErr)
			AfxMessageBox(_T("写入整型失败！"));
		return m_lErr;
	}
}


bool CADSConnect::ReadWriteFtpReq(bool* b, short* nNum, short* nFile)
{
	bool boolData;
	short shortData;
	short *arrayData=new short[];

	int a = sizeof(arrayData);
	int c = sizeof(nFile);


	m_lErr = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_VALBYHND, _bHandle, sizeof(bool), b);
	m_lErr = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_VALBYHND, _iHandle, sizeof(short), nNum);
	m_lErr = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_VALBYHND, _bHandle, sizeof(short)* 50, nFile);
	return true;
}

// CADSConnect 成员函数
