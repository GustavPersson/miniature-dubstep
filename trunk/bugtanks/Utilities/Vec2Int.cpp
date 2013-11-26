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

using namespace utilities;

bool Vec2Int::operator ==(const Vec2Int &vec2Int) const
{
	return (x == vec2Int.x && y == vec2Int.y);
}

bool Vec2Int::operator !=(const Vec2Int &vec2Int) const
{
	return (x != vec2Int.x || y != vec2Int.y);
}

Vec2Int Vec2Int::operator -(const Vec2Int &vec2Int) const
{
	Vec2Int diffVec = *this;
	diffVec.x -= vec2Int.x;
	diffVec.y -= vec2Int.y;
	return diffVec;
}

Vec2Int Vec2Int::operator +(const Vec2Int &vec2Int) const
{
	Vec2Int sumVec = *this;
	sumVec.x += vec2Int.x;
	sumVec.y += vec2Int.y;
	return sumVec;
}

Vec2Int& Vec2Int::operator+=(int value)
{
	x += value;
	y += value;
	return *this;
}

Vec2Int& Vec2Int::operator +=(const Vec2Int &Vec2Int)
{
	x += Vec2Int.x;
	y += Vec2Int.y;
	return *this;
}

Vec2Int& Vec2Int::operator -=(const Vec2Int &vec2Int)
{
	x -= vec2Int.x;
	y -= vec2Int.y;
	return *this;
}

bool Vec2Int::longerThan(int length) const
{
	return (x*x + y*y) > (length*length);
}

bool Vec2Int::longerThan(const Vec2Int &vec2Int) const
{
	return (x*x + y*y) > (vec2Int.x*vec2Int.x + vec2Int.y*vec2Int.y);
}

Vec3Float Vec2Int::convertToWorldCoordinates() const
{
	return Vec3Float((static_cast<float>(x)*MAP_IN_WORLD_COORDS) + MAP_IN_WORLD_COORDS_HALF, 0.0f, (static_cast<float>(y)*MAP_IN_WORLD_COORDS) + MAP_IN_WORLD_COORDS_HALF);
}

std::ostream& operator<<(std::ostream& out, const utilities::Vec2Int& vec2Int)
{
	out << vec2Int.x << " " << vec2Int.y << " ";
	return out;
}

std::istream& operator>>(std::istream& in, utilities::Vec2Int& vec2Int)
{
	in >> vec2Int.x >> vec2Int.y;
	return in;
}