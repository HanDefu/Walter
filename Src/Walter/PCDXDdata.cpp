#include "stdafx.h"
#include "PCDXDdata.h"
#include "Entity/DimensionUtil.h"
#include "Entity/DynamicBlockUtil.h"
int PCDXD::Draw()
{
	CDocLock lock;


	offset = 0.75;
	//插入刀柄
	AcGePoint3d pnt;
	CGetInputUtil::GetPoint(L"请输入插入点：", pnt);
	CString Handle = TY_GetDaoBingFolder() + m_handleName + L".dwg";
	CBlockUtil::InsertDwgAsBlockRef(Handle, NULL, ACDB_MODEL_SPACE, pnt, 0, 1);
	//插入刀身
	AcGePoint3d ptInsert(std::move(pnt));
	double handleLength = GetDisByDBName(m_handleName);
	ptInsert.x = ptInsert.x + m_totalLength - offset;
	CString DaoShen = TY_GetDynamicBlockFolder() + L"PCD铣刀模板.dwg";
	AcDbObjectId DaoShenId = CBlockUtil::InsertDwgAsBlockRef(DaoShen, NULL, ACDB_MODEL_SPACE, ptInsert, 0, 1);


	//动态块参数设置
	CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"L", m_totalLength - handleLength );

	CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"Dc", m_diameter);
	CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"Lc", m_bladeLength);

	double radius = GetRadiusByDiameter(m_diameter);
	double dr_dia = GetHeightByDiameter(m_diameter);
	CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"R", radius);
	CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"DRen", dr_dia);
	
	//切换到标注层
	CLayerSwitch layer(DIMLAYERNAME);
	//添加直径标注
	AcGePoint3d topPoint(ptInsert.x + offset, ptInsert.y + m_diameter / 2.0, ptInsert.z);
	AcGePoint3d bottomPoint(ptInsert.x + offset, ptInsert.y - m_diameter / 2.0, ptInsert.z);
	AcGePoint3d center = CMathUtil::GetMidPoint(topPoint, bottomPoint);
	center.x = ptInsert.x +  10;
	CString temp;
	//temp.Format(L"%%%%C%s", removeLastZero(m_diameter));
	temp = L"%%C";
	CDimensionUtil::AddDimAligned(topPoint, bottomPoint, center, temp, NULL, NULL);

	//添加总长标注
	AcGePoint3d LendPoint(topPoint);
	LendPoint.x -= m_totalLength;
	center =  CMathUtil::GetMidPoint(topPoint, LendPoint);
	center.y = topPoint.y + DIMDISTANCE * 3;
	CDimensionUtil::AddDimRotated(topPoint, LendPoint, center, NULL, NULL);
	
	
	//添加lc标注
	AcGePoint3d LcendPoint(topPoint);
	LcendPoint.x -= m_bladeLength;
	center = CMathUtil::GetMidPoint(topPoint, LcendPoint);
	center.y = topPoint.y + DIMDISTANCE;
	CDimensionUtil::AddDimRotated(topPoint, LcendPoint, center, NULL, NULL);
	//添加lf标注 
	AcGePoint3d LfendPoint(topPoint);
	LfendPoint.x -= m_grooveLength;
	center = CMathUtil::GetMidPoint(topPoint, LfendPoint);
	center.y = topPoint.y + DIMDISTANCE * 2;
	CDimensionUtil::AddDimRotated(topPoint, LfendPoint, center, NULL, NULL);
	
	//切换到显示层
	CLayerSwitch lay(L"1");
	//补线
	AcGePoint3d TopEndPoint(0,0,0);
	TopEndPoint.x = ptInsert.x - m_totalLength + offset;
	TopEndPoint.y = ptInsert.y + (m_diameter - 2* offset) / 2;
	TopEndPoint.z = ptInsert.z;
	AcGePoint3d BottomEndPoint(TopEndPoint);
	BottomEndPoint.y = TopEndPoint.y - (m_diameter - 2 * offset);
	CLineUtil::CreateLine(TopEndPoint, BottomEndPoint);
	return 0;
}

