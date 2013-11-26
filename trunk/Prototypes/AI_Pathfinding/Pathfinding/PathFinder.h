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
* Finds the path
*/

#ifndef __PATH_FINDER_H__
#define __PATH_FINDER_H__

#include "PathFinder.h"
#include "KeyHandler.h"
#include "Vector.h"
#include "VectorList.h"
#include <ostream>
#include <list>
#include <map>
#include <string>

// forward declarations
class Agent;
class AgentHandler;
class EnemyHandler;
class Enemy;
struct PotentialSquare;

const int MAP_HEIGHT = 20;
const int MAP_WIDTH = 30;

#define PRINT_OUTPUT(message) \
	if (mpOutStreams != NULL) \
	{ \
		std::list<std::ostream*>::iterator it; \
		for (it = mpOutStreams->begin(); it != mpOutStreams->end(); ++it) \
		{ \
			(**it) << message << "\n\n"; \
		} \
	}

struct FixedCoordinateWrapper {
	FixedCoordinate mFixedCoordinate;
	KeyType	miAgent;
};

class PathFinder
{
public:
	PathFinder(AgentHandler* pAgentHandler, EnemyHandler* pEnemyHandler, std::list<std::wostream*>* pOutStreams);
	~PathFinder();

	void calculate();
	void addEnemyToEnemyPotentialMap(Enemy* pEnemy);
	
private:
	enum GoalTypes
	{
		GoalType_Goal,
		GoalType_NextGoal
	};

	void solveConflicts(GoalTypes goalType, std::map<KeyType, Agent*>& agents);
	void solveGoalConflict(Agent* pFirstAgent, Agent* pCollidingAgent);
	void solveNextGoalConflict(Agent* pFirstAgent, Agent* pCollidingAgent);
	void sortGoals(PotentialSquare* goals);
	void calculateSurrondingField(GoalTypes goalType, Agent* pAgent, PotentialSquare* field);
	void addCollisions(const std::map<KeyType, Agent*>& agents, GoalTypes goalType);
	bool checkCollision(GoalTypes goalType, Agent* pAgent);
	void printMap();
	void printLine(std::wostream& out, bool firstLine = false, bool lastLine = false);
	std::wstring getArrow(const FixedCoordinate& from, const FixedCoordinate& to);

	float mEnemyPotentialMap[MAP_WIDTH][MAP_HEIGHT];

	std::map<FixedCoordinate, KeyType, FixedCoordinateCompare> mAgentGoalPositions;
	std::map<FixedCoordinate, KeyType, FixedCoordinateCompare> mAgentNextGoalPositions;

	// Temporary information to check for collisions	
	utilities::VectorList<Agent*> mCollidingAgents;

	AgentHandler* mpAgentHandler;
	EnemyHandler* mpEnemyHandler;

	std::list<std::wostream*>* mpOutStreams;
	std::wstring mLineSeparator;

	int mFrame;
};

#endif