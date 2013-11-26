/**
 * @file
 * @author Jonathan Udd <jonis.x@gmail.com>
 * @version 1.0
 * Copyright (©) A-Team.
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

#ifndef __INPUTHANDLER_H__
#define __INPUTHANDLER_H__

#include "../Utilities/Macros.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace logic
{

//Check if the key is pressed, and return true if so.
#define KEYDOWN(buffer, key)		((buffer[key]				& 0x80) != 0)
//Check if the button is pressed, and return true if so.
#define BUTTONDOWN(buffer, button)	((buffer.rgbButtons[button]	& 0x80) != 0)

#define UINPUT_MOUSE		0x01
#define UINPUT_KEYBOARD		0x02

/**
 *	Input class interface that uses dinput. Lets you access keyboard and mouse.
 *	This will class will be used through InputState.
 */

class InputHandler
{
public:
	/**
	* The constructor, initialize the input devices.
	* @param pHInstance This is the common hinstance.
	* @param pHWnd This is the common window.
	* @param inputs Sets the different input devices.
	*/
	InputHandler(const HINSTANCE *pHInstance, const HWND *pHWnd, const BYTE inputs = UINPUT_KEYBOARD | UINPUT_MOUSE);

	/**
	* The destructor, release the input devices.
	*/
	~InputHandler();

	/**
	* Update the input devices states.
	*/
	void update();

	/**
	* Return true if the key provided is pressed.
	* @param key The key to check with.
	* @return True if key is pressed.
	*/
	bool keyDown(const BYTE& key);

	/**
	* Return true if mousebutton provided is pressed.
	* @param button The button to check with.
	* @return True if button is pressed
	*/
	bool buttonDown(const BYTE& button);

	/**
	* Returns true if the provided button is not pressed.
	* @param button The button to check with.
	* @return True if the button is released.
	*/
	bool buttonReleased(const BYTE& button);

	/**
	* Returns true if the provided key is not pressed.
	* @param key The key to check with.
	* @return True if the key is released.
	*/
	bool keyReleased(const BYTE& key);

	/**
	* Sets dx and dy to the delta movement and return true if the mouse was moved.
	* @param dx The mouse movement in x position.
	* @param dy The mouse movement in y position.
	* @return True if mousemovement is recorded.
	*/
	bool mouseMovement(float& dx, float& dy);

private:
	BYTE			mInputs;
	BYTE			mKbBuffer[256];
	DIMOUSESTATE	mMouseBuffer;

	LPDIRECTINPUT8			mDiObj;
	LPDIRECTINPUTDEVICE8	mDiDKeyboard;
	LPDIRECTINPUTDEVICE8	mDiDMouse;
};
}
#endif