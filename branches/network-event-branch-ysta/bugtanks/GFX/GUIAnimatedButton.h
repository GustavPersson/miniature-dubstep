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

#ifndef GUI_ANIMATED_BUTTON_H
#define GUI_ANIMATED_BUTTON_H
#include "GUIAnimatedFrame.h"

namespace gfx
{
	/**
	 * Class for animated buttons.
	 */
	class GUIAnimatedButton : GUIAnimatedFrame
	{
		public:
			/**
			 * Default constructor.
			 */
			GUIAnimatedButton();

			/**
			 * Default copy constructor.
			 */
			GUIAnimatedButton(const GUIAnimatedButton &button);

			/**
			 * Default destructor.
			 */
			virtual ~GUIAnimatedButton();

			/**
			 * Default assignment operator.
			 */
			const GUIAnimatedButton &operator=(const GUIAnimatedButton &button);

		protected:
		private:
	};
}

#endif