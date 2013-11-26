/**
 * @file
 * @author Viktor Svensson <arkangel88@gmail.com>
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
 * An event listener for the sound component.
 */



#include "SoundListener.h"
#include "../Logic/InputStates.h"
#include "../Logic/Events.h"
#include "../Logic/PlayerInfo.h"
#include "../Logic/ActorHandler.h"
#include "../Logic/Actor.h"

using namespace sound;

SoundListener::SoundListener()
{
	//mpsActorHandler = logic::ActorHandler::getInstance();
	soundSystem = sound::SoundSystem::getInstance();
}

SoundListener::~SoundListener()
{
}

bool SoundListener::handleEvent(const logic::EventData &mEvent)
{
	
	/*if(logic::EvtData_To_ActorStartMove::mEventType == mEvent.getEventType()) //TODO: check for more actors
	{
		const logic::EvtData_To_ActorStartMove& evtData = static_cast<const logic::EvtData_To_ActorStartMove&>(mEvent);
		if(logic::Actor::ActorType_Tank == logic::ActorHandler::getInstance()->getActor(evtData.mId)->getActorType())
		{
			//TODO: change soundID
			soundSystem->playSound(sound::Sound_PlayerTankLongRangeMovementMoving, logic::ActorHandler::getInstance()->getActor(evtData.mId)->getKey(), 0); 
		}
	}

	if(logic::EvtData_To_ActorStopMove::mEventType == mEvent.getEventType())  //TODO: check for more actors
	{
		const logic::EvtData_To_ActorStopMove& evtData = static_cast<const logic::EvtData_To_ActorStopMove&>(mEvent);
		if(logic::Actor::ActorType_Tank == logic::ActorHandler::getInstance()->getActor(evtData.mId)->getActorType())
		{
			soundSystem->SoundOff(logic::ActorHandler::getInstance()->getActor(evtData.mId)->getKey(), 0);
		}
	}*/

	if(logic::EvtData_From_ActorCreated::mEventType == mEvent.getEventType()) 
	{
		const logic::EvtData_From_ActorCreated& evtData = static_cast<const logic::EvtData_From_ActorCreated&>(mEvent);
		soundSystem->createChannel(evtData.mId);
		/*if(logic::Actor::ActorType_Tank == logic::ActorHandler::getInstance()->getActor(evtData.mId)->getActorType())
		{
			soundSystem->createChannel(evtData.mId);
		}*/
	}

	if(logic::EvtData_From_ActorDestroyed::mEventType == mEvent.getEventType()) 
	{
		const logic::EvtData_From_ActorDestroyed& evtData = static_cast<const logic::EvtData_From_ActorDestroyed&>(mEvent);
		soundSystem->stopChannel(evtData.mId);
		/*if(logic::Actor::ActorType_Tank == logic::ActorHandler::getInstance()->getActor(evtData.mId)->getActorType())
		{
			soundSystem->stopChannel(evtData.mId);
		}*/
	}

	if(logic::EvtData_From_WeaponFired::mEventType == mEvent.getEventType()) //TODO: check for other weapons
	{
		//const logic::EvtData_From_WeaponFired& evtData = static_cast<const logic::EvtData_From_WeaponFired&>(mEvent);
		soundSystem->playSound(sound::Sound_WeaponMinigunFire, logic::ActorHandler::getInstance()->getActor(logic::PlayerInfo::getInstance()->getTankActorId())->getKey(), Channel_Attack); 
		
	}

	/*if(logic::EvtData_To_ActorStartRotate::mEventType == mEvent.getEventType()) //TODO: check for more actors
	{
		//const logic::EvtData_To_ActorStartRotate& evtData = static_cast<const logic::EvtData_To_ActorStartRotate&>(mEvent);
		soundSystem->playSound(sound::Sound_PlayerTankLongRangeMovementMoving, logic::ActorHandler::getInstance()->getActor(logic::PlayerInfo::getInstance()->getTankActorId())->getKey(), Channel_Rotation); 
		
	}

	if(logic::EvtData_To_ActorStopRotate::mEventType == mEvent.getEventType()) //TODO: check for more actors
	{
		const logic::EvtData_To_ActorStopRotate& evtData = static_cast<const logic::EvtData_To_ActorStopRotate&>(mEvent);
		//if(logic::Actor::ActorType_Tank == logic::ActorHandler::getInstance()->getActor(evtData.mId)->getActorType())
		//{
			soundSystem->SoundOff(logic::ActorHandler::getInstance()->getActor(evtData.mId)->getKey(), Channel_Rotation);
		//}
		
	}*/


	
	return false;
}
