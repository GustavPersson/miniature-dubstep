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

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "../Utilities/Macros.h"
#include "../Ai/AiBehaviors.h"
#include "Unit.h"

namespace logic
{

// forward declarations
class PlayerInfo;
class EnemyWeapon;

/**
 * This will be the baseclass of all enemies in the game
 */
class Enemy:public Unit
{

public:
	enum Sizes
	{
		Size_Invalid,
		Size_Small,
		Size_Medium,
		Size_Large,
	};

	enum EnemyTypes
	{
		EnemyType_Invalid,
		EnemyType_Ant,
		EnemyType_KamikazeAnt,
		EnemyType_FireAnt,
		EnemyType_Tarantula,
		EnemyType_BlackWidow,
		EnemyType_Jumper,
		EnemyType_Scarab,
		EnemyType_PlasmaBeetle,
		EnemyType_Lim
	};
/**
 * Constructor
 * @param networkId the network id of the player
 * @param hp the enemies healthpoints
 * @param damage the amount of damage this enemy does
 * @param size the size of the enemy, used in damage calculations
 * @param range how long range the unit have, incase of melee unit the range is a low value, probably between 0 and 1
 * @param pos the position of this actor
 * @param dir the direction of this actor
 * @param mapSize the size of this enemy on the mapgrid
 * @param aiBehavior the ai behavior of the enemy
 */
	Enemy(
		int networkId,
		float hp,
		float damage,
		Sizes size,
		float range,
		utilities::Position pos,
		utilities::Direction dir,
		const utilities::MapCoordinate& mapSize,
		ai::AiBehaviors aiBehavior);
/**
 * Destructor
 */
	~Enemy();
/**
 * Pure Virtual function, as the attack behavior will have to be different for most of the enemies
 */
	virtual void attack() = 0;
/**
 * pure virtual function implemented in subclasses
 * @param deltaTime time between calls
 */
	virtual void update(float deltaTime);
/**
 * handles all events relevant to enemy and its subclasses
 * @param eventMessage the event to be handled
 */
	virtual void handleEventId(EventData const &eventMessage);
/**
 * GetFunction for size
 */
	Sizes getEnemySize();
/**
 * Reduce the enemies hp
 * @param damage The amount of hp to reduce.
 */
	void reduceHp(float damage, int networkId, KeyType shooter);
/**
 * gets the enemies damage
 */
	float getDamage();

	/**
	 * Returns the subtype of the actor.
	 * @return Return the subtype.
	 */
	inline virtual int getActorSubType() const {return mEnemyType;}

/**
* Gets the enemy type
* @return The enemy type.
*/
	EnemyTypes getEnemyType();

/**
 * Get the fire range of the enemy.
 * @return The range as a float.
 */
	float getRange();
/**
 * Get the enemies weapon
 * @return pointer to the enemies weapon
 */
	EnemyWeapon* getEnemyWeapon();
/**
 * Get the enemies current hp
 * @return mHealthPoints, the current hp of the enemy
 */
	float getHealthPoints();

/**
* Returns the ai behavior of the enemy
* @return ai behavior of the enemy
*/
	inline ai::AiBehaviors getAiBehavior() const {return mAiBehavior;}

/**
 * Sets the fire status on ranged enemies
 * @param fire true or false depending upon if enemy should fire or not
 */
   void setFire(bool fire);

   bool getFiring();
/**
 * Set the target of the enemy
 * @param targetId the id of the target to fire at
 */
   inline void setTarget(const KeyType& targetId)
   {
	   mTarget = targetId;
	   setFocalUnit(mTarget);
   }

protected:
	EnemyWeapon *mpEnemyWeapon;
	EnemyTypes mEnemyType;
	float mCooldownCounter;
	GlobalTimer *mpGlobalTimer;
	float mCooldownStartTime;
	float mDamage;
	KeyType mTarget;

private:
	ai::AiBehaviors mAiBehavior;	
	Sizes mSize;
	float mRange;
};
}

std::istream& operator>>(std::istream &in, logic::Enemy::EnemyTypes& enemyType);

#endif