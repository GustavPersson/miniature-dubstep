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
#include "PlayerInfo.h"
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
		GameState_Editor,
		GameState_GameOver
	};
	enum LetterKeys
	{
		LetterKey_A		= 1,
		LetterKey_B		= 2,
		LetterKey_C		= 4,
		LetterKey_D		= 8,
		LetterKey_E		= 16,
		LetterKey_F		= 32,
		LetterKey_G		= 64,
		LetterKey_H		= 128,
		LetterKey_I		= 256,
		LetterKey_J		= 512,
		LetterKey_K		= 1024,
		LetterKey_L		= 2048,
		LetterKey_M		= 4096,
		LetterKey_N		= 8192,
		LetterKey_O		= 16384,
		LetterKey_P		= 32768,
		LetterKey_Q		= 65536,
		LetterKey_R		= 131072,
		LetterKey_S		= 262144,
		LetterKey_T		= 524288,
		LetterKey_U		= 1048576,
		LetterKey_V		= 2097152,
		LetterKey_W		= 4194304,
		LetterKey_X		= 8388608,
		LetterKey_Y		= 16777216,
		LetterKey_Z		= 33554432,
	};
	enum Keys
	{
		Key_1		= 1,
		Key_2		= 2,
		Key_3		= 4,
		Key_4		= 8,
		Key_5		= 16,
		Key_6		= 32,
		Key_7		= 64,
		Key_8		= 128,
		Key_9		= 256,
		Key_0		= 512,
		Key_Return	= 1024,
		Key_Space	= 2048,
		Key_Up		= 4096,
		Key_Down	= 8192,
		Key_Left	= 16384,
		Key_Right	= 32768,
		Key_Shift	= 65536,
		Key_Tab		= 131072,
		Key_Ctrl	= 262144,
		Key_Backspace = 524288,
	};

	/**
	* Returns the only instance of this object.
	* @return Returns the instance of this object
	*/
	static InputStates* getInstance();

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
	* Check if the provided key was pressed.
	* @param key Key to be checked.
	* @return Returns true if the key was pressed.
	*/
	bool wasKeyPressed(BYTE key);

	/**
	* Check if the provided key was released.
	* @param key Key to be checked.
	* @return Returns true if the key was released.
	*/
	bool wasKeyReleased(BYTE key);

	/**
	* Check if the provided button was pressed.
	* @param button Button to be checked.
	* @return Returns true if the button was pressed.
	*/
	bool wasButtonPressed(BYTE button);

	/**
	* Check if the provided button was released.
	* @param button Button to be checked.
	* @return Returns true if the button was released.
	*/
	bool wasButtonReleased(BYTE button);

	/**
	* Checks if a key is down
	* @param key Key to be checked
	* @return Returns true if the key is down
	*/
	bool isKeyDown(BYTE key);

private:

	/**
	* The constructor, create a InputHandler instance.
	* @param pHInstance This is the common hinstance.
	* @param pHWnd This is the common window.
	*/
	InputStates(HINSTANCE *pHInstance, HWND *pHWnd);

	static InputStates *mpsInstance;

	InputHandler	*mpInputHandler;
	PlayerInfo		*mpPlayerInfo;
    GameStates mState;
	GameStates mOldState;

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
	void gameOverState();

	/**
	* Checks if any keys are pressed. This function always runs and does not
	* depend on any state
	*/
	void allStates();
};
}
#endif