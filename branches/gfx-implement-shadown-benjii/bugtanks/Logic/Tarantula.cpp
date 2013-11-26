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
 * This is the class for the Tarantula
 */

#include "Tarantula.h"

using namespace logic;

Tarantula::Tarantula(double hp,double damage,Sizes size,int aiStyle,float range) :
	Enemy(hp,damage,size,aiStyle,range)
{
}

Tarantula::~Tarantula()
{

}

void Tarantula::attack()
{
 //if enemy is in range of tank
 //send event to have the enemy perform his attack animation
 //send event to collision checking for a hit
}

void Tarantula::update(float deltaTime)
{   

	Actor::mPosition.x += (SPIDER_MOVE_SPEED * Actor::mDirection.x * deltaTime);
	Actor::mPosition.y += (SPIDER_MOVE_SPEED * Actor::mDirection.y * deltaTime);
	Actor::mPosition.z += (SPIDER_MOVE_SPEED * Actor::mDirection.z * deltaTime);

}


void Tarantula::handleEventId(EventData const &eventMessage)
{
	Enemy::handleEventId(eventMessage);
}