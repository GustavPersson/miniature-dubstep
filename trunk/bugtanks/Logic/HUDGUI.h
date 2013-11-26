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

#ifndef HUD_GUI_H
#define HUD_GUI_H
#include "../GFX/GUIHandler.h"
#include "HUDMainFrame.h"
#include "HUDScoreFrame.h"

namespace logic
{
	class HUDGUI : public gfx::GUIHandler
	{
		public:
			/**
			 * Default constructor.
			 * @param pBaseInstance Pointer to a sprite instance base which will render all GUI frames.
			 * @param clientWidth Width of the client area.
			 * @param clientHeight Height of the client area.
			 */
			HUDGUI(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight);

			/**
			 * Default copy constructor.
			 */
			HUDGUI(const HUDGUI &handler);

			/**
			 * Default destructor.
			 */
			virtual ~HUDGUI();

			/**
			 * Default assignment operator.
			 */
			const HUDGUI &operator=(const HUDGUI &handler);

			/**
			 * Returns the name of the listener, as a string.
			 * @return Returns the name as a stdstring
			 */
			virtual const char *getName();

			/**
			 * Shows all HUD frames.
			 */
			virtual void show();

			/**
			 * Hides all HUD frames.
			 */
			virtual void hide();

			virtual void update(float deltaTime);

		protected:
			/**
			 * Create all frames for the GUI.
			 */
			virtual void init();

		private:
			HUDMainFrame *mpMainFrame;
			HUDScoreFrame *mpScoreFrame;
	};
}

#endif