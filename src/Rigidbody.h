
#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Matrix3x3.h"
#include "Vector3.h"
#include "Boolean.h"

/**
 * @brief	Defines the maximum amount of verticies a rigidbody can have. 
 */
enum { MAX_VERTS = 8 }; /* const int MAX_VERTS = 8; doesnt work, can't declare array with const size, only #define or enum */

extern const float GRAVITY;
extern const float LINEAR_DAMPING;
extern const float ANGULAR_DAMPING;
extern const float HORIZONTAL_FRICTION;
extern const float VERTICAL_FRICTION;
extern const float ANGULAR_FRICTION;

/**
 * @brief	Values that represent collision states. 
 */
enum CollisionState { NO_COLLISION, COLLIDING, PENETRATING };
typedef enum CollisionState CollisionState;

/**
 * @brief	Rigidbody. 
 * @author	Matt Drage
 * @date	11/03/2012
 */
struct Rigidbody
{
	float mass;
	Matrix3x3 inverseBodyInertiaTensor;		/* initial resistance to changes in rotation */
	float coefficientOfRestitution;			/* collision 'bounce' amount */

	unsigned numVerts;
	Vector3 bodyVertices[MAX_VERTS];		/* vertices before transformation */
	Vector3 vertices[MAX_VERTS];			/* transformed vertices */

	Vector3 dimensions;

	Vector3 position;
	Matrix3x3 orientation;
	Vector3 velocity;
	Vector3 angularMomentum;
	Vector3 force;							/* linear force */
	Vector3 torque;							/* rotational force */

	Matrix3x3 inverseWorldInertiaTensor;	/* current resistance to changes in rotation */
	Vector3 angularVelocity;				/* speed of rotation in each axis */

	struct CollisionType
	{
		CollisionState state;				/* currently colliding, penetrating, or no collisions */
		Vector3 normal;						/* normal of collision plane */
		Vector3 contactPoint;				/* point of contact */
	} Collision;	
};
typedef struct Rigidbody Rigidbody;

/**
 * @brief	Initialises a rigidbody.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	rigidbody		The rigidbody.
 * @param	position		The position.
 * @param	dimensions		The dimensions.
 * @param	density			The density.
 */
void RBInit(Rigidbody *rigidbody, Vector3 position, Vector3 dimensions, float density);

/**
 * @brief	Calculates a rigidbodys transformed vertices.
 * @detaisl	Call after moving and before checking collisions
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	rigidbody	The rigidbody.
 */
void RBCalculateVertices(Rigidbody *rigidbody);

/**
 * @brief	Applys forces to a rigidbody. eg. gravity.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	rigidbody	The rigidbody.
 */
void RBApplyForces(Rigidbody *rigidbody);

/**
 * @brief	Integrates the rigidbodys parameters with respect to time.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	rigidbody	The rigidbody.
 * @param	deltaTime	Time period to integrate over.
 */
void RBIntegrate(Rigidbody *rigidbody, float deltaTime);

/**
 * @brief	Checks rigidbody for collision with the floor.
 * @details	Foor is poisitioned at y = 0
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	rigidbody	The rigidbody.
 */
void RBCheckCollisionFloor(Rigidbody *rigidbody);

/**
 * @brief	Resolves rigidbody collisions using impulse forces.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	rigidbody	The rigidbody.
 */
void RBResolveCollisions(Rigidbody *rigidbody);

/**
 * @brief	Force the rigidbody to move out of the floor.
 * @details	Not strictly physically accurate. Call to ensure rigidbody does not get stuck.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	rigidbody	The rigidbody.
 */
void RBMoveOutOfFloor(Rigidbody *rigidbody);

/**
 * @brief	Move rigidbody out of other rigidbody.
 * @details	Not strictly physically accurate. Call to ensure rigidbody does not get stuck.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	rigidbody	The rigidbody.
 * @param	other	 	The other rigidbody.
 */
void RBMoveOutOfBody(Rigidbody *rigidbody, Rigidbody *other);

/**
 * @brief	Gets the normal of a plane defined by three vertices.
 * @detials	Used internally by RBGetNormalProjection.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	v1	The first vertex.
 * @param	v2	The second vertex.
 * @param	v3	The third vertex.
 * @return	The normal of the plane.
 */
Vector3 RBGetNormal(Vector3 v1, Vector3 v2, Vector3 v3);

/**
 * @brief	Get the projection of a point from a plane defined by three vertices.
 * @detials	Used internally by RBCheckCollisionPoint.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	point	The point.
 * @param	v1   	The first vertex.
 * @param	v2   	The second vertex.
 * @param	v3   	The third vertex.
 * @return	The projection.
 */
float RBGetNormalProjection(Vector3 point, Vector3 v1, Vector3 v2, Vector3 v3);

/**
 * @brief	Checks whether a rigidbody is colliding with a point.
 * @detials	Used internally by RBCheckCollisionBody.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	rigidbody		   	The rigidbody.
 * @param	point				The point.
 * @param	normal			   	The normal.
 * @param 	penetrationDistance	The penetration distance.
 * @return	true if collision, false if not.
 */
bool RBCheckCollisionPoint(Rigidbody *rigidbody, Vector3 point, Vector3 *normal, float *penetrationDistance);

/**
 * @brief	Checks for collision between two rigidbodys.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	rigidbody	The rigidbody.
 * @param 	other	 	The other rigidbody.
 */
void RBCheckCollisionBody(Rigidbody *rigidbody, Rigidbody *other);

#endif
