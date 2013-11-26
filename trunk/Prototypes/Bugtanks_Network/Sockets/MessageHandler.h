/**
 * @file MessageHandler.h
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

#ifndef _MESSAGEHANDLER_H_
#define _MESSAGEHANDLER_H_

#include "Network.h"

/////////////////////////////////////////////////
/// ... MessageHandler Class ...
/////////////////////////////////////////////////

/**
* Class Description
 * The MessageHandler is a virtual class that is used as a callback function
 * everytime a new message is received. This is just a base class, so to actualy
 * handle any messages, a new message handeling class has to be derived form this class.
 */
class MessageHandler
{
public:
	/**
    * This is a virtual callback function that is called whenever a new message is received.
	* @param src the socket where the message originated from.
	* @param len the length of the message in bytes.
	* @param type the type of the message.
	* @param message the actual message.
    */
	virtual void handleMessage(SOCKET src, int len, int type, const char *message)=0;
};

#endif
