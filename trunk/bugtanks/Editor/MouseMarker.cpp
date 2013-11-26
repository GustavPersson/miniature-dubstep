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

#include "MouseMarker.h"
#include "../Logic/LogicQuadTree.h"
#include "../Logic/Actor.h"

using namespace editor;
using namespace gfx;
using namespace logic;

using std::vector;

MouseMarker::MouseMarker() : mMouseRay(), mPlanePosition(), mObjectScale(), mObjectRotation()
{
	mMouseX = 0;
	mMouseY = 0;
	mMarkerState = MouseMarker::MarkerState_None;
	mMarkerType = MouseMarker::MarkerType_None;
	mFlagGrid = true;
	mGridSizeX = 10;
	mGridSizeZ = 10;
	mGridAngle = (float)(0.25 * D3DX_PI);
	mpObject = NULL;
	mSpeedScale = 0.0f;
	mSpeedRotate = 0.0f;
	mFlagScale = false;
	mFlagRotate = false;
	resetTransform();
}

MouseMarker::MouseMarker(const MouseMarker &marker) : mMouseRay(marker.mMouseRay),
	mPlanePosition(marker.mPlanePosition), mObjectScale(marker.mObjectScale),
	mObjectRotation(marker.mObjectRotation)
{
	mMouseX = marker.mMouseX;
	mMouseY = marker.mMouseY;
	mMarkerState = marker.mMarkerState;
	mMarkerType = marker.mMarkerType;
	mFlagGrid = marker.mFlagGrid;
	mGridSizeX = marker.mGridSizeX;
	mGridSizeZ = marker.mGridSizeZ;
	mpObject = NULL;
	mSpeedScale = marker.mSpeedScale;
	mSpeedRotate = marker.mSpeedRotate;
	mFlagScale = marker.mFlagScale;
	mFlagRotate = marker.mFlagRotate;
	resetTransform();
}

MouseMarker::~MouseMarker()
{

}

const MouseMarker &MouseMarker::operator=(const MouseMarker &marker)
{
	if(this != &marker)
	{
		mMouseX = marker.mMouseX;
		mMouseY = marker.mMouseY;
		mMouseRay = marker.mMouseRay;
		mPlanePosition = marker.mPlanePosition;
		mMarkerState = marker.mMarkerState;
		mMarkerType = marker.mMarkerType;
		mFlagGrid = marker.mFlagGrid;
		mGridSizeX = marker.mGridSizeX;
		mGridSizeZ = marker.mGridSizeZ;
		mpObject = NULL;
		mObjectScale = marker.mObjectScale;
		mObjectRotation = marker.mObjectRotation;
		mSpeedScale = marker.mSpeedScale;
		mSpeedRotate = marker.mSpeedRotate;
		mFlagScale = marker.mFlagScale;
		mFlagRotate = marker.mFlagRotate;
	}
	return *this;
}

void MouseMarker::updateMouseRay()
{
	long cW = DX10Module::getInstance()->getClientWidth();
	long cH = DX10Module::getInstance()->getClientHeight();
	Camera *camera = DX10Module::getInstance()->getCamera();
	D3DXMATRIX viewInverse;
	D3DXMATRIX proj;
	D3DXMATRIX t;
	camera->GetVPMatrices(viewInverse, proj, t);
	D3DXMatrixInverse(&viewInverse, NULL, &viewInverse);
	float scaleX = proj(0, 0);
	float scaleY = proj(1, 1);

	mMouseRay.mOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//Transform to homogenous space.
	mMouseRay.mDirection.x = ((2.0f * mMouseX) / cW - 1.0f) / scaleX;
	mMouseRay.mDirection.y = ((-2.0f * mMouseY) / cH + 1.0f) / scaleY;
	mMouseRay.mDirection.z = 1.0f;

	//Transform to world space.
	D3DXVec3TransformCoord(&mMouseRay.mOrigin, &mMouseRay.mOrigin, &viewInverse);
	D3DXVec3TransformNormal(&mMouseRay.mDirection, &mMouseRay.mDirection, &viewInverse);
	D3DXVec3Normalize(&mMouseRay.mDirection, &mMouseRay.mDirection);
}

