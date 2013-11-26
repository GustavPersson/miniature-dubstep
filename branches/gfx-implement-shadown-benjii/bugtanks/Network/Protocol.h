/**
 * @file Protocol.h
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

#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

namespace network
{

/**
* Message Type defines
* All messages types should be listed in this header.
*/

enum MessageTypes {
	// First type
	MessageType_First = 0,

	// Game Server & Client types.
	MessageType_JoinRequest = 0,
	MessageType_JoinRequest_Granted,
	MessageType_JoinRequest_Denied,
	MessageType_Dropout,
	MessageType_PublishList,

	// Peer network types.
	MessageType_PlayerID,


	// General types.
	MessageType_TextMessage,
	MessageType_Event,


	// Last type
	MessageType_Lim
};
}


#endif