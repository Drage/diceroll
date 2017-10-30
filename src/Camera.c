
#include "Camera.h"
#include "KeyInput.h"
#include "MathUtils.h"
#include <math.h>

void CameraUpdate(Camera *camera, float deltaTime)
{
	float deltaAngle = 0;
	float deltaY = 0;

	/* handle input */
	if (KeyDown(KEY_RIGHT) || KeyDown(KEY_D))
		deltaAngle = -1;
	else if (KeyDown(KEY_LEFT) || KeyDown(KEY_A))
		deltaAngle = 1;

	if (KeyDown(KEY_UP) || KeyDown(KEY_W))
		deltaY = 1;
	else if (KeyDown(KEY_DOWN) || KeyDown(KEY_S))
		deltaY = -1;

	/* orbit horizontally */
	if (deltaAngle != 0)
	{
		deltaAngle *= camera->speed * deltaTime;

		camera->orbitAngle += DegToRad(deltaAngle);
		camera->position.x = camera->orbitRadius * (float)cos(camera->orbitAngle);
		camera->position.z = camera->orbitRadius * (float)sin(camera->orbitAngle);
	}

	/* move up and down */
	if (deltaY != 0)
	{
		deltaY *= camera->speed * deltaTime * 0.15f;
		camera->position.y += deltaY;

		/* constrain y position */
		if (camera->position.y < camera->minY)
			camera->position.y = camera->minY;
		else if (camera->position.y > camera->maxY)
			camera->position.y = camera->maxY;
	}
}
