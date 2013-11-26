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

#include "Macros.h"
#include "EnemyHandler.h"
#include "Enemy.h"
#include <cassert>

EnemyHandler::EnemyHandler()
{
	pthread_mutex_init(&mEnemyMutex, NULL);

#ifdef _DEBUG
	mCheckedOutEnemy = -1;
#endif
}

EnemyHandler::~EnemyHandler()
{
	// Delete all enemies
	pthread_mutex_lock(&mEnemyMutex);
	std::map<KeyType, Enemy*>::iterator it;
	for (it = mEnemies.begin(); it != mEnemies.end(); ++it)
	{
		SAFE_DELETE(it->second);
	}
	pthread_mutex_unlock(&mEnemyMutex);

	SAFE_MUTEX_DESTROY(&mEnemyMutex);
}

void EnemyHandler::addEnemy(Enemy* pEnemy, KeyType id)
{
	mEnemies[id] = pEnemy;
}

void EnemyHandler::removeEnemy(KeyType id)
{
	pthread_mutex_lock(&mEnemyMutex);
	SAFE_DELETE(mEnemies[id]);
	mEnemies.erase(id);
	pthread_mutex_unlock(&mEnemyMutex);
}

Enemy* EnemyHandler::checkOutEnemy(KeyType id)
{
	pthread_mutex_lock(&mEnemyMutex);
	std::map<KeyType, Enemy*>::const_iterator it = mEnemies.find(id);
	if (it != mEnemies.end())
	{

		// checkout/checkin checker
#ifdef _DEBUG
		mCheckedOutEnemy = id;
#endif

		return it->second;
	}
	else
	{
		return NULL;
	}
}

void EnemyHandler::checkInEnemy(KeyType id)
{
#ifdef _DEBUG
	assert(mCheckedOutEnemy == id);
	mCheckedOutEnemy = -1;
#endif
	pthread_mutex_unlock(&mEnemyMutex);
}

std::map<KeyType, Enemy*>* EnemyHandler::checkOutAllEnemies()
{
	pthread_mutex_lock(&mEnemyMutex);
	return &mEnemies;
}

void EnemyHandler::checkInAllEnemies() {
	pthread_mutex_unlock(&mEnemyMutex);
}