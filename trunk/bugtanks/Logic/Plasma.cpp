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
 * Subclass to munitions, it holds plasma spew projectile  
 */

#include "Plasma.h"
#include "Events.h"
#include "Actorhandler.h"
#include "PlayerInfo.h"

using namespace logic;

Plasma::Plasma(int networkId, utilities::Position position, utilities::Direction direction,int efficiency, float damage) :
	Munitions(networkId, position,direction,efficiency,damage)
{
	mMunitionType = MunitionType_Plasma;
	mScale = utilities::Vec3Float(7.5f, 6.0f, 7.5f);
	mpTransformObject = mpsDXModule->createObjectInstanced("Cube");
	mpTransformObject->setID(mId.convertToGfxId());
	mpTransformObject->setName("Plasma");
	mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
	mpTransformObject->setColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	mpTransformObject->setFlagRender(false);
	dynamic_cast<gfx::InstanceObject*>(mpTransformObject)->setScale(mScale.convertToD3DXVECTOR3());
}

Plasma::~Plasma()
{
}

void Plasma::update(float deltaTime)
{
	ActorHandler *mpActorHandler = ActorHandler::getInstance();
	Munitions *pMunition = dynamic_cast<Munitions*>(mpActorHandler->getActor(mId));
	Actor *pActor = mpActorHandler->getActor(pMunition->getOwner());
	if(pActor != NULL)
	{
		utilities::Direction aimDir = mpActorHandler->getActor(PlayerInfo::getInstance()->getPlayerId())->getPosition() - pActor->getPosition();
		aimDir.normalize();
		mPosition.x = pActor->getPosition().x + aimDir.x * 35.0f;
		mPosition.y = pActor->getPosition().y + aimDir.y;
		mPosition.z = pActor->getPosition().z + aimDir.z * 35.0f;
		if(dynamic_cast<Enemy*>(pActor) != NULL && !dynamic_cast<Enemy*>(pActor)->getFiring()
			/*&& (mpActorHandler->getActor(PlayerInfo::getInstance()->getPlayerId())->getPosition() - pActor->getPosition()).longerThan(dynamic_cast<Enemy*>(pActor)->getRange())*/)
		{
			this->setVisible(false);
		}

	}
	if(mOwnerDead && mDestroyTimer.getTime(utilities::Timer::ReturnType_Seconds) > 10)
	{
		safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(mId)));
		DEBUG_MESSAGE(LEVEL_LOWEST, "plasmaupdate");
		mOwnerDead = false;
	}
	if(mVisible)
	{
		for(int i = 0; i < 1; i++)
		{
			mpsDXModule->getParticles("Flames")->addEmitter(mPosition.convertToD3DXVECTOR3(), D3DXVECTOR3(pActor->getDirection().x * 12.0f, pActor->getDirection().y, pActor->getDirection().z * 12.0f));
		}
	}
}

void Plasma::handleEventId(EventData const &eventMessage)
{
	Munitions::handleEventId(eventMessage);
}