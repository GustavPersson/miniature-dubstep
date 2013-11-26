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

#ifndef __VEC2_FLOAT_H__
#define __VEC2_FLOAT_H__

#include "Macros.h"

#include <iomanip>

namespace utilities
{

// Forward declarations
struct Vec2Int;
struct Vec3Float;

/**
* A vector that contains 2 float values
* @see Vec3Float for a vector with 3 floats
*/
typedef struct Vec2Float
{
	float x;
	float y;

	/**
	* Default constructor, takes a x, y coordinate.
	* @param x x-coordinate, default is 0.0f.
	* @param y y-coordinate, default is 0.0f.
	*/
	inline Vec2Float(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

	/**
	* Converts a Vec3Float into a Vec2Float.
	* @param vec3 the vector to convert from
	* @param useVec3Z if we want to use the vec3's z-value as our y-value, true as default
	*/
	Vec2Float(const Vec3Float& vec3, bool useVec3Z = true);

	/**
	* Converts a Vec2Int into a Vec2Float.
	* @param vec2Int the vector to convert from
	*/
	Vec2Float(const Vec2Int& vec2Int);

	/**
	* Sets the vector and returns a reference to it
	* @param vec2 the vectors to copy
	* @return reference to the vector
	*/
	Vec2Float& operator=(const Vec2Float& vec2);

	/**
	* Scalar multiplication, returns a new vector
	* @param scalar the value to multiply the vector with
	* @return new vector with the scalar multiplication applied
	*/
	Vec2Float operator*(float scalar) const;

	/**
	* Returns the difference between the two vectors
	* @param vec2Float the right-sided vector
	* @return new difference vector
	*/
	Vec2Float operator-(const Vec2Float& vec2Float) const;

	/**
	* Returns the difference between the two vectors
	* @param vec2Int the right-sided vector
	* @return new difference vector as a Vec2Float
	*/
	Vec2Float operator-(const Vec2Int& vec2Int) const;

	/**
	* Returns the difference between the two vectors
	* @param vec3Float the right-sided vector
	* @return new difference vector as a Vec2Float
	*/
	Vec2Float operator-(const Vec3Float& vec3Float) const;

	/**
	* Returns the length of the vector
	* @return length of the vector
	* @note use longerThan or shorterThan when testing differences
	*/
	float length() const;
	
	/**
	* Test if the vector is longer than the specified length
	* @param length the length to test with
	* @return true if the vector is longer than 'length'
	*/
	bool longerThan(float length) const;

	/**
	* Test if the vector is longer than the parameter vector
	* @param vec2 the other vector to test with
	* @return true if the vector is longer than the parameter vector
	*/
	bool longerThan(const Vec2Float& vec2) const;

	/**
	* Test if the vector is shorter than the specified length
	* @param length the length to test with
	* @return true if the vector is shorter than 'length'
	*/
	inline bool shorterThan(float length) const
	{
		return !longerThan(length);
	}

	/**
	* Test if the vector is shorter than the parameter vector
	* @param vec2 the other vector to test with
	* @return true if the vector is shorter than the parameter vector
	*/
	inline bool shorterThan(const Vec2Float& vec2) const
	{
		return !longerThan(vec2);
	}

	/**
	* Converts the vector to map coordinates
	* @return map-coordinates
	*/
	Vec2Int convertToMapCoordinates() const;

	/**
	* Returns a string with information about the vector in a more human readable way.
	* @return string with the vector information
	*/
	inline std::string toString() const
	{
		std::stringstream ss;
		

		ss.setf(2);
		ss << std::fixed << "{" << x << ", " << y << "}";
		
		return ss.str();
	}

} PotentialPosition;
}

#endif