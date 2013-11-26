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

#ifndef GUI_HANDLER_TEST_H
#define GUI_HANDLER_TEST_H
#include "GUIHandler.h"

namespace gfx
{
	class GUIHandlerTest : public GUIHandler
	{
		public:
			/**
			 * Default constructor.
			 * @param pBaseInstance Pointer to a sprite instance base which will render all GUIFrames.
			 */
			GUIHandlerTest(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight);

			/**
			 * Default copy constructor.
			 */
			GUIHandlerTest(const GUIHandlerTest &handler);

			/**
			 * Default destructor.
			 */
			virtual ~GUIHandlerTest();

			/**
			 * Default assignment operator.
			 */
			const GUIHandlerTest &operator=(const GUIHandlerTest &handler);

			/**
			 * Returns the name of the listener, as a string.
			 * @return Returns the name as a stdstring
			 */
			virtual const char *getName();

		protected:
			/**
			 * Create all frames for the GUI.
			 */
			virtual void init();

		private:
	};
}

#endif