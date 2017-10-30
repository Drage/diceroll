
#ifndef Vector3_H
#define Vector3_H

/**
 * @brief	A 3-dimensional vector. 
 * @author	Matt Drage
 * @date	11/03/2012
 */
struct Vector3
{
	float x;
	float y;
	float z;
};
typedef struct Vector3 Vector3;

/**
 * @brief	Create a new vector from initial values.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	x	The x component.
 * @param	y	The y component.
 * @param	z	The z component.
 * @return	The new vector.
 */
Vector3 Vec3New(float x, float y, float z);

/**
 * @brief	Adds two vectors together.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	a	The first vector.
 * @param	b	The second vector.
 * @return	The result.
 */
Vector3 Vec3Add(Vector3 a, Vector3 b);

/**
 * @brief	Subtracts one vector from another.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	a	The first vector.
 * @param	b	The second vector.
 * @return	The result.
 */
Vector3 Vec3Sub(Vector3 a, Vector3 b);

/**
 * @brief	Multiplies a vector by a scalar.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	v	   	The vector.
 * @param	operand	The scalar.
 * @return	The result.
 */
Vector3 Vec3Mult(Vector3 v, float operand);

/**
 * @brief	Divides a vector by a scalar.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	v	   	The vector.
 * @param	operand	The scalar.
 * @return	The result.
 */
Vector3 Vec3Div(Vector3 v, float operand);

/**
 * @brief	Gets the Magnitude of a vector
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	v	The vector.
 * @return	The magnitude.
 */
float Vec3Magnitude(Vector3 v);

/**
 * @brief	Gets the Normalized version of a vector.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	v	The vector.
 * @return	The normalized vector.
 */
Vector3 Vec3Normalize(Vector3 v);

/**
 * @brief	Vector dot product.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	a	The first vector.
 * @param	b	The second vector.
 * @return	The dot product.
 */
float Vec3Dot(Vector3 a, Vector3 b);

/**
 * @brief	Vector cross product.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	a	The first vector.
 * @param	b	The second vector.
 * @return	The cross product.
 */
Vector3 Vec3Cross(Vector3 a, Vector3 b);

/**
 * @brief	Get a vector element by index.
 * @details Useful for loops.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	v		The vector.
 * @param	index	Zero-based index of the vector elements.
 * @return	The vector element.
 */
float Vec3GetElement(Vector3 *v, int index);

/**
 * @brief	Sets a vector element by index.
 * @details Useful for loops.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	v		The vector.
 * @param	index	Zero-based index of the vector elements.
 * @param	value	The value.
 */
 void Vec3SetElement(Vector3 *v, int index, float value);

#endif
