#include "Kepler.h"

//! �⿪���շ��� M = E - e*sin(E)
// ����֪e��M���ҵ�E���㿪���շ���
// ����ţ�ٵ������ⷽ�̣����庯��:
// f(E) = E - e*sin(E) - M
// �� f'(E) = 1 - e*cos(E)
// ͨ����ʽ���е�������E:
//                  f(E|k)
// E|k+1 = E|k - ----------
//                 f'(E|k)
// ����˫���߹���������շ���Ϊ M = e*sin(E) - E���ⷨ����
ORBITDYN_API double KeplerFunc(double ee,double MM)
{
	double E0 = MM;
	double E1;
	if(ee<=1.0) E1 = E0-(E0-ee*sin(E0)-MM)/(1-ee*cos(E0));
	else       E1 = E0-(ee*sinh(E0)-E0-MM)/(ee*cosh(E0)-1.0);
	while(fabs(E1-E0)>1e-8)
	{
		E0 = E1;
		if(ee<=1.0) E1 = E0-(E0-ee*sin(E0)-MM)/(1-ee*cos(E0));
		else       E1 = E0-(ee*sinh(E0)-E0-MM)/(ee*cosh(E0)-1.0);
	}
	return E1;
}

//! ����忪���շ��̣�����ֵΪE+w
// ��������������е㲻ͬ���˴�����Ϊ��
// ksi = e*cos(w)
// eta = e*sin(w)
// lamda = w + M
// ���忪���շ���дΪ
// u~ - lamda = ksi*sin(u~) - eta*cos(u~) 
// ����u~ = w + E
// ����ţ�ٵ����������巽��
// f(u~) = u~ - lamda - (ksi*sin(u~) - eta*cos(u~))
// �� f'(u~) = 1 - (ksi*cos(u~) + eta*sin(u~))
ORBITDYN_API double KeplerFunc2(double ksi,double eta,double lamda)
{
	int JJ = 0;
	double Ew = lamda + ksi*sin(lamda) - eta*cos(lamda);
	double sEw = sin(Ew);
	double cEw = cos(Ew);
	double W0 = Ew - (lamda + ksi*sEw - eta*cEw);
	while(fabs(W0)>1e-8)
	{
		Ew = Ew - W0 / (1.0 - (ksi*cEw + eta*sEw));
		JJ++;
		if(JJ>20){
			KeplerException ke;
			ke.SetDetails("KeplerFunc2(): Too many iterations,ksi=%e,eta=%e,lamda=%e",ksi,eta,lamda);
			throw ke;
		}
		sEw = sin(Ew);
		cEw = cos(Ew);
		W0 = Ew - (lamda + ksi*sEw - eta*cEw);
	}
	return Ew;
}

//Ĭ�Ϲ���
Kepler::Kepler():a(6978.14),e(0.0),i(0.0),o(0.0),w(0.0),M(0.0)
{
}

//����ת��
Kepler::Kepler(const double oe[6]) 
{
	a = oe[0];	e = oe[1];	i = oe[2];
	o = oe[3];	w = oe[4];	M = oe[5];
} 

Kepler::Kepler(double SemimajorAxis,double Eccentricity,double Inclination,
	   double RAAN,double ArgPerigee,double MeanAnomaly)
{   
	a = SemimajorAxis;		e = Eccentricity;		i = Inclination;
	o = RAAN;				w = ArgPerigee;			M = MeanAnomaly;
}

Kepler::~Kepler()
{

};


double Kepler::Axis() const 
{
	return a; 
}

double Kepler::Ecce() const 
{ 
	return e;
}

double Kepler::Incl() const
{ 
	return i*DEG;
}

double Kepler::RAAN() const
{ 
	return o*DEG; 
}

double Kepler::AofP() const
{ 
	return w*DEG; 
}

double Kepler::MeanA() const
{ 
	return M*DEG; 
}

double Kepler::r() const
{
	if(a>0)
		return a*( 1.0 - e*cos(KeplerFunc(e,M)) );
	else
		return a*( 1.0 - e*cosh(KeplerFunc(e,M)) );
}

double Kepler::v() const
{
	if(a>0)
		return sqrt(2.0/(a*( 1.0 - e*cos(KeplerFunc(e,M)))) - 1.0/a );
	else
		return sqrt(2.0/(a*( 1.0 - e*cosh(KeplerFunc(e,M)))) - 1.0/a );
}

double Kepler::p() const
{ 
	return a*(1-e*e);
}

//! return f
double Kepler::f() const
{
	double E = KeplerFunc(e,M);
	if(e<=1.0)
		return fmod(atan2(a*sqrt(1-e*e)*sin(E),a*(cos(E)-e))+PI2,PI2);
	else
		return fmod(atan2(-a*sqrt(e*e-1)*sinh(E),a*(cosh(E)-e))+PI2,PI2);
}

//! return u
double Kepler::u() const
{
	return fmod(f()+w+PI2,PI2);
}

//! return E
double Kepler::E() const
{
	return KeplerFunc(e,M);
}

