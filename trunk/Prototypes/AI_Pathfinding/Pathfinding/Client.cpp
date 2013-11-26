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

#include "Macros.h"
#include "Client.h"
#include "EnemyHandler.h"
#include "AgentHandler.h"
#include "EventHandler.h"
#include "PathFinder.h"
#include "../../../bugtanks/Utilities/Timer.h"
#include <list>
#include <fstream>
#include <iostream>
#include <string>

using utilities::Timer;

Client::Client(int clientId, bool outputToFile, bool outputToConsole)
{
	if (outputToFile)
	{
		// Open the output file
		std::string filePath = "debug_AI_";
		filePath += clientId + 48;
		filePath += ".txt";
		DEBUG_MESSAGE(filePath);

		mpFileOutStream = myNew std::wofstream(filePath.c_str());
		if (!mpFileOutStream->is_open())
		{
			DEBUG_MESSAGE("Could not open file: " << filePath << ", exiting...");
			exit(EXIT_FAILURE);
		}
	
		mOutStreams.push_back(mpFileOutStream);
	}
	else
	{
		mpFileOutStream = NULL;
	}

	if (outputToConsole)
	{
		mOutStreams.push_back(&std::wcout);
	}


	mQuit = false;
	mClientId = clientId;
	
	mpKeyHandler = KeyHandler::getInstance(clientId);
	mpAgentHandler = myNew AgentHandler();
	mpEnemyHandler = myNew EnemyHandler();
	mpPathFinder = myNew PathFinder(mpAgentHandler, mpEnemyHandler, &mOutStreams);
	mpEventHandler = myNew EventHandler(clientId, mpAgentHandler, mpEnemyHandler, mpPathFinder);
	mpTimer = myNew Timer();
	mpTimer->start();
}

Client::~Client()
{
	// Wait for the thread to die
	mQuit = true;
	pthread_join(mThread, NULL);

	SAFE_DELETE(mpPathFinder);
	SAFE_DELETE(mpAgentHandler);
	SAFE_DELETE(mpEnemyHandler);
	SAFE_DELETE(mpEventHandler);
	SAFE_DELETE(mpKeyHandler);
	SAFE_DELETE(mpTimer);
	SAFE_DELETE(mpFileOutStream);
}

void Client::detachAndRun()
{
	pthread_create(&mThread, NULL, Client::mainLoop, reinterpret_cast<void*>(this));
}

void* Client::mainLoop(void *vpClient)
{
	Client* pClient = reinterpret_cast<Client*>(vpClient);
	pClient->run();

	return NULL;
}

void Client::run()
{
	DEBUG_MESSAGE("Client::run()");
	float curFrame = mpTimer->getTime(Timer::ReturnType_Seconds);
	float lastFrame = curFrame;
	float deltaTime = 0.0f;

	while (!mQuit)
	{
		mpEventHandler->processEvents();
		mpAgentHandler->update(deltaTime);
		mpPathFinder->calculate();
	
		Sleep(10);

		lastFrame = curFrame;
		curFrame = mpTimer->getTime(Timer::ReturnType_Seconds);
		deltaTime = curFrame - lastFrame;
	}
	DEBUG_MESSAGE("Client::run() - end");
}