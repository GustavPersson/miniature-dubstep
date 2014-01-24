/**
 * @file
 * @author Gustav Persson <gustav@alltomminrov.se>
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
 * A global event manager. Includes all code to handle events and their
 * listeners.
 */

#include "EventManager.h"

using namespace logic;

//The constructor and destructor are pretty bare bones.

EventManager::EventManager(const char *const pName, bool setAsGlobal) : IEventManager(pName, setAsGlobal), mActiveQueue(0)
{
}
 
EventManager::~EventManager()
{
	mActiveQueue = 0;
}

//Following is the code for adding new listeners. Listeners and event types are registered with a single call.
//If either is new to the system, it is validated and added to the data stryctures.

bool EventManager::addListener(const EventListenerPtr &inListener, const EventType &inType)
{
	if (!validateType(inType))
		return false;
	
	//check/update the type list
	EventTypeSet::iterator evIt = mTypeList.find(inType);

	//find listener map entry, create one if no table already
	//exists for this entry

	EventListenerMap::iterator elmIt = mRegistry.find(inType.getHashValue());

	if (elmIt == mRegistry.end())
	{
		EventListenerMapIRes elmIRes = mRegistry.insert(
			EventListenerMapEnt(inType.getHashValue(), EventListenerTable()));

		//could not insert to map?!
		if (elmIRes.second == false)
			return false;

		//did we insert and create an empty table?
		if (elmIRes.first == mRegistry.end())
			return false;

		//store it so we can update the mapped list next
		elmIt = elmIRes.first;
	}

	//Update the mapped list of listeners. walk the existing list
	//(if any entries) to prevent duplicate addition of
	//listeners. This is a bit more costly at registration time
	//but will prevent hard-to-notice duplicate event
	//propogation sequences that would happen if double
	//entries were allowed.

	EventListenerTable &evlTable = (*elmIt).second;

	for (EventListenerTable::iterator it = evlTable.begin(),
		itEnd = evlTable.end(); it != itEnd; it++)
	{
		bool bListenerMatch = ( *it == inListener);

		if (bListenerMatch)
			return false;
	}

	//okay the event type is validated,
	//event listener is not already in a map
	//so add it.

	evlTable.push_back(inListener);
	return true;
}

//here's how you remove a listener

bool EventManager::delListener(const EventListenerPtr &inListener, const EventType &inType)
{
	if (!validateType(inType))
		return false;

	bool rc = false;

	//this is a brute force method. iterate through all
	//existing mapping entries looking for the match and remove it.

	for (EventListenerMap::iterator it = mRegistry.begin(), itEnd = mRegistry.end();
		it != itEnd; it++)
	{
//		unsigned int const eventId = it->first;
		EventListenerTable &table = it->second;

		for (EventListenerTable::iterator innerIt = table.begin(), innerItEnd = table.end();
			innerIt != innerItEnd; innerIt++)
		{
			if (*innerIt == inListener)
			{
				//found a match, remove from table.
				table.erase(innerIt);

				//update the return code
				rc = true;

				//Early quit the code, since there's only one possible
				//listener of this type in the table.
				break;
			}
		}
	}

	return rc;
}

//Fire off an event asynchronous.
bool EventManager::trigger(EventData const &inEvent) const
{
	bool processed = false;
	if (!validateType(inEvent.getEventType()))
		return false;
	
	//This part of the code handles listeners that care about all
	//event types.
	EventListenerMap::const_iterator itWC = mRegistry.find(EventType(gWildcardEventType).getHashValue());

	if (itWC != mRegistry.end())
	{
		EventListenerTable const &table = itWC->second;

		processed = false;

		for (EventListenerTable::const_iterator handleIt = table.begin(),
			handleEnd = table.end(); handleIt != handleEnd; handleIt++)
		{
			(*handleIt)->handleEvent(inEvent);
		}
	}

	//This part of the code handles listeners that care about
	//specific event types.
	EventListenerMap::const_iterator it = mRegistry.find(inEvent.getEventType().getHashValue());

	if (it == mRegistry.end())
		return false;

	EventListenerTable const &table = it->second;

	processed = false;

	for (EventListenerTable::const_iterator handleEatIt = table.begin(),
		handleEatEnd = table.end(); handleEatIt != handleEatEnd; handleEatIt++)
	{
		EventListenerPtr listener = *handleEatIt;
		if (listener->handleEvent(inEvent))
		{
			//only set to true if the listener eats the event
			processed = true;
		}
	}

	return processed;
}

bool EventManager::queueEvent(EventDataPtr const &inEvent)
{
	assert(mActiveQueue >= 0);
	assert(mActiveQueue < mNumOfQueues);

	if (!validateType(inEvent->getEventType()))
		return false;

	EventListenerMap::const_iterator it =
		mRegistry.find(inEvent->getEventType().getHashValue());

	if (it == mRegistry.end())
	{
		//if a global listener isn't active, then abort the queue add.
		EventListenerMap::const_iterator itWC = mRegistry.find(EventType(gWildcardEventType).getHashValue());

		if (itWC == mRegistry.end())
		{
			//no listeners for this event, skip it.
			return false;
		}
		
	}

	mQueues[mActiveQueue].push_back(inEvent);

	return true;
}

bool EventManager::threadSafeQueueEvent(EventDataPtr const &inEvent)
{
	mRealTimeEventQueue.push(inEvent);
	return true;
}

