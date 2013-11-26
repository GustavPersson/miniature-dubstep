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

#include "Vec2Float.h"
#include "Vec3Float.h"
#include <cmath>

using namespace utilities;

Vec2Float::Vec2Float() : x(0.0f), y(0.0f) {}

Vec2Float::Vec2Float(float x, float y) : x(x), y(y) {}

Vec2Float::Vec2Float(const Vec3Float& vec3, bool useVec3Z) : x(vec3.x), y(vec3.z)
{
	if (!useVec3Z)
	{
		y = vec3.y;
	}
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

Vec2Float Vec2Float::operator -(const Vec2Float &vec2) const
{
	Vec2Float newVector = *this;
	newVector.x -= vec2.x;
	newVector.y -= vec2.y;
	return newVector;
}

float Vec2Float::length() const
{
	return sqrtf(x*x + y*y);
}

bool Vec2Float::longerThan(float length) const
{
	return (x*x + y*y) > (length * length);
}

bool Vec2Float::longerThan(const Vec3Float& vec2) const
{
	return (x*x + y*y) > (vec2.x*vec2.x + vec2.y*vec2.y);
}
