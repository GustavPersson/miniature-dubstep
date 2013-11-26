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
*/
#include "../Utilities/Macros.h"
#include "EventMsgHandler.h"
#include "Protocol.h"
#include "../Logic/Events.h"
#include "../Ai/AiEvents.h"
#include "../Logic/PlayerInfo.h"
#include <iostream>
#include <sstream>
#include <winsock.h>

using namespace network;

EventMsgHandler::EventMsgHandler()
{
}

void EventMsgHandler::handleMessage(SOCKET src, int len, int type, const char * message)
{
	std::istrstream in(message, len);

	switch(type)
	{
	case MessageType_Event:
		char eventType[256];
		int networkId;
		in >> eventType;
		in >> networkId;
		if ( networkId != logic::PlayerInfo::getInstance()->threadSafeGetNetworkId())
		{
			if (!_stricmp(eventType, logic::EvtData_To_ActorStartMove::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorStartMove(in,networkId)));
			}

			else if (!_stricmp(eventType, logic::EvtData_To_ActorStopMove::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorStopMove(in, networkId)));
			}

			else if (!_stricmp(eventType, logic::EvtData_To_ActorStartRotate::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorStartRotate(in, networkId)));
			}

			else if (!_stricmp(eventType, logic::EvtData_To_ActorStopRotate::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorStopRotate(in, networkId)));
			}

			else if (!_stricmp(eventType, logic::EvtData_System_StartGame::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_System_StartGame(in, networkId)));
			}

			else if (!_stricmp(eventType, logic::EvtData_System_StartLoading::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_System_StartLoading(in, networkId)));
			}
		
			else if (!_stricmp(eventType, logic::EvtData_System_FinishedLoading::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_System_FinishedLoading(in, networkId)));
			}

			else if (!_stricmp(eventType, logic::EvtData_System_MapVictory::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_System_MapVictory(in, networkId)));
			}

			else if (!_stricmp(eventType, logic::EvtData_System_MapDefeat::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_System_MapDefeat(in, networkId)));
			}

			else if(!_stricmp(eventType, logic::EvtData_To_TankStartFire::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_TankStartFire(in, networkId)));
			}

			else if(!_stricmp(eventType, logic::EvtData_To_TankStopFire::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_TankStopFire(in, networkId)));
			}
			
			else if(!_stricmp(eventType, logic::EvtData_To_ActorCreateTank::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateTank(in, networkId)));
			}

			else if(!_stricmp(eventType, logic::EvtData_To_ActorCreateEnemy::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateEnemy(in, networkId)));
			}

			else if(!_stricmp(eventType, logic::EvtData_To_ActorCreateEnemyID::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateEnemyID(in, networkId)));
			}

			else if(!_stricmp(eventType, logic::EvtData_To_ActorCreateMunition::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateMunition(in, networkId)));
			}

			else if(!_stricmp(eventType, logic::EvtData_To_ActorCreateDriver::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateDriver(in, networkId)));
			}
			else if(!_stricmp(eventType, logic::EvtData_To_ActorCreateSpectator::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateSpectator(in, networkId)));
			}

			else if(!_stricmp(eventType, logic::EvtData_To_ActorReduceHp::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorReduceHp(in, networkId)));
			}

			else if(!_stricmp(eventType, logic::EvtData_To_ActorCreateStaticEnvironment::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateStaticEnvironment(in, networkId)));
			}

			else if(!_stricmp(eventType, logic::EvtData_From_ActorDestroyed::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_From_ActorDestroyed(in, networkId)));
			}

			else if(!_stricmp(eventType, logic::EvtData_From_ActorHandlerSyncActors::msEventType.getStr().c_str()))
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_From_ActorHandlerSyncActors(in, networkId)));
			}

			else if(eventType == ai::EvtData_To_AgentAiGoals::msEventType.getStr())
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew ai::EvtData_To_AgentAiGoals(in, networkId)));
			}

			else if (eventType == ai::EvtData_To_AiAddToConflictMap::msEventType.getStr())
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew ai::EvtData_To_AiAddToConflictMap(in, networkId)));
			}

			else if (eventType == ai::EvtData_To_AiRemoveFromConflictMap::msEventType.getStr())
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew ai::EvtData_To_AiRemoveFromConflictMap(in, networkId)));
			}

			else if (eventType == ai::EvtData_To_AgentAiTarget::msEventType.getStr())
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew ai::EvtData_To_AgentAiTarget(in, networkId)));
			}

			else if (eventType == ai::EvtData_From_JumperAiJump::msEventType.getStr())
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew ai::EvtData_From_JumperAiJump(in, networkId)));
			}

			else if (eventType == ai::EvtData_To_AiTogglePause::msEventType.getStr())
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew ai::EvtData_To_AiTogglePause(in, networkId)));
			}
			else if (eventType == logic::EvtData_From_WeaponFireMeshMunition::msEventType.getStr())
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_From_WeaponFireMeshMunition(in, networkId)));
			}
			else if (eventType == logic::EvtData_From_WeaponFireRayMunition::msEventType.getStr())
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_From_WeaponFireRayMunition(in, networkId)));
			}
			else if (eventType == logic::EvtData_To_ActorCreateStaticEnvironmentID::msEventType.getStr())
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateStaticEnvironmentID(in, networkId)));
			}
			else if (eventType == logic::EvtData_To_ActorDestroyAll::msEventType.getStr())
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorDestroyAll(in, networkId)));
			}
			else if (eventType == logic::EvtData_To_ActorDestroyAllNonPlayer::msEventType.getStr())
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorDestroyAllNonPlayer(in, networkId)));
			}
			else if (eventType == logic::EvtData_To_IncreaseMapWeights::msEventType.getStr())
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_IncreaseMapWeights(in, networkId)));
			}
			else if (eventType == logic::EvtData_To_SetMapWeights::msEventType.getStr())
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_SetMapWeights(in, networkId)));
			}
			else if (eventType == logic::EvtData_From_EnemySmallExplosion::msEventType.getStr())
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_From_EnemySmallExplosion(in, networkId)));
			}
			else if (eventType == logic::EvtData_To_TankChangeWeapon::msEventType.getStr())
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_TankChangeWeapon(in, networkId)));
			}
			else if (eventType == logic::EvtData_From_WeaponFired::msEventType.getStr())
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_From_WeaponFired(in, networkId)));
			}
			else if (eventType == logic::EvtData_System_EnterEditorMode::msEventType.getStr())
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_System_EnterEditorMode(in, networkId)));
			}
			else if (eventType == logic::EvtData_System_ExitEditorMode::msEventType.getStr())
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_System_ExitEditorMode(in, networkId)));
			}
			else if(eventType == logic::EvtData_From_EnemyHitNetwork::msEventType.getStr())
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_From_EnemyHitNetwork(in, networkId)));
			}
			else
			{
				ERROR_MESSAGE("Unknown event type from remote: " << eventType);
			}
		}
		else
		{
			ERROR_MESSAGE("Received message originating from self: " << eventType);
		}
		break;

	case MessageType_Dropout:
		if (WSAGetLastError() != 0)
		{
			int playerId = IdManager::getInstance()->getIdBySocket(src);
			if(playerId != -1)
			{
				logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_System_PlayerDropped(playerId)));
			}
			
			//Reset error status.
			WSASetLastError(0);
		}
		
		break;
	}
}