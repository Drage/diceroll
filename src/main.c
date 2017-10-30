
#include <stdio.h>
#include "Graphics.h"
#include "Scene.h"
#include "KeyInput.h"
#include <time.h>
#include "MathUtils.h"

void Update();
float GetTime();
float GetDeltaTime();

Scene scene;
float lastTime;

int main(int argc, char **argv)
{
	/* set display properties */
	DisplayProperties.FPS = 200;
	DisplayProperties.windowCaption = "Dice Roll Physics Simulation";
	DisplayProperties.windowWidth = 600;
	DisplayProperties.windowHeight = 400;
	DisplayProperties.fullscreen = false;
	DisplayProperties.fov = 60;
	DisplayProperties.zNear = 0.1f;
	DisplayProperties.zFar = 1000.0f;
	DisplayProperties.clearColour = ColourNew(100.0f/255, 149.0f/255, 237.0f/255, 1);
	GraphicsInit(argv[0]);

	/* intialization */
	KeyInputInit();
	InitRandomGeneration();
	SceneInit(&scene);

	lastTime = GetTime();

	/* start simulation loop */
	GraphicsStartLoop(Update);
	return 0;
}

void Update()
{
	if (KeyDown(KEY_ESC))
		ExitProgram();

	SceneUpdate(&scene, GetDeltaTime());
	RenderScene(&scene);
}

float GetTime()
{
	/* return current time */
	return ((float)(clock())) / ((float)CLOCKS_PER_SEC);
}

float GetDeltaTime()
{
	/* return time since last call */
	float time = GetTime();
	float deltaTime = time - lastTime;
	lastTime = time;
	return deltaTime;
}
