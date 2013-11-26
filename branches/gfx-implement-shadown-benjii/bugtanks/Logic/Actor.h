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

#include "../Utilities/Macros.h"
#include "../Utilities/Vectors.h"
#include "KeyHandler.h"
#include "GlobalTimer.h"
#include "PlayerInfo.h"
#include "Events.h"
#include "EventManager.h"
#include "LogicQuadTree.h"
#include <cassert>
#include "../GFX/DX10Module.h"

#ifndef __ACTOR_H__
#define __ACTOR_H__


namespace logic
{

const float ANT_MOVE_SPEED		= 3.0f;
const float SPIDER_MOVE_SPEED	= 0.2f;
const float BEETLE_MOVE_SPEED	= 0.2f;
const float PLAYER_MOVE_SPEED	= 0.2f;
const float TANK_MOVE_SPEED		= 50.0f;
const float BULLET_MOVE_SPEED	= 300.0f;
const float FLAME_MOVE_SPEED	= 0.2f;
const float BEAM_MOVE_SPEED		= 0.2f;
const float PULSE_MOVE_SPEED	= 0.2f;
const float FIRE_MOVE_SPEED		= 250.0f;
const float PLASMA_MOVE_SPEED	= 0.2f;
const float ACID_MOVE_SPEED		= 230.0f;

/**
 * This is the topmost in the actor hierarchy, it will be used to store all game actors in a map
 */

class Actor
{
public:
	enum ActorType
	{
		ActorType_Enemy, 
		ActorType_Tank,
		ActorType_Driver,
		ActorType_Munition,
		ActorType_StaticEnvironment,
		ActorType_Invalid
	};
/**
 * Constructor, assigns the same key used in the map to the actor itself
 */
    Actor();
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
	KeyType getKey();
/**
 * function that returns position struct
 */
	utilities::Position getPosition();
/**
 * function that returns direction struct
 */ 
	utilities::Direction getDirection();
/**
 * function that sets the actors position to the given value
 * @param x the x-value
 * @param y the y-value
 * @param z the z-value
 */
	void setPosition(float x, float y, float z);
/** 
 * function that sets the actors direction to the given value
 * @param x the x-value
 * @param y the y-value
 * @param z the z-value
 */
	void setDirection(float x, float y, float z);
/**
 * function that returns the type of actor
 */
	ActorType getActorType();
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
 * function that sets the direction of subset
 */
	utilities::Direction getSubsetDirection();

	bool getMoving();

	bool getRotating();

protected:
	utilities::Position mPosition;
	utilities::Direction mDirection;
	utilities::Direction mSubsetDirection;
	bool mMoving;
	bool mRotating;
	bool mSubsetRotating;
	float mSpeed;
	int mRotateDirection;
	int mSubsetRotateDirection;
	int mForward;
	ActorType mActorType;
	
	gfx::RenderObject *mpRenderObject;
	
	KeyType mId;	
	
private:
	static KeyHandler *mpsKeyHandler;
	
	
	
};
}

#endif