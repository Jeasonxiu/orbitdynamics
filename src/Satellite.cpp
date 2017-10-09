/*
	Filename: 	CSatellite.cpp
	Created:	2003/10/29
	Author:		HanDle
*/

#include "Satellite.h"
#include "Utility.h"

unsigned int CSatelliteBase::Counter=0;

CSatelliteBase::CSatelliteBase(void)
	:ElapsedTime(0.0),Position("0 0 0"),Velocity("0 0 0")
	,Mass0(1000.0),BurnedFuel(0.0)
	,Cd(2.2),AirDragArea(20.0),Cr(1.0),LightPressArea(20.0)
	,ThrustIsOn(false),ThrustDirection("1 0 0")
	,LL(6),Perturbation(ODP_EARTH_ALL),FirstTimeOutput(true),Save(false)
	,PNTime(0),wbi("0 0 0"),acc("0 0 0")
{
	Counter++;
	Name = char(96+Counter);
	Engine = new CEngine_const(270,20);
}

CSatelliteBase::CSatelliteBase(std::string name)
	:ElapsedTime(0.0),Name(name),Mass0(1000.0),BurnedFuel(0.0)
	,Cd(2.0),AirDragArea(20.0),Cr(1.0),LightPressArea(20.0)
	,ThrustIsOn(false),ThrustDirection("1 0 0")
	,LL(6),Perturbation(ODP_EARTH_ALL),FirstTimeOutput(true),Save(false)
	,PNTime(0)
{
	Counter++;
	Engine = new CEngine_const(270,20);
}

CSatelliteBase::~CSatelliteBase()
{
	delete Engine;
	if(FileInstElem.is_open()) FileInstElem.close();
	Counter--;
}
/*! ��ʼ������
��ElapsedTime����
��BurnedFuel����
���FileInstElem�Ѿ��򿪣��򼴽���رա���д���µ�����ʱ���Ὣ����պ���д��
��FirstTimeOutput��Ϊtrue
#�������Initialize()������Ӧ�õ��ô˺������Ա�֤��ʼ����ȫ#
*/
void CSatelliteBase::Initialize()
{
	ElapsedTime = 0;
	BurnedFuel = 0;
	ThrustIsOn = false;
	if(FileInstElem.is_open())
		FileInstElem.close();
	FirstTimeOutput = true;
	PNTime = 0;
	RefreshStatus();
}

/*! ��ʼ������
��ElapsedTime����
��BurnedFuel����
���FileInstElem�Ѿ��򿪣��򼴽���رա���д���µ�����ʱ���Ὣ����պ���д��
��FirstTimeOutput��Ϊtrue
#�������Initialize()������Ӧ�õ��ô˺������Ա�֤��ʼ����ȫ#
*/
void CSatelliteBase::Initialize(const CDateTime& t,const vec3& r,const vec3& v)
{
	Epoch = t;
	Position = r;
	Velocity = v;
	ElapsedTime = 0;
	BurnedFuel = 0;
	ThrustIsOn = false;
	if(FileInstElem.is_open())
		FileInstElem.close();
	FirstTimeOutput = true;
	PNTime = 0;
	RefreshMatrix(Epoch);
	RefreshStatus();
}

/*!���ļ���ȡ��ʼ������
*/
void CSatelliteBase::Initialize( const char* filename )
{
	char buf[1024],strbuf[100],equator;
	Kepler kp;
	fstream rorbit(filename,ios::in);
	if(!rorbit.is_open()){
		throw SatelliteException("CSatelliteBase::Initialize( const char* filename ) : can't find file");
	}

	while(!rorbit.eof())
	{
		rorbit.getline(buf,1024);
		sscanf_s(buf,"%s",strbuf);
		strdup(strbuf); // ת��Ϊ��д��ĸ
		std::string str(strbuf);
		if(str == "CENTER_NAME") {
			sscanf_s(buf,"%s %s %s",strbuf,strbuf,strbuf);
			if( !strcmp(strbuf,"EARTH") ) center = EARTH;
			if( !strcmp(strbuf,"MOON")  ) center = MOON;
		}
		else if(str == "EPOCH")		{
			sscanf_s(buf,"%s %s %s",strbuf,strbuf,strbuf);
			Epoch = StrToDateTime(strbuf);
		}
		else if(str == "SEMI_MAJOR_AXIS")	{
			sscanf_s(buf,"%s %c %lf",strbuf,&equator,&kp.a);  }
		else if(str == "ECCENTRICITY")		{
			sscanf_s(buf,"%s %c %lf",strbuf,&equator,&kp.e);  }
		else if(str == "INCLINATION")		{
			sscanf_s(buf,"%s %c %lf",strbuf,&equator,&kp.i);    kp.i *= RAD; }
		else if(str == "RA_OF_ASC_NODE")	{
			sscanf_s(buf,"%s %c %lf",strbuf,&equator,&kp.o);	kp.o *= RAD; }
		else if(str == "ARG_OF_PERICENTER")	{
			sscanf_s(buf,"%s %c %lf",strbuf,&equator,&kp.w);	kp.w *= RAD; }
		else if(str == "MEAN_ANOMALY")		{
			sscanf_s(buf,"%s %c %lf",strbuf,&equator,&kp.M);	kp.M *= RAD; }
		else if(str == "MASS")		{
			sscanf_s(buf,"%s %c %lf",strbuf,&equator,&Mass0);  }
	}
	rorbit.close();

	Kepler_Cartesian(kp,Position,Velocity);
	ElapsedTime = 0;
	BurnedFuel = 0;
	ThrustIsOn = false;
	if(FileInstElem.is_open())
		FileInstElem.close();
	FirstTimeOutput = true;
	PNTime = 0;
	RefreshMatrix(Epoch);
	RefreshStatus();
}

/*! ������Ҫ���ǵ��㶯����
\param l �������Ľ���
\param PTB ��Ҫ���ǵ��㶯�ı�־��Ĭ��ΪODP_LEO
*/
void CSatelliteBase::SetForce(int l,unsigned int PTB)
{
	LL = l;
	Perturbation = PTB;
}

/*! ���÷������ıȳ������
\param I �ȳ�
\param f ����
*/
void CSatelliteBase::SetEngine(double I,double f)
{
	Engine->Isp = I;
	Engine->F = f;
}

