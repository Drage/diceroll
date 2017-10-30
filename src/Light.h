
#ifndef LIGHT_H
#define LIGHT_H

#include "Colour.h"
#include "Boolean.h"

/**
 * @brief	Values that represent LightTypes. 
 */
enum LightType { DIRECTIONAL_LIGHT = 0, POINT_LIGHT = 1 };
typedef enum LightType LightType;

/**
 * @brief	Defines a light source. 
 * @author	Matt Drage
 * @date	11/03/2012
 */
struct Light
{
	LightType type;
	Vector3 position;
	Colour ambient;
	Colour diffuse;
	Colour specular;
	bool enabled;
};
typedef struct Light Light;

#endif
