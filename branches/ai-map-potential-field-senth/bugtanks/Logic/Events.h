/**
 * @file
 * @author Gustav Persson <ninjamuff@gmail.com>
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
 *  Definitions of event structs goes in here.
 */

#ifndef __EVENTS_H__
#define __EVENTS_H__
#include "../Utilities/Macros.h"
#include "IEventManager.h"
#include "InputStates.h"
#include "ActorCreateInfo.h"
#include <vector>

namespace logic
{

/******************************************************************
*                         SYSTEM EVENTS                           *
*                                                                 *
*******************************************************************/


/**
* Fired when a new game is started. Not at app start, but when the game world needs building.
*/
struct EvtData_System_StartGame : public EmptyEventData
{
public:
	explicit EvtData_System_StartGame(const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: EmptyEventData()
	{
	}

	explicit EvtData_System_StartGame(std::istrstream &in, int networkId)
	{
		mNetworkId = networkId;
	}

	virtual ~EvtData_System_StartGame()
	{
	}
	
	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_StartGame());
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
	}

	static const EventType msEventType;

};

struct EvtData_System_StartLoading : public BaseEventData
{
public:
	explicit EvtData_System_StartLoading(bool editorEnabled)
		: BaseEventData()
	{
		mEditorEnabled = editorEnabled;
	}

	explicit EvtData_System_StartLoading(std::istrstream &in, int networkId)
	{
		mNetworkId = networkId;
		in >> mEditorEnabled;
	}

	virtual ~EvtData_System_StartLoading()
	{
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_StartLoading(mEditorEnabled));
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
		out << mEditorEnabled << " ";
	}

	bool mEditorEnabled;
	static const EventType msEventType;
};

struct EvtData_System_FinishedLoading : public EmptyEventData
{
public:
	explicit EvtData_System_FinishedLoading()
		: EmptyEventData()
	{
	}

	explicit EvtData_System_FinishedLoading(std::istrstream &in, int networkId)
	{
		mNetworkId = networkId;
	}

	virtual ~EvtData_System_FinishedLoading()
	{
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_FinishedLoading());
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
	}

	static const EventType msEventType;
};


/**
* Fired when a game ends.
*/
struct EvtData_System_EndGame : public EmptyEventData
{
	explicit EvtData_System_EndGame(const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: EmptyEventData(timeStamp)
	{
	}

	virtual ~EvtData_System_EndGame(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_EndGame());
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	static const EventType msEventType;

};

/**
*	Fired when a remote player disconnects without warning.
*/

struct EvtData_System_PlayerDropped : public EmptyEventData
{
	explicit EvtData_System_PlayerDropped(int playerId, const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds), int networkId = PlayerInfo::getInstance()->getNetworkId())
		:EmptyEventData(timeStamp)
	{
		mPlayerId = playerId;
	}

	virtual ~EvtData_System_PlayerDropped(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_PlayerDropped(mPlayerId, mTimeStamp, mNetworkId));
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	static const EventType msEventType;

	int mPlayerId;
};

/**
* Fired when the driver's death animation has ended.
*/
struct EvtData_System_GameOver : public EmptyEventData
{
	explicit EvtData_System_GameOver(const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: EmptyEventData(timeStamp)
	{
	}

	virtual ~EvtData_System_GameOver(){}
	
	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_GameOver());
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	static const EventType msEventType;
};

/**
* Information event fired when the event system is up-and-running.
*/
struct EvtData_System_EventSystemStarted : public EmptyEventData
{
	explicit EvtData_System_EventSystemStarted(const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: EmptyEventData(timeStamp){}

	virtual ~EvtData_System_EventSystemStarted(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_EventSystemStarted());
	}

	static const EventType msEventType;
};

