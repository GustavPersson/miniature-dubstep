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

#include "PlasmaSpit.h"

using namespace logic;

PlasmaSpit::PlasmaSpit(double damage, float weaponCooldown, float range):
		EnemyWeapon(damage,weaponCooldown,range)
{
	mpPlasma = NULL;
	for(int i = 0; i < 2;i++)
	{
		keys[i] = INVALID_KEY;
	}
}

PlasmaSpit::~PlasmaSpit()
{
	SAFE_DELETE(mpPlasma);
}

void PlasmaSpit::fireWeapon()
{
	cooldownCounter = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds) - cooldownStartTime;
	//check if cooldownCounter is higher than weaponCooldown
    if(cooldownCounter > EnemyWeapon::getCooldown())
    {
	  objectCounter++;	
	  cooldownStartTime = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds);
	
	 if(objectCounter != 2)
	 {
	   utilities::Position position; 	
	   position.x = mpActorHandler->getActor(EnemyWeapon::getOwner())->getPosition().x;
	   position.y = mpActorHandler->getActor(EnemyWeapon::getOwner())->getPosition().y;
	   position.z = mpActorHandler->getActor(EnemyWeapon::getOwner())->getPosition().z;

	   utilities::Direction direction; 	
	   direction.x = mpActorHandler->getActor(EnemyWeapon::getOwner())->getDirection().x;
	   direction.y = mpActorHandler->getActor(EnemyWeapon::getOwner())->getDirection().y;
	   direction.z = mpActorHandler->getActor(EnemyWeapon::getOwner())->getDirection().z;

	   if(bullets <= 1)
	   {     
		 gfx::TransformObject *PlasmaSpitObj = mpDX10Module->createObjectShape(gfx::ShapeTypes::CUBE);
		 PlasmaSpitObj->setScale(D3DXVECTOR3(2.0f,2.0f,2.0f));
		 PlasmaSpitObj->setColor(D3DXCOLOR(0.0f,1.0f,0.0f,1.0f));
	 
		 mpPlasma = myNew Plasma(position,0,EnemyWeapon::getDamage());
		 mpActorHandler->insertActor(mpPlasma);
		 mpPlasma->setMoving(true);
	  
		 PlasmaSpitObj->setID(mpPlasma->getKey());
		 keys[objectCounter] = mpPlasma->getKey();
		 bullets++;
	   }

	    mpDX10Module->getRenderObject(keys[objectCounter])->setPosition(D3DXVECTOR3(position.x,position.y,position.z));
		mpActorHandler->getActor(keys[objectCounter])->setDirection(direction.x,direction.y,direction.z);
		mpActorHandler->getActor(keys[objectCounter])->setPosition(position.x,position.y,position.z);
		
		D3DXVECTOR3 origin;
		origin.x = position.x;
		origin.y = position.y;
		origin.z = position.z;
		D3DXVECTOR3 dir;
		dir.x = direction.x;
		dir.y = direction.y;
		dir.z = direction.z;

		LogicQuadTree::getInstance()->collisionRay(origin, dir, EnemyWeapon::getOwner(), mpPlasma->getKey()); 

		if(objectCounter == 1)
			objectCounter = -1;
	  }
	}
}