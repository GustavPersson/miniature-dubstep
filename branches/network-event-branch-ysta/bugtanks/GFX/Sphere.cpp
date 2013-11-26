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

#include "Sphere.h"

using namespace gfx;

Sphere::Sphere(float x,float y,float z,float r,int id) : CollisionObject("Sphere",id)
{
        mX = x;
        mY = y;
        mZ = z;
        mR = r;
		mBaseR = r;
}

CollisionObject *Sphere::clone()
{
	return myNew Sphere(*this);
}

bool Sphere::collision(const CollisionObject * pCollisionObject) const
{
		if(pCollisionObject->getClassName()=="Sphere")
		{
			Sphere * pSphere = (Sphere*) pCollisionObject;

			float distance = 0;

			distance += (pSphere->getX() - mX) * (pSphere->getX() - mX);
			distance += (pSphere->getY() - mY) * (pSphere->getY() - mY);
			distance += (pSphere->getZ() - mZ) * (pSphere->getZ() - mZ);

			float minDistance = (pSphere->getR() + mR) * (pSphere->getR() + mR);

			return distance < minDistance; 
		}
		else if(pCollisionObject->getClassName()=="AABB")
		{
			AABB * pAABB = (AABB*) pCollisionObject;

			float distance = 0;

			if(mX < pAABB->getMinX())
			{
				distance += (mX - pAABB->getMinX()) * (mX - pAABB->getMinX()); 
			}
			else if(mX > pAABB->getMaxX())
			{
				distance += (mX - pAABB->getMaxX()) * (mX - pAABB->getMaxX());
			}

			if(mY < pAABB->getMinY())
			{
				distance += (mY - pAABB->getMinY()) * (mY - pAABB->getMinY()); 
			}
			else if(mY > pAABB->getMaxY())
			{
				distance += (mY - pAABB->getMaxY()) * (mY - pAABB->getMaxY());
			}

			if(mZ < pAABB->getMinZ())
			{
				distance += (mZ - pAABB->getMinZ()) * (mZ - pAABB->getMinZ()); 
			}
			else if(mZ > pAABB->getMaxZ())
			{
				distance += (mZ - pAABB->getMaxZ()) * (mZ - pAABB->getMaxZ());
			}

			return distance < (mR * mR);
		}
		else if(pCollisionObject->getClassName()=="OBB")
		{
			OBB * pOBB = (OBB*) pCollisionObject;

			for(int aIndex=0;aIndex<3;aIndex++)
			{
				D3DXVECTOR3 axis = pOBB->getAxis(aIndex);

				float min1 = 1000000;
				float max1 = -1000000;
				float min2 = 1000000;
				float max2 = -1000000;
              
				for(int cIndex=0;cIndex<8;cIndex++)
				{
					float pos = axis.x * pOBB->getCorner(cIndex).x + axis.y * pOBB->getCorner(cIndex).y + axis.z * pOBB->getCorner(cIndex).z;    
                     
                     if(pos < min1)
                     {
                          min1 = pos;  
                     }
                     else if(pos > max1)     
                     {
                          max1 = pos;
                     }
				}
              
				min2 = axis.x * mX + axis.y * mY + axis.z * mZ - mR;
				max2 = axis.x * mX + axis.y * mY + axis.z * mZ + mR;
              
				if(max1 < min2 ||  min1 > max2)
				{
                     return false;       
				}          
			}
		
			D3DXVECTOR3 obbToSphere = D3DXVECTOR3(mX,mY,mZ);
			obbToSphere -= pOBB->getCenter();

			float length = obbToSphere.x * obbToSphere.x + obbToSphere.y * obbToSphere.y + obbToSphere.z * obbToSphere.z;
			length = sqrt(length);

			obbToSphere /= length;

			float min1 = 1000000;
			float max1 = -1000000;
			float min2 = 1000000;
			float max2 = -1000000;

			for(int cIndex=0;cIndex<8;cIndex++)
			{
				float pos = obbToSphere.x * pOBB->getCorner(cIndex).x + obbToSphere.y * pOBB->getCorner(cIndex).y + obbToSphere.z * pOBB->getCorner(cIndex).z;
                     
				if(pos < min1)
				{
                   min1 = pos;  
				}
				else if(pos > max1)     
				{
                   max1 = pos;
				}
			}
              
			min2 = obbToSphere.x * mX + obbToSphere.y * mY + obbToSphere.z * mZ - mR;
			max2 = obbToSphere.x * mX + obbToSphere.y * mY + obbToSphere.z * mZ + mR;
              
			if(max1 < min2 ||  min1 > max2)
			{
				return false;       
			}
        
			return true;
		}

	return false;
}

bool Sphere::intersection(float xStart, float yStart, float zStart, float xDir, float yDir, float zDir, float *pLenght) const
{
	D3DXVECTOR3 dir = D3DXVECTOR3(xDir,yDir,zDir);
	float dirLenght = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
	dirLenght = sqrt(dirLenght);

	dir.x /= dirLenght;
	dir.y /= dirLenght;
	dir.z /= dirLenght;


	D3DXVECTOR3 oc = D3DXVECTOR3(mX - xStart,mY - yStart,mZ - zStart);

	float t = dir.x * oc.x + dir.y * oc.y + dir.z * oc.z;

	if(t<0)
	{
		return false;
	}

	float ocLenght = oc.x * oc.x + oc.y * oc.y + oc.z * oc.z;

	float d = ocLenght - t * t;

	if(d>mR*mR)
	{
		return false;
	}

	float a = mR * mR - d;

	a = sqrt(a);

	t = t - a;

	(*pLenght) = t;

	return true;
}

void Sphere::move(float x,float y,float z)
{
	mX += x;
	mY += y;
	mZ += z;
}

void Sphere::setPosition(const D3DXVECTOR3 &position)
{
	mX = position.x;
	mY = position.y;
	mZ = position.z;
}

void Sphere::scale(const D3DXVECTOR3 &scale)
{
	float maxScale = max(scale.x, scale.y);
	maxScale = max(maxScale, scale.z);
	mR = mBaseR * maxScale;
}

const float Sphere::getX()
{
	return mX;
}

const float Sphere::getY()
{
	return mY;
}

const float Sphere::getZ()
{
	return mZ;
}

const float Sphere::getR()
{
	return mR;
}