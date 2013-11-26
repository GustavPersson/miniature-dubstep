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
#include "DynamicActor.h"
#include "Weapon.h"

#ifndef __MUNITIONS_H__
#define __MUNITIONS_H__

namespace logic
{

/**
 * This is the base class of all kinds of ammo fired from the different weapons 
 */

class Munitions : public DynamicActor
{

public:

	enum MunitionTypes
	{
		MunitionType_Invalid,
		MunitionType_Beam,
		MunitionType_Bullet,
		MunitionType_Flame,
		MunitionType_Pulse,
		MunitionType_Acid,
		MunitionType_Fire,
		MunitionType_Plasma,
		MunitionType_Lim
	};

/**
 * Constructor 
 * @param networkId the network id of the player
 * @param position the origin of the projectile
 * @param direction the direction of the projectile
 * @param efficiency the weapon shooting this ammos efficiency rating
 * @param damage the damage of the weapon shooting this ammo
 */
	Munitions(int networkId, utilities::Position position, utilities::Direction direction, int efficiency, float damage);
/**
 * Pure virtual update
 * @param deltaTime the time between calls, used in calculating movement
 */
	virtual void update(float deltaTime) = 0;
/**
 * Pure virtual handleEvent
 * @param eventMessage the event to be handled
 */
	virtual void handleEventId(EventData const &eventMessage);
/**
 * method for getting the origin
 */
	utilities::Position getOrigin();
/**
 * method for getting weaponefficiency
 */
	int getEfficiency();
/**
 * method for getting damage
 */
	float getDamage();

/**
 * Setting the owner of the munition
 * @param owner the id of the actor that fired this munition
 * @param ownerActorType the type of the owner
 */
	void setOwner(KeyType owner, Actor::ActorTypes ownerActorType);
/**
 * Getting the id of munitions owner
 */
	KeyType getOwner();
/**
 * Getting the type of the munitions owner
 */
	Actor::ActorTypes getOwnerType();
/**
 * Starts the timer for destroying munitions
 */
	void startDestroyTimer();

/**
 * Sets the type of weapon firing this munition
 * @param weaponType the type of weapon creating this munition
 */
	void setWeaponType(Weapon::WeaponTypes weaponType);

/**
 * Returns the subtype of the actor.
 * @return Return the subtype.
 */
	inline virtual int getActorSubType() const {return mMunitionType;}

/**
 * Gets the type of weapon firing this munition
 */
	Weapon::WeaponTypes getWeaponType();

protected:
	MunitionTypes mMunitionType;
	utilities::Timer mDestroyTimer;
	bool mOwnerDead;
	Weapon::WeaponTypes mWeaponType;
	KeyType mOwner;

private:
	int mEfficiency;
	float mDamage;	
	Actor::ActorTypes mOwnerType;

};
}

std::istream& operator>>(std::istream &in, logic::Munitions::MunitionTypes& munitionType);

#endif
