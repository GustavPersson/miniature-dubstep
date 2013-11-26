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
* The event information
*/

#ifndef __EVENT_H__
#define __EVENT_H__

#include "KeyHandler.h"
#include "Agent.h"

enum Events
{
	Event_First = 0,
	Event_EnemyCreate = 0,
	Event_EnemyDestroy,
	Event_AgentCreate,
	Event_AgentDestroy,
	Event_AgentGoalPosition,
	Event_AgentNextGoalPosition,
	Event_AgentDirection,
	Event_Lim
};

class Data
{
public:
	Data() {}
	virtual ~Data() {}
	virtual Data* copy() = 0;
};

class Event
{
public:
	Event(Events eventType, KeyType key, int playerId, Data* pData);
	Event(const Event& e);
	virtual ~Event();
	Data* getEventData();
	Events getEventType();
	KeyType getKey();
	int getPlayerId();
private:
	int mPlayerId;
	Events mEvent;
	KeyType mKey;
	Data* mpData;
};

//----------------------------
// FixedCoordinate
//----------------------------
class FixedCoordinateData : public Data
{
public:
	FixedCoordinateData(FixedCoordinate coord);
	FixedCoordinateData(int x, int y);
	virtual ~FixedCoordinateData() {}
	FixedCoordinate getData();
	virtual Data* copy();

private:
	FixedCoordinate mCoord;
};

#define GET_FIXED_COORD(pEvent) dynamic_cast<FixedCoordinateData*>(pEvent->getEventData())->getData()

//----------------------------
// Vec2
//----------------------------
class Vec2Data : public Data
{
public:
	Vec2Data(Vec2 vec2);
	virtual ~Vec2Data() {}
	Vec2 getData();
	virtual Data* copy();

private:
	Vec2 mVec2;
};

#define GET_VEC2(pEvent) dynamic_cast<Vec2Data*>(pEvent->getEventData())->getData()

#endif