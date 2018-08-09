// semi-major axis maneuver
#include <OrbitDyn.h>
#include <PerfTimer.h>

using namespace Constant;

//! LEO�����ǲ���
// ��������ı���ǣ���Բ�����С������
// a ����볤�ᣬkm
// i0 ��ʼ�����ǣ���
// it Ŀ������ǣ���
// ascordes = 1 ��������
// ascordes = 0 �ڽ�����
// ���أ����Y������ٶ�����m/s
double leo_Linc(double a,double i0,double it,int ascordes)
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
		dv = v*sin((i0-it)*RAD/2)*2*1000;
		vo(0) = dv*sin((i0-it)/2);
		vo(1) = dv*cos((i0-it)/2);
	}
	else
	{
		dv = v*sin((i0-it)*RAD/2)*2*1000;
		vo(0) = dv*sin((i0-it)/2);
		vo(1) = dv*cos((i0-it)/2);
	}	
	
	
}


int main(int argc, char* argv[])
{
	double dv;

	dv = leo_inclination(7000, 0.1);

	dv = sma_ellipse(7000, 0.2, 0, 10);

	return 0;
}