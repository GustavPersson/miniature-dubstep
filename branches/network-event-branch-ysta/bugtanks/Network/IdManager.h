/**
 * @file IdManager.h
 * @author Martin Wexö Olsson <mawm06@student.bth.se>
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

#ifndef _IDMANAGER_H_
#define _IDMANAGER_H_

#include <winsock.h>

#include <ostream>
#include <vector>
using namespace std;


/// Data structure for ID's.
struct IDstructure
{
	int id;
	string name;
	string ip;
	int port;
	SOCKET socket;
};

/////////////////////////////////////////////////
/// ... IdManager Class ...
/////////////////////////////////////////////////
/**
 * Class Description
 * The IdManager associates Sockets with id numbers and names for easy identification.
 */
class IdManager
{
private:
	vector<IDstructure> ids;
public:

	/**
    * A constructor
    */
	IdManager();
	
	/**
    * Register ID for a socket connection.
	* @param soc a socket connection.
    * @param id id number for connection.
    * @param name a name associated with the connection.
	* @param ip addres to peer connection.
	* @param port the port on witch the peer is accepting connections.
    */
	void registerID(SOCKET soc, int id, string name, string ip=string(""), int port=0);
	
	/**
    * Remove ID
	* @param soc the socket to be removed.
    */
	void removeID(SOCKET soc);
	
	/**
    * Returns a socket associated with the specified id.
	* @param id the id number associated with a socket.
	* @return SOCKET pointer to the socket associated with the specified id, returns NULL if socket not found.
    */
	SOCKET* getSocketByID(int id);
	
	/**
    * Returns a socket associated with the specified name.
	* @param name a name associated with a socket.
	* @return SOCKET pointer to the socket associated with the specified name, returns NULL if socket not found.
    */
	SOCKET* getSocketByName(string name);
	
	/**
    * Returns an id associated with the specified socket.
	* @param soc a registered socket.
	* @return int pointer to the id associated with the specified socket, returns NULL if socket not found.
    */
	int*	getIdBySocket(SOCKET soc);
	
	/**
    * Returns a name associated with the specified socket.
	* @param soc a registered socket.
	* @return string pointer to the name associated with the specified socket, returns NULL if socket not found.
    */
	string* getNameBySocket(SOCKET soc);
	
	/**
    * Set socket ids to the value of their index position in the list.
    */
	void setIdsToIndexValue();
	
	/**
    * Clears the list.
    */
	void clear();
	
	/**
    * Get the vector list of connection ids.
	* @return vector of connection ids.
    */
	vector<IDstructure> getIds()const;
	
	/**
	* Get numbers of ids registered.
    * @return size of the list.
    */
	int size()const;
};

ostream& operator << (ostream &out, const IdManager &idManager);

#endif
