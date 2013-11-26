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
#include "Constants.h"
#include <sstream>

// Forward declaration
struct D3DXVECTOR3;

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
	* Default constructor that takes optional parameters.
	* @param x Sets the x value, default 0.0f
	* @param y Sets the y value, default 0.0f
	* @param z Sets the z value, default 0.0f
	*/
	explicit inline Vec3Float(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

	/**
	* Differ operator, note that the values must be exactly not equal.
	* @param vec3Float the right-sided vector to test with
	* @return true if the vectors differ
	*/
	inline bool operator!=(const Vec3Float& vec3Float)
	{
		return (x != vec3Float.x && y != vec3Float.y && z != vec3Float.z);
	}

	/**
	* Sum-assignment operator, adds another vector to the current vector
	* @param vec3Float the vector to add to the current vector
	* @return reference to this vector
	*/
	inline Vec3Float& operator+=(const Vec3Float& vec3Float)
	{
		x += vec3Float.x;
		y += vec3Float.y;
		z += vec3Float.z;
		return *this;
	}

	/**
	* Difference assignment operator, subtracts another vector from the current vector
	* @param vec3Float the vector to add to the current vector
	* @return reference to this vector
	*/
	inline Vec3Float& operator-=(const Vec3Float& vec3Float)
	{
		x -= vec3Float.x;
		y -= vec3Float.y;
		z -= vec3Float.z;
		return *this;
	}

	/**
	* Multiplies the vector with a float value
	* @param value the value to multiply the vector with
	* @return reference to this vector
	*/
	inline Vec3Float& operator*=(float value)
	{
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}

	/**
	* Returns a new Vec3Float with the difference of the two vectors
	* @param vec3 the vector to subtract from the original vector
	* @return difference vector between right - left
	*/
	Vec3Float operator-(const Vec3Float& vec3) const;

	/**
	* Returns a new Vec3Float with the sum of the two vectors
	* @param vec3Float the vector to add from the original vector
	* @return new Vec3Float with the sum of the two vectors
	*/
	Vec3Float operator+(const Vec3Float& vec3Float) const;

	/**
	* Returns a new Vec3Float with the multiplied value of the float
	* @param fValue the value to multiply with the elements in the vector
	* @return new Vec3Float with the multiplied value of the float
	*/
	Vec3Float operator*(float fValue) const;

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
	* Converts the vector to map coordinates
	* @return map-coordinates
	*/
	Vec2Int convertToMapCoordinates() const;

	/**
	* Returns the current angle between 1,0 and X,Z in radians in a counterclockwise manner
	* @return current angle between 1,0 and X,Z in radians, between 0 and 2PI, counterclockwise
	*/
	float getXZAngleCounterclockwise() const;

	/**
	* Returns the current angle between 1,0 and X,Z in radians in a clockwise manner
	* @return current angle between 1,0 and X,Z in radians, between 0 and 2PI, clockwise
	*/
	inline float getXZAngleClockwise() const
	{
		return utilities::math::PI_TWO - getXZAngleCounterclockwise();
	}

	/**
	* Returns the current angle between 0,1 and X,Z in radians, clockwise
	* Used for rotating meshes
	* @return current angle between 0,1 and X,Z in radians, between 0 AND 2PI, clockwise
	*/
	float getZXAngleClockwise() const;

	/**
	* Returns the current angle between 0,1 and X,Z in radians, counterclockwise
	* @return current angle between 0,1 and X,Z in radians, between 0 AND 2PI, counterclockwise
	*/
	inline float getZXAngleCounterclockwise() const {
		return utilities::math::PI_TWO - getZXAngleClockwise();
	}

	/**
	* Returns a string with information about the vector in a more human readable way.
	* @return string with the vector information
	*/
	inline std::string toString() const
	{
		std::stringstream ss;
		
		ss.setf(2);
		ss << std::fixed << "{" << x << ", " << y << ", " << z << "}";
		
		return ss.str();
	}

	/**
	* Returns a D3DXVECTOR3 struct for easy conversion
	* @return a D3DXVECTOR3 struct.
	*/
	D3DXVECTOR3 convertToD3DXVECTOR3() const;

	/**
	* Returns the smallest angle between this vector and an other.
	* @param vector The vector to compare angle with.
	* @return the smallest angle between the two vectors.
	*/
	float getAngleBetweenVectors(Vec3Float vector) const;

} Direction, Position;

}

/**
* Global Stream right operator for Vec3Float
* @param in the istream object to read from
* @param vec3Float the vector to write to
* @return the istream object (in)
*/
std::istream& operator>>(std::istream &in, utilities::Vec3Float& vec3Float);

/**
* Global Stream left operator for Vec3Float
* @param out the ostream object to write to
* @param vec3Float the vector to read from
* @return the ostream object (out)
*/
std::ostream& operator<<(std::ostream &out, const utilities::Vec3Float& vec3Float);

/**
* Global float multiplication operator for Vec3Float
* @param fValue the float value to multiply the vector with
* @param vec3Float the vector to multiply with
* @return a new Vec3Float with the vec3Float's value multiplied with fValue
*/
utilities::Vec3Float operator*(float fValue, const utilities::Vec3Float& vec3Float);

#endif