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
 * 
 * @section DESCRIPTION
 *
 * This is the base class of all kinds of ammo fired from the different weapons 
 */

#include "Munitions.h"
#include "Events.h"


using namespace logic;


Munitions::Munitions(int networkId, utilities::Position pos, utilities::Direction dir, int efficiency, float damage) : DynamicActor(pos, dir, networkId)
{
	mActorType	= ActorType_Munition;
	mWeaponType = Weapon::WeaponTypes_Invalid;
	mOwnerType	= ActorType_Invalid;
	mOwner		= INVALID_KEY;

	mOwnerDead	= false;
	mVisible	= true;

	mEfficiency = efficiency;
	mDamage		= damage;
}

utilities::Position Munitions::getOrigin()
{
	return mPosition;
}

int Munitions::getEfficiency()
{
	return mEfficiency;
}
float Munitions::getDamage()
{
	return mDamage;
}

void Munitions::setOwner(KeyType owner, Actor::ActorTypes ownerActorType)
{
	mOwner = owner;
	mOwnerType = ownerActorType;
}

KeyType Munitions::getOwner()
{
	return mOwner;
}
Actor::ActorTypes Munitions::getOwnerType()
{
	return mOwnerType;
}

void Munitions::setWeaponType(Weapon::WeaponTypes weaponType)
{
	mWeaponType = weaponType;
}

Weapon::WeaponTypes Munitions::getWeaponType()
{
	return mWeaponType;
}

void Munitions::startDestroyTimer()
{
	mDestroyTimer.start();
	mOwnerDead = true;
}

void Munitions::setVisible(bool visible)
{
	mVisible = visible;
}

bool Munitions::getVisible()
{
	return mVisible;
}

void Munitions::handleEventId(EventData const &eventMessage)
{
}

std::istream& operator>>(std::istream &in, logic::Munitions::MunitionTypes& munitionType)
{
	int intMunitionType;
	in >> intMunitionType;
	munitionType = static_cast<logic::Munitions::MunitionTypes>(intMunitionType);
	return in;
}
