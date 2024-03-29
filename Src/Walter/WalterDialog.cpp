// WalterDialog.cpp : 实现文件
//

#include "stdafx.h"

#include "WalterDialog.h"
#include "afxdialogex.h"
#include "OpenExcelDialog.h"
#include "CutterTool.h"
#include "Com.h"
#include "../Inc/Entity/BlockUtil.h"
#include "../Inc/Interaction/GetInputUtil.h"
#include "Others/ToolingUtil.h"
#include "Others/XDataUtil.h"
#include "Entity/ObjectUtil.h"
#include "Document/DocLock.h"
//#include "TYProgress.h"

WalterDialog* g_Walter = NULL;
CString g_filePath;
CString g_frame;
CString g_draw;
CString g_designer;
CString g_desDate;
CString g_checker;
CString g_checkDate;

// WalterDialog 对话框

IMPLEMENT_DYNAMIC(WalterDialog, CDialogEx)

WalterDialog::WalterDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(WalterDialog::IDD, pParent)
	, FilePath(_T(""))
	, DrawNumber(_T(""))
	, Designer(_T(""))
	, DesDate(_T(""))
	, AuditStaff(_T(""))
	, AudDate(_T(""))
	, DrawFrame(_T(""))
	, flag(0)
{

}

WalterDialog::~WalterDialog()
{
}

void WalterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FilePath, FilePath);
	DDX_Text(pDX, IDC_EDIT2, DrawNumber);
	DDX_Text(pDX, IDC_EDIT3, Designer);
	DDX_Text(pDX, IDC_EDIT4, DesDate);
	DDX_Text(pDX, IDC_EDIT5, AuditStaff);
	DDX_Text(pDX, IDC_EDIT6, AudDate);
	DDX_CBString(pDX, IDC_COMBO1, DrawFrame);
}
LRESULT WalterDialog::OnAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

BEGIN_MESSAGE_MAP(WalterDialog, CDialogEx)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, &WalterDialog::OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BROWSE, &WalterDialog::OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_DRAW, &WalterDialog::OnBnClickedDraw)
	ON_CBN_SELCHANGE(IDC_COMBO1, &WalterDialog::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_UpdateDesDateAndName, &WalterDialog::OnBnClickedUpdatedesdateandname)
	ON_BN_CLICKED(IDC_UpdateCheckDateAndName, &WalterDialog::OnBnClickedUpdatecheckdateandname)
	ON_BN_CLICKED(IDC_UpdateBomExcel, &WalterDialog::OnBnClickedUpdatebomexcel)
END_MESSAGE_MAP()


// WalterDialog 消息处理程序


//读取文件数据
void WalterDialog::OnBnClickedBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	vCString files= COpenExcelDialog::TYCOM_GetFilesFromDialog();
	//CString fileName=CCommonUtil::FilePathToFileName(files[0]);
	if(files.size()==0)
	{
	    AfxMessageBox(L"没有选择文件或者选择文件无效");
	    return;
	}
	SetDlgItemText(IDC_FilePath,files[0]);
}

//判断所由输入框是否为空
int WalterDialog::InputCheck()
{
	UpdateData();

	if(DrawNumber.IsEmpty())
	{
		MessageBox(L"图纸号不能为空,请输入！");
		return 1;
	}
	else if(Designer.IsEmpty())
	{
		MessageBox(L"设计人员不能为空，请输入！");
		return 1;
	}
	else if(DesDate.IsEmpty())
	{
		MessageBox(L"设计日期不能为空，请输入！");
		return 1;
	}
	else if(AuditStaff.IsEmpty())
	{
		MessageBox(L"审核人员不能为空，请输入！");
		return 1;
	}
	else if(AudDate.IsEmpty())
	{
		MessageBox(L"审核日期不能为空，请输入！");
		return 1;
	}
	else if(DrawFrame.IsEmpty())
	{
		MessageBox(L"图框不能为空，请输入！");
		return 1;
	}
	UpdateData(FALSE);
	return 0;
}

//判断设计人员以及日期是否为空
int WalterDialog::InputCheckDesDateOrName()
{
	UpdateData();
    if(Designer.IsEmpty())
	 {
		MessageBox(L"设计人员不能为空，请输入！");
		return 1;
	 }
	else if(DesDate.IsEmpty())
	 {
		MessageBox(L"设计日期不能为空，请输入！");
		return 1;
	 }
	else if (AuditStaff.IsEmpty())
	{
		MessageBox(L"审核日期不能为空，请输入！");
		return 1;
	}
	else if (AudDate.IsEmpty())
	{
		MessageBox(L"审核日期不能为空，请输入！");
		return 1;
	}
	UpdateData(FALSE);
	return 0;
}

