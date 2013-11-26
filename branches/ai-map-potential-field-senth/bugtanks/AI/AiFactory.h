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

#ifndef __AI_FACTORY_H__
#define __AI_FACTORY_H__

#include "../Utilities/Macros.h"
#include "../Utilities/Vec2Int.h"
#include "../Utilities/VectorList.h"
#include "../Logic/KeyHandler.h"

// forward declarations
namespace logic
{
	class Actor;
	class Enemy;
	class Pedestrian;
	class Driver;
	class Tank;
}

namespace ai
{

// forward declarations
class UnitHandler;
class PathFinder;
class Map;
class PotentialUnit;

/**
* Handles the interaction with the other components, is a singleton.
* The default state is paused, so you have to resume the AI component
* for it to work.
*/
class AiFactory
{
public:
	/**
	* Returns a pointer to the ai factory
	* @return pointer to the AiFactory, null if the AiFactory hasn't been created
	* @note you must explicitly call createInstance if you want to create an instance
	* @see create
	*/
	static inline AiFactory* getInstance() {return mpsInstance;}

	/**
	* Creates a new instance if one hasn't been created. Does nothing if an
	* instance has been created.
	*/
	static void create();

	/**
	* Destroys the instance of the AiFactory. Does nothing if an
	* instance hasn't been created.
	*/
	static void destroy();

	/**
	* Adds an enemy (agent) to the AI component
	* @param pEnemy pointer to the enemy to add.
	*/
	void addEnemy(logic::Enemy* pEnemy);

	/**
	* Removes an enemy (agent) from the AI component
	* @param keyId the keyId of the enemy that shall be removed
	* @note prints an error if we want to remove an enemy that isn't in the AI component.
	*/
	void removeEnemy(logic::KeyType keyId);

	/**
	* Adds a tank to the AI component
	* @param pTank pointer to the tank to add.
	*/
	void addTank(logic::Tank* pTank);

	/**
	* Removes a tank from the AI component
	* @param keyId the keyId of the tank that shall be removed
	* @note prints an error if we want to remove a tank that isn't in the AI component.
	*/
	void removeTank(logic::KeyType keyId);

	/**
	* Adds a driver to the AI component
	* @param pDriver pointer to the driver to add.
	*/
	void addDriver(logic::Driver* pDriver);

	/**
	* Removes a driver from the AI component
	* @param keyId the keyId of the driver that shall be removed
	* @note prints an error if we want to remove a driver that isn't in the AI component.
	*/
	void removeDriver(logic::KeyType keyId);

	/**
	* Adds a building to the AI component
	* @param pBuilding pointer to the building actor.
	*/
	void addBuilding(logic::Actor* pBuilding);

	/**
	* Removes a building from the AI component
	* @param keyId the keyId of the building that shall be removed
	*/
	void removeBuilding(logic::KeyType keyId);

	/**
	* Sets the map size that will be used.
	* @param mapSize the size of the map
	*/
	void setMapSize(const utilities::MapCoordinate& mapSize);

	/**
	* Resets the AI Factory and pauses the AI component. This removes all actors
	* of any kind, the map, etc. Should be used when a game has ended.
	*/
	void reset();

	/**
	* Update function that updates the whole AI component
	* @param deltaTime time elapsed since the last frame
	*/
	void update(float deltaTime);

	/**
	* Pauses the AI component. When pausing all the agents will stop moving.
	* Has no effect if the AI component is paused already.
	*/
	void pause();

	/**
	* Resumes the AI component. Has no effect if the AI component isn't been paused.
	* @note When this function is invoked the AI component needs to recalculate
	* lots of stuff, thus it will take some time. Don't pause the AI component too often.
	*/
	void resume();

	/**
	* Returns true if the AI component is paused
	* @return true if the AI component is paused
	*/
	inline bool isPaused() const {return !mRunning;}

private:
	/**
	* Private default constructor
	*/
	AiFactory();

	/**
	* Destructor
	*/
	~AiFactory();

	/**
	* Checks if we should add the potential unit to the map. If we should
	* it adds it to the map.
	* @param pPotentialUnit pointer to the newly created potential unit.
	*/
	void addPotentialUnitToMap(const PotentialUnit* pPotentialUnit);
	
	/**
	* Tries to remove the potential unit from the map. Nothing happens if it
	* isn't in the map
	*/
	void removePotentialUnitFromMap(const logic::KeyType& keyId);


	UnitHandler* mpUnitHandler;
	PathFinder* mpPathFinder;
	Map* mpMap;

	bool mRunning;

	static AiFactory* mpsInstance;
};
}

#endif