/**
* The GFX-system is up and running
*/
struct EvtData_System_GFXSystemStarted : public EmptyEventData
{
	explicit EvtData_System_GFXSystemStarted(const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: EmptyEventData(timeStamp){}

	virtual ~EvtData_System_GFXSystemStarted(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_GFXSystemStarted());
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	static const EventType msEventType;
};

/**
* The ActorHandler Started
*/
struct EvtData_System_ActorHandlerStarted : public EmptyEventData
{
	explicit EvtData_System_ActorHandlerStarted(const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: EmptyEventData(timeStamp){}

	virtual ~EvtData_System_ActorHandlerStarted(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_ActorHandlerStarted());
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	static const EventType msEventType;
};
/**
* The InputStates Initiated
*/
struct EvtData_System_InputStatesStarted : public EmptyEventData
{
	explicit EvtData_System_InputStatesStarted(const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: EmptyEventData(timeStamp){}

	virtual ~EvtData_System_InputStatesStarted(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_InputStatesStarted());
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	static const EventType msEventType;
};

/**
* The game is about to be saved.
*/
struct EvtData_System_PreSave : public EmptyEventData
{
	explicit EvtData_System_PreSave(const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: EmptyEventData(timeStamp){}
	
	virtual ~EvtData_System_PreSave(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_PreSave());
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	static const EventType msEventType;
};

/**
* The game has been saved!
*/
struct EvtData_System_PostSave : public EmptyEventData
{
	explicit EvtData_System_PostSave(const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: EmptyEventData(timeStamp)
	{
	}

	virtual ~EvtData_System_PostSave(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_PostSave());
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	static const EventType msEventType;
};

/**
 * Mouse has moved.
 */
struct EvtData_System_MouseMove : public BaseEventData
{
	explicit EvtData_System_MouseMove(long mouseX, long mouseY, long dX, long dY,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mMouseX = mouseX;
		mMouseY = mouseY;
		mMouseDX = dX;
		mMouseDY = dY;
	}

	virtual ~EvtData_System_MouseMove(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_MouseMove(mMouseX, mMouseY, mMouseDX, mMouseDY));
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
	long mMouseX;
	long mMouseY;
	long mMouseDX;
	long mMouseDY;
};

/**
 * Mouse has pressed a key.
 */
struct EvtData_System_MouseKeyDown : public BaseEventData
{
	explicit EvtData_System_MouseKeyDown(int keys,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mKeys = keys;
	}

	virtual ~EvtData_System_MouseKeyDown(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_MouseKeyDown(mKeys));
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
	int mKeys;

	enum MouseKeyType
	{
		MouseKeyType_Left = 1,
		MouseKeyType_Middle = 2,
		MouseKeyType_Right = 4,
		MouseKeyType_LeftDouble = 8,
		MouseKeyType_MiddleDouble = 16,
		MouseKeyType_RightDouble = 32
	};
};

/**
 * Mouse has released a key.
 */
struct EvtData_System_MouseKeyUp : public BaseEventData
{
	explicit EvtData_System_MouseKeyUp(int keys,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mKeys = keys;
	}

	virtual ~EvtData_System_MouseKeyUp(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_MouseKeyUp(mKeys));
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	static const EventType msEventType;
	int mKeys;

	virtual void serialize(std::ostrstream &streamOut) const {}

	enum MouseKeyType
	{
		MouseKeyType_Left = 1,
		MouseKeyType_Middle = 2,
		MouseKeyType_Right = 4
	};
};
/**
* Fired everytime any keys is pressed.
*/
struct EvtData_System_KeysPressed : public BaseEventData
{
public:
	explicit EvtData_System_KeysPressed(unsigned int letterKeysPressed, unsigned int keysPressed,  const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mLetterKeysPressed = letterKeysPressed;
		mKeysPressed = keysPressed;
	}
	explicit EvtData_System_KeysPressed(std::istrstream &in)
	{
		in >> mLetterKeysPressed;
		in >> mKeysPressed;
	}
	virtual ~EvtData_System_KeysPressed(){}
	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_KeysPressed(mLetterKeysPressed, mKeysPressed));
	}
	virtual void serialize(std::ostream &out) const
	{
		out << mLetterKeysPressed;
		out << mKeysPressed;
	}
	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
	unsigned int mLetterKeysPressed;
	unsigned int mKeysPressed;
};


/******************************************************************
*                           FROM-EVENTS                           *
*                                                                 *
*******************************************************************/

/**
* Fired when a new actor is created.
*/
struct EvtData_From_ActorCreated : public BaseEventData
{
	explicit EvtData_From_ActorCreated(KeyType actorId, const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mId = actorId;
	}
	explicit EvtData_From_ActorCreated(std::istrstream &in) 
	{
		in >> mId;
	}

	virtual ~EvtData_From_ActorCreated(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_From_ActorCreated(mId));
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mId << " ";	
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	static const EventType msEventType;
	KeyType mId;
	
};


/**
* Fired when a actor is destroyed.
*/
struct EvtData_From_ActorDestroyed : public BaseEventData
{
	explicit EvtData_From_ActorDestroyed(KeyType actorId, int networkId = PlayerInfo::getInstance()->getNetworkId(),
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp) 
	{
		mNetworkId = networkId;
		mId = actorId;
	}
	
