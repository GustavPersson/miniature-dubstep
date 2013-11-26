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
*/

#include "../Utilities/Macros.h"
#include "../Utilities/Vector2D.h"
#include "../Utilities/VectorList.h"
#include "AiApplication.h"
#include <map>


void testAiBlock();
void testVector2D();
void testVectorList();
void testVerbosity();
void testAiApplication();
void testErrorMessages();
void testIterator();

int main()
{
	initMemoryCheck();
	setOutputTargetDebugMessage(OUTPUT_FILE | OUTPUT_CONSOLE);
	setVerbosityLevel(LEVEL_LOWEST, OUTPUT_FILE);
	setVerbosityLevel(LEVEL_HIGH, OUTPUT_CONSOLE);
	//testVerbosity();
	//testVector2D();
	//testVectorList();
	//testAiBlock();
	testAiApplication();
	//testErrorMessages();
	//testIterator();

	return 0;
}

void testAiApplication()
{
	AiApplication::createInstance();

	logic::Application* pApp = logic::Application::getInstance();

	if (pApp->initInstance())
	{
		pApp->runGame();
		pApp->onClose();
	}

	SAFE_DELETE(pApp);
}

void testIterator()
{
	setVerbosityLevel(LEVEL_LOWEST, OUTPUT_CONSOLE);
	DEBUG_MESSAGE_STOP(LEVEL_MEDIUM, "testIterator()");
	using namespace std;
	map<int, int> testMap;

	for (int i = 0; i < 5; i++)
	{
		testMap[i*2] = i;
	}

	// Test to iterate through two then add one element and continue the iteration
	map<int, int>::iterator it = testMap.begin();

	++it;

	testMap[1] = 1336;
	testMap[3] = 1337;
	
	++it;

	testMap.erase(2);
	testMap[7] = 1338;
	testMap.erase(8);

	++it;
	testMap.erase(3);

	DEBUG_MESSAGE(LEVEL_LOWER ,it->second);

	for (it = testMap.begin(); it != testMap.end(); ++it)
	{
		DEBUG_MESSAGE(LEVEL_LOWER, "Index: " << it->first << ", Value: " << it->second);
	}

	DEBUG_MESSAGE_STOP(LEVEL_MEDIUM, "ENDOF - testIterator()");
}

void testErrorMessages()
{
	DEBUG_MESSAGE_STOP(LEVEL_MEDIUM, "testErrorMessages()");

	ERROR_MESSAGE("Testing in both");
	setOutputTargetError(OUTPUT_FILE);
	ERROR_MESSAGE("Testing file");
	setOutputTargetError(OUTPUT_CONSOLE);
	ERROR_MESSAGE("Testing in console");

	checkForErrors();

	DEBUG_MESSAGE_STOP(LEVEL_MEDIUM, "ENDOF - testErrorMessages()");
}

void testVerbosity()
{
	DEBUG_MESSAGE_STOP(LEVEL_MEDIUM, "testVerbosity()");
	for (int i = LEVEL_NONE; i <= LEVEL_LOWEST; i++)
	{
		DEBUG_MESSAGE_STOP(LEVEL_NONE, "Setting verbosity level to" << i);
		setVerbosityLevel(i, OUTPUT_CONSOLE);
		DEBUG_MESSAGE(LEVEL_NONE, "LEVEL_NONE");
		DEBUG_MESSAGE(LEVEL_HIGHEST, "LEVEL_HIGHEST");
		DEBUG_MESSAGE(LEVEL_HIGHER, "LEVEL_HIGHER");
		DEBUG_MESSAGE(LEVEL_HIGH, "LEVEL_HIGH");
		DEBUG_MESSAGE(LEVEL_MEDIUM, "LEVEL_MEDIUM");
		DEBUG_MESSAGE(LEVEL_LOW, "LEVEL_LOW");
		DEBUG_MESSAGE(LEVEL_LOWER, "LEVEL_LOWER");
		DEBUG_MESSAGE(LEVEL_LOWEST, "LEVEL_LOWEST");
	}
	DEBUG_MESSAGE_STOP(LEVEL_MEDIUM, "ENDOF - testVerbosity()");
}

void testAiBlock()
{
	DEBUG_MESSAGE_STOP(LEVEL_MEDIUM, "testAiBlock()");

	ERROR_MESSAGE("Boau");

#ifdef USE_AI
		DEBUG_MESSAGE(LEVEL_MEDIUM, "In ai block");
		ERROR_MESSAGE("Banan");
		ERROR_MESSAGE("Bananer");
#endif
	ERROR_MESSAGE("Shit");

	DEBUG_MESSAGE_STOP(LEVEL_MEDIUM, "ENDOF - testAiBlock()");
}

void testVector2D()
{
	DEBUG_MESSAGE_STOP(LEVEL_MEDIUM, "testVector2D()");

	utilities::Vector2D<int> vec(4, 3, 0);

	int i = 0;
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			vec.get(x, y) = i;
			i++;
		}
	}

	// test to shift
	vec.shiftRight(false);

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			int banan = vec.get(x, y);
			banan = 0;
		}
	}

	DEBUG_MESSAGE_STOP(LEVEL_MEDIUM, "ENDOF - testVector2D()");
}

void testVectorList()
{
	DEBUG_MESSAGE_STOP(LEVEL_MEDIUM, "testVectorList()");

	utilities::VectorList<int> vec;

	vec.add(0);
	vec.removeFirst();
	vec.add(0);
	vec.removeFirst();
	vec.add(0);
	vec.removeFirst();
	vec.add(0);
	vec.removeFirst();

	for(int i = 0; i<10; i++)
	{
		vec.add(i);
	}

	for (int i = 0; i < 10; i++)
	{
		if (vec.getFirst() != i)
		{
			DEBUG_MESSAGE_STOP(LEVEL_LOWER, "The value isn't equal!");
		}
		vec.removeFirst();
	}

	for (int i = 0; i < 25; i++)
	{
		vec.add(i);
	}

	for (int i = 0; i < 25; i++)
	{
		if (vec[i] != i)
		{
			DEBUG_MESSAGE_STOP(LEVEL_LOWER, "The value isn't equal!");
		}
	}

	for (int i = 0; i < 25; i++)
	{
		if (vec.getFirst() != i)
		{
			DEBUG_MESSAGE_STOP(LEVEL_LOWER, "The value isn't equal!");
		}
		vec.removeFirst();
	}

	for(int i = 0; i<10; i++)
	{
		vec.add(i);
	}

	for(int i = 0; i<10; i++)
	{
		if (vec[i] != i)
		{
			DEBUG_MESSAGE_STOP(LEVEL_LOWER, "The value isn't equal!");
		}
	}

	DEBUG_MESSAGE_STOP(LEVEL_MEDIUM, "ENDOF - testVectorList()");
}