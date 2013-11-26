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

bool Frustum::collision(const CollisionObject * pCollisionObject) const
{
	if(pCollisionObject->getClassName()=="Sphere")
	{
		Sphere * pSphere = (Sphere*) pCollisionObject;

		float x = pSphere->getX();
		float y = pSphere->getY();
		float z = pSphere->getZ();
		float r = pSphere->getR();
		
		for(int i=0;i<6;i++)
		{
			if(mPlanes[i].x * x + mPlanes[i].y * y + mPlanes[i].z * z + mPlanes[i].w <= -r)
			{
				return false;
			}
		}
		
		return true;
	}
	else if(pCollisionObject->getClassName()=="AABB")
	{
		AABB * pAABB = (AABB*) pCollisionObject;

		D3DXVECTOR3 aabbCorners[8];

		aabbCorners[0] = D3DXVECTOR3(pAABB->getMinX(),pAABB->getMinY(),pAABB->getMinZ());
		aabbCorners[1] = D3DXVECTOR3(pAABB->getMinX(),pAABB->getMinY(),pAABB->getMaxZ());
		aabbCorners[2] = D3DXVECTOR3(pAABB->getMinX(),pAABB->getMaxY(),pAABB->getMinZ());
		aabbCorners[3] = D3DXVECTOR3(pAABB->getMinX(),pAABB->getMaxY(),pAABB->getMaxZ());
		aabbCorners[4] = D3DXVECTOR3(pAABB->getMaxX(),pAABB->getMinY(),pAABB->getMinZ());
		aabbCorners[5] = D3DXVECTOR3(pAABB->getMaxX(),pAABB->getMinY(),pAABB->getMaxZ());
		aabbCorners[6] = D3DXVECTOR3(pAABB->getMaxX(),pAABB->getMaxY(),pAABB->getMinZ());
		aabbCorners[7] = D3DXVECTOR3(pAABB->getMaxX(),pAABB->getMaxY(),pAABB->getMaxZ());

		for(int j=0;j<6;j++)
		{
			int cornersInside = 8;

			for(int i=0;i<8;i++)
			{
				if(mPlanes[j].x * aabbCorners[i].x + mPlanes[j].y * aabbCorners[i].y + mPlanes[j].z * aabbCorners[i].z + mPlanes[j].w <= 0)
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

void Frustum::update(D3DXMATRIX &view, D3DXMATRIX &projection)
{
	D3DXMATRIX matrix;

	D3DXMatrixMultiply(&matrix,&view,&projection);
	
	mPlanes[0].x = matrix._14 - matrix._11;
	mPlanes[0].y = matrix._24 - matrix._21;
	mPlanes[0].z = matrix._34 - matrix._31;
	mPlanes[0].w = matrix._44 - matrix._41;

	mPlanes[1].x = matrix._14 + matrix._11;
	mPlanes[1].y = matrix._24 + matrix._21;
	mPlanes[1].z = matrix._34 + matrix._31;
	mPlanes[1].w = matrix._44 + matrix._41;

	mPlanes[2].x = matrix._14 - matrix._12;
	mPlanes[2].y = matrix._24 - matrix._22;
	mPlanes[2].z = matrix._34 - matrix._32;
	mPlanes[2].w = matrix._44 - matrix._42;

	mPlanes[3].x = matrix._14 + matrix._12;
	mPlanes[3].y = matrix._24 + matrix._22;
	mPlanes[3].z = matrix._34 + matrix._32;
	mPlanes[3].w = matrix._44 + matrix._42;

	mPlanes[4].x = matrix._14 - matrix._13;
	mPlanes[4].y = matrix._24 - matrix._23;
	mPlanes[4].z = matrix._34 - matrix._33;
	mPlanes[4].w = matrix._44 - matrix._43;

	mPlanes[5].x = matrix._14 + matrix._13;
	mPlanes[5].y = matrix._24 + matrix._23;
	mPlanes[5].z = matrix._34 + matrix._33;
	mPlanes[5].w = matrix._44 + matrix._43;
}