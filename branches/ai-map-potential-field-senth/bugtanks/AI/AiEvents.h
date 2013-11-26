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

#ifndef __AI__AI_EVENTS_H__
#define __AI__AI_EVENTS_H__

#include "../Utilities/Macros.h"
#include "../Utilities/Vec2Int.h"
#include "../Logic/IEventManager.h"
#include "../Logic/KeyType.h"

namespace ai
{

struct PotentialSquare;

/**
* Pauses or resumes the the AI component, all agents should stop moving when this event occurs.
*/
struct EvtData_To_AiTogglePause : public logic::EmptyEventData
{
	explicit EvtData_To_AiTogglePause() {}
	explicit EvtData_To_AiTogglePause(std::istrstream &in, int networkId)
	{
		mNetworkId = networkId;
	}
	
	virtual ~EvtData_To_AiTogglePause() {}

	virtual logic::EventDataPtr copy() const
	{
		return logic::EventDataPtr(myNew EvtData_To_AiTogglePause());
	}

	virtual const logic::EventType& getEventType() const
	{
		return msEventType;
	}

	virtual void serialize(std::ostream &out) const
	{
		out << mNetworkId << " ";
	}

	static const logic::EventType msEventType;
};

/**
* Sends all the	goals to the other clients.
*/
struct EvtData_To_AgentAiGoals : public logic::BaseEventData
{
	explicit EvtData_To_AgentAiGoals(const logic::KeyType& key, int goalType, const PotentialSquare* goals);
	explicit EvtData_To_AgentAiGoals(std::istrstream& in, int networkId);
	explicit EvtData_To_AgentAiGoals(const EvtData_To_AgentAiGoals& agentAiGoals);

	virtual ~EvtData_To_AgentAiGoals();

	virtual inline logic::EventDataPtr copy() const
	{
		return logic::EventDataPtr(myNew EvtData_To_AgentAiGoals(mKey, mGoalType, mGoals));
	}

	virtual inline const logic::EventType& getEventType() const {return msEventType;}

	virtual void serialize(std::ostrstream &out) const;

	static const logic::EventType msEventType;
	int mGoalType;
	logic::KeyType mKey;
	PotentialSquare* mGoals;
};

/**
* Event that tries to add the unit to the conflict map
*/
struct EvtData_To_AiAddToConflictMap : public logic::BaseEventData
{
	/**
	* Constructor
	* @param goalPosition the goalPosition we would like to add to the conflict map
	* @param keyId the keyId of the unit we would like to add
	* @param goalType the type of the goal we would like to add to the conflict map
	* @param unitSize the current unitSize of the unit
	*/
	explicit inline EvtData_To_AiAddToConflictMap(
		const utilities::MapCoordinate& goalPosition,
		const logic::KeyType& keyId,
		int goalType,
		const utilities::MapCoordinate& unitSize
	) : mKey(keyId), mGoalType(goalType), mGoalPosition(goalPosition), mUnitSize(unitSize) {}

	explicit EvtData_To_AiAddToConflictMap(std::istrstream& in, int networkId);
	
	virtual inline ~EvtData_To_AiAddToConflictMap() {}

	virtual inline logic::EventDataPtr copy() const
	{
		return logic::EventDataPtr(myNew EvtData_To_AiAddToConflictMap(mGoalPosition, mKey, mGoalType, mUnitSize));
	}

	virtual inline const logic::EventType& getEventType() const {return msEventType;}

	virtual void serialize(std::ostrstream &out) const;

	static const logic::EventType msEventType;
	int mGoalType;
	logic::KeyType mKey;
	utilities::MapCoordinate mGoalPosition;
	utilities::MapCoordinate mUnitSize;
};

/**
* Event that removes an agent from the conflict map
*/
struct EvtData_To_AiRemoveFromConflictMap : public logic::BaseEventData
{
	/**
	* Constructor
	* @param goalPosition the goalPosition we would like to add to the conflict map
	* @param keyId the keyId of the unit we would like to add
	* @param goalType the type of the goal we would like to add to the conflict map
	* @param unitSize the current unitSize of the unit
	*/
	explicit inline EvtData_To_AiRemoveFromConflictMap(
		const utilities::MapCoordinate& goalPosition,
		const logic::KeyType& keyId,
		int goalType,
		const utilities::MapCoordinate& unitSize
	) : mKey(keyId), mGoalType(goalType), mGoalPosition(goalPosition) {}

	explicit EvtData_To_AiRemoveFromConflictMap(std::istrstream& in, int networkId);

	virtual inline ~EvtData_To_AiRemoveFromConflictMap() {}

	virtual inline logic::EventDataPtr copy() const
	{
		return logic::EventDataPtr(myNew EvtData_To_AiRemoveFromConflictMap(mGoalPosition, mKey, mGoalType, mUnitSize));
	}

	virtual inline const logic::EventType& getEventType() const {return msEventType;}

	virtual void serialize(std::ostrstream &out) const;

	static const logic::EventType msEventType;
	int mGoalType;
	logic::KeyType mKey;
	utilities::MapCoordinate mGoalPosition;
	utilities::MapCoordinate mUnitSize;
};

}

#endif