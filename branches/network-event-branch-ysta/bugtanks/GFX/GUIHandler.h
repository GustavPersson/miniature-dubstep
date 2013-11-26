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

#ifndef GUI_HANDLER_H
#define GUI_HANDLER_H
#include "GUIFrame.h"

namespace gfx
{
	/**
	 * Manager class for the GUI.
	 * Each GUI component (menu, game HUD etc.) should be a GUIHandler.
	 */
	class GUIHandler : public EventListener
	{
		public:
			/**
			 * Default constructor.
			 */
			GUIHandler();

			/**
			 * Default copy constructor.
			 */
			GUIHandler(const GUIHandler &handler);

			/**
			 * Default destructor.
			 */
			virtual ~GUIHandler();

			/**
			 * Default assignment operator.
			 */
			const GUIHandler &operator=(const GUIHandler &handler);

			/**
			 * Returns the name of the listener, as a string.
			 * @return Returns the name as a stdstring
			 */
			virtual const char *getName();

			/**
			 * Handles events.
			 * @param mEvent The event that should be handled.
			 * @return Returns true if the event was consumed, and should not be propogated
			 * @return Returns false if the event was not consumed.
			 */
			virtual bool handleEvent(const EventData &mEvent);

		protected:

		private:
	};
}

#endif