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
#include "KeyHandler.h"
#include "InputStates.h"
#include "Actor.h"
#include "GlobalTimer.h"

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
	explicit EvtData_System_StartGame(const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
		: EmptyEventData()
	{
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
		return mEventType;
	}

	static const EventType mEventType;

};

/**
* Fired when a game ends.
*/
struct EvtData_System_EndGame : public BaseEventData
{
	explicit EvtData_System_EndGame(const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
	}

	virtual ~EvtData_System_EndGame(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_EndGame());
	}

	virtual const EventType &getEventType(void) const
	{
		return mEventType;
	}

	static const EventType mEventType;

};

/**
* Fired when the driver's death animation has ended.
*/
struct EvtData_System_GameOver : public BaseEventData
{
	explicit EvtData_System_GameOver(const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
	}

	virtual ~EvtData_System_GameOver(){}
	
	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_GameOver());
	}

	virtual const EventType &getEventType(void) const
	{
		return mEventType;
	}

	static const EventType mEventType;
};

/**
* Information event fired when the event system is up-and-running.
*/
struct EvtData_System_EventSystemStarted : public EmptyEventData
{
	explicit EvtData_System_EventSystemStarted(const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
		: EmptyEventData(timeStamp){}

	virtual ~EvtData_System_EventSystemStarted(){}

	virtual const EventType &getEventType(void) const
	{
		return mEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_EventSystemStarted());
	}

	static const EventType mEventType;
};

/**
* The GFX-system is up and running
*/
struct EvtData_System_GFXSystemStarted : public EmptyEventData
{
	explicit EvtData_System_GFXSystemStarted(const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
		: EmptyEventData(timeStamp){}

	virtual ~EvtData_System_GFXSystemStarted(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_GFXSystemStarted());
	}

	virtual const EventType &getEventType(void) const
	{
		return mEventType;
	}

	static const EventType mEventType;
};

/**
* The game is about to be saved.
*/
struct EvtData_System_PreSave : public BaseEventData
{
	explicit EvtData_System_PreSave(const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp){}
	
	virtual ~EvtData_System_PreSave(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_PreSave());
	}

	virtual const EventType &getEventType(void) const
	{
		return mEventType;
	}

	static const EventType mEventType;
};

/**
* The game has been saved!
*/
struct EvtData_System_PostSave : public BaseEventData
{
	explicit EvtData_System_PostSave(const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
	}

	virtual ~EvtData_System_PostSave(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_System_PostSave());
	}

	virtual const EventType &getEventType(void) const
	{
		return mEventType;
	}

	static const EventType mEventType;
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
	explicit EvtData_From_ActorCreated(int actorId, const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
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

	virtual void serialize(std::ostream &out) const
	{
		out << mId;	
	}

	virtual const EventType &getEventType(void) const
	{
		return mEventType;
	}

	static const EventType mEventType;
	int mId;
	
};

/**
* Fired when a actor is destroyed.
*/
struct EvtData_From_ActorDestroyed : public BaseEventData
{
	explicit EvtData_From_ActorDestroyed(int actorId, const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp) 
	{
		mId = actorId;
	}
	
	explicit EvtData_From_ActorDestroyed(std::istrstream &in,
		const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
		: BaseEventData()
	{
		in >> mId;
	}

	virtual ~EvtData_From_ActorDestroyed(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_From_ActorDestroyed(mId));
	}

	virtual const EventType &getEventType(void) const
	{
		return mEventType;
	}

	static const EventType mEventType;
	int mId;
};

/**
* Fired when two actors collide
*/
struct EvtData_From_ActorCollision : public BaseEventData
{
	explicit EvtData_From_ActorCollision(KeyType actorId1, KeyType actorId2,KeyType actorId3, 
		const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp) 
	{
		mActorId1 = actorId1;
		mActorId2 = actorId2;
		mActorId3 = actorId3;
	}

	virtual ~EvtData_From_ActorCollision(){}
	
	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_From_ActorCollision(mActorId1, mActorId2,mActorId3));
	}

	virtual const EventType &getEventType(void) const
	{
		return mEventType;
	}

	static const EventType mEventType;
	KeyType mActorId1, mActorId2, mActorId3;	
};

/**
* Fired when a driver's life reaches zero
*/
struct EvtData_From_DriverDeath : public BaseEventData
{
	explicit EvtData_From_DriverDeath(const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
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
		return mEventType;
	}

	static const EventType mEventType;
};

/**
* A driver has been hit by something.
*/
struct EvtData_From_DriverHit : public BaseEventData
{
	explicit EvtData_From_DriverHit(double damage, const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
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
		return mEventType;
	}

	static const EventType mEventType;
	double mDamage;
};

/**
* An enemy has been hit by a bullet
*/
struct EvtData_From_EnemyHit : public EmptyEventData
{
	explicit EvtData_From_EnemyHit(const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
		: EmptyEventData(timeStamp)
	{
	}

	virtual ~EvtData_From_EnemyHit(){}

	virtual const EventType &getEventType(void) const
	{
		return mEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_From_EnemyHit());
	}

	static const EventType mEventType;
};

/**
* A bullet has been fired.
*/
struct EvtData_From_WeaponFired : public EmptyEventData
{
	explicit EvtData_From_WeaponFired(const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
		: EmptyEventData(timeStamp)
	{
	}
	
	virtual ~EvtData_From_WeaponFired(){}

	virtual const EventType &getEventType(void) const
	{
		return mEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_From_WeaponFired());
	}

	static const EventType mEventType;
};

/******************************************************************
*                             TO-EVENTS                           *
*                                                                 *
*******************************************************************/


