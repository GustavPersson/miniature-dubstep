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

#include "AABB.h"

using namespace gfx;

AABB::AABB(float minX,float minY,float minZ,float maxX,float maxY,float maxZ,int id) : CollisionObject("AABB",id),
	mCenter(0.0f, 0.0f, 0.0f)
{
      mMinX = minX;
      mMinY = minY;
      mMinZ = minZ;
      mMaxX = maxX;
      mMaxY = maxY;
      mMaxZ = maxZ; 

	  mBaseSize = D3DXVECTOR3((maxX - minX)/2, (maxY - minY)/2, (maxZ - minZ)/2);

	  updateCorners();
}

CollisionObject *AABB::clone()
{
	return myNew AABB(*this);
}

bool AABB::collision(const CollisionObject * pCollisionObject, bool calculateNormal)
{
	  if(pCollisionObject->getCollisionType()==CollisionType_AABB)
      {
           AABB * pAABB = (AABB*)pCollisionObject;
           
           if(pAABB->getMaxX()<mMinX)
           {
                 return false; 
           }
           else if(pAABB->getMinX()>mMaxX)
           {
                 return false;
           }
           else if(pAABB->getMaxY()<mMinY)
           {
                 return false;                   
           }
           else if(pAABB->getMinY()>mMaxY)
           {
                 return false;
           }
           else if(pAABB->getMaxZ()<mMinZ)
           {
                 return false;                   
           }
           else if(pAABB->getMinZ()>mMaxZ)
           {
                 return false;
           }

		   if(calculateNormal)
		   {
			   float smalestDistance = abs(pAABB->getMaxX() - mMinX);
			   D3DXVECTOR3 collisionNormal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

			   if(abs(mMaxX - pAABB->getMinX()) < smalestDistance)
			   {
					smalestDistance = abs(mMaxX - pAABB->getMinX());
					collisionNormal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			   }
			   else if(abs(pAABB->getMaxY() - mMinY) < smalestDistance)
			   {
					smalestDistance = abs(pAABB->getMaxY() - mMinY);
					collisionNormal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			   }
			   else if(abs(mMaxY - pAABB->getMinY()) < smalestDistance)
			   {
					smalestDistance = abs(mMaxY - pAABB->getMinY());
					collisionNormal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			   }
			   else if(abs(pAABB->getMaxZ() - mMinZ) < smalestDistance)
			   {
					smalestDistance = abs(pAABB->getMaxZ() - mMinZ);
					collisionNormal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			   }
			   else if(abs(mMaxZ - pAABB->getMinZ()) < smalestDistance)
			   {
					smalestDistance = abs(mMaxY - pAABB->getMinZ());
					collisionNormal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			   }

			   setCollisionNormal(collisionNormal);
			   pAABB->setCollisionNormal(-collisionNormal);
		   }
           
           return true;           
      }
	  else if(pCollisionObject->getCollisionType()==CollisionType_OBB)
      {
           OBB * pOBB = (OBB*)pCollisionObject;
           
           D3DXVECTOR3 aabbCorners[8];
		
		   aabbCorners[0] = D3DXVECTOR3(mMinX,mMinY,mMinZ);
		   aabbCorners[1] = D3DXVECTOR3(mMinX,mMinY,mMaxZ);
		   aabbCorners[2] = D3DXVECTOR3(mMinX,mMaxY,mMinZ);
		   aabbCorners[3] = D3DXVECTOR3(mMinX,mMaxY,mMaxZ);
		   aabbCorners[4] = D3DXVECTOR3(mMaxX,mMinY,mMinZ);
		   aabbCorners[5] = D3DXVECTOR3(mMaxX,mMinY,mMaxZ);
		   aabbCorners[6] = D3DXVECTOR3(mMaxX,mMaxY,mMinZ);
		   aabbCorners[7] = D3DXVECTOR3(mMaxX,mMaxY,mMaxZ);
		   
		   D3DXVECTOR3 aabbAxis[3];
		   
		   aabbAxis[0] = D3DXVECTOR3(1,0,0);
		   aabbAxis[1] = D3DXVECTOR3(0,1,0);
		   aabbAxis[2] = D3DXVECTOR3(0,0,1);
           
           for(int aIndex=0;aIndex<3;aIndex++)
	       {  
              float min1 = 1000000;
              float max1 = -1000000;
              float min2 = 1000000;
              float max2 = -1000000;
              
              for(int cIndex=0;cIndex<8;cIndex++)
              {
                     float pos = aabbAxis[aIndex].x * aabbCorners[cIndex].x + aabbAxis[aIndex].y * aabbCorners[cIndex].y + aabbAxis[aIndex].z * aabbCorners[cIndex].z;    
                     
                     if(pos < min1)
                     {
                          min1 = pos;  
                     }
                     else if(pos > max1)     
                     {
                          max1 = pos;
                     }                   
              }
              
              for(int cIndex=0;cIndex<8;cIndex++)
              {
                     float pos = aabbAxis[aIndex].x * pOBB->getCorner(cIndex).x + aabbAxis[aIndex].y * pOBB->getCorner(cIndex).y  + aabbAxis[aIndex].z * pOBB->getCorner(cIndex).z;    
                     
                     if(pos < min2)
                     {
                          min2 = pos;  
                     }
                     else if(pos > max2)     
                     {
                          max2 = pos;
                     }                   
              }
              
              if(max1 < min2 || max2 < min1)
              {
                     return false;       
              }          
           }

		   for(int aIndex=0;aIndex<3;aIndex++)
	       {  
			  D3DXVECTOR3 axis = pOBB->getAxis(aIndex);	

              float min1 = 1000000;
              float max1 = -1000000;
              float min2 = 1000000;
              float max2 = -1000000;
              
              for(int cIndex=0;cIndex<8;cIndex++)
              {
                     float pos = axis.x * aabbCorners[cIndex].x + axis.y * aabbCorners[cIndex].y + axis.z * aabbCorners[cIndex].z;    
                     
                     if(pos < min1)
                     {
                          min1 = pos;  
                     }
                     else if(pos > max1)     
                     {
                          max1 = pos;
                     }                   
              }
              
              for(int cIndex=0;cIndex<8;cIndex++)
              {
                     float pos = axis.x * pOBB->getCorner(cIndex).x + axis.y * pOBB->getCorner(cIndex).y  + axis.z * pOBB->getCorner(cIndex).z;    
                     
                     if(pos < min2)
                     {
                          min2 = pos;  
                     }
                     else if(pos > max2)     
                     {
                          max2 = pos;
                     }                   
              }
              
              if(max1 < min2 || max2 < min1)
              {
                     return false;       
              }          
           }

		   if(calculateNormal)
		   {
				D3DXVECTOR3 center = D3DXVECTOR3((mMinX + mMaxX)/2, (mMinY +mMaxY)/2, (mMinZ + mMaxZ)/2);

				setCollisionNormal(pOBB->getCenter() - center);
				pOBB->setCollisionNormal(center - pOBB->getCenter());
		   }
        
           return true;
      }
	  else if(pCollisionObject->getCollisionType()==CollisionType_Sphere)
      {
            Sphere * pSphere = (Sphere*) pCollisionObject;

			float distance = 0;

			if(pSphere->getX() < mMinX)
			{
				distance += (pSphere->getX() - mMinX) * (pSphere->getX() - mMinX); 
			}
			else if(pSphere->getX() > mMaxX)
			{
				distance += (pSphere->getX() - mMaxX) * (pSphere->getX() - mMaxX);
			}

			if(pSphere->getY() < mMinY)
			{
				distance += (pSphere->getY() - mMinY) * (pSphere->getY() - mMinY); 
			}
			else if(pSphere->getY() > mMaxY)
			{
				distance += (pSphere->getY() - mMaxY) * (pSphere->getY() - mMaxY);
			}

			if(pSphere->getZ() < mMinZ)
			{
				distance += (pSphere->getZ() - mMinZ) * (pSphere->getZ() - mMinZ); 
			}
			else if(pSphere->getZ() > mMaxZ)
			{
				distance += (pSphere->getZ() - mMaxZ) * (pSphere->getZ() - mMaxZ);
			}

			if(distance < (pSphere->getR() * pSphere->getR()))
			{
				if(calculateNormal)
				{
					D3DXVECTOR3 center = D3DXVECTOR3(pSphere->getX(), pSphere->getY(), pSphere->getZ());
					unsigned int n00b = 0;

					if(center.x > mMinX && center.x < mMaxX)
					{
						n00b++;
					}

					if(center.y > mMinY && center.y < mMaxY)
					{
						n00b++;
					}

					if(center.z > mMinZ && center.z < mMaxZ)
					{
						n00b++;
					}

					D3DXVECTOR3 collisionNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					if(n00b == 0)
					{
						D3DXVECTOR3 corner = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

						if(center.x < mMinX)
						{
							if(center.y < mMinY)
							{
								if(center.z < mMinZ)
								{
									corner = D3DXVECTOR3(mMinX, mMinY, mMinZ);
								}
								else
								{
									corner = D3DXVECTOR3(mMaxX, mMinY, mMinZ);
								}
							}
							else
							{
								if(center.z < mMinZ)
								{
									corner = D3DXVECTOR3(mMinX, mMaxY, mMinZ);
								}
								else
								{
									corner = D3DXVECTOR3(mMinX, mMaxY, mMaxZ);
								}
							}
						}
						else
						{
							if(center.y < mMinY)
							{
								if(center.z < mMinZ)
								{
									corner = D3DXVECTOR3(mMaxX, mMinY, mMinZ);
								}
								else
								{
									corner = D3DXVECTOR3(mMaxX, mMinY, mMaxZ);
								}
							}
							else
							{
								if(center.z < mMinZ)
								{
									corner = D3DXVECTOR3(mMaxX, mMaxY, mMinZ);
								}
								else
								{
									corner = D3DXVECTOR3(mMaxX, mMaxY, mMaxZ);
								}
							}
						}

						collisionNormal = center - corner;
					}
					else if(n00b == 1)
					{
						if(center.x < mMinX)
						{
							if(center.y < mMinY)
							{
								collisionNormal.x = center.x - mMinX;
								collisionNormal.y = center.y - mMinY;
							}
							else if(center.y < mMaxY)
							{
								if(center.z < mMinZ)
								{
									collisionNormal.x = center.x - mMinX;
									collisionNormal.z = center.z - mMinZ;
								}
								else
								{
									collisionNormal.x = center.x - mMinX;
									collisionNormal.z = center.z - mMaxZ;
								}
							}
							else
							{
								collisionNormal.x = center.x - mMinX;
								collisionNormal.y = center.y - mMaxY;
							}
						}
						else if(center.x < mMaxX)
						{
							if(center.y < mMinY)
							{
								if(center.z < mMinZ)
								{
									collisionNormal.y = center.y - mMinY;
									collisionNormal.z = center.z - mMinZ;
								}
								else
								{
									collisionNormal.y = center.y - mMinY;
									collisionNormal.z = center.z - mMaxZ;
								}
							}
							else
							{
								if(center.z < mMinZ)
								{
									collisionNormal.y = center.y - mMaxY;
									collisionNormal.z = center.z - mMinZ;
								}
								else
								{
									collisionNormal.y = center.y - mMaxY;
									collisionNormal.z = center.z - mMaxZ;
								}
							}
						}
						else
						{
							if(center.y < mMinY)
							{
								collisionNormal.x = center.x - mMaxX;
								collisionNormal.y = center.y - mMinY;
							}
							else if(center.y < mMaxY)
							{
								if(center.z < mMinZ)
								{
									collisionNormal.x = center.x - mMaxX;
									collisionNormal.z = center.z - mMinZ;
								}
								else
								{
									collisionNormal.x = center.x - mMaxX;
									collisionNormal.z = center.z - mMaxZ;
								}
							}
							else
							{
								collisionNormal.x = center.x - mMaxX;
								collisionNormal.y = center.y - mMaxY;
							}
						}
					}
					else if(n00b == 2)
					{
						if(center.x < mMinX)
						{
							collisionNormal.x = -1.0f;
						}
						else if(center.x > mMaxX)
						{
							collisionNormal.x = 1.0f;
						}
						else if(center.y < mMinY)
						{
							collisionNormal.y = -1.0f;
						}
						else if(center.y > mMaxY)
						{
							collisionNormal.y = 1.0f;
						}
						else if(center.z < mMinZ)
						{
							collisionNormal.z = -1.0f;
						}
						else if(center.z > mMaxZ)
						{
							collisionNormal.z = 1.0f;
						}
					}

					setCollisionNormal(collisionNormal);
					pSphere->setCollisionNormal(-collisionNormal);
				}

				return true;
			}

			return false;
      }

	  return false;
}

