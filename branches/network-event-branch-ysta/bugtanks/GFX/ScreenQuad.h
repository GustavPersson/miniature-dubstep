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

#ifndef SCREEN_QUAD_H
#define SCREEN_QUAD_H
#include "RenderObject.h"

namespace gfx
{
	/**
	 * Class for rendering non-rotatable "sprites". They are rendered without any
	 * view transform; only translation, scaling and a special 2D viewport-transform.
	 * In order to prevent confusing them with the more general sprites, they are
	 * called ScreenQuads instead.
	 */
	class ScreenQuad
	{
		public:
			/**
			 * Default constructor.
			 */
			ScreenQuad();

			/**
			 * Default copy constructor.
			 */
			ScreenQuad(const ScreenQuad &quad);

			/**
			 * Default destructor.
			 */
			virtual ~ScreenQuad();

			/**
			 * Default assignment operator.
			 */
			const ScreenQuad &operator=(const ScreenQuad &quad);

		protected:
			float mAbsoluteX;
			float mAbsoluteY;
			float mRelativeX;
			float mRelativeY;
			float mZ;

		private:
	};
}

#endif