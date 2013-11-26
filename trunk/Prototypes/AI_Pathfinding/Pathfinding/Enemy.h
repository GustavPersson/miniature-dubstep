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
*
* The enemy
*/

#ifndef __ENEMY_H__
#define __ENEMY_H__

const int ENEMY_WIDTH = 4;
const int ENEMY_HEIGHT = 2;
const float ENEMY_RANGE = 5.0f;
const int ENEMY_ATTRACT_FORCE = 10;


#include "Agent.h"
#include "PathFinder.h"

class Enemy
{
public:
	Enemy(
		int width,
		int height,
		const FixedCoordinate& position,
		float range
		);
	int getWidth() const;
	int getHeight() const;
	FixedCoordinate getPosition() const;
	void setPosition(const FixedCoordinate& newPosition);
	float getRange() const;
	float calculatePotentialValue(float distance);
private:
	//int mPotentialMap[MAP_WIDTH][MAP_HEIGHT];

	int mWidth;
	int mHeight;	
	FixedCoordinate	mPosition;
	float mRange;
};


#endif