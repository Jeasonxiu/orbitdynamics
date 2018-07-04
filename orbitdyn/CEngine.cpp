/*!
\file Engine.cpp
\author HanDle
*/

#include <cmath>
#include "CEngine.h"
#include "constant.h"

double CEngine::dm()
{
	return -F/Isp/Constant::g;
}

bool CEngine_const::dv_dt(double SatMass,const double dv,double &halfdt) const
{

	return true;
}


CEngine_Poly::CEngine_Poly(void)
{
//��Ͳ���
	D0 = -0.97f;
	D1 = 28.64f;
	D2 = -6.09f;
	D3 = 0.5f;
	D4 = 0.0f;
	G0 = 1888.8f;
	G1 = 635.1f;
	G2 = -403.6f;
	G3 = 89.61f;
//��������
	FullFuelMass=120.0f;
	FuelMass0=100.0f;
	FuelDensity=1008.0f;
	Temperature0=273.0f;
	Press0=1.7f;
//����״̬
	Temperature=Temperature0;
	M=FuelMass0;
	Press=Press0;
//�����������������ʱ��(��)
	MaxManeuverTime=2400.0f;
}

/*!
����dv��Ҫ��ǰ���ʱ�䡢ѹ����ȼ������\n
SatMass�����Ǹ���     dv����Ҫ���ٶȱ仯��\n
halfdt1���ڱ���ǰ��ʱ��\n
ֻ���ǰ��ε�ʱ�䣬�����ڲ��ı�maneuver��tank��ֵ\n
\return ����trueʱ����ʾ���halfdt��ֵ������falseʱ����ʾ��Ҫ�ķ���������ʱ�䳬�������������ʱ�䣬ֻ�ܹ����ʱ���80��(�ɵ���̫���ᵼ�±���������)
*/
bool CEngine_Poly::dv_dt(double SatMass,const double dv,double &halfdt) const
{
	double p1,p2,m,F,I;
	double h=4.0;
	double dvt=0;
	double t=0.0;
	p1=Press;
	m=M;
	while(dvt<dv)
	{
		F=D0+p1*(D1+p1*(D2+p1*(D3+p1*D4)));
		I=G0+p1*(G1+p1*(G2+p1*G3));
		p2=p1/(1.0f+(F*h/I/(FullFuelMass-m)));
		dvt+=F*h/(SatMass+m);
		if(dvt<dv)
		{
            p1=p2;
            m=FullFuelMass-(FullFuelMass-FuelMass0)*Temperature/Temperature0*Press0/p2;
			t+=h;
		}
		else
		{
            dvt-=F*h/(SatMass+m);
			if((dv-dvt)<1e-4) break;
			h=h/2.0f;
		}
	}
    halfdt=t/2;
	if(halfdt>MaxManeuverTime/2.0f)  //���������������������ʱ��
	{
		//halfdt=MaxManeuverTime/2.0;
		return false;
	}
	return true;
}  //end of dv_dt

/*!����dv1��dv2��Ҫ��ǰ���ʱ�䡢ѹ����ȼ������\n
SatMass�����Ǹ���     dv1,dv2����Ҫ���ٶȱ仯��\n
halfdt1,halfdt2���ڱ���ǰ��ʱ��\n
ֻ���ǰ��ε�ʱ�䣬�����ڲ��ı�maneuver��tank��ֵ\n
\return ����trueʱ����ʾ���halfdt��ֵ������falseʱ����ʾ��Ҫ�ķ���������ʱ�䳬�������������ʱ�䣬ֻ�ܹ����ʱ���80��(�ɵ���̫���ᵼ�±���������)
*/
bool CEngine_Poly::dv_dt(double SatMass,const double dv1,double &halfdt1,const double dv2,double &halfdt2) const
{
	double p1,p2,m,F,I;
	double h=1.0f;
	double dvt1=0.0f,dvt2=0.0f;
	double t1=0.0f,t2=0.0f;
	p1=Press;
	m=M;
	while(dvt1<dv1)
	{
		F=D0+p1*(D1+p1*(D2+p1*(D3+p1*D4)));
		I=G0+p1*(G1+p1*(G2+p1*G3));
		p2=p1/(1.0f+(F*h/I/(FullFuelMass-m)));
		dvt1+=F*h/(SatMass+m);
		if(dvt1<dv1)
		{
			p1=p2;
			m=FullFuelMass-(FullFuelMass-FuelMass0)*Temperature/Temperature0*Press0/p2;
			t1+=h;
		}
		else
		{
			dvt1-=F*h/(SatMass+m);
			if((dv1-dvt1)<1e-4) break;
			h=h/2.0f;
		}
	}
	h=1.0;
	while(dvt2<dv2)
	{
		F=D0+p1*(D1+p1*(D2+p1*(D3+p1*D4)));
		I=G0+p1*(G1+p1*(G2+p1*G3));
		p2=p1/(1.0f+(F*h/I/(FullFuelMass-m)));
		dvt2+=F*h/(SatMass+m);
		if(dvt2<dv2)
		{
			p1=p2;
			m=FullFuelMass-(FullFuelMass-FuelMass0)*Temperature/Temperature0*Press0/p2;
			t2+=h;
		}
		else
		{
			dvt2-=F*h/(SatMass+m);
			if((dv2-dvt2)<1e-4) break;
			h=h/2.0f;
		}
	}

	halfdt1=t1/2;
	halfdt2=t2/2;

	if(halfdt1>MaxManeuverTime/2.0 ||halfdt2>MaxManeuverTime)  //���������������������ʱ��
	{
		//halfdt=MaxManeuverTime/2.0;
		return false;
	}
	return true;
}
//! ���ݵ�ǰ����ѹ�����㷢��������
double CEngine_Poly::GetPower()
{
	return F = D0 + Press * (D1 + Press * (D2 + Press * (D3 + Press * D4) ) );
}
//! ���´����ѹ����ʣ��ȼ������
void CEngine_Poly::UpdateStatus(double dt)
{
	Isp = G0 + Press * (G1 + Press * (G2 + Press * G3));
	double Pt = Press/(1 + F*dt/(FullFuelMass-M)/Isp);
	Press = Pt;
	M = FullFuelMass-(FullFuelMass-FuelMass0)*(Temperature/Temperature0)*(Press0/Pt);
}

/*! Ĭ��Ϊ�������
*/
CThrusterSet::CThrusterSet():Num(3)
{
	CT.set_size(3,3);
	CF.set_size(3,3);	
}

CThrusterSet::~CThrusterSet()
{

}

void CThrusterSet::Install(int n,mat& f,mat& t)
{
	Num = n;
	CF.set_size(3,Num);
	CF = f;
	CT.set_size(3,Num);
	CT = t;
}