/*! �����Ƿ��Զ�����������
SetAutoSave() �� SetAutoSave(true) �Զ�����\n
SetAutoSave(false)   ���Զ�����
*/
void CSatelliteBase::SetAutoSave(bool save /* =true */)
{
	Save = save;
}
/*!  �Զ�����������
�Զ�����ĸ�ʽΪ��\n
ʱ��t       �볤��a    ƫ����e    ���i    ������ྭOmega     ���ص����w     ƽ�����M      �������u
*/
void CSatellite::SaveElem()
{
#define SPACE  "     "
	if(!FileInstElem.is_open())
	{
		std::string finst = Name + ".dat";
		const char * fi = finst.c_str();
		FileInstElem.open(fi,ios::out|ios::trunc);
	}
	FileInstElem.setf(ios::fixed);
	FileInstElem.precision(3);
	FileInstElem.width(15);
	FileInstElem<<ElapsedTime<<SPACE;
	FileInstElem.precision(5);
	FileInstElem.width(11);
	FileInstElem<<a<<SPACE;
	FileInstElem.width(11);
	FileInstElem.precision(8);
	FileInstElem<<e<<SPACE;
	FileInstElem.width(9);
	FileInstElem<<i*DEG<<SPACE;
	FileInstElem.width(9);
	FileInstElem<<Omega*DEG<<SPACE;
	FileInstElem.width(9);
	FileInstElem<<w*DEG<<SPACE;
	FileInstElem.width(9);
	FileInstElem<<M*DEG<<SPACE;
	FileInstElem.width(9);
	FileInstElem<<u*DEG<<SPACE;
	FileInstElem.precision(6);
	FileInstElem.width(14);
	FileInstElem<<Position[0]<<SPACE;
	FileInstElem<<Position[1]<<SPACE;
	FileInstElem<<Position[2]<<SPACE;
	FileInstElem<<Velocity[0]<<SPACE;
	FileInstElem<<Velocity[1]<<SPACE;
	FileInstElem<<Velocity[2]<<endl;
#undef SPACE
}

/*! ����һ�����������ƣ�ֹͣ��������
	һ��˺����Ĺ���˳��Ϊ��
- 1. ��������״̬����x
- 2. ����RKF78����������һ��������״̬����xh
- 3. ����ֹͣ����ȷ���Ƿ���xh�������ǵ�״̬��\n
     �������ֹͣ��������С�����ٴε���RKF78,ֱ���ﵽֹͣ����
- 4. ���Ϊ�䲽�����֣�����RKF78���صĽض�����޸Ĳ���

\n�����Ҫ���ش˺���������밴��������������������ɡ�
*/
int CSatelliteBase::OneStep(double StepSize,double MaxCutError /* = 1 */,CStopCondition* StopCon/* = NULL */)
{
	vec7 x,xh;
	double ee;
	x.subvec(0,2) = Position;
	x.subvec(3,5) = Velocity;
	x[6] = Mass();
	ee = RKF78(*this,StepSize,x,xh);

	if(StopCon == NULL)
	{
		ElapsedTime += StepSize;
		Position = xh.subvec(0,2);
		Velocity = xh.subvec(3,5);
		BurnedFuel += Mass() - xh[6];
		RefreshStatus();		
	}
	else
	{
		// ֹͣ�������ж�
	}

	vec7 y;
	DynFunction(ElapsedTime,xh,y);
	acc = y.subvec(3,5);

	return 1;
}

/*! ����dt��ʱ��
\param h ���Ƶļ��㲽��(��)
\param dt ���Ƶ�ʱ��(��)
����������й��������ֱ�ӵ���һ��˺������ɣ�������60��Ĳ�������һ���ã�\n
Satellite.Propagate(60,86400); \n
һ�������������Ҫ�鿴��һ��Ĺ�����ݣ�����Ҫ���������Զ�����: \n
Satellite.SetAutoSave(true); \n
������ɺ����ֱ�Ӷ�ȡ�Զ�������ļ�\n\n
����ѹ�����Ʒŵ���������ѭ���У���ʹ��:\n
Satellite.Propagate(Step,Step); \n
����StepΪ����ѧ��������/���沽��/��������
*/
void CSatelliteBase::Propagate(double h,const double dt)
{	
	double te = ElapsedTime + dt;
	bool calculate = true;
	if(FirstTimeOutput){
		if(Save) SaveElem();
		FirstTimeOutput = false;
	}

	while(calculate)
	{
		if(te-ElapsedTime<h)
		{
			calculate = false;
			h = te - ElapsedTime;
			if(h<1e-9) return;
		}
		OneStep(h);
		if(Save) SaveElem();
		if(fabs(ElapsedTime - PNTime) >= 1800.0)
		{
			RefreshMatrix(Epoch+PNTime);
			PNTime = ElapsedTime;
	}
}
}

//! ˢ������¶��Ⱦ���
void CSatelliteBase::RefreshMatrix( const CDateTime tt )
{
	PR = Precession(tt);
	NU = Nutation(tt);
	PW = PolarMotion(Epoch);
}

/*!
�������
*/
void CSatelliteBase::PropagateBackward(double h,const double dt)
{
	double te = ElapsedTime + dt;
	bool calculate = true;
	if(FirstTimeOutput){
		if(Save) SaveElem();
		FirstTimeOutput = false;
	}

	while(calculate)
	{
		if(te-ElapsedTime>h)
		{
			calculate = false;
			h = te - ElapsedTime;
			if(fabs(h)<1e-9) return;
		}
		OneStep(h);
		if(Save) SaveElem();
		if(fabs(ElapsedTime - PNTime) >= 1800.0)
		{
			RefreshMatrix(Epoch+PNTime);
			PNTime = ElapsedTime;
	}
}
}

/*! ���Ƶ����ĵ�
*/
void CSatelliteBase::Propagate2Perigee()
{
	double Step = 10;
	double pM;
	bool first = true , endwile = false;
	while( true )
	{
		Propagate(Step,Step);

		if(endwile)
			break;

		if(fabs(PI2-M)<15.0*RAD)
		{
			if(first)
			{
				first = false;
			}
			else
			{
				double n = (M - pM)/Step;
				if( M+n*Step>PI2 )
				{
					endwile = true;
					Step = fabs( (PI2-M) / n); // ���һ���ı䲽��ʹM�պ�Ϊ0����������ĵ�
				}
			}
			pM = M;
		}
	}
}

