#pragma once
#include "afxwin.h"
#include "GridCtrl/GridCtrlEx.h"
#include "Com.h"
#include "ZYDJData.h"

// CDlgZyJd 对话框

class CDlgZyJd : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgZyJd)

public:
	CDlgZyJd(CWnd* pParent = NULL);   // 标准构造函数
	virtual BOOL OnInitDialog();
	
	virtual ~CDlgZyJd();


//初始化参数
private:
	void InitDefaultPara();
	void InitGrid();
	void SetCellHight();
	MultiRowText GetDefaultGridData(int rowCount);
	void SetGridData();
	MultiRowText GetGridData();
	void ReloadGridData();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PCD_XD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ui_DbCtrl;
	CComboBox m_ui_LabberCtrl;

	MultiRowText m_allrowData; //与gridctrl对应的数据
	CGridCtrlEx m_XdLabberDataCtrl;
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedButton1();
	// 扩孔刀预孔直径
	double m_diameter;

	//数据填充
	CZYDJData m_ZyDjData;
	CComboBox m_SelKnifeClass;
	afx_msg void OnCbnSelchangeCombo3();
	double m_totalLength;
};
