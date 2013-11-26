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
 * This is going to be the base class of all weapons in the game, it will contain data and functions that is common between all weapons, 
 * the subclasses will deal with operations specific for that weapontype
 */
#include "Weapon.h"
#include <cmath>
#include <iostream>
#include "GlobalTimer.h"
#include "ActorHandler.h"

using namespace std;

using namespace logic;


Weapon::Weapon(float damage, float weaponCd, int efficiency)
{
	mDamage = damage;
    mWeaponCooldown = weaponCd;
	mEfficiency = efficiency;
	mBulletCounter = -1;
	mBullets = 0;
	mCooldownStartTime = 0;
	mCooldownCounter = 0;
	mpGlobalTimer = logic::GlobalTimer::getInstance();
	mpDX10Module = gfx::DX10Module::getInstance();
	mpActorHandler = logic::ActorHandler::getInstance();
	mpPlayerInfo = logic::PlayerInfo::getInstance();
	mpLogicQuadTree = logic::LogicQuadTree::getInstance();
}

Weapon::~Weapon()
{
}

float Weapon::calculateDamage(int weaponEfficiency, int enemySize, float damage)
{
	float finalDamage = 0; 
   //if weaponType equals PedestrianGun
   //send event to AI to make the hit pedestrian switch behavior to fleeing(or something) 	

   //if weaponType is efficient vs enemyType
	//set the final damage to weapontypes damage times 1
	if(weaponEfficiency == enemySize)
	{
		finalDamage = damage * 1.0f;
	}
   //else if weaponType is inefficient vs enemyType
   //set the final damage to weapontypes damage times 0.5
	else if((abs(weaponEfficiency - enemySize)) == 2) 
	{
	  finalDamage = damage * 0.5f;
	}
   //else set the final damage to weapontypes damage times 0.75
	else
	{
		finalDamage = damage * 0.75f;
	}
	return finalDamage;
}

float Weapon::getCooldown()
{
	return mWeaponCooldown;
}

float Weapon::getCooldownPercentage() const
{
	float time = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds) - mCooldownStartTime;
	return 1.0f - time / mWeaponCooldown;
}

float Weapon::getDamage()
{
	return mDamage;
}

int Weapon::getEfficiency()
{
	return mEfficiency;
}

Weapon::WeaponTypes Weapon::getWeaponType()
{
	return mWeaponType;
}

std::istream& operator>>(std::istream &in, logic::Weapon::WeaponTypes& weaponType)
{
	int intWeaponType;
	in >> intWeaponType;
	weaponType = static_cast<logic::Weapon::WeaponTypes>(intWeaponType);
	return in;
}