/*! ���Ƶ�Զ�ĵ�
*/
void CSatelliteBase::Propagate2Apogee()
{
	double Step = 10;
	double pM;
	bool first = true , endwile = false;
	while( true )
	{
		Propagate(Step,Step);

		if(endwile)
			break;

		if( PI-M>0.0 && PI-M<10.0*RAD)
		{
			if(first)
			{
				first = false;
			}
			else
			{
				double n = (M - pM)/Step;
				if( M+n*Step>PI )
				{
					endwile = true;
					Step = fabs( (PI-M) / n); // ���һ���ı䲽��ʹM�պ�ΪPI��������Զ�ĵ�
				}
			}
			pM = M;
		}
	}
}

/*! ���Ƶ�������
*/
void CSatelliteBase::Propagate2AscendingNode()
{
	double Step = 10;
	double pu;
	bool first = true , endwile = false;
	while( true )
	{
		Propagate(Step,Step);

		if(endwile)
			break;

		if(fabs(PI2-u)<15.0*RAD)
		{
			if(first)
			{
				first = false;
			}
			else
			{
				double n = (u - pu)/Step;
				if( u+n*Step>PI2 )
				{
					endwile = true;
					Step = fabs( (PI2-u) / n); // ���һ���ı䲽��ʹu�պ�Ϊ0��������������
				}
			}
			pu = u;
		}
	}
}

/*! ���Ƶ�������
*/
void CSatelliteBase::Propagate2DescendingNode()
{
	double Step = 10;
	double pu;
	bool first = true , endwile = false;
	while( true )
	{
		Propagate(Step,Step);

		if(endwile)
			break;

		if( PI-u>0.0 && PI-u<10.0*RAD)
		{
			if(first)
			{
				first = false;
			}
			else
			{
				double n = (u - pu)/Step;
				if( u+n*Step>PI )
				{
					endwile = true;
					Step = fabs( (PI-u) / n); // ���һ���ı䲽��ʹu�պ�ΪPI�������ｵ����
				}
			}
			pu = u;
		}
	}
}

/*!���Ƶ�һ������Ԫ*/
void CSatelliteBase::Propagate2Epoch(const CDateTime epoch)
{
	if(epoch < CurrentEpoch() )
		PropagateBackward(-100,epoch-CurrentEpoch());
	else
		Propagate(100,epoch-CurrentEpoch());
}

/*! ˲ʱ�������
Input:  Dv:�ٶ�����(km/s)  d:�ٶ�����������ϵ

Output:   None
Return:     None
*/
void CSatelliteBase::ImpluseManeuver(vec3 Dv,Coordination d /*= VVLH*/ )
{
	mat33 coi;
	switch(d) {
	case VVLH:
		coi = GetCoi(GetOrbitElements());
		Velocity += coi*Dv;
		break;
	case ECI:
		Velocity += Dv;
		break;
	default:
		throw SatelliteException("ImpluseManeuver(): Can't find Operation in this Direction,Maneuver is ignored!");
	}
	RefreshStatus();
}

ORBITDYN_API ostream & operator<<(ostream & os, const CSatelliteBase& sat)
{
	os<<sat.a<<"\t"<<sat.e<<"\t"<<sat.i*DEG<<"\t"
	  <<sat.Omega*DEG<<"\t"<<sat.w*DEG<<"\t"<<sat.M*DEG;
	return os;
}

/*! ��ʼ�����
\param elem �������a,e,i,O,w,M
\param flag ƽ������˲ʱ�����ı�־
flag = 'i' ��ʾ˲ʱ����\n
flag = 'm' ��ʾƽ������\n
*/
void CSatellite::Initialize(const CDateTime& t,const Kepler elem,const char flag/*='i'*/)
{
	Epoch = t;
	if(flag=='i')
	{
		Status0 = elem;
	}
	else if(flag=='m')
	{
		Status0 = Instant(elem);
	}
	a = Status0.a;
	e = Status0.e;
	i = Status0.i;
	Omega = Status0.o;
	w = Status0.w;
	M = Status0.M;
	E = Status0.E();

	f = Status0.f();
	u = fmod(f + w + PI2,PI2);
	n = Status0.n();

	Kepler_Cartesian(Status0,Position,Velocity);
	
	RefreshMatrix(Epoch);
	CSatelliteBase::Initialize();
}

/*! ���ɳ�ʼ�����ı���
*/
//void CSatellite::ReportInitial()
//{
//	std::string finit = Name + "Initial.dat";
//	const char * fi = finit.c_str();
//	fstream file(fi,ios::out);
	//fprintf(InitCon,"\t\t%s\n%s\n\n%s\n\n%s%s\n","Propagator Initial Conditions",
	//	"------------------------------------------------------------------------------",
	//	"Propagator Name = EarthFull","Gravity File Name = ",GrvFile);
	//fprintf(InitCon,"%s%d%s%d\n","Gravity Degree = ",LL," X ",LL);
	//fprintf(InitCon,"%s%s\n\n","Include Tesseral Terms?  ",P_Tesseral?"Yes":"No");
	//fprintf(InitCon,"%s%s\n","Include Lunar Gravity?  ",P_Lunar?"Yes":"No");
	//fprintf(InitCon,"%s%s\n\n","Include Solar Gravity?  ",P_Solar?"Yes":"No");
	//fprintf(InitCon,"%s%s\n\n","Include Post-Newton?  ",P_PostNewton?"Yes":"No");
	//fprintf(InitCon,"%s%s\n","Include Air Drag?  ",P_Air?"Yes":"No");
	////if(P_Air) fprintf(InitCon,"%s%s\n","Atmosphere Density File = ",AtmosFile);
	//if(P_Air) fprintf(InitCon,"%s%lf\n","DragArea = ",AirDragArea);
	//fprintf(InitCon,"\n%s%s\n","Include Light Pressure?  ",P_Light?"Yes":"No");
	//if(P_Light) fprintf(InitCon,"%s%lf\n\n","LightArea = ",LightPressArea);
	//fprintf(InitCon,"%s%lf\n\n","CSatellite Mass = ",DryMass);
	////fprintf(InitCon,"%s%d%s%d%s%d%s%d%s%d%s%lf\n","Orbit Epoch : ",StartEpoch.year,"-",StartEpoch.month,"-",StartEpoch.day,
	////	" ",StartEpoch.hour,":",StartEpoch.minute,":",StartEpoch.second);
	//fprintf(InitCon,"%s\n\t%s%lf\n","Initial Orbit Elements:","Semi-major Axis = ",a*Re);
	//fprintf(InitCon,"\t%s%lf\n","Eccentricity = ",e);
	//fprintf(InitCon,"\t%s%lf\n","Inclination = ",i/RAD);
	//fprintf(InitCon,"\t%s%lf\n","RAAN = ",Omega/RAD);
	//fprintf(InitCon,"\t%s%lf\n","Angle of Periapsis = ",w/RAD);
	//fprintf(InitCon,"\t%s%lf\n","Mean Anomaly = ",M/RAD);
	//fprintf(InitCon,"\n%s\n","Integrator = RKF7(8)");
	////fprintf(InitCon,"%s%lf","Initial Integrator Step Size = ",h);
	////if(int(dd)==1) fprintf(InitCon,"\t%s","Fixed");
	////	else fprintf(InitCon,"\n\t%s%lf","Relative Error Tolerance = ",dd);
	//fclose(InitCon);
