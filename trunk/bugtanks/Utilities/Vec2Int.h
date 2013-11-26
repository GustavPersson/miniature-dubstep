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

#ifndef __VEC2_INT_H__
#define __VEC2_INT_H__

#include <climits>
#include <iostream>
#include <sstream>

namespace utilities
{

// forward declarations
struct Vec3Float;

/**
* A vector that holds two int, used for map-coordinates, etc.
*/
typedef struct Vec2Int
{
	int x;
	int y;

	/**
	* Default constructor, can also be used as a regular constructor.
	* @param x x-coordinate, default is 0
	* @param y y-coordinate, default is 0
	*/
	explicit Vec2Int(int x = 0, int y = 0) : x(x), y(y) {}

	/**
	* Equality operator
	* @param vec2Int the vector to compare with
	* @return true if the vectors are equal
	*/
	bool operator==(const Vec2Int& vec2Int) const;

	/**
	* Differ operator
	* @param vec2Int the vector to compare with
	* @return true if the vectors differ
	*/
	bool operator!=(const Vec2Int& vec2Int) const;

	/**
	* Tests if a vector is less than the specified vector
	* @param vec2Int the vector to compare with
	* @return true if the left vector is less than the right vector
	*/
	inline bool operator<(const Vec2Int& vec2Int) const {return (x < vec2Int.x || (x == vec2Int.x && y < vec2Int.y));}

	/**
	* Returns the difference between the two vectors as a new vector.
	* @param vec2Int the right-sided vector
	* @return new difference vector
	*/
	Vec2Int operator-(const Vec2Int& vec2Int) const;

	/**
	* Return the addition between the two vectors as a new vector.
	* @param vec2Int the right-sided vector
	* @return new difference vector
	*/
	Vec2Int operator+(const Vec2Int& vec2Int) const;

	/**
	* Bit operator that returns a new vector where the x and y values
	* have had their bits shifted to the right
	* @param bits the number of bits to shift
	* @return a new Vec2Int who's values has been shifted to the right
	*/
	inline Vec2Int operator>>(int bits) const
	{
		Vec2Int shiftedVec = *this;
		shiftedVec.x >>= bits;
		shiftedVec.y >>= bits;
		return shiftedVec;
	}

	/**
	* Increments both the x, and y with the specified value
	* @param value the value to add to the x, and y values.
	* @return reference to the current vector
	*/
	Vec2Int& operator+=(int value);

	/**
	* Increments the vector with the specified vector
	* @param vec2Int the right-sided vector
	* @return reference to the vector
	*/
	Vec2Int& operator+=(const Vec2Int& vec2Int);

	/**
	* Decrements the vector with the specified vector
	* @param vec2Int the right-sided vector
	* @return reference to the vector
	*/
	Vec2Int& operator-=(const Vec2Int& vec2Int);

	/**
	* Test if the vector is longer than the specified length
	* @param length the length to test with
	* @return true if the vector is longer than 'length'
	*/
	bool longerThan(int length) const;

	/**
	* Test if the vector is longer than the parameter vector
	* @param vec2Int the other vector to test with
	* @return true if the vector is longer than the parameter vector
	*/
	bool longerThan(const Vec2Int& vec2Int) const;

	/**
	* Test if the vector is shorter than the specified length
	* @param length the length to test with
	* @return true if the vector is shorter than 'length'
	*/
	inline bool shorterThan(int length) const
	{
		return !longerThan(length);
	}

	/**
	* Test if the vector is shorter than the parameter vector
	* @param vec2Int the other vector to test with
	* @return true if the vector is shorter than the parameter vector
	*/
	inline bool shorterThan(const Vec2Int& vec2Int) const
	{
		return !longerThan(vec2Int);
	}

	/**
	* Converts the Vec2Int into world coordinates.
	* @note Y in Vec2Int will be set to Z in Vec3Float
	* @return a new Vec3Float vector.
	*/
	Vec3Float convertToWorldCoordinates() const;

	/**
	* Returns a string with information about the vector in a more human readable way.
	* @return string with the vector information
	*/
	inline std::string toString() const
	{
		std::stringstream ss;
		
		ss << "{" << x << ", " << y << "}";

		return ss.str();
	}

} MapCoordinate;

const MapCoordinate INVALID_MAP_COORDINATE(INT_MIN, INT_MIN);	/**< Invalid map coordinate */

}

/**
* Stream out operator for Vec2Int
* @param out ostream object to write to
* @param vec2Int the vector to read from
* @return reference to the ostream object (out)
*/
std::ostream& operator<<(std::ostream& out, const utilities::Vec2Int& vec2Int);

/**
* Stream in operator for Vec2Int
* @param in istream object to read from
* @param vec2Int the vector to write to
* @return reference to the istream object (in)
*/
std::istream& operator>>(std::istream& in, utilities::Vec2Int& vec2Int);

#endif