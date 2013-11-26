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

#include "Vec3Float.h"
#include <cmath>

using namespace utilities;

Vec3Float::Vec3Float() : x(0.0f), y(0.0f), z(0.0f) {}

Vec3Float::Vec3Float(float x, float y, float z) : x(x), y(y), z(z) {}

void Vec3Float::rotateAroundY(float radian)
{
	x = cos(radian) * x - sin(radian) * z;
	z = sin(radian) * x + cos(radian) * z;
}

void Vec3Float::normalize()
{
	float length = this->length();
	x /= length;
	y /= length;
	z /= length;
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