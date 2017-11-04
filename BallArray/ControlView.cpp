// ControlView.cpp : 实现文件
//
//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "stdafx.h"
#include "BallArray.h"
#include "ControlView.h"
#include "MyConstMeth.h"
#include "ModifyDlg.h"
#include "TCPServerDlg.h"
#include "CurveData.h"
#include "BallCurve.h"
#include "BallArrayView.h"
#include "3DView.h"
#include "NewProjectDlg.h"
#include "ADSConnect.h"
// CControlView

IMPLEMENT_DYNCREATE(CControlView, CFormView)

CControlView::CControlView()
	: CFormView(CControlView::IDD)
	, m_nIpPort(0)
	, m_nRow(0)
	, m_nCol(0)
	, m_nHeight(0)
	, m_eProName(_T(""))
	, m_nServerPort(60000)
{
	m_bListInited = false;
	m_bCtrlsValid = false;
	m_bStopped = true;
	m_bWaiting = false;
	m_nNextListIndex = 0;
	m_nCurrentListIndex = 0;
	m_nRunMode = MODE_CONTENTS;
	m_nPlayIndex = 0;
	m_nFrameIndex = 0;
	m_nPauseIndex = 0;
	m_nFrameTotal = 0;
	m_nPauseTotal = 0;
	m_nClickedItem = -1;

	m_pADS = new CADSConnect;

	CMyConstMeth MyMeth;
	m_sPath = MyMeth.ReadModuleFileNm();

	for (int i = 0; i<CURVE_NUM;++i)
	{
		p_tPrjDir[i] = new ProjectDir;
	}
}

CControlView::~CControlView()
{
	for (int i = 0; i < CURVE_NUM; ++i)
	{
		delete p_tPrjDir[i];
	}
}

void CControlView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CONTENT, m_clContent);
	DDX_Control(pDX, IDC_BTN_NEW, m_btNew);
	DDX_Control(pDX, IDC_BTN_OPEN, m_btOpen);
	DDX_Control(pDX, IDC_BTN_PREVIEW, m_btPreview);
	DDX_Control(pDX, IDC_BTN_PREVIEW_ALL, m_btPreviewAll);
	DDX_Control(pDX, IDC_BTN_RUN, m_btRun);
	DDX_Control(pDX, IDC_BTN_SEND, m_btSend);
	DDX_Control(pDX, IDC_BTN_MODIFY, m_btModify);
	DDX_Control(pDX, IDC_BTN_STOP, m_btStop);
	DDX_Control(pDX, IDC_BIN_PATH, m_eBinPath);
	DDX_Control(pDX, IDC_BIN_NAME, m_eBinName);
	DDX_Control(pDX, IDC_IPADDRESS_LOCAL, m_ipAddr);
	DDX_Text(pDX, IDC_PORT, m_nIpPort);
	DDX_Text(pDX, IDC_ROW, m_nRow);
	DDX_Text(pDX, IDC_COL, m_nCol);
	DDX_Text(pDX, IDC_HEIGHT, m_nHeight);
	DDV_MinMaxInt(pDX, m_nRow, 0, 23);
	DDV_MinMaxInt(pDX, m_nCol, 0, 35);
	DDV_MinMaxInt(pDX, m_nHeight, 0, 9999);
	DDX_Text(pDX, IDC_PROJECT_NAME, m_eProName);
	DDX_Control(pDX, IDC_BTN_ROTATION, m_btRotation);
	DDX_Text(pDX, IDC_SERVER_PORT, m_nServerPort);
}

BEGIN_MESSAGE_MAP(CControlView, CFormView)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_CONTENT, &CControlView::OnNMClickedItem)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CONTENT, &CControlView::OnNMDbClickedItem)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CControlView::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_PREVIEW, &CControlView::OnBnClickedBtnPreview)
	ON_BN_CLICKED(IDC_BTN_RUN, &CControlView::OnBnClickedBtnRun)
	ON_BN_CLICKED(IDC_BTN_MODIFY, &CControlView::OnBnClickedBtnModify)
	ON_BN_CLICKED(IDC_BTN_STOP, &CControlView::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_MAKE_BIN, &CControlView::OnBnClickedBtnMakeBin)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_BIN, &CControlView::OnBnClickedButtonOpenBin)
	ON_BN_CLICKED(IDC_BTN_NEW, &CControlView::OnBnClickedBtnNew)
	ON_BN_CLICKED(IDC_BTN_MOTOR, &CControlView::OnBnClickedBtnMotor)
	ON_BN_CLICKED(IDC_BTN_LIMIT, &CControlView::OnBnClickedBtnLimit)
	ON_BN_CLICKED(IDC_BTN_ZERO, &CControlView::OnBnClickedBtnZero)
	ON_BN_CLICKED(IDC_BTN_CEIL, &CControlView::OnBnClickedBtnCeil)
	ON_BN_CLICKED(IDC_BTN_START, &CControlView::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_ROTATION, &CControlView::OnBnClickedBtnRotation)
	ON_BN_CLICKED(IDC_BTN_END, &CControlView::OnBnClickedBtnEnd)
	ON_BN_CLICKED(IDC_BTN_SERVER, &CControlView::OnBnClickedBtnServe)
