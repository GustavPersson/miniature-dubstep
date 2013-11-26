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
* 
* @section DESCRIPTION
*
* Handles all the enemies, should be thread-safe
*/

#ifndef __ENEMY_HANDLER_H__
#define __ENEMY_HANDLER_H__

#include "KeyHandler.h"
#include <map>
#include <pthread.h>

// Forward declarations
class Enemy;

class EnemyHandler
{
public:
	EnemyHandler();
	~EnemyHandler();

	void addEnemy(Enemy* pEnemy, KeyType id);
	void removeEnemy(KeyType id);

	/**
	* Check outs an enemy, you need to check it back in to unlock the EnemyHandler class
	* @param id the enemy id
	*/
	Enemy* checkOutEnemy(KeyType id);

	/**
	* Checks in the enemy, call this when you won't use the enemy you checked out.
	* @param id the enemy id
	*/
	void checkInEnemy(KeyType id);

	std::map<KeyType, Enemy*>* checkOutAllEnemies();
	void checkInAllEnemies();

private:
	std::map<KeyType, Enemy*>	mEnemies;
	mutable pthread_mutex_t mEnemyMutex;

	// Test the checkout/checkin function
#ifdef _DEBUG
	KeyType mCheckedOutEnemy;
#endif
};

#endif