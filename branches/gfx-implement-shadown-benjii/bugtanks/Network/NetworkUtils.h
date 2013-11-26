/**
 * @file NetworkUtils.h
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

#ifndef _NETWORKUTILS_H_
#define _NETWORKUTILS_H_

#include <winsock.h>

namespace network
{

/**
 * Get the address and port to the other end of the socket
 * @param soc the socket we want to get the address from.
 * @param addr a refference to a string pointer.
 * @param port a refference to an int.
 */
void getPeerAddress(SOCKET soc, char **addr, int *port);

}

#endif
