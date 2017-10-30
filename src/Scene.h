
#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Rigidbody.h"
#include "Light.h"
#include "Boolean.h"

/**
 * @brief	Defines the maximum number of objects allowed. 
 */
enum { MAX_OBJECTS = 10 };

/**
 * @brief	Contains objects, light, and camera. 
 * @author	Matt Drage
 * @date	11/03/2012
 */
struct Scene
{
	Camera camera;

	Rigidbody objects[MAX_OBJECTS];
	int numObjects;				/* current number of objects */
	int numObjectsCreate;		/* number of objects created when the key c is pressed */

	Light light;

	bool createdObjects;		/* makes action occur once per key press rather than continuously */
	bool increasedNumObjects;	/* makes action occur once per key press rather than continuously */	
	bool decreasedNumObjects;	/* makes action occur once per key press rather than continuously */
};
typedef struct Scene Scene;

/**
 * @brief	Handle user input.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	scene	The scene.
 */
void SceneHandleInput(Scene *scene);

/**
 * @brief	Creates and initializes rigidbodys.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	scene	The scene.
 */
void SceneCreateRigidbodys(Scene *scene);

/**
 * @brief	Initializes the scene
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	scene	The scene.
 */
void SceneInit(Scene *scene);

/**
 * @brief	Updates the scene.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	scene		The scene.
 * @param	deltaTime	Time elapsed since last update.
 */
void SceneUpdate(Scene *scene, float deltaTime);

/**
 * @brief	Checks a rigidbody for collisions.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	scene	The scene.
 * @param 	rb   	The rigidbody.
 */
void SceneCheckBodyCollisions(Scene *scene, Rigidbody *rb);

/**
 * @brief	Resolve any penetrating objects.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	scene	The scene.
 * @param 	rb   	The rigidbody.
 */
void SceneResolvePenetration(Scene *scene, Rigidbody *rb);

#endif
