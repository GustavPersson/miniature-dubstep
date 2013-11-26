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

#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H
#include "GUIFrame.h"

namespace gfx
{
	/**
	 * Class for GUI buttons.
	 */
	class GUIButton : public GUIFrame
	{
		public:
			/**
			 * Default constructor.
			 */
			GUIButton();

			/**
			 * Default copy constructor.
			 */
			GUIButton(const GUIButton &button);

			/**
			 * Default destructor.
			 */
			virtual ~GUIButton();

			/**
			 * Default assignment operator.
			 */
			const GUIButton &operator=(const GUIButton &button);

		protected:

		private:
	};
}

#endif