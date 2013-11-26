/**
 * @file
 * @author Viktor Svensson <arkangel88@gmail.com>
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
 * An event listener for the sound component.
 */



#include "SoundListener.h"
#include "../Logic/InputStates.h"
#include "../Logic/Events.h"
#include "../Logic/PlayerInfo.h"
#include "../Logic/ActorHandler.h"
#include "../Logic/Actor.h"
#include "../Logic/Tank.h"

using namespace sound;

SoundListener::SoundListener()
{
	mpActorHandler = logic::ActorHandler::getInstance();
	mPlayer = logic::PlayerInfo::getInstance();
	mSoundSystem = sound::SoundSystem::getInstance();

	mCollisionHouseChanger = 0;
}

SoundListener::~SoundListener()
{
}

bool SoundListener::handleEvent(const logic::EventData &mEvent)
{
	
	/*

	// *** Moved to SoundSystem ***

	if(logic::EvtData_To_ActorStartMove::msEventType == mEvent.getEventType()) 
	{
		const logic::EvtData_To_ActorStartMove& evtData = static_cast<const logic::EvtData_To_ActorStartMove&>(mEvent);
		if(logic::Actor::ActorType_Tank == logic::ActorHandler::getInstance()->getActor(evtData.mId)->getActorType())
		{
			//TODO: change soundID
			mSoundSystem->playSound(sound::Sound_PlayerTankLongRangeMovementMoving, logic::ActorHandler::getInstance()->getActor(evtData.mId)->getKey(), 0); 
		}
	}

	if(logic::EvtData_To_ActorStopMove::msEventType == mEvent.getEventType()) 
	{
		const logic::EvtData_To_ActorStopMove& evtData = static_cast<const logic::EvtData_To_ActorStopMove&>(mEvent);
		if(logic::Actor::ActorType_Tank == logic::ActorHandler::getInstance()->getActor(evtData.mId)->getActorType())
		{
			mSoundSystem->SoundOff(logic::ActorHandler::getInstance()->getActor(evtData.mId)->getKey(), 0);
		}
	}*/

	//Create new channels for the new pActor
	if(logic::EvtData_From_ActorCreated::msEventType == mEvent.getEventType()) 
	{
		const logic::EvtData_From_ActorCreated& evtData = static_cast<const logic::EvtData_From_ActorCreated&>(mEvent);
		mSoundSystem->createChannel(evtData.mId);

		logic::Actor* pActor = mpActorHandler->getActor(evtData.mId);

		if( pActor->getActorType() == logic::Actor::ActorType_Enemy )
		{
			logic::Enemy* pEnemy = dynamic_cast<logic::Enemy*>(pActor);
			// Handle error
			if (pEnemy == NULL)
			{
				ERROR_MESSAGE("Could not dynamic_cast actor into pEnemy!");
				return false;
			}

			if( pEnemy->getEnemySize() == logic::Enemy::Size_Small )
			{
				mSoundSystem->playSound(sound::Sound_EnemyAntSpawn, evtData.mId, Channel_Rotation);
			}
			else if( pEnemy->getEnemySize() == logic::Enemy::Size_Medium )
			{
				mSoundSystem->playSound(sound::Sound_EnemySpiderSpawn, evtData.mId, Channel_Rotation);
			}
			else if( pEnemy->getEnemySize() == logic::Enemy::Size_Large )
			{
				mSoundSystem->playSound(sound::Sound_EnemyBeetleSpawn, evtData.mId, Channel_Rotation);
			}
		}
	}

	//Plays a sound where the actor died and then stops its channels
	else if(logic::EvtData_From_ActorDestroyed::msEventType == mEvent.getEventType()) 
	{

		const logic::EvtData_From_ActorDestroyed& evtData = static_cast<const logic::EvtData_From_ActorDestroyed&>(mEvent);
		logic::Actor* actor = mpActorHandler->getActor(evtData.mId);

		if(actor->getActorType() == logic::Actor::ActorType_Pedestrian)
		{
			utilities::Position pos = actor->getPosition();
			mSoundSystem->playSoundOnce(sound::Sound_CivilianDeath, pos.x, pos.y, pos.z); 
		}

		else if(actor->getActorType() == logic::Actor::ActorType_Enemy)
		{
			logic::Enemy::Sizes size = dynamic_cast<logic::Enemy*>(actor)->getEnemySize();
			utilities::Position pos = actor->getPosition();

			if(size == logic::Enemy::Size_Small)
			{
  				mSoundSystem->playSoundOnce(sound::Sound_EnemyAntDeath, pos.x, pos.y, pos.z);
			}

			else if(size == logic::Enemy::Size_Medium)
			{
				mSoundSystem->playSoundOnce(sound::Sound_EnemySpiderDeath, pos.x, pos.y, pos.z);
			}

			else if(size == logic::Enemy::Size_Large)
			{
				mSoundSystem->playSoundOnce(sound::Sound_EnemyBeetleDeath, pos.x, pos.y, pos.z);
			}
		}

		else if(actor->getActorType() == logic::Actor::ActorType_Driver)
		{
			utilities::Position pos = actor->getPosition();
			mSoundSystem->playSoundOnce(sound::Sound_PlayerFootDeath, pos.x, pos.y, pos.z);
		}

		else if(actor->getActorType() == logic::Actor::ActorType_Tank)
		{
			utilities::Position pos = actor->getPosition();
			mSoundSystem->playSoundOnce(sound::Sound_PlayerTankDeath, pos.x, pos.y, pos.z);
		}



		mSoundSystem->stopChannel(evtData.mId);
		
	}

	//Finds out what pActor is shooting and plays a sounds based on what weapon is equiped
	else if( logic::EvtData_From_WeaponFired::msEventType == mEvent.getEventType() ) 
	{

		const logic::EvtData_From_WeaponFired& evtData = static_cast<const logic::EvtData_From_WeaponFired&>(mEvent);
		logic::Actor* pActor = mpActorHandler->getActor(evtData.mActorId);

		
		if( pActor != NULL && pActor->getActorType() == logic::Actor::ActorType_Tank ) 
		{
			logic::Weapon* weapon = dynamic_cast<logic::Tank*> (pActor)->getWeapon();

			/*
			//Minigun handled in SoundSystem
			if(weapon->getWeaponType() == logic::Weapon::WeaponTypes_Minigun) 
			{

				if( mSoundSystem->mChannels[mSoundSystem->findActor(pActor->getKey())].mFire == true )
				{
					mSoundSystem->playSound(sound::Sound_WeaponMinigunFire, pActor->getKey(), Channel_Attack);
				}
				else
				{
					mSoundSystem->SoundOff(pActor->getKey(), Channel_Attack);
					mSoundSystem->playSound(sound::Sound_WeaponMinigunEnd, pActor->getKey(), Channel_Attack);
				}
			}*/
			if(weapon->getWeaponType() == logic::Weapon::WeaponTypes_Cannon)
			{
				mSoundSystem->playSound(sound::Sound_WeaponCannonFire, pActor->getKey(), Channel_Attack);
			}
			else if(weapon->getWeaponType() == logic::Weapon::WeaponTypes_GaussCannon)
			{
				mSoundSystem->playSound(sound::Sound_WeaponGausscannonFire, pActor->getKey(), Channel_Attack);
			}
			else if(weapon->getWeaponType() == logic::Weapon::WeaponTypes_PedestrianGun) 
			{
				mSoundSystem->playSound(sound::Sound_WeaponPedestrianGunFire, pActor->getKey(), Channel_Attack);
			}
			else if(weapon->getWeaponType() == logic::Weapon::WeaponTypes_BeamCannon)
			{
				mSoundSystem->playSound(sound::Sound_WeaponBeamcannonFire, pActor->getKey(), Channel_Attack);
			}
			else if(weapon->getWeaponType() == logic::Weapon::WeaponTypes_Flamethrower)
			{
				//if(!mSoundSystem->isPlaying(pActor->getKey(), Channel_Attack))
				//{
					mSoundSystem->playSound(sound::Sound_WeaponFlamethrowerFire, pActor->getKey(), Channel_Attack);
				//}
			}
			else if(weapon->getWeaponType() == logic::Weapon::WeaponTypes_PulseCannon)
			{
				mSoundSystem->playSound(sound::Sound_WeaponPulsecannonFire, pActor->getKey(), Channel_Attack);
			}
		}
		//TODO: Is this event called even when enemies shoot? 
		else if( pActor != NULL && pActor->getActorType() == logic::Actor::ActorType_Enemy )
		{
			logic::Enemy* pEnemy = dynamic_cast<logic::Enemy*>(pActor);

			if (pEnemy == NULL)
			{
				ERROR_MESSAGE("Could not dynamic_cast pActor into pEnemy");
				return false;
			}

			if( pEnemy->getEnemySize() == logic::Enemy::Size_Small ) 
			{
				mSoundSystem->playSound(sound::Sound_EnemyAntAttackRanged, pActor->getKey(), Channel_Attack);
			}
			else if( pEnemy->getEnemySize() == logic::Enemy::Size_Medium )
			{
				mSoundSystem->playSound(sound::Sound_EnemySpiderAttackRanged, pActor->getKey(), Channel_Attack);
			}
			else if( pEnemy->getEnemySize() == logic::Enemy::Size_Large )
			{
				mSoundSystem->playSound(sound::Sound_EnemyBeetleAttackRanged, pActor->getKey(), Channel_Attack);
			}
		}
		else
		{
			mSoundSystem->playSound(sound::Sound_WeaponShotgunFire, pActor->getKey(), Channel_Attack);
		}
	}

	/*
	// *** Moved to SoundSystem ***
	if(logic::EvtData_To_ActorStartRotate::msEventType == mEvent.getEventType()) //TODO: check for more actors
	{
		//const logic::EvtData_To_ActorStartRotate& evtData = static_cast<const logic::EvtData_To_ActorStartRotate&>(mEvent);
		mSoundSystem->playSound(sound::Sound_PlayerTankLongRangeMovementMoving, logic::ActorHandler::getInstance()->getActor(logic::PlayerInfo::getInstance()->getPlayerId())->getKey(), Channel_Rotation); 
		
	}

	if(logic::EvtData_To_ActorStopRotate::msEventType == mEvent.getEventType()) //TODO: check for more actors
	{
		const logic::EvtData_To_ActorStopRotate& evtData = static_cast<const logic::EvtData_To_ActorStopRotate&>(mEvent);
		//if(logic::Actor::ActorType_Tank == logic::ActorHandler::getInstance()->getActor(evtData.mId)->getActorType())
		//{
			mSoundSystem->SoundOff(logic::ActorHandler::getInstance()->getActor(evtData.mId)->getKey(), Channel_Rotation);
		//}
		
	}*/

	else if( logic::EvtData_From_EnemyHit::msEventType == mEvent.getEventType() ) 
	{
		const logic::EvtData_From_EnemyHit& evtData = static_cast<const logic::EvtData_From_EnemyHit&>(mEvent);
		logic::Actor* pShooter = mpActorHandler->getActor(evtData.mShooter);
		logic::Weapon* pWeapon = dynamic_cast<logic::Tank*> (pShooter)->getWeapon();
		utilities::Position pos = mpActorHandler->getActor(evtData.mEnemy)->getPosition();
		logic::Enemy* pEnemy = dynamic_cast<logic::Enemy*>(mpActorHandler->getActor(evtData.mEnemy));

		assert(pShooter != NULL || pWeapon != NULL || pEnemy != NULL);
		
		if(pWeapon->getWeaponType() == logic::Weapon::WeaponTypes_Minigun)
		{
			mSoundSystem->playSoundOnce(sound::Sound_WeaponMinigunImpact, pos.x, pos.y, pos.z); 
		}
		else if(pWeapon->getWeaponType() == logic::Weapon::WeaponTypes_Cannon)
		{
			mSoundSystem->playSoundOnce(sound::Sound_WeaponCannonImpact, pos.x, pos.y, pos.z); 
		}
		else if(pWeapon->getWeaponType() == logic::Weapon::WeaponTypes_GaussCannon)
		{
			mSoundSystem->playSoundOnce(sound::Sound_WeaponGausscannonImpact, pos.x, pos.y, pos.z); 
		}
		//else if(pWeapon->getWeaponType() == logic::Weapon::WeaponTypes_PedestrianGun) 
		//{
		//	mSoundSystem->playSoundOnce(sound::Sound_WeaponPedestrianGunImpact, pos.x, pos.y, pos.z); 
		//}
		//else if(pWeapon->getWeaponType() == logic::Weapon::WeaponTypes_BeamCannon)
		//{
		//	mSoundSystem->playSoundOnce(sound::Sound_WeaponBeamcannonImpact, pos.x, pos.y, pos.z); 
		//}
		//else if(pWeapon->getWeaponType() == logic::Weapon::WeaponTypes_Flamethrower)
		//{
		//	mSoundSystem->playSoundOnce(sound::Sound_WeaponFlamethrowerImpact, pos.x, pos.y, pos.z); 
		//}
		//else if(pWeapon->getWeaponType() == logic::Weapon::WeaponTypes_PulseCannon)
		//{
		//	mSoundSystem->playSoundOnce(sound::Sound_WeaponPulsecannonImpact, pos.x, pos.y, pos.z); 
		//}
		else if(pWeapon->getWeaponType() == logic::Weapon::WeaponTypes_Shotgun)
		{
			mSoundSystem->playSoundOnce(sound::Sound_WeaponShotgunImpact, pos.x, pos.y, pos.z); 
		}

		if( pEnemy->getEnemySize() == logic::Enemy::Size_Small )
		{
			mSoundSystem->playSound(sound::Sound_EnemyAntHit, evtData.mEnemy, Channel_Rotation);
		}
		else if( pEnemy->getEnemySize() == logic::Enemy::Size_Medium )
		{
			mSoundSystem->playSound(sound::Sound_EnemySpiderHit, evtData.mEnemy, Channel_Rotation);
		}
		else if( pEnemy->getEnemySize() == logic::Enemy::Size_Large )
		{
			mSoundSystem->playSound(sound::Sound_EnemyBeetleHit, evtData.mEnemy, Channel_Rotation);
		}
	}

	else if( logic::EvtData_From_ActorCollisionObject::msEventType == mEvent.getEventType() ) //TODO: Not ready. Not at all. 
	{
		const logic::EvtData_From_ActorCollisionObject& evtData = static_cast<const logic::EvtData_From_ActorCollisionObject&>(mEvent);
		logic::Actor* pFirstActor = mpActorHandler->getActor(evtData.mFirstActorId);
		logic::Actor* pSecondActor = mpActorHandler->getActor(evtData.mSecondActorId);

		if( pFirstActor != NULL && pSecondActor != NULL )
		{
			if( pFirstActor->getActorType() == logic::Actor::ActorType_Tank )
			{
				if( pSecondActor->getActorType() == logic::Actor::ActorType_Enemy )
				{
					if(!mSoundSystem->isPlaying(pSecondActor->getKey(), Channel_Rotation))
					{
						utilities::Position pos = pSecondActor->getPosition();
						mSoundSystem->playSound(sound::Sound_PlayerTankCollisionLargeEnemy, pSecondActor->getKey(), Channel_Rotation);
					}
				}
				else if( pSecondActor->getActorType() == logic::Actor::ActorType_Munition )
				{
					//TODO: Fix new sound
					utilities::Position pos = pSecondActor->getPosition();
					mSoundSystem->playSoundOnce(sound::Sound_PlayerTankHitMelee, pos.x, pos.y, pos.z); 
				}
				else if( pSecondActor->getActorType() == logic::Actor::ActorType_StaticEnvironment )
				{
					if( !mSoundSystem->isPlaying(pSecondActor->getKey(), Channel_Movement) )
					{
						if( mCollisionHouseChanger == 0 )
						{
							mSoundSystem->playSound(sound::Sound_PlayerTankCollisionHouse1, pSecondActor->getKey(), Channel_Movement);
							mCollisionHouseChanger++;
						}
						else
						{
							mSoundSystem->playSound(sound::Sound_PlayerTankCollisionHouse2, pSecondActor->getKey(), Channel_Movement);
							mCollisionHouseChanger = 0;
						}
					}
				}
				else if( pSecondActor->getActorType() == logic::Actor::ActorType_Tank )
				{
					utilities::Position pos = pSecondActor->getPosition();
					mSoundSystem->playSoundOnce(sound::Sound_PlayerTankCollisionTank, pos.x, pos.y, pos.z);
				}
			}

			else if( pFirstActor->getActorType() == logic::Actor::ActorType_Driver )
			{
				if( pSecondActor->getActorType() == logic::Actor::ActorType_Munition )
				{
					utilities::Position pos = pSecondActor->getPosition();
					mSoundSystem->playSoundOnce(sound::Sound_PlayerFootHit, pos.x, pos.y, pos.z);
				}
			}

			else if( pFirstActor->getActorType() == logic::Actor::ActorType_Munition ) //TODO: Does not work. 
			{
				logic::Weapon::WeaponTypes weapon = dynamic_cast<logic::Munitions*>(pFirstActor)->getWeaponType();
				utilities::Position pos = pFirstActor->getPosition();

				//if( weapon == logic::Weapon::WeaponTypes_Minigun )
				//{
				//	mSoundSystem->playSound(sound::Sound_WeaponMinigunImpact, pSecondActor->getKey(), Channel_Attack);
				//}
				//else if( weapon == logic::Weapon::WeaponTypes_Flamethrower )
				//{
				//	mSoundSystem->playSound(sound::Sound_WeaponFlamethrowerImpact, pSecondActor->getKey(), Channel_Attack);
				//}
				if( weapon == logic::Weapon::WeaponTypes_PulseCannon )
				{
					mSoundSystem->playSound(sound::Sound_WeaponPulsecannonImpact, pSecondActor->getKey(), Channel_Attack);
				}
				else if( weapon == logic::Weapon::WeaponTypes_Cannon )
				{
					mSoundSystem->playSound(sound::Sound_WeaponCannonImpact, pFirstActor->getKey(), Channel_Attack);
					//mSoundSystem->playSoundOnce(sound::Sound_WeaponCannonImpact, pos.x, pos.y,  pos.z);
				}
				else if( weapon == logic::Weapon::WeaponTypes_GaussCannon )
				{
					mSoundSystem->playSound(sound::Sound_WeaponGausscannonImpact, pSecondActor->getKey(), Channel_Attack);
				}
				else if( weapon == logic::Weapon::WeaponTypes_BeamCannon )
				{
					mSoundSystem->playSound(sound::Sound_WeaponBeamcannonImpact, pSecondActor->getKey(), Channel_Attack);
				}


			}
			//else if( pFirstActor->getActorType() == logic::Actor::ActorType_Enemy )
			//{
				//if( pSecondActor->getActorType() == logic::Actor::ActorType_Munition )
				//{
					//TODO: Enemy hit sound
				//}
				//else if( pSecondActor->getActorType() == logic::Actor::ActorType_StaticEnvironment )
				//{
					//TODO: Remove ? 
					//utilities::Position pos = pSecondActor->getPosition();
					//mSoundSystem->playSoundOnce(sound::Sound_PlayerTankCollisionHouse1, pos.x, pos.y, pos.z);
				//}
				//else if( pSecondActor->getActorType() == logic::Actor::ActorType_Tank )
				//{
					// Remove ?
					//utilities::Position pos = pSecondActor->getPosition();
					//mSoundSystem->playSoundOnce(sound::Sound_PlayerTankCollisionLargeEnemy, pos.x, pos.y, pos.z);
				//}
			//}
		}
	}

	/* *** Events not used. ***

	//Plays sound when the driver dies
	else if( logic::EvtData_From_DriverDeath::msEventType == mEvent.getEventType() ) 
	{
		const logic::EvtData_From_DriverDeath& evtData = static_cast<const logic::EvtData_From_DriverDeath&>(mEvent);
		mSoundSystem->playSound(sound::Sound_PlayerFootDeath, evtData., Channel_Movement);
	}

	//Plays sound when the driver is hit
	else if( logic::EvtData_From_DriverHit::msEventType == mEvent.getEventType() ) //TODO: fix for multiple players
	{
		const logic::EvtData_From_DriverHit& evtData = static_cast<const logic::EvtData_From_DriverHit&>(mEvent);
		mSoundSystem->playSound(sound::Sound_PlayerFootHit, mPlayer->getPlayerId(), Channel_Movement);
	}
	*/

	//Plays a sound when the driver or a civilian enters a tank
	else if( logic::EvtData_From_DriverEnterTank::msEventType == mEvent.getEventType() )
	{
		const logic::EvtData_From_DriverEnterTank& evtData = static_cast<const logic::EvtData_From_DriverEnterTank&>(mEvent);
		mSoundSystem->playSound(sound::Sound_PlayerFootEnterTank, evtData.mActorId, Channel_Movement);
	}

	else if( logic::EvtData_From_KamikazeAntExplode::msEventType == mEvent.getEventType() )
	{
		const logic::EvtData_From_KamikazeAntExplode& evtData = static_cast<const logic::EvtData_From_KamikazeAntExplode&>(mEvent);
		utilities::Position pos = mpActorHandler->getActor(evtData.mEnemy)->getPosition();
		mSoundSystem->playSoundOnce(sound::Sound_EnemyAntAttackExplosion, pos.x, pos.y, pos.z); 
	}

	else if( logic::EvtData_From_EnemyMeleeAttack::msEventType == mEvent.getEventType() )
	{
		const logic::EvtData_From_EnemyMeleeAttack& evtData = static_cast<const logic::EvtData_From_EnemyMeleeAttack&>(mEvent);
		logic::Actor* pActor = mpActorHandler->getActor(evtData.mEnemy);
		logic::Enemy* pEnemy = dynamic_cast<logic::Enemy*>(pActor);

		if (pEnemy == NULL)
		{
			ERROR_MESSAGE("Could not dynamic_cast pActor into pEnemy");
			return false;
		}

		if( pEnemy->getEnemySize() == logic::Enemy::Size_Small )
		{
			mSoundSystem->playSound(sound::Sound_EnemyAntAttackMelee, pActor->getKey(), Channel_Attack);
		}
		else if( pEnemy->getEnemySize() == logic::Enemy::Size_Medium )
		{
			mSoundSystem->playSound(sound::Sound_EnemySpiderAttackMelee, pActor->getKey(), Channel_Attack);
		}
		else if( pEnemy->getEnemySize() == logic::Enemy::Size_Large )
		{
			mSoundSystem->playSound(sound::Sound_EnemyBeetleAttackMelee, pActor->getKey(), Channel_Attack);
		}
	}

	else if( logic::EvtData_From_EnemyWeaponFired::msEventType == mEvent.getEventType() )
	{
		const logic::EvtData_From_EnemyWeaponFired& evtData = static_cast<const logic::EvtData_From_EnemyWeaponFired&>(mEvent);
		logic::Actor* pActor = mpActorHandler->getActor(evtData.mActorId);	
		logic::Enemy* pEnemy = dynamic_cast<logic::Enemy*>(pActor);

		if (pEnemy == NULL)
		{
			ERROR_MESSAGE("Could not dynamic_cast pActor into pEnemy");
			return false;
		}

		if( pEnemy->getEnemySize() == logic::Enemy::Size_Small )
		{
			mSoundSystem->playSound(sound::Sound_EnemyAntAttackRanged, pActor->getKey(), Channel_Attack);
		}
		else if( pEnemy->getEnemySize() == logic::Enemy::Size_Medium )
		{
			mSoundSystem->playSound(sound::Sound_EnemySpiderAttackRanged, pActor->getKey(), Channel_Attack);
		}
		else if( pEnemy->getEnemySize() == logic::Enemy::Size_Large )
		{
			mSoundSystem->playSound(sound::Sound_EnemyBeetleAttackRanged, pActor->getKey(), Channel_Attack);
		}
	}

	
	return false;
}
