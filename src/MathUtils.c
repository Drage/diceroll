
#include "MathUtils.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

const float PI = 3.14159265f;

float DegToRad(float degrees)
{
	return degrees / 180.0f * PI;
}

float RadToDeg(float radians)
{
	return radians * 180.0f / PI;
}

void InitRandomGeneration()
{
	srand((unsigned)time(NULL));
}

int GetRandomInt(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

float GetRandomFloat(float min, float max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return r * (max - min) + min;
}

float Min(float a, float b)
{
	return a < b ? a : b;
}

float Max(float a, float b)
{
	return a > b ? a : b;
}