	explicit EvtData_From_ActorDestroyed(std::istrstream &in, int networkId,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData()
	{
		mNetworkId = networkId;
		in >> mId;
	}

	virtual ~EvtData_From_ActorDestroyed(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_From_ActorDestroyed(mId));
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
		out << mId << " ";
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	static const EventType msEventType;
	KeyType mId;
};

/**
* Fired when two actors collide
*//*
struct EvtData_From_ActorCollision : public BaseEventData
{
	explicit EvtData_From_ActorCollision(KeyType actorId1, KeyType actorId2,KeyType actorId3, 
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp) 
	{
		mActorId1 = actorId1; // Bullet
		mActorId2 = actorId2; // Target
		mActorId3 = actorId3; // Shooter
	}

	virtual ~EvtData_From_ActorCollision(){}
	
	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_From_ActorCollision(mActorId1, mActorId2,mActorId3));
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	static const EventType msEventType;
	KeyType mActorId1, mActorId2, mActorId3;	
};

*/
/**
* Fired when a ray hits something
*/
struct EvtData_From_ActorCollisionRay : public BaseEventData
{
	explicit EvtData_From_ActorCollisionRay(D3DXVECTOR3 position,KeyType bulletActorId, KeyType targetActorId, KeyType shooterActorId, 
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp) 
	{
		mPosition = position;
		mBulletActorId = bulletActorId;
		mTargetActorId = targetActorId;
		mShooterActorId = shooterActorId;
	}

	virtual ~EvtData_From_ActorCollisionRay(){}
	
	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_From_ActorCollisionRay(mPosition,mBulletActorId, mTargetActorId,mShooterActorId));
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
	D3DXVECTOR3 mPosition;
	KeyType mBulletActorId, mTargetActorId, mShooterActorId;	
};

/**
* Fired when two actors collide
*/
struct EvtData_From_ActorCollisionObject : public BaseEventData
{
	explicit EvtData_From_ActorCollisionObject(KeyType firstActorId, KeyType secondActorId, utilities::Direction collisionNormal,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp) 
	{
		mFirstActorId = firstActorId; 
		mSecondActorId = secondActorId; 
		mCollisionNormal = collisionNormal;
	}

	virtual ~EvtData_From_ActorCollisionObject(){}
	
	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_From_ActorCollisionObject(mFirstActorId, mSecondActorId, mCollisionNormal));
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
	KeyType mFirstActorId, mSecondActorId;
	utilities::Direction mCollisionNormal;
};

/**
* Fired when a driver's life reaches zero
*/
struct EvtData_From_DriverDeath : public BaseEventData
{
	explicit EvtData_From_DriverDeath(const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp) 
	{
	}
	virtual ~EvtData_From_DriverDeath(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_From_DriverDeath());
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
};

/**
* A driver has been hit by something.
*/
struct EvtData_From_DriverHit : public BaseEventData
{
	explicit EvtData_From_DriverHit(double damage, const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mDamage = damage;
	}
	
	virtual ~EvtData_From_DriverHit(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_From_DriverHit(mDamage));
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
	double mDamage;
};

/**
 * The driver Enters a new Tank
 */
struct EvtData_From_DriverEnterTank : public BaseEventData
{
	explicit EvtData_From_DriverEnterTank(KeyType actorId,const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mActorId = actorId;
	}
	
	virtual ~EvtData_From_DriverEnterTank(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_From_DriverEnterTank(mActorId));
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
	KeyType mActorId;
};

/**
* An enemy has been hit by a bullet
*/
struct EvtData_From_EnemyHit : public BaseEventData
{
	explicit EvtData_From_EnemyHit(KeyType shooter, KeyType enemy, const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mShooter = shooter;
		mEnemy = enemy;
	}

	virtual ~EvtData_From_EnemyHit(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_From_EnemyHit(mShooter, mEnemy));
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
	KeyType mShooter;
	KeyType mEnemy;
};

/**
* A melee enemy have hit something
*/
struct EvtData_From_EnemyMeleeAttack : public BaseEventData
{
	explicit EvtData_From_EnemyMeleeAttack(KeyType enemy, const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mEnemy = enemy;
	}

