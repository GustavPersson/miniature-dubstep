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
 * This is the class for the Black Widow
 */

#include "BlackWidow.h"
#include <iostream>

using namespace std;
using namespace logic;


BlackWidow::BlackWidow(double hp,double damage,Sizes size,int aiStyle,float range) :
	Enemy(hp,damage,size,aiStyle,range)
{
	mFire = true;
	mpEnemyWeapon = myNew AcidSpit(damage, 1.0f,range);
	mpEnemyWeapon->setOwner(this->getKey());

	mpRenderObject = gfx::DX10Module::getInstance()->createMeshInstanced("LongRangeTank");
	mpRenderObject->setID(mId);
	mpRenderObject->setName("BlackWidow");
	mpRenderObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
	dynamic_cast<gfx::InstanceMesh*>(mpRenderObject)->setScale(D3DXVECTOR3(2.5f, 1.5f, 2.5f));
	mpRenderObject->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
}

BlackWidow::~BlackWidow()
{
	SAFE_DELETE(mpEnemyWeapon);
}

void BlackWidow::attack()
{
 //if enemy is in range of tank
	if((ActorHandler::getInstance()->getActor(mpPlayerInfo->getTankActorId())->getPosition() - mPosition).longerThan(Enemy::getRange(), false) == false)
    {
		mpEnemyWeapon->fireWeapon();
	}
  
}

void BlackWidow::update(float deltaTime)
{
	if(mFire)
	{	
		attack();
	}
	Actor::mPosition.x += (SPIDER_MOVE_SPEED * Actor::mDirection.x * deltaTime);
	Actor::mPosition.y += (SPIDER_MOVE_SPEED * Actor::mDirection.y * deltaTime);
	Actor::mPosition.z += (SPIDER_MOVE_SPEED * Actor::mDirection.z * deltaTime);

	mpRenderObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));

}

void BlackWidow::handleEventId(EventData const &eventMessage)
{
	Enemy::handleEventId(eventMessage);
}