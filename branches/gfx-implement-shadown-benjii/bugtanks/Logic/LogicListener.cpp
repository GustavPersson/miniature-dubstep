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

#include "LogicListener.h"
#include "Weapon.h"
#include "InputStates.h"
#include "../GFX//DX10Module.h"
#include "../Utilities/Macros.h"

using namespace logic;

LogicListener::LogicListener()
{
}

LogicListener::~LogicListener()
{
}

bool LogicListener::handleEvent(const logic::EventData &mEvent)
{
	if(EvtData_System_StartGame::mEventType == mEvent.getEventType())
	{

	}
	else if(EvtData_System_EndGame::mEventType == mEvent.getEventType())
	{
		gfx::DX10Module::getInstance()->exit();
	}
	else if(EvtData_From_ActorCreated::mEventType == mEvent.getEventType())
	{

	}
	else if(EvtData_From_ActorDestroyed::mEventType == mEvent.getEventType())
	{

	}
	else if(EvtData_From_DriverDeath::mEventType == mEvent.getEventType())
	{

	}
	else if(EvtData_From_ActorCollision::mEventType == mEvent.getEventType())
	{

	}
	else if(EvtData_To_ActorStartMove::mEventType == mEvent.getEventType())
	{

	}
	else if(EvtData_System_EventSystemStarted::mEventType == mEvent.getEventType())
	{

	}
	else if(EvtData_From_EnemyHit::mEventType == mEvent.getEventType())
	{
		return false;
	}
	else if(EvtData_To_InputChangeGameState::mEventType == mEvent.getEventType())
	{
		const EvtData_To_InputChangeGameState& mEvtData = static_cast<const EvtData_To_InputChangeGameState&>(mEvent);
		mEvtData.mpInputStates->setState(mEvtData.mState);
		return false;
	}
	
	return false;
}