	virtual ~EvtData_From_EnemyMeleeAttack(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_From_EnemyMeleeAttack(mEnemy));
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
	KeyType mEnemy;
};

/**
* An kamikaze ant have hit something
*/
struct EvtData_From_KamikazeAntExplode : public BaseEventData
{
	explicit EvtData_From_KamikazeAntExplode(KeyType enemy, const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mEnemy = enemy;
	}

	virtual ~EvtData_From_KamikazeAntExplode(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_From_KamikazeAntExplode(mEnemy));
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
	KeyType mEnemy;
};

/**
* A bullet has been fired.
*/
struct EvtData_From_WeaponFired : public BaseEventData
{
	explicit EvtData_From_WeaponFired(KeyType actor, const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mActorId = actor;
	}

	virtual ~EvtData_From_WeaponFired(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_From_WeaponFired(mActorId));
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
	KeyType mActorId;
};

struct EvtData_From_WeaponSecondaryFired : public BaseEventData
{
	explicit EvtData_From_WeaponSecondaryFired(KeyType actor, const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mActorId = actor;
	}
	
	virtual ~EvtData_From_WeaponSecondaryFired(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_From_WeaponSecondaryFired(mActorId));
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
	KeyType mActorId;
};

struct EvtData_From_ActorHandlerSyncActors : public BaseEventData
{
	explicit EvtData_From_ActorHandlerSyncActors(std::vector<logic::Actor::ActorSyncInfo> syncInfo)
		: BaseEventData()
	{
		mSyncInfo = syncInfo;
		mStructSize = mSyncInfo.size();
	}

	explicit EvtData_From_ActorHandlerSyncActors(std::istrstream &in, int networkId)
	{
		logic::Actor::ActorSyncInfo tempSync;
		mNetworkId = networkId;
		in >> mStructSize;

		for (unsigned int i = 0; i < mStructSize; i++)
		{
			in >> tempSync;
			mSyncInfo.push_back(tempSync);
		}
	}

	virtual ~EvtData_From_ActorHandlerSyncActors(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const 
	{
		return EventDataPtr(myNew EvtData_From_ActorHandlerSyncActors(mSyncInfo));
	}

	virtual void serialize(std::ostrstream &out) const
	{

		out << mNetworkId << " ";
		out << mStructSize << " ";
		
		for (unsigned int i = 0; i < mStructSize; i++)
		{
			out << mSyncInfo.at(i) << " ";
		}
	}

	static const EventType msEventType;
	unsigned int mStructSize;
	std::vector<logic::Actor::ActorSyncInfo> mSyncInfo;
};

struct EvtData_From_EnemyWeaponFired : public BaseEventData
{
	explicit EvtData_From_EnemyWeaponFired(KeyType actor, const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mActorId = actor;
	}

	virtual ~EvtData_From_EnemyWeaponFired(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_From_EnemyWeaponFired(mActorId));
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
	KeyType mActorId;
};

/******************************************************************
*                             TO-EVENTS                           *
*                                                                 *
*******************************************************************/

/**
* Used to create a tank
*/
struct EvtData_To_ActorCreateTank : public BaseEventData
{
	explicit EvtData_To_ActorCreateTank(Tank::TankTypes tankType, utilities::Position pos, utilities::Direction dir, Actor::PlayerMovingInfo playerMovingInfo, const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mTankType = tankType;
		mPos = pos;
		mDir = dir;
		mPlayerMovingInfo = playerMovingInfo;
	}
	explicit EvtData_To_ActorCreateTank(std::istrstream &in, int networkId, 
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		in >> mTankType;
		in >> mPos;
		in >> mDir;
		in >> mPlayerMovingInfo;

		mNetworkId = networkId;
	}

	virtual ~EvtData_To_ActorCreateTank(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_ActorCreateTank(mTankType, mPos, mDir, mPlayerMovingInfo));
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
		out << mTankType << " ";	
		out << mPos << " ";
		out << mDir << " ";
		out << mPlayerMovingInfo << " ";
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	static const EventType msEventType;
	Tank::TankTypes mTankType;
	utilities::Position mPos;
	utilities::Direction mDir;
	Actor::PlayerMovingInfo mPlayerMovingInfo;
	
};

/**
* Used to create a enemy
*/
struct EvtData_To_ActorCreateEnemy : public BaseEventData
{
	explicit EvtData_To_ActorCreateEnemy(Enemy::EnemyTypes enemyType, utilities::Position pos, utilities::Direction dir, const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mEnemyType = enemyType;
		mPos = pos;
		mDir = dir;
	}
	explicit EvtData_To_ActorCreateEnemy(std::istrstream &in, int networkId, 
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		in >> mEnemyType;
		in >> mPos;
		in >> mDir;
		mNetworkId = networkId;
	}