END_MESSAGE_MAP()


// CControlView 诊断

#ifdef _DEBUG
void CControlView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CControlView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CControlView 消息处理程序


void CControlView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO:  在此添加专用代码和/或调用基类
	if (!m_bListInited)
	{
		CRect rect;
		// 获取列表视图控件的位置和大小   
		m_clContent.GetClientRect(&rect);

		m_clContent.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_ONECLICKACTIVATE);
		//选中某行使整行高亮（只适用与report风格的listctrl）| 网格线（只适用与report风格的listctrl）| 单击高亮
		CImageList   m_imageList;
		m_imageList.Create(1, rect.Height()/10, ILC_COLOR24, 1, 0);   //1是图片宽，rect.Height()/10是高度 
		m_clContent.SetImageList(&m_imageList, LVSIL_SMALL);
		/*CFont m_font;
		m_font.CreatePointFont(5, _T("宋体"));
		m_clContent.SetFont(&m_font);*/
		m_clContent.InsertColumn(0, NULL, LVCFMT_CENTER, 0);
		m_clContent.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rect.Width() / 7, 0);
		m_clContent.InsertColumn(2, _T("造型名称"), LVCFMT_CENTER, rect.Width() * 5 / 14, 1);
		m_clContent.InsertColumn(3, _T("运行时间"), LVCFMT_CENTER, rect.Width() * 7 / 28, 2);
		m_clContent.InsertColumn(4, _T("停顿时间"), LVCFMT_CENTER, rect.Width() * 7 / 28, 3);
		m_clContent.DeleteColumn(0);

		ReadProject();

		unsigned char chAddr[6];
		memcpy(chAddr, m_pADS->m_pAddr->netId.b, 6);
		m_ipAddr.SetAddress(chAddr[0], chAddr[1], chAddr[2], chAddr[3]);//本地地址
		m_nIpPort = m_pADS->m_pAddr->port;//端口
		UpdateData(FALSE);

		m_bListInited = true;
	}
	m_btStop.EnableWindow(FALSE);
	m_bCtrlsValid = true;
}

void CControlView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
}


void CControlView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == IDT_RUN)
	{
		PlayRun();
	}
	CFormView::OnTimer(nIDEvent);
}


void CControlView::OnNMClickedItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_nClickedItem = pNMItemActivate->iItem;//选中的行
	m_clContent.Invalidate();//重绘
	m_nListItems = m_clContent.GetItemCount();
	for (int i = 0; i < m_nListItems; i++)
	{
		if (i != m_nClickedItem)
		{
			m_clContent.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED | LVS_SHOWSELALWAYS | LVS_SINGLESEL);
		}
	}
	*pResult = 0;
}

void CControlView::OnNMDbClickedItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_nClickedItem = pNMItemActivate->iItem;
	m_clContent.Invalidate();
	if (m_nClickedItem < 0)
		return;
	if (!m_bStopped)//停止按钮不显示时，不运行时
	{
		m_bWaiting = true;
		m_nNextListIndex = m_nClickedItem;
		return;
	}

	PlayBegin();

	KillTimer(IDT_RUN);
	m_nRunMode = MODE_CONTENTS;
	m_nPlayIndex = 0;
	m_clContent.SetItemColor(m_nCurrentListIndex, RGB(0, 0, 0));
	m_nCurrentListIndex = m_nClickedItem;
	m_clContent.SetItemColor(m_nCurrentListIndex, RGB(255, 0, 0));
	m_clContent.Invalidate();
	SetTimer(IDT_RUN, 100, NULL);

	*pResult = 0;
}

void CControlView::OnBnClickedBtnNew()
{
	// TODO:  在此添加控件通知处理程序代码
	//创建模态对话框
	CNewProjectDlg NewPrjDlg(this);
	if (NewPrjDlg.DoModal() == IDOK)
	{
		ReadProject();
	}
}

void CControlView::OnBnClickedBtnOpen()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog fileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("TXT Files(*.txt)|*.txt||"));
	CString strPath;
	strPath = m_sPath + _T(PROJECTS) + _T("\\");
	fileDialog.m_ofn.lpstrInitialDir = strPath;//
	if (fileDialog.DoModal() == IDOK)
	{
		CString path = fileDialog.GetPathName();
		CString str = m_sPath + _T(CONFIGURE_FILE) + _T("\\") + _T(CONFIGURE_NAME);
		WritePrivateProfileString(_T("ProjectPath"), _T("Path"), path, str);
	}
	ReadProject();
}


