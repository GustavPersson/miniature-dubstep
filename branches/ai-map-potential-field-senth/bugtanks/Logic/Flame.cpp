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
 * Subclass to munitions, it holds the flamethrower munitions  
 */

#include "Flame.h"
#include "Events.h"
#include "ActorHandler.h"

using namespace logic;


Flame::Flame(int networkId, utilities::Position position, utilities::Direction direction, int efficiency, float damage) :
	Munitions(networkId, position,direction, efficiency, damage)
{
	mScale = utilities::Vec3Float(5.0f, 4.0f, 5.0f);
	mpTransformObject = mpsDXModule->createObjectInstanced("Cube");
	mpTransformObject->setID(mId.convertToGfxId());
	mpTransformObject->setName("Flame");
	mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
	mpTransformObject->setColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	dynamic_cast<gfx::InstanceObject*>(mpTransformObject)->setScale(mScale.convertToD3DXVECTOR3());
}

Flame::~Flame()
{
}

void Flame::update(float deltaTime)
{
	ActorHandler *mpActorHandler = ActorHandler::getInstance();
	Actor *pActor = mpActorHandler->getActor(mpsPlayerInfo->getPlayerId());
	/*Actor::mPosition.x += (FLAME_MOVE_SPEED * Actor::mDirection.x * deltaTime);
	Actor::mPosition.y += (FLAME_MOVE_SPEED * Actor::mDirection.y * deltaTime);
	Actor::mPosition.z += (FLAME_MOVE_SPEED * Actor::mDirection.z * deltaTime);*/
	mPosition.x = pActor->getPosition().x + pActor->getSubsetDirection().x * 35.0f;
	mPosition.y = pActor->getPosition().y + pActor->getSubsetDirection().y;
	mPosition.z = pActor->getPosition().z + pActor->getSubsetDirection().z * 35.0f;
	if(dynamic_cast<Tank*>(pActor) != NULL && !dynamic_cast<Tank*>(pActor)->getFiring())
		this->setVisible(false);

}

void Flame::handleEventId(EventData const &eventMessage)
{

}