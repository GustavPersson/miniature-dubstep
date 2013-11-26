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
* 
* @section DESCRIPTION
*
* blaj
*/

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <cmath>

struct FixedCoordinate
{
	int x;
	int y;

	FixedCoordinate(int x = 0, int y = 0) : x(x), y(y) {}

	bool operator==(const FixedCoordinate& coord)
	{
		if (x == coord.x && y == coord.y)
		{
			return true;
		}
		return false;
	}

	bool operator!=(const FixedCoordinate& coord)
	{
		if (x != coord.x || y != coord.y)
		{
			return true;
		}
		return false;
	}

	bool operator<(const FixedCoordinate& coord)
	{
		if (x < coord.x && y < coord.y)
		{
			return true;
		}
		return false;
	}

	FixedCoordinate operator-(const FixedCoordinate& coord) const {
		FixedCoordinate temp = *this;
		temp.x -= coord.x;
		temp.y -= coord.y;
		return temp;
	}

	bool longerThan(const FixedCoordinate& coord)
	{
		return (x*x + y*y) > (coord.x*coord.x + coord.y*coord.y);
	}

};

struct FixedCoordinateCompare
{
	bool operator()(const FixedCoordinate& left, const FixedCoordinate& right)
	{
		return (left.x < right.x || (left.x == right.x && left.y < right.y));
	}
};

const FixedCoordinate INVALID_COORDINATE(-1, -1);

typedef struct Vec2 {
	float x;
	float y;

	Vec2() : x(0.0f), y(0.0f) {}

	Vec2(const Vec2& coord) : x(coord.x), y(coord.y) {}

	Vec2(const FixedCoordinate& coord) : x(static_cast<float>(coord.x)), y(static_cast<float>(coord.y)) {}

	Vec2(float x, float y) : x(x), y(y) {}

	Vec2(int x, int y) : x(static_cast<float>(x)), y(static_cast<float>(y)) {}

	Vec2& operator=(const Vec2& vec)
	{
		x = vec.x;
		y = vec.y;
		return *this;
	}

	Vec2& operator=(const FixedCoordinate& coord)
	{
		x = static_cast<float>(coord.x);
		y = static_cast<float>(coord.y);
		return *this;
	}

	// Scalar
	Vec2 operator*(const float value) const
	{
		Vec2 temp;
		temp.x = x * value;
		temp.y = y * value;
		return temp;
	}

	// Scalar
	float operator*(const Vec2& coord) const
	{
		float diffX = x - coord.x;
		float diffY = y - coord.y;
		return sqrt(diffX * diffX + diffY * diffY);
	}

	Vec2 operator-(const Vec2& coord) const {
		Vec2 temp = *this;
		temp.x -= coord.x;
		temp.y -= coord.y;
		return temp;
	}

	Vec2& operator+=(const Vec2& coord)
	{
		x += coord.x;
		y += coord.y;
		return *this;
	}

	Vec2& operator-=(const Vec2& coord)
	{
		x -= coord.x;
		y -= coord.y;
		return *this;
	}

	void normalize()
	{
		float vecLength = length();
		x /= vecLength;
		y /= vecLength;
	}

	float length()
	{
		return sqrtf(x*x + y*y);
	}

	bool longerThan(const Vec2& coord)
	{
		return (x*x + y*y) > (coord.x*coord.x + coord.y*coord.y);
	}

	bool longerThan(float length)
	{
		return (x*x + y*y) > (length*length);
	}

} Direction, Coordinate;

#endif