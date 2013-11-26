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
 * 
 * @section DESCRIPTION
 *
 *	This class will be a layer between the InputHandler and and the logic. 
 *	There will be different states with different keybinds i.e. in the menu 
 *	and the actual game. This class will send events to the eventhandler 
 *	depending on which state and what keys are pressed.
 */

#include "InputStates.h"
#include "Events.h"
#include "../Ai/AiEvents.h"
#include "EventManager.h"
#include "Actor.h"
#include "ActorHandler.h"
#include "PlayerInfo.h"
#include "GlobalTimer.h"
#include "Application.h"
#include "../Editor/EditorHandler.h"

using namespace logic;

InputStates* InputStates::mpsInstance = NULL;

InputStates* InputStates::getInstance()
{
	// If there is no instance of this object
	if(mpsInstance == NULL)
	{
		// Create an instance of this object
		mpsInstance = myNew InputStates(gfx::DX10Module::getInstance()->getHInstancePointer(), gfx::DX10Module::getInstance()->getHWNDPointer());
	}
	// return GlobalTimer
	return mpsInstance;
}

InputStates::InputStates(HINSTANCE *pHInstance, HWND *pHWnd)
{
	//Create a new InputHandler.
	mpInputHandler = myNew InputHandler(pHInstance, pHWnd);
	mpPlayerInfo = PlayerInfo::getInstance();
	mOldState = InputStates::GameState_Menu;
	setState(GameState_Menu);

	for(int i=0;i<256;i++)
		mKeyPressed[i] = false;
	for(int i=0;i<8;i++)
		mButtonPressed[i] = false;
	for(int i=0;i<256;i++)
		mTimeKeyPressed[i] = 0;
	for(int i=0;i<8;i++)
		mTimeButtonPressed[i] = 0;

	safeTriggerEvent(EvtData_System_InputStatesStarted());	
}

InputStates::~InputStates() 
{
	//Delete the InputHandler.
	SAFE_DELETE(mpInputHandler);
	mpsInstance = NULL;
}

void InputStates::setState(GameStates state)
{
	mOldState = mState;
	mState = state;
}

bool InputStates::wasKeyPressed(BYTE key)
{
	bool temp = false;
	if(mpInputHandler->keyDown(key) && !mKeyPressed[key])
	{
		mKeyPressed[key] = true;
		mKeyTimers[key].start();
		temp = true;
	}
	return temp;
}

bool InputStates::wasKeyReleased(BYTE key)
{
	bool temp = false;
	if(!mpInputHandler->keyDown(key) && mKeyPressed[key])
	{
		mKeyPressed[key] = false;
		mTimeKeyPressed[key] = mKeyTimers[key].getTime(utilities::Timer::ReturnType_MilliSeconds);
		temp = true;
	}	
	return temp;
}

bool InputStates::isKeyDown(BYTE key)
{
	return mpInputHandler->keyDown(key);
}

void InputStates::update() 
{
	mpInputHandler->update();
	//switch for game states
	switch (mState)
	{
	//case Menu
	case GameState_Menu:
		menuState();
		break;
	//case GameInTank
	case GameState_Game_Inside_Tank:
		//check keys assigned to this game state  
		gameInsideTankState();
		break;
        //case GameOutTank
	case GameState_Game_Outside_Tank:
		//check keys assigned to this game state
		gameOutsideTankState();
		break;
		//case Editor
	case GameState_Editor:
		editorState();
		break;
		//case GameOver
	case GameState_GameOver:
		gameOverState();
		break;
	default:
		ERROR_MESSAGE("Invalid state is set, no keys are updated in InputStates update().");
		break;
	}

	allStates();
}

