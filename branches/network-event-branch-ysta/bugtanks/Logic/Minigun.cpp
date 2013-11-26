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
 * This is a subclass to weapon, minigun is the closecombat tanks basic weapon
 */

#include "Minigun.h"

using namespace logic;


Minigun::Minigun(double damage, float weaponCd,int weaponId,int efficiency) :
	Weapon(damage,weaponCd,weaponId,efficiency)
{
	mpBullet = NULL;
	for(int i = 0; i < 12;i++)
	{
		keys[i] = INVALID_KEY;
	}
}

Minigun::~Minigun()
{
	SAFE_DELETE(mpBullet);
}


void Minigun::fireWeapon()
{

  //check if cooldownCounter is higher than weaponCooldown
	cooldownCounter = mpGlobalTimer->getTime(logic::Timer::ReturnType_Seconds) - cooldownStartTime;
  if(cooldownCounter > Weapon::getCooldown())
  {
	  safeQueueEvent(EventDataPtr(myNew EvtData_From_WeaponFired()));
	  objectCounter++;	
	  cooldownStartTime = mpGlobalTimer->getTime(logic::Timer::ReturnType_Seconds);

	if(objectCounter != 12)
	{
	  utilities::Position position; 	
	  position.x = mpActorHandler->getActor(mpPlayerInfo->getTankActorId())->getPosition().x;
	  position.y = mpActorHandler->getActor(mpPlayerInfo->getTankActorId())->getPosition().y;
	  position.z = mpActorHandler->getActor(mpPlayerInfo->getTankActorId())->getPosition().z;

	  utilities::Direction direction; 	
	  direction.x = mpActorHandler->getActor(mpPlayerInfo->getTankActorId())->getSubsetDirection().x;
	  direction.y = mpActorHandler->getActor(mpPlayerInfo->getTankActorId())->getSubsetDirection().y;
	  direction.z = mpActorHandler->getActor(mpPlayerInfo->getTankActorId())->getSubsetDirection().z;
	  //do not create more than specified objects	
	  if(bullets <= 12)
	  {     
		gfx::TransformObject *MinigunRoundObj = mpDX10Module->createObjectShape(gfx::ShapeTypes::CUBE);
		MinigunRoundObj->setScale(D3DXVECTOR3(1.0f,1.0f,1.0f));
		MinigunRoundObj->setColor(D3DXCOLOR(1.0f,0.0f,0.0f,1.0f));
	 
		mpBullet = myNew Bullet(position,Weapon::getEfficiency(),Weapon::getDamage());
		mpActorHandler->insertActor(mpBullet);
		mpBullet->setMoving(true);
	  
		MinigunRoundObj->setID(mpBullet->getKey());
		keys[objectCounter] = mpBullet->getKey();
		bullets++;
	  }	 
	

		mpDX10Module->getRenderObject(keys[objectCounter])->setPosition(D3DXVECTOR3(position.x,position.y + 5.0f,position.z) + D3DXVECTOR3(direction.x,direction.y,direction.z) * 35.0f);
		mpActorHandler->getActor(keys[objectCounter])->setDirection(direction.x,direction.y,direction.z);
		mpActorHandler->getActor(keys[objectCounter])->setPosition(position.x + direction.x * 35.0f,position.y + 5.0f,position.z + direction.z * 35.0f);
		//mpDX10Module->intersection(position.x + direction.x * 35.0f,position.y + 5.0f,position.z + direction.z * 35.0f,direction.x,direction.y,direction.z,mpBullet->getKey());
		
		D3DXVECTOR3 origin;
		origin.x = position.x;
		origin.y = position.y;
		origin.z = position.z;
		D3DXVECTOR3 dir;
		dir.x = direction.x;
		dir.y = direction.y;
		dir.z = direction.z;

		LogicQuadTree::getInstance()->collisionRay(origin, dir, PlayerInfo::getInstance()->getTankActorId(), mpBullet->getKey()); 

		if(objectCounter == 11)
			objectCounter = -1;
	    
	}
	

  }

 }