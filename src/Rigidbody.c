
#include "Rigidbody.h"
#include "MathUtils.h"

const float GRAVITY = 20;
const float LINEAR_DAMPING = 0.1f;
const float ANGULAR_DAMPING = 0.3f;
const float HORIZONTAL_FRICTION = 0.8f;
const float VERTICAL_FRICTION = 0.04f;
const float ANGULAR_FRICTION = 0.08f;
const float BOUNCE_FACTOR = 0.6f;
const float MASS_MULTIPLIER = 8.0f;

void RBInit(Rigidbody *rigidbody, Vector3 position, Vector3 dimensions, float density)
{
	float x, y, z;

	rigidbody->position = position;
	rigidbody->dimensions = dimensions;

	/* get half dimensions */
	x = dimensions.x / 2;
	y = dimensions.y / 2;
	z = dimensions.z / 2;

	/* init vertices */
	rigidbody->numVerts = 8;
	rigidbody->bodyVertices[0] = Vec3New(-x, -y,  z);
    rigidbody->bodyVertices[1] = Vec3New(-x, -y, -z);
    rigidbody->bodyVertices[2] = Vec3New( x, -y, -z);
    rigidbody->bodyVertices[3] = Vec3New( x, -y,  z);
    rigidbody->bodyVertices[4] = Vec3New(-x,  y,  z);
    rigidbody->bodyVertices[5] = Vec3New( x,  y,  z);
    rigidbody->bodyVertices[6] = Vec3New( x,  y, -z);
    rigidbody->bodyVertices[7] = Vec3New(-x,  y, -z);

	/* calc mass */
	rigidbody->mass = MASS_MULTIPLIER * density * x * y * z;

	/* calc inertia tensor */
	rigidbody->inverseBodyInertiaTensor.elements[0][0] = 3.0f / (rigidbody->mass * (y * y + z * z));
	rigidbody->inverseBodyInertiaTensor.elements[1][1] = 3.0f / (rigidbody->mass * (x * x + z * z));
	rigidbody->inverseBodyInertiaTensor.elements[2][2] = 3.0f / (rigidbody->mass * (x * x + y * y));

	/* bounce factor */
	rigidbody->coefficientOfRestitution = BOUNCE_FACTOR;

	/* set orientation to identity */
	rigidbody->orientation = M3New();
}

void RBCalculateVertices(Rigidbody *rigidbody)
{        
	unsigned i;
	Vector3 temp;

	for(i = 0; i < rigidbody->numVerts; i++)
    {
		temp = M3TransformVector(rigidbody->orientation, rigidbody->bodyVertices[i]);
        rigidbody->vertices[i] = Vec3Add(rigidbody->position, temp);
    }
}

void RBApplyForces(Rigidbody *rigidbody)
{
	float radius;

	/* clear last updates forces */
	rigidbody->torque = Vec3New(0, 0, 0);
	rigidbody->force = Vec3New(0, 0, 0);

	/* apply gravity */
	rigidbody->force.y -= GRAVITY * rigidbody->mass;

	/* apply damping (air resistance) */
	rigidbody->force = Vec3Add(rigidbody->force, Vec3Mult(rigidbody->velocity, -LINEAR_DAMPING));
	rigidbody->torque = Vec3Add(rigidbody->torque, Vec3Mult(rigidbody->angularVelocity, -ANGULAR_DAMPING));

	/* apply friction if touching floor */
	radius = Min(Min(rigidbody->dimensions.x, rigidbody->dimensions.y), rigidbody->dimensions.z) / 2;
	if (rigidbody->position.y <= radius + 0.003f)
	{
		rigidbody->force.x *= HORIZONTAL_FRICTION;
		rigidbody->force.z *= HORIZONTAL_FRICTION;
		rigidbody->force.y *= VERTICAL_FRICTION;

		rigidbody->velocity.x *= HORIZONTAL_FRICTION;
		rigidbody->velocity.z *= HORIZONTAL_FRICTION;
		rigidbody->velocity.y *= VERTICAL_FRICTION;

		rigidbody->torque.y *= ANGULAR_FRICTION;
		rigidbody->angularVelocity.y *= ANGULAR_FRICTION;
	}
}

