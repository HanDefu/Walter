﻿#pragma once
#include "Com.h"

//整硬铣刀阶梯数据
typedef struct ZYDJStepData
{
	int index;
	double m_diameter;//直径
	double m_stepLength;//阶梯长度
	double m_angle;//阶梯主偏角
}ZYXDStepData;

//整硬刀具共用的类
class CZYDJData
{
public:
	CZYDJData();
	~CZYDJData();
	//在模型空间中绘图
	void Draw();

	void SetStepData(vector<ZYXDStepData> const& data);
	void SetPreDiameter(double diameter);
	void SetDaobing(CString Daobing);
	void SetLabberCount(int labberCount);
	//设置是否为扩孔刀
	void SetKKD(bool isKKD) { m_IsKKD = isKKD; }
	bool GetIsKKD() const { return m_IsKKD; }
	void SetTotalLength(double length) { m_totalLength = length; }
private:
	vector<ZYXDStepData> m_StepData;
	
	//扩孔刀数据预孔直径
	double m_Prediameter;

	//刀柄名称
	CString m_DaoBingName;

	//刀具阶梯数量
	int m_DjLabberCount;
	//是否为扩孔刀
	bool m_IsKKD;
	//悬浮长度总长
	double m_totalLength;
};
