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
 * This is the tank class, it will provide functions and hold information needed for the tank
 */

#include "Tank.h"
#include "Driver.h"
#include "ActorHandler.h"
#include "Weapon.h"
#include "PedestrianGun.h"
#include "Minigun.h"
#include "Flamethrower.h"
#include "PulseCannon.h"
#include "Cannon.h"
#include "GaussCannon.h"
#include "BeamCannon.h"
#include "ActorHandler.h"
#include "../gfx/Particle.h"

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

Tank::Tank(int networkId, TankTypes tankType, utilities::Position pos, utilities::Direction dir, Actor::PlayerMovingInfo playerMovingInfo) : Unit(pos, dir, 0.0f, networkId, utilities::MapCoordinate(3,2))
{
	mActorType = ActorType_Tank;
	mTankType = tankType;
	mFireSecondary = false;
	mCooldownStartTime = 0.0f;
	mSwitchCooldown = 10.0f;
	mCooldownCounter = 0.0;
	mGodMode = false;

	mMoving = playerMovingInfo.mMoving;
	mMovingDirection = playerMovingInfo.mMovingDirection;
	mRotating = playerMovingInfo.mRotating;
	mRotateDirection = playerMovingInfo.mRotatingDirection;

	mpGlobalTimer = logic::GlobalTimer::getInstance();
	
	if(tankType == TankType_CloseCombat)
	{
		mArmor			 = ActorConstants::CLOSE_COMBAT_TANK_ARMOR;
		mHealthPoints	 = ActorConstants::CLOSE_COMBAT_TANK_HP;
		mHealthPointsMax = ActorConstants::CLOSE_COMBAT_TANK_HP;
		mRotateSpeed	 = ActorConstants::CLOSE_COMBAT_TANK_ROTATE_SPEED;
		mAcceleration	 = ActorConstants::CLOSE_COMBAT_TANK_ACCELERATION;
		mMaxSpeed		 = ActorConstants::CLOSE_COMBAT_TANK_MAX_SPEED;
		mFriction		 = ActorConstants::CLOSE_COMBAT_TANK_FRICTION_HIGH_SPEED;

		mScale = utilities::Vec3Float(1.5f, 3.5f, 1.5f);
		mpTransformObject = mpsDXModule->createMeshInstanced(logic::ModelInfo::getInstance()->getModelName(mActorType, mTankType));
		dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setFlagUpdateAbsolute(false);
		mpTransformObject->setID(mId.convertToGfxId());
		mpTransformObject->setName("CloseRangeTank");
		mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
		dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(mScale.convertToD3DXVECTOR3());
		mpTransformObject->setActor(this);

		for(std::vector<gfx::InstanceSubset *>::const_iterator instanceIter = ((gfx::InstanceMesh *)mpTransformObject)->getSubsetList().begin();
			instanceIter != ((gfx::InstanceMesh *)mpTransformObject)->getSubsetList().end(); ++instanceIter)
		{
			(*instanceIter)->setLogicBoundingVolume((*instanceIter)->getBoundingVolume()->clone());
			if(instanceIter - ((gfx::InstanceMesh *)mpTransformObject)->getSubsetList().begin() == 0)
			{
				mpPrePosCollisionObject = (*instanceIter)->getLogicBoundingVolume();
			}
		}

		mpWeapon = myNew Minigun();
		mpPedestrianWeapon = myNew PedestrianGun();
	}
	else
	{
		mArmor			 = ActorConstants::LONG_RANGE_TANK_ARMOR;
		mHealthPoints	 = ActorConstants::LONG_RANGE_TANK_HP;
		mHealthPointsMax = ActorConstants::LONG_RANGE_TANK_HP;
		mRotateSpeed	 = ActorConstants::LONG_RANGE_TANK_ROTATE_SPEED;
		mAcceleration	 = ActorConstants::LONG_RANGE_TANK_ACCELERATION;
		mMaxSpeed		 = ActorConstants::LONG_RANGE_TANK_MAX_SPEED;
		mFriction		 = ActorConstants::LONG_RANGE_TANK_FRICTION_HIGH_SPEED;

		mScale = utilities::Vec3Float(2.5f, 3.5f, 2.5f);
		mpTransformObject = mpsDXModule->createMeshInstanced(logic::ModelInfo::getInstance()->getModelName(mActorType, mTankType));
		dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setFlagUpdateAbsolute(false);
		mpTransformObject->setID(mId.convertToGfxId());
		mpTransformObject->setName("LongRangeTank");
		mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
		dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(mScale.convertToD3DXVECTOR3());
		mpTransformObject->setActor(this);
		
		for(std::vector<gfx::InstanceSubset *>::const_iterator instanceIter = ((gfx::InstanceMesh *)mpTransformObject)->getSubsetList().begin();
			instanceIter != ((gfx::InstanceMesh *)mpTransformObject)->getSubsetList().end(); ++instanceIter)
		{
			(*instanceIter)->setLogicBoundingVolume((*instanceIter)->getBoundingVolume()->clone());
			if(instanceIter - ((gfx::InstanceMesh *)mpTransformObject)->getSubsetList().begin() == 0)
			{
				mpPrePosCollisionObject = (*instanceIter)->getLogicBoundingVolume();
			}
		}

		mpWeapon = myNew Cannon();
		mpPedestrianWeapon = myNew PedestrianGun();
	}


// ------------------------------
// AI Section BEGIN
// ------------------------------
#ifdef USE_AI
	// Add the tank to the AiFactory
	ai::AiFactory* pAiFactory = ai::AiFactory::getInstance();
	if (pAiFactory != NULL)
	{
		pAiFactory->addTank(this);
	}
#endif
// ------------------------------
// AI Section END
// ------------------------------
}