void CControlView::OnBnClickedBtnPreview()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_clContent.GetItemCount() <= 0)
	{
		AfxMessageBox(_T("请添加工程文件！"));
		return;
	}
	if (m_nClickedItem < 0)
	{
		AfxMessageBox(_T("您没有选中任何内容！"));
		return;         //防止弹出框不退出
	}

	PlayBegin();//运行准备

	KillTimer(IDT_RUN);
	m_nRunMode = MODE_CONTENTS;//运行模式
	m_nPlayIndex = 0;
	m_clContent.SetItemColor(m_nCurrentListIndex, RGB(255, 0, 0));
	m_clContent.Invalidate();
	SetTimer(IDT_RUN, 100, NULL);//定时100ms
}


void CControlView::OnBnClickedBtnRun()
{
	// TODO:  在此添加控件通知处理程序代码
}


void CControlView::OnBnClickedBtnModify()
{
	// TODO:  在此添加控件通知处理程序代码
	//创建非模态对话框
	/*CModifyDlg *modifyDlg = new CModifyDlg;
	modifyDlg->Create(IDD_MODIFY_DLG, NULL);
	modifyDlg->ShowWindow(SW_SHOW);*/

	//创建模态对话框
    CModifyDlg modifyDlg(this);
	if (modifyDlg.DoModal()==IDCANCEL)
	{
		ReadProject();
	}
}


void CControlView::OnBnClickedBtnStop()
{
	// TODO:  在此添加控件通知处理程序代码
}

void CControlView::OnBnClickedButtonOpenBin()
{
	// TODO:  在此添加控件通知处理程序代码
	BROWSEINFO   bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	TCHAR tchPath[255];

	bInfo.hwndOwner = m_hWnd;
	bInfo.lpszTitle = _T("所选择路径:   ");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	LPITEMIDLIST lpDlist;//用来保存返回信息的IpDList，使用SHGetPathFromIDList函数转换为字符串 
	lpDlist = SHBrowseForFolder(&bInfo);   //显示选择对话框 
	if (lpDlist != NULL)
	{
		SHGetPathFromIDList(lpDlist, tchPath);//把项目标识列表转化成目录 
		m_eBinPath.SetWindowText(tchPath);
		CString str = m_sPath + _T(CONFIGURE_FILE) + _T("\\") + _T(CONFIGURE_NAME);
		WritePrivateProfileString(_T("BinPath"), _T("Path"), tchPath, str);
	}
}

