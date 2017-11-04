// ADSConnect.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BallArray.h"
#include "ADSConnect.h"

// CADSConnect
char szVarName[5][50] =
{
	"MAIN.bNewFile_down",  //fasle���ļ�����
	"MAIN.iFileNume_Ftp",  //�����ļ����� 
	"MAIN.iNoFile",        //�ļ�������     

};
CADSConnect::CADSConnect()
{
	m_lPort = AdsPortOpen(); //��ADSͨѶ�˿�
	m_lErr = AdsGetLocalAddress(m_pAddr); //�õ����ض˿ڵ�ַ
	if (m_lErr)
	{
		AfxMessageBox(_T("���ض˿ڵ�ַ��ȡʧ�ܣ�"));
		return;
	}
	m_pAddr->port = AMSPORT_R0_PLC_RTS1; //��ָ��ָ��801�˿�
	//��ʼ�� ������д���
	if (m_lErr == 0)
	{
		m_lErr = AdsSyncReadWriteReq(m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(_bHandle), &_bHandle, sizeof(szVarName[0]), szVarName[0]);
		m_lErr = AdsSyncReadWriteReq(m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(_iHandle), &_iHandle, sizeof(szVarName[1]), szVarName[1]);
		m_lErr = AdsSyncReadWriteReq(m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(_aHandle), &_aHandle, sizeof(szVarName[2]), szVarName[2]);
	}

}

CADSConnect::~CADSConnect()
{
	m_lErr = AdsPortClose(); //�ر�ADSͨѶ�˿�
	//if (nErr) cerr << "Error: AdsPortClose: " << nErr << '\n'; //���ر�ͨѶ�˿ڵĲ����Ƿ�ִ�гɹ�
	if (m_lErr)
	{
		AfxMessageBox(_T("ͨѶ�˿ڹر�ʧ�ܣ�"));
		return;
	}
}

bool CADSConnect::ReadWriteBoolValue(int i,bool b)
{
	if (i==1)//��boolֵ
	{
		while (1); //��while�����ʵ��ѭ����ȡ
		{
			//��ADS������ͬ����ȡ���ݣ�pAddr��ADS�豸�ĵ�ַ��0x4020���ε�ַ��0x0ƫ�Ƶ�ַ��0x1�����ݳ��ȣ� &m_bBool���������ݵĻ���
			m_lErr = AdsSyncReadReq(m_pAddr, 0x4020, 0x0, 0x1, &m_bBool); 
			//if (nErr) cerr << "Error: AdsSyncReadReq: " << nErr << '\n'; //����ȡ��ַ�Ĳ����Ƿ�ִ�гɹ�
			return m_bBool;
		}
	}
	if (i == 2)//дboolֵ
	{
		//ͬ��д���ݵ�ADS�豸��pAddr��ADS�豸�ĵ�ַ��0x4020���ε�ַ��0x0ƫ�Ƶ�ַ��0x1�����ݳ��ȣ�@BOOL1:�������ݵĻ���
		m_lErr = AdsSyncWriteReq(m_pAddr, 0x4020, 0x0, 0x1, &b); 
		if (m_lErr)
			AfxMessageBox(_T("д�벼����ʧ�ܣ�"));
		return m_lErr;
	}
}

short CADSConnect::ReadWriteIntValue(int i, short n)//short�����ֽ�
{
	if (i == 1)//��boolֵ
	{
		while (1); //��while�����ʵ��ѭ����ȡ
		{
			m_lErr = AdsSyncReadReq(m_pAddr, 0x4020, 0x2, 0x4, &m_nsInt); 
			return m_bBool;
		}
		if (m_lErr)
		{
			AfxMessageBox(_T("��ȡ����ʧ�ܣ�"));
			return m_lErr;
		}
	}
	if (i == 2)//дboolֵ
	{
		m_lErr = AdsSyncWriteReq(m_pAddr, 0x4020, 0x0, 0x1, &n); 
		if (m_lErr)
			AfxMessageBox(_T("д������ʧ�ܣ�"));
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

// CADSConnect ��Ա����
