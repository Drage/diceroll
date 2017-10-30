
#include "Colour.h"

Colour ColourNew(float r, float g, float b, float a)
{
	Colour newColour;

	/* rescale input if given as 0-255 range instead of 0-1 */
	if (r > 1) r /= 255;
	if (g > 1) g /= 255;
	if (b > 1) b /= 255;
	if (a > 1) a /= 255;

	newColour.r = r;
	newColour.g = g;
	newColour.b = b;
	newColour.a = a;

	return newColour;
}
