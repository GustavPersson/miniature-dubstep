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
*/

#ifndef __VEC3_FLOAT_H__
#define __VEC3_FLOAT_H__

#include "Macros.h"

namespace utilities
{

/**
* A struct that hold 3 floats with some extra functionalities.
* @see Vec2Float for a vector with 2 floats
*/
typedef struct Vec3Float
{
	float x;
	float y;
	float z;

	/**
	* Default constructor. Sets the vector to 0.0f.
	*/
	Vec3Float();

	/**
	* Constructor.
	* @param x Sets the x value.
	* @param y Sets the y value.
	* @param z Sets the z value.
	*/
	Vec3Float(float x, float y, float z);

	/**
	* Rotate the vector around y.
	* @param radian The angle to rotate in radians.
	*/
	void rotateAroundY(float radian);

	/**
	* Set the length of the vector to 1.
	*/
	void normalize();

	/**
	* Calculate the dot product between this vector and the parameter vector.
	* @param x The x value of the vector.
	* @param y The y value of the vector.
	* @param z The z value of the vector.
	* @return the dot value of the two vectors.
	*/
	float dotProduct(float x, float y, float z);

	/**
	* Calculate the dot product between this vector and vec.
	* @param vec3 The vector to calculate dot value of.
	* @return the dot value of the two vectors.
	*/
	float dotProduct(const Vec3Float& vec3) const;

	/**
	* Returns the length of the vector
	* @return the length of the vector
	*/
	float length() const;

	/**
	* Test if the vector is longer than the specified length
	* @param length the length to test with
	* @param useY if we want to use the y-coordinate or not, default is true
	* @return true if the vector is longer than 'length'
	*/
	bool longerThan(float length, bool useY = true) const;

	/**
	* Test if the vector is longer than the parameter vector
	* @param vec3 the other vector to test with
	* @param useY if we want to use the y-coordinate or not, default is true
	* @return true if the vector is longer than the parameter vector
	*/
	bool longerThan(const Vec3Float& vec3, bool useY = true) const;

	/**
	* Test if the vector is shorter than the specified length
	* @param length the length to test with
	* @param useY if we want to use the y-coordinate or not, default is true
	* @return true if the vector is shorter than 'length'
	*/
	inline bool shorterThan(float length, bool useY = true) const
	{
		return !longerThan(length, useY);
	}

	/**
	* Test if the vector is shorter than the parameter vector
	* @param vec3 the other vector to test with
	* @param useY if we want to use the y-coordinate or not, default is true
	* @return true if the vector is shorter than the parameter vector
	*/
	inline bool shorterThan(const Vec3Float& vec3, bool useY = true) const
	{
		return !longerThan(vec3, useY);
	}

	/**
	* Returns a new float with the difference of the two vectors
	* @param vec3 the vector to substract from the original vector
	* @return difference vector between right - left
	*/
	Vec3Float operator-(const Vec3Float& vec) const;

} Direction, Position;

}

#endif