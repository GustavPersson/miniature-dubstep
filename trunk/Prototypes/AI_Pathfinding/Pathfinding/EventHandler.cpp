/**
* @file
* @author Matteus Magnusson <senth.wallace@gmail.com>
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
* Handles all the events, queues them etc.
*/

#include "Macros.h"
#include "EventHandler.h"
#include "Event.h"
#include "Enemy.h"
#include "AgentHandler.h"
#include "EnemyHandler.h"
#include "EventRouter.h"

EventHandler::EventHandler(int client, AgentHandler *pAgentHandler, EnemyHandler *pEnemyHandler, PathFinder* pPathFinder)
{
	mClient = client;
	mpAgentHandler = pAgentHandler;
	mpEnemyHandler = pEnemyHandler;
	mpEventRouter = EventRouter::getInstance();
	mpPathFinder = pPathFinder;
	mpKeyHandler = KeyHandler::getInstance(mClient);
	
	pthread_mutex_init(&mQueueMutex, NULL);

	// Add itself as an event router listener
	mpEventRouter->addListener(this);
}

EventHandler::~EventHandler()
{
	mpAgentHandler = NULL;
	mpEnemyHandler = NULL;
	mpEventRouter = NULL;
	mpKeyHandler = NULL;
	mpPathFinder = NULL;

	pthread_mutex_destroy(&mQueueMutex);
}

void EventHandler::pushEvent(const Event *pEvent)
{
	pthread_mutex_lock(&mQueueMutex);
	mEvents.push(new Event(*pEvent));
	pthread_mutex_unlock(&mQueueMutex);
}

void EventHandler::processEvents()
{
	// Process all events in the queue
	pthread_mutex_lock(&mQueueMutex);
	while (!mEvents.empty())
	{
		Event* pEvent = mEvents.front();
		mEvents.pop();
		pthread_mutex_unlock(&mQueueMutex);

		switch (pEvent->getEventType())
		{
			case Event_EnemyCreate:
			{
				FixedCoordinate pos = GET_FIXED_COORD(pEvent);
				Enemy* pEnemy = new Enemy(ENEMY_WIDTH, ENEMY_HEIGHT, pos, ENEMY_RANGE);
				KeyType key = mpKeyHandler->allocateKey(pEvent->getPlayerId());
				mpPathFinder->addEnemyToEnemyPotentialMap(pEnemy);
				mpEnemyHandler->addEnemy(pEnemy, key);
				DEBUG_MESSAGE(mClient << " | Event_EnemyCreate - Enemy: " << pEvent->getKey() << " (" << pos.x << ", " << pos.y << ")");
				break;
			}

			case Event_EnemyDestroy:
				mpEnemyHandler->removeEnemy(pEvent->getKey());
				DEBUG_MESSAGE(mClient << " | Event_EnemyDestroy - Enemy: " << pEvent->getKey());
				break;
			
			case Event_AgentCreate:
			{
				FixedCoordinate pos = GET_FIXED_COORD(pEvent);
				KeyType key = mpKeyHandler->allocateKey(pEvent->getPlayerId());
				mpAgentHandler->addAgent(new Agent(pos, key), key);
				DEBUG_MESSAGE(mClient << " | Event_AgentCreate - Agent: " << key << " (" << pos.x << ", " << pos.y << ")");
				break;
			}

			case Event_AgentDestroy:
				mpAgentHandler->removeAgent(pEvent->getKey());
				DEBUG_MESSAGE(mClient << " | Event_AgentDestroy - Agent: " << pEvent->getKey());
				break;

			case Event_AgentGoalPosition:
			{
				FixedCoordinate pos = GET_FIXED_COORD(pEvent);
				Agent* pAgent = mpAgentHandler->checkOutAgent(pEvent->getKey());
				//pAgent->setGoalPosition(pos);
				mpAgentHandler->checkInAgent(pEvent->getKey());
				DEBUG_MESSAGE(mClient << " | Event_AgentGoalPosition - Agent: " << pEvent->getKey() << " (" << pos.x << ", " << pos.y << ")");
				break;
			}

			case Event_AgentNextGoalPosition:
			{
				FixedCoordinate pos = GET_FIXED_COORD(pEvent);
				Agent* pAgent = mpAgentHandler->checkOutAgent(pEvent->getKey());
				//pAgent->setNextGoalPosition(pos);
				mpAgentHandler->checkInAgent(pEvent->getKey());
				DEBUG_MESSAGE(mClient << " | Event_AgentNextGoalPosition - Agent: " << pEvent->getKey() << " (" << pos.x << ", " << pos.y << ")");
				break;
			}

			case Event_AgentDirection:
			{
				Direction dir = GET_VEC2(pEvent);
				Agent* pAgent = mpAgentHandler->checkOutAgent(pEvent->getKey());
				pAgent->setDirection(dir);
				mpAgentHandler->checkInAgent(pEvent->getKey());
				DEBUG_MESSAGE(mClient << " | Event_AgentDirection - Agent: " << pEvent->getKey() << " (" << dir.x << ", " << dir.y << ")");
				break;
			}

			default:
				DEBUG_MESSAGE("Unhandled Message");
				break;			
		}

		// Delete the event
		SAFE_DELETE(pEvent);

		pthread_mutex_lock(&mQueueMutex);
	}
	pthread_mutex_unlock(&mQueueMutex);
}