Tank::~Tank()
{
	SAFE_DELETE(mpWeapon);
    SAFE_DELETE(mpPedestrianWeapon);

// ------------------------------
// AI Section BEGIN
// ------------------------------
#ifdef USE_AI
	// Remove the tank from the factory
	ai::AiFactory* pAiFactory = ai::AiFactory::getInstance();
	if (pAiFactory != NULL)
	{
		pAiFactory->removeTank(getKey());
	}
#endif
// ------------------------------
// AI Section END
// ------------------------------
}

void Tank::tankDestroyed()
{
	PlayerMovingInfo info;
	info.mMoving = this->mMoving;
	info.mRotating = this->mRotating;
	info.mMovingDirection = this->mMovingDirection;
	info.mRotatingDirection = this->mRotateDirection;
	mpsDXModule->getParticles("TankExplosion")->addEmitter(mPosition.convertToD3DXVECTOR3());
	ActorHandler::getInstance()->playerDied(mId.getOwnerId(), info);
	safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateDriver(mPosition, mDirection, info)));
	safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_InputChangeGameState(logic::InputStates::GameState_Game_Outside_Tank)));
}

void Tank::reduceHp(float damage)
{	
	if (!mGodMode)
	{
		float finalDamage = damage - mArmor;
		mHealthPoints -= finalDamage;
		DEBUG_MESSAGE(LEVEL_HIGHEST, "HP: " << mHealthPoints);

		if(mHealthPoints <= 0 && !mDead && mId.isOwner())
		{
			DEBUG_MESSAGE(LEVEL_MEDIUM, "Lol i die ");
			safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(this->getKey(), -3)));
			DEBUG_MESSAGE(LEVEL_LOWEST, "tankreducehp");
			tankDestroyed();
			mDead = true;
			mFire = false;
		}
	}
}

void Tank::fireMain() 
{
	mpWeapon->fireWeapon(mId);
}


void Tank::firePedestrianGun()
{
	mpPedestrianWeapon->fireWeapon(mId);
}

void Tank::update(float deltaTime)
{
	if(mPosition.y != 0.0f)
	{
		mPosition.y = 0.0f;
		mDirection.y = 0.0f;

	}
	if(mFire && mId.isOwner())
	{
		fireMain();
	}
	if(mFireSecondary && mId.isOwner())
	{
		firePedestrianGun();
	}
	calculateSpeed(deltaTime);

	if( fabsf(mSpeed) > 0.0f)
	{
		mPrePosition = mPosition;
		mPosition = mPosition + (mSpeed * mDirection * deltaTime);
		
	}

	if(mRotating)
	{
		mPreDirection = mDirection;
		mDirection.rotateAroundY(mRotateSpeed * deltaTime * mRotateDirection);
		mDirection.normalize();
		mSubsetDirection.rotateAroundY(mRotateSpeed * deltaTime * mRotateDirection);
		mSubsetDirection.normalize();

		float angle = mDirection.getZXAngleClockwise();
		mpPrePosCollisionObject->rotateY(angle);
	}
	if(mSubsetRotating)
	{
		mSubsetDirection.rotateAroundY(mRotateSpeed * deltaTime * mSubsetRotateDirection);
		mSubsetDirection.normalize();
	}
	mpPrePosCollisionObject->setPosition(mPosition.convertToD3DXVECTOR3());
}