//Just in case you change your mind and want to cancel an already sent event.
bool EventManager::abortEvent(const EventType &inType, bool allOfType)
{
	assert(mActiveQueue >= 0);
	assert(mActiveQueue < mNumOfQueues);

	if (!validateType(inType))
		return false;

	EventListenerMap::iterator it = mRegistry.find(inType.getHashValue());

	if (it == mRegistry.end())
		return false; //no listeners for this event, so SKIPIT

	//return control
	bool rc = false;

	EventQueue &evtQueue = mQueues[mActiveQueue];

	for (EventQueue::iterator it = evtQueue.begin(), 
		itEnd = evtQueue.end(); it != itEnd; it++)
	{
		if ((*it)->getEventType() == inType)
		{
			it = evtQueue.erase(it);
			rc = true;
			if (!allOfType)
				break; //only delete the first one found
		}
		else
		{
			++it;
		}
	}
	return rc;
}

//all the queued events need to be processed. Somewhere in the main loop
//the event manager's tick() method should be called.
bool EventManager::tick(unsigned long maxMillis)
{
	unsigned long curMs = GetTickCount();
	unsigned long maxMs = 
		(maxMillis == IEventManager::KINFINITE) ?
		IEventManager::KINFINITE : (curMs + maxMillis);

	EventListenerMap::const_iterator itWC = mRegistry.find(EventType(gWildcardEventType).getHashValue());

	EventDataPtr rte;
	while(mRealTimeEventQueue.tryPop(rte))
	{
		queueEvent(rte);
		curMs = GetTickCount();
		if (maxMillis != IEventManager::KINFINITE)
		{
			if (curMs >= maxMs)
			{
				assert(0 && "A realtime process is spamming the event manager!");
			}
		}
	}

	//swap the active queues, make sure the new queue is empty after the swap.

	int queueToProcess = mActiveQueue;
	mActiveQueue = (mActiveQueue + 1) % mNumOfQueues;
	mQueues[mActiveQueue].clear();

	//now process as many events as we can if there's a limit
	//always do at least one event, if ANY are available.

	while (mQueues[queueToProcess].size() > 0)
	{
		EventDataPtr event = mQueues[queueToProcess].front();
		mQueues[queueToProcess].pop_front();

		EventType const & eventType = event->getEventType();

		EventListenerMap::const_iterator itListeners =
			mRegistry.find(eventType.getHashValue());

		if (itWC != mRegistry.end())
		{
			EventListenerTable const &table = itWC->second;

			for (EventListenerTable::const_iterator
				itHandle = table.begin(), itHandleEnd = table.end();
				itHandle != itHandleEnd; itHandle++)
			{
				(*itHandle)->handleEvent(*event);
			}
		}
		//Currently no listeners for this event type, skipppiiittttt
		if (itListeners == mRegistry.end())
			continue;

		//unsigned int const eventId = itListeners->first;
		EventListenerTable const &table = itListeners->second;

		for (EventListenerTable::const_iterator
			it = table.begin(), end = table.end();
			it != end; it++)
		{
			if ((*it)->handleEvent(*event))
			{
				break;
			}
		}
		curMs = GetTickCount();

		if (maxMillis != IEventManager::KINFINITE)
		{
			if (curMs >= maxMs)
			{
				//time ran out, abort processing
				break;
			}
		}
	}
	//If there's any events left to process, push them onto the active
	//queue.
	//
	//To prevent sequencing, go bottom-up on the remainder
	//inserting them at the head of the active queue.

	bool queueFlushed = (mQueues[queueToProcess].size() == 0);

	if (!queueFlushed)
	{
		while (mQueues[queueToProcess].size() > 0)
		{
			EventDataPtr event = mQueues[queueToProcess].back();
			mQueues[queueToProcess].pop_back();
			mQueues[mActiveQueue].push_front(event);
		}
	}

	return queueFlushed;
}

//The following method gives you validation if an event is legal.
bool EventManager::validateType(EventType const &inType) const
{
	//An event needs a name
	if (0 == inType.getStr().length())
		return false;

	//The event either needs to have a valid hash value, or be a wildcard.
	if ((inType.getHashValue() == 0) &&
		 (strcmp(inType.getStr().c_str(), gWildcardEventType) != 0))
		return false;

	//if the hash for two event types with different names share the same hash value,
	//you're basically the most unlucky person ever, and go ahead and change the event name
	//because it won't get validated.

	EventTypeSet::const_iterator evIt = mTypeList.find(inType);

	//if (evIt == mTypeList.end())
	//{
	//	assert(0 && "Failed validation of an event type; it was probably not registered with the eventmanager!");
	//	return false;
	//}

	return true;
}

//the last two functions gives you what listeners are attached to the manager,
//and what event types have been registered to all the listeners.

EventListenerList EventManager::getListenerList(const EventType &eventType) const
{
	//invalid event type. that's depressing.
	if (!validateType(eventType))
		return EventListenerList();

	EventListenerMap::const_iterator itListeners = mRegistry.find(eventType.getHashValue());

	//No listeners currently for this event type.
	if (itListeners == mRegistry.end())
		return EventListenerList();

	EventListenerTable const &table = itListeners->second;

	//there was, but is not now, any listeners for this event type
	if (table.size() == 0)
		return EventListenerList();

	EventListenerList result;
	result.reserve(table.size());

	for (EventListenerTable::const_iterator it = table.begin(),
		end = table.end(); it != end; it++)
	{
		result.push_back(*it);
	}

	return result;
}

EventTypeList EventManager::getTypeList(void) const
{
	//no entries
	if (mTypeList.size() == 0)
	{
		return EventTypeList();
	}

	EventTypeList result;
	result.reserve(mTypeList.size());

	for (EventTypeSet::const_iterator typeIterator = mTypeList.begin(),
		itEnd = mTypeList.end(); typeIterator != itEnd; typeIterator++)
	{
		result.push_back(*typeIterator);
	}

	return result;
}
