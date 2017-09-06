#ifndef __MANEUVER_H
#define __MANEUVER_H

#include "config.h"
#include <float.h>

//! ������
class ORBITDYN_API Maneuver
{
public:
	//! ����ʱ��
	double StartTime;
	//! �ػ�ʱ��
	double EndTime;
	//! ��ط���
	vec3 DV;
	//! ��ص��ƽ�����
	double M;
	//! ȼ��������
	double Fuel;
	Maneuver():DV("0 0 0"){
		StartTime = DBL_MAX;
		EndTime = DBL_MAX;
		Fuel = 0;
	}
	~Maneuver(){}
	//! tʱ���Ƿ�ﵽ����ʱ��
	bool Start(double t) const{
		return t>StartTime;
	}
	//! tʱ���Ƿ�ﵽ�ػ�ʱ��
	bool Finish(double t) const{
		return t>EndTime;
	}
};

////! ������:ʱ�俪�ػ�
//class Maneuver_Duration : public Maneuver
//{
//public:
//	vec DV;          // ���������Ҫ��DV
//	double StartTime;   // ������ʼʱ��
//	double EndTime;     // ��������ʱ��
//	Maneuver_Duration()	{
//	}
//	~Maneuver_Duration(){
//	}
//	bool Start(double t) const{
//		return t>=StartTime;
//	}
//	bool Finish(double t) const{
//		return t>=EndTime;
//	}
//};

////! ������:�ٶ������ػ�
//class Maneuver_DV : public Maneuver
//{
//public:
//	double DV_Need;     // ���������Ҫ��DV
//	double DV_Now;
//	Maneuver_DV(){
//		DV_Need = 0;
//		DV_Now = 0;
//	}
//	~Maneuver_DV(){}
//	bool Finish() const{
//		return DV_Now>=DV_Need ? true : false;
//	}
//};

#endif //__MANEUVER_H
