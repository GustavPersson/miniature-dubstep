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
#include "Weapon.h"
#include "Minigun.h"
#include "Flamethrower.h"
#include "PulseCannon.h"
#include "Cannon.h"
#include "GaussCannon.h"
#include "BeamCannon.h"

#ifndef __TANK_H__
#define __TANK_H__

namespace logic
{

/**
 * This is the tank class, it will provide functions and hold information needed for the tank
 */

class Tank:public Actor
{

public:
	enum TankTypes 
	{
		TankType_CloseCombat,
		TankType_LongRange,
	};
/**
 * Constructor
 * @param hp the amount of health points the tank have
 * @param armor the amount of armor the tank have
 * @param tankType what type of tank it is
 */
	Tank(double hp,int armor, TankTypes tankType);
/**
 * Deconstructor
 */
	~Tank();
/**
 * When the tank reaches 0 hitpoints this function will be called to eject the driver
 */
	void tankDestroyed();
/**
 * fires the tanks main weapon
 */
	void fireMain();
/**
 * fires the tanks pedestrian discouragement system
 */
	void firePedestrianGun();
/**
 * update tank position
 * @param deltaTime the time between calls, used in calculating movement
 */ 
	void update(float deltaTime);
 /**
  * handles various events for tank
  * @param eventMessage the event to be handled
  */
	void handleEventId(EventData const &eventMessage);
 /**
  * sets the tanks weapon
  * @param weaponId the id on the weapon that we want to set
  */
	void setWeapon(int weaponId);
 /**
  * reduces the tanks hp by damage
  * @param damage the amount of damage to be taken
  */
	void reduceHp(double damage);

	TankTypes getTankType();

private:
	
	

	/**
	* Calculate the new speed of the tank
	* @param deltaTime Time elapsed since last fram.
	*/
	void calculateSpeed(float deltaTime);

	double mHealthPoints;
	int mArmor;
	TankTypes mTankType;
	Weapon* mpWeapon;
	bool mFire;	
	
	enum Weapons
	{
		Weapon_PedestrianGun,
		Weapon_Minigun,
		Weapon_Flamethrower,
		Weapon_PulseCannon,
		Weapon_Cannon,
		Weapon_GaussCannon,
		Weapon_BeamCannon,
	};
};
}

#endif