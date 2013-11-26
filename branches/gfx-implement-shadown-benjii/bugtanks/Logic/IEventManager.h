#pragma once
#pragma warning(disable: 4100)
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
 */

#ifndef __IEVENTMANAGER_H__
#define __IEVENTMANAGER_H__

#include <boost/shared_ptr.hpp>
#include <strstream>
#include "../Utilities/HashedString.h"
#include "ConcurrentQueue.h"
#include "GlobalTimer.h"
#include <assert.h>

namespace logic
{
/*
* A definition for a wildcard event type.
*/
char const * const gWildcardEventType = "*";

typedef utilities::HashedString EventType;

/**
* Holds event data like timestamps, eventtypes etc.
*/
class EventData;

/**
*	An shared_ptr that holds event data.
*/
typedef boost::shared_ptr<EventData> EventDataPtr;

/**
 * Interface classess for event managers, events and event listeners.
 */

class EventData
{

public:
	 /**
	 * Returns the event type
	 * @return EventType hashed string. 
	 */
	virtual const EventType &getEventType(void) const = 0;

	/**
	 * Returns the time stamp for when the event was fired.
	 * @return The time created as a float
	 */
	virtual float getTimeStamp() const = 0;

	/**
	 * Serializes the event for use in for example sending through networks.
	 */
	virtual void  serialize(std::ostrstream &out) const = 0;

	/**
	* Copies an event.
	*/
	virtual EventDataPtr copy() const = 0;
};



class BaseEventData : public EventData
{
public:
	/**
	 * Constructor. Creates a new event with the time of creation being the
	 * input float.
	 * @param timeStamp Time of creation of event.
	**/
	explicit BaseEventData(const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: mTimeStamp(timeStamp){}

	/**
	 * Destructor.
	 */
	virtual ~BaseEventData(){}

	/**
	 * Returns the event type
	 *
	 */
	virtual const EventType& getEventType(void) const = 0;
	
	/**
	 * Returns the time stamp for when the event was created.
	 */
	float getTimeStamp(void) const
	{
		return mTimeStamp;
	}

	/**
	 * Serializes the event for use in for example sending through networks.
	 */
	virtual void serialize(std::ostrstream &streamOut) const{}

	BaseEventData& operator = (const BaseEventData&){}

protected:
	const float mTimeStamp;
};



/*
* An empty event, containing absolutely no extra data.
* Mostly useful for triggers and the like.
*/
class EmptyEventData : public BaseEventData
{
public:
	/*
	* Constructor. Creates the event.
	* param timeStamp the time of creation for the event.
	*/
	explicit EmptyEventData(const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds)) 
		: BaseEventData(timeStamp){}

	/*
	* Destructor. Destroys the event.
	*/
	virtual ~EmptyEventData(){}
};



/*Base type for all event handler objects. Note that this is 
* a c++ interface in the classic sense that it 
* defines the sequence of calls, but does not provide 
* any functionality nor aggregate data. 
*/
class EventListener
{
public:
	/**
	* Constructor. Creates a new event listener.
	*/
	explicit EventListener(){}

	/**
	* Destructor
	*/
	virtual ~EventListener(){}

	/**
	 * Returns the name of the listener, as a string.
	 * @return Returns the name as a stdstring
	 */
	virtual char const *getName(void) = 0;


	/**
	* Handles events.
	*@param eventData the event that should be handled.
	*@return returns true if the event was consumed, and should not be propogated
	*@return returns false if the event was not consumed.
	*
	*/
	virtual bool handleEvent(EventData const &eventData) = 0
	{
		return true;
	}

};

typedef boost::shared_ptr<EventListener> EventListenerPtr;
typedef ConcurrentQueue<EventDataPtr> ThreadSafeEventQueue;

/**
* poop class for poop stuff.
**/
class IEventManager
{
public:
	enum eConstants
	{
		KINFINITE = 0xffffffff //TODO: Move this to utilities?
	};

	explicit IEventManager(char const * const pName, bool setAsGlobal);
	virtual ~IEventManager();

	/**
	 * Registers a handler for a specific event type. More exactly, the event type will be added to the known event types
	 * if not already known. The listener is first validated and then entered into the data structures.
	 *
	 * @param inHandler The handler to be registered
	 * @param inType The event to be registered
	 *
	 * @return Returns false on failure for any reason. The only anticipated failure reason is
	 * if the input event type is bad.
	 */
	virtual bool addListener(EventListenerPtr const & inHandler, EventType const & inType) = 0;
	
