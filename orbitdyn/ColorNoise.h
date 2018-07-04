#pragma once
#include <OrbitDyn.h>
class ColorNoise
{
public:

	ColorNoise(void)
		:sigma(1),mean(0),delta(1),a(0.2),s(0)
	{
	}

	ColorNoise(double sg,double mn)
		:sigma(sg),mean(mn),delta(1),a(0.2),s(0)
	{
	}

	ColorNoise(double sg,double mn,double dlt,double aa)
		:sigma(sg),mean(mn),delta(dlt),a(aa),s(0)
	{
	}

	~ColorNoise(void)
	{
	}

	double sigma; // �����������ķ���
	double mean;  // �����������ľ�ֵ

	double delta; // ��ԼΪ����
	double a;     // �����ϵ��0<a<1,a=0ʱΪ������

	double s;     // ״̬

	// ȡ������
	double GetNum()
	{
		s = delta*WhiteNoise(sigma*3,mean) + a*s;
		return s;
	}
};
