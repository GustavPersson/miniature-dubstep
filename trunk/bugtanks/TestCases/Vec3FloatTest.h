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

#include <cxxtest/TestSuite.h>
#include "../Utilities/Vectors.h"
#include "../Utilities/Constants.h"

using utilities::Vec3Float;
using utilities::MapCoordinate;

using namespace utilities::math;

class Vec3FloatTest : public CxxTest::TestSuite
{
public:
	void testGetXZAngleCounterclockwise()
	{
		Vec3Float vec10__1(1.0f,0,0.0001f);
		Vec3Float vec10__2(1.0f,0,-0.0001f);
		Vec3Float vec01__1(0.0001f,0,1.0f);
		Vec3Float vec01__2(-0.0001f,0,1.0f);
		Vec3Float vec_10__1(-1.0f,0,0.0001f);
		Vec3Float vec_10__2(-1.0f,0,-0.0001f);
		Vec3Float vec0_1__1(0.0001f,0,-1.0f);
		Vec3Float vec0_1__2(-0.0001f,0,-1.0f);

		// normalize
		vec10__1.normalize();
		vec10__2.normalize();
		vec01__1.normalize();
		vec01__2.normalize();
		vec_10__1.normalize();
		vec_10__2.normalize();
		vec0_1__1.normalize();
		vec0_1__2.normalize();

		TS_ASSERT_DELTA(vec10__1.getXZAngleCounterclockwise(), PI, 0.01f);
		TS_ASSERT_DELTA(vec10__2.getXZAngleCounterclockwise(), PI, 0.01f);
		TS_ASSERT_DELTA(vec01__1.getXZAngleCounterclockwise(), PI + PI_HALF, 0.01f);
		TS_ASSERT_DELTA(vec01__2.getXZAngleCounterclockwise(), PI + PI_HALF, 0.01f);
		TS_ASSERT_DELTA(vec_10__1.getXZAngleCounterclockwise(), PI + PI, 0.01f);
		TS_ASSERT_DELTA(vec_10__2.getXZAngleCounterclockwise(), 0.0f, 0.01f);
		TS_ASSERT_DELTA(vec0_1__1.getXZAngleCounterclockwise(), PI_HALF, 0.01f);
		TS_ASSERT_DELTA(vec0_1__2.getXZAngleCounterclockwise(), PI_HALF, 0.01f);
	}

	void testGetZXAngleClockwise()
	{
		Vec3Float vec10__1(1.0f,0,0.0001f);
		Vec3Float vec10__2(1.0f,0,-0.0001f);
		Vec3Float vec01__1(0.0001f,0,1.0f);
		Vec3Float vec01__2(-0.0001f,0,1.0f);
		Vec3Float vec_10__1(-1.0f,0,0.0001f);
		Vec3Float vec_10__2(-1.0f,0,-0.0001f);
		Vec3Float vec0_1__1(0.0001f,0,-1.0f);
		Vec3Float vec0_1__2(-0.0001f,0,-1.0f);

		// normalize
		vec10__1.normalize();
		vec10__2.normalize();
		vec01__1.normalize();
		vec01__2.normalize();
		vec_10__1.normalize();
		vec_10__2.normalize();
		vec0_1__1.normalize();
		vec0_1__2.normalize();

		TS_ASSERT_DELTA(vec10__1.getZXAngleClockwise(), PI + PI_HALF, 0.01f);
		TS_ASSERT_DELTA(vec10__2.getZXAngleClockwise(), PI + PI_HALF, 0.01f);
		TS_ASSERT_DELTA(vec01__1.getZXAngleClockwise(), PI, 0.01f);
		TS_ASSERT_DELTA(vec01__2.getZXAngleClockwise(), PI, 0.01f);
		TS_ASSERT_DELTA(vec_10__1.getZXAngleClockwise(), PI_HALF, 0.01f);
		TS_ASSERT_DELTA(vec_10__2.getZXAngleClockwise(), PI_HALF, 0.01f);
		TS_ASSERT_DELTA(vec0_1__1.getZXAngleClockwise(), PI_TWO, 0.01f);
		TS_ASSERT_DELTA(vec0_1__2.getZXAngleClockwise(), 0.0f, 0.01f);
	}

