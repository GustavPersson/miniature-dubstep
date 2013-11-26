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
 */

#ifndef __ACTOR_H__
#define __ACTOR_H__

#include "../Utilities/Macros.h"
#include "../Utilities/Vectors.h"
#include "../GFX/DX10Module.h"
#include "ActorConstants.h"
#include <strstream>
#include "ModelInfo.h"
#include <string>

namespace logic
{

// Forward declarations
class PlayerInfo;
class GlobalTimer;
class KeyHandler;
class EventData;
class ActorHandler;


/**
 * This is the topmost in the actor hierarchy, it will be used to store all game actors in a map
 */

class Actor
{
public:
	enum ActorTypes
	{
		ActorType_Invalid,
		ActorType_Enemy, 
		ActorType_Tank,
		ActorType_Driver,
		ActorType_Munition,
		ActorType_StaticEnvironment,
		ActorType_Pedestrian,
		ActorType_Spectator
	};

	struct PlayerMovingInfo
	{
		PlayerMovingInfo() : mMoving(false), mRotating(false), mRotatingDirection(0), mMovingDirection(0) {}
		bool mMoving;
		bool mRotating;
		int mRotatingDirection;
		int mMovingDirection;
	};

	struct ActorSyncInfo
	{
		KeyType mId;
		utilities::Position mPosition;
		utilities::Direction mDirection;
		utilities::Direction mSubsetDirection;
		utilities::Vec3Float mScale;
	};

	/**
	* Constructor, assigns the same key used in the map to the actor itself
	* @param networkId the network id of the player
	* @param size the size in map coordinates the actor has, default is 0,0.
	*/
	Actor(utilities::Position pos, utilities::Direction dir, int networkId, const utilities::MapCoordinate& size);

/**
 * Destructor.
 */
	virtual ~Actor();

/**
 * initiates the keyhandler
 */
    static void init();
/**
 * Pure Virtual update function
 * @param deltaTime the time between calls, used in calculating movement
 */
	virtual void update(float deltaTime) = 0;
/**
 * Pure Virtual handleEvent function
 * @param eventMessage an event of any type to be handled
 */
	virtual void handleEventId(EventData const &eventMessage);
/**
 * function that returns the key associated with the actor
 */
	inline KeyType getKey() const {return mId;}

/**
 * function that returns position struct
 * @return the position of the unit
 */
	inline const utilities::Position& getPosition() const {return mPosition;}

/**
 * function that returns direction struct
 * @return the direction of the unit
 */ 
	inline const utilities::Direction& getDirection() const {return mDirection;}

/**
 * function that sets the actors position to the given value
 * @param x the x-value
 * @param y the y-value
 * @param z the z-value
 */
	void setPosition(float x, float y, float z);
	
/**
 * function that sets the actors position to the given value
 * @param pos The position as an utilities position
 */
	void setPosition(utilities::Position pos);

/**
 * function that sets the actors destPosition to the given value
 * @param destPos The position as an utilities position
 */
	void setDestPosition(utilities::Position destPos);

/** 
 * function that sets the actors direction to the given value
 * @param x the x-value
 * @param y the y-value
 * @param z the z-value
 */
	void setDirection(float x, float y, float z);

/**
 * Set the direction of the actor
 * @param direction the new direction
 */
	inline void setDirection(const utilities::Direction& direction)
	{
		setDirection(direction.x, direction.y, direction.z);
	}

/**
 * function that returns the type of actor
 */
	ActorTypes getActorType();

/**
 * Returns the subtype of the actor.
 * @return Return the subtype.
 */
	inline virtual int getActorSubType() const {return -1;}
/**
 * function that sets the moving status of an actor
 */
	void setMoving(bool move);
/**
 * function that sets the direction of a subset mesh
 * @param x the x-value
 * @param y the y-value
 * @param z the z-value
 */
	void setSubsetDirection(float x, float y, float z);

/**
 * function that sets the direction of a subset mesh
 * @param new subsetDirection.
 */
	void setSubsetDirection(utilities::Direction subsetDirection);

/**
 * function that sets the direction of subset
 */
	utilities::Direction getSubsetDirection();

/**
* Returns if the actor is moving or not.
* @return Returns the bool that defines if the actor is moving.
*/
	bool getMoving();

/**
* Returns if the actor is rotating or not.
* @return Returns the bool that defines if the actor is rotating or not.
*/
	bool getRotating();

/** 
* Returns the TransformObject that is bound to the actor.
* @return A pointer to the transformobject that is bound to the actor.
*/
	gfx::TransformObject* getTransformObject();

/**
* Returns the size of the actor in map coordinates
* @return the size of the actor in map coordinates
*/
	inline utilities::MapCoordinate getSize() {return mSize;}
/**
 * pure virtual function for getting actor names
 */
	virtual std::string getName(void) = 0;

/**
* Returns if the subset is rotating or not.
* @return Returns the bool that defines if the actors subset is rotating.
*/
	bool getSubsetRotating();

/**
* Returns the actors current movementspeed.
* @return The movementspeed variable.
*/
	inline float getSpeed() const {return mSpeed;}

/**
* Sets the speed of the unit
* @param speed the new speed of the unit
*/
	inline void setSpeed(float speed) {mSpeed = speed;}

/**
* Returns the previous position the actor was in. (Used for collision handling)
* @return Returns the previous position.
*/
	const utilities::Position& getPrePosition();

/**
* Returns the previous direction the actor had. (Used for collision handling)
* @return Returns the previous direction.
*/
	const utilities::Direction& getPreDirection();

/**
* Returns the moving direction the actor was moving in. (1 = Forward, -1 = Backwards)
* @return Returns the moving direction.
*/
	const int& getMovingDirection();

/**
* Returns the collision object used for logic collision check.
* @return Returns the collision object.
*/
	gfx::CollisionObject* getPreCollisionObject();

/**
* Sets the scale of the actor.
* @param scale The scale to be used.
*/
	void setScale(utilities::Vec3Float scale);
	
/**
* Returns the scale of the actor.
* @return Returns the scale of the actor.
*/
	utilities::Vec3Float getScale();
	
/**
* Set the actor to be visible or not.
* @param visible The bool defining if the actor shall be visible.
*/
	void setVisible(bool visible);
	
/**
* Returns if the actor is visible.
* @return Returns a bool defining if the actor is visible.
*/
	bool getVisible();

/**
 * Sets the editor sync flag.
 * @param sync New value for mSync.
 */
	inline void setSync(bool sync){mSync = sync;}

/**
 * Retrieves the sync flag. Should be used when syncing actors while using the editor.
 * @return Returns mSync.
 */
	bool getSync() const {return mSync;}


protected:
	
