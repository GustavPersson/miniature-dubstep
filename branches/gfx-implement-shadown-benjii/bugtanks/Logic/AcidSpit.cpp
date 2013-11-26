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

#include "AcidSpit.h"

using namespace logic;

AcidSpit::AcidSpit(double damage, float weaponCooldown, float range):
		EnemyWeapon(damage,weaponCooldown,range)
{
	mpAcid = NULL;
	for(int i = 0; i < 2;i++)
	{
		keys[i] = INVALID_KEY;
	}
}

AcidSpit::~AcidSpit()
{
	SAFE_DELETE(mpAcid);
}

void AcidSpit::fireWeapon()
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
		 /*gfx::TransformObject *AcidSpitObj = mpDX10Module->createObjectShape(gfx::ShapeTypes::CUBE);
		 AcidSpitObj->setScale(D3DXVECTOR3(2.0f,2.0f,2.0f));
		 AcidSpitObj->setColor(D3DXCOLOR(0.0f,1.0f,0.0f,1.0f));*/
	 
		 mpAcid = myNew Acid(position,0,EnemyWeapon::getDamage());
		 mpActorHandler->insertActor(mpAcid);
		 mpAcid->setMoving(true);
	  
		 //AcidSpitObj->setID(mpAcid->getKey());
		 keys[objectCounter] = mpAcid->getKey();
		 bullets++;
	   }
	    utilities::Direction aimDir;
		aimDir.x = mpActorHandler->getActor(mpPlayerInfo->getTankActorId())->getPosition().x - position.x;
		aimDir.y = mpActorHandler->getActor(mpPlayerInfo->getTankActorId())->getPosition().y - position.y;
		aimDir.z = mpActorHandler->getActor(mpPlayerInfo->getTankActorId())->getPosition().z - position.z;
		aimDir.normalize();
	
		mpActorHandler->getActor(keys[objectCounter])->setDirection(aimDir.x,aimDir.y,aimDir.z);

	   // mpDX10Module->getRenderObject(keys[objectCounter])->setPosition(D3DXVECTOR3(position.x,position.y,position.z));
		//mpActorHandler->getActor(keys[objectCounter])->setDirection(direction.x,direction.y,direction.z);
		mpActorHandler->getActor(keys[objectCounter])->setPosition(position.x,position.y,position.z);
		//mpDX10Module->getRenderObject(keys[objectCounter])->setPosition(D3DXVECTOR3(position.x,position.y,position.z));
		
		D3DXVECTOR3 origin;
		origin.x = position.x;
		origin.y = position.y;
		origin.z = position.z;
		D3DXVECTOR3 dir;
		dir.x = direction.x;
		dir.y = direction.y;
		dir.z = direction.z;

		LogicQuadTree::getInstance()->collisionRay(origin, dir, EnemyWeapon::getOwner(), mpAcid->getKey()); 

		if(objectCounter == 1)
			objectCounter = -1;
	  }
	}
}