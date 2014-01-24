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
 */

#ifndef __EVENTMANAGER_H__
#define __EVENTMANAGER_H__
#include "../Utilities/Macros.h"
#include "IEventManager.h"
#include "Windows.h"

#include <list>
#include <vector>
#include <set>
#include <map>

namespace logic
{

typedef std::vector<EventListenerPtr>	EventListenerList;
typedef std::vector<EventType>			EventTypeList;

/**
 * A global event manager. Includes all code to handle events and their
 * listeners.
 */

class EventManager : public IEventManager
{
public:
	explicit EventManager(char const* const pName, bool setAsGlobal);
	
	virtual ~EventManager();

	
	/**
	 * Registers a handler for a specific event type. More exactly, the event type will be added to the known event types
	 * if not already known. The listener is first validated and then entered into the data structures.
	 *
	 * @param inListener The listener to be registered
	 * @param inType The event to be registered
	 *
	 * @return Returns false on failure for any reason. The only anticipated failure reason is
	 * if the input event type is bad.
	 */
	virtual bool addListener(EventListenerPtr const &inListener, EventType const &inType);

	/**
	 * Removes a listener/event-type pairing from the internal table.
	 * @param inListener	the listener to be removed.
	 * @param inType		the event to be removed.
	 * @return Returns false if the pairing was not found.
	 */
	virtual bool delListener(EventListenerPtr const &inListener, EventType const &inType);

	/**
	 * Fire off event, synchronous.
	 * @param inEvent the event to trigger.
	 * @return Returns true if the event is consumed, false if not. NOTE that it is acceptable for a
	 * listener to act on an event WITHOUT consuming it. The return is only there to propagate 
	 * that information back to outside users from the internals of this system, if the need is there.
	 */
	virtual bool trigger(EventData const &inEvent) const;

	/**
	* Fire off an event, asynchronous. Do it when the event system tick() method is called, normally at a
	* special time during the game-loop.
	* @param inEvent the event to be triggered!.
	*/
	virtual bool queueEvent(EventDataPtr const &inEvent);
	
	/**
	* A thread safe version of queueEvent().
	* @param inEvent The event to be queued.
	*/
	virtual bool threadSafeQueueEvent(EventDataPtr const &inEvent);

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
	virtual bool abortEvent(EventType const &inType, bool allOfType);
	
	/**
	 * Allows for queued events to be processed. Optionally specify a max processing
	 * time, so that the processing doesn't take too long. NOTE that using a max processing time
	 * might mean that all messages DOES NOT actually get processed.
	 * @param maxMillis the maximum tick time in milliseconds. Use with caution.
	 * @return true if all messages ready to be processed were, false otherwise (e.g. timeout)
	 */	
	virtual bool tick(unsigned long maxMillis);
	
	/**
	* Validates an event type. Note that this does NOT add the event to the internal registry,
	* it just verifies it to be legal.
	* @param inType the event type to be validated.
	* @return returns true if the event is assigned to a matching signature text, and the text is not empty.
	* @return returns false if the event is not yet assigned to anything.
	*/
	virtual bool validateType(EventType const &inType) const;

	/**
	* Returns all event listeners registered to this manager as a list.
	* @return Returns a list of listeners as an EventListenerList
	*/
	EventListenerList getListenerList(EventType const &eventType) const;

	/**
	* Returns all Event Types registered to this manager as a list
	* @return Returns all event types regged with this manager as an EventType List
	*/
	EventTypeList getTypeList(void) const;


private:

	//one global instance
	typedef std::set<EventType> EventTypeSet;

	//insert result into event type set
	typedef std::pair<EventTypeSet::iterator, bool>	EventTypeSetIRes;

	//one list per event type
	typedef std::list<EventListenerPtr>				EventListenerTable;

	//mapping of event ident to listener list
	typedef std::map<unsigned int, EventListenerTable> EventListenerMap;

	//entry in the event ölistener map
	typedef std::pair<unsigned int, EventListenerTable> EventListenerMapEnt;

	//insert result into listener map
	typedef std::pair<EventListenerMap::iterator, bool> EventListenerMapIRes;

	//queue of pending or processing events
	typedef std::list<EventDataPtr>						EventQueue;

	enum eConstants
	{
		mNumOfQueues = 2
	};

	EventTypeSet			mTypeList; //list of registered event types
	EventListenerMap		mRegistry; //mapping of event types to listeners
	EventQueue				mQueues[mNumOfQueues]; //Event processing queue, double buffered to prevent infinite bicycling
	ThreadSafeEventQueue	mRealTimeEventQueue;
	int						mActiveQueue; //valid denoting which queue is active. enqueueing events goes to opposing queue.
	const char*				mName;				


};
}
#endif