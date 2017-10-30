
/**
 * @file	MathUtils.h
 * @brief	Declares mathematics utilities functions.
 */

#ifndef MATHUTILS_H
#define MATHUTILS_H

const float PI;

/**
 * @brief	Converts degrees to radians.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	degrees	The degrees.
 * @return	The angle in radians.
 */
float DegToRad(float degrees);

/**
 * @brief	Converts radians to degrees.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	radians	The radians.
 * @return	The angle in degrees.
 */
float RadToDeg(float radians);

/**
 * @brief	Initialises the random generation seed.
 * @author	Matt Drage
 * @date	11/03/2012
 */
void InitRandomGeneration();

/**
 * @brief	Gets a random integer between min and max.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	min	The minimum.
 * @param	max	The maximum.
 * @return	The random int.
 */
int GetRandomInt(int min, int max);

/**
 * @brief	Gets a random float between min and max.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	min	The minimum.
 * @param	max	The maximum.
 * @return	The random float.
 */
float GetRandomFloat(float min, float max);

/**
 * @brief	Determines the minimum of the given parameters.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	a	The first parameter to compare.
 * @param	b	The second parameter to compare.
 * @return	The minimum value.
 */
float Min(float a, float b);

/**
 * @brief	Determines the maximum of the given parameters.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	a	The first parameter to compare.
 * @param	b	The second parameter to compare.
 * @return	The maximum value.
 */
float Max(float a, float b);

#endif
