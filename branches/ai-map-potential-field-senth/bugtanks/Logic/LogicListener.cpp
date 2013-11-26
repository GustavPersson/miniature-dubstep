/**
 * @file
 * @author Gustav Persson <ninjamuff@gmail.com>
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
 * An event listener for the logic component.
 */
#include "../Utilities/Macros.h"

#include "LogicListener.h"
#include "Weapon.h"
#include "InputStates.h"
#include "../GFX//DX10Module.h"
#include "Application.h"

using namespace logic;

LogicListener::LogicListener()
{
}

LogicListener::~LogicListener()
{
}

bool LogicListener::handleEvent(const logic::EventData &mEvent)
{
	if(EvtData_System_StartGame::msEventType == mEvent.getEventType())
	{
		Application::getInstance()->setGameState(GameState_Game);
	}
	else if(EvtData_System_StartLoading::msEventType == mEvent.getEventType())
	{
		const EvtData_System_StartLoading& evtData = static_cast<const EvtData_System_StartLoading&>(mEvent);

		Application::getInstance()->setGameState(GameState_Starting);
		Application::getInstance()->mpGameOptions->mEditorMode = evtData.mEditorEnabled;
	}
	else if(EvtData_System_FinishedLoading::msEventType == mEvent.getEventType())
	{
		DEBUG_MESSAGE(LEVEL_LOW, "Player nr: " << mEvent.getNetworkId() << " finished loading.");
		Application::getInstance()->increaseFinishedLoading();
	}
	else if(EvtData_System_EndGame::msEventType == mEvent.getEventType())
	{
		ActorHandler::getInstance()->destroyDynamicActors();
		PostMessage(gfx::DX10Module::getInstance()->getHWND(), WM_QUIT, 0, 0);
	}
	else if(EvtData_System_PlayerDropped::msEventType == mEvent.getEventType())
	{
		const EvtData_System_PlayerDropped& evtData = static_cast<const EvtData_System_PlayerDropped&>(mEvent);
		ActorHandler::getInstance()->destroyDynamicActors(evtData.mPlayerId);
	}
	else if(EvtData_From_ActorCreated::msEventType == mEvent.getEventType())
	{

	}
	else if(EvtData_From_ActorDestroyed::msEventType == mEvent.getEventType())
	{

	}
	else if(EvtData_From_DriverDeath::msEventType == mEvent.getEventType())
	{

	}
	/*else if(EvtData_From_ActorCollision::msEventType == mEvent.getEventType())
	{

	}*/
	else if(EvtData_To_ActorStartMove::msEventType == mEvent.getEventType())
	{

	}
	else if(EvtData_System_EventSystemStarted::msEventType == mEvent.getEventType())
	{

	}
	else if(EvtData_From_EnemyHit::msEventType == mEvent.getEventType())
	{
		return false;
	}
	else if(EvtData_To_InputChangeGameState::msEventType == mEvent.getEventType())
	{
		const EvtData_To_InputChangeGameState& mEvtData = static_cast<const EvtData_To_InputChangeGameState&>(mEvent);
		InputStates::getInstance()->setState(mEvtData.mState);
		return false;
	}
	
	return false;
}