//	file.close();
//}

void CSatellite::GetECF(vec3& ECFr,vec3& ECFv)
{
	mat33 hg = ECI_ECF(Epoch+ElapsedTime);
	vec3 w;
	w.zeros();
	w(2) = We;
	w = PolarMotion(Epoch+ElapsedTime)*w;

	ECFr = hg*Position;
	ECFv = hg*Velocity - cross(w,ECFr);
}

/*!
   ÿ������״̬Position��Velocity�仯ʱ���ô˺����������ǹ������
*/
void CSatellite::RefreshStatus()
{
	Kepler newkepler = Cartesian_Kepler(Position,Velocity);
	a = newkepler.a;
	e = newkepler.e;
	i = newkepler.i;
	Omega = newkepler.o;
	w = newkepler.w;
	M = newkepler.M;

	E = newkepler.E();
	f = newkepler.f();
	u = fmod(f + w + PI2,PI2);
	n = newkepler.n();
	r = norm(Position,2);
	v = norm(Velocity,2);
	w0 = sqrt(GE/a/a/a)*(1+2*e*cos(f)+e*e*cos(f)*cos(f))/sqrt(pow(1-e*e,3));
	Coi = GetCoi(newkepler);
}
/*! ����ƽ������
*/
Kepler CSatellite::MedianElement() const
{
	Kepler Inst(a,e,i,Omega,w,M);
	return Mean(Inst);
}
/*! ����ѧ����
   Input:  t:ʱ��   x[6]:״̬����
   Output:    y[6]:״̬������΢��
*/
void CSatellite::DynFunction(const double t,const vec& x,vec& y)
{
	vec3 F,rs,FG,ECFr,F12,r,rd;
	F.zeros();

	r = x.subvec(0,2);
	rd = x.subvec(3,5);

	CDateTime tt = Epoch + t;

	// Zonal Harmonic Terms and Tesseral Harmonic Terms
	mat33 HG = PW*EarthRotation(tt)*NU*PR;
	ECFr =  HG * r;  //J2000.0==>ECF  //�˴���Ϊ�����ǳ�ʼ��Ԫ��ֵ

	if(Perturbation & ODP_EARTH_ZONAL)
	{
		FG = PZonalHarmonic(LL,ECFr);
		if(Perturbation & ODP_EARTH_TESSERAL)
		{
			FG += PTesseralHarmonic(LL,ECFr);
		}
		F12 = HG.t() * FG;  //ECF==>J2000.0   //�˴���Ϊ�����ǳ�ʼ��Ԫ��ֵ
		F = F + F12;
	}


	// Lunar Gravitation
	if(Perturbation & ODP_LUNAR_CENT)
	{
		F += PThirdBodyGrv( r , Moon::Instance()->GetECI(tt) , GM );
	}

	// Solar Gravitation
	if( (Perturbation & ODP_SOLAR_CENT)  || (Perturbation & ODP_SOLAR_PRESSURE) || (Perturbation & ODP_AIR_DRAG) )
	{
		rs = Sun::Instance()->GetECI(tt);   // rs:solar coordinate in J2000.0
	}

	// solar gravity
	if( Perturbation & ODP_SOLAR_CENT )
	{
		F += PThirdBodyGrv(r,rs,GS);
	}

	// Sunlight Pressure
	if( Perturbation & ODP_SOLAR_PRESSURE )
	{
		F += PLightPressure(Cr*LightPressArea/(Mass0-BurnedFuel),r,rs);
	}

	// Atmospheric Drag
	if( Perturbation & ODP_AIR_DRAG )
	{
		F += PAirDrag(tt,Cd*AirDragArea/(Mass0-BurnedFuel),ECFr,r,rd);
	}

 	// Post-Newtonian Effect
	if( Perturbation & ODP_POSTNEWTON )
	{
		F += PPostNewton(r,rd);
	}

	//if(Perturbation & ODP_LUNAR_NON_SPHERE)
	//{   // һ�����ڵ���ת�ƹ����������Ӱ����֮��
	//	F += PLunar_NonSphere(TDT,ECI_MCI(r,TDT));
	//}

	// Thrust
	if(ThrustIsOn)
	{
		F += trans(qbi.C()) * ThrustDirection * Engine->F/(Mass0-BurnedFuel)/1000.0;
	}

	// Perturbation Functions
	double R = norm(r,2);
	double miurrr = GE/R/R/R;
	y.subvec(0,2) = x.subvec(3,5);
	y.subvec(3,5) = - miurrr * r + F;
	if( ThrustIsOn )
		y[6] = Engine->dm();
	else
		y[6] = 0;
}

