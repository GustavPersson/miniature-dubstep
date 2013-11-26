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
 * This is the class for the plasma beetle
 */

#include "PlasmaBeetle.h"

using namespace logic;


PlasmaBeetle::PlasmaBeetle(double hp,double damage,Sizes size,int aiStyle,float range) :
	Enemy(hp,damage,size,aiStyle,range)
{
	mFire = false;
	mpEnemyWeapon = myNew PlasmaSpit(damage, 1.0f,range);
	mpEnemyWeapon->setOwner(this->getKey());
}

PlasmaBeetle::~PlasmaBeetle()
{
	SAFE_DELETE(mpEnemyWeapon);
}

void PlasmaBeetle::attack()
{
 //if enemy is in range of tank
   if((ActorHandler::getInstance()->getActor(mpPlayerInfo->getTankActorId())->getPosition() - mPosition).longerThan(Enemy::getRange(), false) == false)
	{
		mpEnemyWeapon->fireWeapon();
	}
}

void PlasmaBeetle::update(float deltaTime)
{
	if(mFire)
	{	
		attack();
	}
	Actor::mPosition.x += (BEETLE_MOVE_SPEED * Actor::mDirection.x * deltaTime);
	Actor::mPosition.y += (BEETLE_MOVE_SPEED * Actor::mDirection.y * deltaTime);
	Actor::mPosition.z += (BEETLE_MOVE_SPEED * Actor::mDirection.z * deltaTime);

	mpRenderObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
}


void PlasmaBeetle::handleEventId(EventData const &eventMessage)
{
	Enemy::handleEventId(eventMessage);
}