void RBIntegrate(Rigidbody *rigidbody, float deltaTime)
{
	Vector3 newPosition, newVelocity, newAngularMomentum, newAngularVelocity;
	Matrix3x3 newOrientation, newInverseWorldInertiaTensor;

	/* calculate new values by Euler intergration */
	newPosition = Vec3Add(rigidbody->position, Vec3Mult(rigidbody->velocity, deltaTime));

	newOrientation = M3Add(rigidbody->orientation, M3Scale(M3Mult(M3SkewSymetricFromVector(rigidbody->angularVelocity), rigidbody->orientation), deltaTime));
	newOrientation = M3Orthonormalize(newOrientation);

	newVelocity = Vec3Add(rigidbody->velocity, Vec3Mult(rigidbody->force, deltaTime / rigidbody->mass));

	newAngularMomentum = Vec3Add(rigidbody->angularMomentum, Vec3Mult(rigidbody->torque, deltaTime));

	newInverseWorldInertiaTensor = M3Mult(M3Mult(newOrientation, rigidbody->inverseBodyInertiaTensor), M3Transpose(newOrientation));

	newAngularVelocity = M3TransformVector(newInverseWorldInertiaTensor, newAngularMomentum);

	/* update parameters */
	rigidbody->position = newPosition;
	rigidbody->orientation = newOrientation;
	rigidbody->velocity = newVelocity;
	rigidbody->angularMomentum = newAngularMomentum;
	rigidbody->inverseWorldInertiaTensor = newInverseWorldInertiaTensor;
	rigidbody->angularVelocity = newAngularVelocity;
}

void RBCheckCollisionFloor(Rigidbody *rigidbody)
{
    float const depthEpsilon = 0.001f;
	unsigned i;
	Vector3 offset, velocity;
	float projection, relativeVelocity;
	Vector3 floorNormal = Vec3Normalize(Vec3New(0, 1, 0));

	rigidbody->Collision.state = NO_COLLISION;

	/* loop through each vertex */
    for (i = 0; i < rigidbody->numVerts && rigidbody->Collision.state == NO_COLLISION; i++)
    {
		/* get distance between vertex and ground plane */
		projection = Vec3Dot(rigidbody->vertices[i], floorNormal);
        
        if (projection < -depthEpsilon)
        {
            rigidbody->Collision.state = PENETRATING;
        }
        else if (projection < depthEpsilon)
        {
			/* check if moving down towards floor */
			offset = Vec3Sub(rigidbody->vertices[i], rigidbody->position);
			velocity = Vec3Add(rigidbody->velocity, Vec3Cross(rigidbody->angularVelocity, offset));
            relativeVelocity = Vec3Dot(floorNormal, velocity);

            if (relativeVelocity < 0)
            {
				/* handle collision response */
                rigidbody->Collision.state = COLLIDING;
                rigidbody->Collision.normal = floorNormal;
                rigidbody->Collision.contactPoint = rigidbody->vertices[i];
            }
        }
    }
}

void RBResolveCollisions(Rigidbody *rigidbody)
{
	Vector3 position = rigidbody->Collision.contactPoint;
	Vector3 offset = Vec3Sub(position, rigidbody->position);
	Vector3 velocity = Vec3Add(rigidbody->velocity, Vec3Cross(rigidbody->angularVelocity, offset));

	/* calculate impulse */
	float impulseNumerator = -(1.0f + rigidbody->coefficientOfRestitution) * Vec3Dot(velocity, rigidbody->Collision.normal);
	float impulseDenominator = (1.0f / rigidbody->mass) + Vec3Dot(Vec3Cross(M3TransformVector(rigidbody->inverseWorldInertiaTensor, Vec3Cross(offset, rigidbody->Collision.normal)), offset), rigidbody->Collision.normal);

	/* apply impulse */
	Vector3 impulse = Vec3Mult(rigidbody->Collision.normal, (impulseNumerator/impulseDenominator));
	rigidbody->velocity = Vec3Add(rigidbody->velocity, Vec3Mult(impulse, 1.0f / rigidbody->mass));
	rigidbody->angularMomentum = Vec3Add(rigidbody->angularMomentum, Vec3Cross(offset, impulse));
	rigidbody->angularVelocity = M3TransformVector(rigidbody->inverseWorldInertiaTensor, rigidbody->angularMomentum);
}

void RBMoveOutOfFloor(Rigidbody *rigidbody)
{
	unsigned i;
	float projection, maxProjection = 1;
	Vector3 floorNormal = Vec3Normalize(Vec3New(0, 1, 0));

	/* find max penetration */
    for (i = 0; i < rigidbody->numVerts; i++)
    {
		projection = Vec3Dot(rigidbody->vertices[i], floorNormal);
		
		if (projection < maxProjection)
			maxProjection = projection;
    }

	/* move outside if penetrating */
	if (maxProjection < 0)
		rigidbody->position = Vec3Add(rigidbody->position, Vec3Mult(floorNormal, -maxProjection));
}

