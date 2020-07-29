#pragma once

#include "GridCtrl/GridCtrlEx.h"
#include "res/resource.h"
#include "afxwin.h"
#include "afxwin.h"
#include "ThreadData.h"
#include "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\atlmfc\include\afxwin.h"
typedef vector<CString>	OneRowData;
typedef vector<OneRowData> MultiRowData;
// CZTInfoDlg �Ի���

class CZTInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CZTInfoDlg)

public:
	CZTInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CZTInfoDlg();
	virtual BOOL OnInitDialog();


private:
	void InitGridCtrl();
	void SetCellHight();
	void InitDefaultPara();
	void LoadGridData();
	MultiRowData getDefaultGridData(int index);
	MultiRowData getTableData();
	void loadDaoBing();
// �Ի�������
#if ARX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CWDR };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedBtnok();
	afx_msg void OnCbnSelchangeComboStepnum();
	
public:
	CGridCtrlEx m_djInfoCtrl;
	double m_TotalLength;
	double m_VertAngle;
	CComboBox m_StepNum;
	CComboBox m_ui_DrNumCtrl;
	MultiRowData m_alldjInfos;
	CEdit m_TotalLenEdit;
	CEdit m_VertexEdit;

	CThreadSegData m_segdata;
	CThreadData m_data;
	CString m_daobing;
	CComboBox m_DaoBingCtrl;

};