	virtual ~EvtData_To_ActorCreateEnemy(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_ActorCreateEnemy(mEnemyType, mPos, mDir));
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
		out << mEnemyType << " ";	
		out << mPos << " ";
		out << mDir << " ";
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	static const EventType msEventType;
	Enemy::EnemyTypes mEnemyType;
	utilities::Position mPos;
	utilities::Direction mDir;
};


/**
* Used to create a munition
*/
struct EvtData_To_ActorCreateMunition : public BaseEventData
{
	explicit EvtData_To_ActorCreateMunition(Munitions::MunitionTypes munitionType, utilities::Position pos, utilities::Direction dir, KeyType creatorId, const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mMunitionType = munitionType;
		mPos = pos;
		mDir = dir;
		mCreatorId = creatorId;
	}
	explicit EvtData_To_ActorCreateMunition(std::istrstream &in, int networkId, 
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		in >> mMunitionType;
		in >> mPos;
		in >> mDir;
		in >> mCreatorId;
		mNetworkId = networkId;
	}

	virtual ~EvtData_To_ActorCreateMunition(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_ActorCreateMunition(mMunitionType, mPos, mDir, mCreatorId));
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
		out << mMunitionType << " ";	
		out << mPos << " ";
		out << mDir << " ";
		out << mCreatorId << " ";
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	static const EventType msEventType;
	Munitions::MunitionTypes mMunitionType;
	utilities::Position mPos;
	utilities::Direction mDir;
	KeyType mCreatorId;
	
};

/**
* Used to create a driver
*/
struct EvtData_To_ActorCreateDriver : public BaseEventData
{
	explicit EvtData_To_ActorCreateDriver(utilities::Position pos, utilities::Direction dir, Actor::PlayerMovingInfo playerMovingInfo, const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mPos = pos;
		mDir = dir;
		mPlayerMovingInfo = playerMovingInfo;
	}
	explicit EvtData_To_ActorCreateDriver(std::istrstream &in, int networkId, 
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		in >> mPos;
		in >> mDir;
		in >> mPlayerMovingInfo;
		mNetworkId = networkId;
	}

	virtual ~EvtData_To_ActorCreateDriver(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_ActorCreateDriver(mPos, mDir, mPlayerMovingInfo));
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
		out << mPos << " ";
		out << mDir << " ";
		out << mPlayerMovingInfo << " ";
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	static const EventType msEventType;
	utilities::Position mPos;
	utilities::Direction mDir;
	Actor::PlayerMovingInfo mPlayerMovingInfo;
};

struct EvtData_To_ActorCreateSpectator : public BaseEventData
{
	explicit EvtData_To_ActorCreateSpectator(utilities::Position pos, utilities::Direction dir, Actor::PlayerMovingInfo playerMovingInfo, const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mPos = pos;
		mDir = dir;
		mPlayerMovingInfo = playerMovingInfo;
	}
	explicit EvtData_To_ActorCreateSpectator(std::istrstream &in, int networkId, 
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		in >> mPos;
		in >> mDir;
		in >> mPlayerMovingInfo;
		mNetworkId = networkId;
	}

	virtual ~EvtData_To_ActorCreateSpectator(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_ActorCreateSpectator(mPos, mDir, mPlayerMovingInfo));
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
		out << mPos << " ";
		out << mDir << " ";
		out << mPlayerMovingInfo << " ";
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	static const EventType msEventType;
	utilities::Position mPos;
	utilities::Direction mDir;
	Actor::PlayerMovingInfo mPlayerMovingInfo;
};

/**
* Used to create a StaticEnvironment
*/
struct EvtData_To_ActorCreateStaticEnvironment : public BaseEventData
{
	explicit EvtData_To_ActorCreateStaticEnvironment(StaticEnvironment::StaticEnvironmentTypes staticEnvironmentType, utilities::Position pos, utilities::Direction dir, utilities::Vec3Float scale, const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mStaticEnvironmentType = staticEnvironmentType;
		mPos = pos;
		mDir = dir;
		mScale = scale;
	}
	explicit EvtData_To_ActorCreateStaticEnvironment(std::istrstream &in, int networkId, 
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		in >> mStaticEnvironmentType;
		in >> mPos;
		in >> mDir;
		in >> mScale;
		mNetworkId = networkId;
	}