void RBMoveOutOfBody(Rigidbody *rigidbody, Rigidbody *other)
{
	unsigned i;
	Vector3 normal;
	float penetrationDistance;
	float radius, distance;
	Vector3 direction;

	/* get smallest dimension of body */
	radius = Min(Min(rigidbody->dimensions.x, rigidbody->dimensions.y), rigidbody->dimensions.z);

	/* get distance between rigidbody centers */
	distance = Vec3Magnitude(Vec3Sub(rigidbody->position, other->position));

	if (distance < radius / 2)
	{
		/* objects are too close, move away */
		direction = Vec3Normalize(Vec3Sub(rigidbody->position, other->position));
		rigidbody->position = Vec3Add(rigidbody->position, Vec3Mult(direction, -(radius - distance)));
	}

	/* Check verts for penetration */
	for (i = 0; i < rigidbody->numVerts; i++)
    {
		if (RBCheckCollisionPoint(other, rigidbody->vertices[i], &normal, &penetrationDistance))
			rigidbody->position = Vec3Add(rigidbody->position, Vec3Mult(normal, -penetrationDistance));
	}
	for (i = 0; i < rigidbody->numVerts; i++)
    {
		if (RBCheckCollisionPoint(rigidbody, other->vertices[i], &normal, &penetrationDistance))
			rigidbody->position = Vec3Add(rigidbody->position, Vec3Mult(Vec3New(-normal.x, -normal.y, -normal.z), -penetrationDistance));
	}
}

Vector3 RBGetNormal(Vector3 v1, Vector3 v2, Vector3 v3)
{
	Vector3 side1, side2, normal;
	side1 = Vec3Sub(v2, v1);
	side2  = Vec3Sub(v3, v1);
	normal = Vec3Cross(side1, side2);
	normal = Vec3Normalize(normal);      
	return normal;
}

float RBGetNormalProjection(Vector3 point, Vector3 v1, Vector3 v2, Vector3 v3)
{
	Vector3 normal, vect;
	normal = RBGetNormal(v1, v2, v3);
	vect = Vec3Sub(point, v1);
	return Vec3Dot(vect, normal);
}

bool RBCheckCollisionPoint(Rigidbody *rigidbody, Vector3 point, Vector3 *normal, float *penetrationDistance)
{
	float projection;
	int i;
	int hitFace;
	float minProjection = 1;
	float const depthEpsilon = 0.001f;

	/* Clockwise winding */
	int indexes[6][3] =  {{ 7, 6, 2 },	/* Front */
						  { 5, 4, 0 },	/* Back */
						  { 4, 5, 6 },	/* Top */
						  { 1, 2, 3 },	/* Bottom */
						  { 4, 7, 1 },	/* Left */
						  { 6, 5, 3 }};	/* Right */

	for (i = 0; i < 6; i++)
	{
		projection = RBGetNormalProjection(point, rigidbody->vertices[indexes[i][0]], rigidbody->vertices[indexes[i][1]], rigidbody->vertices[indexes[i][2]]);

		if (projection >= 0)
			return false; /* Found a seperating axis */
		else
		{
			if (projection > minProjection || minProjection == 1)
			{
				minProjection = projection;
				hitFace = i;
			}
		}
	}

	*penetrationDistance = minProjection;
	*normal = RBGetNormal(rigidbody->vertices[indexes[hitFace][0]], rigidbody->vertices[indexes[hitFace][1]], rigidbody->vertices[indexes[hitFace][2]]);

	return true;
}

void RBCheckCollisionBody(Rigidbody *rigidbody, Rigidbody *other)
{
	unsigned i;
	bool result;
	Vector3 normal;
	float penetrationDistance;

	/* test rigidbody verts against other */
	for (i = 0; i < rigidbody->numVerts; i++)
    {
		result = RBCheckCollisionPoint(other, rigidbody->vertices[i], &normal, &penetrationDistance);

		if (result)
		{
			rigidbody->Collision.normal = normal;
			rigidbody->Collision.contactPoint = rigidbody->vertices[i];
			rigidbody->Collision.state = COLLIDING;
			rigidbody->position = Vec3Add(rigidbody->position, Vec3Mult(rigidbody->Collision.normal, -penetrationDistance));
			return;
		}
	}

	/* test other verts against rigidbody */
	for (i = 0; i < rigidbody->numVerts; i++)
    {
		result = RBCheckCollisionPoint(rigidbody, other->vertices[i], &normal, &penetrationDistance);

		if (result)
		{
			rigidbody->Collision.normal = Vec3New(-normal.x, -normal.y, -normal.z);
			rigidbody->Collision.contactPoint = other->vertices[i];
			rigidbody->Collision.state = COLLIDING;
			rigidbody->position = Vec3Add(rigidbody->position, Vec3Mult(rigidbody->Collision.normal, -penetrationDistance));
			return;
		}
	}
}
