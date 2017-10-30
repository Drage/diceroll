
#include "Vector3.h"
#include <math.h>
#include <stdlib.h>

Vector3 Vec3New(float x, float y, float z)
{
	Vector3 newVec3;
	newVec3.x = x;
	newVec3.y = y;
	newVec3.z = z;
	return newVec3;
}

Vector3 Vec3Add(Vector3 a, Vector3 b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
    return a;
}

Vector3 Vec3Sub(Vector3 a, Vector3 b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
    return a;
}

Vector3 Vec3Mult(Vector3 v, float operand)
{
	v.x *= operand;
	v.y *= operand;
	v.z *= operand;
    return v;
}

Vector3 Vec3Div(Vector3 v, float operand)
{
	v.x /= operand;
	v.y /= operand;
	v.z /= operand;
    return v;
}

float Vec3Magnitude(Vector3 v)
{
	return (float)sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

Vector3 Vec3Normalize(Vector3 v)
{
	float magnitude = Vec3Magnitude(v);

	if (magnitude > 0)
	{
		v.x /= magnitude;
		v.y /= magnitude;
		v.z /= magnitude;
	}

	return v;
}

float Vec3Dot(Vector3 a, Vector3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Vec3Cross(Vector3 a, Vector3 b)
{
	float x, y, z;

	x = a.y * b.z - a.z * b.y;
	y = a.z * b.x - a.x * b.z;
	z = a.x * b.y - a.y * b.x;
	
	a.x = x;
	a.y = y;
	a.z = z;

	return a;
}

float Vec3GetElement(Vector3 *v, int index)
{
	switch (index)
	{
		case 0:
			return v->x;
		case 1:
			return v->y;
		case 2:
			return v->z;
		default:
			return 0.0f;
	}
}

void Vec3SetElement(Vector3 *v, int index, float value)
{
	switch (index)
	{
		case 0:
			v->x = value;
			break;
		case 1:
			v->y = value;
			break;
		case 2:
			v->z = value;
			break;
	}
}
