#pragma once
#pragma comment (lib,"TcAdsDll.lib")
#include ".\Include\TcAdsDef.h" //结构体和常量的声明
#include ".\Include\TcAdsAPI.h" // ADS函数的声明
// CADSConnect 命令目标

struct STR_DATA
{
	short nDat;
	byte bData;
};
class CADSConnect : public CObject
{
public:
	CADSConnect();
	virtual ~CADSConnect();

public:
	long       m_lErr;
	long       m_lPort;	            //定义端口变量
	AmsAddr    m_Addr;		        //定义AMS地址变量
	PAmsAddr   m_pAddr = &m_Addr;  	//定义端口地址变量
	bool       m_bBool;		        //定义布尔量
	short      m_nsInt;             //定义短整型
	//short*     m_shortData=new short[];

	//ADS读写句柄
	unsigned long _bHandle;   //布尔句柄
	unsigned long _iHandle;   //整型句柄 
	unsigned long _aHandle;   //数组句柄
public:
	bool  ReadWriteBoolValue(int i, bool b);
	short ReadWriteIntValue(int i, short n);
	bool  ReadWriteFtpReq(bool* b, short* nNum,short* nFile);
};