	virtual ~EvtData_To_ActorCreateStaticEnvironment(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_ActorCreateStaticEnvironment(mStaticEnvironmentType, mPos, mDir, mScale));
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
		out << mStaticEnvironmentType << " ";
		out << mPos << " ";
		out << mDir << " ";
		out << mScale << " ";
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	static const EventType msEventType;
	utilities::Position mPos;
	utilities::Direction mDir;
	StaticEnvironment::StaticEnvironmentTypes mStaticEnvironmentType;
	utilities::Vec3Float mScale;
};


/**
* Fired when an actor start to move.
*/
struct EvtData_To_ActorStartMove : public BaseEventData
{
	explicit EvtData_To_ActorStartMove(KeyType actorID, bool forward, const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp) 
	{
		mId = actorID;
		mForward = forward;
	}
	explicit EvtData_To_ActorStartMove(std::istrstream &in, int networkId,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		in >> mId;
		in >> mForward;
		mNetworkId = networkId;
	}
	virtual ~EvtData_To_ActorStartMove(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_ActorStartMove(mId, mForward));
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
		out << mId << " ";
		out << mForward << " ";
	}

	static const EventType msEventType;
	KeyType mId;
	bool mForward;
};
/**
* Fired when an actor stop to move.
*/
struct EvtData_To_ActorStopMove : public BaseEventData
{
	explicit EvtData_To_ActorStopMove(KeyType actorID, bool forward, const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp) 
	{
		mId = actorID;
		mForward = forward;
	}

	explicit EvtData_To_ActorStopMove(std::istrstream &in, int networkId,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		in >> mId;	
		in >> mForward;
		mNetworkId = networkId;
	}

	virtual ~EvtData_To_ActorStopMove(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_ActorStopMove(mId, mForward));
	}

	virtual void serialize(std::ostrstream &out) const
	{		
		out << mNetworkId << " ";
		out << mId << " ";
		out << mForward << " ";
	}

	static const EventType msEventType;
	KeyType mId;
	bool mForward;
};


/**
* The enemy HP needs to be updated
*/

struct EvtData_To_EnemyUpdateHp : public BaseEventData
{
	explicit EvtData_To_EnemyUpdateHp(KeyType id, float damage,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mDamage = damage;
		mId = id;
	}

	virtual ~EvtData_To_EnemyUpdateHp(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_EnemyUpdateHp(mId, mDamage));
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
	KeyType mId;
	float mDamage;
};

/**
* The Game State is being changed.
*/
struct EvtData_To_InputChangeGameState : public BaseEventData
{
	explicit EvtData_To_InputChangeGameState(InputStates::GameStates state,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mState = state;
	}

	virtual ~EvtData_To_InputChangeGameState(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_InputChangeGameState(mState));
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
	InputStates::GameStates mState;
};

/**
* Fired when the tank needs to start firing.
*/
struct EvtData_To_TankStartFire : public BaseEventData
{
	explicit EvtData_To_TankStartFire(KeyType tankId,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mTankId = tankId;
	}

	explicit EvtData_To_TankStartFire(std::istrstream &in, int networkId,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
	{
		in >> mTankId;
		mNetworkId = networkId;
	}

	virtual ~EvtData_To_TankStartFire(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_TankStartFire(mTankId));
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
		out << mTankId << " ";
	}

	static const EventType msEventType;
	KeyType mTankId;
};

/**
* Fired when the tank should stop firing.
*/
struct EvtData_To_TankStopFire : public BaseEventData
{
	explicit EvtData_To_TankStopFire(KeyType tankId,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mTankId = tankId;
	}

	explicit EvtData_To_TankStopFire(std::istrstream &in, int networkId)
	{
		mNetworkId = networkId;
		in >> mTankId;
	}

	virtual ~EvtData_To_TankStopFire(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_TankStopFire(mTankId));
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
		out << mTankId << " ";
	}

	static const EventType msEventType;
	KeyType mTankId;
};

/**
* Fired when the tank needs to start firing pedestrian gun.
*/
struct EvtData_To_TankStartFireSecondary : public BaseEventData
{
	explicit EvtData_To_TankStartFireSecondary(KeyType tankId,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mTankId = tankId;
	}

	explicit EvtData_To_TankStartFireSecondary(std::istrstream &in, int networkId,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
	{
		in >> mTankId;
		mNetworkId = networkId;
	}

	virtual ~EvtData_To_TankStartFireSecondary(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_TankStartFireSecondary(mTankId));
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
		out << mTankId << " ";
	}

