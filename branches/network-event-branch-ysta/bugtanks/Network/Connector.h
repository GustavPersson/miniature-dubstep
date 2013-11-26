/**
 * @file Connector.h
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

#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include <winsock.h>

#include <string>
#include <pthread.h>

#include "ConnectionManager.h"

/////////////////////////////////////////////////
/// ... Connector Class ...
/////////////////////////////////////////////////
/**
 * Class Description
 * The Connector is used as a component for establishing and listening for connections.
 * If a connection manager is set, all new connnections made are addded to it.
 */
class Connector
{
private:
	SOCKET mListeningSocket;
	pthread_t mSocketListener;
	ConnectionManager *mConnectionManager;


	/**
    * This is the listening function, it runs as a separate thread.
	* @param args a pointer to the functions argument.
    */
	static void* socketFunc(void *args);
	
public:
	/**
    * A constructor.
	* Sets the ConnectionManager to operate on.
	* @param connectionManager a pointer to a ConnectionManager object, Default value is NULL.
    */
	Connector(ConnectionManager *connectionManager=NULL);

	/**
    * Sets the ConnectionManager to operate on.
    * @param CM a pointer to a ConnectionManager object.
    */
	void setConnectionManager(ConnectionManager *CM);
	
	/**
    * Setup a listening socket.
    * @param port the port to listen to.
    */
	bool startAccept(int port=0);

	/**
    * Stop listening, terminates the listening thread and closes the socket.
    * @see startAccept(int port=0)
    */
	void stopAccept();

	/**
    * Tries to create a connection and then if there is a connection manager registered
	* the connection will be added there aswell.
    * @param addr a pointer to a c string with the addres.
	* @param port the port number the other side is listening to.
	* @param connection a pointer to a MessageHandler object, used as a secondary return value.
	* Returns NULL if no connection was established.
	* @return the success of the operation, false means that no exeption was established.
    */
	bool connectTo(const char *addr, int port, SOCKET *connection=NULL);
};

#endif 