//判断审核人员以及日期输入框是否为空
int WalterDialog::InputCheckAtuDateOrName()
{
	UpdateData();
	if(AuditStaff.IsEmpty())
	{
		MessageBox(L"审核人员不能为空，请输入！");
		return 1;
	}
	else if(AudDate.IsEmpty())
	{
		MessageBox(L"审核日期不能为空，请输入！");
		return 1;
	}
	UpdateData(FALSE);
	return 0;
}

//设置块默认属性
int WalterDialog::SetBlockAttribute(AcDbObjectId blkId)
{
    UpdateData();
	AcDbEntityPointer pEnt(blkId, AcDb::kForWrite);
	if (pEnt.openStatus() == Acad::eOk)
	{
		AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
		if (pBlkRef == NULL)
		{
			if(pEnt != NULL)
				pEnt->close();
			return -1;
		}

		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_VERSION_1", L"00");//第一个参数是块参照，第二个是属性标记，第三个是属性值
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_NUMMER_1", L"REV00");
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_DATUM_1", DesDate);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_NAME_1", Designer);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"DWG_ERST_DATUM", DesDate);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"DWG_ERST_INIT",Designer);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"AT17",AudDate);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"AT18",AuditStaff);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"!ALT_WZG_BEZ4",L"machining plan");
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"MAT_BEZ",DrawNumber);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"DOKTYPE",L"D10");
		pBlkRef->close();
	}
	
	UpdateData(FALSE);
	return 0;

}

//修改设计人员名称以及日期属性
int WalterDialog::SetDesBlockAttribute(AcDbObjectId blkId)
{
	UpdateData();
	AcDbEntityPointer pEnt(blkId, AcDb::kForWrite);
	if (pEnt.openStatus() == Acad::eOk)
	{
		AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
		if (pBlkRef == NULL)
		{
			return -1;
		}
		CBlockUtil::GetAttributeValue(pBlkRef,L"L_AE_VERSION_1",flagNum);

		if(flagNum.Left(1)==L"0")
			flagNum =flagNum.Right(1);

		//flag =_ttoi(flagNum);
		//flag++;
		//CString version;
		CString nunner;
		//version.Format(L"0%2d",flag);
		//nunner.Format(L"REV0%d",flag);
		//CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_VERSION_1",version);//第一个参数是块参照，第二个是属性标记，第三个是属性值
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_NUMMER_1",nunner);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_DATUM_1", DesDate);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_NAME_1", Designer);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"DWG_ERST_DATUM", DesDate);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"DWG_ERST_INIT",Designer);
		pBlkRef->close();
	}

	UpdateData(FALSE);
	return 0;

}

//修改审核人员名称以及日期属性
int WalterDialog::SetAutBlockAttribute(AcDbObjectId blkId)
{
	UpdateData();
	AcDbEntityPointer pEnt(blkId, AcDb::kForWrite);
	if (pEnt.openStatus() == Acad::eOk)
	{
		AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
		if (pBlkRef == NULL)
		{
			return -1;
		}
		CBlockUtil::GetAttributeValue(pBlkRef,L"L_AE_VERSION_1",flagNum);

		if(flagNum.Left(1)==L"0")
			flagNum =flagNum.Right(1);

		flag =_ttoi(flagNum);
		flag++;
		CString version;
		CString nunner;
		version.Format(L"0%2d",flag);
		nunner.Format(L"REV0%d",flag);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_VERSION_1",version);//第一个参数是块参照，第二个是属性标记，第三个是属性值
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_NUMMER_1", nunner);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"AT17",AudDate);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"AT18",AuditStaff);
		pBlkRef->close();
	}

	UpdateData(FALSE);
	return 0;

}