void MouseMarker::updateMousePlanePosition()
{
	//Increase the mouse origin an amount of mouse directions in order to reach Y = 0.
	float dY = mMouseRay.mOrigin.y / -mMouseRay.mDirection.y;
	mPlanePosition = mMouseRay.mOrigin + mMouseRay.mDirection * dY;
	mPlanePosition.y = 0.0f;
}

void MouseMarker::updateTransform(float deltaTime)
{
	if(mFlagScale)
	{
		//Scale along the X and Z axis with an extra speed factor.
		if(this->mMarkerType == MouseMarker::MarkerType_Trigger)
		{
			mObjectScale += D3DXVECTOR3(1.0f, 0.0f, 1.0f) * 100.0f * mSpeedScale * deltaTime;
		}
		//Scale in all directions.
		else
		{
			mObjectScale += D3DXVECTOR3(1.0f, 1.0f, 1.0f) * mSpeedScale * deltaTime;
		}
		mFlagScale = false;
	}
	if(mFlagRotate)
	{
		//Only update rotation around the Y-axis.
		mObjectRotation.y += mSpeedRotate * deltaTime;
		mFlagRotate = false;
	}
}

void MouseMarker::resetTransform()
{
	mObjectRotation = D3DXVECTOR3(0.0f, (float)(1.0 * D3DX_PI), 0.0f);
	mObjectScale = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
}

void MouseMarker::updateObject()
{
	//Rotate a direction around the Y-axis.
	D3DXVECTOR3 objectDir;
	getDirection(objectDir);

	utilities::Vec3Float actorScale;

	switch(mMarkerType)
	{
		case MouseMarker::MarkerType_Object:
			((TransformObject *)mpObject)->setPosition(mPlanePosition);
			((TransformObject *)mpObject)->setRotation(mObjectRotation + D3DXVECTOR3(0.0f, -(float)(1.5 * D3DX_PI), 0.0f));
			((TransformObject *)mpObject)->setScale(mObjectScale);
			snapToGrid();
			break;

		case MouseMarker::MarkerType_Actor:
			((Actor *)mpObject)->setPosition(mPlanePosition.x, mPlanePosition.y, mPlanePosition.z);
			((Actor *)mpObject)->setDirection(utilities::Direction(objectDir.x, objectDir.y, objectDir.z));
			actorScale = utilities::Vec3Float(mObjectScale.x, mObjectScale.y, mObjectScale.z);
			((Actor *)mpObject)->setScale(actorScale);
			snapToGrid();
			break;

		case MouseMarker::MarkerType_Trigger:
			((TransformObject *)mpObject)->setPosition(mPlanePosition + D3DXVECTOR3(0.0f, ((TransformObject *)mpObject)->getScaleY() * 0.5f, 0.0f));
			((TransformObject *)mpObject)->setRotation(mObjectRotation + D3DXVECTOR3(0.0f, -(float)(1.5 * D3DX_PI), 0.0f));
			((TransformObject *)mpObject)->setScale(mObjectScale);
			snapToGrid();
			break;

		default:
			break;
	}
}

void MouseMarker::updateObjectPosition()
{
	switch(mMarkerType)
	{
		case MouseMarker::MarkerType_Object:
			((TransformObject *)mpObject)->setPosition(mPlanePosition);
			snapToGrid();
			break;

		case MouseMarker::MarkerType_Actor:
			((Actor *)mpObject)->setPosition(mPlanePosition.x, mPlanePosition.y, mPlanePosition.z);
			snapToGrid();
			break;

		case MouseMarker::MarkerType_Trigger:
			((TransformObject *)mpObject)->setPosition(mPlanePosition + D3DXVECTOR3(0.0f, ((TransformObject *)mpObject)->getScaleY() * 0.5f, 0.0f));
			snapToGrid();
			break;

		default:
			break;
	}
}