void InputStates::menuState()
{
	//if a menu binded key is pressed "for the first time" (not held down)
	//send the right event to the eventhandler

	//if a game binded key is released send the right event to the eventhandler

	unsigned int letterKeysPressed = 0;
	unsigned int keysPressed = 0;
	//if a game binded key is pressed "for the first time" (not held down)
	//send the right event to the eventhandler
	if(wasKeyPressed(DIK_Q))
	{
		letterKeysPressed |= LetterKey_Q;
	}
	if(wasKeyPressed(DIK_W))
	{
		letterKeysPressed |= LetterKey_W;
	}
	if(wasKeyPressed(DIK_E))
	{
		letterKeysPressed |= LetterKey_E;
	}
	if(wasKeyPressed(DIK_R))
	{
		letterKeysPressed |= LetterKey_R;
	}
	if(wasKeyPressed(DIK_T))
	{
		letterKeysPressed |= LetterKey_T;
	}
	if(wasKeyPressed(DIK_Y))
	{
		letterKeysPressed |= LetterKey_Y;
	}
	if(wasKeyPressed(DIK_U))
	{
		letterKeysPressed |= LetterKey_U;
	}
	if(wasKeyPressed(DIK_I))
	{
		letterKeysPressed |= LetterKey_I;
	}
	if(wasKeyPressed(DIK_O))
	{
		letterKeysPressed |= LetterKey_O;
	}
	if(wasKeyPressed(DIK_P))
	{
		letterKeysPressed |= LetterKey_P;
	}
	if(wasKeyPressed(DIK_A))
	{
		letterKeysPressed |= LetterKey_A;
	}
	if(wasKeyPressed(DIK_S))
	{
		letterKeysPressed |= LetterKey_S;
	}
	if(wasKeyPressed(DIK_D))
	{
		letterKeysPressed |= LetterKey_D;
	}
	if(wasKeyPressed(DIK_F))
	{
		letterKeysPressed |= LetterKey_F;
	}
	if(wasKeyPressed(DIK_G))
	{
		letterKeysPressed |= LetterKey_G;
	}
	if(wasKeyPressed(DIK_H))
	{
		letterKeysPressed |= LetterKey_H;
	}
	if(wasKeyPressed(DIK_J))
	{
		letterKeysPressed |= LetterKey_J;
	}
	if(wasKeyPressed(DIK_K))
	{
		letterKeysPressed |= LetterKey_K;
	}
	if(wasKeyPressed(DIK_L))
	{
		letterKeysPressed |= LetterKey_L;
	}
	if(wasKeyPressed(DIK_Z))
	{
		letterKeysPressed |= LetterKey_Z;
	}
	if(wasKeyPressed(DIK_X))
	{
		letterKeysPressed |= LetterKey_X;
	}
	if(wasKeyPressed(DIK_C))
	{
		letterKeysPressed |= LetterKey_C;
	}
	if(wasKeyPressed(DIK_V))
	{
		letterKeysPressed |= LetterKey_V;
	}
	if(wasKeyPressed(DIK_B))
	{
		letterKeysPressed |= LetterKey_B;
	}
	if(wasKeyPressed(DIK_N))
	{
		letterKeysPressed |= LetterKey_N;
	}
	if(wasKeyPressed(DIK_M))
	{
		letterKeysPressed |= LetterKey_M;
	}
	if(wasKeyPressed(DIK_SPACE))
	{
		keysPressed |= Key_Space;
	}
	if(wasKeyPressed(DIK_ESCAPE))
	{
		//pressedKeys |= DIK_ESCAPE;
		safeQueueEvent(EventDataPtr(myNew EvtData_System_EndGame()));
	}
	if(wasKeyPressed(DIK_RETURN))
	{
		keysPressed |= Key_Return;
	}
	if(wasKeyPressed(DIK_DOWN))
	{
		keysPressed |= Key_Down;
	}
	if(wasKeyPressed(DIK_UP))
	{
		keysPressed |= Key_Up;
	}
	if(wasKeyPressed(DIK_RIGHT))
	{
		keysPressed |= Key_Right;
	}
	if(wasKeyPressed(DIK_LEFT))
	{
		keysPressed |= Key_Left;
	}
	if(wasKeyPressed(DIK_RSHIFT))
	{
		keysPressed |= Key_Shift;
	}
	if(wasKeyPressed(DIK_LSHIFT))
	{
		keysPressed |= Key_Shift;
	}
	if(wasKeyPressed(DIK_TAB))
	{
		keysPressed |= Key_Tab;
	}
	if(wasKeyPressed(DIK_LCONTROL))
	{
		keysPressed |= Key_Ctrl;
	}
	if(wasKeyPressed(DIK_RCONTROL))
	{
		keysPressed |= Key_Ctrl;
	}
	if(wasKeyPressed(DIK_BACKSPACE))
	{
		keysPressed |= Key_Backspace;
	}
	if(wasKeyPressed(DIK_1))
	{
		keysPressed |= Key_1;
	}
	if(wasKeyPressed(DIK_2))
	{
		keysPressed |= Key_2;
	}
	if(wasKeyPressed(DIK_3))
	{
		keysPressed |= Key_3;
	}
	if(wasKeyPressed(DIK_4))
	{
		keysPressed |= Key_4;
	}
	if(wasKeyPressed(DIK_5))
	{
		keysPressed |= Key_5;
	}
	if(wasKeyPressed(DIK_6))
	{
		keysPressed |= Key_6;
	}
	if(wasKeyPressed(DIK_7))
	{
		keysPressed |= Key_7;
	}
	if(wasKeyPressed(DIK_8))
	{
		keysPressed |= Key_8;
	}
	if(wasKeyPressed(DIK_9))
	{
		keysPressed |= Key_9;
	}
	if(wasKeyPressed(DIK_0))
	{
		keysPressed |= Key_0;
	}



	//if a game binded key is released send the right event to the eventhandler
	if(wasKeyReleased(DIK_Q))
	{
	}
	if(wasKeyReleased(DIK_W))
	{
	}
	if(wasKeyReleased(DIK_E))
	{
	}
	if(wasKeyReleased(DIK_R))
	{
	}
	if(wasKeyReleased(DIK_T))
	{
	}
	if(wasKeyReleased(DIK_Y))
	{
	}
	if(wasKeyReleased(DIK_U))
	{
	}
	if(wasKeyReleased(DIK_I))
	{
	}
	if(wasKeyReleased(DIK_O))
	{
	}
	if(wasKeyReleased(DIK_P))
	{
	}
	if(wasKeyReleased(DIK_A))
	{
	}
	if(wasKeyReleased(DIK_S))
	{
	}
	if(wasKeyReleased(DIK_D))
	{
	}
	if(wasKeyReleased(DIK_F))
	{
	}
	if(wasKeyReleased(DIK_G))
	{
	}
	if(wasKeyReleased(DIK_H))
	{
	}
	if(wasKeyReleased(DIK_J))
	{
	}
	if(wasKeyReleased(DIK_K))
	{
	}
	if(wasKeyReleased(DIK_L))
	{
	}
	if(wasKeyReleased(DIK_Z))
	{
	}
	if(wasKeyReleased(DIK_X))
	{
	}
	if(wasKeyReleased(DIK_C))
	{
	}
	if(wasKeyReleased(DIK_V))
	{
	}
	if(wasKeyReleased(DIK_B))
	{
	}
	if(wasKeyReleased(DIK_N))
	{
	}
	if(wasKeyReleased(DIK_M))
	{
	}
	if(wasKeyReleased(DIK_SPACE))
	{
	}
	if(wasKeyReleased(DIK_RETURN))
	{
	}
	if(wasKeyReleased(DIK_DOWN))
	{
	}
	if(wasKeyReleased(DIK_UP))
	{
	}
	if(wasKeyReleased(DIK_RIGHT))
	{
	}
	if(wasKeyReleased(DIK_LEFT))
	{
	}
	if(wasKeyReleased(DIK_LSHIFT))
	{
	}
	if(wasKeyReleased(DIK_RSHIFT))
	{
	}
	if(wasKeyReleased(DIK_TAB))
	{
	}
	if(wasKeyReleased(DIK_LCONTROL))
	{
	}
	if(wasKeyReleased(DIK_RCONTROL))
	{
	}
	if(wasKeyReleased(DIK_BACKSPACE))
	{
	}
	if(wasKeyReleased(DIK_1))
	{
	}
	if(wasKeyReleased(DIK_2))
	{
	}
	if(wasKeyReleased(DIK_3))
	{
	}
	if(wasKeyReleased(DIK_4))
	{
	}
	if(wasKeyReleased(DIK_5))
	{
	}
	if(wasKeyReleased(DIK_6))
	{
	}
	if(wasKeyReleased(DIK_7))
	{
	}
	if(wasKeyReleased(DIK_8))
	{
	}
	if(wasKeyReleased(DIK_9))
	{
	}
	if(wasKeyReleased(DIK_0))
	{
	}
	

	if(letterKeysPressed != 0 || keysPressed != 0)
		safeQueueEvent(EventDataPtr(myNew EvtData_System_KeysPressed(letterKeysPressed, keysPressed)));
}

