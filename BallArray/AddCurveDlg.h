#pragma once
#include "ModifyDlg.h"

// CAddCurverDlg 对话框

class CAddCurveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddCurveDlg)

public:
	CAddCurveDlg(int nNum, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddCurveDlg();

// 对话框数据
	enum { IDD = IDD_ADD_CURVER_DLG };
public:
	CModifyDlg*     m_pModDlg;
	CEdit		    m_ePath;
	CEdit		    m_eType;
	CEdit		    m_eName;
	CEdit		    m_eRunTime;
	CEdit		    m_ePauseTime;
	CEdit		    m_eRowNum;
	CEdit		    m_eRowTip;
	CString         m_sContent;
	CString         m_sPath;
	int             m_nNum;
	int             m_nMaxRowNum;//可插入的最大行数
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedOk();
};