//生成255个
//void CControlView::OnBnClickedBtnMakeBin()
//{
//	// TODO:  在此添加控件通知处理程序代码
//	CString strPath;
//	CString currentType;
//	CString currentName;
//	int currentRunTm;
//	int currentPauseTm;
//
//	currentType = p_tPrjDir[m_nClickedItem]->sCurveType;
//	currentName = p_tPrjDir[m_nClickedItem]->sCurveName;//currentName = m_clContent.GetItemText(m_nClickedItem,1);
//	currentRunTm = p_tPrjDir[m_nClickedItem]->nCurveTime;
//	currentPauseTm = p_tPrjDir[m_nClickedItem]->nPauseTime;
//
//	strPath = m_sPath + _T(CURVE_DATA) + _T("\\") + currentType + _T("\\") + currentName + _T(".txt");
//	if (!m_pCurveData->ReadCurveData(strPath))//读取数据
//	{
//		return;
//	}
//	m_nFrameTotal = m_pCurveData->m_nFrameNum;//总共帧数
//	m_nRunTotal = currentRunTm;
//	m_dTime = (double)m_nRunTotal / (double)(m_nFrameTotal - 1);//每帧运行时间,第一帧为起始帧，即工作开始面
//	m_nPauseTotal = currentPauseTm;
//
//	//每帧805个数据，分成5份，每份161个数据
//	FILE *fp = NULL;
//	FILE *fd = NULL;
//	CString sPosPath;
//	CString sLightPath;
//	CString sPathName;
//	CString sFileName;
//	CString sPath;
//	CString sPosFile;
//	CString sLightFile;
//	CString sBinPosName;
//	CString sBinLightName;
//
//	m_eBinPath.GetWindowText(sPathName);
//	m_eBinName.GetWindowText(sFileName);
//	if (sFileName.IsEmpty())
//	{
//		AfxMessageBox(_T("文件名不能为空！"));
//		return;
//	}
//	sPath = sPathName + _T("\\") + sFileName;
//	sPosFile = sPath + _T("\\") + sFileName + _T("_Pos");
//	sLightFile = sPath + _T("\\") + sFileName + _T("_Lig");
//	//不存在则创建
//	if (!PathIsDirectory(sPath))
//	{
//		if (!CreateDirectory(sPath, 0))//创建不成功
//		{
//			MessageBox(_T("文件创建失败！"), _T("文件创建提示"), MB_OKCANCEL);
//			return;
//		}
//		else
//		{
//			CreateDirectory(sPosFile, 0);
//			CreateDirectory(sLightFile, 0);
//		}
//	}
//	else
//	{
//		if (MessageBox(_T("文件已存在，确定覆盖吗？"), _T("文件创建提示"), MB_OKCANCEL) == IDCANCEL)
//			return;
//	}
//
//	sBinPosName.Format(_T("%s%d%s"), sFileName, 255, _T("_Pos"));
//	sBinLightName.Format(_T("%s%d%s"), sFileName, 255, _T("_Lig"));
//	sPosPath = sPosFile + _T("\\") + sBinPosName + _T(".bin");
//	sLightPath = sLightFile + _T("\\") + sBinLightName + _T(".bin");
//	_wfopen_s(&fp, sPosPath, _T("wb"));
//	_wfopen_s(&fd, sLightPath, _T("wb"));
//	for (int i = 0; i < m_nFrameTotal; i++)//每帧5*161个数据，i为帧
//	{
//		int nNum = m_dTime / POS_TIME;//两帧之间插入nNum-1个位置
//		int nPuase = m_nPauseTotal / POS_TIME;//暂停插入nPuase+1个最后位置
//		for (int j = 0; j < 805; j++)//j为i帧里的第j个数据
//		{
//			if (i + 1 < m_nFrameTotal)//最后一帧之前
//			{
//				double dDistance = fabs((double)m_pCurveData->m_nCurveData[i + 1][j] - (double)m_pCurveData->m_nCurveData[i][j]);
//				double dPos = dDistance / nNum;//两帧之间插入nNum-1个位置的平均间距
//				for (int p = 0; p < nNum; p++)
//				{
//					if (m_pCurveData->m_nCurveData[i + 1][j]>m_pCurveData->m_nCurveData[i][j])//向下运动
//					{
//						m_dPos255[p][j] = m_pCurveData->m_nCurveData[i][j] + p * dPos;//每一列插入nNum个,从第一帧的位置开始
//					}
//					else//向上运动
//					{
//						m_dPos255[p][j] = m_pCurveData->m_nCurveData[i][j] - p * dPos;//每一列插入nNum个,从第一帧的位置开始
//					}
//				}
//			}
//			if (i + 1 == m_nFrameTotal)//最后一帧
//			{
//				for (int q = 0; q <= nPuase; q++)
//				{
//					m_dPos255[q][j] = m_pCurveData->m_nCurveData[i][j];
//				}
//			}
//		}
//		//第k份文件，第i帧与第i+1帧之间的数据生成后写入二进制文件
//		if (i + 1 < m_nFrameTotal)//最后一帧之前
//		{
//			for (int r = 0; r < nNum; r++)
//			{
//				double dPos[805];
//				memcpy(dPos, m_dPos255[r], sizeof(dPos));
//				ChangeWritePos(805, dPos);
//				for (int t = 0; t < 255; t++)
//				{
//					fwrite(&m_dWriteBall[t], sizeof(double), 1, fp);//fwrite(const void * buffer,size_t size,size_t count,FILE* stream);buffer内容的首地址指针，size每个元素的字节数，count要写入多少个元素，stream文件指针。
//					unsigned int unLight = unsigned int(m_dWriteBall[t] * (double)0xFFFFF / (double)BALL_MAX_POS);//生成灯光文件
//					fwrite(&unLight, sizeof(unsigned int), 1, fd);
//				}
//			}
//		}
//		//if (i + 1 == m_nFrameTotal)//最后一帧
//		//{
//		//	for (int r = 0; r <= nPuase; r++)
//		//	{
//		//		double dPos[805];
//		//		memcpy(dPos, m_dPos255[r], sizeof(dPos));
//		//		ChangeWritePos(805, dPos);
//		//		for (int t = 0; t < 255; t++)
//		//		{
//		//			fwrite(&m_dWriteBall[t], sizeof(double), 1, fp);//fwrite(const void * buffer,size_t size,size_t count,FILE* stream);buffer内容的首地址指针，size每个元素的字节数，count要写入多少个元素，stream文件指针。
//		//			unsigned int unLight = unsigned int(m_dWriteBall[t] * (double)0xFFFFF / (double)BALL_MAX_POS);//生成灯光文件
//		//			fwrite(&unLight, sizeof(unsigned int), 1, fd);
//		//		}
//		//	}
//		//}
//	}
//	fclose(fp);
//	fclose(fd);
//	AfxMessageBox(_T("生成bin文件成功！"));
//	return;
//}

 void CControlView::OnBnClickedBtnMakeBin()
 {
 	// TODO:  在此添加控件通知处理程序代码
 	CString strPath;
 	CString currentType;
 	CString currentName;
 	int currentRunTm;
 	int currentPauseTm;
 
 	currentType = p_tPrjDir[m_nClickedItem]->sCurveType;
 	currentName = p_tPrjDir[m_nClickedItem]->sCurveName;//currentName = m_clContent.GetItemText(m_nClickedItem,1);
 	currentRunTm = p_tPrjDir[m_nClickedItem]->nCurveTime;
 	currentPauseTm = p_tPrjDir[m_nClickedItem]->nPauseTime;
 
 	strPath = m_sPath + _T(CURVE_DATA) + _T("\\") + currentType + _T("\\") + currentName + _T(".txt");
 	if (!m_pCurveData->ReadCurveData(strPath))//读取数据
 	{
 		return ;
 	}
 	m_nFrameTotal = m_pCurveData->m_nFrameNum;//总共帧数
 	m_nRunTotal = currentRunTm;
 	m_dTime = (double)m_nRunTotal / (double)(m_nFrameTotal - 1);//每帧运行时间,第一帧为起始帧，即工作开始面
 	m_nPauseTotal = currentPauseTm;
 
 	//每帧805个数据，分成5份，每份161个数据
 	FILE *fp = NULL;
 	FILE *fd = NULL;
 	CString sPosPath;
 	CString sLightPath;
 	CString sPathName;
 	CString sFileName;
 	CString sPath;
 	CString sPosFile;
 	CString sLightFile;
 	CString sBinPosName;
 	CString sBinLightName;
 
 	m_eBinPath.GetWindowText(sPathName);
 	m_eBinName.GetWindowText(sFileName);
 	if (sFileName.IsEmpty())
 	{
 		AfxMessageBox(_T("文件名不能为空！"));
 		return;
 	}
 	sPath = sPathName + _T("\\") + sFileName;
 	sPosFile = sPath + _T("\\") + sFileName + _T("_Pos");
 	sLightFile = sPath + _T("\\") + sFileName + _T("_Lig");
 	//不存在则创建
 	if (!PathIsDirectory(sPath))
 	{
 		if (!CreateDirectory(sPath, 0))//创建不成功
 		{
 			MessageBox(_T("文件创建失败！"), _T("文件创建提示"), MB_OKCANCEL);
 			return;
 		}
 		else
 		{
 			CreateDirectory(sPosFile, 0);
 			CreateDirectory(sLightFile, 0);
 		}
 	}
 	else
 	{
 		if (MessageBox(_T("文件已存在，确定覆盖吗？"), _T("文件创建提示"), MB_OKCANCEL) == IDCANCEL)
 			return;
 	}
 	for (int k = 0; k < 5; k++)//分5个文件，第k个文件
 	{
 		sBinPosName.Format(_T("%s%d%s"), sFileName, k + 1, _T("_Pos"));
 		sBinLightName.Format(_T("%s%d%s"), sFileName, k + 1, _T("_Lig"));
 		sPosPath = sPosFile + _T("\\") + sBinPosName + _T(".bin");
 		sLightPath = sLightFile + _T("\\") + sBinLightName + _T(".bin");
 		_wfopen_s(&fp, sPosPath, _T("wb"));
 		_wfopen_s(&fd, sLightPath, _T("wb"));
 		for (int i = 0; i < m_nFrameTotal; i++)//每帧5*161个数据，i为帧
 		{
 			int nNum = m_dTime / POS_TIME;//两帧之间插入nNum-1个位置
 			int nPuase = m_nPauseTotal / POS_TIME;//暂停插入nPuase+1个最后位置
 			for (int j = k * 161; j < 161 * (k + 1); j++)//j为i帧里的第j个数据
 			{
 				if (i + 1 < m_nFrameTotal)//最后一帧之前
 				{
 					double dDistance = fabs((double)m_pCurveData->m_nCurveData[i + 1][j] - (double)m_pCurveData->m_nCurveData[i][j]);
 					double dPos = dDistance / nNum;//两帧之间插入nNum-1个位置的平均间距
 					for (int p = 0; p < nNum; p++)
 					{
 						if (m_pCurveData->m_nCurveData[i + 1][j]>m_pCurveData->m_nCurveData[i][j])//向下运动
 						{
 							m_dPos[p][j % 161] = m_pCurveData->m_nCurveData[i][j] + p * dPos;//每一列插入nNum个,从第一帧的位置开始
 						}
 						else//向上运动
 						{
 							m_dPos[p][j % 161] = m_pCurveData->m_nCurveData[i][j] - p * dPos;//每一列插入nNum个,从第一帧的位置开始
 						}
 					}
 				}
 				if (i + 1 == m_nFrameTotal)//最后一帧
 				{
 					for (int q = 0; q <= nPuase; q++)
 					{
 						m_dPos[q][j % 161] = m_pCurveData->m_nCurveData[i][j];
 					}
 				}
 			}
 			//第k份文件，第i帧与第i+1帧之间的数据生成后写入二进制文件
 			if (i + 1 < m_nFrameTotal)//最后一帧之前
 			{
 				for (int r = 0; r < nNum; r++)
 				{
 					double dPos[161] ;
 					memcpy(dPos, m_dPos[r], sizeof(dPos));
 					ChangeWritePos(161,dPos);
 					for (int t = 0; t < 161; t++)
 					{
 						fwrite(&m_dWritePos[t], sizeof(double), 1, fp);//fwrite(const void * buffer,size_t size,size_t count,FILE* stream);buffer内容的首地址指针，size每个元素的字节数，count要写入多少个元素，stream文件指针。
 						unsigned int unLight = unsigned int(m_dWritePos[t] * (double)0xFFFFF / (double)BALL_MAX_POS);//生成灯光文件
 						fwrite(&unLight, sizeof(unsigned int), 1, fd);
 					}
 				}
 			}
 			//if (i + 1 == m_nFrameTotal)//最后一帧
 			//{
 			//	for (int r = 0; r <= nPuase; r++)
 			//	{
 			//		double dPos[161];
 			//		memcpy(dPos, m_dPos[r], sizeof(dPos));
 			//		ChangeWritePos(161,dPos);
 			//		for (int t = 0; t < 161; t++)
 			//		{
 			//			fwrite(&m_dWritePos[t], sizeof(double), 1, fp);//fwrite(const void * buffer,size_t size,size_t count,FILE* stream);buffer内容的首地址指针，size每个元素的字节数，count要写入多少个元素，stream文件指针。
 			//			unsigned int unLight = unsigned int(m_dWritePos[t] * (double)0xFFFFF / (double)BALL_MAX_POS);//生成灯光文件
 			//			fwrite(&unLight, sizeof(unsigned int), 1, fd);
 			//		}
 			//	}
 			//}
 		}
 		fclose(fp);
 		fclose(fd);
 	}
 	AfxMessageBox(_T("生成bin文件成功！"));
 	return;
 }

