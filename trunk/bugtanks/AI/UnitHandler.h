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

#ifndef __UNIT_HANDLER_H__
#define __UNIT_HANDLER_H__

#include "../Utilities/Macros.h"
#include "../Utilities/VectorList.h"
#include "../Logic/KeyHandler.h"
#include <map>

namespace ai
{

// Forward declarations
class AgentAi;
class Player;
class StaticUnit;

/**
* Handles all the unit types that the AI Components needs.
* Is a singleton
*/
class UnitHandler
{
public:
	/**
	* Returns a pointer to UnitHandler
	* @return pointer to the unit handler
	*/
	static UnitHandler* getInstance();

	/**
	* Should only be destroyed by AiFactory.
	*/
	~UnitHandler();

	/**
	* Add an AgentAi to the unit handler
	* @param pAgentAi pointer to an ai that shall be added.
	*/
	void addAgentAi(AgentAi* pAgentAi);

	/**
	* Removes the AgentAi from the handler
	* @param keyId the keyId of the AgentAi
	*/
	void removeAgentAi(const logic::KeyType& keyId);

	/**
	* Add a player to the unit handler
	* @param pPlayer pointer to an player
	*/
	void addPlayer(Player* pPlayer);

	/**
	* Removes a player from the unit handler
	* @param keyId the keyId of the player
	*/
	void removePlayer(const logic::KeyType& keyId);

	/**
	* Add a static unit to the unit handler
	* @param pStaticUnit pointer to an static unit that shall be added
	*/
	void addStaticUnit(StaticUnit* pStaticUnit);

	/**
	* Removes a static unit from the unit handler
	* @param keyId the keyId of the unit
	*/
	void removeStaticUnit(const logic::KeyType& keyId);

	/**
	* Clears the Units Handler from all units
	*/
	void clear();

	/**
	* Returns a pointer to the agent with the specified id
	* @param keyId the agents key id
	* @return pointer to the player with the specified key, null if not found
	*/
	AgentAi* getAgentAi(const logic::KeyType& keyId);

	/**
	* Returns a const pointer to the agent with the specified key.
	* @param keyId the agents key id
	* @return const pointer to the player with the specified key, null if not found
	*/
	inline const AgentAi* getAgentAi(const logic::KeyType& keyId) const;

	/**
	* Returns const reference to the map containing all the AgentAis
	* @return const reference to the map containing all the AgentAis
	*/
	inline const std::map<logic::KeyType, AgentAi*>& getAgentAis() const
	{
		return mAgentAis;
	}

	/**
	* Returns a reference to the map containing all the AgentAis
	* @return reference to the map containing all the AgentAis
	*/
	inline std::map<logic::KeyType, AgentAi*>& getAgentAis()
	{
		return mAgentAis;
	}

	/**
	* Returns const reference to the map containing all the players
	* @return const reference to the map containing all the players
	*/
	inline const std::map<logic::KeyType, Player*>& getPlayers() const
	{
		return mPlayers;
	}

	/**
	* Returns reference to the map containing all the players
	* @return reference to the map containing all the players
	*/
	inline std::map<logic::KeyType, Player*>& getPlayers()
	{
		return mPlayers;
	}

	/**
	* Returns a pointer to the specified player.
	* @param keyId the key of the player to return
	* @return pointer to the player, NULL if not found.
	*/
	Player* getPlayer(const logic::KeyType& keyId);

	/**
	* Returns a const pointer to the specified player
	* @param keyId the key of the player to return
	* @return const pointer to the specified player, NULL if not found
	*/
	const Player* getPlayer(const logic::KeyType& keyId) const;

	/**
	* Returns a pointer to the specified static unit
	* @param keyId the key of the static unit to return
	* @return pointer to the static unit, NULL if not found.
	*/
	StaticUnit* getStaticUnit(const logic::KeyType& keyId);

	/**
	* Returns a const pointer to the specified static unit
	* @param keyId the key of the static unit to return
	* @return const pointer to the static unit, NULL if not found.
	*/
	const StaticUnit* getStaticUnit(const logic::KeyType& keyId) const;

	/**
	* Updates all the units in the unit handler.
	* @param deltaTime time elapsed since the last frame.
	*/
	void update(float deltaTime);

	/**
	* Pauses the units that are able to pause
	*/
	void pause();

	/**
	* Resumes the units
	*/
	void resume();

private:
	/**
	* Private Default Constructor to enforce the singleton pattern.
	*/
	UnitHandler();

	std::map<logic::KeyType, AgentAi*>	mAgentAis;
	std::map<logic::KeyType, Player*>	mPlayers;
	std::map<logic::KeyType, StaticUnit*> mStaticUnits;	/**< Units that affect the potential fields and are static */

	std::map<logic::KeyType, AgentAi*>::iterator mAgentAiIt;
	std::map<logic::KeyType, Player*>::iterator mPlayerIt;
	std::map<logic::KeyType, StaticUnit*>::iterator mStaticUnitIt;

	float mAgentAiTotalTime;
	float mPlayerTotaltime;
	float mStaticUnitsTotalTime;

	float mSecondsPerAgentAi;
	float mSecondsPerPlayer;
	float mSecondsPerStaticUnit;


	static UnitHandler* mpsInstance;
};
}

#endif