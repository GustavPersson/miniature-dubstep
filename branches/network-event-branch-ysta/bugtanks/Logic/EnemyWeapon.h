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
#include "ActorHandler.h"
#include "Fire.h"
#include "Acid.h"
#include "Plasma.h"
#include "Events.h"
#include "EventManager.h"

#ifndef __ENEMYWEAPON_H__
#define __ENEMYWEAPON_H__


namespace logic
{

/**
 * This is going to be the base class of the ranged enemies weapons in the game, it will contain data and functions that is common between all enemy weapons, 
 * the subclasses will deal with operations specific for that weapontype
 */

class EnemyWeapon
{

public:
/**
 * Constructor
 * @param damage How much damage this weapon does
 * @param weaponCooldown How long time you have to wait before being able to fire again
 * @param range this weapons range
 */
	EnemyWeapon(double damage,float weaponCooldown,float range);
/**
 * Pure virtual function, because the weapons behave differently we need different implementations in the subclasses
 */
	virtual void fireWeapon() = 0;
/**
 * Function that returns the weapons cooldown
 */
	float getCooldown();
/**
 * Function that returns the weapons damage
 */
	double getDamage();
/**
 * Function that returns the weapons range
 */
	float getRange();

/**
 * Function that sets the owner of this weapon
 */
	void setOwner(KeyType owner);
/**
 * Function that gets the owner of this weapon
 */
	KeyType getOwner();

protected:
	GlobalTimer *mpGlobalTimer;
	gfx::DX10Module *mpDX10Module;
	ActorHandler *mpActorHandler;
	int objectCounter;
	int bullets;
	float cooldownStartTime;
	float cooldownCounter;

private:
	KeyType mOwner;
	double mDamage;
	float mWeaponCooldown;
	float mRange;
};
}

#endif