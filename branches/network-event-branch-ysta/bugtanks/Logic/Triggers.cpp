/**
 * @file
 * @author Victor Lundberg <lundberg.victor@gmail.com>
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

#include "Triggers.h"

using namespace logic;

/****************************************************************
 * TriggerEndMap
 ****************************************************************/

TriggerEndMap::TriggerEndMap() : TriggerBase()
{
	setTriggerType(TriggerTypes::END_MAP);
}

TriggerEndMap::TriggerEndMap(const TriggerEndMap &trigger) : TriggerBase(trigger)
{

}

TriggerEndMap::~TriggerEndMap()
{

}

const TriggerEndMap &TriggerEndMap::operator=(const TriggerEndMap &trigger)
{
	if(this != &trigger)
	{
		TriggerBase::operator=(trigger);
	}
	return *this;
}

TriggerBase *TriggerEndMap::clone()
{
	return myNew TriggerEndMap(*this);
}

bool TriggerEndMap::handleEvent(EventData const &eventData)
{
	TriggerBase::handleEvent(eventData);
	return false;
}

void TriggerEndMap::activate()
{
	TriggerBase::activate();
}

/****************************************************************
 * TriggerCreateUnit
 ****************************************************************/

TriggerCreateUnit::TriggerCreateUnit() : TriggerBase()
{
	setTriggerType(TriggerTypes::CREATE_UNIT);
}

TriggerCreateUnit::TriggerCreateUnit(const TriggerCreateUnit &trigger) : TriggerBase(trigger)
{

}

TriggerCreateUnit::~TriggerCreateUnit()
{

}

const TriggerCreateUnit &TriggerCreateUnit::operator=(const TriggerCreateUnit &trigger)
{
	if(this != &trigger)
	{
		TriggerBase::operator=(trigger);
	}
	return *this;
}

TriggerBase *TriggerCreateUnit::clone()
{
	return myNew TriggerCreateUnit(*this);
}

bool TriggerCreateUnit::handleEvent(EventData const &eventData)
{
	return true;
}

void TriggerCreateUnit::activate()
{
	TriggerBase::activate();
}

/****************************************************************
 * TriggerDestroyUnit
 ****************************************************************/

TriggerDestroyUnit::TriggerDestroyUnit() : TriggerBase()
{
	setTriggerType(TriggerTypes::DESTROY_UNIT);
}

TriggerDestroyUnit::TriggerDestroyUnit(const TriggerDestroyUnit &trigger) : TriggerBase(trigger)
{

}

TriggerDestroyUnit::~TriggerDestroyUnit()
{

}

const TriggerDestroyUnit &TriggerDestroyUnit::operator=(const TriggerDestroyUnit &trigger)
{
	if(this != &trigger)
	{
		TriggerBase::operator=(trigger);
	}
	return *this;
}

TriggerBase *TriggerDestroyUnit::clone()
{
	return myNew TriggerDestroyUnit(*this);
}

bool TriggerDestroyUnit::handleEvent(EventData const &eventData)
{
	return true;
}

void TriggerDestroyUnit::activate()
{
	TriggerBase::activate();
}

/****************************************************************
 * TriggerCreateGroup
 ****************************************************************/

TriggerCreateGroup::TriggerCreateGroup() : TriggerBase()
{
	setTriggerType(TriggerTypes::CREATE_GROUP);
}

TriggerCreateGroup::TriggerCreateGroup(const TriggerCreateGroup &trigger) : TriggerBase(trigger)
{

}

TriggerCreateGroup::~TriggerCreateGroup()
{

}

const TriggerCreateGroup &TriggerCreateGroup::operator=(const TriggerCreateGroup &trigger)
{
	if(this != &trigger)
	{
		TriggerBase::operator=(trigger);
	}
	return *this;
}

TriggerBase *TriggerCreateGroup::clone()
{
	return myNew TriggerCreateGroup(*this);
}

bool TriggerCreateGroup::handleEvent(EventData const &eventData)
{
	return true;
}

void TriggerCreateGroup::activate()
{
	TriggerBase::activate();
}