void MouseMarker::updateObjectRotation()
{
	//Rotate a direction around the Y-axis.
	D3DXVECTOR3 objectDir;
	getDirection(objectDir);

	switch(mMarkerType)
	{
		case MouseMarker::MarkerType_Object:
			((TransformObject *)mpObject)->setRotation(mObjectRotation + D3DXVECTOR3(0.0f, -(float)(1.5 * D3DX_PI), 0.0f));
			snapToGrid();
			break;

		case MouseMarker::MarkerType_Actor:
			((Actor *)mpObject)->setDirection(utilities::Direction(objectDir.x, objectDir.y, objectDir.z));
			snapToGrid();
			break;

		case MouseMarker::MarkerType_Trigger:
			((TransformObject *)mpObject)->setRotation(mObjectRotation + D3DXVECTOR3(0.0f, -(float)(1.5 * D3DX_PI), 0.0f));
			snapToGrid();
			break;

		default:
			break;
	}
}

void MouseMarker::updateObjectScale()
{
	utilities::Vec3Float actorScale;

	switch(mMarkerType)
	{
		case MouseMarker::MarkerType_Object:
			((TransformObject *)mpObject)->setScale(mObjectScale);
			break;

		case MouseMarker::MarkerType_Actor:
			actorScale = utilities::Vec3Float(mObjectScale.x, mObjectScale.y, mObjectScale.z);
			((Actor *)mpObject)->setScale(actorScale);
			break;

		case MouseMarker::MarkerType_Trigger:
			((TransformObject *)mpObject)->setScale(mObjectScale);
			break;

		default:
			break;
	}
}

bool MouseMarker::selectObject(int types, bool overrideObject)
{
	//Attempt to select an object.
	if(!mpObject || overrideObject)
	{
		LogicQuadTree *pLogicTree = LogicQuadTree::getInstance();
		QuadTree *pTree = pLogicTree->getQuadTree();

		vector<CollisionObject *> objects;

		//Check intersection with the mouse ray.
		pTree->intersectionRay(mMouseRay.mOrigin, mMouseRay.mDirection, objects);

		float minDistance = (float)INT_MAX;
		float distance = 0.0f;
		CollisionObject *pClosestObject = NULL;
		//Find the closest object.
		for(vector<CollisionObject *>::iterator objectIter = objects.begin();
			objectIter != objects.end(); ++objectIter)
		{
			//Ignore ground objects.
			if((*objectIter)->getID() != KEY_GROUND)
			{
				distance = (*objectIter)->getDistance(mMouseRay.mOrigin);
				if(distance < minDistance)
				{
					minDistance = distance;
					pClosestObject = (*objectIter);
				}
			}
		}
		if(pClosestObject)
		{
			TransformObject *pObject = (TransformObject *)pClosestObject->getParent();
			Actor *pActor = (Actor *)pObject->getActor();

			mObjectRotation = pObject->getRotation();

			//Apply rotation offset.
			mObjectRotation += D3DXVECTOR3(0.0f, -(float)(0.5 * D3DX_PI), 0.0f);
			mObjectScale = pObject->getScale();

			//Check for trigger.
			if(pObject->getID() == KEY_TRIGGER.convertToGfxId())
			{
				if(types & MouseMarker::MarkerType_Trigger)
				{
					mMarkerState = MouseMarker::MarkerState_Select;
					mMarkerType = MouseMarker::MarkerType_Trigger;
					mpObject = pObject;
					return true;
				}
			}
			//Check if the selected object is an actor.
			else if(pActor)
			{
				if(types & MouseMarker::MarkerType_Actor)
				{
					//Check that the actor is owned by the player.
					if(pActor->getKey().isOwner())
					{
						//Set type to actor.
						mMarkerState = MouseMarker::MarkerState_Select;
						mMarkerType = MouseMarker::MarkerType_Actor;
						mpObject = pActor;
						return true;
					}
				}
			}
			//Should be an object otherwise.
			else
			{
				if(types & MouseMarker::MarkerType_Object)
				{
					//Set type to object.
					if(pObject->getID() >= 0)
					{
						mMarkerState = MouseMarker::MarkerState_Select;
						mMarkerType = MouseMarker::MarkerType_Object;
						mpObject = pObject;
						return true;
					}
				}
			}
		}
		else
		{
			clearObject();
		}
	}
	return false;
}