	static const EventType msEventType;
	KeyType mTankId;
};

/**
* Fired when the tank should stop firing pedestrian gun.
*/
struct EvtData_To_TankStopFireSecondary : public BaseEventData
{
	explicit EvtData_To_TankStopFireSecondary(KeyType tankId,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mTankId = tankId;
	}

	explicit EvtData_To_TankStopFireSecondary(std::istrstream &in, int networkId)
	{
		in >> mTankId;
		mNetworkId = networkId;
	}

	virtual ~EvtData_To_TankStopFireSecondary(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_TankStopFire(mTankId));
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
		out << mTankId << " ";
	}

	static const EventType msEventType;
	KeyType mTankId;
};

/**
* Fired when the driver should start firing
*/
struct EvtData_To_DriverStartFire : public BaseEventData
{
	explicit EvtData_To_DriverStartFire(KeyType driverId, 
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds)) 
		: BaseEventData(timeStamp)
	{
		mDriverId = driverId;
	}

	virtual ~EvtData_To_DriverStartFire(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_DriverStartFire(mDriverId));
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
	KeyType mDriverId;
};

/**
* Fired when the driver should stop firing.
*/
struct EvtData_To_DriverStopFire : public BaseEventData
{
	explicit EvtData_To_DriverStopFire(KeyType driverId,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mDriverId = driverId;
	}

	virtual ~EvtData_To_DriverStopFire(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_DriverStopFire(mDriverId));
	}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
	KeyType mDriverId;
};

/**
* Fired when a actor shall start to rotate
*/
struct EvtData_To_ActorStartRotate : public BaseEventData
{
	explicit EvtData_To_ActorStartRotate(KeyType id, bool rotateLeft, bool rotateSubset) 
	{
		mId = id;
		mRotateLeft = rotateLeft;
		mRotateSubset = rotateSubset;
	}

	explicit EvtData_To_ActorStartRotate(std::istrstream &in, int networkId,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		in >> mId;
		in >> mRotateLeft;
		in >> mRotateSubset;
		mNetworkId = networkId;

	}

	virtual ~EvtData_To_ActorStartRotate(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_ActorStartRotate(mId, mRotateLeft, mRotateSubset));
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
		out << mId << " ";
		out << mRotateLeft << " ";
		out << mRotateSubset << " ";
	}

	static const EventType msEventType;
	KeyType mId;
	bool mRotateLeft;
	bool mRotateSubset;
};

/**
* Fired when a actor shall stop rotating
*/
struct EvtData_To_ActorStopRotate : public BaseEventData
{
	explicit EvtData_To_ActorStopRotate(KeyType id, bool rotateLeft, bool rotateSubset)
	{
		mId = id;
		mRotateLeft = rotateLeft;
		mRotateSubset = rotateSubset;
	}

	explicit EvtData_To_ActorStopRotate(std::istrstream &in, int networkId,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		in >> mId;
		in >> mRotateLeft;
		in >> mRotateSubset;
		mNetworkId = networkId;
	}

	virtual ~EvtData_To_ActorStopRotate(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_ActorStopRotate(mId, mRotateLeft, mRotateSubset));
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
		out << mId << " ";
		out << mRotateLeft << " ";
		out << mRotateSubset << " ";
	}

	static const EventType msEventType;
	KeyType mId;
	bool mRotateLeft;
	bool mRotateSubset;
};

/**
* Used to change weapon on a tank.
*/
struct EvtData_To_TankChangeWeapon : public BaseEventData
{
	explicit EvtData_To_TankChangeWeapon(KeyType id, Weapon::WeaponTypes weaponType) 
	{
		mId = id;
		mWeaponType = weaponType;
	}

	explicit EvtData_To_TankChangeWeapon(std::istrstream &in, int networkId,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		in >> mId;
		in >> mWeaponType;
		mNetworkId = networkId;

	}

	virtual ~EvtData_To_TankChangeWeapon(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_TankChangeWeapon(mId, mWeaponType));
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
		out << mId << " ";
		out << mWeaponType << " ";
	}

	static const EventType msEventType;
	KeyType mId;
	Weapon::WeaponTypes mWeaponType;
};

/**
* Fired when a actor shall start to rotate
*/
struct EvtData_To_DriverStrafe : public BaseEventData
{
	explicit EvtData_To_DriverStrafe(KeyType id, bool strafeLeft, bool strafing) 
	{
		mId = id;
		mStrafeLeft = strafeLeft;
		mStrafing = strafing;
	}

