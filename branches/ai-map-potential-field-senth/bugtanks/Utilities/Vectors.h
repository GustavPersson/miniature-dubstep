/**
 * @file
 * @author Jonathan Udd <jonis.x@gmail.com>
 * @author Matteus Magnusson <senth.wallace@gmail.com>
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
 * 
 * @section DESCRIPTION
 *
 * This file includes all vectors 
 */

#ifndef __VECTORS_H__
#define __VECTORS_H__

#include "Vec2Int.h"
#include "Vec2Float.h"
#include "Vec3Float.h"

// ----------------------------------------------------------------------------------
// Constants for converting map coordinates into world coordinates
// and vice versa.
// ----------------------------------------------------------------------------------

namespace utilities {
/** The number of map coordinates you have to multiply to get the world coordinates. */
const float MAP_IN_WORLD_COORDS = 15.0f;	
/** Half the number of map coordinates you have to multiply to get the world coordinates. */
const float MAP_IN_WORLD_COORDS_HALF = MAP_IN_WORLD_COORDS * 0.5f;

/** Half the number of map coordinates as a Vec3Float
 * @see MAP_IN_WORLD_COORDS_HALF for the float value
 */
const Vec3Float MAP_IN_WORLD_COORDS_HALF_VEC(MAP_IN_WORLD_COORDS_HALF, 0.0f, MAP_IN_WORLD_COORDS_HALF);

/** The number of world coordinates you have to multiply to get the map coordinates. */
const float WORLD_IN_MAP_COORDS = 1.0f / MAP_IN_WORLD_COORDS;

/** An invalid position in world coordinates. Defined in Vec3Float.cpp */
extern const Vec3Float INVALID_WORLD_POSITON;

}

#endif