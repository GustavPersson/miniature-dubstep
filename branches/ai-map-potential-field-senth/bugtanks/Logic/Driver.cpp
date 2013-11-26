/**
 * @file
 * @author Dennis Jönsson <zelebsel@gmail.com>
 * @author Matteus Magnusson <senth.wallace@gmail.com>
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
 * This is the driver class, it holds information and functions for the driver, I have defined that as the man driving the tank here
 */

#include "Driver.h"
#include "Tank.h"
#include "Shotgun.h"
#include "ActorHandler.h"

// ------------------------------
// AI Section BEGIN
// ------------------------------
#ifdef USE_AI
#include "../Ai/AiFactory.h"
#endif
// ------------------------------
// AI Section END
// ------------------------------

using namespace logic;
const float DRIVER_ROTATE_SPEED			= 2.0f;
const float DRIVER_STRAFE_SPEED			= 20.0f;

Driver::Driver(int networkId,utilities::Position pos, utilities::Direction dir, Actor::PlayerMovingInfo playerMovingInfo) : Unit(pos, dir,ActorConstants::DRIVER_HP,networkId, utilities::MapCoordinate(1,1))
{
	mScale = utilities::Vec3Float(1.5f, 3.5f, 1.5f);

	mActorType = ActorType_Driver;

	mStrafing = false;
	mStrafeLeft = false;
	mFlying = false;

	mStrafeDirection = utilities::Direction(0.0f,0.0f,0.0f);
	
	mpShotgun = myNew Shotgun();
	
	mSpeed			= ActorConstants::DRIVER_SPEED;
	mRotateSpeed	= ActorConstants::DRIVER_ROTATE_SPEED;
	mStrafeSpeed	= ActorConstants::DRIVER_STRAFE_SPEED;

	mMoving				= playerMovingInfo.mMoving;
	mMovingDirection	= playerMovingInfo.mMovingDirection;
	mRotating			= playerMovingInfo.mRotating;
	mRotateDirection	= playerMovingInfo.mRotatingDirection;
		
	mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, 0));
	//mpTransformObject->setTextureID(1);
    //dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setFlagUpdateAbsolute(false);
	mpTransformObject->setID(mId.convertToGfxId());
	mpTransformObject->setName("Human");
	mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
	dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(mScale.convertToD3DXVECTOR3());
	mpTransformObject->setActor(this);
	((gfx::InstanceAnimationMesh *)mpTransformObject)->setAnimation("Bugman_Walk");
	((gfx::InstanceAnimationMesh *)mpTransformObject)->setAnimationSpeedFactor(3.0f);

	for(std::vector<gfx::InstanceSubset *>::const_iterator instanceIter = ((gfx::InstanceMesh *)mpTransformObject)->getSubsetList().begin();
		instanceIter != ((gfx::InstanceMesh *)mpTransformObject)->getSubsetList().end(); ++instanceIter)
	{
		(*instanceIter)->setLogicBoundingVolume((*instanceIter)->getBoundingVolume()->clone());
		if(instanceIter - ((gfx::InstanceMesh *)mpTransformObject)->getSubsetList().begin() == 0)
		{
			mpPrePosCollisionObject = (*instanceIter)->getLogicBoundingVolume();
		}
	}
	thrownOutOfTank();

// ------------------------------
// AI Section BEGIN
// ------------------------------
#ifdef USE_AI
	// Add the driver to the AiFactory
	ai::AiFactory* pAiFactory = ai::AiFactory::getInstance();
	if (pAiFactory != NULL)
	{
		pAiFactory->addDriver(this);
	}
#endif
// ------------------------------
// AI Section END
// ------------------------------
}


Driver::~Driver()
{
	SAFE_DELETE(mpShotgun);

// ------------------------------
// AI Section BEGIN
// ------------------------------
#ifdef USE_AI
	// Remove the driver from the factory
	ai::AiFactory* pAiFactory = ai::AiFactory::getInstance();
	if (pAiFactory != NULL)
	{
		pAiFactory->removeDriver(getKey());
	}
#endif
// ------------------------------
// AI Section END
// ------------------------------
}

void Driver::thrownOutOfTank()
{
	utilities::Direction jumpDirection = utilities::Direction(1.0f,0.0f,0.0f);
	float randomMin = 0.0f;
	float randomMax = utilities::math::PI_TWO;
	float random = ((randomMax-randomMin)*((float)rand()/RAND_MAX))+randomMin;
	jumpDirection.rotateAroundY(random);
	//HACK 
	float jumpLength = ActorConstants::DRIVER_EJECT_RANGE;
	float halfTime = jumpLength * 0.5f/ActorConstants::DRIVER_EJECT_SPEED;

	float ySpeed = ActorConstants::GRAVITATION * halfTime;
	jumpDirection.normalize();
	jumpDirection *= ActorConstants::DRIVER_EJECT_SPEED;
	jumpDirection.y = ySpeed;
	jumpDirection.normalize();
	this->setDirection(jumpDirection);
	this->setSpeed(ActorConstants::DRIVER_EJECT_SPEED + ySpeed);
	mMoving = true;
	mFlying = true;
}


