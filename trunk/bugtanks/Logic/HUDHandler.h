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

#ifndef HUD_HANDLER_H
#define HUD_HANDLER_H
#include "HUDGUI.h"

namespace logic
{
	class HUDHandler
	{
		public:
			/**
			 * Default destructor.
			 */
			virtual ~HUDHandler();

			/**
			 * Retrieves the HUDHandler instance.
			 *  Note: The graphics thread must be locked during the first
			 *  call to this method.
			 * @return Returns mpHUDInstance.
			 */
			static HUDHandler *getInstance();

			/**
			 * Updates the HUD frames.
			 * @param deltaTime Time since last update.
			 */
			virtual void update(float deltaTime);

			/**
			 * Updates graphic components of the HUD frames.
			 * @param deltaTime Time since last update.
			 */
			virtual void updateGFX(float deltaTime);

			/**
			 * Shows the HUD.
			 */
			virtual void show();

			/**
			 * Hides the HUD.
			 */
			virtual void hide();

		protected:

		private:
			/**
			 * Default constructor.
			 */
			HUDHandler();

			/**
			 * Default copy constructor.
			 */
			HUDHandler(const HUDHandler &handler);

			/**
			 * Default assignment operator.
			 */
			const HUDHandler &operator=(const HUDHandler &handler);

			static HUDHandler *mpHUDInstance;
			HUDGUI *mpHUDGUI;
	};
}

#endif