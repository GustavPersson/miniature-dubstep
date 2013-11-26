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

#ifndef __TANK_H__
#define __TANK_H__

#include "../Utilities/Macros.h"
#include "Unit.h"
#include "Weapon.h"

namespace logic
{

// forward declarations
//class Weapon;

/**
 * This is the tank class, it will provide functions and hold information needed for the tank
 */

class Tank:public Unit
{

public:
	enum TankTypes 
	{
		TankType_Invalid,
		TankType_CloseCombat,
		TankType_LongRange,
		TankType_Lim
	};

/**
 * Constructor
 * @param networkId the networkId of the player
 * @param hp the amount of health points the tank have
 * @param armor the amount of armor the tank have
 * @param tankType what type of tank it is
 * @param pos the position of this actor
 * @param dir the direction of this actor
 * @param playerMovingInfo moving info passed on from a driver
 */
	Tank(int networkId, TankTypes tankType, utilities::Position pos, utilities::Direction dir, Actor::PlayerMovingInfo playerMovingInfo);
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
 * @param weapon the id on the weapon that we want to set
 */
	void setWeapon(Weapon::WeaponTypes weapon);
/**
 * reduces the tanks hp by damage
 * @param damage the amount of damage to be taken
 */
	void reduceHp(float damage);

/**
 * Returns the subtype of the actor.
 * @return Return the subtype.
 */
	inline virtual int getActorSubType() const {return mTankType;}

/**
 * Returns the tanks type, either closerange or longrange.
 * @return The type of the tank.
 */
	TankTypes getTankType();
/**
 * function that returns a tanks weapon
 */
	Weapon* getWeapon();

/**
 * Calculates and retrieves the weapon selection cooldown in percentage.
 * @return Returns the cooldown percentage.
 */
	float getSelectionCooldownPercentage() const;

	bool getFiring();
/**
 * function that returns the name of the actor class
 */
	std::string getName(void) {return "Tank";}

/**
 * Toggles god mode in the tank
 */
	inline void toggleGodMode() {mGodMode = !mGodMode;}

private:	

	/**
	* Calculate the new speed of the tank
	* @param deltaTime Time elapsed since last frame.
	*/
	void calculateSpeed(float deltaTime);

	float mArmor;
	float mRotateSpeed;
	float mAcceleration;
	float mFriction;
	float mMaxSpeed;
	float mCooldownCounter;
	float mSwitchCooldown;
	float mCooldownStartTime;

	TankTypes mTankType;

	Weapon* mpWeapon;
	Weapon* mpPedestrianWeapon;

	GlobalTimer* mpGlobalTimer;

	bool mFireSecondary;

	bool mGodMode;
};
}

std::istream& operator>>(std::istream &in, logic::Tank::TankTypes& tankType);

#endif