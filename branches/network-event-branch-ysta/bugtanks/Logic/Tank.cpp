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
 * This is the tank class, it will provide functions and hold information needed for the tank
 */

#include "Tank.h"


using namespace logic;

const float TANK_ROTATE_SPEED			= 1.0f;
const float TANK_ACCELERATION			= 10.0f;
const float TANK_FRICTION_HIGH_SPEED	= -20.0f;
//const float TANK_FRICTION_LOW_SPEED		= 
const float TANK_HIGH_SPEED_THRESHOLD	= 10.0f;
const float TANK_MAX_SPEED				= 50.0f;

Tank::Tank(double hp,int armor,TankTypes tankType)
{
	mHealthPoints = hp;
	mArmor = armor;
	mTankType = tankType;
	mFire = false;
	if(tankType == TankType_CloseCombat)
		mpWeapon = myNew Minigun(5,0.1f,1,1);
	else
		mpWeapon = myNew Cannon(50,1.0f,4,3,1,5);

	mActorType = ActorType_Tank;

	mSpeed = 0.0f;
}


Tank::~Tank()
{
	SAFE_DELETE(mpWeapon);
}


void Tank::tankDestroyed()
{
	//send event to GFX to do some cool explosions when the tank gets destroyed
	//send event to GFX to render a driver model and have him thrown abit away
	//change state to outsideTank, meaning input will now move driver and position will be updated for driver
}

void Tank::reduceHp(double damage)
{
	double finalDamage = damage - mArmor;
	mHealthPoints -= finalDamage;
	DEBUG_MESSAGE("HP: " << mHealthPoints);
	if(mHealthPoints <= 0)
	{
		DEBUG_MESSAGE("Lol i die ");
		tankDestroyed();
		safeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(this->getKey(), GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))));
	}
}

void Tank::fireMain() 
{
	//send event with information about position and direction you are firing in to Collision
	mpWeapon->fireWeapon();
	
}


void Tank::firePedestrianGun()
{
	//send event with information about position and direction you are firing in to Collision 
}

void Tank::update(float deltaTime)
{
	//position will not be updated for tank while gamestate is outsideTank
	if(mFire)
	{
		fireMain();
	}

	calculateSpeed(deltaTime);

	if( fabsf(mSpeed) > 0.0f)
	{	

		Actor::mPosition.x += (mSpeed * Actor::mDirection.x * deltaTime);
		Actor::mPosition.y += (mSpeed * Actor::mDirection.y * deltaTime);
		Actor::mPosition.z += (mSpeed * Actor::mDirection.z * deltaTime);

	}
	if(mRotating)
	{
		mDirection.rotateAroundY(TANK_ROTATE_SPEED * deltaTime * mRotateDirection);
		mDirection.normalize();
		mSubsetDirection.rotateAroundY(TANK_ROTATE_SPEED * deltaTime * mRotateDirection);
		mSubsetDirection.normalize();
	}
	if(mSubsetRotating)
	{
		mSubsetDirection.rotateAroundY(TANK_ROTATE_SPEED * deltaTime * mSubsetRotateDirection);
		mSubsetDirection.normalize();
	}
	
}

void Tank::handleEventId(EventData const &eventMessage)
{	
	if(EvtData_To_TankStartFire::mEventType == eventMessage.getEventType())
	{
		mFire = true;
	}
	else if(EvtData_To_TankStopFire::mEventType == eventMessage.getEventType())
	{
		mFire = false;
	}
	else
		Actor::handleEventId(eventMessage);
}

void Tank::setWeapon(int weaponId)
{
	if(weaponId == Weapon_Minigun)
		mpWeapon = myNew Minigun(5,0.2f,1,1);
	if(weaponId == Weapon_Flamethrower)
		mpWeapon = myNew Flamethrower(10,0.1f,2,1,5);
	if(weaponId == Weapon_PulseCannon)
		mpWeapon = myNew PulseCannon(10,4.0f,3,1);
	if(weaponId == Weapon_Cannon)
		mpWeapon = myNew Cannon(50,1.0f,4,3,1,5);
	if(weaponId == Weapon_GaussCannon)
		mpWeapon = myNew GaussCannon(10,3.0f,5,3);
	if(weaponId == Weapon_BeamCannon)
		mpWeapon = myNew BeamCannon(10,4.0f,6,3);
}

void Tank::calculateSpeed(float deltaTime)
{
	/*float maxSpeed;
	float acceleration;
	float friction;
	if(mTankType == TankType_LongRange)
	{
		maxSpeed = TANK_MAX_SPEED;
		acceleration = TANK_ACCELERATION;
		friction = TANK_FRICTION_HIGH_SPEED;*/
		if(mMoving)
		{
			if(mSpeed >= 0.0f && mSpeed < TANK_MAX_SPEED)
			{
				if(mForward == 1)
				{
					mSpeed += (TANK_ACCELERATION) * deltaTime;
				}
				else if(mForward == -1)
				{
					mSpeed -= (TANK_ACCELERATION - TANK_FRICTION_HIGH_SPEED) * deltaTime;
				}
			}
			else if(mSpeed <= 0.0f && fabsf(mSpeed) < TANK_MAX_SPEED)
			{
				if(mForward == -1)
				{
					mSpeed -= (TANK_ACCELERATION) * deltaTime;
				}
				else if(mForward == 1)
				{
					mSpeed += (TANK_ACCELERATION - TANK_FRICTION_HIGH_SPEED) * deltaTime;
				}
			}
		}
		else if(fabsf(mSpeed) > 0.0f)
		{
			if(mSpeed > 0.0f)
			{
				mSpeed += TANK_FRICTION_HIGH_SPEED * deltaTime;
				if(mSpeed < 0.0f)
				{
					mSpeed = 0.0f;
				}
			}
			else if (mSpeed < 0.0f)
			{
				mSpeed -= TANK_FRICTION_HIGH_SPEED * deltaTime;
				if(mSpeed > 0.0f)
				{
					mSpeed = 0.0f;
				}
			}
		}
	//}
	/*else if(mTankType == TankType_CloseCombat)
	{
		maxSpeed = TANK_MAX_SPEED;
		acceleration = TANK_ACCELERATION;
		friction = TANK_FRICTION_HIGH_SPEED;
		if(mMoving)
		{
			if(mSpeed >= 0.0f && mSpeed < maxSpeed)
			{
				if(mForward)
				{
					mSpeed += (acceleration) * deltaTime;
				}
				else
				{
					mSpeed -= (acceleration - friction) * deltaTime;
				}
			}
			else if(mSpeed <= 0.0f && fabsf(mSpeed) < maxSpeed)
			{
				if(mForward)
				{
					mSpeed += (acceleration - friction) * deltaTime;
				}
				else
				{
					mSpeed -= (acceleration) * deltaTime;
				}
			}
		}
		else if(fabsf(mSpeed) > 0.0f)
		{
			if(mSpeed > 0.0f)
			{
				mSpeed += friction * deltaTime;
				if(mSpeed < 0.0f)
				{
					mSpeed = 0.0f;
				}
			}
			else if (mSpeed < 0.0f)
			{
				mSpeed -= friction * deltaTime;
				if(mSpeed > 0.0f)
				{
					mSpeed = 0.0f;
				}
			}
		}
	}*/
}