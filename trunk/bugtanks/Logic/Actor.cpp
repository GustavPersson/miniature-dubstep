/**
 * @file
 * @author Dennis Jönsson <zelebsel@gmail.com>
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
 * 
 * @section DESCRIPTION
 *
 * This is the topmost in the actor hierarchy, it will be used to store all game actors in a map
 */

#include "Actor.h"
#include "PlayerInfo.h"
#include "GlobalTimer.h"
#include "KeyHandler.h"
#include "Events.h"
#include "ActorHandler.h"
#include "../Utilities/Constants.h"
#include "../GFX/DX10Module.h"
#include <math.h>
#include <cassert>

using namespace logic;
using namespace std;

KeyHandler *Actor::mpsKeyHandler = NULL;
PlayerInfo *Actor::mpsPlayerInfo = NULL;
ActorHandler *Actor::mpsActorHandler = NULL;
gfx::DX10Module* Actor::mpsDXModule = NULL;

const int ROTATE_LEFT = 1;
const int ROTATE_RIGHT = -1;
const int MOVE_FORWARD = 1;
const int MOVE_BACKWARDS = -1;

Actor::Actor(utilities::Position pos, utilities::Direction dir, int networkId, const utilities::MapCoordinate& size) : mSize(size)
{
	assert(mpsKeyHandler != NULL);
	mId = mpsKeyHandler->allocateKey(networkId);
	mActorType	= ActorType_Invalid;

	mPosition			= pos;
	mPrePosition		= pos;
	mDirection			= dir;
	mSubsetDirection	= dir;

	mMoving			= false;
	mRotating		= false;
	mSubsetRotating = false;
	mDead			= false;
	mScaleChange	= true;
	mSync = false;

	mSpeed = 0.0f;

	mRotateDirection		= 0;
	mSubsetRotateDirection	= 0;
	mMovingDirection		= 0;

	mpTransformObject		= NULL;
	mpPrePosCollisionObject = NULL;
}
Actor::~Actor()
{
}

void Actor::init()
{
  mpsKeyHandler = KeyHandler::getInstance();
  mpsDXModule = gfx::DX10Module::getInstance();
  mpsPlayerInfo = PlayerInfo::getInstance();
  mpsActorHandler = ActorHandler::getInstance();
}

void Actor::handleEventId(const logic::EventData &eventMessage)
{
	if(EvtData_To_ActorStartMove::msEventType == eventMessage.getEventType())
	{
		const EvtData_To_ActorStartMove& evtData = static_cast<const EvtData_To_ActorStartMove&>(eventMessage);
		mMoving = true;
		if(evtData.mForward)
			mMovingDirection += MOVE_FORWARD;
		else
			mMovingDirection += MOVE_BACKWARDS;

		if(mMovingDirection == 0)
			mMoving = false;
	}
	else if(EvtData_To_ActorStopMove::msEventType == eventMessage.getEventType())
	{
		const EvtData_To_ActorStartMove& evtData = static_cast<const EvtData_To_ActorStartMove&>(eventMessage);
		if(evtData.mForward)
			mMovingDirection -= MOVE_FORWARD;
		else
			mMovingDirection -= MOVE_BACKWARDS;

		if(mMovingDirection == 0)
			mMoving = false;
		else
			mMoving = true;
	}
	else if(EvtData_To_ActorStartRotate::msEventType == eventMessage.getEventType())
	{
		const EvtData_To_ActorStartRotate& evtData = static_cast<const EvtData_To_ActorStartRotate&>(eventMessage);
		if(evtData.mRotateSubset)
		{
			mSubsetRotating = true;
			if(evtData.mRotateLeft)
				mSubsetRotateDirection += ROTATE_LEFT;
			else
				mSubsetRotateDirection += ROTATE_RIGHT;

			if(mSubsetRotateDirection == 0)
		 		mSubsetRotating = false;
		}
		else
		{
			mRotating = true;
			if(evtData.mRotateLeft)
				mRotateDirection += ROTATE_LEFT;
			else
				mRotateDirection += ROTATE_RIGHT;

			if(mRotateDirection == 0)
				mRotating = false;
		}
	}
	else if(EvtData_To_ActorStopRotate::msEventType == eventMessage.getEventType())
	{
		const EvtData_To_ActorStopRotate& evtData = static_cast<const EvtData_To_ActorStopRotate&>(eventMessage);
		if(evtData.mRotateSubset)
		{
			if(evtData.mRotateLeft)
				mSubsetRotateDirection -= ROTATE_LEFT;
			else
				mSubsetRotateDirection -= ROTATE_RIGHT;

			if(mSubsetRotateDirection == 0)
				mSubsetRotating = false;
			else
				mSubsetRotating = true;
		}
		else
		{
			if(evtData.mRotateLeft)
				mRotateDirection -= ROTATE_LEFT;
			else
				mRotateDirection -= ROTATE_RIGHT;

			if(mRotateDirection == 0)
				mRotating = false;
			else
				mRotating = true;
		}
	}
	else if(EvtData_From_ActorCollisionObject::msEventType == eventMessage.getEventType())
	{
		const EvtData_From_ActorCollisionObject& evtData = static_cast<const EvtData_From_ActorCollisionObject&>(eventMessage);
		utilities::Direction colNorm = evtData.mCollisionNormal;
		
		//mSpeed = 0;

		float angle = mDirection.getAngleBetweenVectors(colNorm);

		if(mActorType == ActorType_Tank)
		{
			if(angle < 0.174532925f)
			{
				//DEBUG_MESSAGE(LEVEL_MEDIUM, "mindre än 10");
				if(mSpeed > 10.0f)
				{
					mSpeed = 10.0f;
				}
			}
			else if(angle < 1.04719755f)
			{
				//DEBUG_MESSAGE(LEVEL_MEDIUM, "mindre än 60");
				if(mSpeed > 20.0f)
				{
					mSpeed = 20.0f;
				}
			}
			else if(angle < 2.26892803f)
			{
				//DEBUG_MESSAGE(LEVEL_MEDIUM, "mindre än 130");
				if(mSpeed > 30.0f)
				{
					mSpeed = 30.0f;
				}
			}
		}
		
		//colNorm.rotateAroundY(utilities::math::PI_HALF);
		//mPosition = mPosition + colNorm * 1.0f;
		//setPosition(mPrePosition.x + colNorm.x, mPrePosition.y, mPrePosition.z + colNorm.z);
		/*float speedReduce = (fabsf(mDirection.getXZAngleCounterclockwise()) - fabsf(colNorm.getXZAngleCounterclockwise() - utilities::math::PI)) / utilities::math::PI_HALF;
			//(fabsf(mDirection.getXZAngleCounterclockwise()) - fabsf((colNorm - utilities::math::PI).getXZAngleCounterclockwise())) / utilities::math::PI_HALF;
		mSpeed = mSpeed * speedReduce;
		DEBUG_MESSAGE(LEVEL_LOWEST,"SPEED: " << mSpeed);*/
	}

}

