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

#ifndef __INPUTSTATES_H__
#define __INPUTSTATES_H__

#include "../Utilities/Macros.h"

#include "InputHandler.h"
#include "../Utilities/Timer.h"

namespace logic
{	

/**
 *	This class will be a layer between the InputHandler and and the logic. 
 *	There will be different states with different keybinds i.e. in the menu 
 *	and the actual game. This class will send events to the eventhandler 
 *	depending on which state and what keys are pressed.
 */

class InputStates
{
public:
	enum GameStates
	{
		GameState_Menu,
		GameState_Game_Inside_Tank,
		GameState_Game_Outside_Tank,
		GameState_Editor
	};

	/**
	* The constructor, create a InputHandler instance.
	* @param pHInstance This is the common hinstance.
	* @param pHWnd This is the common window.
	*/
	InputStates(HINSTANCE *pHInstance, HWND *pHWnd);

	/**
	* Destructor, delete InputHandler.
	*/
	~InputStates();
	
	/**
	* Check which state the game is in and call the right state function.
	*/
	void update();
	
	/**
	* Sets which game state that is currently active.
	* @param state This is the new current game state.
	*/
	void setState(GameStates state);

	/**
	* Check if the provided key is pressed.
	* @param key Key to be checked.
	* @return Returns true if the key was pressed.
	*/
	bool checkKeyDown(BYTE key);

	/**
	* Check if the provided key is released.
	* @param key Key to be checked.
	* @return Returns true if the key is released.
	*/
	bool checkKeyUp(BYTE key);

	/**
	* Check if the provided button is pressed.
	* @param button Button to be checked.
	* @return Returns true if the button was pressed.
	*/
	bool checkButtonDown(BYTE button);

	/**
	* Check if the provided button is released.
	* @param button Button to be checked.
	* @return Returns true if the button is released.
	*/
	bool checkButtonUp(BYTE button);

private:
	InputHandler	*mpInputHandler;
	GameStates mState;

	bool	mKeyPressed[256];
	bool	mButtonPressed[8];

	double	mTimeKeyPressed[256];
	double	mTimeButtonPressed[8];

	utilities::Timer	mKeyTimers[256];
	utilities::Timer	mButtonTimers[8];
	
	/**
	* Check if any keys are pressed that are assigned to the respective state.
	*/
	void menuState();
	void gameInsideTankState();
	void gameOutsideTankState();
	void editorState();

	
};
}
#endif