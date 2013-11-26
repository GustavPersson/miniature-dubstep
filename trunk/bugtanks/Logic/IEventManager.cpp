/**
 * @file
 * @author Gustav Persson <ninjamuff@gmail.com>
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
 * A definition of an event manager.
 */

#include "IEventManager.h"
#include <cstring>

using namespace logic;

static IEventManager* mEventMgr = NULL;

IEventManager* IEventManager::Get()
{
	return mEventMgr;
}

IEventManager::IEventManager(const char *const pName, bool setAsGlobal)
{
	mName = pName;

	if(setAsGlobal)
		mEventMgr = this;
}

IEventManager::~IEventManager()
{
	if(mEventMgr == this)
		mEventMgr = NULL;
}

bool logic::safeAddListener(EventListenerPtr const &inHandler, EventType const &inType)
{
	assert(IEventManager::Get());
	return IEventManager::Get()->addListener(inHandler, inType);	
}

bool logic::safeDelListener(EventListenerPtr const &inHandler, EventType const &inType)
{
	assert(IEventManager::Get());
	return IEventManager::Get()->delListener(inHandler, inType);
}

bool logic::safeTriggerEvent(EventData const &inEvent)
{
	assert(IEventManager::Get());
	return IEventManager::Get()->trigger(inEvent);
}

bool logic::safeQueueEvent(EventDataPtr const &inEvent)
{
	assert(IEventManager::Get());
	return IEventManager::Get()->queueEvent(inEvent);
}

bool logic::safeThreadSafeQueueEvent(EventDataPtr const &inEvent)
{
	assert(IEventManager::Get());
	return IEventManager::Get()->threadSafeQueueEvent(inEvent);
}

bool logic::safeAbortEvent(EventType const &inType, bool allOfType /*=false*/)
{
	assert(IEventManager::Get());
	return IEventManager::Get()->abortEvent(inType, allOfType);
}

bool logic::safeTickEventManager(unsigned long maxMillis /*=INFINITE*/)
{
	assert(IEventManager::Get());
	return IEventManager::Get()->tick(maxMillis);
}

bool logic::safeValidateEventType(EventType const &inType)
{
	assert(IEventManager::Get());
	return IEventManager::Get()->validateType(inType);
}