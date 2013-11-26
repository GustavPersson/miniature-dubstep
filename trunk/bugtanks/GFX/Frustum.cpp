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

#include "Frustum.h"

using namespace gfx;

Frustum::Frustum() : CollisionObject("Frustum",0)
{

}

bool Frustum::collision(const CollisionObject * pCollisionObject, bool calculateNormal)
{
	calculateNormal;

	if(pCollisionObject->getCollisionType()==CollisionType_Sphere)
	{
		Sphere * pSphere = (Sphere*) pCollisionObject;

		float x = pSphere->getX();
		float y = pSphere->getY();
		float z = pSphere->getZ();
		float r = pSphere->getR();
		
		for(int i=0;i<6;i++)
		{
			if(mPlanes[i].a * x + mPlanes[i].b * y + mPlanes[i].c * z + mPlanes[i].d <= -r)
			{
				return false;
			}
		}
		
		return true;
	}
	else if(pCollisionObject->getCollisionType()==CollisionType_AABB)
	{
		AABB * pAABB = (AABB*) pCollisionObject;
		
		for(unsigned int j=0;j<6;j++)
		{
			unsigned int cornersInside = 8;

			for(unsigned int i=0;i<8;i++)
			{
				D3DXVECTOR3 corner = pAABB->getCorner(i);

				/*
				if(mPlanes[j].a * corner.x + mPlanes[j].b * corner.y + mPlanes[j].c * corner.z + mPlanes[j].d <= 0)
				{
					cornersInside--;
				}
				*/

				if(D3DXPlaneDotNormal(&mPlanes[j], &corner) +  mPlanes[j].d <= 0)
				{
					cornersInside--;
				}
			}

			if(cornersInside==0)
			{
				return false;
			}
		}
		
		return true;
	}
	if(pCollisionObject->getCollisionType()==CollisionType_OBB)
	{
		OBB * pOBB = (OBB*)pCollisionObject;

		for(unsigned int j=0;j<6;j++)
		{
			unsigned int cornersInside = 8;

			for(unsigned int i=0; i<8; i++)
			{
				D3DXVECTOR3 corner = pOBB->getCorner(i);

				if(D3DXPlaneDotNormal(&mPlanes[j], &corner) +  mPlanes[j].d <= 0)
				{
					cornersInside--;
				}				
			}

			if(cornersInside==0)
			{
				return false;
			}
		}
		
		return true;
	}

	return false;
}

void Frustum::update(const D3DXMATRIX &view, const D3DXMATRIX &projection)
{
	D3DXMATRIX matrix;
	D3DXMatrixMultiply(&matrix,&view,&projection);

	float offset = 100.0f;

	//Right.
	mPlanes[0].a = (matrix._14 - matrix._11);
	mPlanes[0].b = (matrix._24 - matrix._21);
	mPlanes[0].c = (matrix._34 - matrix._31);
	mPlanes[0].d = (matrix._44 - matrix._41) + offset;

	//D3DXPlaneNormalize(&mPlanes[0], &mPlanes[0]);

	//Left.
	mPlanes[1].a = (matrix._14 + matrix._11);
	mPlanes[1].b = (matrix._24 + matrix._21);
	mPlanes[1].c = (matrix._34 + matrix._31);
	mPlanes[1].d = (matrix._44 + matrix._41) + offset;

	//D3DXPlaneNormalize(&mPlanes[1], &mPlanes[1]);

	//Top.
	mPlanes[2].a = (matrix._14 - matrix._12);
	mPlanes[2].b = (matrix._24 - matrix._22);
	mPlanes[2].c = (matrix._34 - matrix._32);
	mPlanes[2].d = (matrix._44 - matrix._42) + offset;

	//D3DXPlaneNormalize(&mPlanes[2], &mPlanes[2]);

	//Bottom.
	mPlanes[3].a = (matrix._14 + matrix._12);
	mPlanes[3].b = (matrix._24 + matrix._22);
	mPlanes[3].c = (matrix._34 + matrix._32);
	mPlanes[3].d = (matrix._44 + matrix._42) + offset;

	//D3DXPlaneNormalize(&mPlanes[3], &mPlanes[3]);

	//Far.
	mPlanes[4].a = (matrix._14 - matrix._13);
	mPlanes[4].b = (matrix._24 - matrix._23);
	mPlanes[4].c = (matrix._34 - matrix._33);
	mPlanes[4].d = (matrix._44 - matrix._43);

	//D3DXPlaneNormalize(&mPlanes[4], &mPlanes[4]);

	//Near.
	mPlanes[5].a = matrix._13;
	mPlanes[5].b = matrix._23;
	mPlanes[5].c = matrix._33;
	mPlanes[5].d = matrix._43;

	//D3DXPlaneNormalize(&mPlanes[5], &mPlanes[5]);
}