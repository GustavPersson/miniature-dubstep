/**
 * @file
 * @author Dennis J�nsson <zelebsel@gmail.com>
 * @version 1.0
 * Copyright (�) A-Team.
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
 * Subclass to munitions, it holds all kinds of bullet type projectiles  
 */

#include "Bullet.h"

using namespace logic;

Bullet::Bullet(utilities::Position position, int efficiency, double damage) :
	Munitions(position, efficiency, damage)
{
	mpRenderObject = gfx::DX10Module::getInstance()->createObjectInstanced("Cube");
	mpRenderObject->setID(mId);
	mpRenderObject->setName("Bullet");
	mpRenderObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
	mpRenderObject->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	dynamic_cast<gfx::InstanceObject*>(mpRenderObject)->setScale(D3DXVECTOR3(2.0f,2.0f,2.0f));
}

Bullet::~Bullet()
{
}

void Bullet::update(float deltaTime)
{	

	Actor::mPosition.x += (BULLET_MOVE_SPEED * Actor::mDirection.x * deltaTime);
	Actor::mPosition.y += (BULLET_MOVE_SPEED * Actor::mDirection.y * deltaTime);
	Actor::mPosition.z += (BULLET_MOVE_SPEED * Actor::mDirection.z * deltaTime);
	
	mpRenderObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
}

void Bullet::handleEventId(EventData const &eventMessage)
{
}