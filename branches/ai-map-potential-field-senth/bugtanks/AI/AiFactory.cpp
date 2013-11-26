/**
* @file
* @author Matteus Magnusson <senth.wallace@gmail.com>
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

#include "AiFactory.h"
#include "PotentialUnits.h"
#include "UnitHandler.h"
#include "PathFinder.h"
#include "AiConstants.h"
#include "Map.h"
#include "../Logic/KeyHandler.h"
#include "../Logic/Enemy.h"
#include "../Logic/Tank.h"
#include "../Logic/Driver.h"

using namespace ai;

AiFactory* AiFactory::mpsInstance = NULL;

void AiFactory::create()
{
	if (mpsInstance == NULL)
	{
		mpsInstance = myNew AiFactory();
	}
}

void AiFactory::destroy()
{
	if (mpsInstance != NULL)
	{
		SAFE_DELETE(mpsInstance);
	}
}

AiFactory::AiFactory()
{
	mpUnitHandler = NULL;
	mpPathFinder = NULL;
	mpMap = NULL;

	AiConstants::loadConstants();

	mpMap = Map::getInstance();
	mpUnitHandler = UnitHandler::getInstance();
	mpPathFinder = PathFinder::getInstance();

	PotentialUnit::init();
	AgentAi::init(AiConstants::POTENTIAL_WIDTH_AGENT);
	Building::initPotentialField(AiConstants::POTENTIAL_BUILDING_RANGE);
	Player::init(AiConstants::POTENTIAL_WIDTH_PLAYER);

	mRunning = false;
}

AiFactory::~AiFactory()
{
	mpUnitHandler->clear();
	mpPathFinder->clear();
	mpMap->clear();

	SAFE_DELETE(mpUnitHandler);
	SAFE_DELETE(mpPathFinder);
	SAFE_DELETE(mpMap);

	AgentAi::destroy();
	Building::destroyPotentialField();
	Player::destroy();
}

void AiFactory::addEnemy(logic::Enemy *pEnemy)
{
	DEBUG_MESSAGE(LEVEL_LOW, "AiFactory::addEnemy() | ");

	// Create an agent ai
	AgentAi* pAgentAi = NULL;
	switch (pEnemy->getAiBehavior())
	{
		case AiBehavior_Jumper:
			pAgentAi = myNew JumperAi(pEnemy);
			break;

		case AiBehavior_RangedAttacker:
			pAgentAi = myNew RangedAi(pEnemy);
			break;

		case AiBehavior_MeleeAttacker:
			pAgentAi = myNew MeleeAi(pEnemy);
			break;

		case AiBehavior_Invalid:
			ERROR_MESSAGE("Invalid AiBehavior!");
			return;

		// Use a default AI for the rest, until they are implemented (or never implemented)
		default:
			pAgentAi = myNew AgentAi(pEnemy);

	}

	mpUnitHandler->addAgentAi(pAgentAi);

	// If we own the agent and we're running, calculate it's goals and next goals
	if (pAgentAi->getKey().isOwner() && mRunning)
	{
		mpPathFinder->calculateGoal(pAgentAi, PathFinder::GoalType_Goal);
		mpPathFinder->calculateGoal(pAgentAi, PathFinder::GoalType_NextGoal);
	}

	addPotentialUnitToMap(pAgentAi);
}

void AiFactory::removeEnemy(logic::KeyType keyId)
{
	DEBUG_MESSAGE(LEVEL_LOW, "AiFactory::removeEnemy() | " << keyId);

	mpUnitHandler->removeAgentAi(keyId);

	removePotentialUnitFromMap(keyId);
}

void AiFactory::addTank(logic::Tank *pTank)
{
	// Create tank
	Tank* pPlayer = myNew Tank(pTank);

	mpUnitHandler->addPlayer(pPlayer);
	mpPathFinder->addPlayer(pPlayer, mRunning);
}

void AiFactory::removeTank(logic::KeyType keyId)
{
	mpPathFinder->removePlayer(keyId, mRunning);
	mpUnitHandler->removePlayer(keyId);
}

void AiFactory::addDriver(logic::Driver *pDriver)
{
	// Using the player for now
	Player* pPlayer = myNew Tank(pDriver);

	mpUnitHandler->addPlayer(pPlayer);
	mpPathFinder->addPlayer(pPlayer, mRunning);
}

void AiFactory::removeDriver(logic::KeyType keyId)
{
	mpPathFinder->removePlayer(keyId, mRunning);
	mpUnitHandler->removePlayer(keyId);
}

void AiFactory::addBuilding(logic::Actor *pBuilding)
{
	Building* pUnit = myNew Building(pBuilding);
	mpUnitHandler->addStaticUnit(pUnit);

	addPotentialUnitToMap(pUnit);
}

void AiFactory::removeBuilding(logic::KeyType keyId)
{
	mpUnitHandler->removeStaticUnit(keyId);

	removePotentialUnitFromMap(keyId);
}

void AiFactory::setMapSize(const utilities::MapCoordinate& mapSize)
{
	mpMap->setMapSize(mapSize);
}

void AiFactory::reset()
{
	mpUnitHandler->clear();
	mpPathFinder->clear();

	pause();
}

void AiFactory::update(float deltaTime)
{
	if (mRunning)
	{
		mpUnitHandler->update(deltaTime);
		mpPathFinder->update(deltaTime);
	}
}

void AiFactory::pause()
{
	DEBUG_MESSAGE(LEVEL_LOW, "AiFactory::pause()");
	mRunning = false;

	mpUnitHandler->pause();
}

void AiFactory::resume()
{
	// Only resume if we have paused
	if (!mRunning)
	{
		DEBUG_MESSAGE(LEVEL_LOW, "AiFactory::resume()");
		mpPathFinder->restart();
		mpUnitHandler->resume();
		mRunning = true;
	}
}

void AiFactory::addPotentialUnitToMap(const PotentialUnit *pPotentialUnit)
{
	if (pPotentialUnit->applyOnGlobalPotentialField())
	{
		mpMap->addPotentialUnit(pPotentialUnit);
	}
}

void AiFactory::removePotentialUnitFromMap(const logic::KeyType& keyId)
{
	mpMap->removePotentialUnit(keyId);
}