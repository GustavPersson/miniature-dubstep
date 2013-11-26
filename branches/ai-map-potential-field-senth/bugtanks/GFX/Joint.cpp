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

#include "Joint.h"

using namespace gfx;
using namespace std;

Joint::Joint()
{
	mPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	mRotation = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f);
	mOrientation = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f);

	mToWorld = D3DXMATRIX();
	mToParent = D3DXMATRIX();
	mBindInverse = D3DXMATRIX();

	D3DXMatrixIdentity(&mToWorld);
	D3DXMatrixIdentity(&mToParent);
	D3DXMatrixIdentity(&mBindInverse);

	mChildren = vector<Joint*>();
}

Joint::~Joint()
{

}

void Joint::setPosition(const D3DXVECTOR3 &position)
{
	mPosition = position;
}

void Joint::setRotation(const D3DXQUATERNION &rotation)
{
	mRotation = rotation;
}

void Joint::setToParent(const D3DXMATRIX &toParent)
{
	mToParent = toParent;
}

void Joint::setToWorld(const D3DXMATRIX &toWorld)
{
	mToWorld = toWorld;
}

void Joint::setBindPose()
{
	D3DXMatrixInverse(&mBindInverse, NULL, &mToWorld);

	for(unsigned int i=0;i<mChildren.size();i++)
	{
		mChildren[i]->setBindPose();
	}
}

const D3DXVECTOR3 &Joint::getPosition() const
{
	return mPosition;
}

const D3DXQUATERNION &Joint::getRotation() const
{
	return mRotation;
}

const D3DXMATRIX &Joint::getToParent() const
{
	return mToParent;
}

const D3DXMATRIX &Joint::getToWorld() const
{
	return mToWorld;
}

const D3DXMATRIX &Joint::getBindInverse() const
{
	return mBindInverse;
}

void Joint::addChild(Joint * child)
{
	mChildren.push_back(child);
}

void Joint::update(bool root)
{
	D3DXMATRIX translationMatrix;
	D3DXMATRIX rotationMatrix;
	D3DXMATRIX orientationMatrix;
	D3DXMATRIX orientationMatrixInverse;
	D3DXQUATERNION rotation;

	D3DXQuaternionMultiply(&rotation, &mRotation, &mOrientation);
	D3DXMatrixRotationQuaternion(&rotationMatrix, &rotation);

	D3DXMatrixTranslation(&translationMatrix,mPosition.x,mPosition.y,mPosition.z);

	mToParent = rotationMatrix * translationMatrix;

	if(root)
	{
		mToWorld = mToParent;
	}
	else
	{
		mToWorld = mToParent * mToWorld;
	}

	for(unsigned int i=0;i<mChildren.size();i++)
	{
		mChildren[i]->setToWorld(mToWorld);
		mChildren[i]->update(false);
	}
}