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

#include "JumperAi.h"
#include "../logic/ActorConstants.h"
#include "../logic/ActorHandler.h"

using namespace ai;


JumperAi::JumperAi(logic::Enemy *pEnemy) :AgentAi(pEnemy)
{
	mJumping = false;
}

JumperAi::~JumperAi()
{

}

void JumperAi::update()
{
	AgentAi::update();
	
	if (!mJumping)
	{
		logic::Actor* pTarget = logic::ActorHandler::getInstance()->getActor(mTarget);
		if(pTarget != NULL && (pTarget->getPosition() - mpActor->getPosition()).longerThan(logic::ActorConstants::JUMPER_JUMP_RANGE_MAX, false) == false && (pTarget->getPosition() - mpActor->getPosition()).longerThan(logic::ActorConstants::JUMPER_JUMP_RANGE_MIN, false) == true  && getKey().isOwner())
		{
			// HACK - Should be changed to a certain chance every second
			bool shouldJump = rand() % 200 == 0;
			if (shouldJump)
			{
				jump(pTarget);
			}
		}
	}
	// Else - We're jumping
	else
	{
		// if we're at the ground again, restore the jumping state
		if (mpActor->getPosition().y == 0.0f)
		{
			jumpFinished();
		}
	}
}

void JumperAi::resume()
{
	AgentAi::resume();

	// If we were jumping resume the movement
	if (mJumping)
	{
		mpActor->setMoving(true);
	}
}

void JumperAi::jump(logic::Actor *pTarget)
{
	DEBUG_MESSAGE(LEVEL_LOWER, "JumperAi::jump() | Jumping :D");

	utilities::Direction jumpDirection = pTarget->getPosition() - mpActor->getPosition();
	float jumpLength = jumpDirection.length();
	float halfTime = jumpLength * 0.5f/logic::ActorConstants::JUMPER_JUMP_SPEED;

	float ySpeed = logic::ActorConstants::GRAVITATION * halfTime;
	jumpDirection.normalize();
	jumpDirection *= logic::ActorConstants::JUMPER_JUMP_SPEED;
	jumpDirection.y = ySpeed;
	jumpDirection.normalize();
	mpActor->setDirection(jumpDirection);
	mpActor->setSpeed(logic::ActorConstants::JUMPER_JUMP_SPEED + ySpeed);

	// TODO - Add negative potential field where we're going to land.

	// HACK - Send event that we want to jump

	setUseGoals(false);
	mJumping = true;
}

void JumperAi::jumpFinished()
{
	DEBUG_MESSAGE(LEVEL_LOWER, "JumperAi::jumpFinished() | On the ground again.");
	mpActor->setSpeed(logic::ActorConstants::JUMPER_SPEED);
	setUseGoals(true);

	// TODO - Remove the negative potential field.

	mJumping = false;
}