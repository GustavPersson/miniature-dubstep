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

#include "OBB.H"

using namespace gfx;

OBB::OBB(float minX,float minY,float minZ,float maxX,float maxY,float maxZ,int id) : CollisionObject("OBB",id), mCenterOffset(0.0f, 0.0f, 0.0f)
{
	mBaseSize = D3DXVECTOR3((maxX - minX)/2, (maxY - minY)/2, (maxZ - minZ)/2);

	mCorners[0] = D3DXVECTOR3(minX,minY,minZ);
	mCorners[1] = D3DXVECTOR3(minX,minY,maxZ);
	mCorners[2] = D3DXVECTOR3(minX,maxY,minZ);
	mCorners[3] = D3DXVECTOR3(minX,maxY,maxZ);
	mCorners[4] = D3DXVECTOR3(maxX,minY,minZ);
	mCorners[5] = D3DXVECTOR3(maxX,minY,maxZ);
	mCorners[6] = D3DXVECTOR3(maxX,maxY,minZ);
	mCorners[7] = D3DXVECTOR3(maxX,maxY,maxZ);
	
	mAxis[0] = D3DXVECTOR3(1,0,0);
	mAxis[1] = D3DXVECTOR3(0,1,0);
	mAxis[2] = D3DXVECTOR3(0,0,1);

	mCenter = D3DXVECTOR3((minX+maxX)/2,(minY+maxY)/2,(minZ+maxZ)/2);
	mCenterOffset = mCenter;

	mSizeX = (maxX - minX)/2;
	mSizeY = (maxY - minY)/2;
	mSizeZ = (maxZ - minZ)/2;

	mRotX = 0;
	mRotY = 0;
	mRotZ = 0;

	D3DXMatrixIdentity(&mRotXM);
	D3DXMatrixIdentity(&mRotYM);
	D3DXMatrixIdentity(&mRotZM);
}

CollisionObject *OBB::clone()
{
	return myNew OBB(*this);
}

