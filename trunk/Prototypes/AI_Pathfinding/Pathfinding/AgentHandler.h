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
#include "KeyHandler.h"
#include <map>
#include <pthread.h>

class Agent;

#ifndef __AGENT_HANDLER_H__
#define __AGENT_HANDLER_H__

class AgentHandler
{
public:
	AgentHandler();
	~AgentHandler();

	void addAgent(Agent* pAgent, KeyType id);
	void removeAgent(KeyType id);

	Agent* checkOutAgent(KeyType id);
	void checkInAgent(KeyType id);

	std::map<KeyType, Agent*>* checkOutAllAgents();
	void checkInAllAgents();

	void update(float deltaTime);

private:
	std::map<KeyType, Agent*> mAgents;
	mutable pthread_mutex_t mAgentMutex;

	// Tost the checkout/checkin function
#ifdef _DEBUG
	KeyType mCheckedOutAgent;
#endif
};

#endif