/*!  �����ǵĵع�ϵ����
*/
const vec3 CSatellite::ECFPos()
{
	return ECI_ECF(Epoch + ElapsedTime) * Position;
}
/*! �����ǵľ�γ��(��������)
*/
CSpherical CSatellite::GetLLA()
{
	vec3 ECF = ECFPos(); // �ع�ϵλ��
	return ECF_LLA(ECF);
}
/*! �ɸ������������µ㾭γ��
*/
CSpherical CSatellite::SubSatPoint(CSpherical & Geodetic,CSpherical & SSP)
{
	double r = a*(1 - e * cos(E));
	// ����
	double lmdG = GetSG(Epoch+ElapsedTime);
	Geodetic.Longitude = fmod(atan2(Position(1),Position(0))-lmdG+4*PI,PI2);
	if(Geodetic.Longitude>PI) Geodetic.Longitude -= PI2;
	// γ��
	double zsr = Position(2)/r;
	double Lan,BB,Rs0,hh;
	if( zsr<-1 ) Geodetic.Latitude = -90;
	else if( zsr>1 ) Geodetic.Latitude = 90;
	else Geodetic.Latitude = asin(zsr);
	Lan = Geodetic.Latitude;

	// ���µ㾭��
	SSP.Longitude = Geodetic.Longitude;
	// ���µ�γ��
	double Lane = Geodetic.Latitude;
	double Ob1 = 1-Oblate;
	for(int II=1;II<=4;II++)
	{
		double sinLane = sin(Lane);
		double cosLane = cos(Lane);
		BB = atan(tan(Lane)/Ob1/Ob1);
		Rs0 = Re*Ob1/sqrt(sinLane*sinLane+Ob1*Ob1*cosLane*cosLane);
		hh = sqrt(r*r-(Rs0*Rs0*sin(BB-Lane)*sin(BB-Lane)))-Rs0*cos(BB-Lane);
		Lane = Lan-asin(hh*sin(BB-Lane)/r);
	}
	Geodetic.Altitude = hh;
	SSP.Latitude = Lane;
	SSP.Altitude = 0;
	return SSP;
}

//double CSatellite::Propagate2u(double &t,double h,CSatellite &s,double ut,const double dd,FILE* out)
//{//failure
//	double x1[6],x[6],tmp[6],tt;
//	s.mean();
//	x[0]=s.M_a;x[1]=s.M_i;x[2]=s.M_omiga;x[3]=s.M_ksi;x[4]=s.M_eta;x[5]=s.M_lamda;
//	x1[0]=s.a;x1[1]=s.i;x1[2]=s.Omega;x1[3]=s.ksi;x1[4]=s.eta;x1[5]=s.lamda;
//	tt=t;
//	for(int i=0;i<6;i++) tmp[i]=x1[i];
////----lamda1--------------------------------------------------------------------------
//	double n=sqrt(1/x[0]/x[0]/x[0]); //(unit:rad/s)
//	double sini2=sin(x[1])*sin(x[1]);
//	double sini4=sini2*sini2;
//	double e=sqrt(x[3]*x[3]+x[4]*x[4]);
//	double ee=sqrt(1-x[3]*x[3]-x[4]*x[4]);
//	double lamda1=A2/x[0]/ee*n*((2-2.5*sini2)+(1-1.5*sini2)*ee);
////----end lamda1---------------------------------------------------------------------
//	//double sinu=sin(ut);
//	//double cosu=cos(ut);
//	s.other('m');
//	double u1=s.M_u;
//	double du=ut-u1;
//	du=du>0?du:(du+PI2);
////����Сƫ���ʹ����df+dwԼ����dM+dwԼ����dlamda
//	double dt1=du/(n+lamda1);   //(unit:minute)
//	double dt=dt1;
//	double dt2=dt1;
//	while(fabs(dt2)>1/60.0/TU)
//	{
//		Propagate2T(tt,h,x1,dt,dd,NULL);
//		//double Ew1=kepler2(x1[3],x1[4],x1[5]);
//		//double w1=fmod(atan2(x1[4],x1[3])+PI2,PI2);
//		//double E1=fmod(Ew1-w1+4*PI,PI2);
//		//double e1=sqrt(x1[3]*x1[3]+x1[4]*x1[4]);
//		//double r1=x1[0]*(1-e1*cos(E1));
//		//double ee1=sqrt(1-e1*e1);
//		CSatellite ttt;
//		ttt.a=x1[0];ttt.i=x1[1];ttt.Omega=x1[2];ttt.ksi=x1[3];ttt.eta=x1[4];ttt.lamda=x1[5];
//		ttt.elem_to_xyz();
//		dt2=-ttt.z/ttt.dz;
//		//dt2=-(r1*sqrt(x1[0]))*((cos(E1)-e1)*sin(w1)-ee1*cos(w1))/(-sin(E1)*sin(w1)+ee1*cos(E1)*cos(w1));
//		dt=dt+dt2;  //refresh dt
//		for(int ii=0;ii<6;ii++) x1[ii]=tmp[ii];  //return to start state
//		tt=t;                                    //return to start state
//	}
//	Propagate2T(t,h,x,dt,dd,out);
//	s.a=x[0];s.i=x[1];s.Omega=x[2];s.ksi=x[3];s.eta=x[4];s.lamda=x[5];
//	return dt;
//}

//double CSatellite::Propagate2M(double &t,double h,CSatellite &s,const double Mt,const double dd,FILE* out)	//���Ƶ�ƽ�����M		|
//{
//	double n=sqrt(1/s.a/s.a/s.a);
//	s.other('i');
//	double dM=s.M-Mt;
//	dM=dM>0?dM:(dM+PI2);
//	double dt=dM/n;
//
//	double tt=t;
//	double lx[6];
//	lx[0]=s.a;lx[1]=s.i;lx[2]=s.Omega;lx[3]=s.ksi;lx[4]=s.eta;lx[5]=s.lamda;
//	while(true)
//	{
//		Propagate2T(tt,h,s,dt,dd,out);
//	}
//}

//! ��ʼ�������ú���
void CRapidSatellite::Initialize(const CDateTime& t,const Kepler elem,const char flag)
{
	ElapsedTimems = 0;
	ElapsedTime = 0;
	m_lastpert = 0;
	CSatellite::Initialize(t,elem,flag);
	// �����ʼʱ�̵���ת�����㶯���ٶ�
	RefreshPert(Epoch);
}

