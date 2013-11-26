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
#include "EventManager.h"
#include "Actor.h"
#include "PlayerInfo.h"
#include "GlobalTimer.h"

using namespace logic;

InputStates::InputStates(HINSTANCE *pHInstance, HWND *pHWnd)
{
	//Create a new InputHandler.
	mpInputHandler = myNew InputHandler(pHInstance, pHWnd);
	setState(GameState_Menu);

	for(int i=0;i<256;i++)
		mKeyPressed[i] = false;
	for(int i=0;i<8;i++)
		mButtonPressed[i] = false;
	for(int i=0;i<256;i++)
		mTimeKeyPressed[i] = 0;
	for(int i=0;i<8;i++)
		mTimeButtonPressed[i] = 0;

}

InputStates::~InputStates() 
{
	//Delete the InputHandler.
	SAFE_DELETE(mpInputHandler);
}

void InputStates::setState(GameStates state)
{
	mState = state;
}

bool InputStates::checkKeyDown(BYTE key)
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

bool InputStates::checkKeyUp(BYTE key)
{
	bool temp = false;
	if(!mpInputHandler->keyDown(key) && mKeyPressed[key])
	{
		mKeyPressed[key] = false;
		mTimeKeyPressed[key] = mKeyTimers[key].getTime(Timer::ReturnType_MilliSeconds);
		temp = true;
	}	
	return temp;
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
	default:
		ERROR_MESSAGE(20002, "Invalid state is set, no keys are updated in InputStates update().");
		break;
	}
}

void InputStates::menuState()
{
	//if a menu binded key is pressed "for the first time" (not held down)
	//send the right event to the eventhandler

	//if a game binded key is released send the right event to the eventhandler
}

void InputStates::gameInsideTankState()
{
	//if a game binded key is pressed "for the first time" (not held down)
	//send the right event to the eventhandler
	if(checkKeyDown(DIK_Q))
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartRotate(PlayerInfo::getInstance()->getTankActorId(), true, true, GlobalTimer::getInstance()->getTime(logic::Timer::ReturnType_MilliSeconds))));
	if(checkKeyDown(DIK_W))
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartMove(PlayerInfo::getInstance()->getTankActorId(), true, GlobalTimer::getInstance()->getTime(logic::Timer::ReturnType_MilliSeconds))));
	if(checkKeyDown(DIK_E))
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartRotate(PlayerInfo::getInstance()->getTankActorId(), false, true, GlobalTimer::getInstance()->getTime(logic::Timer::ReturnType_MilliSeconds))));
	if(checkKeyDown(DIK_A))
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartRotate(PlayerInfo::getInstance()->getTankActorId(), true, false, GlobalTimer::getInstance()->getTime(logic::Timer::ReturnType_MilliSeconds))));
	if(checkKeyDown(DIK_S))
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartMove(PlayerInfo::getInstance()->getTankActorId(), false, GlobalTimer::getInstance()->getTime(logic::Timer::ReturnType_MilliSeconds))));
	if(checkKeyDown(DIK_D))
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStartRotate(PlayerInfo::getInstance()->getTankActorId(), false, false, GlobalTimer::getInstance()->getTime(logic::Timer::ReturnType_MilliSeconds))));
	if(checkKeyDown(DIK_SPACE))
		safeQueueEvent(EventDataPtr(myNew EvtData_To_TankStartFire(PlayerInfo::getInstance()->getTankActorId(), GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))));
	if(checkKeyDown(DIK_ESCAPE))
		safeQueueEvent(EventDataPtr(myNew EvtData_System_EndGame()));
	
	//--------- Keys used for testing ----------//
	if(checkKeyDown(DIK_P))
		safeQueueEvent(EventDataPtr(myNew EvtData_To_TankStartFire(10001, GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))));
	//--------- Keys used for testing ----------//


	//if a game binded key is released send the right event to the eventhandler
	if(checkKeyUp(DIK_Q))
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopRotate(PlayerInfo::getInstance()->getTankActorId(), true, true, GlobalTimer::getInstance()->getTime(logic::Timer::ReturnType_MilliSeconds))));
	if(checkKeyUp(DIK_W))
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopMove(PlayerInfo::getInstance()->getTankActorId(), true, GlobalTimer::getInstance()->getTime(logic::Timer::ReturnType_MilliSeconds))));
	if(checkKeyUp(DIK_E))
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopRotate(PlayerInfo::getInstance()->getTankActorId(), false, true, GlobalTimer::getInstance()->getTime(logic::Timer::ReturnType_MilliSeconds))));
	if(checkKeyUp(DIK_A))
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopRotate(PlayerInfo::getInstance()->getTankActorId(), true, false, GlobalTimer::getInstance()->getTime(logic::Timer::ReturnType_MilliSeconds))));
	if(checkKeyUp(DIK_S))
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopMove(PlayerInfo::getInstance()->getTankActorId(), false, GlobalTimer::getInstance()->getTime(logic::Timer::ReturnType_MilliSeconds))));
	if(checkKeyUp(DIK_D))
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorStopRotate(PlayerInfo::getInstance()->getTankActorId(), false, false, GlobalTimer::getInstance()->getTime(logic::Timer::ReturnType_MilliSeconds))));
	if(checkKeyUp(DIK_SPACE))
		safeQueueEvent(EventDataPtr(myNew EvtData_To_TankStopFire(PlayerInfo::getInstance()->getTankActorId(), GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))));
	

	//--------- Keys used for testing ----------//
	//--------- Keys used for testing ----------//
}
void InputStates::gameOutsideTankState()
{
	//if a game binded key is pressed "for the first time" (not held down)
	//send the right event to the eventhandler
		
	//if a game binded key is released send the right event to the eventhandler
}
void InputStates::editorState()
{
	//if a game binded key is pressed "for the first time" (not held down)
	//send the right event to the eventhandler
		
	//if a game binded key is released send the right event to the eventhandler
}
