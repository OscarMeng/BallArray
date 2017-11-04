#pragma once

#include "afxcmn.h"
#include "ColorList.h"
#include "MyConstMeth.h"
#include "afxwin.h"

// CControlView ������ͼ

class CMainFrame;
class C3DView;
class CBallArrayView;
class CBallCurve;
class CCurveData;
class CADSConnect;

class CControlView : public CFormView
{
	DECLARE_DYNCREATE(CControlView)

protected:
	CControlView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CControlView();

public:
	enum { IDD = IDD_CONTROL_VIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
private:
	static const int m_nTopMargin = 10;
	static const int m_nLeftMargin = 10;
	static const int m_nRightMargin = 10;
	static const int m_nBottomMargin = 10;
	static const int m_nButtonWidth = 40;
	static const int m_nButtonHeight = 30;
	static const int m_nRadioHeight = 20;
	static const int m_nStaticHeight = 20;
	static const int m_nStaticWidth1 = 80;
	static const int m_nStaticWidth2 = 300;
	static const int m_nBarWidth = 4;
	static const int m_nBarSpace = 2;
	static const int m_nCtrlSpace = 10;
	static const int m_nAllWidth = 560;
	static const int m_nTimerTime = 40;
public:
	CColorList	m_clContent;

	CButton     m_btNew;
	CButton		m_btOpen;
	CButton		m_btPreview;
	CButton		m_btPreviewAll;
	CButton		m_btRun;
	CButton     m_btSend;
	CButton		m_btModify;
	CButton		m_btStop;
	CEdit		m_eBinPath;//�������ļ�·��
	CEdit		m_eBinName;//�������ļ���

	CButton		m_btBar1;
	CButton		m_btBar2;
	CStatic		m_stRunContents;
	CButton		m_rbOrder;
	CButton		m_rbRand;
	CStatic		m_stManually;
	CButton		m_btLimit;
	CButton		m_btLimitToZero;
	CButton		m_btBackToZero;
	CButton		m_btMiddle;
	CStatic		m_stPos;
	CStatic		m_stSpeed;
	CEdit		m_ePos;
	CEdit		m_eTime;
	CButton		m_btDo;
	CButton		m_btShutDown;
	CButton		m_chLoop;
	CButton		m_chAutoRotation;

	CString      m_sPath;                                                   //���·��
	ProjectDir*  p_tPrjDir[CURVE_NUM];                           //�����ļ���Ϣ
	int          m_nCurveNum;                                        //�����ļ���Ϣ����
	double       m_dPos[MAX_FRAME][WRITE_NUM];    //������10000�У���161��
	double       m_dWritePos[WRITE_NUM];                  //����д���BIN��λ��
	double       m_dBallPos[BALLS_NUM];                     //��ŵ���ʱС���λ��
	double       m_dWriteBall[BALLS_NUM];                 //�������ʱС���λ��
	double       m_dPos255[MAX_FRAME][BALLS_NUM];    //������10000�У���805��
private:
	bool  	m_bStopped;
	bool	m_bWaiting;
	bool	m_bListInited;
	bool	m_bCtrlsValid;
	int		m_nRunMode;                     //����ģʽ����ȡ�ļ����У�������������
	int		m_nCurrentListIndex;          //��ǰ��
	int		m_nNextListIndex;              //��һ��
	int		m_nPlayIndex;
	int		m_nPauseIndex;
	int     m_nFrameIndex;
	double	m_dRunFrameTm;             //����֡������Ҫ��ʱ��
	int		m_nFrameTotal;                //��֡��
	int		m_nPauseTotal;                //��ͣ������
	DWORD	m_nStartTime;		         //��ʼ����ʱ�ĺ�����
	int		m_nRunTotal;				     //����������
	double  m_dTime;					     //ÿ֡��ʾʱ��
	int     m_nClickedItem;             //ѡ�е�����
	int     m_nListItems;                 //������		
	int     m_nSpecialTime;            //���е��ض�λ������Ҫʱ�䣬�õ���������ٶ�

	CIPAddressCtrl m_ipAddr;           //��ַ
	long           m_nIpPort;         //�˿�
private:
	CMainFrame*		m_pMainFrame;
	C3DView*		m_p3DView;
	CBallArrayView*	m_pBallArrayView;
	CBallCurve*		m_pBallCurve;
	CCurveData*		m_pCurveData;
	CADSConnect*    m_pADS;
public:
	inline void		SetMainFrame(CMainFrame* pMainFrame)					    { m_pMainFrame = pMainFrame; }
	inline void		Set3DView(C3DView* p3DView)										{ m_p3DView = p3DView; }
	inline void		SetBallArrayView(CBallArrayView*	pBallArrayView)		{ m_pBallArrayView = pBallArrayView; }
	inline void		SetBallCurve(CBallCurve* pBallCurve)					            { m_pBallCurve = pBallCurve; }
	inline void		SetCurveData(CCurveData* pCurveData)							{ m_pCurveData = pCurveData; }
	inline bool		IsRunning()			{ return !m_bStopped; }

private:
	void ReadProject();
	void ChangeWritePos(int nNum,double dPos[]);  //��С���λ�ñ仯ΪҪ��д���˳��
	void PlayBegin();
	void PlayRun();
	bool SetBallValue();//��ȡ���ݸ�С��
	void SetPosColor();//����С��ǰλ�ã��Ӷ��õ���ɫ
public:
	afx_msg void OnNMClickedItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDbClickedItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnPreview();
	afx_msg void OnBnClickedBtnRun();
	afx_msg void OnBnClickedBtnModify();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnMakeBin();
	afx_msg void OnBnClickedButtonOpenBin();
	afx_msg void OnBnClickedBtnNew();

	// С�������
	int m_nRow;
	// С�������
	int m_nCol;
	// С������и߶�
	int m_nHeight;
	afx_msg void OnBnClickedBtnMotor();
	afx_msg void OnBnClickedBtnLimit();
	afx_msg void OnBnClickedBtnZero();
	afx_msg void OnBnClickedBtnCeil();
	afx_msg void OnBnClickedBtnStart();
	// ��������
	CString m_eProName;
	CButton m_btRotation;
	afx_msg void OnBnClickedBtnRotation();
	afx_msg void OnBnClickedBtnEnd();
	// �������˿ں�
	int m_nServerPort;
	afx_msg void OnBnClickedBtnServe();

};


