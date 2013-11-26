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

bool Sphere::collision(const CollisionObject * pCollisionObject, bool calculateNormal)
{
	    if(pCollisionObject->getCollisionType()==CollisionType_Sphere)
		{
			Sphere * pSphere = (Sphere*) pCollisionObject;

			float distance = 0;

			distance += (pSphere->getX() - mX) * (pSphere->getX() - mX);
			distance += (pSphere->getY() - mY) * (pSphere->getY() - mY);
			distance += (pSphere->getZ() - mZ) * (pSphere->getZ() - mZ);

			float minDistance = (pSphere->getR() + mR) * (pSphere->getR() + mR);

			if(distance < minDistance)
			{
				if(calculateNormal)
				{
					D3DXVECTOR3 center = D3DXVECTOR3(mX, mY, mZ);
					D3DXVECTOR3 otherCenter = D3DXVECTOR3(pSphere->getX(), pSphere->getY(), pSphere->getZ());

					setCollisionNormal(otherCenter - center);
					pSphere->setCollisionNormal(center - otherCenter);
				}

				return true;
			}

			return false;
		}
		else if(pCollisionObject->getCollisionType()==CollisionType_AABB)
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

			if(distance < (mR * mR))
			{
				if(calculateNormal)
				{

				D3DXVECTOR3 center = D3DXVECTOR3(mX, mY, mZ);
				unsigned int n00b = 0;

				if(mX > pAABB->getMinX() && mX < pAABB->getMaxX())
				{
					n00b++;
				}

				if(mY > pAABB->getMinY() && mY < pAABB->getMaxY())
				{
					n00b++;
				}

				if(mZ > pAABB->getMinZ() && mZ < pAABB->getMaxZ())
				{
					n00b++;
				}

				D3DXVECTOR3 collisionNormal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

				if(n00b == 0)
				{
					D3DXVECTOR3 corner = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					if(mX < pAABB->getMinX())
					{
						if(mY < pAABB->getMinY())
						{
							if(mZ < pAABB->getMinZ())
							{
								corner = D3DXVECTOR3(pAABB->getMinX(), pAABB->getMinY(), pAABB->getMinZ());
							}
							else
							{
								corner = D3DXVECTOR3(pAABB->getMinX(), pAABB->getMinY(), pAABB->getMaxZ());
							}
						}
						else
						{
							if(mZ < pAABB->getMinZ())
							{
								corner = D3DXVECTOR3(pAABB->getMinX(), pAABB->getMaxY(), pAABB->getMinZ());
							}
							else
							{
								corner = D3DXVECTOR3(pAABB->getMinX(), pAABB->getMaxY(), pAABB->getMaxZ());
							}
						}
					}
					else
					{
						if(mY < pAABB->getMinY())
						{
							if(mZ < pAABB->getMinZ())
							{
								corner = D3DXVECTOR3(pAABB->getMaxX(), pAABB->getMinY(), pAABB->getMinZ());
							}
							else
							{
								corner = D3DXVECTOR3(pAABB->getMaxX(), pAABB->getMinY(), pAABB->getMaxZ());
							}
						}
						else
						{
							if(mZ < pAABB->getMinZ())
							{
								corner = D3DXVECTOR3(pAABB->getMaxX(), pAABB->getMaxY(), pAABB->getMinZ());
							}
							else
							{
								corner = D3DXVECTOR3(pAABB->getMaxX(), pAABB->getMaxY(), pAABB->getMaxZ());
							}
						}
					}

					setCollisionNormal(corner - center);
					pAABB->setCollisionNormal(center - corner);
				}
				else if(n00b == 1)
				{
					if(mX < pAABB->getMinX())
					{
						if(mY < pAABB->getMinY())
						{
							collisionNormal.x = pAABB->getMinX() - center.x;
							collisionNormal.y = pAABB->getMinY() - center.y;
						}
						else if(mY < pAABB->getMaxY())
						{
							if(mZ < pAABB->getMinZ())
							{
								collisionNormal.x = pAABB->getMinX() - center.x;
								collisionNormal.z = pAABB->getMinZ() - center.z;
							}
							else
							{
								collisionNormal.x = pAABB->getMinX() - center.x;
								collisionNormal.z = pAABB->getMaxZ() - center.z;
							}
						}
						else
						{
							collisionNormal.x = pAABB->getMinX() - center.x;
							collisionNormal.y = pAABB->getMaxY() - center.y;
						}
					}
					else if(mX < pAABB->getMaxX())
					{
						if(mY < pAABB->getMinY())
						{
							if(mZ < pAABB->getMinZ())
							{
								collisionNormal.y = pAABB->getMinY() - center.y;
								collisionNormal.z = pAABB->getMinZ() - center.z;
							}
							else
							{
								collisionNormal.y = pAABB->getMinY() - center.y;
								collisionNormal.z = pAABB->getMaxZ() - center.z;
							}
						}
						else
						{
							if(mZ < pAABB->getMinZ())
							{
								collisionNormal.y = pAABB->getMaxY() - center.y;
								collisionNormal.z = pAABB->getMinZ() - center.z;
							}
							else
							{
								collisionNormal.y = pAABB->getMaxY() - center.y;
								collisionNormal.z = pAABB->getMaxZ() - center.z;
							}
						}
					}
					else
					{
						if(mY < pAABB->getMinY())
						{
							collisionNormal.x = pAABB->getMaxX() - center.x;
							collisionNormal.y = pAABB->getMinY() - center.y;
						}
						else if(mY < pAABB->getMaxY())
						{
							if(mZ < pAABB->getMinZ())
							{
								collisionNormal.x = pAABB->getMaxX() - center.x;
								collisionNormal.z = pAABB->getMinZ() - center.z;
							}
							else
							{
								collisionNormal.x = pAABB->getMaxX() - center.x;
								collisionNormal.z = pAABB->getMaxZ() - center.z;
							}
						}
						else
						{
							collisionNormal.x = pAABB->getMaxX() - center.x;
							collisionNormal.y = pAABB->getMaxY() - center.y;
						}
					}

					setCollisionNormal(collisionNormal);
					pAABB->setCollisionNormal(-collisionNormal);
				}
				else if(n00b == 2)
				{
					if(mX < pAABB->getMinX())
					{
						collisionNormal.x = 1.0f;
					}
					else if(mX > pAABB->getMaxX())
					{
						collisionNormal.x = -1.0f;
					}
					else if(mY < pAABB->getMinY())
					{
						collisionNormal.y = 1.0f;
					}
					else if(mY > pAABB->getMaxY())
					{
						collisionNormal.y = -1.0f;
					}
					else if(mZ < pAABB->getMinZ())
					{
						collisionNormal.z = 1.0f;
					}
					else if(mZ > pAABB->getMaxZ())
					{
						collisionNormal.z = -1.0f;
					}

					setCollisionNormal(collisionNormal);
					pAABB->setCollisionNormal(-collisionNormal);
				}

				}

				return true;
			}

			return false;
		}
		else if(pCollisionObject->getCollisionType()==CollisionType_OBB)
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

			if(calculateNormal)
			{
				D3DXVECTOR3 center = D3DXVECTOR3(mX, mY, mZ);

				setCollisionNormal(pOBB->getCenter() - center);
				pOBB->setCollisionNormal(center - pOBB->getCenter());
			}
        
			return true;
		}

	return false;
}

bool Sphere::intersection(const D3DXVECTOR3 &start, const D3DXVECTOR3 &direction) const
{
	D3DXVECTOR3 d;

	D3DXVec3Normalize(&d, &direction);

	D3DXVECTOR3 center = D3DXVECTOR3(mX, mY, mZ);

	D3DXVECTOR3 oc = center - start;

	float t = d.x * oc.x + d.y * oc.y + d.z * oc.z;

	if(t<0)
	{
		return false;
	}

	float ocLenght = D3DXVec3LengthSq(&oc);

	float l = ocLenght - t * t;

	if(l>mR*mR)
	{
		return false;
	}

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