//插入图框与图纸
int WalterDialog::InsertDwgsAccordingToCutterTools(CString tuKuangFileName, TUKuangType tktype, std::vector<CCutterTool> &cutterTools)
{
	CDocLock lock;

	const double A1_wide=880;
	const double A2_wide=630;
	const double A3_wide=450;
	const double A1_height=650;
	const double A2_height=470;
	const double A3_height=350;
	
	TY_Progress_Init();
	for (int i = 0; i < cutterTools.size(); i++)
	{
		//图框插入点
		AcGePoint3d pnt(0, 0, 0);
		int Px = i / 20;
		int Py = i % 20;
		TY_SetProgress((int)cutterTools.size(), i+1);
		if (tktype == A1)
		{
			//图框的插入坐标值
			AcGePoint3d pnt1(pnt.x + Py*A1_wide, pnt.y - Px*A1_height, pnt.z);
			//插入图框
			bool isExistTK = CCommonUtil::IsFileExist(tuKuangFileName);
			if (!isExistTK)
			{
				acutPrintf(L"File Name:" + tuKuangFileName + " not exist\n");
			}
			else
			{
				cutterTools[i].m_tkId = CBlockUtil::InsertDwgAsBlockRef(tuKuangFileName, L"A1", ACDB_MODEL_SPACE, pnt1, 0, 1);
			}

			for (int j = 0; j < cutterTools[i].toType.size(); j++)
			{
				//获取图纸
				int zPx = j / 2;
					int zPy=j%2;
				CString tzFilename = TY_GetDwgFolder() + cutterTools[i].toType[j] + ".dwg";
				bool isExist = CCommonUtil::IsFileExist(tzFilename);
				if (!isExist)
				{
					acutPrintf(L"File Name:" + tzFilename + "not exist\n");
					continue;
				}
				//刀具图纸插入坐标值
					AcGePoint3d pnt2(pnt1.x+((A1_wide)/3)+zPy*350,pnt1.y+(A1_height*2/3.0)-zPx*200,pnt.z);
				
				//插入刀具图纸
				AcDbObjectId tzId = CBlockUtil::InsertDwgAsBlockRef(tzFilename, NULL, ACDB_MODEL_SPACE, pnt2, 0, 1);
				AcGeVector3d vect = CBlockUtil::GetBlkVectorFromMiddlePointToOri(tzId);
				CBlockUtil::SetBlockReferencePosition(tzId, AcGePoint3d(pnt2.x+vect.x, pnt2.y + vect.y, pnt2.z + vect.z));
			}
		}
		else if(tktype==A2)
		{
			AcGePoint3d pnt1(pnt.x+Py*A2_wide,pnt.y-Px*A2_height,pnt.z);
			//插入图框
			bool isExistTK = CCommonUtil::IsFileExist(tuKuangFileName);
			if (!isExistTK)
			{
				acutPrintf(L"File Name:" + tuKuangFileName + "not exist\n");
			}
			else
			    cutterTools[i].m_tkId = CBlockUtil::InsertDwgAsBlockRef(tuKuangFileName, L"A2", ACDB_MODEL_SPACE, pnt1, 0, 1);

			
			for (int j = 0; j < cutterTools[i].toType.size(); j++)
			{
				//获取图纸
				int zPx=j/2;
				int zPy=j%2;
				CString tzFilename= TY_GetDwgFolder() +cutterTools[i].toType[j]+".dwg";
				bool isExist = CCommonUtil::IsFileExist(tzFilename);
				if (!isExist)
				{
					acutPrintf(L"File Name:"+tzFilename+"not exist\n");
					continue;
				}

				//刀具图纸插入坐标值
				AcGePoint3d pnt2(pnt1.x+((A2_wide)/3)+zPy*200,pnt1.y+(A2_height*2/3.0)-zPx*150,pnt.z);
				//插入刀具图纸
				AcDbObjectId tzId = CBlockUtil::InsertDwgAsBlockRef(tzFilename, NULL, ACDB_MODEL_SPACE, pnt2, 0, 1);

				AcGeVector3d vect = CBlockUtil::GetBlkVectorFromMiddlePointToOri(tzId);
				CBlockUtil::SetBlockReferencePosition(tzId, AcGePoint3d(pnt2.x + vect.x, pnt2.y + vect.y, pnt2.z + vect.z));
			}
		}
		else
		{
			AcGePoint3d pnt1(pnt.x+Py*A3_wide,pnt.y-Px*A3_height,pnt.z);
			//插入图框
			bool isExistTK = CCommonUtil::IsFileExist(tuKuangFileName);
			if (!isExistTK)
			{
				acutPrintf(L"File Name:" + tuKuangFileName + "not exist\n");
			}
			else
			cutterTools[i].m_tkId = CBlockUtil::InsertDwgAsBlockRef(tuKuangFileName, L"A3", ACDB_MODEL_SPACE, pnt1, 0, 1);

			for (int j = 0; j < cutterTools[i].toType.size(); j++)
			{
				//获取图纸
				int zPx=j/2;
				int zPy=j%2;
				CString tzFilename= TY_GetDwgFolder() + cutterTools[i].toType[j]+".dwg";
				bool isExist = CCommonUtil::IsFileExist(tzFilename);
				if (!isExist)
				{
					acutPrintf(L"File Name:"+tzFilename+"not exist\n");
					continue;
				}

				//刀具图纸插入坐标值
				AcGePoint3d pnt2(pnt1.x+((A3_wide)/3)+zPy*100,pnt1.y+(A3_height*2/3.0)-zPx*100,pnt.z);
				//插入刀具图纸
				AcDbObjectId tzId = CBlockUtil::InsertDwgAsBlockRef(tzFilename, NULL, ACDB_MODEL_SPACE, pnt2, 0, 1);

				AcGeVector3d vect = CBlockUtil::GetBlkVectorFromMiddlePointToOri(tzId);
				CBlockUtil::SetBlockReferencePosition(tzId, AcGePoint3d(pnt2.x + vect.x, pnt2.y + vect.y, pnt2.z + vect.z));
			}
		}
	}
	TY_Progress_Close();
	return 0;
}

