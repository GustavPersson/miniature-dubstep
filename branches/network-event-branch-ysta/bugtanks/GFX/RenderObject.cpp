/**
 * @file
 * @author Victor Lundberg <lundberg.victor@gmail.com>
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

#include "RenderObject.h"

using namespace gfx;

RenderObject::RenderObject(ID3D10Device *pDevice, Effect *pEffect) : mPosition(0.0f, 0.0f, 0.0f), mColor(1.0f, 1.0f, 1.0f, 1.0f)
{
	mType = RenderTypes::RENDER_OBJECT;
	mID = -1;
	mName = "Unknown";
	mpDevice = pDevice;
	mpEffect = pEffect;
	mTextureID = -1;
	mFlagMoved = false;
	mFlagNewMoved = false;
	mFlagRender = true;
	mEffectTechIndex = 0;
	mpBoundingVolume = NULL;
}

RenderObject::RenderObject(const RenderObject &object) : mPosition(object.mPosition), mColor(object.mColor)
{
	mType = RenderTypes::RENDER_OBJECT;
	mID = object.mID;
	mName = object.mName;
	mpDevice = object.mpDevice;
	mpEffect = object.mpEffect;
	mTextureID = object.mTextureID;
	mFlagMoved = object.mFlagMoved;
	mFlagNewMoved = object.mFlagNewMoved;
	mFlagRender = object.mFlagRender;
	mEffectTechIndex = object.mEffectTechIndex;
	if(object.mpBoundingVolume)
	{
		mpBoundingVolume = object.mpBoundingVolume->clone();
		mpBoundingVolume->setParent((void *)this);
	}
	else
	{
		mpBoundingVolume = NULL;
	}
}

RenderObject::~RenderObject()
{
	if(mpBoundingVolume)
	{
		delete mpBoundingVolume;
		mpBoundingVolume = NULL;
	}
}

const RenderObject &RenderObject::operator=(const RenderObject &object)
{
	if(this != &object)
	{
		mID = object.mID;
		mName = object.mName;
		mPosition = object.mPosition;
		mColor = object.mColor;
		mpDevice = object.mpDevice;
		mpEffect = object.mpEffect;
		mTextureID = object.mTextureID;
		mFlagMoved = object.mFlagMoved;
		mFlagNewMoved = object.mFlagNewMoved;
		mFlagRender = object.mFlagRender;
		mEffectTechIndex = object.mEffectTechIndex;
		if(mpBoundingVolume)
		{
			delete mpBoundingVolume;
			mpBoundingVolume = NULL;
		}
		if(object.mpBoundingVolume)
		{
			mpBoundingVolume = object.mpBoundingVolume->clone();
		}
	}
	return *this;
}

RenderObject *RenderObject::clone()
{
	return NULL;
}

void RenderObject::update()
{
	mFlagMoved = mFlagNewMoved;
	mFlagNewMoved = false;
	if(mFlagMoved)
	{
		if(mpBoundingVolume)
		{
			mpBoundingVolume->setPosition(mPosition);
		}
	}
}

bool RenderObject::render()
{
	return mFlagRender;
}

void RenderObject::increasePosition(const D3DXVECTOR3 &offset)
{
	mPosition += offset;
	mFlagNewMoved = true;
}

void RenderObject::increasePositionX(float x)
{
	mPosition.x += x;
	mFlagNewMoved = true;
}

void RenderObject::increasePositionY(float y)
{
	mPosition.y += y;
	mFlagNewMoved = true;
}

void RenderObject::increasePositionZ(float z)
{
	mPosition.z += z;
	mFlagNewMoved = true;
}

void RenderObject::setID(int ID)
{
	mID = ID;
	if(mpBoundingVolume)
	{
		mpBoundingVolume->setID(ID);
	}
}

void RenderObject::setPosition(const D3DXVECTOR3 &position)
{
	mPosition = position;
	mFlagNewMoved = true;
}

void RenderObject::setPositionX(float x)
{
	mPosition.x = x;
	mFlagNewMoved = true;
}

void RenderObject::setPositionY(float y)
{
	mPosition.y = y;
	mFlagNewMoved = true;
}

void RenderObject::setPositionZ(float z)
{
	mPosition.z = z;
	mFlagNewMoved = true;
}

void RenderObject::setBoundingVolume(CollisionObject *pBV)
{
	if(mpBoundingVolume)
	{
		delete mpBoundingVolume;
		mpBoundingVolume = NULL;
	}
	mpBoundingVolume = pBV;
	mpBoundingVolume->setParent((void *)this);
}

CollisionObject *RenderObject::getBoundingVolume()
{
	return mpBoundingVolume;
}