bool AABB::intersection(const D3DXVECTOR3 &start, const D3DXVECTOR3 &direction) const
{
	float tMax, tMin, tZMin, tZMax, tYMin, tYMax;

	if(direction.x>=0)
    {
		tMin = (mMinX-start.x)/direction.x;
		tMax = (mMaxX-start.x)/direction.x;                
	}
	else
	{
		tMin = (mMaxX-start.x)/direction.x;
		tMax = (mMinX-start.x)/direction.x;
	}

	if(direction.y>=0)
	{
		tYMin = (mMinY-start.y)/direction.y;
		tYMax = (mMaxY-start.y)/direction.y;                
	}
	else
	{
		tYMin = (mMaxY-start.y)/direction.y;
		tYMax = (mMinY-start.y)/direction.y;
	}

	if(tMin>tYMax||tYMin>tMax)
	{
		return false;                         
	}
    
	if(tYMin>tMin)
	{
		tMin = tYMin;                      
	}
    
	if(tYMax<tMax)
	{
		tMax = tYMax;             
	}
    
	if(direction.z>=0)
	{
		tZMin = (mMinZ-start.z)/direction.z;
		tZMax = (mMaxZ-start.z)/direction.z;                
	}
	else
	{
		tZMin = (mMaxZ-start.z)/direction.z;
		tZMax = (mMinZ-start.z)/direction.z;
	}
    
	if(tMin>tZMax||tZMin>tMax)
	{
		return false;                         
	}
    
	if(tZMin>tMin)
	{
		tMin = tZMin;                      
	}
    
	if(tZMax<tMax)
	{
		tMax = tZMax;             
	}
    
	if(tMax<0)
	{
		return false;    
	}
					
				 
	//(*pLenght) = sqrt((tMin * xDir) * (tMin * xDir) + (tMin * yDir) * (tMin * yDir) + (tMin * zDir) * (tMin * zDir));

	return true;		 
}