//生成图纸
void WalterDialog::OnBnClickedDraw()
{
	CDocLock lock;
	UpdateData();
	g_filePath = FilePath;
	g_frame = DrawFrame;
	g_draw = DrawNumber;
	g_designer = Designer;
	g_desDate = DesDate;
	g_checker = AuditStaff;
	g_checkDate = AudDate;
	//第一步：参数检测
	if(InputCheck() != 0)
	{
		return;
	}
	
	//先关闭对话框
	WalterDialog::OnOK();
	
    //第二步：读取excel数据
	std::vector<CCutterTool> cutterTools;

	//CAcModuleResourceOverride resOverride;
	//TYProgress progress;
	//progress.SetInfo(100,30);
	TY_ReadCutterToolsFromExcel(FilePath, cutterTools);
	//progress.SetInfo(100, 100);
	//第三步：根据读取的excel数据插入对应的图框
	//获取图框文件名
	//获取CAD安装那个路径
	
	//获取图纸框
	CString tkFilename= TY_GetFrameFolder() +DrawFrame+".dwg";

	TUKuangType type;
	if (DrawFrame == L"A1")
	{
		type = A1;
	}
	else if(DrawFrame == L"A2")
	{
		type=A2;
	}
	else
	{
		type=A3;
	}
	//第四步插入图框以及图纸
	InsertDwgsAccordingToCutterTools(tkFilename, type, cutterTools);

	
	//4.1 写入图框属性
	//第四步：绘制表格
	for (int i = 0; i < cutterTools.size(); i++)
	{
		
		cutterTools[i].m_tkType = type;
		AcDbObjectId tableId = cutterTools[i].CreateTable();
		CXDataUtil::AddEntityInfo(tableId, L"BOM_TABLE", cutterTools[i].toolNumber);
		//解决识别图框中表格中的图纸号
		CXDataUtil::AddEntityInfo(cutterTools[i].m_tkId, TUKANGE_NAME_IDENTITY, cutterTools[i].toolNumber);
		//输入实体ID得到指针
		int result=SetBlockAttribute(cutterTools[i].m_tkId);

		if(result==-1)
		{
			acutPrintf(L"设置块属性值失败！");
			continue;
		}	
	}
	UpdateData(FALSE);
}


void WalterDialog::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码

}


//初始化下拉框
BOOL WalterDialog::OnInitDialog()
{
	int initNum=0;
	CDialogEx::OnInitDialog();
	if(g_frame==L""||g_frame==L"A1")
	{
		((CComboBox *)GetDlgItem(IDC_COMBO1))->SetCurSel(0);
	}
	else if(g_frame==L"A2")
	{
		((CComboBox *)GetDlgItem(IDC_COMBO1))->SetCurSel(1);
	}
	else
	{
		((CComboBox *)GetDlgItem(IDC_COMBO1))->SetCurSel(2);
	}
	UpdateData(TRUE);
	//初始化下拉框
	FilePath = g_filePath;
	DrawNumber = g_draw;
	Designer = g_designer;
	DesDate = g_desDate;
	AuditStaff = g_checker;
	AudDate = g_checkDate;
	//下拉框对应的变量.SetCurSel(0);
	UpdateData(FALSE);
	return true;
}