void InputStates::gameInsideTankState()
{
	unsigned int letterKeysPressed = 0;
	unsigned int keysPressed = 0;
	//if a game binded key is pressed "for the first time" (not held down)
	//send the right event to the eventhandler
	if(wasKeyPressed(DIK_Q))
	{
		letterKeysPressed |= LetterKey_Q;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartRotate(mpPlayerInfo->getPlayerId(), true, true)));
	}
	if(wasKeyPressed(DIK_W))
	{
		letterKeysPressed |= LetterKey_W;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartMove(mpPlayerInfo->getPlayerId(), true)));
	}
	if(wasKeyPressed(DIK_E))
	{
		letterKeysPressed |= LetterKey_E;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartRotate(mpPlayerInfo->getPlayerId(), false, true)));
	}
	if(wasKeyPressed(DIK_R))
	{
		letterKeysPressed |= LetterKey_R;
	}
	if(wasKeyPressed(DIK_T))
	{
		letterKeysPressed |= LetterKey_T;
	}
	if(wasKeyPressed(DIK_Y))
	{
		letterKeysPressed |= LetterKey_Y;
	}
	if(wasKeyPressed(DIK_U))
	{
		letterKeysPressed |= LetterKey_U;
	}
	if(wasKeyPressed(DIK_I))
	{
		letterKeysPressed |= LetterKey_I;
	}
	if(wasKeyPressed(DIK_O))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_From_DriverEnterTank(mpPlayerInfo->getPlayerId())));
		letterKeysPressed |= LetterKey_O;
	}
	if(wasKeyPressed(DIK_P))
	{
		letterKeysPressed |= LetterKey_P;
	}
	if(wasKeyPressed(DIK_A))
	{
		letterKeysPressed |= LetterKey_A;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartRotate(mpPlayerInfo->getPlayerId(), true, false)));
	}
	if(wasKeyPressed(DIK_S))
	{
		letterKeysPressed |= LetterKey_S;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartMove(mpPlayerInfo->getPlayerId(), false)));
	}
	if(wasKeyPressed(DIK_D))
	{
		letterKeysPressed |= LetterKey_D;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartRotate(mpPlayerInfo->getPlayerId(), false, false)));
	}
	if(wasKeyPressed(DIK_F))
	{
		letterKeysPressed |= LetterKey_F;
	}
	if(wasKeyPressed(DIK_G))
	{
		letterKeysPressed |= LetterKey_G;
	}
	if(wasKeyPressed(DIK_H))
	{
		letterKeysPressed |= LetterKey_H;
	}
	if(wasKeyPressed(DIK_J))
	{
		letterKeysPressed |= LetterKey_J;
	}
	if(wasKeyPressed(DIK_K))
	{
		letterKeysPressed |= LetterKey_K;
	}
	if(wasKeyPressed(DIK_L))
	{
		letterKeysPressed |= LetterKey_L;
	}
	if(wasKeyPressed(DIK_Z))
	{
		letterKeysPressed |= LetterKey_Z;
	}
	if(wasKeyPressed(DIK_X))
	{
		letterKeysPressed |= LetterKey_X;
	}
	if(wasKeyPressed(DIK_C))
	{
		letterKeysPressed |= LetterKey_C;
	}
	if(wasKeyPressed(DIK_V))
	{
		letterKeysPressed |= LetterKey_V;
	}
	if(wasKeyPressed(DIK_B))
	{
		letterKeysPressed |= LetterKey_B;
	}
	if(wasKeyPressed(DIK_N))
	{
		letterKeysPressed |= LetterKey_N;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_CameraStartRotate(true)));
	}
	if(wasKeyPressed(DIK_M))
	{
		letterKeysPressed |= LetterKey_M;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_CameraStartRotate(false)));
	}
	if(wasKeyPressed(DIK_SPACE))
	{
		keysPressed |= Key_Space;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_TankStartFire(mpPlayerInfo->getPlayerId())));
	}
	if(wasKeyPressed(DIK_ESCAPE))
	{
		//pressedKeys |= DIK_ESCAPE;
		safeQueueEvent(EventDataPtr(myNew EvtData_System_EndGame()));
	}
	if(wasKeyPressed(DIK_RETURN))
	{
		keysPressed |= Key_Return;
#ifdef USE_EDITOR
		if(Application::getInstance()->mpGameOptions->mEditorMode || Application::getInstance()->mpGameOptions->mEnableEditor)
		{
			setState(InputStates::GameState_Editor);
			editor::EditorHandler::getInstance()->addMessage(myNew editor::EMActivate());
		}
#endif
	}
	if(wasKeyPressed(DIK_DOWN))
	{
		keysPressed |= Key_Down;
	}
	if(wasKeyPressed(DIK_UP))
	{
		keysPressed |= Key_Up;
	}
	if(wasKeyPressed(DIK_RIGHT))
	{
		keysPressed |= Key_Right;
	}
	if(wasKeyPressed(DIK_LEFT))
	{
		keysPressed |= Key_Left;
	}
	if(wasKeyPressed(DIK_RSHIFT))
	{
		keysPressed |= Key_Shift;
	}
	if(wasKeyPressed(DIK_LSHIFT))
	{
		keysPressed |= Key_Shift;
	}
	if(wasKeyPressed(DIK_TAB))
	{
		keysPressed |= Key_Tab;
	}
	if(wasKeyPressed(DIK_LCONTROL))
	{
		keysPressed |= Key_Ctrl;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_TankStartFireSecondary(mpPlayerInfo->getPlayerId())));
	}
	if(wasKeyPressed(DIK_RCONTROL))
	{
		keysPressed |= Key_Ctrl;
	}
	if(wasKeyPressed(DIK_BACKSPACE))
	{
		keysPressed |= Key_Backspace;
	}
	if(wasKeyPressed(DIK_1))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_TankChangeWeapon(mpPlayerInfo->getPlayerId(), Weapon::WeaponTypes_Minigun)));
		keysPressed |= Key_1;
	}
	if(wasKeyPressed(DIK_2))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_TankChangeWeapon(mpPlayerInfo->getPlayerId(), Weapon::WeaponTypes_Flamethrower)));
		keysPressed |= Key_2;
	}
	if(wasKeyPressed(DIK_3))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_TankChangeWeapon(mpPlayerInfo->getPlayerId(), Weapon::WeaponTypes_PulseCannon)));
		keysPressed |= Key_3;
	}
	if(wasKeyPressed(DIK_4))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_TankChangeWeapon(mpPlayerInfo->getPlayerId(), Weapon::WeaponTypes_Cannon)));
		keysPressed |= Key_4;
	}
	if(wasKeyPressed(DIK_5))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_TankChangeWeapon(mpPlayerInfo->getPlayerId(), Weapon::WeaponTypes_GaussCannon)));
		keysPressed |= Key_5;
	}
	if(wasKeyPressed(DIK_6))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_TankChangeWeapon(mpPlayerInfo->getPlayerId(), Weapon::WeaponTypes_BeamCannon)));
		keysPressed |= Key_6;
	}
	if(wasKeyPressed(DIK_7))
	{
		keysPressed |= Key_7;
	}
	if(wasKeyPressed(DIK_8))
	{
		keysPressed |= Key_8;
	}
	if(wasKeyPressed(DIK_9))
	{
		keysPressed |= Key_9;
	}
	if(wasKeyPressed(DIK_0))
	{
		keysPressed |= Key_0;
	}



	//if a game binded key is released send the right event to the eventhandler
	if(wasKeyReleased(DIK_Q))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopRotate(mpPlayerInfo->getPlayerId(), true, true)));
	}
	if(wasKeyReleased(DIK_W))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopMove(mpPlayerInfo->getPlayerId(), true)));
	}
	if(wasKeyReleased(DIK_E))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopRotate(mpPlayerInfo->getPlayerId(), false, true)));
	}
	if(wasKeyReleased(DIK_R))
	{
	}
	if(wasKeyReleased(DIK_T))
	{
	}
	if(wasKeyReleased(DIK_Y))
	{
	}
	if(wasKeyReleased(DIK_U))
	{
	}
	if(wasKeyReleased(DIK_I))
	{
	}
	if(wasKeyReleased(DIK_O))
	{
	}
	if(wasKeyReleased(DIK_P))
	{
	}
	if(wasKeyReleased(DIK_A))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopRotate(mpPlayerInfo->getPlayerId(), true, false)));
	}
	if(wasKeyReleased(DIK_S))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopMove(mpPlayerInfo->getPlayerId(), false)));
	}
	if(wasKeyReleased(DIK_D))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopRotate(mpPlayerInfo->getPlayerId(), false, false)));
	}
	if(wasKeyReleased(DIK_F))
	{
	}
	if(wasKeyReleased(DIK_G))
	{
	}
	if(wasKeyReleased(DIK_H))
	{
	}
	if(wasKeyReleased(DIK_J))
	{
	}
	if(wasKeyReleased(DIK_K))
	{
	}
	if(wasKeyReleased(DIK_L))
	{
	}
	if(wasKeyReleased(DIK_Z))
	{
	}
	if(wasKeyReleased(DIK_X))
	{
	}
	if(wasKeyReleased(DIK_C))
	{
	}
	if(wasKeyReleased(DIK_V))
	{
	}
	if(wasKeyReleased(DIK_B))
	{
	}
	if(wasKeyReleased(DIK_N))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_CameraStopRotate(true)));
	}
	if(wasKeyReleased(DIK_M))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_CameraStopRotate(false)));
	}
	if(wasKeyReleased(DIK_SPACE))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_TankStopFire(mpPlayerInfo->getPlayerId())));
	}
	if(wasKeyReleased(DIK_RETURN))
	{
	}
	if(wasKeyReleased(DIK_DOWN))
	{
	}
	if(wasKeyReleased(DIK_UP))
	{
	}
	if(wasKeyReleased(DIK_RIGHT))
	{
	}
	if(wasKeyReleased(DIK_LEFT))
	{
	}
	if(wasKeyReleased(DIK_LSHIFT))
	{
	}
	if(wasKeyReleased(DIK_RSHIFT))
	{
	}
	if(wasKeyReleased(DIK_TAB))
	{
	}
	if(wasKeyReleased(DIK_LCONTROL))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_TankStopFireSecondary(mpPlayerInfo->getPlayerId())));
	}
	if(wasKeyReleased(DIK_RCONTROL))
	{
	}
	if(wasKeyReleased(DIK_BACKSPACE))
	{
	}
	if(wasKeyReleased(DIK_1))
	{
	}
	if(wasKeyReleased(DIK_2))
	{
	}
	if(wasKeyReleased(DIK_3))
	{
	}
	if(wasKeyReleased(DIK_4))
	{
	}
	if(wasKeyReleased(DIK_5))
	{
	}
	if(wasKeyReleased(DIK_6))
	{
	}
	if(wasKeyReleased(DIK_7))
	{
	}
	if(wasKeyReleased(DIK_8))
	{
	}
	if(wasKeyReleased(DIK_9))
	{
	}
	if(wasKeyReleased(DIK_0))
	{
	}
	

	if(letterKeysPressed != 0 || keysPressed != 0)
		safeQueueEvent(EventDataPtr(myNew EvtData_System_KeysPressed(letterKeysPressed, keysPressed)));
}
void InputStates::gameOutsideTankState()
{
	unsigned int letterKeysPressed = 0;
	unsigned int keysPressed = 0;
	//if a game binded key is pressed "for the first time" (not held down)
	//send the right event to the eventhandler
	if(wasKeyPressed(DIK_Q))
	{
		letterKeysPressed |= LetterKey_Q;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_DriverStrafe(mpPlayerInfo->getPlayerId(), true,true)));
	}
	if(wasKeyPressed(DIK_W))
	{
		letterKeysPressed |= LetterKey_W;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartMove(mpPlayerInfo->getPlayerId(), true)));
	}
	if(wasKeyPressed(DIK_E))
	{
		letterKeysPressed |= LetterKey_E;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_DriverStrafe(mpPlayerInfo->getPlayerId(), false,true)));
	}
	if(wasKeyPressed(DIK_R))
	{
		letterKeysPressed |= LetterKey_R;
	}
	if(wasKeyPressed(DIK_T))
	{
		letterKeysPressed |= LetterKey_T;
	}
	if(wasKeyPressed(DIK_Y))
	{
		letterKeysPressed |= LetterKey_Y;
	}
	if(wasKeyPressed(DIK_U))
	{
		letterKeysPressed |= LetterKey_U;
	}
	if(wasKeyPressed(DIK_I))
	{
		letterKeysPressed |= LetterKey_I;
	}
	if(wasKeyPressed(DIK_O))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_From_DriverEnterTank(mpPlayerInfo->getPlayerId())));
		letterKeysPressed |= LetterKey_O;
	}
	if(wasKeyPressed(DIK_P))
	{
		letterKeysPressed |= LetterKey_P;
	}
	if(wasKeyPressed(DIK_A))
	{
		letterKeysPressed |= LetterKey_A;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartRotate(mpPlayerInfo->getPlayerId(), true, false)));
	}
	if(wasKeyPressed(DIK_S))
	{
		letterKeysPressed |= LetterKey_S;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartMove(mpPlayerInfo->getPlayerId(), false)));
	}
	if(wasKeyPressed(DIK_D))
	{
		letterKeysPressed |= LetterKey_D;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartRotate(mpPlayerInfo->getPlayerId(), false, false)));
	}
	if(wasKeyPressed(DIK_F))
	{
		letterKeysPressed |= LetterKey_F;
	}
	if(wasKeyPressed(DIK_G))
	{
		letterKeysPressed |= LetterKey_G;
	}
	if(wasKeyPressed(DIK_H))
	{
		letterKeysPressed |= LetterKey_H;
	}
	if(wasKeyPressed(DIK_J))
	{
		letterKeysPressed |= LetterKey_J;
	}
	if(wasKeyPressed(DIK_K))
	{
		letterKeysPressed |= LetterKey_K;
	}
	if(wasKeyPressed(DIK_L))
	{
		letterKeysPressed |= LetterKey_L;
	}
	if(wasKeyPressed(DIK_Z))
	{
		letterKeysPressed |= LetterKey_Z;
	}
	if(wasKeyPressed(DIK_X))
	{
		letterKeysPressed |= LetterKey_X;
	}
	if(wasKeyPressed(DIK_C))
	{
		letterKeysPressed |= LetterKey_C;
	}
	if(wasKeyPressed(DIK_V))
	{
		letterKeysPressed |= LetterKey_V;
	}
	if(wasKeyPressed(DIK_B))
	{
		letterKeysPressed |= LetterKey_B;
	}
	if(wasKeyPressed(DIK_N))
	{
		letterKeysPressed |= LetterKey_N;
	}
	if(wasKeyPressed(DIK_M))
	{
		letterKeysPressed |= LetterKey_M;
	}
	if(wasKeyPressed(DIK_SPACE))
	{
		keysPressed |= Key_Space;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_DriverStartFire(mpPlayerInfo->getPlayerId())));
	}
	if(wasKeyPressed(DIK_ESCAPE))
	{
		//pressedKeys |= DIK_ESCAPE;
		safeQueueEvent(EventDataPtr(myNew EvtData_System_EndGame()));
	}
	if(wasKeyPressed(DIK_RETURN))
	{
		keysPressed |= Key_Return;
#ifdef USE_EDITOR
		if(Application::getInstance()->mpGameOptions->mEditorMode || Application::getInstance()->mpGameOptions->mEnableEditor)
		{
			setState(InputStates::GameState_Editor);
			editor::EditorHandler::getInstance()->addMessage(myNew editor::EMActivate());
		}
#endif
	}
	if(wasKeyPressed(DIK_DOWN))
	{
		keysPressed |= Key_Down;
	}
	if(wasKeyPressed(DIK_UP))
	{
		keysPressed |= Key_Up;
	}
	if(wasKeyPressed(DIK_RIGHT))
	{
		keysPressed |= Key_Right;
	}
	if(wasKeyPressed(DIK_LEFT))
	{
		keysPressed |= Key_Left;
	}
	if(wasKeyPressed(DIK_RSHIFT))
	{
		keysPressed |= Key_Shift;
	}
	if(wasKeyPressed(DIK_LSHIFT))
	{
		keysPressed |= Key_Shift;
	}
	if(wasKeyPressed(DIK_TAB))
	{
		keysPressed |= Key_Tab;
	}
	if(wasKeyPressed(DIK_LCONTROL))
	{
		keysPressed |= Key_Ctrl;
	}
	if(wasKeyPressed(DIK_RCONTROL))
	{
		keysPressed |= Key_Ctrl;
	}
	if(wasKeyPressed(DIK_BACKSPACE))
	{
		keysPressed |= Key_Backspace;
	}
	if(wasKeyPressed(DIK_1))
	{
		keysPressed |= Key_1;
	}
	if(wasKeyPressed(DIK_2))
	{
		keysPressed |= Key_2;
	}
	if(wasKeyPressed(DIK_3))
	{
		keysPressed |= Key_3;
	}
	if(wasKeyPressed(DIK_4))
	{
		keysPressed |= Key_4;
	}
	if(wasKeyPressed(DIK_5))
	{
		keysPressed |= Key_5;
	}
	if(wasKeyPressed(DIK_6))
	{
		keysPressed |= Key_6;
	}
	if(wasKeyPressed(DIK_7))
	{
		keysPressed |= Key_7;
	}
	if(wasKeyPressed(DIK_8))
	{
		keysPressed |= Key_8;
	}
	if(wasKeyPressed(DIK_9))
	{
		keysPressed |= Key_9;
	}
	if(wasKeyPressed(DIK_0))
	{
		keysPressed |= Key_0;
	}



	//if a game binded key is released send the right event to the eventhandler
	if(wasKeyReleased(DIK_Q))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_DriverStrafe(mpPlayerInfo->getPlayerId(), true,false)));
	}
	if(wasKeyReleased(DIK_W))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopMove(mpPlayerInfo->getPlayerId(), true)));
	}
	if(wasKeyReleased(DIK_E))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_DriverStrafe(mpPlayerInfo->getPlayerId(), false,false)));
	}
	if(wasKeyReleased(DIK_R))
	{
	}
	if(wasKeyReleased(DIK_T))
	{
	}
	if(wasKeyReleased(DIK_Y))
	{
	}
	if(wasKeyReleased(DIK_U))
	{
	}
	if(wasKeyReleased(DIK_I))
	{
	}
	if(wasKeyReleased(DIK_O))
	{
	}
	if(wasKeyReleased(DIK_P))
	{
	}
	if(wasKeyReleased(DIK_A))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopRotate(mpPlayerInfo->getPlayerId(), true, false)));
	}
	if(wasKeyReleased(DIK_S))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopMove(mpPlayerInfo->getPlayerId(), false)));
	}
	if(wasKeyReleased(DIK_D))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopRotate(mpPlayerInfo->getPlayerId(), false, false)));
	}
	if(wasKeyReleased(DIK_F))
	{
	}
	if(wasKeyReleased(DIK_G))
	{
	}
	if(wasKeyReleased(DIK_H))
	{
	}
	if(wasKeyReleased(DIK_J))
	{
	}
	if(wasKeyReleased(DIK_K))
	{
	}
	if(wasKeyReleased(DIK_L))
	{
	}
	if(wasKeyReleased(DIK_Z))
	{
	}
	if(wasKeyReleased(DIK_X))
	{
	}
	if(wasKeyReleased(DIK_C))
	{
	}
	if(wasKeyReleased(DIK_V))
	{
	}
	if(wasKeyReleased(DIK_B))
	{
	}
	if(wasKeyReleased(DIK_N))
	{
	}
	if(wasKeyReleased(DIK_M))
	{
	}
	if(wasKeyReleased(DIK_SPACE))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_DriverStopFire(mpPlayerInfo->getPlayerId())));
	}
	if(wasKeyReleased(DIK_RETURN))
	{
	}
	if(wasKeyReleased(DIK_DOWN))
	{
	}
	if(wasKeyReleased(DIK_UP))
	{
	}
	if(wasKeyReleased(DIK_RIGHT))
	{
	}
	if(wasKeyReleased(DIK_LEFT))
	{
	}
	if(wasKeyReleased(DIK_LSHIFT))
	{
	}
	if(wasKeyReleased(DIK_RSHIFT))
	{
	}
	if(wasKeyReleased(DIK_TAB))
	{
	}
	if(wasKeyReleased(DIK_LCONTROL))
	{
	}
	if(wasKeyReleased(DIK_RCONTROL))
	{
	}
	if(wasKeyReleased(DIK_BACKSPACE))
	{
	}
	if(wasKeyReleased(DIK_1))
	{
	}
	if(wasKeyReleased(DIK_2))
	{
	}
	if(wasKeyReleased(DIK_3))
	{
	}
	if(wasKeyReleased(DIK_4))
	{
	}
	if(wasKeyReleased(DIK_5))
	{
	}
	if(wasKeyReleased(DIK_6))
	{
	}
	if(wasKeyReleased(DIK_7))
	{
	}
	if(wasKeyReleased(DIK_8))
	{
	}
	if(wasKeyReleased(DIK_9))
	{
	}
	if(wasKeyReleased(DIK_0))
	{
	}
	

	if(letterKeysPressed != 0 || keysPressed != 0)
		safeQueueEvent(EventDataPtr(myNew EvtData_System_KeysPressed(letterKeysPressed, keysPressed)));
}
void InputStates::editorState()
{
	unsigned int letterKeysPressed = 0;
	unsigned int keysPressed = 0;
	//if a game binded key is pressed "for the first time" (not held down)
	//send the right event to the eventhandler
	if(wasKeyPressed(DIK_Q))
	{
		letterKeysPressed |= LetterKey_Q;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartRotate(mpPlayerInfo->getPlayerId(), true, true)));
	}
	if(wasKeyPressed(DIK_W))
	{
		letterKeysPressed |= LetterKey_W;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartMove(mpPlayerInfo->getPlayerId(), true)));
	}
	if(wasKeyPressed(DIK_E))
	{
		letterKeysPressed |= LetterKey_E;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartRotate(mpPlayerInfo->getPlayerId(), false, true)));
	}
	if(wasKeyPressed(DIK_R))
	{
		letterKeysPressed |= LetterKey_R;
	}
	if(wasKeyPressed(DIK_T))
	{
		letterKeysPressed |= LetterKey_T;
	}
	if(wasKeyPressed(DIK_Y))
	{
		letterKeysPressed |= LetterKey_Y;
	}
	if(wasKeyPressed(DIK_U))
	{
		letterKeysPressed |= LetterKey_U;
	}
	if(wasKeyPressed(DIK_I))
	{
		letterKeysPressed |= LetterKey_I;
	}
	if(wasKeyPressed(DIK_O))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_From_DriverEnterTank(mpPlayerInfo->getPlayerId())));
		letterKeysPressed |= LetterKey_O;
	}
	if(wasKeyPressed(DIK_P))
	{
		letterKeysPressed |= LetterKey_P;
	}
	if(wasKeyPressed(DIK_A))
	{
		letterKeysPressed |= LetterKey_A;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartRotate(mpPlayerInfo->getPlayerId(), true, false)));
	}
	if(wasKeyPressed(DIK_S))
	{
		letterKeysPressed |= LetterKey_S;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartMove(mpPlayerInfo->getPlayerId(), false)));
	}
	if(wasKeyPressed(DIK_D))
	{
		letterKeysPressed |= LetterKey_D;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartRotate(mpPlayerInfo->getPlayerId(), false, false)));
	}
	if(wasKeyPressed(DIK_F))
	{
		letterKeysPressed |= LetterKey_F;
	}
	if(wasKeyPressed(DIK_G))
	{
		letterKeysPressed |= LetterKey_G;
	}
	if(wasKeyPressed(DIK_H))
	{
		letterKeysPressed |= LetterKey_H;
	}
	if(wasKeyPressed(DIK_J))
	{
		letterKeysPressed |= LetterKey_J;
	}
	if(wasKeyPressed(DIK_K))
	{
		letterKeysPressed |= LetterKey_K;
	}
	if(wasKeyPressed(DIK_L))
	{
		letterKeysPressed |= LetterKey_L;
	}
	if(wasKeyPressed(DIK_Z))
	{
		letterKeysPressed |= LetterKey_Z;
	}
	if(wasKeyPressed(DIK_X))
	{
		letterKeysPressed |= LetterKey_X;
	}
	if(wasKeyPressed(DIK_C))
	{
		letterKeysPressed |= LetterKey_C;
	}
	if(wasKeyPressed(DIK_V))
	{
		letterKeysPressed |= LetterKey_V;
	}
	if(wasKeyPressed(DIK_B))
	{
		letterKeysPressed |= LetterKey_B;
	}
	if(wasKeyPressed(DIK_N))
	{
		letterKeysPressed |= LetterKey_N;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_CameraStartRotate(true)));
	}
	if(wasKeyPressed(DIK_M))
	{
		letterKeysPressed |= LetterKey_M;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_CameraStartRotate(false)));
	}
	if(wasKeyPressed(DIK_SPACE))
	{
		keysPressed |= Key_Space;
	}
	if(wasKeyPressed(DIK_ESCAPE))
	{
		//pressedKeys |= DIK_ESCAPE;
		safeQueueEvent(EventDataPtr(myNew EvtData_System_EndGame()));
	}
	if(wasKeyPressed(DIK_RETURN))
	{
		keysPressed |= Key_Return;
#ifdef USE_EDITOR
		if(Application::getInstance()->mpGameOptions->mEditorMode || Application::getInstance()->mpGameOptions->mEnableEditor)
		{
			setState(mOldState);
			editor::EditorHandler::getInstance()->addMessage(myNew editor::EMDeactivate());
		}
#endif
	}
	if(wasKeyPressed(DIK_DOWN))
	{
		keysPressed |= Key_Down;
	}
	if(wasKeyPressed(DIK_UP))
	{
		keysPressed |= Key_Up;
	}
	if(wasKeyPressed(DIK_RIGHT))
	{
		keysPressed |= Key_Right;
	}
	if(wasKeyPressed(DIK_LEFT))
	{
		keysPressed |= Key_Left;
	}
	if(wasKeyPressed(DIK_RSHIFT))
	{
		keysPressed |= Key_Shift;
	}
	if(wasKeyPressed(DIK_LSHIFT))
	{
		keysPressed |= Key_Shift;
	}
	if(wasKeyPressed(DIK_TAB))
	{
		keysPressed |= Key_Tab;
	}
	if(wasKeyPressed(DIK_LCONTROL))
	{
		keysPressed |= Key_Ctrl;
	}
	if(wasKeyPressed(DIK_RCONTROL))
	{
		keysPressed |= Key_Ctrl;
	}
	if(wasKeyPressed(DIK_BACKSPACE))
	{
		keysPressed |= Key_Backspace;
	}
	if(wasKeyPressed(DIK_1))
	{
		keysPressed |= Key_1;
	}
	if(wasKeyPressed(DIK_2))
	{
		keysPressed |= Key_2;
	}
	if(wasKeyPressed(DIK_3))
	{
		keysPressed |= Key_3;
	}
	if(wasKeyPressed(DIK_4))
	{
		keysPressed |= Key_4;
	}
	if(wasKeyPressed(DIK_5))
	{
		keysPressed |= Key_5;
	}
	if(wasKeyPressed(DIK_6))
	{
		keysPressed |= Key_6;
	}
	if(wasKeyPressed(DIK_7))
	{
		keysPressed |= Key_7;
	}
	if(wasKeyPressed(DIK_8))
	{
		keysPressed |= Key_8;
	}
	if(wasKeyPressed(DIK_9))
	{
		keysPressed |= Key_9;
	}
	if(wasKeyPressed(DIK_0))
	{
		keysPressed |= Key_0;
	}


	//if a game binded key is released send the right event to the eventhandler
	if(wasKeyReleased(DIK_Q))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopRotate(mpPlayerInfo->getPlayerId(), true, true)));
	}
	if(wasKeyReleased(DIK_W))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopMove(mpPlayerInfo->getPlayerId(), true)));
	}
	if(wasKeyReleased(DIK_E))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopRotate(mpPlayerInfo->getPlayerId(), false, true)));
	}
	if(wasKeyReleased(DIK_R))
	{
	}
	if(wasKeyReleased(DIK_T))
	{
	}
	if(wasKeyReleased(DIK_Y))
	{
	}
	if(wasKeyReleased(DIK_U))
	{
	}
	if(wasKeyReleased(DIK_I))
	{
	}
	if(wasKeyReleased(DIK_O))
	{
	}
	if(wasKeyReleased(DIK_P))
	{
	}
	if(wasKeyReleased(DIK_A))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopRotate(mpPlayerInfo->getPlayerId(), true, false)));
	}
	if(wasKeyReleased(DIK_S))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopMove(mpPlayerInfo->getPlayerId(), false)));
	}
	if(wasKeyReleased(DIK_D))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopRotate(mpPlayerInfo->getPlayerId(), false, false)));
	}
	if(wasKeyReleased(DIK_F))
	{
	}
	if(wasKeyReleased(DIK_G))
	{
	}
	if(wasKeyReleased(DIK_H))
	{
	}
	if(wasKeyReleased(DIK_J))
	{
	}
	if(wasKeyReleased(DIK_K))
	{
	}
	if(wasKeyReleased(DIK_L))
	{
	}
	if(wasKeyReleased(DIK_Z))
	{
	}
	if(wasKeyReleased(DIK_X))
	{
	}
	if(wasKeyReleased(DIK_C))
	{
	}
	if(wasKeyReleased(DIK_V))
	{
	}
	if(wasKeyReleased(DIK_B))
	{
	}
	if(wasKeyReleased(DIK_N))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_CameraStopRotate(true)));
	}
	if(wasKeyReleased(DIK_M))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_CameraStopRotate(false)));
	}
	if(wasKeyReleased(DIK_SPACE))
	{
	}
	if(wasKeyReleased(DIK_RETURN))
	{
	}
	if(wasKeyReleased(DIK_DOWN))
	{
	}
	if(wasKeyReleased(DIK_UP))
	{
	}
	if(wasKeyReleased(DIK_RIGHT))
	{
	}
	if(wasKeyReleased(DIK_LEFT))
	{
	}
	if(wasKeyReleased(DIK_LSHIFT))
	{
	}
	if(wasKeyReleased(DIK_RSHIFT))
	{
	}
	if(wasKeyReleased(DIK_TAB))
	{
	}
	if(wasKeyReleased(DIK_LCONTROL))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_TankStopFireSecondary(mpPlayerInfo->getPlayerId())));
	}
	if(wasKeyReleased(DIK_RCONTROL))
	{
	}
	if(wasKeyReleased(DIK_BACKSPACE))
	{
	}
	if(wasKeyReleased(DIK_1))
	{
	}
	if(wasKeyReleased(DIK_2))
	{
	}
	if(wasKeyReleased(DIK_3))
	{
	}
	if(wasKeyReleased(DIK_4))
	{
	}
	if(wasKeyReleased(DIK_5))
	{
	}
	if(wasKeyReleased(DIK_6))
	{
	}
	if(wasKeyReleased(DIK_7))
	{
	}
	if(wasKeyReleased(DIK_8))
	{
	}
	if(wasKeyReleased(DIK_9))
	{
	}
	if(wasKeyReleased(DIK_0))
	{
	}
	
	if(letterKeysPressed != 0 || keysPressed != 0)
		safeQueueEvent(EventDataPtr(myNew EvtData_System_KeysPressed(letterKeysPressed, keysPressed)));
}
void InputStates::gameOverState()
{
	unsigned int letterKeysPressed = 0;
	unsigned int keysPressed = 0;
	//if a game binded key is pressed "for the first time" (not held down)
	//send the right event to the eventhandler
	if(wasKeyPressed(DIK_Q))
	{
		letterKeysPressed |= LetterKey_Q;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartRotate(mpPlayerInfo->getPlayerId(), true, true)));
	}
	if(wasKeyPressed(DIK_W))
	{
		letterKeysPressed |= LetterKey_W;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartMove(mpPlayerInfo->getPlayerId(), true)));
	}
	if(wasKeyPressed(DIK_E))
	{
		letterKeysPressed |= LetterKey_E;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartRotate(mpPlayerInfo->getPlayerId(), false, true)));
	}
	if(wasKeyPressed(DIK_A))
	{
		letterKeysPressed |= LetterKey_A;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartRotate(mpPlayerInfo->getPlayerId(), true, false)));
	}
	if(wasKeyPressed(DIK_S))
	{
		letterKeysPressed |= LetterKey_S;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartMove(mpPlayerInfo->getPlayerId(), false)));
	}
	if(wasKeyPressed(DIK_D))
	{
		letterKeysPressed |= LetterKey_D;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartRotate(mpPlayerInfo->getPlayerId(), false, false)));
	}
	if(wasKeyPressed(DIK_N))
	{
		letterKeysPressed |= LetterKey_N;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_CameraStartRotate(true)));
	}
	if(wasKeyPressed(DIK_M))
	{
		letterKeysPressed |= LetterKey_M;
		safeQueueEvent(EventDataPtr(myNew EvtData_To_CameraStartRotate(false)));
	}
	if(wasKeyPressed(DIK_O))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_From_DriverEnterTank(mpPlayerInfo->getPlayerId())));
		letterKeysPressed |= LetterKey_O;
	}
	if(wasKeyPressed(DIK_ESCAPE))
	{
		//pressedKeys |= DIK_ESCAPE;
		safeQueueEvent(EventDataPtr(myNew EvtData_System_EndGame()));
	}

	//if a game binded key is released send the right event to the eventhandler
	if(wasKeyReleased(DIK_Q))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopRotate(mpPlayerInfo->getPlayerId(), true, true)));
	}
	if(wasKeyReleased(DIK_W))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopMove(mpPlayerInfo->getPlayerId(), true)));
	}
	if(wasKeyReleased(DIK_E))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopRotate(mpPlayerInfo->getPlayerId(), false, true)));
	}
	if(wasKeyReleased(DIK_A))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopRotate(mpPlayerInfo->getPlayerId(), true, false)));
	}
	if(wasKeyReleased(DIK_S))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopMove(mpPlayerInfo->getPlayerId(), false)));
	}
	if(wasKeyReleased(DIK_D))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopRotate(mpPlayerInfo->getPlayerId(), false, false)));
	}
	if(wasKeyReleased(DIK_N))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_CameraStopRotate(true)));
	}
	if(wasKeyReleased(DIK_M))
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_CameraStopRotate(false)));
	}

	if(letterKeysPressed != 0 || keysPressed != 0)
		safeQueueEvent(EventDataPtr(myNew EvtData_System_KeysPressed(letterKeysPressed, keysPressed)));
}

void InputStates::allStates()
{
	// Kill our tank
	if (wasKeyPressed(DIK_F11))
	{
		KeyType key = mpPlayerInfo->getTankActorId();
		if (key!=INVALID_KEY)
		{
			dynamic_cast<Tank*>(ActorHandler::getInstance()->getActor(key))->reduceHp(20010);
		}
	}
	else if (wasKeyReleased(DIK_F11))
	{

	}

	// Pause / Resume AI
	if (wasKeyPressed(DIK_F12))
	{
		safeQueueEvent(EventDataPtr(myNew ai::EvtData_To_AiTogglePause()));
	}
	else if (wasKeyReleased(DIK_F12))
	{

	}
}