void Tank::handleEventId(EventData const &eventMessage)
{	
	if(EvtData_To_TankStartFire::msEventType == eventMessage.getEventType())
	{
		mFire = true;
	}
	else if(EvtData_To_TankStopFire::msEventType == eventMessage.getEventType())
	{
		mFire = false;
	}
	if(EvtData_To_TankStartFireSecondary::msEventType == eventMessage.getEventType())
	{
		mFireSecondary = true;
	}
	else if(EvtData_To_TankStopFireSecondary::msEventType == eventMessage.getEventType())
	{
		mFireSecondary = false;
	}
	else
		Actor::handleEventId(eventMessage);
}

void Tank::setWeapon(Weapon::WeaponTypes weapon)
{
	mCooldownCounter = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds) - mCooldownStartTime;
	if(mCooldownCounter > mSwitchCooldown)
	{	
	if(mTankType == TankType_CloseCombat && weapon != Weapon::WeaponTypes_Cannon && weapon != Weapon::WeaponTypes_GaussCannon && weapon != Weapon::WeaponTypes_BeamCannon)
	{
		SAFE_DELETE(mpWeapon);
		mCooldownStartTime = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds);
		if(weapon == Weapon::WeaponTypes_Minigun)
			mpWeapon = myNew Minigun();
		else if(weapon == Weapon::WeaponTypes_Flamethrower)
			mpWeapon = myNew Flamethrower();
		else if(weapon == Weapon::WeaponTypes_PulseCannon)
			mpWeapon = myNew PulseCannon();
	}
	else if(mTankType == TankType_LongRange && weapon != Weapon::WeaponTypes_Minigun && weapon != Weapon::WeaponTypes_Flamethrower && weapon != Weapon::WeaponTypes_PulseCannon)
	{
		SAFE_DELETE(mpWeapon);
		mCooldownStartTime = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds);
	    if(weapon == Weapon::WeaponTypes_Cannon)
			mpWeapon = myNew Cannon();
		else if(weapon == Weapon::WeaponTypes_GaussCannon)
			mpWeapon = myNew GaussCannon();
		else if(weapon == Weapon::WeaponTypes_BeamCannon)
			mpWeapon = myNew BeamCannon();
	}
	}
}

void Tank::calculateSpeed(float deltaTime)
{
		if(mMoving)
		{
			if(mSpeed >= 0.0f && mSpeed < mMaxSpeed)
			{
				if(mMovingDirection == 1)
				{
					mSpeed += (mMaxSpeed) * deltaTime;
				}
				else if(mMovingDirection == -1)
				{
					mSpeed -= (mAcceleration - mFriction) * deltaTime;
				}
			}
			else if(mSpeed <= 0.0f && fabsf(mSpeed) < mMaxSpeed)
			{
				if(mMovingDirection == -1)
				{
					mSpeed -= (mAcceleration) * deltaTime;
				}
				else if(mMovingDirection == 1)
				{
					mSpeed += (mAcceleration - mFriction) * deltaTime;
				}
			}
		}
		else if(fabsf(mSpeed) > 0.0f)
		{
			if(mSpeed > 0.0f)
			{
				mSpeed += mFriction * deltaTime;
				if(mSpeed < 0.0f)
				{
					mSpeed = 0.0f;
				}
			}
			else if (mSpeed < 0.0f)
			{
				mSpeed -= mFriction * deltaTime;
				if(mSpeed > 0.0f)
				{
					mSpeed = 0.0f;
				}
			}
		}
	
}

Tank::TankTypes Tank::getTankType()
{
	return mTankType;
}

Weapon* Tank::getWeapon()
{
	return mpWeapon;
}

float Tank::getSelectionCooldownPercentage() const
{
	float time = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds) - mCooldownStartTime;
	return 1.0f - time / mSwitchCooldown;
}

bool Tank::getFiring()
{
	return mFire;
}

std::istream& operator>>(std::istream &in, logic::Tank::TankTypes& tankType)
{
	int intTankType;
	in >> intTankType;
	tankType = static_cast<logic::Tank::TankTypes>(intTankType);
	return in;
}