void AABB::move(float x, float y, float z)
{
	mMinX += x;
	mMaxX += x;
	mMinY += y;
	mMaxY += y;
	mMinZ += z;
	mMaxZ += z;

	updateCorners();
}

void AABB::setPosition(const D3DXVECTOR3 &position)
{
	mMinX = position.x - (mMaxX - mMinX)/2;
	mMinY = position.y - (mMaxY - mMinY)/2;
	mMinZ = position.z - (mMaxZ - mMinZ)/2;
	mMaxX = position.x + (mMaxX - mMinX)/2;
	mMaxY = position.y + (mMaxY - mMinY)/2;
	mMaxZ = position.z + (mMaxZ - mMinZ)/2;

	updateCorners();
}

void AABB::scale(const D3DXVECTOR3 &scale)
{
	D3DXVECTOR3 center = D3DXVECTOR3((mMinX + mMaxX)/2,(mMinY + mMaxY)/2, (mMinZ + mMaxZ)/2);

	D3DXVECTOR3 size = D3DXVECTOR3(0,0,0);

	size.x = mBaseSize.x * scale.x;
	size.y = mBaseSize.y * scale.y;
	size.z = mBaseSize.z * scale.z;

	D3DXVECTOR3 min = center - size;
	D3DXVECTOR3 max = center + size;

	mMinX = min.x;
	mMinY = min.y;
	mMinZ = min.z;
	mMaxX = max.x;
	mMaxY = max.y;
	mMaxZ = max.z;

	updateCorners();
}

