/**
 * @file
 * @author Martin Jönsson <snowman.mrgames@gmail.com>
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
#ifndef __GFXLISTENER_H__
#define __GFXLISTENER_H__

#include "IEventManager.h"
#include "Events.h"
#include "../Utilities/VectorList.h"

namespace logic
{

/**
 * Event listener for the GFX component.
 */

	class GFXListener : public EventListener
	{
	public:
		/**
		* Constructor()
		*/
		explicit GFXListener();
		/**
		* Destructor()
		*/
		virtual ~GFXListener();
		/**
		 * Returns the name of the listener
		 */
		char const * getName(void) {return "GFXListener";}
		/*
		* Handles the event.
		* @param eventData The event to handle.
		*/
		bool handleEvent(EventData const &eventData);
		utilities::VectorList<EventDataPtr>& getEventList();
	private:
		utilities::VectorList<EventDataPtr> mEvents;
	};
}

#endif