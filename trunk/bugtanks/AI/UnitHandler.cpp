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

#include "UnitHandler.h"
#include "AgentAi.h"
#include "Player.h"
#include "StaticUnit.h"

using namespace ai;
using logic::KeyType;

UnitHandler* UnitHandler::mpsInstance = NULL;

UnitHandler* UnitHandler::getInstance()
{
	if (mpsInstance == NULL)
	{
		mpsInstance = myNew UnitHandler();
	}
	return mpsInstance;
}

UnitHandler::UnitHandler()
{
	// UNDONE - UnitHandler constructor
	mAgentAiIt		= mAgentAis.end();
	mPlayerIt		= mPlayers.end();
	mStaticUnitIt	= mStaticUnits.end();
	
	mAgentAiTotalTime		= 0.0f;
	mPlayerTotaltime		= 0.0f;
	mStaticUnitsTotalTime	= 0.0f;
	mSecondsPerAgentAi		= 0.0f;
	mSecondsPerPlayer		= 0.0f;
	mSecondsPerStaticUnit	= 0.0f;
}

UnitHandler::~UnitHandler()
{
	clear();
	mpsInstance = NULL;
}

void UnitHandler::addAgentAi(AgentAi *pAgentAi)
{
	// Assert if we already have an agent there
	DEBUG_MESSAGE(LEVEL_LOWER, "UnitHandler::addAgentAi() | Agent keyId: " << pAgentAi->getKey());
	assert(mAgentAis.find(pAgentAi->getKey()) == mAgentAis.end());

	mAgentAis[pAgentAi->getKey()] = pAgentAi;
}

void UnitHandler::removeAgentAi(const logic::KeyType& keyId)
{
	std::map<KeyType, AgentAi*>::iterator it = mAgentAis.find(keyId);
	
	// Remove the agent if we found it
	if (it != mAgentAis.end())
	{
		if(it == mAgentAiIt)
		{
			++mAgentAiIt;
		}
		SAFE_DELETE(it->second);

		mAgentAis.erase(it);
	}
}

void UnitHandler::addPlayer(Player *pPlayer)
{
	// Assert if we already have a player there
	assert(mPlayers.find(pPlayer->getKey()) == mPlayers.end());

	mPlayers[pPlayer->getKey()] = pPlayer;
}

void UnitHandler::removePlayer(const logic::KeyType& keyId)
{
	std::map<KeyType, Player*>::iterator it = mPlayers.find(keyId);
	
	// Remove the player if we found it
	if (it != mPlayers.end())
	{
		if(it == mPlayerIt)
		{
			++mPlayerIt;
		}
		SAFE_DELETE(it->second);

		mPlayers.erase(it);
	}
}

void UnitHandler::addStaticUnit(StaticUnit* pStaticUnit)
{
	// Assert if we already have a static unit with that key
	assert(mStaticUnits.find(pStaticUnit->getKey()) == mStaticUnits.end());

	mStaticUnits[pStaticUnit->getKey()] = pStaticUnit;
}

void UnitHandler::removeStaticUnit(const logic::KeyType& keyId)
{
	std::map<KeyType, StaticUnit*>::iterator it = mStaticUnits.find(keyId);

	// Remove the unit if we found it
	if (it != mStaticUnits.end())
	{
		if(it == mStaticUnitIt)
		{
			++mStaticUnitIt;
		}
		SAFE_DELETE(it->second);

		mStaticUnits.erase(it);
	}
}

void UnitHandler::clear()
{
	// Delete AgentAis
	std::map<KeyType, AgentAi*>::iterator agentIt;
	for (agentIt = mAgentAis.begin(); agentIt != mAgentAis.end(); ++agentIt)
	{
		SAFE_DELETE(agentIt->second);
	}
	mAgentAis.clear();

	// Delete the players
	std::map<KeyType, Player*>::iterator playerIt;
	for (playerIt = mPlayers.begin(); playerIt != mPlayers.end(); ++playerIt)
	{
		SAFE_DELETE(playerIt->second);
	}
	mPlayers.clear();

	// Delete static units
	std::map<KeyType, StaticUnit*>::iterator staticIt;
	for (staticIt = mStaticUnits.begin(); staticIt != mStaticUnits.end(); ++staticIt)
	{
		SAFE_DELETE(staticIt->second);
	}
	mStaticUnits.clear();
}

