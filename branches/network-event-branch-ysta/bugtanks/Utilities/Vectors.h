/**
 * @file
 * @author Jonathan Udd <jonis.x@gmail.com>
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
 * This file contains various vector structs. 
 */

#ifndef __VECTORS_H__
#define __VECTORS_H__

#include <cmath>

namespace utilities
{
/**
* A struct that hold 3 floats with some extra functionalities.
*/
typedef struct Vec3Float
{
	float x;
	float y;
	float z;

	/**
	* Default constructor. Sets the vector to null.
	*/
	Vec3Float()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
	/**
	* Constructor.
	* @param x Sets the x value.
	* @param y Sets the y value.
	* @param z Sets the z value.
	*/
	Vec3Float(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	/**
	* Rotate the vector around y.
	* @param radian The angle to rotate in radians.
	*/
	void rotateAroundY(float radian)
	{
		this->x = cos(radian) * x - sin(radian) * z;
		this->z = sin(radian) * x + cos(radian) * z;
	}
	/**
	* Set the length of the vector to 1.
	*/
	void normalize()
	{
		float length = sqrt(x*x + y*y + z*z);
		x /= length;
		y /= length;
		z /= length;
	}
	/**
	* Calculate the dot product between this vector and the parameter vector.
	* @param x The x value of the vector.
	* @param y The y value of the vector.
	* @param z The z value of the vector.
	* @return Returns the dot value of the two vectors.
	*/
	float dotProduct(float x, float y, float z)
	{
		return this->x*x + this->y*y + this->z*z;
	}
	/**
	* Calculate the dot product between this vector and vec.
	* @param vec The vector to calculate dot value of.
	* @return Returns the dot value of the two vectors.
	*/
	float dotProduct(const Vec3Float& vec)
	{
		return this->x*vec.x + this->y*vec.y + this->z*vec.z;
	}
} Direction, Position;

}
#endif