void Actor::setPosition(float x, float y, float z)
{
	mPosition.x = x;
	mPosition.y = y;
	mPosition.z = z;
	mSync = true;
}

void Actor::setPosition(utilities::Position pos)
{
	mPosition = pos;
	mSync = true;
}

void Actor::setDestPosition(utilities::Position destPos)
{
	mDestPos = destPos;
	mSync = true;
}

void Actor::setDirection(float x, float y, float z)
{
	mDirection.x = x;
	mDirection.y = y;
	mDirection.z = z;
	mSync = true;
}


Actor::ActorTypes Actor::getActorType()
{
	return mActorType;
}

void Actor::setMoving(bool move)
{
	mMoving = move;
}

void Actor::setSubsetDirection(float x, float y, float z)
{
	mSubsetDirection.x = x;
	mSubsetDirection.y = y;
	mSubsetDirection.z = z;
	mSync = true;
}

void Actor::setSubsetDirection(utilities::Direction subsetDirection)
{
	mSubsetDirection = subsetDirection;
}

utilities::Direction Actor::getSubsetDirection()
{
	return mSubsetDirection;
}

bool Actor::getMoving()
{
	return mMoving;
}

bool Actor::getRotating()
{
	return mRotating;
}

gfx::TransformObject* Actor::getTransformObject()
{
	return mpTransformObject;
}

bool Actor::getSubsetRotating()
{
	return mSubsetRotating;
}

const utilities::Position& Actor::getPrePosition()
{
	return mPrePosition;
}

const utilities::Direction& Actor::getPreDirection()
{
	return mPreDirection;
}

const int& Actor::getMovingDirection()
{
	return mMovingDirection;
}

gfx::CollisionObject* Actor::getPreCollisionObject()
{
	return mpPrePosCollisionObject;
}

void Actor::setScale(utilities::Vec3Float scale)
{
	mScale = scale;
	mScaleChange = true;
	mSync = true;
}
utilities::Vec3Float Actor::getScale()
{
	return mScale;
}

void Actor::setVisible(bool visible)
{
	mVisible = visible;
}

bool Actor::getVisible()
{
	return mVisible;
}

std::istream& operator>>(std::istream &in, logic::Actor::ActorTypes& actorType)
{
	int intActorType;
	in >> intActorType;
	actorType = static_cast<logic::Actor::ActorTypes>(intActorType);
	return in;
}

std::istream& operator>>(std::istream &in, logic::Actor::PlayerMovingInfo& playerMovingInfo)
{
	in >> playerMovingInfo.mMoving >> playerMovingInfo.mRotating >> playerMovingInfo.mRotatingDirection >> playerMovingInfo.mMovingDirection;
	return in;
}
std::ostream& operator<<(std::ostream &out, const logic::Actor::PlayerMovingInfo& playerMovingInfo)
{
	out << playerMovingInfo.mMoving << " " << playerMovingInfo.mRotating << " " << playerMovingInfo.mRotatingDirection << " " << playerMovingInfo.mMovingDirection << " ";
	return out;
}

std::istream& operator>>(std::istream &in, logic::Actor::ActorSyncInfo& syncInfo)
{
	in >> syncInfo.mId >> syncInfo.mPosition >> syncInfo.mDirection >> syncInfo.mSubsetDirection >> syncInfo.mScale;
	return in;
}

std::ostream& operator<<(std::ostream &out, const logic::Actor::ActorSyncInfo& syncInfo)
{
	out << syncInfo.mId << " " << syncInfo.mPosition << " " << syncInfo.mDirection << " " << syncInfo.mSubsetDirection << " " << syncInfo.mScale << " ";
	return out;
}