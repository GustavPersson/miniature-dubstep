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
#include "Actor.h"
#include "EnemyWeapon.h"
#include "FireSpit.h"
#include "AcidSpit.h"
#include "PlasmaSpit.h"

#ifndef __ENEMY_H__
#define __ENEMY_H__


namespace logic
{

/**
 * This will be the baseclass of all enemies in the game
 */

class Enemy:public Actor
{

public:
	enum Sizes
	{
		Size_Invalid,
		Size_Small,
		Size_Medium,
		Size_Large,
	};
/**
 * Constructor
 * @param hp the enemies healthpoints
 * @param damage the amount of damage this enemy does
 * @param size the size of the enemy, used in damage calculations
 * @param aiStyle a flag that tells the ai in what way this enemy should behave
 * @param range how long range the unit have, incase of melee unit the range is a low value, probably between 0 and 1
 */
	Enemy(double hp,double damage,Sizes size,int aiStyle,float range);
/**
 * Deconstructor
 */
	~Enemy();
/**
 * Pure Virtual function, as the attack behavior will have to be different for most of the enemies
 */
	virtual void attack() = 0;
/**
 * pure virtual function implemented in subclasses
 * @param deltaTime time between calls, used to calculate movement
 */
	virtual void update(float deltaTime) = 0;
/**
 * handles all events relevant to enemy and its subclasses
 * @param eventMessage the event to be handled
 */
	virtual void handleEventId(EventData const &eventMessage);
/**
 * GetFunction for size
 */
	Sizes getSize();
/**
 * Reduce the enemies hp
 * @param damage The amount of hp to reduce.
 */
	void reduceHp(double damage);
/**
 * gets the enemies damage
 */
	double getDamage();

	float getRange();

protected:
	PlayerInfo *mpPlayerInfo;
	EnemyWeapon *mpEnemyWeapon;
	bool mFire;

private:
	double mHealthPoints;
	double mDamage;
	Sizes mSize;
	int mAiStyle;
	float mRange;
};
}

#endif