bool OBB::collision(const CollisionObject * pCollisionObject, bool calculateNormal)
{
	if(pCollisionObject->getCollisionType()==CollisionType_OBB)
	{
		OBB * pOBB = (OBB*) pCollisionObject;
		
		for(unsigned int aIndex=0; aIndex<3; aIndex++)
	    {
			  D3DXVECTOR3 axis = mAxis[aIndex];

              float min1 = 1000000.0f;
              float max1 = -1000000.0f;
              float min2 = 1000000.0f;
              float max2 = -1000000.0f;
              
              for(unsigned int cIndex=0; cIndex<8; cIndex++)
              {
				  float pos = D3DXVec3Dot(&axis, &mCorners[cIndex]);// axis.x * mCorners[cIndex].x + axis.y * mCorners[cIndex].y + axis.z * mCorners[cIndex].z;    
                     
                     if(pos < min1)
                     {
                          min1 = pos;  
                     }
                     else if(pos > max1)     
                     {
                          max1 = pos;
                     }                   
              }
              
              for(unsigned int cIndex=0; cIndex<8; cIndex++)
              {
					 D3DXVECTOR3 corner = pOBB->getCorner(cIndex);

                     float pos = D3DXVec3Dot(&axis, &corner);//axis.x * corner.x + axis.y * corner.y  + axis.z * corner.z;    
                     
                     if(pos < min2)
                     {
                          min2 = pos;  
                     }
                     else if(pos > max2)     
                     {
                          max2 = pos;
                     }                   
              }
              
              if(max1 < min2 ||  min1 > max2)
              {
                     return false;       
              }          
        }

		for(unsigned int aIndex=0; aIndex<3; aIndex++)
	    {
			  D3DXVECTOR3 axis = pOBB->getAxis(aIndex);

              float min1 = 1000000.0f;
              float max1 = -1000000.0f;
              float min2 = 1000000.0f;
              float max2 = -1000000.0f;
              
              for(unsigned int cIndex=0; cIndex<8; cIndex++)
              {
                     float pos = D3DXVec3Dot(&axis, &mCorners[cIndex]);//axis.x * mCorners[cIndex].x + axis.y * mCorners[cIndex].y + axis.z * mCorners[cIndex].z;    
                     
                     if(pos < min1)
                     {
                          min1 = pos;  
                     }
                     else if(pos > max1)     
                     {
                          max1 = pos;
                     }                   
              }
              
              for(unsigned int cIndex=0; cIndex<8; cIndex++)
              {
					 D3DXVECTOR3 corner = pOBB->getCorner(cIndex);

                     float pos = D3DXVec3Dot(&axis, &corner);//axis.x * corner.x + axis.y * corner.y  + axis.z * corner.z;    
                     
                     if(pos < min2)
                     {
                          min2 = pos;  
                     }
                     else if(pos > max2)     
                     {
                          max2 = pos;
                     }                   
              }
              
              if(max1 < min2 ||  min1 > max2)
              {
                     return false;       
              }          
        }
        
		if(calculateNormal)
		{
			setCollisionNormal(pOBB->getCenter() - mCenter);
			pOBB->setCollisionNormal(mCenter - pOBB->getCenter());
		}

        return true; 

	}
	else if(pCollisionObject->getCollisionType()==CollisionType_AABB)
	{
		AABB * pAABB = (AABB*) pCollisionObject;
		
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
					 //D3DXVECTOR3 corner = pAABB->getCorner(cIndex);

                     //float pos = mAxis[aIndex].x * mCorners[cIndex].x + mAxis[aIndex].y * mCorners[cIndex].y + mAxis[aIndex].z * mCorners[cIndex].z;
					 float pos = D3DXVec3Dot(&mAxis[aIndex], &mCorners[cIndex]);
                     
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
					 D3DXVECTOR3 corner = pAABB->getCorner(cIndex);

                     //float pos = mAxis[aIndex].x * aabbCorners[cIndex].x + mAxis[aIndex].y * aabbCorners[cIndex].y  + mAxis[aIndex].z * aabbCorners[cIndex].z;
				     float pos = D3DXVec3Dot(&mAxis[aIndex], &corner);
                     
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
              float min1 = 1000000;
              float max1 = -1000000;
              float min2 = 1000000;
              float max2 = -1000000;
            
              for(int cIndex=0;cIndex<8;cIndex++)
              {
					 //D3DXVECTOR3 corner = pAABB->getCorner(cIndex);

                     //float pos = aabbAxis[aIndex].x * mCorners[cIndex].x + aabbAxis[aIndex].y * mCorners[cIndex].y + aabbAxis[aIndex].z * mCorners[cIndex].z;
					 float pos = D3DXVec3Dot(&aabbAxis[aIndex], &mCorners[cIndex]);
                     
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
					 D3DXVECTOR3 corner = pAABB->getCorner(cIndex);

                     //float pos = aabbAxis[aIndex].x * aabbCorners[cIndex].x + aabbAxis[aIndex].y * aabbCorners[cIndex].y  + aabbAxis[aIndex].z * aabbCorners[cIndex].z;  
				     float pos = D3DXVec3Dot(&aabbAxis[aIndex], &corner);
                     
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
			setCollisionNormal(pAABB->getCenter() - mCenter);
			pAABB->setCollisionNormal(mCenter - pAABB->getCenter());
		}
        
        return true;

	}
	else if(pCollisionObject->getCollisionType()==CollisionType_Sphere)
	{
		Sphere * pSphere = (Sphere*) pCollisionObject;

		for(int aIndex=0;aIndex<3;aIndex++)
	    {
              float min1 = 1000000;
			  float max1 = -1000000;
			  float min2 = 1000000;
              float max2 = -1000000;
              
              for(int cIndex=0;cIndex<8;cIndex++)
              {
                     float pos = mAxis[aIndex].x * mCorners[cIndex].x + mAxis[aIndex].y * mCorners[cIndex].y + mAxis[aIndex].z * mCorners[cIndex].z;    
                     
                     if(pos < min1)
                     {
                          min1 = pos;  
                     }
                     else if(pos > max1)     
                     {
                          max1 = pos;
                     }
              }
              
			  min2 = mAxis[aIndex].x * pSphere->getX() + mAxis[aIndex].y * pSphere->getY() + mAxis[aIndex].z * pSphere->getZ() - pSphere->getR();
			  max2 = mAxis[aIndex].x * pSphere->getX() + mAxis[aIndex].y * pSphere->getY() + mAxis[aIndex].z * pSphere->getZ() + pSphere->getR();
              
              if(max1 < min2 ||  min1 > max2)
              {
                     return false;       
              }          
        }
		
		D3DXVECTOR3 obbToSphere = D3DXVECTOR3(pSphere->getX(),pSphere->getY(),pSphere->getZ());
		obbToSphere -= mCenter;

		float length = obbToSphere.x * obbToSphere.x + obbToSphere.y * obbToSphere.y + obbToSphere.z * obbToSphere.z;
		length = sqrt(length);

		obbToSphere /= length;

		float min1 = 1000000;
		float max1 = -1000000;
		float min2 = 1000000;
        float max2 = -1000000;

		for(int cIndex=0;cIndex<8;cIndex++)
        {
             float pos = obbToSphere.x * mCorners[cIndex].x + obbToSphere.y * mCorners[cIndex].y + obbToSphere.z * mCorners[cIndex].z;    
                     
             if(pos < min1)
             {
                   min1 = pos;  
             }
             else if(pos > max1)     
             {
                   max1 = pos;
             }
        }
              
	    min2 = obbToSphere.x * pSphere->getX() + obbToSphere.y * pSphere->getY() + obbToSphere.z * pSphere->getZ() - pSphere->getR();
	    max2 = obbToSphere.x * pSphere->getX() + obbToSphere.y * pSphere->getY() + obbToSphere.z * pSphere->getZ() + pSphere->getR();
              
        if(max1 < min2 ||  min1 > max2)
        {
              return false;       
        }

		if(calculateNormal)
		{
			D3DXVECTOR3 sphereCenter = D3DXVECTOR3(pSphere->getX(), pSphere->getY(), pSphere->getZ());

			setCollisionNormal(sphereCenter - mCenter);
			pSphere->setCollisionNormal(mCenter - sphereCenter);
		}
        
        return true;
	}

	return false;
}