void MouseMarker::clearObject()
{
	mpObject = NULL;
	mMarkerType = MouseMarker::MarkerType_None;
}

void MouseMarker::removeObject()
{
	switch(mMarkerType)
	{
		case MouseMarker::MarkerType_Object:
			DX10Module::getInstance()->removeRenderObject((RenderObject *)mpObject);
			break;

		case MouseMarker::MarkerType_Actor:
			break;

		case MouseMarker::MarkerType_Trigger:
			break;

		default:
			break;
	}
	clearObject();
}

void MouseMarker::getDirection(D3DXVECTOR3 &direction) const
{
	//Rotate a direction around the Y-axis.
	direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXMATRIX rotY;
	D3DXMatrixRotationY(&rotY, mObjectRotation.y - (float)(1.0 * D3DX_PI));
	D3DXVec3TransformNormal(&direction, &direction, &rotY);
	D3DXVec3Normalize(&direction, &direction);
}

void MouseMarker::getDirection(D3DXVECTOR3 &direction, float angleY) const
{
	//Rotate a direction around the Y-axis.
	direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXMATRIX rotY;
	D3DXMatrixRotationY(&rotY, angleY);
	D3DXVec3TransformNormal(&direction, &direction, &rotY);
	D3DXVec3Normalize(&direction, &direction);
}

void MouseMarker::snapToGrid()
{
	if(mFlagGrid)
	{
		switch(mMarkerType)
		{
			case MouseMarker::MarkerType_Object:
			{
				//Snap position to grid.
				D3DXVECTOR3 position = ((TransformObject *)mpObject)->getPosition();
				position.x = (float)(((int)position.x / (int)mGridSizeX) * (int)mGridSizeX + (int)mGridSizeX / 2);
				position.z = (float)(((int)position.z / (int)mGridSizeZ) * (int)mGridSizeZ + (int)mGridSizeZ / 2);
				((TransformObject *)mpObject)->setPosition(position);

				//Snap rotation to grid angle.
				D3DXVECTOR3 rotation = mObjectRotation;
				rotation.y = (int)((rotation.y - (float)(1.5 * D3DX_PI) + 0.01f) / mGridAngle) * mGridAngle;
				((TransformObject *)mpObject)->setRotationY(rotation.y);
				break;
			}

			case MouseMarker::MarkerType_Actor:
			{
				//Snap position to grid.
				utilities::Position position = ((Actor *)mpObject)->getPosition();
				position.x = (float)(((int)position.x / (int)mGridSizeX) * (int)mGridSizeX + (int)mGridSizeX / 2);
				position.z = (float)(((int)position.z / (int)mGridSizeZ) * (int)mGridSizeZ + (int)mGridSizeZ / 2);
				((Actor *)mpObject)->setPosition(position);

				//Snap rotation to grid angle.
				D3DXVECTOR3 rotation = mObjectRotation;
				rotation.y = (int)((rotation.y + 0.01f) / mGridAngle) * mGridAngle - (float)(1.0 * D3DX_PI);
				getDirection(rotation, rotation.y);
				((Actor *)mpObject)->setDirection(utilities::Direction(rotation.x, rotation.y, rotation.z));
				break;
			}

			default:
				break;
		}
	}
}