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
#include "Bullet.h"
#include "Events.h"
#include "EventManager.h"

#ifndef __WEAPON_H__
#define __WEAPON_H__


namespace logic
{

/**
 * This is going to be the base class of all weapons in the game, it will contain data and functions that is common between all weapons, 
 * the subclasses will deal with operations specific for that weapontype
 */

class Weapon
{

public:
/**
 * Constructor
 * @param damage How much damage this weapon does
 * @param weaponCooldown How long time you have to wait before being able to fire again
 * @param weaponId Id for gamelogical recognition
 * @param efficiency Tells against which kind of enemy this weapon is efficient
 */
	Weapon(double damage,float weaponCooldown,int weaponId, int efficiency);
/**
 * Pure virtual function, because the weapons behave differently we need different implementations in the subclasses
 */
	virtual void fireWeapon() = 0;
/**
 * When an enemy gets hit an event is triggered sending information about what enemy type got hit and by what weapon type
 * @param weaponEfficiency the efficiency on the weapon that was used, information comes from event
 * @param enemySize the size of the hit enemy, information come from event
 * @param damage the damage of the weapon that was used, again comes from event
 */
	static double calculateDamage(int weaponEfficiency,int enemySize,double damage);
/**
 * Function that returns the weapons cooldown
 */
	float getCooldown();
/**
 * Function that returns the weapons damage
 */
	double getDamage();
/**
 * Function that returns the weapons efficiency
 */
	int getEfficiency();

protected:
	GlobalTimer *mpGlobalTimer;
	gfx::DX10Module *mpDX10Module;
	ActorHandler *mpActorHandler;
	PlayerInfo *mpPlayerInfo;
	int objectCounter;
	int bullets;
	float cooldownStartTime;
	float cooldownCounter;

private:
	double mDamage;
	int mWeaponId;
	int mEfficiency;
	float mWeaponCooldown;
};
}

#endif