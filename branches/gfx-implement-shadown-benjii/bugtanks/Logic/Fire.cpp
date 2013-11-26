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
 * Subclass to munitions, it holds fire spit projectiles  
 */

#include "Fire.h"

using namespace logic;

Fire::Fire(utilities::Position position,int efficiency,double damage) :
	Munitions(position,efficiency,damage)
{
	mpRenderObject = gfx::DX10Module::getInstance()->createObjectInstanced("Cube");
	mpRenderObject->setID(mId);
	mpRenderObject->setName("Acid");
	mpRenderObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
	mpRenderObject->setColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	dynamic_cast<gfx::InstanceObject*>(mpRenderObject)->setScale(D3DXVECTOR3(2.0f,2.0f,2.0f));
}

Fire::~Fire()
{
}

void Fire::update(float deltaTime)
{
	Actor::mPosition.x += (FIRE_MOVE_SPEED * Actor::mDirection.x * deltaTime);
	Actor::mPosition.y += (FIRE_MOVE_SPEED * Actor::mDirection.y * deltaTime);
	Actor::mPosition.z += (FIRE_MOVE_SPEED * Actor::mDirection.z * deltaTime);
	
	mpRenderObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
}

void Fire::handleEventId(EventData const &eventMessage)
{

}