bool OBB::intersection(const D3DXVECTOR3 &start, const D3DXVECTOR3 &direction)
{
	D3DXVECTOR3 d;
	D3DXVec3Normalize(&d, &direction);

	float tMin = -1000000;
	float tMax = 1000000;

	D3DXVECTOR3 p = mCenter - start;

	float h[3];
	h[0] = mSizeX;
	h[1] = mSizeY;
	h[2] = mSizeZ;

	for(int i=0;i<3;i++)
	{
		float e = mAxis[i].x * p.x + mAxis[i].y * p.y + mAxis[i].z * p.z;
		float f = mAxis[i].x * d.x + mAxis[i].y * d.y + mAxis[i].z * d.z;

		if(abs(f)>0)
		{
			float t1 = (e + h[i])/f;
			float t2 = (e - h[i])/f;

			if(t1>t2)
			{
				float tmp = t1;
				t1 = t2;
				t2 = tmp;
			}

			if(t1>tMin)
			{
				tMin = t1;
			}

			if(t2<tMax)
			{
				tMax = t2;
			}

			if(tMin>tMax)
			{
				return false;
			}
			else if(tMax<0)
			{
				return false;
			}
		}
		else if((-e - h[i] > 0)||(-e + h[i] < 0))
		{
			return false;
		}
	}
	
	D3DXVec3Scale(&d, &d, tMin);

	D3DXVec3Add(&mIntersectionPoint, &start, &p);

	return true;
}

void OBB::move(float x,float y,float z)
{
	D3DXVECTOR3 moveVector = D3DXVECTOR3(x,y,z);

	mCenter += moveVector;

	mCorners[0] += moveVector;
	mCorners[1] += moveVector;
	mCorners[2] += moveVector;
	mCorners[3] += moveVector;
	mCorners[4] += moveVector;
	mCorners[5] += moveVector;
	mCorners[6] += moveVector;
	mCorners[7] += moveVector;
}

void OBB::setPosition(const D3DXVECTOR3 &position)
{
	mCorners[0] = mCorners[0] - mCenter + position + mCenterOffset;
	mCorners[1] = mCorners[1] - mCenter + position + mCenterOffset;
	mCorners[2] = mCorners[2] - mCenter + position + mCenterOffset;
	mCorners[3] = mCorners[3] - mCenter + position + mCenterOffset;
	mCorners[4] = mCorners[4] - mCenter + position + mCenterOffset;
	mCorners[5] = mCorners[5] - mCenter + position + mCenterOffset;
	mCorners[6] = mCorners[6] - mCenter + position + mCenterOffset;
	mCorners[7] = mCorners[7] - mCenter + position + mCenterOffset;

	mCenter = position + mCenterOffset;
}

