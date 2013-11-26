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

#include "QuadTree.h"

using namespace gfx;
using namespace logic;
using namespace std;


QuadTree::QuadTree(float minX, float minZ, float maxX, float maxZ, int levels)
{
	mpAABB = myNew AABB(minX,-1000000.0f,minZ,maxX,1000000.0f,maxZ,0);

	float width = maxX - minX;
	float depth = maxZ - minZ;

	if(levels == 0)
	{
		mLeaf = true;
		return;
	}

	mLeaf = false;

	mpChildren[0] = myNew QuadTree(minX,minZ,minX + width/2,minZ + depth/2,levels - 1);
	mpChildren[1] = myNew QuadTree(minX + width/2,minZ,maxX,minZ + depth/2,levels - 1);
	mpChildren[2] = myNew QuadTree(minX,minZ + depth/2,minX + width/2,maxZ,levels - 1);
	mpChildren[3] = myNew QuadTree(minX + width/2,minZ + depth/2,maxX,maxZ,levels - 1);
}

QuadTree::~QuadTree()
{
	SAFE_DELETE(mpAABB);

	if(!mLeaf)
	{
		SAFE_DELETE(mpChildren[0]);
		SAFE_DELETE(mpChildren[1]);
		SAFE_DELETE(mpChildren[2]);
		SAFE_DELETE(mpChildren[3]);
	}
}

bool QuadTree::addCollisionObjectToNodes(CollisionObject * co)
{	
	if(!co->collision(mpAABB))
	{
		return false;
	}
	
	
	if(mLeaf)
	{
		mCollisionObjects.push_back(co);
	}
	else
	{
		mpChildren[0]->addCollisionObjectToNodes(co);
		mpChildren[1]->addCollisionObjectToNodes(co);
		mpChildren[2]->addCollisionObjectToNodes(co);
		mpChildren[3]->addCollisionObjectToNodes(co);
	}

	return true;
	
}

void QuadTree::update(std::vector<CollisionObject*> * pCollisionObjects)
{	
	if(mLeaf)
	{
		mCollisionObjects.clear();
	}
	else
	{
		mpChildren[0]->update(NULL);
		mpChildren[1]->update(NULL);
		mpChildren[2]->update(NULL);
		mpChildren[3]->update(NULL);
	}


	if(pCollisionObjects)
	{
		for(unsigned int i=0;i<pCollisionObjects->size();i++)
		{
			addCollisionObjectToNodes((*pCollisionObjects)[i]);
		}
	}
}

void QuadTree::intersection(float xStart, float yStart, float zStart, float xDir, float yDir, float zDir,float maxLength,float * lenght, KeyType actorID,vector<KeyType> * pOutput) const
{   
	if(mpAABB->intersection(xStart,yStart,zStart,xDir,yDir,zDir,lenght))
	{
		if(mLeaf)
		{		
			for(unsigned int i=0;i<mCollisionObjects.size();i++)
			{
				if(mCollisionObjects[i]->intersection(xStart,yStart,zStart,xDir,yDir,zDir,lenght))
				{
					if(mCollisionObjects[i]->getID()!=actorID)
					{
						pOutput->push_back(mCollisionObjects[i]->getID());
					}
				}
			}
		}
		else
		{
			mpChildren[0]->intersection(xStart,yStart,zStart,xDir,yDir,zDir,maxLength,lenght,actorID,pOutput);
			mpChildren[1]->intersection(xStart,yStart,zStart,xDir,yDir,zDir,maxLength,lenght,actorID,pOutput);
			mpChildren[2]->intersection(xStart,yStart,zStart,xDir,yDir,zDir,maxLength,lenght,actorID,pOutput);
			mpChildren[3]->intersection(xStart,yStart,zStart,xDir,yDir,zDir,maxLength,lenght,actorID,pOutput);
		}
	}
}

void QuadTree::collisionCheck(const CollisionObject* pCollisionObject, vector<CollisionObject*> * pOutput) const
{	
	if(pCollisionObject->collision(mpAABB))
	{
		if(mLeaf)
		{
			for(unsigned int i=0;i<mCollisionObjects.size();i++)
			{
				if(pCollisionObject!=mCollisionObjects[i])
				{
					if(pCollisionObject->collision(mCollisionObjects[i]))
					{
						pOutput->push_back(mCollisionObjects[i]);
					}
				}
			}
		}
		else
		{
			mpChildren[0]->collisionCheck(pCollisionObject,pOutput);
			mpChildren[1]->collisionCheck(pCollisionObject,pOutput);
			mpChildren[2]->collisionCheck(pCollisionObject,pOutput);
			mpChildren[3]->collisionCheck(pCollisionObject,pOutput);
		}

	}

}