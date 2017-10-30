
#ifndef KEYINPUT_H
#define KEYINPUT_H

#include "Boolean.h"

/**
 * @brief	Values that represent Keys. 
 */
enum Key { KEY_UP = 0, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_W, KEY_A, KEY_S, KEY_D, KEY_C, KEY_PLUS, KEY_MINUS, KEY_ESC };
typedef enum Key Key;

///< Stores the state of each key.
extern int keyState[];

/**
 * @brief	Initialized keyboard input.
 * @author	Matt Drage
 * @date	11/03/2012
 */
void KeyInputInit();

/**
 * @brief	Gets whether a key is currently being pressed.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	key	The key to check.
 * @return	true if key is pressed, false if not.
 */
bool KeyDown(Key key);

/**
 * @brief	Glut key down callback function.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	k	The key.
 * @param	x	The mouse x coordinate - required by glut.
 * @param	y	The mouse y coordinate - required by glut.
 */
void GlutKeyDownCallback(unsigned char k, int x, int y);

/**
 * @brief	Glut key up callback function.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	k	The key.
 * @param	x	The mouse x coordinate - required by glut.
 * @param	y	The mouse y coordinate - required by glut.
 */
void GlutKeyUpCallback(unsigned char k, int x, int y);

/**
 * @brief	Glut special key down callback function.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	k	The key.
 * @param	x	The mouse x coordinate - required by glut.
 * @param	y	The mouse y coordinate - required by glut.
 */
void GlutSpecialKeyDownCallback(int k, int x, int y);

/**
 * @brief	Glut special key up callback function.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param	k	The key.
 * @param	x	The mouse x coordinate - required by glut.
 * @param	y	The mouse y coordinate - required by glut.
 */
void GlutSpecialKeyUpCallback(int k, int x, int y);

#endif
