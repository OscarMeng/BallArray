#pragma once

#include "afxcmn.h"
#include "ColorList.h"
#include "MyConstMeth.h"
#include "afxwin.h"

// CControlView 窗体视图

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
	CControlView();           // 动态创建所使用的受保护的构造函数
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	CEdit		m_eBinPath;//二进制文件路径
	CEdit		m_eBinName;//二进制文件名

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

	CString      m_sPath;                                                   //软件路径
	ProjectDir*  p_tPrjDir[CURVE_NUM];                           //工程文件信息
	int          m_nCurveNum;                                        //工程文件信息数量
	double       m_dPos[MAX_FRAME][WRITE_NUM];    //最多插入10000行，有161列
	double       m_dWritePos[WRITE_NUM];                  //交换写入的BIN的位置
	double       m_dBallPos[BALLS_NUM];                     //存放调试时小球的位置
	double       m_dWriteBall[BALLS_NUM];                 //存入调试时小球的位置
	double       m_dPos255[MAX_FRAME][BALLS_NUM];    //最多插入10000行，有805列
private:
	bool  	m_bStopped;
	bool	m_bWaiting;
	bool	m_bListInited;
	bool	m_bCtrlsValid;
	int		m_nRunMode;                     //运行模式：读取文件运行，操作界面运行
	int		m_nCurrentListIndex;          //当前行
	int		m_nNextListIndex;              //下一行
	int		m_nPlayIndex;
	int		m_nPauseIndex;
	int     m_nFrameIndex;
	double	m_dRunFrameTm;             //运行帧数所需要的时间
	int		m_nFrameTotal;                //总帧数
	int		m_nPauseTotal;                //暂停总秒数
	DWORD	m_nStartTime;		         //开始运行时的毫秒数
	int		m_nRunTotal;				     //运行总秒数
	double  m_dTime;					     //每帧显示时间
	int     m_nClickedItem;             //选中的行数
	int     m_nListItems;                 //总行数		
	int     m_nSpecialTime;            //运行到特定位置所需要时间，用电机的正常速度

	CIPAddressCtrl m_ipAddr;           //地址
	long           m_nIpPort;         //端口
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
	void ChangeWritePos(int nNum,double dPos[]);  //把小球的位置变化为要求写入的顺序
	void PlayBegin();
	void PlayRun();
	bool SetBallValue();//读取数据给小球
	void SetPosColor();//设置小球当前位置，从而得到颜色
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

	// 小球的行数
	int m_nRow;
	// 小球的列数
	int m_nCol;
	// 小球的运行高度
	int m_nHeight;
	afx_msg void OnBnClickedBtnMotor();
	afx_msg void OnBnClickedBtnLimit();
	afx_msg void OnBnClickedBtnZero();
	afx_msg void OnBnClickedBtnCeil();
	afx_msg void OnBnClickedBtnStart();
	// 工程名称
	CString m_eProName;
	CButton m_btRotation;
	afx_msg void OnBnClickedBtnRotation();
	afx_msg void OnBnClickedBtnEnd();
	// 服务器端口号
	int m_nServerPort;
	afx_msg void OnBnClickedBtnServe();

};


