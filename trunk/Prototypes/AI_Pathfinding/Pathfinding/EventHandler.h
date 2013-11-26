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

#ifndef __EVENT_HANDLER_H__
#define __EVENT_HANDLER_H__

#include "KeyHandler.h"
#include <queue>
#include <pthread.h>

// forward declarations
class Event;
class EnemyHandler;
class AgentHandler;
class EventRouter;
class PathFinder;

class EventHandler
{
public:
	EventHandler(int client, AgentHandler* pAgentHandler, EnemyHandler* pEnemyHandler, PathFinder* pPathFinder);
	~EventHandler();
	void pushEvent(const Event* pEvent);
	void processEvents();

private:
	int mClient;
	EnemyHandler*	mpEnemyHandler;
	AgentHandler*	mpAgentHandler;
	EventRouter*	mpEventRouter;
	KeyHandler*		mpKeyHandler;
	PathFinder*		mpPathFinder;

	std::queue<Event*> mEvents;
	pthread_mutex_t mQueueMutex;
};

#endif