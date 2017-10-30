
#ifndef MATRIX3X3_H
#define MATRIX3X3_H

#include "Vector3.h"

/**
 * @brief	Matrix of size 3x3.
 * @details	Matrix is row major.
 * @author	Matt Drage
 * @date	11/03/2012
 */
struct Matrix3x3
{
	float elements[3][3];
};
typedef struct Matrix3x3 Matrix3x3;

/**
 * @brief	Creates a new matrix set to the identity.
 * @author	Matt Drage
 * @date	11/03/2012
 * @return	the Identity matrix.
 */
Matrix3x3 M3New();

/**
 * @brief	Transforms a vector by a matrix.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	m	The transformation matrix.
 * @param	v	The vector to transform.
 * @return	The transformed vector.
 */
Vector3 M3TransformVector(Matrix3x3 m, Vector3 v);

/**
 * @brief	Multiplys two matrixes.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	a	The first matrix.
 * @param	b	The second matrix.
 * @return	The result of the multiplication.
 */
Matrix3x3 M3Mult(Matrix3x3 a, Matrix3x3 b);

/**
 * @brief	Creates a skew symetric matrix from a vector.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	v	The vector.
 * @return	A skew-symetric matrix.
 */
Matrix3x3 M3SkewSymetricFromVector(Vector3 v);

/**
 * @brief	Scales all matrix elements by a scalar.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	m	   	The matrix.
 * @param	operand	The scalar.
 * @return	The scaled matrix.
 */
Matrix3x3 M3Scale(Matrix3x3 m, float operand);

/**
 * @brief	Adds two matrices.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	a	The first matrix.
 * @param	b	The second matrix.
 * @return	The result of the addition.
 */
Matrix3x3 M3Add(Matrix3x3 a, Matrix3x3 b);

/**
 * @brief	Gets the orthonormalized version of a matrix.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	m	The matrix to find the orthonormal of.
 * @return	An orthonormlaized matrix.
 */
Matrix3x3 M3Orthonormalize(Matrix3x3 m);

/**
 * @brief	Gets the transpose of a matrix.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	m	The matrix to transpose.
 * @return	The transposed matrix.
 */
Matrix3x3 M3Transpose(Matrix3x3 m);

/**
 * @brief	Creates an orientation matrix from a set of euler angles.
 * @details	Assumes euler angles are in degrees.
 * 			Constructs the rotation matrix in YXZ order.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	euler	The euler angles (degrees).
 * @return	The rotation matrix.
 */
Matrix3x3 M3FromEuler(Vector3 euler);

#endif
