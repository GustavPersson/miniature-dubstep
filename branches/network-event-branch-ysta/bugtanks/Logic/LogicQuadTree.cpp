/**
 * @file
 * @author Jonathan Udd <jonis.x@gmail.com>
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


#include "LogicQuadTree.h"
#include <vector>

using namespace logic;
using namespace gfx;
using std::vector;

LogicQuadTree* LogicQuadTree::mpsInstance = NULL;

LogicQuadTree::~LogicQuadTree()
{
	mpsInstance = NULL;
	deleteAllBVs();
}

LogicQuadTree* LogicQuadTree::getInstance()
{
	if(mpsInstance == NULL)
	{
		mpsInstance = myNew LogicQuadTree();
	}

	return mpsInstance;
}

void LogicQuadTree::collisionRay(const D3DXVECTOR3 &origin, const D3DXVECTOR3 &direction, KeyType actorID, KeyType bulletID)
{	
	bool mTrueCollision;
	std::vector<int> mCheckedIds;
	std::vector<KeyType> mIDs;
	float length = 0.0f;
	mQuadTree.intersection(origin.x, origin.y, origin.z, direction.x, direction.y,
						   direction.z, (float)INT_MAX, &length, actorID, &mIDs);
	
	
	for(unsigned int i = 0; i < mIDs.size(); i++)
	{
		mTrueCollision = true;
		if(mIDs.at(i))
		{
			if(mIDs.at(i) != -1)
			{
				for(unsigned int j = 0; j < mCheckedIds.size(); j++)
				{
					if(mCheckedIds[j] == mIDs[i])
					{
						mTrueCollision = false;
					}
				}
				if(mTrueCollision)
				{
					mCheckedIds.push_back(mIDs[i]);
					float time = logic::GlobalTimer::getInstance()->getTime(logic::Timer::ReturnType_MilliSeconds);
					safeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_From_ActorCollision(bulletID, mIDs.at(i),actorID, time)));
				}
			}
		}
	}
}

void LogicQuadTree::updateQuadTree()
{
	deleteAllBVs();
	DX10Module *module = DX10Module::getInstance();
	module->lock(0);

	module->copyBoundingVolumes(&mBVs);
	mQuadTree.update(&mBVs);

	module->unlock(0);
}

void LogicQuadTree::deleteAllBVs()
{
	for(vector<CollisionObject *>::iterator BVIter = mBVs.begin();
		BVIter != mBVs.end(); ++BVIter)
	{
		if((*BVIter))
		{
			delete (*BVIter);
			(*BVIter) = NULL;
		}
	}
	mBVs.clear();
}

LogicQuadTree::LogicQuadTree() : mBVs(), mQuadTree(-10000.0f, -10000.0f, 10000.0f, 10000.0f, 3)
{

}