double Kepler::n(double miu /*= GE*/) const
{
	return a>0 ? sqrt(miu/a/a/a) : sqrt(-miu/a/a/a); 
}

double Kepler::ApogeeRadius() const
{
	return a*(1+e);
}

double Kepler::PerigeeRadius() const
{
	return a*(1-e);
}

double Kepler::ApogeeAltitude(double r /*= Re*/) const
{
	return a*(1+e)-r;
}

double Kepler::PerigeeAltitude(double r /*= Re*/) const
{
	return a*(1-e)-r; 
}

//! ���ù������,����Ƕȱ���Ϊ���ȣ����λ��Ϊƽ�����
void Kepler::SetElements(double SemimajorAxis,double Eccentricity,double Inclination,
								double RAAN,double ArgPerigee,double MeanAnomaly)
{
	a = SemimajorAxis;	e = Eccentricity;	i = Inclination;
	o = RAAN;			w = ArgPerigee;		M = MeanAnomaly;
}
//! ���ù������,����Ƕȱ���Ϊ���ȣ����λ��Ϊ������
void Kepler::SetElementsF(double SemimajorAxis,double Eccentricity,double Inclination,
								 double RAAN,double ArgPerigee,double TrueAnomaly)
{
	a = SemimajorAxis;	e = Eccentricity;	i = Inclination;
	o = RAAN;			w = ArgPerigee;
	double r = a*(1-e*e)/(1+e*cos(TrueAnomaly));
	// ������Բ��˫���ߣ���ʽ����ʽ�ϻ�����ͬ��ֻ���ڸ����²�һ�����ţ�ֻҪ��1-e^2ȡ����ֵ�Ϳ�����
	double E;
	if(e<1.0)
	{
		E = fmod(atan2(r*sin(TrueAnomaly)/a/sqrt(1-e*e),r/a*cos(TrueAnomaly)+e)+PI2,PI2);
		M = E - e*sin(E);
	}
	else if(e>1.0)
	{
		double x = (-r*sin(TrueAnomaly)/a/sqrt(e*e-1)) / (r/a*cos(TrueAnomaly)+e);
		E = 0.5*log((1+x)/(1-x));
		M = e*sinh(E) - E;
	}
	else
	{
		//??
	}
}
//! ���ù������,����Ƕȱ���Ϊ���ȣ����λ��Ϊ�������
void Kepler::SetElementsU(double SemimajorAxis,double Eccentricity,double Inclination,
								 double RAAN,double ArgPerigee,double Argument)
{
	a = SemimajorAxis;	e = Eccentricity;	i = Inclination;
	o = RAAN;			w = ArgPerigee;
	double r = a*(1-e*e)/(1+e*cos(Argument-w));

	double E;

	if(e<1.0)
	{
		E = fmod(atan2(r*sin(Argument-w)/a/sqrt(1-e*e),r/a*cos(Argument-w)+e)+PI2,PI2);
		M = E - e*sin(E);
	}
	else if(e>1.0)
	{
		double x = (-r*sin(Argument-w)/a/sqrt(e*e-1)) / (r/a*cos(Argument-w)+e);
		E = 0.5*log((1+x)/(1-x));
		M = e*sinh(E) - E;
	}
	else
	{
		//??
	}
}
//! ���ù������,����Ƕȱ���Ϊ����
void Kepler::SetElements(const double elem[6])
{
	a = elem[0];	e = elem[1];	i = elem[2];
	o = elem[3];	w = elem[4];	M = elem[5];
}

//! �����������������
double * Kepler::ToArray(double array[6])
{
	array[0]=a;	array[1]=e;	array[2]=i;
	array[3]=o;	array[4]=w;	array[5]=M;
	return array;
}


// ��׼�����,�Ƕȵ�λΪ��
ORBITDYN_API ostream & operator << (ostream & out,const Kepler & kp)
{
//#define SPACE  "     "
//	out.setf(ios::fixed);
//	int w = out.width();
//	out.precision(5);
//	out.width(11);
//	out<<kp.a<<SPACE;
//	out.width(11);
//	out.precision(8);
//	out<<kp.e<<SPACE;
//	out.precision(5);
//	out.width(9);
//	out<<kp.i*DEG<<SPACE;
//	out.width(9);
//	out<<kp.o*DEG<<SPACE;
//	out.width(9);
//	out<<kp.w*DEG<<SPACE;
//	out.width(9);
//	out<<kp.M*DEG;
//	out.width(w);
//	out.unsetf(ios::fixed);
//	return out;
//#undef SPACE
	out << kp.a << "\t" << kp.e << "\t" << kp.i*DEG << "\t" << kp.o*DEG << "\t" << kp.w*DEG << "\t" << kp.M*DEG;
	return out;
}

// ��׼������,�Ƕȵ�λΪ����
ORBITDYN_API istream & operator >> (istream & in,Kepler & kp)
{
	in>>kp.a;	in>>kp.e;	in>>kp.i;
	in>>kp.o;	in>>kp.w;	in>>kp.M;
	return in;
}