	ActorTypes mActorType;
	KeyType mId;

	utilities::Position mPosition;
	utilities::Position mPrePosition;
	utilities::Position mDestPos;

	utilities::Direction mDirection;
	utilities::Direction mPreDirection;
	utilities::Direction mSubsetDirection;

	utilities::Vec3Float mScale;

	bool mMoving;
	bool mRotating;
	bool mSubsetRotating;
	bool mDead;
	bool mScaleChange;
	bool mVisible;
	bool mSync;

	float mSpeed;
	
	int mRotateDirection;
	int mSubsetRotateDirection;
	int mMovingDirection;

	utilities::MapCoordinate mSize;		/**< The size of the actor in map coordinates */
	
	
	gfx::TransformObject *mpTransformObject;
	gfx::CollisionObject *mpPrePosCollisionObject;

	static gfx::DX10Module *mpsDXModule;
	static PlayerInfo *mpsPlayerInfo;
	static ActorHandler *mpsActorHandler;

	


private:
	static KeyHandler *mpsKeyHandler;
};
}

std::istream& operator>>(std::istream &in, logic::Actor::ActorTypes& actorType);
std::istream& operator>>(std::istream &in, logic::Actor::PlayerMovingInfo& playerMovingInfo);
std::ostream& operator<<(std::ostream &out, const logic::Actor::PlayerMovingInfo& playerMovingInfo);
std::istream& operator>>(std::istream &in, logic::Actor::ActorSyncInfo& syncInfo);
std::ostream& operator<<(std::ostream &out, logic::Actor::ActorSyncInfo& syncInfo);
std::ostream& operator<<(std::ostream &out, const logic::Actor::ActorSyncInfo& syncInfo);
#endif