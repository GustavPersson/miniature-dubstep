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

CollisionObject::CollisionObject(std::string className, logic::KeyType id)
{
		mID = id;
		mpParent = NULL;
		mCollisionNormal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		mCenterToCenterVectorNormalized = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		mIntersectionPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		if(className=="Sphere")
		{
			mCollisionType = CollisionType_Sphere;
		}
		else if(className=="AABB")
		{
			mCollisionType = CollisionType_AABB;
		}
		else if(className=="OBB")
		{
			mCollisionType = CollisionType_OBB;
		}
		else if(className=="Frustum")
		{
			mCollisionType = CollisionType_Frustum;
		}
		else
		{
			mCollisionType = CollisionType_Invalid;
		}

		mClassName = className;

}

CollisionObject *CollisionObject::clone()
{
	return NULL;
}

bool CollisionObject::intersection(const D3DXVECTOR3 &start, const D3DXVECTOR3 &direction)
{
		start;
		direction;
		return false;
}

void CollisionObject::move(float x,float y,float z)
{
		x;
		y;
		z;
		return;
}

void CollisionObject::setPosition(const D3DXVECTOR3 &position)
{
		position;
		return;
}

void CollisionObject::scale(const D3DXVECTOR3 &scale)
{
		scale;
		return;
}

void CollisionObject::rotateX(float angle)
{
		angle;
		return;
}

void CollisionObject::rotateY(float angle)
{
		angle;
		return;
}

void CollisionObject::rotateZ(float angle)
{
		angle;
		return;
}

float CollisionObject::getDistance(const D3DXVECTOR3 &point)
{
	return 0.0f;
}

void CollisionObject::setID(logic::KeyType id)
{
		mID = id;
}

void CollisionObject::setCollisionNormal(D3DXVECTOR3 collisionNormal)
{
		D3DXVec3Normalize(&collisionNormal, &collisionNormal);
		mCenterToCenterVectorNormalized = collisionNormal;
}