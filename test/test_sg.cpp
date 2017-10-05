/*!
\file test.cpp
\author HanDle
���Գ���
*/
#include <OrbitDyn.h>
#include <PerfTimer.h>

using namespace Constant;

void test_sg()
{
	CDateTime t(2006,9,13,1,2,3);

	double dpsi,deps;
	nutation_angle(t,dpsi,deps); // dpsi:�ƾ��¶�
	double eps = Ecliptic_Equator_Angle(t);
	double zeta,theta,Z;
	precession_angle(t,zeta,theta,Z);

	double dt = t.GetUT1CentNum()*36525.0; // + Earth.GetdUT1(t)/86400.0; // �� t.GetUT1CentNum()�п���dUT1
	double sgm = fmod((280.4606184 + 360.985647365*dt + 0.2908e-12*dt*dt)*RAD,PI2);
	double sgs =  fmod(dpsi*cos(eps) + sgm, PI2);
	double sgj2000 = sgm - zeta - Z;

	cout << "��������ƽ����ʱ��" << sgm << "rad    " << sgm*DEG << "deg" << endl;
	cout << "�������������ʱ��" << sgs << "rad    " << sgs*DEG << "deg" << endl;
	cout << "J2000��������ƽ����ʱ��" << sgj2000 << "rad    " << sgj2000*DEG << "deg" << endl;
}

int main(int argc, char* argv[])
{
	CPerfTimer timer;
	timer.Start();

	try
	{
		cout.precision(12);

		test_sg();
	}
	catch(BaseException& e)
	{
		cerr << e.GetFullMessage() << endl;
	}
 	catch(exception* e)
 	{
 		cerr << ((BaseException*)e)->what() << endl;
 	}

	timer.Stop();
	cout<<"Total Timer:"<<timer.Elapsed()<<"s"<<endl;
	return 0;
}