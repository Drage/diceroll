
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Scene.h"
#include "Colour.h"
#include "Boolean.h"
#include "Rigidbody.h"
#include "Light.h"

/**
 * @brief	Defines the display properties used by the rendering system. 
 * @author	Matt Drage
 * @date	11/03/2012
 */
typedef struct 
{
	int FPS;
	int windowWidth;
	int windowHeight;
	char *windowCaption;
	float fov;
	float zNear;
	float zFar;
	bool fullscreen;
	Colour clearColour;
	float shadowMatrix[16]; /* Matrix used to project shadows onto ground plane */
} DisplayPropertiesType;
DisplayPropertiesType DisplayProperties;

/**
 * @brief	Stores texture indexes. 
 * @author	Matt Drage
 * @date	11/03/2012
 */
typedef struct
{
	unsigned dice[6];
	unsigned floor;
} TexturesType;
TexturesType Textures;

/**
 * @brief	Defines an type alias for the 'loop' callback function.
 */
typedef void (*LoopCallbackType)();
LoopCallbackType loop;

/**
 * @brief	Initializes the rendering system and openGL settings
 * @author	Matt Drage
 * @date	11/03/2012
 */
void GraphicsInit(char *path);

/**
 * @brief	Starts the simulation loop.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	updateFunction	The update callback function - called every frame of the simulation.
 */
void GraphicsStartLoop(LoopCallbackType updateFunction);

/**
 * @brief	Updates openGL setting to reflect changes to the display proterties.
 * @details	Call this function after modifying the DisplayProperties struct.
 * @author	Matt Drage
 * @date	11/03/2012
 */
void GraphicsUpdateDisplayProperties();

/**
 * @brief	Renders the scene.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	scene	The scene to render.
 */
void RenderScene(Scene *scene);

/**
 * @brief	Renders a rigidbody object.
 * @details	Rigidbody will be textured as a dice.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	rb	The rb to render.
 */
void RenderRigidbody(Rigidbody *rb);

/**
 * @brief	Closes the simulation.
 * @author	Matt Drage
 * @date	11/03/2012
 */
void ExitProgram();

/**
 * @brief	Renders the floor plane.
 * @details	Used internally by the Renderer. Floor plane is located at y = 0.
 * @author	Matt Drage
 * @date	11/03/2012
 */
void RenderFloor();

/**
 * @brief	Creates a shadow matrix to project shadows onto the floor plane.
 * @details	Floor plane is located at y = 0.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	lightPosition	The light position.
 */
void CreateShadowMatrix(Vector3 lightPosition);

/**
 * @brief	Loads the textures.
 * @details	Used internally by the Renderer.
 * @author	Matt Drage
 * @date	11/03/2012
 */
void LoadTextures(char *path);

/**
 * @brief	Creates an openGL compatible transform from an orientation matrix and position vector.
 * @details	Used internally by the Renderer.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	orientation	The orientation.
 * @param	position   	The position.
 * @param	transform  	The openGL transform.
 */
void CreateOpenGLTransform(Matrix3x3 orientation, Vector3 position, float transform[]);

/**
 * @brief	Sets the lighting parameters for rendering.
 * @details	Used internally by the Renderer.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	light	The light.
 */
void SetLighting(Light *light);

/**
 * @brief	Callback for the glut update timer.
 * @details	Used internally by the Renderer.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	x	Required by GLUT - not used.
 */
void GlutTimerCallback(int x);

/**
 * @brief	Updates the window size.
 * @details	Used internally by the Renderer. GLUTs resize callback function doesn't work - this
 * 			function manually checks for window resize.
 * @author	Matt Drage
 * @date	11/03/2012
 */
void UpdateWindowSize();

#endif
