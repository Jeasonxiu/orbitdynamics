#include "ublas.h"

Vector Cross(const Vector &v1, const Vector &v2)
{
	Vector vo;
	vo[0] = v1[1]*v2[2] - v1[2]*v2[1];
	vo[1] = v1[2]*v2[0] - v1[0]*v2[2];
	vo[2] = v1[0]*v2[1] - v1[1]*v2[0];
	return vo;
}


void SetVectorValue(Vector& v, double d1,double d2,double d3)
{
	v[0] = d1;
	v[1] = d2;
	v[2] = d3;
}

ORBITDYN_API void SetVectorValue( Vector& v, double *da )
{
	v[0] = da[0];
	v[1] = da[1];
	v[2] = da[2];
}

/*!��X��ת��angle����ת����
[ 1          0         0     ]
[ 0    cos(angle)  sin(angle)]
[ 0   -sin(angle)  cos(angle)]
\param angle ת���Ľ�(����)
\return X��ת����
*/
CosineMatrix RotationX(double angle)
{
	CosineMatrix m;
	m.clear();
	m(0,0) = 1.0;
	m(1,1) = cos(angle);
	m(2,2) = m(1,1);
	m(1,2) = sin(angle);
	m(2,1) = -m(1,2);
	return m;
}
/*!��Y��ת��angle����ת����
[cos(angle)  0    -sin(angle)]
[     0      1         0     ]
[sin(angle)  0     cos(angle)]
\param angle ת���Ľ�(����)
\return Y��ת����
*/
CosineMatrix RotationY(double angle)
{
	CosineMatrix m;
	m.clear();
	m(0,0) = cos(angle);
	m(1,1) = 1.0;
	m(2,2) = m(0,0);
	m(0,2) = -sin(angle);
	m(2,0) = -m(0,2);
	return m;
}
/*!��Z��ת��angle����ת����
[ cos(angle)  sin(angle)  0]
[-sin(angle)  cos(angle)  0]
[     0            0      1]
\param angle ת���Ľ�(����)
\return Z��ת����
*/
CosineMatrix RotationZ(double angle)
{
	CosineMatrix m;
	m.clear();
	m(0,0) = cos(angle);
	m(1,1) = m(0,0);
	m(2,2) = 1.0;
	m(0,1) = sin(angle);
	m(1,0) = -m(0,1);
	return m;
}
// ����ת��
CosineMatrix operator~(const CosineMatrix& m)
{
	return trans(m);
}


// ����˷�
CosineMatrix operator*(const CosineMatrix& m,const CosineMatrix& n)
{
	return prod(m,n);
}

// �����ʸ��
Vector operator*(const CosineMatrix& m,const Vector& n)
{
	return prod(m,n);
}
// ����ʸ��
ORBITDYN_API Vector MakeVector( double x,double y,double z )
{
	Vector v;
	v[0] = x;
	v[1] = y;
	v[2] = z;
	return v;
}

// ����ʸ��
ORBITDYN_API Vector MakeVector( double* d )
{
	Vector v;
	v[0] = d[0];
	v[1] = d[1];
	v[2] = d[2];
	return v;
}
