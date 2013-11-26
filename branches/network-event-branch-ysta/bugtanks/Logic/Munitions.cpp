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


using namespace logic;


Munitions::Munitions(utilities::Position position, int efficiency, double damage)
{
	mPosition.x = position.x;
	mPosition.y = position.y;
	mPosition.z = position.z;

	mEfficiency = efficiency;
	mDamage = damage;
	
	mActorType = ActorType_Munition;
}

utilities::Position Munitions::getOrigin()
{
	return mPosition;
}

int Munitions::getEfficiency()
{
	return mEfficiency;
}
double Munitions::getDamage()
{
	return mDamage;
}