
#include "Graphics.h"
#include "ImageTGA.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#ifdef __APPLE__
#include <OpenGL/gl.h> 
#include <GLUT/glut.h>
#else
#include <GL/gl.h> 
#include <GL/glut.h>
#endif

void GlutDisplay() {}

void GraphicsInit(char *path)
{
	int argc = 0;
	char** argv = 0;
	float ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f};

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);

	/* create window */
	glutInitWindowSize(DisplayProperties.windowWidth, DisplayProperties.windowHeight);
	glutCreateWindow(DisplayProperties.windowCaption);
	GraphicsUpdateDisplayProperties();

	/* depth settings */
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	/* light/shadow settings */
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	glEnable(GL_LIGHTING);
	glClearStencil(0);

	/* blend settings */
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	/* load textures */
	glEnable(GL_TEXTURE_2D);
	LoadTextures(path);

	/* set update timer function */
	glutTimerFunc(1000 / DisplayProperties.FPS, GlutTimerCallback, 0);
	
	/* keep glut happy */
	glutDisplayFunc(GlutDisplay);
}

void LoadTextures(char *path)
{
	ImageTGA tga;
	int i;
	char filename[] = "Assets/Dice00.tga";

	for (i = 0; i < 6; i++)
	{
		/* create filename */
		filename[strlen(filename)-5] = (char)((int)'1' + i);

		/* load tga */
		TGALoad(&tga, filename);

		/* create openGL texture */
		glGenTextures(1, (GLuint*)&Textures.dice[i]);
		glBindTexture(GL_TEXTURE_2D, (GLuint)Textures.dice[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (tga.bpp == 32)
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, tga.width, tga.height, GL_RGBA, GL_UNSIGNED_BYTE, tga.image);
		else if (tga.bpp == 24)
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, tga.width, tga.height, GL_RGB, GL_UNSIGNED_BYTE, tga.image);
	}
}

void CreateShadowMatrix(Vector3 lightPosition)
{
	DisplayProperties.shadowMatrix[0]  = lightPosition.y;
	DisplayProperties.shadowMatrix[4]  = -lightPosition.x;
	DisplayProperties.shadowMatrix[8]  = 0;
	DisplayProperties.shadowMatrix[12] = 0;

	DisplayProperties.shadowMatrix[1]  = 0;
	DisplayProperties.shadowMatrix[5]  = 0;
	DisplayProperties.shadowMatrix[9]  = 0;
	DisplayProperties.shadowMatrix[13] = 0;

	DisplayProperties.shadowMatrix[2]  = 0;
	DisplayProperties.shadowMatrix[6]  = -lightPosition.z;
	DisplayProperties.shadowMatrix[10] = lightPosition.y;
	DisplayProperties.shadowMatrix[14] = 0;

	DisplayProperties.shadowMatrix[3]  = 0;
	DisplayProperties.shadowMatrix[7]  = -lightPosition.z;
	DisplayProperties.shadowMatrix[11] = 0;
	DisplayProperties.shadowMatrix[15] = lightPosition.y;
}

void GraphicsStartLoop(LoopCallbackType updateFunction)
{
	loop = updateFunction;
	glutMainLoop();
}

void GraphicsUpdateDisplayProperties()
{
	int w, h;
	float aspectRatio;

	glutSetWindowTitle(DisplayProperties.windowCaption);

	glClearColor(DisplayProperties.clearColour.r, DisplayProperties.clearColour.g, DisplayProperties.clearColour.b, DisplayProperties.clearColour.a);

	if (DisplayProperties.fullscreen)
	{
		glutFullScreen();
		w = glutGet(GLUT_SCREEN_WIDTH);
		h = glutGet(GLUT_SCREEN_HEIGHT);
	}
	else
	{
		w = DisplayProperties.windowWidth;
		h = DisplayProperties.windowHeight;
	}

	aspectRatio = 1.0f * w / h;

	/* update projection and display port */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, DisplayProperties.windowWidth, DisplayProperties.windowHeight);
	gluPerspective(DisplayProperties.fov, aspectRatio, DisplayProperties.zNear, DisplayProperties.zFar);
	glMatrixMode(GL_MODELVIEW);
}

void RenderScene(Scene *scene)
{
	int i;

	/* set lighting */
	SetLighting(&scene->light);

	/* clear buffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();

	/* set view */
	gluLookAt(scene->camera.position.x, scene->camera.position.y, scene->camera.position.z, scene->camera.focus.x, scene->camera.focus.y, scene->camera.focus.z, 0, 1, 0);

	/* render floor */
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		RenderFloor();
	glDisable(GL_STENCIL_TEST);
	
	/* render shadows */
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE);
	glDisable(GL_DEPTH_TEST);
	glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
	CreateShadowMatrix(scene->light.position);
	glPushMatrix();
		glMultMatrixf(DisplayProperties.shadowMatrix);
		for (i = 0; i < scene->numObjects; i++)
			RenderRigidbody(&scene->objects[i]);
	glPopMatrix();
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE);
	glEnable(GL_DEPTH_TEST);

	/* render objects */
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPushMatrix();
		for (i = 0; i < scene->numObjects; i++)
			RenderRigidbody(&scene->objects[i]);
	glPopMatrix();
	
	glFlush();
	glutSwapBuffers();
}

