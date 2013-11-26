/**
 * @file
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
 * 
 * @section DESCRIPTION
 *
 * Class Description
 */

#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <pthread.h>
#include "Network.h"

//--------------------------------------------------
//	Connection Class
//--------------------------------------------------
class Connection
{
private:
	SOCKET mSocket;
	pthread_t mHandleConnection;
	void *mHandleFunc(void* args);

public:
	Connection();
	Connection(SOCKET soc);

	void connectTo(const char *address, int port);
	bool isConnected()const;

	void handleMe(void *handleFunc(void* args));
};

#endif