	/**
	 * Removes a listener/event-type pairing from the internal table.
	 * @param inHandler the listener to be removed.
	 * @param inType        the event to be removed.
	 * @return Returns false if the pairing was not found.
	 */
	virtual bool delListener(EventListenerPtr const & inHandler, EventType const & inType) = 0;
	
	/**
	 * Fire off event, synchronous.
	 * @param inEvent the event to trigger.
	 * @return Returns true if the event is consumed, false if not. NOTE that it is acceptable for a
	 * listener to act on an event WITHOUT consuming it. The return is only there to propagate 
	 * that information back to outside users from the internals of this system, if the need is there.
	 */
	virtual bool trigger(EventData const & inEvent) const = 0;
	
	/**
	* Fire off an event, asynchronous. Do it when the event system tick() method is called, normally at a
	* special time during the game-loop.
	* @param inEvent the event to be triggered!.
	*/
	virtual bool queueEvent(EventDataPtr const & inEvent)  = 0;
	
	/**
	* A thread safe version of queueEvent().
	* @param inEvent The event to be queued.
	*/
	virtual bool threadSafeQueueEvent(EventDataPtr const & inEvent) = 0;
	
	/**
	 * Finds the next availible instance of the named event, and removes it from the queue.
	 * This may be done up until the point where it is actually being processed. e.g. it is safe
	 * to happen during event processing itself.
	 *
	 * If allOfType is true, then all the events of that type will be aborted.
	 *
	 * @param inType the type of event to be removed.
	 * @param allOfType set to true if all events of this type should be aborted, otherwise only the first one is aborted.
	 * @return Returns true if the event was found and removed. Otherwise returns false.
	 */
	virtual bool abortEvent(EventType const & inType, bool allOfType = false) = 0;
	
	/**
	 * Allows for queued events to be processed. Optionally specify a max processing
	 * time, so that the processing doesn't take too long. NOTE that using a max processing time
	 * might mean that all messages DOES NOT actually get processed.
	 * @param maxMillis the maximum tick time in milliseconds. Use with caution.
	 * @return true if all messages ready to be processed were, false otherwise (e.g. timeout)
	 */	
	virtual bool tick(unsigned long maxMillis = KINFINITE) = 0;
	
	/**
	* Validates an event type. Note that this does NOT add the event to the internal registry,
	* it just verifies it to be legal.
	* @param inType the event type to be validated.
	* @return returns true if the event is assigned to a matching signature text, and the text is not empty.
	* @return returns false if the event is not yet assigned to anything.
	*/
	virtual bool validateType(EventType const & inType) const = 0;

private:
	/*
	* Returns the global event manager
	*/
	static IEventManager* Get();

	/*
	* A safe version of addListener
	*/
    friend bool safeAddListener(EventListenerPtr const & inHandler, EventType const & inType);

	/*
	* A safe version of deLListener
	*/
	friend bool safeDelListener(EventListenerPtr const & inHandler, EventType const & inType);

	/*
	* A safe version of trigger
	*/
    friend bool safeTriggerEvent(EventData const & inEvent);

	/*
	* A safe version of queueEvent
	*/
    friend bool safeQueueEvent(EventDataPtr const & inEvent);

	/*
	* A safe version of threadQueueEvent
	*/
    friend bool safeThreadSafeQueueEvent(EventDataPtr const & inEvent);

	/* 
	* A safe version of abortEvent
	*/
    friend bool safeAbortEvent(EventType const & inType, bool allOfType = false);

	/*
	* A safe version of tick
	*/
    friend bool safeTickEventManager(unsigned long maxMillis = IEventManager::KINFINITE);
	
	/*
	* A safe version of validateEventType
	*/
    friend bool safeValidateEventType(EventType const & inType);

	const char* mName;

};

// Lastly, these methods are used for easy-access methods to carry
// out basic operations without needing to pass around a pointer-to
// for sharing a global event manager.

bool safeAddListener(EventListenerPtr const & inHandler, EventType const & inType);
        
bool safeDelListener(EventListenerPtr const & inHandler, EventType const & inType);

bool safeTriggerEvent(EventData const & inEvent);

bool safeQueueEvent(EventDataPtr const & inEvent);

bool safeThreadSafeQueueEvent(EventDataPtr const &inEvent);

bool safeAbortEvent(EventType const & inType, bool allOfType /* = false */);

bool safeTickEventManager(unsigned long maxMillis /* = IEventManager::INFINITE */);

bool safeValidateEventType(EventType const & inType);

}
#endif