void CControlView::ReadProject()
{
	CString strPath;
	CString strTemp;
	CString strBinPath;
	CString strName;
	strPath = m_sPath + _T(CONFIGURE_FILE) + _T("\\") + _T(CONFIGURE_NAME);
	GetPrivateProfileString(_T("ProjectPath"), _T("Path"), _T(""), strTemp.GetBuffer(100), 100, strPath);
	strTemp.ReleaseBuffer();
	/*int n = strTemp.ReverseFind(_T('\\'));
	int m = strTemp.ReverseFind(_T('.txt'));
	strName = strTemp.Mid(n + 1, m - n-1);*/
	GetPrivateProfileString(_T("ProjectPath"), _T("Name"), _T(""), strName.GetBuffer(100), 100, strPath);
	strName.ReleaseBuffer();
	GetDlgItem(IDC_PROJECT_NAME)->EnableWindow(FALSE);
	m_eProName = strName;
	UpdateData(FALSE);

	GetPrivateProfileString(_T("BinPath"), _T("Path"), _T(""), strBinPath.GetBuffer(100), 100, strPath);
	strBinPath.ReleaseBuffer();
	m_eBinPath.SetWindowText(strBinPath);

	m_clContent.DeleteAllItems();
	int index = 0;

	FILE* ff;
	CFileStatus status;
	errno_t err;
	if (CFile::GetStatus(strTemp, status))
	{
		err = _wfopen_s(&ff, strTemp, _T("rt"));
		CString str;
		CString rowNum;
		CString curverName;
		CString runTime;
		CString pauseTime;

		if (err != 0)
		{
			str.Format(_T("文件%s无法打开！"), strTemp);
			AfxMessageBox(str);
			return;
		}
		while (fscanf(ff, "%s %s %d %d %s %s", p_tPrjDir[index]->sCurveType, p_tPrjDir[index]->sCurveName, &p_tPrjDir[index]->nCurveTime, &p_tPrjDir[index]->nPauseTime, p_tPrjDir[index]->sCurveUndet2, p_tPrjDir[index]->sCurveUndet3)>0)
		{
			rowNum.Format(_T("%d"), index + 1);
			curverName = p_tPrjDir[index]->sCurveName;//char数组直接赋给CString
			runTime.Format(_T("%d"), p_tPrjDir[index]->nCurveTime);
			pauseTime.Format(_T("%d"), p_tPrjDir[index]->nPauseTime);

			m_clContent.InsertItem(index, rowNum);
			m_clContent.SetItemText(index, 1, curverName);
			m_clContent.SetItemText(index, 2, runTime + _T("s"));
			m_clContent.SetItemText(index, 3, pauseTime + _T("s"));

			index++;
		}
		m_nCurveNum = index;//工程文件信息数量
		fclose(ff);
	}
}

