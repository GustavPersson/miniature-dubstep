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
 * This class will have a map of all the game objects in the game and it will be used to update all objects smoothly
 */

#include "Actor.h"

using namespace logic;
KeyHandler *Actor::mpsKeyHandler = NULL;

const int ROTATE_LEFT = 1;
const int ROTATE_RIGHT = -1;
const int MOVE_FORWARD = 1;
const int MOVE_BACKWARDS = -1;

Actor::Actor()
{
  assert(mpsKeyHandler != NULL);
  mId  = mpsKeyHandler->allocateKey(1);
  mMoving = false;
  mRotating = false;
  mForward = false;
  mSubsetRotating = false;
  mRotateDirection = 0;
  mSubsetRotateDirection = 0;
  mSpeed = 0.0f;
  mActorType = ActorType_Invalid;
  
}
Actor::~Actor()
{
}

void Actor::init()
{
  mpsKeyHandler = KeyHandler::getInstance();
}

KeyType Actor::getKey()
{
	return mId;
}

utilities::Direction Actor::getDirection()
{
	return mDirection;
}

utilities::Position Actor::getPosition()
{
    return mPosition;
}

utilities::Direction Actor::getSubsetDirection()
{
	return mSubsetDirection;
}

void Actor::setPosition(float x, float y, float z)
{
	mPosition.x = x;
	mPosition.y = y;
	mPosition.z = z;
}
void Actor::setDirection(float x, float y, float z)
{
	mDirection.x = x;
	mDirection.y = y;
	mDirection.z = z;
}

void Actor::setSubsetDirection(float x, float y, float z)
{
	mSubsetDirection.x = x;
	mSubsetDirection.y = y;
	mSubsetDirection.z = z;
}

Actor::ActorType Actor::getActorType()
{
	return mActorType;
}

void Actor::handleEventId(const logic::EventData &eventMessage)
{
	if(EvtData_To_ActorStartMove::mEventType == eventMessage.getEventType())
	{
		const EvtData_To_ActorStartMove& evtData = static_cast<const EvtData_To_ActorStartMove&>(eventMessage);
		mMoving = true;
		if(evtData.mForward)
			mForward += MOVE_FORWARD;
		else
			mForward += MOVE_BACKWARDS;

		if(mForward == 0)
			mMoving = false;
	}
	else if(EvtData_To_ActorStopMove::mEventType == eventMessage.getEventType())
	{
		const EvtData_To_ActorStartMove& evtData = static_cast<const EvtData_To_ActorStartMove&>(eventMessage);
		if(evtData.mForward)
			mForward -= MOVE_FORWARD;
		else
			mForward -= MOVE_BACKWARDS;

		if(mForward == 0)
			mMoving = false;
		else
			mMoving = true;
	}
	else if(EvtData_To_ActorStartRotate::mEventType == eventMessage.getEventType())
	{
		const EvtData_To_ActorStartRotate& evtData = static_cast<const EvtData_To_ActorStartRotate&>(eventMessage);
		if(evtData.mRotateSubset)
		{
			mSubsetRotating = true;
			if(evtData.mRotateLeft)
				mSubsetRotateDirection += ROTATE_LEFT;
			else
				mSubsetRotateDirection += ROTATE_RIGHT;
		}
		else
		{
			mRotating = true;
			if(evtData.mRotateLeft)
				mRotateDirection += ROTATE_LEFT;
			else
				mRotateDirection += ROTATE_RIGHT;
		}
	}
	else if(EvtData_To_ActorStopRotate::mEventType == eventMessage.getEventType())
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
		}
		else
		{
			if(evtData.mRotateLeft)
				mRotateDirection -= ROTATE_LEFT;
			else
				mRotateDirection -= ROTATE_RIGHT;
			if(mRotateDirection == 0)
				mRotating = false;
		}
	}
}

void Actor::setMoving(bool move)
{
	mMoving = move;
}

bool Actor::getMoving()
{
	return mMoving;
}
bool Actor::getRotating()
{
	return mRotating;
}