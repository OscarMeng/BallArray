#pragma once
//#include "ColorList.h"
#include "ControlView.h"
#include "MyConstMeth.h"

// CModifyDlg 对话框
class CModifyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyDlg)

public:
	CModifyDlg( CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModifyDlg();

// 对话框数据
	enum { IDD = IDD_MODIFY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	CColorList	         m_clModify;
	CControlView*     m_pCtrlView;

	CButton		m_btAdd;
	CButton		m_btUp;
	CButton		m_btDown;
	CButton		m_btSave;
	CButton		m_btDelete;
public:
	int		m_nItem;
	int        m_nNum;
	int        m_nFlag;//添加/修改标识符
	CString m_sType;
	CString m_sName;
	int        m_nRunTm;
	int        m_nPauseTm;
	int        m_nRowNm;  //插入的行数
	ProjectDir*  p_tPrjDir[CURVE_NUM];

public:
	void Swap(ProjectDir* lp, ProjectDir* rp);//交换数据

public:
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnDown();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnNMClickListItem(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnBnClickedMbtResive();
};