/**
* Fired when an actor start to move.
*/
struct EvtData_To_ActorStartMove : public BaseEventData
{
	explicit EvtData_To_ActorStartMove(int actorID, bool forward, const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp) 
	{
		mId = actorID;
		mForward = forward;
	}
	explicit EvtData_To_ActorStartMove(std::istrstream &in, 
		const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		in >> mId;
		in >> mForward;
	}
	virtual ~EvtData_To_ActorStartMove(){}

	virtual const EventType &getEventType(void) const
	{
		return mEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_ActorStartMove(mId, mForward));
	}

	static const EventType mEventType;
	int mId;
	bool mForward;
};
/**
* Fired when an actor stop to move.
*/
struct EvtData_To_ActorStopMove : public BaseEventData
{
	explicit EvtData_To_ActorStopMove(int actorID, bool forward, const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp) 
	{
		mId = actorID;
		mForward = forward;
	}

	explicit EvtData_To_ActorStopMove(std::istrstream &in,
		const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		in >> mId;
		in >> mForward;
	}

	virtual ~EvtData_To_ActorStopMove(){}

	virtual const EventType &getEventType(void) const
	{
		return mEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_ActorStopMove(mId, mForward));
	}

	static const EventType mEventType;
	int mId;
	bool mForward;
};


/**
* The enemy HP needs to be updated
*/

struct EvtData_To_EnemyUpdateHp : public BaseEventData
{
	explicit EvtData_To_EnemyUpdateHp(KeyType id, double damage,
		const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
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
		return mEventType;
	}

	static const EventType mEventType;
	KeyType mId;
	double mDamage;
};

/**
* The Game State is being changed.
*/
struct EvtData_To_InputChangeGameState : public BaseEventData
{
	explicit EvtData_To_InputChangeGameState(InputStates *inputStates, InputStates::GameStates state,
		const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mState = state;
		mpInputStates = inputStates;
	}

	virtual ~EvtData_To_InputChangeGameState(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_InputChangeGameState(mpInputStates, mState));
	}

	virtual const EventType &getEventType(void) const
	{
		return mEventType;
	}

	static const EventType mEventType;
	InputStates::GameStates mState;
	InputStates *mpInputStates;
};

/**
* Fired when the tank needs to start firing.
*/
struct EvtData_To_TankStartFire : public BaseEventData
{
	explicit EvtData_To_TankStartFire(KeyType tankId,
		const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mTankId = tankId;
	}

	virtual ~EvtData_To_TankStartFire(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_TankStartFire(mTankId));
	}

	virtual const EventType &getEventType(void) const
	{
		return mEventType;
	}

	static const EventType mEventType;
	KeyType mTankId;
};

/**
* Fired when the tank should stop firing.
*/
struct EvtData_To_TankStopFire : public BaseEventData
{
	explicit EvtData_To_TankStopFire(KeyType tankId,
		const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
		: BaseEventData(timeStamp)
	{
		mTankId = tankId;
	}

	virtual ~EvtData_To_TankStopFire(){}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_TankStopFire(mTankId));
	}

	virtual const EventType &getEventType(void) const
	{
		return mEventType;
	}

	static const EventType mEventType;
	KeyType mTankId;
};

/**
* Fired when the driver should start firing
*/
struct EvtData_To_DriverStartFire : public BaseEventData
{
	explicit EvtData_To_DriverStartFire(KeyType driverId, 
		const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds)) 
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
		return mEventType;
	}

	static const EventType mEventType;
	KeyType mDriverId;
};

/**
* Fired when the driver should stop firing.
*/
struct EvtData_To_DriverStopFire : public BaseEventData
{
	explicit EvtData_To_DriverStopFire(KeyType driverId,
		const float timeStamp = GlobalTimer::getInstance()->getTime(Timer::ReturnType_MilliSeconds))
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
		return mEventType;
	}

	static const EventType mEventType;
	KeyType mDriverId;
};

/**
* Fired when a actor shall start to rotate
*/
struct EvtData_To_ActorStartRotate : public BaseEventData
{
	explicit EvtData_To_ActorStartRotate(KeyType id, bool rotateLeft, bool rotateSubset, const float timeStamp = 0.0f) : BaseEventData(timeStamp)
	{
		mId = id;
		mRotateLeft = rotateLeft;
		mRotateSubset = rotateSubset;
	}

	virtual ~EvtData_To_ActorStartRotate(){}

	virtual const EventType &getEventType(void) const
	{
		return mEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_ActorStartRotate(mId, mRotateLeft, mRotateSubset));
	}

	static const EventType mEventType;
	KeyType mId;
	bool mRotateLeft;
	bool mRotateSubset;
};

/**
* Fired when a actor shall stop rotating
*/
struct EvtData_To_ActorStopRotate : public BaseEventData
{
	explicit EvtData_To_ActorStopRotate(KeyType id, bool rotateLeft, bool rotateSubset, const float timeStamp = 0.0f) : BaseEventData(timeStamp)
	{
		mId = id;
		mRotateLeft = rotateLeft;
		mRotateSubset = rotateSubset;
	}

	virtual ~EvtData_To_ActorStopRotate(){}

	virtual const EventType &getEventType(void) const
	{
		return mEventType;
	}

	virtual EventDataPtr copy() const
	{
		return EventDataPtr(myNew EvtData_To_ActorStopRotate(mId, mRotateLeft, mRotateSubset));
	}

	static const EventType mEventType;
	KeyType mId;
	bool mRotateLeft;
	bool mRotateSubset;
};


}
#endif
