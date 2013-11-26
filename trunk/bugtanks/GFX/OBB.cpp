/**
 * @file
 * @author Carl Nordlander <cand05@student.bth.se>, Victor Lundberg <lundberg.victor@gmail.com>
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
	mBaseSize = D3DXVECTOR3((maxX - minX) / 2, (maxY - minY) / 2, (maxZ - minZ) / 2);

	mCorners[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	mCorners[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	mCorners[2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	mCorners[3] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	mCorners[4] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	mCorners[5] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	mCorners[6] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	mCorners[7] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	mAxis[0] = D3DXVECTOR3(1,0,0);
	mAxis[1] = D3DXVECTOR3(0,1,0);
	mAxis[2] = D3DXVECTOR3(0,0,1);

	mAngle = 0.0f;
	mOrigin[0] = 0.0f;
	mOrigin[1] = 0.0f;

	mCenter = D3DXVECTOR3((minX + maxX) / 2, (minY + maxY) / 2, (minZ + maxZ) / 2);
	mCenterOffset = D3DXVECTOR3(0.0f, (minY + maxY) / 2, 0.0f);

	mSizeX = (maxX - minX) / 2;
	mSizeY = (maxY - minY) / 2;
	mSizeZ = (maxZ - minZ) / 2;

	mRotX = 0;
	mRotY = 0;
	mRotZ = 0;

	D3DXMatrixIdentity(&mRotXM);
	D3DXMatrixIdentity(&mRotYM);
	D3DXMatrixIdentity(&mRotZM);

	update();
}

CollisionObject *OBB::clone()
{
	return myNew OBB(*this);
}

bool OBB::collision(const CollisionObject * pCollisionObject, bool calculateNormal)
{
	//OBB - OBB. 2D check only.
	if(pCollisionObject->getCollisionType() == CollisionType_OBB)
	{
		OBB *pOBB = (OBB *)pCollisionObject;

		for(int nAxis = 0; nAxis < 2; ++nAxis)
		{
			float projection = D3DXVec3Dot(&mCorners[0], &pOBB->getCornerAxis(nAxis));

			float min = projection;
			float max = projection;

			for(int nCorner = 1; nCorner < 4; ++nCorner)
			{
				projection = D3DXVec3Dot(&mCorners[nCorner], &pOBB->getCornerAxis(nAxis));

				if(projection < min)
				{
					min = projection;
				}
				else if(projection > max)
				{
					max = projection;
				}
			}

			if(min > 1.0f + pOBB->getOrigin(nAxis) || max < pOBB->getOrigin(nAxis))
			{
				return false;
			}
		}

		for(int nAxis = 0; nAxis < 2; ++nAxis)
		{
			float projection = D3DXVec3Dot(&pOBB->getCorner(0), &mCornerAxis[nAxis]);

			float min = projection;
			float max = projection;

			for(int nCorner = 1; nCorner < 4; ++nCorner)
			{
				projection = D3DXVec3Dot(&pOBB->getCorner(nCorner), &mCornerAxis[nAxis]);

				if(projection < min)
				{
					min = projection;
				}
				else if(projection > max)
				{
					max = projection;
				}
			}

			if(min > 1.0f + mOrigin[nAxis] || max < mOrigin[nAxis])
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
	//OBB - AABB.
	else if(pCollisionObject->getCollisionType() == CollisionType_AABB)
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
	//OBB - Sphere.
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

	float tMin = -FLT_MAX;
	float tMax = FLT_MAX;

	D3DXVECTOR3 p = mCenter - start;

	float h[3];
	h[0] = mSizeX;
	h[1] = mSizeY;
	h[2] = mSizeZ;

	for(int i = 0; i < 3; ++i)
	{
		float e = mAxis[i].x * p.x + mAxis[i].y * p.y + mAxis[i].z * p.z;
		float f = mAxis[i].x * d.x + mAxis[i].y * d.y + mAxis[i].z * d.z;

		if(abs(f) > 0)
		{
			float t1 = (e + h[i]) / f;
			float t2 = (e - h[i]) / f;

			if(t1 > t2)
			{
				float tmp = t1;
				t1 = t2;
				t2 = tmp;
			}

			if(t1 > tMin)
			{
				tMin = t1;
			}

			if(t2 < tMax)
			{
				tMax = t2;
			}

			if(tMin > tMax)
			{
				return false;
			}
			else if(tMax < 0)
			{
				return false;
			}
		}
		else if((-e - h[i] > 0) || (-e + h[i] < 0))
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
	D3DXVECTOR3 moveVector(x, 0.0f, z);
	mCenter += moveVector;
	update();
}

void OBB::setPosition(const D3DXVECTOR3 &position)
{
	mCenter = position;
	mCenter.y = 0.0f;
	mCenter += mCenterOffset;
	update();
}

void OBB::scale(const D3DXVECTOR3 &scale)
{
	mSizeX = mBaseSize.x * scale.x;
	mSizeY = mBaseSize.y * scale.y;
	mSizeZ = mBaseSize.z * scale.z;
	update();
}

void OBB::update()
{
	//Calculate axes.
	D3DXMATRIX r = mRotXM * mRotYM * mRotZM;
	
	mAxis[0] = D3DXVECTOR3(1,0,0);
	mAxis[1] = D3DXVECTOR3(0,1,0);
	mAxis[2] = D3DXVECTOR3(0,0,1);

	D3DXVec3TransformNormal(&mAxis[0], &mAxis[0], &r);
	D3DXVec3TransformNormal(&mAxis[1], &mAxis[1], &r);
	D3DXVec3TransformNormal(&mAxis[2], &mAxis[2], &r);

	D3DXVec3Normalize(&mAxis[0], &mAxis[0]);
	D3DXVec3Normalize(&mAxis[1], &mAxis[1]);
	D3DXVec3Normalize(&mAxis[2], &mAxis[2]);

	//Calculate corners.
	D3DXVECTOR3 vX(cos(mAngle), 0.0f, sin(mAngle));
	D3DXVECTOR3 vZ(-sin(mAngle), 0.0f, cos(mAngle));

	mCorners[0] = mCenter - vX * mSizeX - vZ * mSizeZ;
	mCorners[0].y = 0.0f;
	mCorners[1] = mCenter + vX * mSizeX - vZ * mSizeZ;
	mCorners[1].y = 0.0f;
	mCorners[2] = mCenter - vX * mSizeX + vZ * mSizeZ;
	mCorners[2].y = 0.0f;
	mCorners[3] = mCenter + vX * mSizeX + vZ * mSizeZ;
	mCorners[3].y = 0.0f;

	//Calculate corner axes.
	mCornerAxis[0] = mCorners[1] - mCorners[0];
	mCornerAxis[1] = mCorners[2] - mCorners[0];
	float lengthX = D3DXVec3Length(&mCornerAxis[0]);
	float lengthZ = D3DXVec3Length(&mCornerAxis[1]);
	D3DXVec3Normalize(&mCornerAxis[0], &mCornerAxis[0]);
	D3DXVec3Normalize(&mCornerAxis[1], &mCornerAxis[1]);
	mCornerAxis[0] /= lengthX;
	mCornerAxis[1] /= lengthZ;

	mOrigin[0] = D3DXVec3Dot(&mCorners[0], &mCornerAxis[0]);
	mOrigin[1] = D3DXVec3Dot(&mCorners[0], &mCornerAxis[1]);
}

void OBB::rotateX(float angle)
{
	mRotX = angle;
	D3DXMatrixRotationX(&mRotXM, mRotX);
	update();
}

void OBB::rotateY(float angle)
{
	mRotY = angle;
	mAngle = angle;
	D3DXMatrixRotationY(&mRotYM, mRotY);
	update();
}

void OBB::rotateZ(float angle)
{
	mRotZ = angle;
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