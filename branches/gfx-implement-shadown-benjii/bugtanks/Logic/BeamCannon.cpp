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
 * @section DESCRIPTION
 *
 * This is a subclass to weapon, Beam Cannon is the longrange tanks third weapon
 */

#include "BeamCannon.h"

using namespace logic;


BeamCannon::BeamCannon(double damage, float weaponCd, int weaponId, int efficiency) :
	Weapon(damage,weaponCd,weaponId,efficiency)
{
}


BeamCannon::~BeamCannon()
{
	
}

void BeamCannon::fireWeapon()
{
  //check if cooldownCounter is higher than weaponCooldown 
  //send event with information like position and direction of fire to the GFX engine
  //send event to sound telling it to play beam cannon fire sound
}