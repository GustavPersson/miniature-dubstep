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

#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "../Utilities/Macros.h"
#include "../Utilities/Vectors.h"
#include "../GFX/DX10Module.h"
#include "LogicQuadTree.h"

namespace logic
{

// Forward declarations
class ActorHandler;
class PlayerInfo;
class EventData;
class GlobalTimer;
//class EventManager;

/**
 * This is going to be the base class of all weapons in the game, it will contain data and functions that is common between all weapons, 
 * the subclasses will deal with operations specific for that weapontype
 */

class Weapon
{

public:

	enum WeaponTypes
	{
		WeaponTypes_Invalid,
		WeaponTypes_PedestrianGun,
		WeaponTypes_Minigun,
		WeaponTypes_Flamethrower,
		WeaponTypes_PulseCannon,
		WeaponTypes_Cannon,
		WeaponTypes_GaussCannon,
		WeaponTypes_BeamCannon,
		WeaponTypes_Shotgun
	};

/**
 * Constructor
 * @param damage How much damage this weapon does
 * @param weaponCooldown How long time you have to wait before being able to fire again
 * @param efficiency Tells against which kind of enemy this weapon is efficient
 */
	Weapon(float damage, float weaponCooldown, int efficiency);

/**
 * Destructor
 */
	virtual ~Weapon();
/**
 * Pure virtual function, because the weapons behave differently we need different implementations in the subclasses
 */
	virtual void fireWeapon(KeyType ownerId) = 0;
/**
 * When an enemy gets hit an event is triggered sending information about what enemy type got hit and by what weapon type
 * @param weaponEfficiency the efficiency on the weapon that was used, information comes from event
 * @param enemySize the size of the hit enemy, information come from event
 * @param damage the damage of the weapon that was used, again comes from event
 */
	static float calculateDamage(int weaponEfficiency, int enemySize, float damage);
/**
 * Function that returns the weapons cooldown
 */
	float getCooldown();
/**
 * Function that returns the weapons damage
 */
	float getDamage();
/**
 * Function that returns the weapons efficiency
 */
	int getEfficiency();

	/**
	* Returns the id for the different weapons.
	* @return The id type of the weapon.
	*/
	WeaponTypes getWeaponType();

	virtual void addBullet(KeyType bulletId) = 0;

protected:
	GlobalTimer *mpGlobalTimer;
	gfx::DX10Module *mpDX10Module;
	ActorHandler *mpActorHandler;
	PlayerInfo *mpPlayerInfo;
	LogicQuadTree *mpLogicQuadTree;

	int mBulletCounter;
	int mBullets;
	float mCooldownStartTime;
	float mCooldownCounter;
	
	WeaponTypes mWeaponType;

private:
	float mDamage;	
	int mEfficiency;
	float mWeaponCooldown;
};
}

std::istream& operator>>(std::istream &in, logic::Weapon::WeaponTypes& weaponType);

#endif