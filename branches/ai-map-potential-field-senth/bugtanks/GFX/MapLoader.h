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

#ifndef MAP_LOADER_H
#define MAP_LOADER_H
#include "DX10Module.h"

namespace gfx
{
	/**
	 * Temporary map loader.
	 */
	class MapLoader
	{
		public:
			/**
			 * Default constructor.
			 */
			MapLoader();

			/**
			 * Default destructor.
			 */
			virtual ~MapLoader();

			/**
			 * Loads a map file.
			 * @param filePath Absolute/relative path of the map file.
			 */
			static void loadMap(const char *filePath);

		protected:

		private:
	};
}

#endif