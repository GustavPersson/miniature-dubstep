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
 * This is going to be the base class of the ranged enemies weapons in the game, it will contain data and functions that is common between all enemy weapons, 
 * the subclasses will deal with operations specific for that weapontype
 */
#include "EnemyWeapon.h"
#include "GlobalTimer.h"

using namespace logic;

EnemyWeapon::EnemyWeapon(float damage, float weaponCd, float range)
{
	mDamage = damage;
    mWeaponCooldown = weaponCd;
	mRange = range;
	mBulletCounter = -1;
	mBullets = 0;
	mCooldownStartTime = 0;
	mCooldownCounter = 0;
	mpGlobalTimer = logic::GlobalTimer::getInstance();
	mpDX10Module = gfx::DX10Module::getInstance();
	mpActorHandler = logic::ActorHandler::getInstance();
	mpPlayerInfo = logic::PlayerInfo::getInstance();
}

EnemyWeapon::~EnemyWeapon()
{
}

float EnemyWeapon::getCooldown()
{
	return mWeaponCooldown;
}

float EnemyWeapon::getDamage()
{
	return mDamage;
}

float EnemyWeapon::getRange()
{
	return mRange;
}

void EnemyWeapon::setOwner(KeyType owner)
{
	mOwner = owner;
}

KeyType EnemyWeapon::getOwner()
{
	return mOwner;
}