void OBB::scale(const D3DXVECTOR3 &scale)
{
	mSizeX = mBaseSize.x * scale.x;
	mSizeY = mBaseSize.y * scale.y;
	mSizeZ = mBaseSize.z * scale.z;

	mCorners[0] = mCenter - mAxis[0] * mSizeX - mAxis[1] * mSizeY - mAxis[2] * mSizeZ;
	mCorners[1] = mCenter - mAxis[0] * mSizeX - mAxis[1] * mSizeY + mAxis[2] * mSizeZ;
	mCorners[2] = mCenter - mAxis[0] * mSizeX + mAxis[1] * mSizeY - mAxis[2] * mSizeZ;
	mCorners[3] = mCenter - mAxis[0] * mSizeX + mAxis[1] * mSizeY + mAxis[2] * mSizeZ;
	mCorners[4] = mCenter + mAxis[0] * mSizeX - mAxis[1] * mSizeY - mAxis[2] * mSizeZ;
	mCorners[5] = mCenter + mAxis[0] * mSizeX - mAxis[1] * mSizeY + mAxis[2] * mSizeZ;
	mCorners[6] = mCenter + mAxis[0] * mSizeX + mAxis[1] * mSizeY - mAxis[2] * mSizeZ;
	mCorners[7] = mCenter + mAxis[0] * mSizeX + mAxis[1] * mSizeY + mAxis[2] * mSizeZ;
}

const D3DXVECTOR3 &OBB::getCorner(int index) const
{
	return mCorners[index];
}

const D3DXVECTOR3 &OBB::getAxis(int index) const
{
	return mAxis[index];
}

const D3DXVECTOR3 &OBB::getCenter() const
{
	return mCenter;
}

void OBB::update()
{
	D3DXMATRIX r = mRotXM * mRotYM * mRotZM;
	
	mAxis[0] = D3DXVECTOR3(1,0,0);
	mAxis[1] = D3DXVECTOR3(0,1,0);
	mAxis[2] = D3DXVECTOR3(0,0,1);

	D3DXVec3TransformCoord(&mAxis[0], &mAxis[0], &r);
	D3DXVec3TransformCoord(&mAxis[1], &mAxis[1], &r);
	D3DXVec3TransformCoord(&mAxis[2], &mAxis[2], &r);

	mCorners[0] = mCenter - mAxis[0] * mSizeX - mAxis[1] * mSizeY - mAxis[2] * mSizeZ;
	mCorners[1] = mCenter - mAxis[0] * mSizeX - mAxis[1] * mSizeY + mAxis[2] * mSizeZ;
	mCorners[2] = mCenter - mAxis[0] * mSizeX + mAxis[1] * mSizeY - mAxis[2] * mSizeZ;
	mCorners[3] = mCenter - mAxis[0] * mSizeX + mAxis[1] * mSizeY + mAxis[2] * mSizeZ;
	mCorners[4] = mCenter + mAxis[0] * mSizeX - mAxis[1] * mSizeY - mAxis[2] * mSizeZ;
	mCorners[5] = mCenter + mAxis[0] * mSizeX - mAxis[1] * mSizeY + mAxis[2] * mSizeZ;
	mCorners[6] = mCenter + mAxis[0] * mSizeX + mAxis[1] * mSizeY - mAxis[2] * mSizeZ;
	mCorners[7] = mCenter + mAxis[0] * mSizeX + mAxis[1] * mSizeY + mAxis[2] * mSizeZ;
}

void OBB::rotateX(float angle)
{
	mRotX += angle;
	D3DXMatrixRotationX(&mRotXM, mRotX);
	update();
}

void OBB::rotateY(float angle)
{
	mRotY += angle;
	D3DXMatrixRotationY(&mRotYM, mRotY);
	update();
}

void OBB::rotateZ(float angle)
{
	mRotZ += angle;
	D3DXMatrixRotationZ(&mRotZM, mRotZ);
	update();
}

void OBB::setCollisionNormal(D3DXVECTOR3 collisionNormal)
{	
	if(abs(D3DXVec3Dot(&collisionNormal, &mAxis[0])) > abs(D3DXVec3Dot(&collisionNormal, &mAxis[2])))
	{
		if(D3DXVec3Dot(&collisionNormal, &mAxis[0]) > 0)
		{
			mCollisionNormal = mAxis[0];
		}
		else
		{
			mCollisionNormal = -mAxis[0];
		}
	}
	else
	{
		if(D3DXVec3Dot(&collisionNormal, &mAxis[2]) > 0)
		{
			mCollisionNormal = mAxis[2];
		}
		else
		{
			mCollisionNormal = -mAxis[2];
		}
	}

	D3DXVec3Normalize(&collisionNormal, &collisionNormal);
	mCenterToCenterVectorNormalized = collisionNormal;
}

float OBB::getDistance(const D3DXVECTOR3 &point)
{
	float distance = 0.0f;
	D3DXVECTOR3 distanceVector = mCenter - point;
	distance = D3DXVec3Length(&distanceVector);
	return distance;
}