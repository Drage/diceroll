
#ifndef COLOUR_H
#define COLOUR_H

/**
 * @brief	Defines a colour as RGB and alpha intensities. 
 * @details	Values range between 0 (min intensity) and 1 (max intensity)
 * @author	Matt Drage
 * @date	11/03/2012
 */
struct Colour
{
	float r;
	float g;
	float b;
	float a;
};
typedef struct Colour Colour;

/**
 * @brief	Creates a new colour from parameters.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	r	The red intensity.
 * @param	g	The green intensity.
 * @param	b	The blue intensity.
 * @param	a	The alpha transparency.
 * @return	A colour stuct.
 */
Colour ColourNew(float r, float g, float b, float a);

#endif
