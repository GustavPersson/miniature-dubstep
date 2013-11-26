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
#include "AiEvents.h"
#include "../logic/ActorConstants.h"
#include "../logic/ActorHandler.h"

using namespace ai;


JumperAi::JumperAi(logic::Enemy *pEnemy) :AgentAi(pEnemy)
{
	mJumping = false;
	mLastJumpTime = currentTime();
	mLastTestTime = currentTime();
}

JumperAi::~JumperAi()
{

}

void JumperAi::update()
{
	AgentAi::update();
	
	if (mJumping)
	{
		// if we're at the ground again, restore the jumping state
		if (mpActor->getPosition().y == 0.0f)
		{	
			jumpFinished();
		}
	}
}

void JumperAi::updateOnlyOwner()
{
	AgentAi::updateOnlyOwner();

	if (!mJumping)
	{
		logic::Actor* pTarget = logic::ActorHandler::getInstance()->getActor(mTarget);
		if(pTarget != NULL && (pTarget->getPosition() - mpActor->getPosition()).longerThan(logic::ActorConstants::JUMPER_JUMP_RANGE_MAX, false) == false && (pTarget->getPosition() - mpActor->getPosition()).longerThan(logic::ActorConstants::JUMPER_JUMP_RANGE_MIN, false) == true  && getKey().isOwner())
		{
			// HACK - Should be changed to a certain chance every second
			float diffTime = currentTime() - mLastJumpTime;
			if(diffTime > logic::ActorConstants::JUMPER_JUMP_TIME_MIN)
			{
				int randValue = static_cast<int>(logic::ActorConstants::JUMPER_JUMP_TIME_MAX - diffTime);
				if(randValue <= 0)
				{
					jump(pTarget);
				}
				else if(currentTime() > mLastTestTime + 1.0f)
				{
					mLastTestTime = currentTime();
					bool shouldJump = rand() % randValue == 0;
					if (shouldJump)
					{
						jump(pTarget);
					}
				}
			}
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

	// Send event that we want to jump
	logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew EvtData_From_JumperAiJump(getKey(), pTarget->getPosition())));
	//logic::safeQueueEvent(logic::EventDataPtr( myNew EvtData_From_JumperAiJump(getKey(), pTarget->getPosition()) ));

	setUseGoals(false);
	mJumping = true;
}

void JumperAi::jump(const utilities::Position& position)
{
	DEBUG_MESSAGE(LEVEL_LOWER, "JumperAi::jump() | Jumping :D");

	utilities::Direction jumpDirection = position - mpActor->getPosition();
	float jumpLength = jumpDirection.length();
	float halfTime = jumpLength * 0.5f/logic::ActorConstants::JUMPER_JUMP_SPEED;

	float ySpeed = logic::ActorConstants::GRAVITATION * halfTime;
	jumpDirection.normalize();
	jumpDirection *= logic::ActorConstants::JUMPER_JUMP_SPEED;
	jumpDirection.y = ySpeed;
	jumpDirection.normalize();
	mpActor->setDirection(jumpDirection);
	mpActor->setSpeed(logic::ActorConstants::JUMPER_JUMP_SPEED + ySpeed);

	setUseGoals(false);
	mJumping = true;
}

void JumperAi::jumpFinished()
{
	DEBUG_MESSAGE(LEVEL_LOWER, "JumperAi::jumpFinished() | On the ground again.");
	setUseGoals(true);

	// TODO - Remove the negative potential field.

	mJumping = false;
	mLastJumpTime = currentTime();
}