//把小球的位置变化为要求写入的顺序,5份161个顺序都如此
void CControlView::ChangeWritePos(int nNum,double dPos[])
{
	if (nNum == WRITE_NUM)
	{
		for (int i = 0; i < WRITE_NUM; i++)
		{
			if ((i >= 0 && i < ROW_NUM) || (i >= 2 * ROW_NUM && i < 3 * ROW_NUM) || (i >= 4 * ROW_NUM && i < 5 * ROW_NUM) || (i >= 6 * ROW_NUM && i < 7 * ROW_NUM))
			{
				m_dWritePos[i] = dPos[i];
			}
			if (i >= ROW_NUM && i < 2 * ROW_NUM)
			{
				m_dWritePos[3 * ROW_NUM - i - 1] = dPos[i];
			}
			if (i >= 3 * ROW_NUM && i < 4 * ROW_NUM)
			{
				m_dWritePos[7 * ROW_NUM - i - 1] = dPos[i];
			}
			if (i >= 5 * ROW_NUM && i < 6 * ROW_NUM)
			{
				m_dWritePos[11 * ROW_NUM - i - 1] = dPos[i];
			}
		}
	}
	else
	{
		for (int i = 0; i < BALLS_NUM; i++)
		{
			for (int k = 0; k < 5;k++)
			{
				if ((i >= k * 161 && i < ROW_NUM + k * 161) || (i >= 2 * ROW_NUM + k * 161 && i < 3 * ROW_NUM + k * 161) || (i >= 4 * ROW_NUM + k * 161 && i < 5 * ROW_NUM + k * 161) || (i >= 6 * ROW_NUM + k * 161 && i < 7 * ROW_NUM + k * 161))
				{
					m_dWriteBall[i] = dPos[i];
				}
				if ((i >= ROW_NUM + k * 161) &&( i < 2 * ROW_NUM + k * 161))
				{
					m_dWriteBall[3 * ROW_NUM + k * 161 - i %161- 1] = dPos[i];
				}
				if ((i >= 3 * ROW_NUM + k * 161) && (i < 4 * ROW_NUM + k * 161))
				{
					m_dWriteBall[7 * ROW_NUM + k * 161 - i % 161 - 1] = dPos[i];
				}
				if ((i >= 5 * ROW_NUM + k * 161) && (i < 6 * ROW_NUM + k * 161))
				{
					m_dWriteBall[11 * ROW_NUM + k * 161 - i % 161 - 1] = dPos[i];
				}
			}
		}
	}
}


