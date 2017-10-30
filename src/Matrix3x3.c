
#include "Matrix3x3.h"
#include "MathUtils.h"
#include <math.h>

Matrix3x3 M3New()
{
	Matrix3x3 m;
	int i, j;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			m.elements[i][j] = 0;

    m.elements[0][0] = 1;
	m.elements[1][1] = 1;
	m.elements[2][2] = 1;

	return m;
}

Vector3 M3TransformVector(Matrix3x3 m, Vector3 v)
{
	Vector3 result = Vec3New(0, 0, 0);
	int i, j;
	float value;

	for (i = 0; i < 3; i++)
    {
        value = 0;
      
        for (j = 0; j < 3; j++)
            value += m.elements[i][j] * Vec3GetElement(&v, j);

		Vec3SetElement(&result, i, value);
    }

	return result;
}

Matrix3x3 M3Mult(Matrix3x3 a, Matrix3x3 b)
{
	Matrix3x3 result;
	int i, j, k;
	float value;

	for (i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            value = 0;
          
            for (k = 0; k < 3; k++)
                value += a.elements[i][k] * b.elements[k][j];

			result.elements[i][j] = value;
        }
    }

	return result;
}

Matrix3x3 M3SkewSymetricFromVector(Vector3 v)
{
	Matrix3x3 m;

	m.elements[0][0] = 0;		m.elements[0][1] = -v.z;	m.elements[0][2] = v.y;
	m.elements[1][0] = v.z;		m.elements[1][1] = 0;		m.elements[1][2] = -v.x;
	m.elements[2][0] = -v.y;	m.elements[2][1] =  v.x;	m.elements[2][2] = 0;

	return m;
}

Matrix3x3 M3Scale(Matrix3x3 m, float operand)
{
	int i, j;
	
	for (i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
			m.elements[i][j] *= operand;

	return m;
}

Matrix3x3 M3Add(Matrix3x3 a, Matrix3x3 b)
{
	int i, j;
	
	for (i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
			a.elements[i][j] += b.elements[i][j];

	return a;
}

Matrix3x3 M3Orthonormalize(Matrix3x3 m)
{
	Vector3 x, y, z;

	x = Vec3New(m.elements[0][0], m.elements[1][0], m.elements[2][0]);
	y = Vec3New(m.elements[0][1], m.elements[1][1], m.elements[2][1]);

    x = Vec3Normalize(x);
	z = Vec3Normalize(Vec3Cross(x, y));
    y = Vec3Normalize(Vec3Cross(z, x));

	m.elements[0][0] = x.x;		m.elements[0][1] = y.x;		m.elements[0][2] = z.x;
	m.elements[1][0] = x.y;		m.elements[1][1] = y.y;		m.elements[1][2] = z.y;
	m.elements[2][0] = x.z;		m.elements[2][1] = y.z;		m.elements[2][2] = z.z;

	return m;
}

Matrix3x3 M3Transpose(Matrix3x3 m)
{
	Matrix3x3 transposed;
	int i, j;

    for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			transposed.elements[j][i] = m.elements[i][j];
        
    return transposed;
}

Matrix3x3 M3FromEuler(Vector3 euler)
{
	Matrix3x3 X, Y, Z;
	float x, y, z;

	x = DegToRad(euler.x);
	y = DegToRad(euler.y);
	z = DegToRad(euler.z);

	Y.elements[0][0] = (float)cos(y);	Y.elements[0][1] = 0;				Y.elements[0][2] = (float)sin(y);
	Y.elements[1][0] = 0;				Y.elements[1][1] = 1;				Y.elements[1][2] = 0;
	Y.elements[2][0] = (float)-sin(y);	Y.elements[2][1] = 0;				Y.elements[2][2] = (float)cos(y);

	X.elements[0][0] = 1;				X.elements[0][1] = 0;				X.elements[0][2] = 0;
	X.elements[1][0] = 0;				X.elements[1][1] = (float)cos(x);	X.elements[1][2] = (float)-sin(x);
	X.elements[2][0] = 0;				X.elements[2][1] = (float)sin(x);	X.elements[2][2] = (float)cos(x);

	Z.elements[0][0] = (float)cos(z);	Z.elements[0][1] = (float)-sin(z);	Z.elements[0][2] = 0;
	Z.elements[1][0] = (float)sin(z);	Z.elements[1][1] = (float)cos(z);	Z.elements[1][2] = 0;
	Z.elements[2][0] = 0;				Z.elements[2][1] = 0;				Z.elements[2][2] = 1;

	return M3Mult(M3Mult(Y, X), Z);
}
