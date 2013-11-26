/**
 * @file
 * @author Carl Nordlander <cand05@student.bth.se>
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

#include "CollisionObject.h"

using namespace gfx;
using std::string;

CollisionObject::CollisionObject(string className,int id)
{
        mClassName = className;
		mID = id;
		mpParent = NULL;
}

CollisionObject *CollisionObject::clone()
{
	return NULL;
}

bool CollisionObject::collision(const CollisionObject * pCollisionObject) const
{
        return false;
}

bool CollisionObject::intersection(float xStart, float yStart, float zStart, float xDir, float yDir, float zDir,float * pLenght) const
{
		return false;
}

void CollisionObject::move(float x,float y,float z)
{
		return;
}

void CollisionObject::setPosition(const D3DXVECTOR3 &position)
{

}

void CollisionObject::scale(const D3DXVECTOR3 &scale)
{

}

string CollisionObject::getClassName() const
{
        return mClassName;    
}

void CollisionObject::setID(int id)
{
		mID = id;
}

int CollisionObject::getID() const
{
		return mID;
}