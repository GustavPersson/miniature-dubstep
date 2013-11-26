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
 * This is the class for the pedestrian discouragement gun, with which you force away people trying to hijack your tank without hurting them
 */

#include "PedestrianGun.h"

using namespace logic;



PedestrianGun::PedestrianGun(float weaponCd, int weaponId) :
	Weapon(0,weaponCd,weaponId,0)
{

}


PedestrianGun::~PedestrianGun()
{

}


void PedestrianGun::fireWeapon()
{
  //check if cooldownCounter is higher than weaponCooldown 
  //send event with information like position and direction of fire to the GFX engine
  //send event to sound telling it to play pedestriangun fire sound

}