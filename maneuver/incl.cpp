// semi-major axis maneuver
#include <OrbitDyn.h>
//#include <PerfTimer.h>

using namespace Constant;

//! LEO�����ǲ���
// ��������ı���ǣ���Բ�����С������
// a ����볤�ᣬkm
// i0 ��ʼ�����ǣ���
// it Ŀ������ǣ���
// ascordes = 1 ��������
// ascordes = 0 �ڽ�����
// ���أ����Y������ٶ�����m/s
double leo_inclination(double a,double i0,double it,int ascordes)
{	
	double v = sqrt(GE /a);
	if(ascordes == 1) // ������
		return v*(i0-it)*RAD*1000;
	else
		return v*(it-i0)*RAD*1000;
}

//! LEO�����ǲ���
// ����������ı���ǣ���Բ���������Ǹı�
// a ����볤�ᣬkm
// i0 ��ʼ�����ǣ���
// it Ŀ������ǣ���
// ascordes = 1 ��������
// ascordes = 0 �ڽ�����
// ���أ����ϵ���ٶ�����m/s
vec3 leo_Binc(double a,double i0,double it,int ascordes)
{
	double v = sqrt(GE /a);
	double dv; 
	vec3 vo;
	if(ascordes == 1) // ������
	{
		dv = v*sin((it - i0)*RAD/2)*2*1000;
		vo(0) = -dv*sin((it - i0) * RAD / 2);
		vo(1) = -dv*cos((it - i0) * RAD / 2) * sign(it - i0);
	}
	else
	{
		dv = v*sin((it - i0)*RAD/2)*2*1000;
		vo(0) = -dv*sin((it - i0) * RAD / 2);
		vo(1) = dv*cos((it - i0) * RAD / 2) * sign(it-i0);
	}
	vo(2) = 0;
	
	return vo;
}

int main(int argc, char* argv[])
{
	double dv;

	dv = leo_inclination(7000, 98.2, 98.22,1);

	vec3 dvb = leo_Binc(7000, 98.2, 98.22, 0);

	return 0;
}