	explicit EvtData_To_DriverStrafe(std::istrstream &in, int networkId,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		in >> mId;
		in >> mStrafeLeft;
		in >> mStrafing;
		mNetworkId = networkId;

	}

	virtual ~EvtData_To_DriverStrafe(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_DriverStrafe(mId, mStrafeLeft,mStrafing));
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
		out << mId << " ";
		out << mStrafeLeft << " ";
		out << mStrafing << " ";
	}

	static const EventType msEventType;
	KeyType mId;
	bool mStrafeLeft;
	bool mStrafing;
};

/**
* Fired to make the camera rotate
*/
struct EvtData_To_CameraStartRotate : public BaseEventData
{
	explicit EvtData_To_CameraStartRotate(bool rotateLeft) 
	{
		mRotateLeft = rotateLeft;
	}

	virtual ~EvtData_To_CameraStartRotate(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_CameraStartRotate(mRotateLeft));
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
	bool mRotateLeft;
};

struct EvtData_To_CameraStopRotate : public BaseEventData
{
	explicit EvtData_To_CameraStopRotate(bool rotateLeft) 
	{
		mRotateLeft = rotateLeft;
	}

	virtual ~EvtData_To_CameraStopRotate(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_CameraStopRotate(mRotateLeft));
	}

	virtual void serialize(std::ostrstream &streamOut) const {}

	static const EventType msEventType;
	bool mRotateLeft;
};

/**
* Fired to reduce a enemies hp
*/
struct EvtData_To_ActorReduceHp : public BaseEventData
{
	explicit EvtData_To_ActorReduceHp(KeyType id, float damage) 
	{
		mId = id;
		mDamage = damage;
	}

	explicit EvtData_To_ActorReduceHp(std::istrstream &in, int networkId,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		in >> mId;
		in >> mDamage;
		mNetworkId = networkId;

	}

	virtual ~EvtData_To_ActorReduceHp(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_ActorReduceHp(mId, mDamage));
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
		out << mId << " ";
		out << mDamage << " ";
	}

	static const EventType msEventType;
	KeyType mId;
	float mDamage;
};

/**
 * Creates a stored trigger based on its ID.
 */
struct EvtData_To_CreateTrigger : public BaseEventData
{
	explicit EvtData_To_CreateTrigger(int triggerID) 
	{
		mTriggerID = triggerID;
	}

	explicit EvtData_To_CreateTrigger(std::istrstream &in, int networkId,
		const float timeStamp = GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		in >> mTriggerID;
		mNetworkId = networkId;

	}

	virtual ~EvtData_To_CreateTrigger(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_CreateTrigger(mTriggerID));
	}

	virtual void serialize(std::ostrstream &out) const
	{
		out << mNetworkId << " ";
		out << mTriggerID << " ";
	}

	static const EventType msEventType;
	int mTriggerID;
};

/**
 * Shows all triggers.
 */
struct EvtData_To_ShowTriggers : public BaseEventData
{
	explicit EvtData_To_ShowTriggers() 
	{

	}

	virtual ~EvtData_To_ShowTriggers(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_ShowTriggers());
	}

	virtual void serialize(std::ostrstream &out) const {}

	static const EventType msEventType;
};

/**
 * Hides all triggers.
 */
struct EvtData_To_HideTriggers : public BaseEventData
{
	explicit EvtData_To_HideTriggers() 
	{

	}

	virtual ~EvtData_To_HideTriggers(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_HideTriggers());
	}

	virtual void serialize(std::ostrstream &out) const {}

	static const EventType msEventType;
};

/**
 * Occurs when a tank or driver collides with a trigger bounding volume.
 *  Note: sends a pointer to the bounding volume as all triggers use the same ID.
 *  Should therefore never be sent over the network.
 */
struct EvtData_To_TriggerCollision : public BaseEventData
{
	explicit EvtData_To_TriggerCollision(void *pBoundingVolume) 
	{
		mpBoundingVolume = pBoundingVolume;
	}

	virtual ~EvtData_To_TriggerCollision(){}

	virtual const EventType &getEventType(void) const
	{
		return msEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_TriggerCollision(mpBoundingVolume));
	}

	virtual void serialize(std::ostrstream &out) const {}

	static const EventType msEventType;
	void *mpBoundingVolume;
};

}

#endif