void CControlView::PlayBegin()
{
	m_btNew.EnableWindow(false);
	m_btOpen.EnableWindow(false);
	m_btPreview.EnableWindow(false);
	m_btPreviewAll.EnableWindow(false);
	m_btRun.EnableWindow(false);
	m_btSend.EnableWindow(false);
	m_btModify.EnableWindow(false);
	m_btStop.EnableWindow(true);
}

void CControlView::PlayRun()
{
	KillTimer(IDT_RUN);

	switch (m_nPlayIndex)
	{
	case 0:
		m_nSpecialTime = m_pBallCurve->SetPosSpecialContents(MODE_START);
		m_nPlayIndex++;
	case 1:
		m_nStartTime = GetTickCount();
		m_nPlayIndex++;
	case 2:
		if ((GetTickCount() - m_nStartTime) / 1000.0 < (double)m_nSpecialTime)
		{
			SetPosColor();
			m_pBallArrayView->Invalidate();
			m_p3DView->InvalidateRect(NULL, false);
		}
		else
		{
			m_nPlayIndex++;
		}
		break;
	case 3:
		if (!SetBallValue())
			m_nPlayIndex = 20;
		else
			m_nPlayIndex++;
		break;
	case 4:
		m_nPlayIndex++;
		m_nStartTime = GetTickCount();
		break;
	case 5:
		if ((GetTickCount() - m_nStartTime) / 1000.0 < m_dTime)
		{
			SetPosColor();
			m_pBallArrayView->Invalidate();
			m_p3DView->InvalidateRect(NULL, false);
		}
		else
		{
			m_nPlayIndex++;
		}
		break;
	case 6:
		m_nFrameIndex++;
		if (m_nFrameIndex < m_nFrameTotal)
		{
			m_nPlayIndex = 3;
		}
		break;
	}

	SetTimer(IDT_RUN, 100, NULL);
}

bool CControlView::SetBallValue()
{
	CString strPath;
	CString currentType;
	CString currentName;
	int currentRunTm;
	int currentPauseTm;

	currentType = p_tPrjDir[m_nClickedItem]->sCurveType;
	currentName = p_tPrjDir[m_nClickedItem]->sCurveName;//currentName = m_clContent.GetItemText(m_nClickedItem,1);
	currentRunTm = p_tPrjDir[m_nClickedItem]->nCurveTime;
	currentPauseTm = p_tPrjDir[m_nClickedItem]->nPauseTime;

	strPath = m_sPath + _T(CURVE_DATA) + _T("\\") + currentType + _T("\\") + currentName + _T(".txt");
	if (!m_pCurveData->ReadCurveData(strPath))//读取数据
	{
		return false;
	}
	m_nFrameTotal = m_pCurveData->m_nFrameNum;//总共帧数
	m_nRunTotal = currentRunTm;
	m_dTime = (double)m_nRunTotal / (double)m_nFrameTotal;//每帧运行时间
	m_nPauseTotal = currentPauseTm;

	m_dRunFrameTm = m_pBallCurve->SetPosSpeedContents(m_pCurveData->m_nCurveData[m_nFrameIndex], m_dTime);//第m_nFrameIndex帧的数据及运行时间
	return true;
}