void CRapidSatellite::RefreshPert( const CDateTime tt )
{
	mat33 ER = EarthRotation(tt);
	mat33 HG = PW*ER*NU*PR;
	vec3 ECFr =  HG * Position;  //J2000.0==>ECF  //�˴���Ϊ�����ǳ�ʼ��Ԫ��ֵ
	if(Perturbation & ODP_EARTH_ZONAL)
	{
		F_zonal = PZonalHarmonic(LL,ECFr);
		F_zonal = HG.t()*F_zonal;
		if(Perturbation & ODP_EARTH_TESSERAL)
		{
			F_tesseral = PTesseralHarmonic(LL,ECFr);
			F_tesseral = HG.t()*F_tesseral;
		}
	}
	rm = Moon::Instance()->GetECI(tt);
	rs = Sun::Instance()->GetECI(tt);
	// Lunar Gravitation
	if(Perturbation & ODP_LUNAR_CENT)
	{
		F_moon = PThirdBodyGrv(Position,rm,GM);
	}

	// solar gravity
	if( Perturbation & ODP_SOLAR_CENT )
	{
		F_sun = PThirdBodyGrv(Position,rs,GS);
	}

	// Sunlight Pressure
	if( Perturbation & ODP_SOLAR_PRESSURE )
	{
		F_light = PLightPressure(Cr*LightPressArea/(Mass0-BurnedFuel),Position,rs);
	}

	// Atmospheric Drag
	if( Perturbation & ODP_AIR_DRAG )
	{
		F_air = PAirDrag(tt,Cd*AirDragArea/(Mass0-BurnedFuel),ECFr,Position,Velocity);
	}

	// Post-Newtonian Effect
	if( Perturbation & ODP_POSTNEWTON )
	{
		F_ppn = PPostNewton(Position,Velocity);
	}
}
// hΪ���Ʋ�������λΪms
void CRapidSatellite::Propagate(int h)
{	
	if(FirstTimeOutput){
		if(Save) SaveElem();
		FirstTimeOutput = false;
	}

	OneStep(h);
	if(Save) SaveElem();
	if(fabs(ElapsedTime - PNTime) >= 1800.0)
	{
		RefreshMatrix(Epoch+PNTime);
		PNTime = ElapsedTime;
}
}

// StepSize��λΪms
int CRapidSatellite::OneStep(int StepSize,double MaxCutError /* = 1 */,CStopCondition* StopCon/* = NULL */)
{
	vec7 x,xh;
	double ee;
	x.subvec(0,2) = Position;
	x.subvec(3,5) = Velocity;
	x[6] = Mass();

	CDateTime tt = Epoch + ElapsedTimems/1000.0;

	// �����³�ʼ���Ժ�ElapsedTime��Ϊ0����m_lastpert����ԭ����ֵ�������㶯��û�и���
	//if(ElapsedTime>=m_lastpert+0.05-DBL_EPSILON��
	// ���ڸ�Ϊһ�����������䣬��֤ElapsedTime��m_lastpert�Ĳ��һ��ʱ����Ի�����㶯��
	if( fabs(double(ElapsedTimems - m_lastpert)) >= PeriodPert)
	{
		m_lastpert = ElapsedTimems;

		RefreshPert(tt);
	}

	// Thrust
	if(ThrustIsOn)
	{
		F_Thrust = trans(qbi.C()) * ThrustDirection * Engine->F/(Mass0-BurnedFuel)/1000.0;
	}

	//ee = RKF78(*this,StepSize,x,xh,7);
	ee = Gill4(*this,StepSize/1000.0,x,xh);

	if(StopCon == NULL)
	{
		ElapsedTimems += StepSize;
		ElapsedTime = ElapsedTimems/1000.0;
		Position = xh.subvec(0,2);
		Velocity = xh.subvec(3,5);
		BurnedFuel += Mass() - xh[6];
		RefreshStatus();
	}
	else
	{
		// ֹͣ�������ж�
	}

	vec7 y;
	DynFunction(ElapsedTime,xh,y); //ElapsedTimeʵ��û����
	acc = y.subvec(3,5);

	return 1;
}

/*! ����ѧ����
Input:  t:ʱ��   x[6]:״̬����
Output:    y[6]:״̬������΢��
*/
void CRapidSatellite::DynFunction(const double t,const vec& x,vec& y)
{
	vec3 F;
	F.zeros();

	if(Perturbation & ODP_EARTH_ZONAL)
	{
		F += F_zonal;
	}

	if(Perturbation & ODP_EARTH_TESSERAL)
	{
		F += F_tesseral;
	}

	// Lunar Gravitation
	if(Perturbation & ODP_LUNAR_CENT)
	{
		F += F_moon;
	}

	// solar gravity
	if( Perturbation & ODP_SOLAR_CENT )
	{
		F += F_sun;
	}

	// Sunlight Pressure
	if( Perturbation & ODP_SOLAR_PRESSURE )
	{
		F += F_light;
	}

	// Atmospheric Drag
	if( Perturbation & ODP_AIR_DRAG )
	{
		F += F_air;
	}

	// Post-Newtonian Effect
	if( Perturbation & ODP_POSTNEWTON )
	{
		F += F_ppn;
	}

	// Thrust
	if(ThrustIsOn)
	{
		F += F_Thrust;
	}

	// Perturbation Functions
	double R = sqrt(x[0]*x[0]+x[1]*x[1]+x[2]*x[2]);
	double miurrr = GE/R/R/R;
	y.subvec(0,2) = x.subvec(3,5);
	y.subvec(3,5) = -miurrr * x.subvec(0,2) + F;
	if( ThrustIsOn )
		y[6] = Engine->dm();
	else
		y[6] = 0;
}

/*!
ÿ������״̬Position��Velocity�仯ʱ���ô˺����������ǹ������
*/
void CRapidSatellite::RefreshStatus()
{
	Kepler newkepler = Cartesian_Kepler(Position,Velocity);
	a = newkepler.a;
	e = newkepler.e;
	i = newkepler.i;
	Omega = newkepler.o;
	w = newkepler.w;
	M = newkepler.M;

	E = newkepler.E();
	f = newkepler.f();
	u = fmod(f + w + PI2,PI2);
	n = newkepler.n();
	r = norm(Position,2);
	v = norm(Velocity,2);
	w0 = sqrt(GE/a/a/a)*(1+2*e*cos(f)+e*e*cos(f)*cos(f))/sqrt(pow(1-e*e,3));
	Coi = GetCoi(newkepler);
}

