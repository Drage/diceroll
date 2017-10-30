
#include "Scene.h"
#include "Rigidbody.h"
#include <stdlib.h>
#include "Boolean.h"
#include "KeyInput.h"
#include "MathUtils.h"
#include <assert.h>
#include <stdio.h>

void SceneInit(Scene *scene)
{
	/* init camera */
	scene->camera.position = Vec3New(10, 3, 0);
	scene->camera.focus = Vec3New(0, 0, 0);
	scene->camera.speed = 80;
	scene->camera.orbitAngle = 0;
	scene->camera.orbitRadius = 10;
	scene->camera.minY = 3;
	scene->camera.maxY = 10;

	/* init light */
	scene->light.enabled = true;
	scene->light.type = DIRECTIONAL_LIGHT;
	scene->light.position = Vec3New(1, 1, 0);
	scene->light.ambient = ColourNew(0.5f, 0.5f, 0.5f, 1);
	scene->light.diffuse = ColourNew(1, 1, 1, 1);
	scene->light.specular = ColourNew(1, 1, 1, 1);

	/* init input control */
	scene->createdObjects = false;
	scene->increasedNumObjects = false;
	scene->decreasedNumObjects = false;

	/* init objects */
	scene->numObjectsCreate = 8;
	SceneCreateRigidbodys(scene);
}

void SceneUpdate(Scene *scene, float deltaTime)
{
	int timeDivisionsCount;
	int maxTimeDivisions = 20;

	float currentTime;
	float targetTime = deltaTime;

	Rigidbody object;

	int i;

	/* reset objects, increase/decrease quantity */
	SceneHandleInput(scene);

	/* loop through each object */
	for (i = 0; i < scene->numObjects; i++)
	{
		currentTime = 0;
		targetTime = deltaTime;

		timeDivisionsCount = 0;

		/* make sure nothing is stuck */
		SceneResolvePenetration(scene, &scene->objects[i]);

		while (currentTime < deltaTime && timeDivisionsCount < maxTimeDivisions)
		{
			/* copy object so we can revert back if penetration occurs */
			object = scene->objects[i];

			/* step simulation forward */
			RBApplyForces(&object);
			RBIntegrate(&object, targetTime - currentTime);
			RBCalculateVertices(&object);

			/* check for collisions */
			RBCheckCollisionFloor(&object);
			SceneCheckBodyCollisions(scene, &object);

			switch (object.Collision.state)
			{
				case PENETRATING:
					/* subdivide time - binary search for exact time of collison */
					targetTime = (currentTime + targetTime) / 2.0f;
					/* limit time subdivisions to prevent infinite loop if collision can't be found */
					timeDivisionsCount++;
					break;

				case COLLIDING:
					/* respond to collision and check nothing it penetrating */
					RBResolveCollisions(&object);
					SceneResolvePenetration(scene, &scene->objects[i]);
				
				case NO_COLLISION:
					/* successful step - move forward in time */
					currentTime = targetTime;
					targetTime = deltaTime;

					/* save updated object back into array */
					scene->objects[i] = object;
					break;
			}
		}
	}

	/* camera movement */
	CameraUpdate(&scene->camera, deltaTime);
}

void SceneCheckBodyCollisions(Scene *scene, Rigidbody *rb)
{
	int i;
	for (i = 0; i < scene->numObjects && rb->Collision.state == NO_COLLISION; i++)
	{
		if (rb != &scene->objects[i])
			RBCheckCollisionBody(rb, &scene->objects[i]);
	}
}

void SceneResolvePenetration(Scene *scene, Rigidbody *rb)
{
	int i, j;
	const int numInterations = 10;

	for (i = 0; i < numInterations; i++)
	{
		RBCalculateVertices(rb);
		RBMoveOutOfFloor(rb);

		for (j = 0; j < scene->numObjects; j++)
		{
			RBCalculateVertices(rb);
			RBMoveOutOfBody(rb, &scene->objects[j]);
		}
	}
}

void SceneCreateRigidbodys(Scene *scene)
{
	float size;
	int i;

	/* random initialization of rigidbodys */
	for (i = 0; i < scene->numObjectsCreate; i++)
	{	
		size = GetRandomFloat(0.5f, 1);
		RBInit(&scene->objects[i], Vec3New(GetRandomFloat(-1.2f, 1.2f), GetRandomFloat(5, 20), GetRandomFloat(-1, 1)), Vec3New(size, size, size), 3);
		scene->objects[i].orientation = M3FromEuler(Vec3New(GetRandomFloat(0, 90), GetRandomFloat(0, 90), GetRandomFloat(0, 90)));
	}

	scene->numObjects = scene->numObjectsCreate;
}

void SceneHandleInput(Scene *scene)
{
	/* create new rigidbodys */
	if (KeyDown(KEY_C) && scene->createdObjects == false)
	{
		scene->createdObjects = true;
		SceneCreateRigidbodys(scene);
	}
	if (!KeyDown(KEY_C))
		scene->createdObjects = false;

	/* increase number of rigidbodys */
	if (KeyDown(KEY_PLUS) && scene->increasedNumObjects == false)
	{
		scene->increasedNumObjects = true;
		if (scene->numObjectsCreate < MAX_OBJECTS)
			scene->numObjectsCreate++;
	}
	if (!KeyDown(KEY_PLUS))
		scene->increasedNumObjects = false;

	/* decrease number of rigidbodys */
	if (KeyDown(KEY_MINUS) && scene->decreasedNumObjects == false)
	{
		scene->decreasedNumObjects = true;
		if (scene->numObjectsCreate > 1)
			scene->numObjectsCreate--;
	}
	if (!KeyDown(KEY_MINUS))
		scene->decreasedNumObjects = false;
}
