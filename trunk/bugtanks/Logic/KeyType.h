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

#ifndef __LOGIC__KEY_TYPE_H__
#define __LOGIC__KEY_TYPE_H__

#include "../Utilities/Macros.h"
#include <iostream>

// Forward declarations
namespace logic {class KeyType;}

/**
* Global istream operator for the KeyType
* @param in an istream object to read from
* @param key the key to write to
* @return the changed istream object
*/
std::istream& operator>>(std::istream& in, logic::KeyType& key);

/**
* Global ostream operator for the KeyType
* @param out an ostream object to write to
* @param key the key to read from
* @return the changed ostream object
*/
std::ostream& operator<<(std::ostream& out, const logic::KeyType& key);

namespace logic
{

// Forward declaration
class KeyHandler;
class PlayerInfo;

/**
* The key type that all actors have
*/
class KeyType
{
public:
	// Friends :D
	friend std::istream& (::operator>>(std::istream& in, KeyType& key));
	friend std::ostream& (::operator<<(std::ostream& out, const KeyType& key));

	/**
	* Default Constructor for Key Type
	* @param key the key value, default -1 (INVALID)
	*/
	inline KeyType(int key = -1) : mKey(key) {}

	/**
	* Initializes the KeyType
	* 
	*/
	static void init();

	/**
	* Assignment operator
	* @param key assigns a new key value to the key
	*/
	inline void operator=(const KeyType& key) {mKey = key.mKey;}

	/**
	* Equality operator
	* @param key the right-handed key to compare with
	* @return true if the keys are equal
	*/
	inline bool operator==(const KeyType& key) const {return key.mKey == mKey;}

	/**
	* Differ operator
	* @param key the right-handed key to compare with
	* @return true if the keys differ
	*/
	inline bool operator!=(const KeyType& key) const {return key.mKey != mKey;}

	/**
	* Returns true if the key is less than the right key
	* @param key the right-handed key to compare with
	* @return true if the left key is less than the right key
	*/
	inline bool operator<(const KeyType& key) const {return mKey < key.mKey;}

	/**
	* Returns true if the key is less or equal to the right key
	* @param key the right-handed key to compare with
	* @return true if the left key is less or equal to the right key
	*/
	inline bool operator<=(const KeyType& key) const {return mKey <= key.mKey;}

	/**
	* Returns true if the key have a higher value than the right key
	* @param key the right-handed key to compare with
	* @return true if the left key have a higher value than the right key
	*/
	inline bool operator>(const KeyType& key) const {return mKey > key.mKey;}

	/**
	* Returns true if the key have a higher or equal value than the right key
	* @param key the right-handed key to compare with
	* @return true if the left key have a higher or equal value than the right key
	*/
	inline bool operator>=(const KeyType& key) const {return mKey >= key.mKey;}

	/**
	* Check if we're the owner of the key.
	* @return true if we're the owner of the key.
	*/
	bool isOwner() const;

	/**
	* Returns the owner of the key.
	* @return the player network id of the owner, INVALID_NETWORK_ID if it's invalid
	*/
	int getOwnerId() const;

	/**
	* Converts the id to a gfx id
	* @return converts the id to a gfx id
	*/
	inline int convertToGfxId() const {return mKey;}

	/**
	* KeyHandler is a friend class to access some private operators that shouldn't
	* be accessible to the public
	*/
	friend class KeyHandler;
	
private:
	/**
	* Division operator for an int
	* @param value the integer value we want to divide with
	* @return an integer with the divided value
	*/
	inline int operator/(int value) const {return mKey / value;}
	
	int mKey;

	static PlayerInfo* mpsPlayerInfo;
};

const int MAX_PLAYERS = 4;
const int MAX_KEYS_PER_PLAYER = 10000;
const KeyType INVALID_KEY(-1);
const KeyType KEY_FIRST(0);
const KeyType KEY_LIM(MAX_PLAYERS * MAX_KEYS_PER_PLAYER);
const KeyType KEY_GROUND(-2);
const KeyType KEY_TRIGGER(-3);

}

#endif