/*!
ʹ��Сƫ���ʹ��������Ϊ���ֱ�����RKF78��������汾ʹ�õķ���
ֻ�����еĵ�λ������������Ϊkm,s,rad
*/
int CLEO::OneStep(double StepSize,double MaxCutError /* = 1.0 */,CStopCondition* SC /* = NULL */)
{
	vec x,xh;
	double ee;
	// ���ֱ�������Ϊ a  i  Omega  ksi  eta  lamda
	x[0] = a;
	x[1] = i;
	x[2] = Omega;
	x[3] = e*cos(w);
	x[4] = e*sin(w);
	x[5] = fmod(w+M+PI2,PI2);
	if(ThrustIsOn)
	{
		x[6] = Mass();
		ee = RKF78(*this,StepSize,x,xh);
	}
	else
	{
		ee = RKF78(*this,StepSize,x,xh);
	}
	if(SC == NULL)
	{
		ElapsedTime += StepSize;
		xh[2] = fmod(xh[2]+PI2,PI2);
		xh[5] = fmod(xh[5]+PI2,PI2);
		a = xh[0];
		i = xh[1];
		Omega = xh[2];
		e = sqrt(xh[3]*xh[3]+xh[4]*xh[4]);
		w = fmod(atan2(xh[4],xh[3])+PI2,PI2);
		M = fmod(xh[5]-w+PI2,PI2);
		//... �����������������λ�á��ٶ�
		RefreshStatus();
		BurnedFuel += Mass() - xh[6];
	}
	else
	{

	}
	//if(MaxCutError>0.5) return 0;
	//if(ee>MaxCutError)  return StepSize = StepSize/2;
	//if(ee<MaxCutError*0.01) return StepSize*2;
	//return StepSize;
	return 1;
}
/*! �˺���ʵ��CSatelliteBase::RefreshKepler()��������
*/
void CLEO::RefreshStatus()
{
	Kepler k(a,e,i,Omega,w,M);
	Kepler_Cartesian(k,Position,Velocity);

	E = k.E();
	f = k.f();
	u = fmod(f + w + PI2,PI2);
	n = k.n();
	r = norm(Position,2);
	v = norm(Velocity,2);
}

/*!����ѧ����
t:ʱ��   x[6]:״̬����a  i  Omega  ksi  eta  lamda
y[6]:״̬������΢��
*/
void CLEO::DynFunction(const double t,const vec& x,vec& y)
{
	vec3 F;
	F.zeros();
	static vec3 rs,FG,ECFr,F12,r,rd,RR,TT,P,Q,S;
	// ����u
	double uE=KeplerFunc2(x[3],x[4],x[5]);
	double SuE=sin(uE);
	double CuE=cos(uE);
	double e2=x[3]*x[3]+x[4]*x[4];
	double GE2=sqrt(1.0-e2);
	double WE2=1.0+GE2;
	double ZuE=x[3]*CuE+x[4]*SuE;
	double FuE=x[3]*SuE-x[4]*CuE;
	double r0=x[0]*(1.0-ZuE); //ʸ��
	double Gp=sqrt(x[0]*(1.0-e2));
	double Sdu=x[0]/r0*FuE*(1.0-ZuE/WE2);
	double Cdu=sqrt(1.0-Sdu*Sdu);
	double du=atan2(Sdu,Cdu);
	double u=uE+du;
	// �������굥λʸ��
	double Su=sin(u);
	double Cu=cos(u);
	double Si=sin(x[1]);
	double Ci=cos(x[1]);
	double SQ=sin(x[2]);
	double CQ=cos(x[2]);
	P(0) = CQ;
	P(1) = SQ;
	P(2) = 0;
	Q(0) = -Ci*SQ;
	Q(1) = Ci*CQ;
	Q(2) = Si;
	RR=Cu*P+Su*Q;
	TT=-Su*P+Cu*Q;

	r = r0 * RR;
	rd = sqrt(GE)*Gp / r0 * (FuE/GE2*RR + TT);

	CDateTime tt = Epoch + t/86400;
	mat33 HG = PW*EarthRotation(tt)*NU*PR;;

	// Zonal Harmonic Terms and Tesseral Harmonic Terms
	ECFr = HG * r;	//J2000.0==>ECF  //�˴���Ϊ�����ǳ�ʼ��Ԫ��ֵ
	if(Perturbation & ODP_EARTH_TESSERAL) FG = PZonalHarmonic(LL,ECFr) + PTesseralHarmonic(LL,ECFr);
	else FG = PZonalHarmonic(LL,ECFr);
	F12 = HG.t() * FG; //ECF==>J2000.0   //�˴���Ϊ�����ǳ�ʼ��Ԫ��ֵ
	F = F + F12;

	// Lunar Gravitation
	if(Perturbation & ODP_LUNAR_CENT)
		F += PThirdBodyGrv( r , Moon::Instance()->GetECI(tt) , GM );

	// Solar Gravitation
	if( (Perturbation & ODP_SOLAR_CENT)  || (Perturbation & ODP_SOLAR_PRESSURE) || (Perturbation & ODP_AIR_DRAG) )
		rs = Sun::Instance()->GetECI(tt);   // rs:solar coodinate in J2000.0

	if( Perturbation & ODP_SOLAR_CENT )
		F += PThirdBodyGrv(r,rs,GS);

	// Sunlight Pressure
	if(Perturbation & ODP_SOLAR_PRESSURE)
		F += PLightPressure(Cr*LightPressArea/(Mass0-BurnedFuel),r,rs);

	// Atmospheric Drag
	if( Perturbation & ODP_AIR_DRAG )
	{
		F += PAirDrag(tt,Cd*AirDragArea/(Mass0-BurnedFuel),ECFr,r,rd);
	}

	// Post-Newtonian Effect
	if( Perturbation & ODP_POSTNEWTON )
		F += PPostNewton(r,rd);

	// Thrust
	if(ThrustIsOn)		
		F += trans(qbi.C())*(ThrustDirection*Engine->F/(Mass0-BurnedFuel)/1000.0);  //����ϵ�е��������ٶ�

	// All Perturbation Accelerations
	S = RotationZ(u)*RotationX(x[1])*RotationZ(x[2])*F;

	// Perturbation Functions
	double An = sqrt(GE/x[0]/x[0]/x[0]);
	double Ana = An * x[0];
	double Ana2 = Ana * x[0];
	double r0p = r0/(Gp*Gp);
	double CuSu = x[3]*Cu+x[4]*Su;
	double SuCu = x[3]*Su-x[4]*Cu;
	y[0] = 2.0/(An*GE2)*(S(0)*SuCu+S(1)/r0p);
	y[1] = r0*Cu*S(2)/(Ana2*GE2);
	y[2] = r0*Su*S(2)/(Ana2*GE2)/Si;
	y[3] = x[4]*Ci*y[2]+GE2/Ana*(S(0)*Su+S(1)/GE2*(CuE+GE2*Cu-x[3]*ZuE/WE2));
	y[4] = -x[3]*Ci*y[2]-GE2/Ana*(S(0)*Cu-S(1)/GE2*(SuE+GE2*Su-x[4]*ZuE/WE2));
	y[5] = An-Ci*y[2]-GE2/Ana*(2.0*S(0)*GE2*r0p+(S(0)*CuSu-S(1)*(1.0+r0p)*SuCu)/WE2);
	if( ThrustIsOn )
		y[6] = Engine->dm();
	else
		y[6] = 0;
}