	void testRotateAroundY()
	{
		Vec3Float vec10__1(1.001f, 0.0f, 0.001f);
		Vec3Float vec10__2(0.999f, 0.0f, 0.001f);

		// normalize
		vec10__1.normalize();
		vec10__2.normalize();

		// Rotate 90 degrees to the left
		Vec3Float vecTest = vec10__1;
		vecTest.rotateAroundY(PI_HALF);
		vecTest.normalize();
		TS_ASSERT_DELTA(vecTest.x, 0.0f, 0.01f);
		TS_ASSERT_DELTA(vecTest.z, 1.0f, 0.01f);

		vecTest = vec10__2;
		vecTest.rotateAroundY(PI_HALF);
		vecTest.normalize();
		TS_ASSERT_DELTA(vecTest.x, 0.0f, 0.01f);
		TS_ASSERT_DELTA(vecTest.z, 1.0f, 0.01f);

		
		// Rotate 90 degrees to the right
		vecTest = vec10__1;
		vecTest.rotateAroundY(-PI_HALF);
		vecTest.normalize();
		TS_ASSERT_DELTA(vecTest.x, 0.0f, 0.01f);
		TS_ASSERT_DELTA(vecTest.z, -1.0f, 0.01f);

		vecTest = vec10__2;
		vecTest.rotateAroundY(-PI_HALF);
		vecTest.normalize();
		TS_ASSERT_DELTA(vecTest.x, 0.0f, 0.01f);
		TS_ASSERT_DELTA(vecTest.z, -1.0f, 0.01f);
	}

	void testConvertToMapCoordinates()
	{
		// Test close to 0 value
		Vec3Float vec(0.0f, 0.0f, 0.0f);
		MapCoordinate map(0,0);

		TS_ASSERT_EQUALS(vec.convertToMapCoordinates(), map);

		vec.x = 0.01f;
		TS_ASSERT_EQUALS(vec.convertToMapCoordinates(), map);

		vec.z = 0.01f;
		TS_ASSERT_EQUALS(vec.convertToMapCoordinates(), map);

		vec.x = 0.0f;
		vec.z = 0.01f;
		TS_ASSERT_EQUALS(vec.convertToMapCoordinates(), map);

		map.y = -1;
		vec.z = -0.01f;
		TS_ASSERT_EQUALS(vec.convertToMapCoordinates(), map);

		map.x = -1;
		vec.x = -0.01f;
		TS_ASSERT_EQUALS(vec.convertToMapCoordinates(), map);

		map.y = 0;
		vec.z = 0.0f;
		TS_ASSERT_EQUALS(vec.convertToMapCoordinates(), map);
	}

	void testMultiOperator()
	{
		Vec3Float vec(1.0f, 1.0f, 1.0f);

		Vec3Float productVec = vec * 1.0f;
		TS_ASSERT_DELTA(productVec.x, 1.0f, 0.001f);
		TS_ASSERT_DELTA(productVec.y, 1.0f, 0.001f);
		TS_ASSERT_DELTA(productVec.z, 1.0f, 0.001f);

		productVec = vec * 5.0f;
		TS_ASSERT_DELTA(productVec.x, 5.0f, 0.001f);
		TS_ASSERT_DELTA(productVec.y, 5.0f, 0.001f);
		TS_ASSERT_DELTA(productVec.z, 5.0f, 0.001f);

		productVec = 0.5f * vec;
		TS_ASSERT_DELTA(productVec.x, 0.5f, 0.001f);
		TS_ASSERT_DELTA(productVec.y, 0.5f, 0.001f);
		TS_ASSERT_DELTA(productVec.z, 0.5f, 0.001f);

		vec = Vec3Float(1.0f, 0.0f, 0.0f);

		productVec = 20.0f * vec * 0.5f;

		Vec3Float finalVec(10.0f, 0.0f, 0.0f);
		TS_ASSERT_DELTA(productVec.x, finalVec.x, 0.001f);
	}

};