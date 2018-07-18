// semi-major axis maneuver
#include <OrbitDyn.h>
#include <PerfTimer.h>

using namespace Constant;

//! ��Բ����İ볤�����
double sma_nearcircle(double a,double delta)
{
	double n = sqrt(GE / a / a / a);
	return 2 / n * deltaa;
}

double sma_ellipse(double a, double e, double f, double deltaa)
{
	double r = a * (1 - e * e) / (1 + e * cos(f));
	double v1 = sqrt(GE*(2 / r - 1 / a));
	double v2 = sqrt(GE*(2 / r - 1 / (a + deltaa)));
	return v2 - v1; // TODO: �и����ǣ�
}


int main(int argc, char* argv[])
{
	double dv;

	dv = sma_nearcircle(7000, 10);

	dv = sma_ellipse(7000, 0.2, 0, 10);

	return 0;
}