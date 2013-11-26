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
* Various constants
*/

#ifndef __UTILITIES__CONSTANTS_H__
#define __UTILITIES__CONSTANTS_H__

namespace utilities {
	namespace math {
		const float PI = 3.14159265f;
		const float PI_HALF = 1.57079633f;
		const float PI_QUARTER = 0.785398163f;
		const float PI_TWO = 6.28318531f;

		/**
		* Function that converts degrees to radians
		* @param degrees the number of degrees
		* @return the number of degrees in radians
		*/
		inline float degToRad(float degrees) {return degrees / 360.0f * PI_TWO;}

		/**
		* Function that converts radians to degrees
		* @param radians the number of radians
		* @return the number of radians in degrees
		*/
		inline float radToDeg(float radian) {return radian / PI_TWO * 360.0f;}
	}
}

#endif