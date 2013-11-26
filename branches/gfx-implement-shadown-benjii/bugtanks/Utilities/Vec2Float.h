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

namespace utilities
{

// Forward declarations
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
	* Default constructor. Initializes the vector to 0.0f.
	*/
	Vec2Float();

	/**
	* Constructor, takes a x, y coordinate.
	* @param x x-coordinate
	* @param y y-coordinate
	*/
	Vec2Float(float x, float y);

	/**
	* Converts a Vec3Float into a Vec2Float.
	* @param vec3 the vector to convert from
	* @param useVec3Z if we want to use the vec3's z-value as our y-value, true as default
	*/
	Vec2Float(const Vec3Float& vec3, bool useVec3Z = true);

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
	* @param vec2 the right-sided vector
	* @return new difference vector
	*/
	Vec2Float operator-(const Vec2Float& vec2) const;

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
	bool longerThan(const Vec3Float& vec2) const;

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
	inline bool shorterThan(const Vec3Float& vec2) const
	{
		return !longerThan(vec2);
	}

} PotentialPosition;
}

#endif