void RenderFloor()
{
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, Textures.dice[2]);
		glColor4f(0.2f, 0.2f, 0.2f, 0.5f);
		glBegin(GL_QUADS);
			glNormal3f(0, 1, 0); glVertex3f(-100, 0,-100);
			glNormal3f(0, 1, 0); glVertex3f(-100, 0, 100);
			glNormal3f(0, 1, 0); glVertex3f( 100, 0, 100);
			glNormal3f(0, 1, 0); glVertex3f( 100, 0,-100);
		glEnd();
	glPopMatrix();
}

void RenderRigidbody(Rigidbody *rb)
{
	float m[16];
	float x, y, z;

	x = rb->dimensions.x / 2;
	y = rb->dimensions.y / 2;
	z = rb->dimensions.z / 2;

    glPushMatrix();
		/* convert orientation and poisition to openGL matrix */
        CreateOpenGLTransform(rb->orientation, rb->position, m);
        glMultMatrixf(m);
        
		glBindTexture(GL_TEXTURE_2D, Textures.dice[0]);
		glBegin(GL_QUADS);
			glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,-y,-z);
			glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-x,-y, z);
			glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, y, z);
			glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f(-x, y,-z);
		glEnd();
                
		glBindTexture(GL_TEXTURE_2D, Textures.dice[1]);
		glBegin(GL_QUADS);
			glNormal3f( 1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f( x, y, z);
			glNormal3f( 1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f( x,-y, z);
			glNormal3f( 1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f( x,-y,-z);
			glNormal3f( 1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f( x, y,-z);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, Textures.dice[2]);
		glBegin(GL_QUADS);
			glNormal3f( 0.0f,-1.0f, 0.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,-y,-z);
			glNormal3f( 0.0f,-1.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f( x,-y,-z);
			glNormal3f( 0.0f,-1.0f, 0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f( x,-y, z);
			glNormal3f( 0.0f,-1.0f, 0.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,-y, z);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, Textures.dice[3]);
		glBegin(GL_QUADS);
			glNormal3f( 0.0f, 1.0f, 0.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f( x, y, z);
			glNormal3f( 0.0f, 1.0f, 0.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f( x, y,-z);
			glNormal3f( 0.0f, 1.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, y,-z);
			glNormal3f( 0.0f, 1.0f, 0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, y, z);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, Textures.dice[4]);
		glBegin(GL_QUADS);
			glNormal3f( 0.0f, 0.0f,-1.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,-y,-z);
			glNormal3f( 0.0f, 0.0f,-1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, y,-z);
			glNormal3f( 0.0f, 0.0f,-1.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f( x, y,-z);
			glNormal3f( 0.0f, 0.0f,-1.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f( x,-y,-z);
        glEnd();

		glBindTexture(GL_TEXTURE_2D, Textures.dice[5]);
		glBegin(GL_QUADS);
			glNormal3f( 0.0f, 0.0f, 1.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f( x, y, z);
			glNormal3f( 0.0f, 0.0f, 1.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(-x, y, z);
			glNormal3f( 0.0f, 0.0f, 1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-x,-y, z);
			glNormal3f( 0.0f, 0.0f, 1.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f( x,-y, z);
		glEnd();
    glPopMatrix();
}

void ExitProgram()
{
	exit(0);
}

void SetLighting(Light *light)
{
	if (light->enabled)
	{
		float ambientLight[] = { light->ambient.r, light->ambient.b, light->ambient.g, light->ambient.a };
		float diffuseLight[] = { light->diffuse.r, light->diffuse.b, light->diffuse.g, light->diffuse.a };
		float specularLight[] = { light->specular.r, light->specular.b, light->specular.g, light->specular.a };
		float position[] = { light->position.x, light->position.y, light->position.z, light->type };   

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
	}
	else
	{
		glDisable(GL_LIGHT0);
	}
}

void CreateOpenGLTransform(Matrix3x3 orientation, Vector3 position, float transform[])
{
	/* create float[4][4] alias for float[16] array - easier to use loops */
	float (*m)[4] = (float(*)[4])transform;
    int i, j;
    
	/* set rotation */
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            m[j][i] = orientation.elements[i][j];
        }
    }

	/* fourth column values set to identity - no scaling */
    m[0][3] = 0;
	m[1][3] = 0;
	m[2][3] = 0; 
	m[3][3] = 1;

	/* set translation */
    for (i = 0; i < 3; i++)
        m[3][i] = Vec3GetElement(&position, i);
}

void GlutTimerCallback(int x)
{
	/* set timer for next loop */
	glutTimerFunc(1000 / DisplayProperties.FPS, GlutTimerCallback, 0);

	/* check if window size has changed */
	UpdateWindowSize();

	/* call simulation update function via pointer */
	(*loop)();
}

void UpdateWindowSize()
{
	int w, h;

	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);

	/* update display if window size has changed */
	if (DisplayProperties.windowWidth != w || DisplayProperties.windowHeight != h)
	{
		DisplayProperties.windowWidth = w;
		DisplayProperties.windowHeight = h;
		GraphicsUpdateDisplayProperties();
	}
}
