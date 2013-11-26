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

#include "Vectors.h"
#include "Constants.h"
#include <D3DX10MATH.h>
#include <cmath>
#include <climits>

using namespace utilities;

const Vec3Float utilities::INVALID_WORLD_POSITON(FLT_MAX, FLT_MAX, FLT_MAX);

void Vec3Float::rotateAroundY(float radian)
{
	float tempX = cos(radian) * x - sin(radian) * z;
	z = sin(radian) * x + cos(radian) * z;
	x = tempX;
}

void Vec3Float::normalize()
{
	float length = this->length();
	if (length != 0.0f)
	{
		x /= length;
		y /= length;
		z /= length;
	}
}

float Vec3Float::dotProduct(float x, float y, float z)
{
	return this->x*x + this->y*y + this->z*z;
}

float Vec3Float::dotProduct(const Vec3Float& vec3) const
{
	return x*vec3.x + y*vec3.y + z*vec3.z;
}

float Vec3Float::length() const
{
	return sqrtf(x*x + y*y + z*z);
}

bool Vec3Float::longerThan(float length, bool useY) const
{
	if(useY)
	{
		return x*x + y*y + z*z > length * length;
	}
	else
	{
		return  x*x + z*z > length * length;
	}
}

bool Vec3Float::longerThan(const Vec3Float& vec3, bool useY) const
{
	if(useY)
	{
		return (x*x + y*y + z*z) > (vec3.x*vec3.x + vec3.y*vec3.y + vec3.z*vec3.z);
	}
	else
	{
		return  x*x + z*z > (vec3.x*vec3.x + vec3.z*vec3.z);
	}
}

Vec3Float Vec3Float::operator-(const Vec3Float& vec3) const
{
	Vec3Float diffVector = *this;
	diffVector.x -= vec3.x;
	diffVector.y -= vec3.y;
	diffVector.z -= vec3.z;
	return diffVector;
}

Vec3Float Vec3Float::operator +(const Vec3Float &vec3Float) const
{
	Vec3Float sumVec = *this;
	sumVec.x += vec3Float.x;
	sumVec.y += vec3Float.y;
	sumVec.z += vec3Float.z;
	return sumVec;
}

Vec3Float Vec3Float::operator *(float fValue) const
{
	Vec3Float productVec = *this;
	productVec.x *= fValue;
	productVec.y *= fValue;
	productVec.z *= fValue;
	return productVec;
}

Vec2Int Vec3Float::convertToMapCoordinates() const
{
	Vec2Int newVector(static_cast<int>(x*WORLD_IN_MAP_COORDS), static_cast<int>(z*WORLD_IN_MAP_COORDS));
	// Fix negative offset
	if (x < 0.0f)
	{
		newVector.x--;
	}
	if (z < 0.0f)
	{
		newVector.y--;
	}
	return newVector;
}

float Vec3Float::getXZAngleCounterclockwise() const
{
	// Because we compare with the (1,0) vector we don't have to multiply
	// the y coordinate for the dot product and thus the x value will be the
	// dot-product
	float angle = 0.0f;
	if (z < 0.0f)
	{
		angle = acosf(-x);
	}
	else
	{
		angle = acosf(x) + math::PI;
	}

	return angle;
}

float Vec3Float::getZXAngleClockwise() const
{
	// Because we compare with the (0,1) vector we don't have to multiply
	// the x coordinate for the dot product and thus the z value will be the
	// dot-product
	float angle = 0.0f;
	if (x < 0.0f)
	{
		angle = acosf(-z);
	}
	else
	{
		angle = acosf(z) + math::PI;
	}

	return angle;
}

D3DXVECTOR3 Vec3Float::convertToD3DXVECTOR3() const
{
	return D3DXVECTOR3(x, y, z);
}

float Vec3Float::getAngleBetweenVectors(Vec3Float vector) const
{
	float angle = fabsf(this->getXZAngleClockwise() - vector.getXZAngleClockwise());
	if(angle > math::PI)
	{
		angle -= math::PI;
	}
	return angle;
}

std::istream& operator>>(std::istream &in, utilities::Vec3Float& vec3Float)
{
	in >> vec3Float.x >> vec3Float.y >> vec3Float.z;
	return in;
}
std::ostream& operator<<(std::ostream &out, const utilities::Vec3Float& vec3Float)
{
	out << vec3Float.x << " " << vec3Float.y << " " << vec3Float.z << " ";
	return out;
}

utilities::Vec3Float operator*(float fValue, const utilities::Vec3Float& vec3Float)
{
	Vec3Float productVec = vec3Float;
	productVec.x *= fValue;
	productVec.y *= fValue;
	productVec.z *= fValue;
	return productVec;
}