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

using namespace std;

using namespace logic;


Weapon::Weapon(double damage,float weaponCd,int weaponId,int efficiency)
{
	mDamage = damage;
    mWeaponCooldown = weaponCd;
	mWeaponId = weaponId;
	mEfficiency = efficiency;
	objectCounter = -1;
	bullets = 0;
	cooldownStartTime = 0;
	cooldownCounter = 0;
	mpGlobalTimer = logic::GlobalTimer::getInstance();
	mpDX10Module = gfx::DX10Module::getInstance();
	mpActorHandler = logic::ActorHandler::getInstance();
	mpPlayerInfo = logic::PlayerInfo::getInstance();
}

double Weapon::calculateDamage(int weaponEfficiency,int enemySize,double damage)
{
	double finalDamage = 0; 
   //if weaponType equals PedestrianGun
   //send event to AI to make the hit pedestrian switch behavior to fleeing(or something) 	

   //if weaponType is efficient vs enemyType
	//set the final damage to weapontypes damage times 1
	if(weaponEfficiency == enemySize)
	{
		finalDamage = damage * 1.0;
	}
   //else if weaponType is inefficient vs enemyType
   //set the final damage to weapontypes damage times 0.5
	else if((abs(weaponEfficiency - enemySize)) == 2) 
	{
	  finalDamage = damage * 0.5;
	}
   //else set the final damage to weapontypes damage times 0.75
	else
	{
		finalDamage = damage * 0.75;
	}
	return finalDamage;
}

float Weapon::getCooldown()
{
	return mWeaponCooldown;
}

double Weapon::getDamage()
{
	return mDamage;
}

int Weapon::getEfficiency()
{
	return mEfficiency;
}