void CControlView::SetPosColor()
{
	m_pBallCurve->SetColorContents(m_nStartTime);
}


void CControlView::OnBnClickedBtnMotor()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int nxy = (m_nCol - 1) * ROW_NUM+ m_nRow -1;
	int nheight = m_pBallCurve->GetAbsolutePosition(m_nHeight);
	if (m_nRow==0&&m_nCol!=0)//整列运行
	{
		nxy = (m_nCol - 1) * ROW_NUM;
		for (int i = 0; i < ROW_NUM; i++)
		{
			m_pBallCurve->SetCurrentBall(nxy+i, nheight);
			m_dBallPos[nxy + i] = m_nHeight;
		}
	}
	else if (m_nRow != 0 && m_nCol == 0)//整行运行
	{
		nxy = m_nRow - 1;
		for (int i = 0; i < COL_NUM; i++)
		{
			m_pBallCurve->SetCurrentBall(nxy + i*ROW_NUM, nheight);
			m_dBallPos[nxy + i * ROW_NUM] = m_nHeight;
		}
	}
	else if (m_nRow == 0 && m_nCol == 0)//整体运行
	{
		for (int i = 0; i < BALLS_NUM; i++)
		{
			m_pBallCurve->SetCurrentBall( i, nheight);
			m_dBallPos[i] = m_nHeight;
		}
	}
	else//单个运行
	{
		m_pBallCurve->SetCurrentBall(nxy, nheight);
		m_dBallPos[nxy] = m_nHeight;
	}
	ChangeWritePos(805, m_dBallPos);
	m_pBallArrayView->Invalidate();
	m_p3DView->InvalidateRect(NULL, false);
}


void CControlView::OnBnClickedBtnLimit()
{
	// TODO:  在此添加控件通知处理程序代码
	int nheight = m_pBallCurve->GetAbsolutePosition(MODE_LIMIT);
	for (int i = 0; i < BALLS_NUM; i++)
	{
		m_pBallCurve->SetCurrentBall(i, nheight);
		m_dBallPos[i] = 0;
	}
	m_pBallArrayView->Invalidate();
	m_p3DView->InvalidateRect(NULL, false);
}


void CControlView::OnBnClickedBtnZero()
{
	// TODO:  在此添加控件通知处理程序代码
	int nheight = m_pBallCurve->GetAbsolutePosition(MODE_ZERO);
	for (int i = 0; i < BALLS_NUM; i++)
	{
		m_pBallCurve->SetCurrentBall(i, nheight);
		m_dBallPos[i] = 0;
	}
	m_pBallArrayView->Invalidate();
	m_p3DView->InvalidateRect(NULL, false);
}


void CControlView::OnBnClickedBtnCeil()
{
	// TODO:  在此添加控件通知处理程序代码
	int nheight = m_pBallCurve->GetAbsolutePosition(MODE_CEIL);
	for (int i = 0; i < BALLS_NUM; i++)
	{
		m_pBallCurve->SetCurrentBall(i, nheight);
		m_dBallPos[i] = 0;
	}
	m_pBallArrayView->Invalidate();
	m_p3DView->InvalidateRect(NULL, false);
}


void CControlView::OnBnClickedBtnStart()
{
	// TODO:  在此添加控件通知处理程序代码
	int nheight = m_pBallCurve->GetAbsolutePosition(MODE_START);
	for (int i = 0; i < BALLS_NUM; i++)
	{
		m_pBallCurve->SetCurrentBall(i, nheight);
		m_dBallPos[i] = 0;
	}
	m_pBallArrayView->Invalidate();
	m_p3DView->InvalidateRect(NULL, false);
}


void CControlView::OnBnClickedBtnRotation()
{
	// TODO:  在此添加控件通知处理程序代码
	CString sBtn;
	m_btRotation.GetWindowText(sBtn);
	if (sBtn == _T("启动旋转"))
	{
		m_btRotation.SetWindowText(_T("停止旋转"));
		m_pBallCurve->SetAutoRotation(true);
	}
	else
	{
		m_btRotation.SetWindowText(_T("启动旋转"));
		m_pBallCurve->SetAutoRotation(false);
	}
}


void CControlView::OnBnClickedBtnEnd()
{
	// TODO:  在此添加控件通知处理程序代码
	bool b = false;
	short n = 1;
	short nFile[50] = { 0 };
	
	m_pADS->ReadWriteFtpReq(&b,&n,nFile);
}


void CControlView::OnBnClickedBtnServe()
{
	// TODO:  在此添加控件通知处理程序代码
	//创建非模态对话框
	UpdateData(TRUE);
	CTCPServerDlg *pServerDlg = new CTCPServerDlg(this);
	pServerDlg->Create(IDD_TCP_SERVER_DLG, NULL);
	pServerDlg->ShowWindow(SW_SHOW);

	
	//创建模态对话框
	/*CTCPServerDlg ServeDlg(this);
	ServeDlg.DoModal();*/
}