//获取刀具图框的ID
vAcDbObjectId WalterDialog::GetToolsObjectId()
{
	CDocLock lock;
	vAcDbObjectId blockIds;
	vAcDbObjectId vids;
	CString blockName;
	int num = CBlockUtil::CycleAllBlockReferences(vids);
	if(num>0)
	{
		for(int i=0;i<num;i++)
		{
			//获取所有Id对应的实体名称
			CBlockUtil::GetBlockNameByBlockRef(vids[i],blockName);
			//与图框名称比较，保留图框Id
			if(blockName=="A1"||blockName=="A2"||blockName=="A3")
			{
				blockIds.push_back(vids[i]);

			}
		}
	}
	else
	{
		acutPrintf(L"未找到对应的图块！");
	}
	return blockIds;

}
 
//更新设计人员以及日期
void WalterDialog::OnBnClickedUpdatedesdateandname()
{
	//WalterDialog::OnOK();
	//第一步：检查输入是否为空
	if (InputCheckDesDateOrName() != 0)
	{
		return;
	}

	UpdateData();
	g_designer = Designer;
	g_desDate = DesDate;
	g_checker = AuditStaff;
	g_checkDate = AudDate;

	UpdateData(FALSE);
	CGetInputUtil::SendCommandToCad(L"gx ");
}

//更新审核人员以及日期
void WalterDialog::OnBnClickedUpdatecheckdateandname()
{
	UpdateData(TRUE);
	g_checker = AuditStaff;
	g_checkDate = AudDate;
	UpdateData(FALSE);
	//第一步：检测输入是否为空
	CGetInputUtil::SendCommandToCad(L"setCheck ");
}

//更新设计和审核日期
void WalterDialog::UpdateDesignerAttr()
{
	//第一步：获取所有图块
	 vAcDbObjectId blockId = GetToolsObjectId();

	for (int i=0;i<blockId.size();i++)
	{
		int result=SetDesBlockAttribute(blockId[i]);

		if(result==-1)
		{
			acutPrintf(L"设置块属性值失败！");
			continue;
		}	

		result = SetAutBlockAttribute(blockId[i]);

		if (result == -1)
		{
			acutPrintf(L"设置块属性值失败！");
			continue;
		}
	}
}

void WalterDialog::UpdateAtuAttr()
{
	if(InputCheckAtuDateOrName()!=0)
	{
		return;
	}
	//第二步：获取图块ID
	vAcDbObjectId blockId = GetToolsObjectId();

	for (int i=0;i<blockId.size();i++)
	{
		int result=SetAutBlockAttribute(blockId[i]);

		if(result==-1)
		{
			acutPrintf(L"设置块属性值失败！");
			continue;
		}	
	}
}

//更新Bom表格
void WalterDialog::OnBnClickedUpdatebomexcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CGetInputUtil::SendCommandToCad(L"setBom ");
}

void WalterDialog::UpdateBomExcel()
{
	CDocLock lock;
	UpdateData();
	//第一步：找到所有的table
	vAcDbObjectId tableIds;
	CToolingUtil::CycleAllTypedObjectsInAllLayer(CToolingUtil::ACDB_TABLE,tableIds);

	//第二步：删除所有的table
	for (int i = 0; i < tableIds.size(); i++)
	{
		CString value;
		if(CXDataUtil::ReadEntityInfo(tableIds[i], L"BOM_TABLE", value) == 0)
		    CObjectUtil::DeleteObject(tableIds[i]);
	}
	
	//第三步 给每个图框建立新表
	CString blkName;
	vAcDbObjectId blockIds = GetToolsObjectId(); 
	if(blockIds.empty())
	{
		return;
	}
	CBlockUtil::GetBlockNameByBlockRef(blockIds[0],blkName);
	//3.1 读取excel
	std::vector<CCutterTool> cutterTools;
	TY_ReadCutterToolsFromExcel(FilePath, cutterTools);
	//3.2 重新插入表即可
	TUKuangType type;
	if (blkName == L"A1")
	{
		type = A1;
	}
	else if(blkName == L"A2")
	{
		type=A2;
	}
	else
	{
		type=A3;
	}
	for (int i = 0; i < cutterTools.size(); i++)
	{
		cutterTools[i].m_tkId=blockIds[i];
		cutterTools[i].m_tkType = type;
	    AcDbObjectId tableId = cutterTools[i].CreateTable();
		CXDataUtil::AddEntityInfo(tableId, L"BOM_TABLE", cutterTools[i].toolNumber);
	}
	UpdateData(FALSE);
}