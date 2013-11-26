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
* Handles all the agents
*/

#include "Macros.h"
#include "AgentHandler.h"
#include "Agent.h"
#include <cassert>

AgentHandler::AgentHandler()
{
	pthread_mutex_init(&mAgentMutex, NULL);

#ifdef _DEBUG
	mCheckedOutAgent = -1;
#endif
}

AgentHandler::~AgentHandler()
{
	// Delete all enemies
	pthread_mutex_lock(&mAgentMutex);
	std::map<KeyType, Agent*>::iterator it;
	for (it = mAgents.begin(); it != mAgents.end(); ++it)
	{
		SAFE_DELETE(it->second);
	}
	pthread_mutex_unlock(&mAgentMutex);

	SAFE_MUTEX_DESTROY(&mAgentMutex);
}

void AgentHandler::addAgent(Agent* pAgent, KeyType id)
{
	mAgents[id] = pAgent;
}

void AgentHandler::removeAgent(KeyType id)
{
	pthread_mutex_lock(&mAgentMutex);
	SAFE_DELETE(mAgents[id]);
	mAgents.erase(id);
	pthread_mutex_unlock(&mAgentMutex);
}

Agent* AgentHandler::checkOutAgent(KeyType id)
{
	pthread_mutex_lock(&mAgentMutex);
	std::map<KeyType, Agent*>::const_iterator it = mAgents.find(id);
	if (it != mAgents.end())
	{

		// checkout/checkin checker
#ifdef _DEBUG
		mCheckedOutAgent = id;
#endif

		return it->second;
	}
	else
	{
		return NULL;
	}
}

void AgentHandler::checkInAgent(KeyType id)
{
#ifdef _DEBUG
	assert(mCheckedOutAgent == id);
	mCheckedOutAgent = -1;
#endif
	pthread_mutex_unlock(&mAgentMutex);
}

std::map<KeyType, Agent*>* AgentHandler::checkOutAllAgents()
{
	pthread_mutex_lock(&mAgentMutex);
	return &mAgents;
}

void AgentHandler::checkInAllAgents()
{
	pthread_mutex_unlock(&mAgentMutex);
}

void AgentHandler::update(float deltaTime)
{
	// Update the agents
	std::map<KeyType, Agent*>::iterator it;
	for (it = mAgents.begin(); it != mAgents.end(); ++it)
	{
		it->second->update(deltaTime);
	}
}