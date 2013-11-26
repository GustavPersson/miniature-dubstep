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

#include "TransformObject.h"
#include "CollisionObject.h"

using namespace gfx;

TransformObject::TransformObject(ID3D10Device *pDevice, Effect *pEffect) : RenderObject(pDevice, pEffect),
				 mScale(1.0f, 1.0f, 1.0f), mRotation(0.0f, 0.0f, 0.0f), mMatrixTransform(), mMatrixTranslate(),
				 mMatrixScale(), mMatrixRotate(), mMaterial()
{
	setType(RenderTypes::TRANSFORM_OBJECT);
	D3DXMatrixIdentity(&mMatrixTransform);
	D3DXMatrixIdentity(&mMatrixTranslate);
	D3DXMatrixIdentity(&mMatrixScale);
	D3DXMatrixIdentity(&mMatrixRotate);
}

TransformObject::TransformObject(const TransformObject &object) : RenderObject(object),
				 mScale(object.mScale), mRotation(object.mRotation), mMatrixTransform(object.mMatrixTransform),
				 mMatrixTranslate(object.mMatrixTranslate), mMatrixScale(object.mMatrixScale),
				 mMatrixRotate(object.mMatrixRotate)
{
	setType(RenderTypes::TRANSFORM_OBJECT);
}

TransformObject::~TransformObject()
{

}

const TransformObject &TransformObject::operator=(const TransformObject &object)
{
	if(this != &object)
	{
		RenderObject::operator=(object);
		mScale = object.mScale;
		mRotation = object.mRotation;
		mMatrixTransform = object.mMatrixTransform;
		mMatrixTranslate = object.mMatrixTranslate;
		mMatrixScale = object.mMatrixScale;
		mMatrixRotate = object.mMatrixRotate;
	}
	return *this;
}

RenderObject *TransformObject::clone()
{
	return NULL;
}

void TransformObject::update(float deltaTime)
{
	RenderObject::update(deltaTime);
	if(getFlagMoved())
	{
		updateMatrixTranslate();
		updateMatrixScale();
		updateMatrixRotate();
		updateMatrixTransform();

		if(getBoundingVolume())
		{
			//getBoundingVolume()->rotateX(mRotation.x);
			getBoundingVolume()->rotateY(mRotation.y);
			//getBoundingVolume()->rotateZ(mRotation.z);
			getBoundingVolume()->scale(mScale);
		}
	}
}

bool TransformObject::render()
{
	return RenderObject::render();
}

void TransformObject::increaseScale(const D3DXVECTOR3 &offset)
{
	mScale += offset;
	setFlagMoved(true);
}

void TransformObject::increaseScaleX(float x)
{
	mScale.x += x;
	setFlagMoved(true);
}

void TransformObject::increaseScaleY(float y)
{
	mScale.y += y;
	setFlagMoved(true);
}

void TransformObject::increaseScaleZ(float z)
{
	mScale.z += z;
	setFlagMoved(true);
}

void TransformObject::increaseRotation(const D3DXVECTOR3 &offset)
{
	mRotation += offset;
	setFlagMoved(true);
}

void TransformObject::increaseRotationX(float x)
{
	mRotation.x += x;
	setFlagMoved(true);
}

void TransformObject::increaseRotationY(float y)
{
	mRotation.y += y;
	setFlagMoved(true);
}

void TransformObject::increaseRotationZ(float z)
{
	mRotation.z += z;
	setFlagMoved(true);
}

void TransformObject::setScale(const D3DXVECTOR3 &scale)
{
	mScale = scale;
	setFlagMoved(true);
}

void TransformObject::setScaleX(float x)
{
	mScale.x = x;
	setFlagMoved(true);
}

void TransformObject::setScaleY(float y)
{
	mScale.y = y;
	setFlagMoved(true);
}

void TransformObject::setScaleZ(float z)
{
	mScale.z = z;
	setFlagMoved(true);
}

void TransformObject::setRotation(const D3DXVECTOR3 &rotation)
{
	mRotation = rotation;
	setFlagMoved(true);
}

void TransformObject::setRotationX(float x)
{
	mRotation.x = x;
	setFlagMoved(true);
}

void TransformObject::setRotationY(float y)
{
	mRotation.y = y;
	setFlagMoved(true);
}

void TransformObject::setRotationZ(float z)
{
	mRotation.z = z;
	setFlagMoved(true);
}

void TransformObject::updateMatrixTransform()
{
	mMatrixTransform = mMatrixScale * mMatrixRotate * mMatrixTranslate;
}

void TransformObject::updateMatrixTranslate()
{
	D3DXMatrixTranslation(&mMatrixTranslate, getPositionX(), getPositionY(), getPositionZ());
}

void TransformObject::updateMatrixScale()
{
	D3DXMatrixScaling(&mMatrixScale, mScale.x, mScale.y, mScale.z);
}

void TransformObject::updateMatrixRotate()
{
	D3DXMatrixRotationYawPitchRoll(&mMatrixRotate, mRotation.y, mRotation.x, mRotation.z);
}