AgentAi* UnitHandler::getAgentAi(const logic::KeyType& keyId)
{
	std::map<logic::KeyType, AgentAi*>::iterator it = mAgentAis.find(keyId);
	if (it != mAgentAis.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

StaticUnit* UnitHandler::getStaticUnit(const logic::KeyType &keyId)
{
	std::map<logic::KeyType, StaticUnit*>::iterator it = mStaticUnits.find(keyId);
	if (it != mStaticUnits.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

const StaticUnit* UnitHandler::getStaticUnit(const logic::KeyType &keyId) const
{
	std::map<logic::KeyType, StaticUnit*>::const_iterator it = mStaticUnits.find(keyId);
	if (it != mStaticUnits.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void UnitHandler::update(float deltaTime)
{
	// Update agents
	if(mAgentAiIt == mAgentAis.end())
	{
		mAgentAiIt = mAgentAis.begin();
		mAgentAiTotalTime = 0.0f;
		if(mAgentAis.size() != 0.0f)
		{
			mSecondsPerAgentAi = AiConstants::UPDATE_TIME_AGENT / mAgentAis.size();
		}
	}
	else
	{
		mAgentAiTotalTime += deltaTime;
		while (mAgentAiIt != mAgentAis.end() && mAgentAiTotalTime >= mSecondsPerAgentAi)
		{
			mAgentAiTotalTime -= mSecondsPerAgentAi;
			mAgentAiIt->second->update();

			++mAgentAiIt;
		}
	}

	// Players
	if(mPlayerIt == mPlayers.end())
	{
		mPlayerIt = mPlayers.begin();
		mPlayerTotaltime = 0.0f;
		if(mPlayers.size() != 0.0f)
		{
			mSecondsPerPlayer = AiConstants::UPDATE_TIME_PLAYER / mPlayers.size();
		}
	}
	else
	{
		mPlayerTotaltime += deltaTime;
		while(mPlayerIt != mPlayers.end() && mPlayerTotaltime >= mSecondsPerPlayer)
		{
			mPlayerTotaltime -= mSecondsPerPlayer;
			mPlayerIt->second->update();
			++mPlayerIt;			
		}
	}

	// static units
	//if(mStaticUnitIt == mStaticUnits.end())
	//{
	//	mStaticUnitIt = mStaticUnits.begin();
	//	mStaticUnitsTotalTime = 0.0f;
	//	if(mStaticUnits.size() != 0.0f)
	//	{
	//		mSecondsPerStaticUnit = AiConstants::UPDATE_TIME_BUILDING / mStaticUnits.size();
	//	}
	//}
	//else
	//{
	//	mStaticUnitsTotalTime += deltaTime;
	//	while(mStaticUnitIt != mStaticUnits.end() && mStaticUnitsTotalTime >= mSecondsPerStaticUnit)
	//	{
	//		mStaticUnitsTotalTime -= mSecondsPerStaticUnit;
	//		mStaticUnitIt->second->update();
	//		++mStaticUnitIt;			
	//	}
	//}
}

void UnitHandler::pause()
{
	// Stop agents
	std::map<logic::KeyType, AgentAi*>::iterator agentIt;
	for (agentIt = mAgentAis.begin(); agentIt != mAgentAis.end(); ++agentIt)
	{
		agentIt->second->pause();
	}
}

void UnitHandler::resume()
{
	// Resume the agent and update them
	std::map<logic::KeyType, AgentAi*>::iterator agentIt;

	for (agentIt = mAgentAis.begin(); agentIt != mAgentAis.end(); ++agentIt)
	{
		agentIt->second->resume();
		agentIt->second->update();
	}

	// Update all the other potential units
	std::map<logic::KeyType, Player*>::iterator playerIt;
	for (playerIt = mPlayers.begin(); playerIt != mPlayers.end(); ++playerIt)
	{
		playerIt->second->update();
	}

	std::map<logic::KeyType, StaticUnit*>::iterator staticUnitIt;
	for (staticUnitIt = mStaticUnits.begin(); staticUnitIt != mStaticUnits.end(); ++staticUnitIt)
	{
		staticUnitIt->second->update();
	}
	
	// Reset the iterators
	mAgentAiIt = mAgentAis.end();
	mPlayerIt = mPlayers.end();
	mStaticUnitIt = mStaticUnits.end();
}

Player* UnitHandler::getPlayer(const logic::KeyType& keyId)
{
	std::map<logic::KeyType, Player*>::iterator it = mPlayers.find(keyId);
	if (it != mPlayers.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

const Player* UnitHandler::getPlayer(const logic::KeyType& keyId) const
{
	std::map<logic::KeyType, Player*>::const_iterator it = mPlayers.find(keyId);
	if (it != mPlayers.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}