float AABB::getMinX() const
{
      return mMinX;    
}

float AABB::getMinY() const
{
      return mMinY;      
}

float AABB::getMinZ() const
{
      return mMinZ;
}

float AABB::getMaxX() const
{
      return mMaxX;
}

float AABB::getMaxY() const
{
      return mMaxY;
}

float AABB::getMaxZ() const
{
      return mMaxZ;
}

void AABB::updateCorners()
{
	mCenter = D3DXVECTOR3((mMinX + mMaxX) / 2, (mMinY + mMaxY) / 2, (mMinZ + mMaxZ) / 2);
	mCorners[0] = D3DXVECTOR3(mMinX, mMinY, mMinZ);
	mCorners[1] = D3DXVECTOR3(mMinX, mMinY, mMaxZ);
	mCorners[2] = D3DXVECTOR3(mMinX, mMaxY, mMinZ);
	mCorners[3] = D3DXVECTOR3(mMinX, mMaxY, mMaxZ);
	mCorners[4] = D3DXVECTOR3(mMaxX, mMinY, mMinZ);
	mCorners[5] = D3DXVECTOR3(mMaxX, mMinY, mMaxZ);
	mCorners[6] = D3DXVECTOR3(mMaxX, mMaxY, mMinZ);
	mCorners[7] = D3DXVECTOR3(mMaxX, mMaxY, mMaxZ);
}