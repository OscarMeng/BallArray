#pragma once
#pragma comment (lib,"TcAdsDll.lib")
#include ".\Include\TcAdsDef.h" //�ṹ��ͳ���������
#include ".\Include\TcAdsAPI.h" // ADS����������
// CADSConnect ����Ŀ��

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
	long       m_lPort;	            //����˿ڱ���
	AmsAddr    m_Addr;		        //����AMS��ַ����
	PAmsAddr   m_pAddr = &m_Addr;  	//����˿ڵ�ַ����
	bool       m_bBool;		        //���岼����
	short      m_nsInt;             //���������
	//short*     m_shortData=new short[];

	//ADS��д���
	unsigned long _bHandle;   //�������
	unsigned long _iHandle;   //���;�� 
	unsigned long _aHandle;   //������
public:
	bool  ReadWriteBoolValue(int i, bool b);
	short ReadWriteIntValue(int i, short n);
	bool  ReadWriteFtpReq(bool* b, short* nNum,short* nFile);
};


