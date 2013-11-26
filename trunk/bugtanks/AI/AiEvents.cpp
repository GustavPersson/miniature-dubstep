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
* All the AI Events.
*/

#include "AiEvents.h"
#include "PotentialUnit.h"
#include "AiConstants.h"

using namespace ai;

//--------------------------------
// TO AgentAiGoals
//--------------------------------
EvtData_To_AgentAiGoals::EvtData_To_AgentAiGoals(const logic::KeyType& key, int goalType, const utilities::MapCoordinate& conflictPosition, const PotentialSquare *goals) : mConflictPosition(conflictPosition)
{
	mGoals = myNew PotentialSquare [AiConstants::GOAL_ARRAY_SIZE];

	mGoalType = goalType;
	mKey = key;

	for (int i = 0; i < AiConstants::GOAL_ARRAY_SIZE; i++)
	{
		mGoals[i] = goals[i];
	}
}

EvtData_To_AgentAiGoals::EvtData_To_AgentAiGoals(const EvtData_To_AgentAiGoals& agentAiGoals)
{
	mGoals = myNew PotentialSquare [AiConstants::GOAL_ARRAY_SIZE];

	mConflictPosition = agentAiGoals.mConflictPosition;
	mGoalType = agentAiGoals.mGoalType;
	mKey = agentAiGoals.mKey;

	for (int i = 0; i < AiConstants::GOAL_ARRAY_SIZE; i++)
	{
		mGoals[i] = agentAiGoals.mGoals[i];
	}
}

EvtData_To_AgentAiGoals::EvtData_To_AgentAiGoals(std::istrstream& in, int networkId)
{
	mGoals = myNew PotentialSquare [AiConstants::GOAL_ARRAY_SIZE];

	mNetworkId = networkId;

	in >> mKey >> mGoalType >> mConflictPosition;

	for (int i = 0; i < AiConstants::GOAL_ARRAY_SIZE; i++)
	{
		in >> mGoals[i];
	}
}

EvtData_To_AgentAiGoals::~EvtData_To_AgentAiGoals()
{
	SAFE_DELETE_ARRAY(mGoals);
}

void EvtData_To_AgentAiGoals::serialize(std::ostrstream &out) const
{
	out << mNetworkId << " " << mKey << " " << mGoalType << " " << mConflictPosition << " ";

	for (int i = 0; i < AiConstants::GOAL_ARRAY_SIZE; i++)
	{
		out << mGoals[i];
	}
}


//--------------------------------
// TO AiAddToConlfictMap
//--------------------------------
EvtData_To_AiAddToConflictMap::EvtData_To_AiAddToConflictMap(std::istrstream &in, int networkId)
{
	mNetworkId = networkId;

	in >> mKey >> mGoalType >> mGoalPosition >> mUnitSize;
}

void EvtData_To_AiAddToConflictMap::serialize(std::ostrstream &out) const
{
	out << mNetworkId << " " << mKey << " " << mGoalType << " " << mGoalPosition << " " << mUnitSize;
}

//--------------------------------
// TO AiRemoveFromConflictMap
//--------------------------------
EvtData_To_AiRemoveFromConflictMap::EvtData_To_AiRemoveFromConflictMap(std::istrstream &in, int networkId)
{
	mNetworkId = networkId;

	in >> mKey >> mGoalType >> mGoalPosition;
}

void EvtData_To_AiRemoveFromConflictMap::serialize(std::ostrstream &out) const
{
	out << mNetworkId << " " << mKey << " " << mGoalType << " " << mGoalPosition;
}