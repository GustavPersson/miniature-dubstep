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
* The client
*/

#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "Macros.h"
#include "KeyHandler.h"
#include <pthread.h>
#include <list>
#include <ostream>

class EnemyHandler;
class AgentHandler;
class PathFinder;
class EventHandler;

namespace utilities {
	class Timer;
}

class Client
{
public:
	Client(int clientId, bool outputToFile = true, bool outputToConsole = false);
	~Client();

	void detachAndRun();

private:
	static void* mainLoop(void* vpClient);
	void run();

	int mClientId;
	bool mQuit;

	KeyHandler*	mpKeyHandler;
	EnemyHandler* mpEnemyHandler;
	AgentHandler* mpAgentHandler;
	PathFinder* mpPathFinder;
	EventHandler* mpEventHandler;
	utilities::Timer* mpTimer;
	std::list<std::wostream*> mOutStreams;
	std::wofstream* mpFileOutStream;

	pthread_t mThread;
};

#endif