void Driver::fireShotgun()
{
	mpShotgun->fireWeapon(mId);
   //if gamestate is outsideTank the fire command will now be directed here
}

Weapon* Driver::getWeapon()
{
	return mpShotgun;
}

void Driver::reduceHp(float damage)
{
	DEBUG_MESSAGE(LEVEL_LOW, "HP: " << mHealthPoints);
	mHealthPoints -= damage;
	if(mHealthPoints <= 0 && !mDead && mId.isOwner())
	{

		PlayerMovingInfo info;
		info.mMoving = this->mMoving;
		info.mRotating = this->mRotating;
		info.mMovingDirection = this->mMovingDirection;
		info.mRotatingDirection = this->mRotateDirection;
		DEBUG_MESSAGE(LEVEL_MEDIUM, "Lol i die ");
		ActorHandler::getInstance()->playerDied(mId.getOwnerId(), info);
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateSpectator(mPosition, mDirection, info)));
		safeQueueEvent(EventDataPtr(myNew EvtData_To_InputChangeGameState(logic::InputStates::GameState_GameOver)));
		safeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(this->getKey())));
		mDead = true;
	}
}

void Driver::enterTank()
{
	PlayerMovingInfo info;
	info.mMoving = this->mMoving;
	info.mRotating = this->mRotating;
	info.mMovingDirection = this->mMovingDirection;
	info.mRotatingDirection = this->mRotateDirection;
	ActorHandler::getInstance()->playerDied(mId.getOwnerId(), info);
  //if a tank is in range the driver will jump in
	safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateTank(Tank::TankType_CloseCombat, mPosition, mDirection, info)));
	safeQueueEvent(EventDataPtr(myNew EvtData_To_InputChangeGameState(logic::InputStates::GameState_Game_Inside_Tank)));
	safeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(this->getKey())));
  //stop rendering driver
  //change gamestate to insideTank, meaning input will now move the tank and positions will be update for tank
}

void Driver::update(float deltaTime)
{
	//position will not be updated for driver while gamestate is insideTank
 	if(mFire)
		fireShotgun();

	if(mFlying)
	{
		DynamicActor::update(deltaTime);
		if(mPosition.y == 0.0f)
		{
			mFlying = false;
			mMoving = false;
			mSpeed  = 0.0f;
		}
	}
	else
	{
		if(mMoving)
		{
			if(mMovingDirection == 1)
				mSpeed = ActorConstants::DRIVER_SPEED;
			else if(mMovingDirection == -1)
				mSpeed = -ActorConstants::DRIVER_SPEED*0.5f;
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
			mDirection.rotateAroundY(mRotateSpeed * deltaTime * mRotateDirection);
			mDirection.normalize();
		}
		if(mStrafing)
		{
			if(mStrafeLeft)
			{
				mStrafeDirection = mDirection;
				mStrafeDirection.rotateAroundY(utilities::math::PI_HALF);
				mStrafeDirection.normalize();
				mPosition = mPosition + mStrafeDirection * deltaTime * mStrafeSpeed;
			}
			else
			{
				mStrafeDirection = mDirection;
				mStrafeDirection.rotateAroundY(-utilities::math::PI_HALF);
				mStrafeDirection.normalize();
				mPosition = mPosition + mStrafeDirection * deltaTime * mStrafeSpeed;
			}
		}
	}

	mpPrePosCollisionObject->setPosition(mPosition.convertToD3DXVECTOR3());

}

void Driver::handleEventId(EventData const &eventMessage)
{
	if(EvtData_To_DriverStartFire::msEventType == eventMessage.getEventType())
	{
		mFire = true;
	}
	if(EvtData_To_DriverStopFire::msEventType == eventMessage.getEventType())
	{
		mFire = false;
	}
	if(EvtData_From_DriverEnterTank::msEventType == eventMessage.getEventType())
	{
		enterTank();
	}
	if(EvtData_To_DriverStrafe::msEventType == eventMessage.getEventType())
	{
		const EvtData_To_DriverStrafe& evtData = static_cast<const EvtData_To_DriverStrafe&>(eventMessage);
		mStrafeLeft = evtData.mStrafeLeft;
		mStrafing = evtData.mStrafing;
	}
	else
		Actor::handleEventId(eventMessage);
}
