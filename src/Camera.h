
#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"

/**
 * @brief	Handles camera orientation and movement. 
 * @author	Matt Drage
 * @date	11/03/2012
 */
struct Camera
{
	Vector3 position;
	Vector3 focus;
	float speed;
	float orbitRadius;
	float orbitAngle;
	float minY;
	float maxY;
};
typedef struct Camera Camera;

/**
 * @brief	Camera update.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	camera			The camera to update.
 * @param	deltaTime	  	Time elapsed from last update.
 */
void CameraUpdate(Camera *camera, float deltaTime);

#endif
