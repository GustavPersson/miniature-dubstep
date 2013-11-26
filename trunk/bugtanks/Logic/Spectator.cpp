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
 */

#include "Spectator.h"

using namespace logic;

Spectator::Spectator(int networkId,utilities::Position pos, utilities::Direction dir, Actor::PlayerMovingInfo playerMovingInfo) : Unit(pos,dir,0.0f,networkId,utilities::MapCoordinate(0,0))
{
	mScale = utilities::Vec3Float(1.5f, 3.5f, 1.5f);
	
	mActorType = ActorType_Spectator;
	mSpeed = ActorConstants::SPECTATOR_SPEED;
	mOriginalSpeed	= mSpeed;

	mMoving				= playerMovingInfo.mMoving;
	mMovingDirection	= playerMovingInfo.mMovingDirection;
	mRotating			= playerMovingInfo.mRotating;
	mRotateDirection	= playerMovingInfo.mRotatingDirection;
}

Spectator::~Spectator()
{

}

void Spectator::update(float deltaTime)
{
	if(mPosition.y != 0.0f)
	{
		DynamicActor::update(deltaTime);
	}
	if(mMoving)
	{
		if(mMovingDirection == 1)
			mSpeed = ActorConstants::SPECTATOR_SPEED;
		else if(mMovingDirection == -1)
			mSpeed = -ActorConstants::SPECTATOR_SPEED;
	}
	else
		mSpeed = 0.0f;
	if( fabsf(mSpeed) > 0.0f)
	{
		mPrePosition = mPosition;
		mPosition = mPosition + (mSpeed * mDirection * deltaTime);

	}
	if(mRotating)
	{
		mDirection.rotateAroundY(2.0f * deltaTime * mRotateDirection);
		mDirection.normalize();
	}
	//DynamicActor::update(deltaTime);
}

void Spectator::handleEventId(EventData const &eventMessage)
{
	if(EvtData_From_DriverEnterTank::msEventType == eventMessage.getEventType())
	{
		enterTank();
	}
	else
	Actor::handleEventId(eventMessage);
}

void Spectator::enterTank()
{
	PlayerMovingInfo info;
	info.mMoving = this->mMoving;
	info.mRotating = this->mRotating;
	info.mMovingDirection = this->mMovingDirection;
	info.mRotatingDirection = this->mRotateDirection;
	ActorHandler::getInstance()->playerDied(mId.getOwnerId(), info);
	//if a tank is in range the driver will jump in
	safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateTank(Tank::TankType_CloseCombat, mPosition, mDirection, info)));
	safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_InputChangeGameState(logic::InputStates::GameState_Game_Inside_Tank)));
	DEBUG_MESSAGE(LEVEL_LOWEST, "spectatorentertank");
	safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(this->getKey(),-3)));
}