/*! ��ʼ�����
\param elem ���Ĺ���ϵ�������a,e,i,O,w,M
*/
void CMoonSat::Initialize(const CDateTime& t,const Kepler elem)
{
	Epoch = t;
	Status0 = elem;
	a = Status0.a;
	e = Status0.e;
	i = Status0.i;
	Omega = Status0.o;
	w = Status0.w;
	M = Status0.M;
	E = Status0.E();

	f = Status0.f();
	u = fmod(f + w + PI2,PI2);
	n = Status0.n();

	Kepler_Cartesian(Status0,Position,Velocity,GM);

	CSatelliteBase::Initialize();
}

/*! ��ʼ�����
\param elem ���Ĺ���ϵ�������a,e,i,O,w,M
*/
void CMoonSat::Initialize(const CDateTime& t,const vec3 p,const vec3 v)
{
	CSatelliteBase::Initialize(t,p,v);
	RefreshStatus();
}

/*!
ÿ������״̬Position��Velocity�仯ʱ���ô˺����������ǹ������
*/
void CMoonSat::RefreshStatus()
{
	Kepler newkepler = Cartesian_Kepler(Position,Velocity,GM);
	a = newkepler.a;
	e = newkepler.e;
	i = newkepler.i;
	Omega = newkepler.o;
	w = newkepler.w;
	M = newkepler.M;

	E = newkepler.E();
	f = newkepler.f();
	u = fmod(f + w + PI2,PI2);
	n = newkepler.n(GM);
	r = norm(Position,2);
	v = norm(Velocity,2);
}

/*! ���¹������ѧ����
\param t ʱ��
\param x ״̬����
\param y ״̬������΢��
*/
void CMoonSat::DynFunction(const double t,const vec& x,vec& y)
{
	vec3 F,rs,FG,r,rd;
	F.zeros();

	r = x.subvec(0,2);
	rd = x.subvec(3,5);

	CDateTime tt = Epoch + t;

	// �������������
	mat33 MIF = MCI_MCF(tt); //���Ĺ���ϵ���¹�ϵת������
	vec3 SatMCF = MIF*r;
	if(Perturbation & ODP_LUNAR_TESSERAL)
		FG = PZonalHarmonic(LL,SatMCF,Moon::Instance()) + PTesseralHarmonic(LL,SatMCF,Moon::Instance());
	else if(Perturbation & ODP_LUNAR_ZONAL)
		FG = PZonalHarmonic(LL,SatMCF,Moon::Instance());
	else
		FG = vec3("0 0 0");

	F += MIF.t()*FG;

	vec3 Moon = Moon::Instance()->GetECI(tt);
	// earth gravity
	if(Perturbation & ODP_EARTH_CENT) //�������ODP_LUNAR��ʾ�Ƿ��ǵ������� 
	{	
		F += PThirdBodyGrv(r, -Moon , GE); // ������MCI��λ�ü�����ϵ����λ�õķ�����
	}

	// Solar Gravitation
	if( (Perturbation & ODP_SOLAR_CENT)  || (Perturbation & ODP_SOLAR_PRESSURE))
	{
		rs = Sun::Instance()->GetECI(tt);   // rs:solar coordinate in J2000.0
		rs -= Moon; // ���Ĺ���ϵ̫������
    }

	// solar gravity
	if( Perturbation & ODP_SOLAR_CENT )
	{
		F += PThirdBodyGrv(r,rs,GS);
	}

	// Sunlight Pressure
	if( Perturbation & ODP_SOLAR_PRESSURE )
	{		
		F += PLightPressure(Cr*LightPressArea/(Mass0-BurnedFuel),r,rs,Rm);
	}

	// Thrust
	if(ThrustIsOn)
	{
		F += trans(qbi.C()) * ThrustDirection * Engine->F/(Mass0-BurnedFuel)/1000.0;
	}

	// Perturbation Functions
	double R = norm(r,2);
	double miurrr = GM/R/R/R;
	y.subvec(0,2) = rd;
	y.subvec(3,5) = - miurrr * r + F;
	if( ThrustIsOn )
		y[6] = Engine->dm();
	else
		y[6] = 0;
}

/*!  �Զ�����������
�Զ�����ĸ�ʽΪ��\n
ʱ��t       �볤��a    ƫ����e    ���i    ������ྭOmega     ���ص����w     ƽ�����M      �������u
*/
void CMoonSat::SaveElem()
{
#define SPACE  "     "
	if(!FileInstElem.is_open())
	{
		std::string finst = Name + ".dat";
		const char * fi = finst.c_str();
		FileInstElem.open(fi,ios::out|ios::trunc);
	}
	FileInstElem.setf(ios::fixed);
	FileInstElem.precision(3);
	FileInstElem.width(11);
	FileInstElem<<ElapsedTime<<SPACE;
	FileInstElem.precision(5);
	FileInstElem.width(11);
	FileInstElem<<a<<SPACE;
	FileInstElem.width(11);
	FileInstElem.precision(9);
	FileInstElem<<e<<SPACE;
	FileInstElem.precision(8);
	FileInstElem.width(12);
	FileInstElem<<i*DEG<<SPACE;
	FileInstElem.width(12);
	FileInstElem<<Omega*DEG<<SPACE;
	FileInstElem.width(12);
	FileInstElem<<w*DEG<<SPACE;
	FileInstElem.width(12);
	FileInstElem<<M*DEG<<SPACE;
	FileInstElem.width(12);
	FileInstElem<<u*DEG<<endl;
#undef SPACE
}

