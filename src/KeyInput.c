
#include "KeyInput.h"

#ifdef __APPLE__
#include <OpenGL/gl.h> 
#include <GLUT/glut.h>
#else
#include <GL/gl.h> 
#include <GL/glut.h>
#endif

int keyState[] = { false, false, false, false, false, false, false, false, false, false, false, false };

void KeyInputInit()
{
	/* register glut callbacks */
	glutKeyboardFunc(GlutKeyDownCallback);
	glutKeyboardUpFunc(GlutKeyUpCallback);
	glutSpecialFunc(GlutSpecialKeyDownCallback);
	glutSpecialUpFunc(GlutSpecialKeyUpCallback);
}

bool KeyDown(Key key)
{
	return keyState[key];
}

void GlutKeyDownCallback(unsigned char k, int x, int y)
{
	switch (k)
	{
		case 'w':
			keyState[KEY_W] = true;
			break;
		case 'a':
			keyState[KEY_A] = true;
			break;
		case 's':
			keyState[KEY_S] = true;
			break;
		case 'd':
			keyState[KEY_D] = true;
			break;
		case 'c':
			keyState[KEY_C] = true;
			break;
		case '=':
		case '+':
			keyState[KEY_PLUS] = true;
			break;
		case '-':
		case '_':
			keyState[KEY_MINUS] = true;
			break;
		case 27:
			keyState[KEY_ESC] = true;
			break;
	}
}

void GlutKeyUpCallback(unsigned char k, int x, int y)
{
	switch (k)
	{
		case 'w':
			keyState[KEY_W] = false;
			break;
		case 'a':
			keyState[KEY_A] = false;
			break;
		case 's':
			keyState[KEY_S] = false;
			break;
		case 'd':
			keyState[KEY_D] = false;
			break;
		case 'c':
			keyState[KEY_C] = false;
			break;
		case '=':
		case '+':
			keyState[KEY_PLUS] = false;
			break;
		case '-':
		case '_':
			keyState[KEY_MINUS] = false;
			break;
		case 27:
			keyState[KEY_ESC] = false;
			break;
	}
}

void GlutSpecialKeyDownCallback(int k, int x, int y)
{
	switch (k)
	{
		case GLUT_KEY_UP:
			keyState[KEY_UP] = true;
			break;
		case GLUT_KEY_DOWN:
			keyState[KEY_DOWN] = true;
			break;
		case GLUT_KEY_LEFT:
			keyState[KEY_LEFT] = true;
			break;
		case GLUT_KEY_RIGHT:
			keyState[KEY_RIGHT] = true;
			break;
	}
}

void GlutSpecialKeyUpCallback(int k, int x, int y)
{
	switch (k)
	{
		case GLUT_KEY_UP:
			keyState[KEY_UP] = false;
			break;
		case GLUT_KEY_DOWN:
			keyState[KEY_DOWN] = false;
			break;
		case GLUT_KEY_LEFT:
			keyState[KEY_LEFT] = false;
			break;
		case GLUT_KEY_RIGHT:
			keyState[KEY_RIGHT] = false;
			break;
	}
}
