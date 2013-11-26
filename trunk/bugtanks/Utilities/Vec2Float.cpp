/**
* @file
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
*/

#include "Vectors.h"
#include <cmath>

using namespace utilities;

Vec2Float::Vec2Float(const Vec3Float& vec3, bool useVec3Z) : x(vec3.x), y(vec3.z)
{
	if (!useVec3Z)
	{
		y = vec3.y;
	}
}

Vec2Float::Vec2Float(const Vec2Int& vec2Int) : x(static_cast<float>(vec2Int.x)), y(static_cast<float>(vec2Int.y))
{
}

Vec2Float& Vec2Float::operator =(const utilities::Vec2Float &vec2)
{
	x = vec2.x;
	y = vec2.y;
	return *this;
}

Vec2Float Vec2Float::operator *(float scalar) const
{
	Vec2Float newVector = *this;
	newVector.x *= scalar;
	newVector.y *= scalar;
	return newVector;
}

Vec2Float Vec2Float::operator -(const Vec2Float &vec2Float) const
{
	Vec2Float newVector = *this;
	newVector.x -= vec2Float.x;
	newVector.y -= vec2Float.y;
	return newVector;
}

Vec2Float Vec2Float::operator -(const Vec2Int &vec2Int) const
{
	return Vec2Float(x - static_cast<float>(vec2Int.x), y - static_cast<float>(vec2Int.y));
}

Vec2Float Vec2Float::operator -(const Vec3Float &vec3Float) const
{
	return Vec2Float(x - vec3Float.x, y - vec3Float.z);
}

float Vec2Float::length() const
{
	return sqrtf(x*x + y*y);
}

bool Vec2Float::longerThan(float length) const
{
	return (x*x + y*y) > (length * length);
}

bool Vec2Float::longerThan(const Vec2Float& vec2) const
{
	return (x*x + y*y) > (vec2.x*vec2.x + vec2.y*vec2.y);
}

Vec2Int Vec2Float::convertToMapCoordinates() const
{
	Vec2Int newVector(static_cast<int>(x*MAP_IN_WORLD_COORDS), static_cast<int>(y*MAP_IN_WORLD_COORDS));
	return newVector;
}