/*!
\file  Facility.cpp
\author HanDle
����վ����ϵλ��
*/

#ifndef __FACILITY_H
#define __FACILITY_H

#include "Coordination.h"
#include "Utility.h"

/*!����վ
*/
class ORBITDYN_API CFacility
{
public:
	//! ��С����
	double Elevation;

	//! ��������
	double Longitude,Latitude,Altitude;

	//! �ع�ϵλ��
	vec3 ECFPosition;

	//! Ĭ�Ϲ��캯��
	CFacility()
		:Elevation(0.08726646),
		Longitude(116.388),
		Latitude(39.9),
		Altitude(0.2),
		ECFPosition("0 0 0")
	{
		CalculateECF();
	}

	//! ����γ�ȹ��캯��
	CFacility(double lon,double lat,double alt,double elev = 5.0);

	//! ����
	~CFacility(){ }

	//! ���þ�γ��
	void SetGeodetic(double Longitude,double Latitude,double Altitude);

	//! ����ϵλ���ٶ�
	void GetECI(const CDateTime time,vec3 & Position,vec3 & Velocity);

	//! J2000.0����ϵλ��
	vec3 ECIPos(const CDateTime time);

private:

	//! �������ϵλ���ٶ�
	void CalculateECI(CDateTime time);

	//! ����ع�ϵλ��
	void CalculateECF();
};

// �ع�ϵ������վ����ϵ��ת������
ORBITDYN_API mat33 VehicleCoord(double Longitude,double Latitude);

#endif  // __FACILITY_H
