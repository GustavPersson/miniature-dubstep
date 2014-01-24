/**
 * @file
 * @author Gustav Persson <gustav@alltomminrov.se>
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
 * Definition of the Event Snooper class.
 */

#ifndef __SNOOPER_H__
#define __SNOOPER_H__
#include "../Utilities/Macros.h"
#include "IEventManager.h"
#include <vector>
#include <list>
#include <map>
#include <set>
#include <stdio.h>
#include "windows.h"

namespace logic
{

	/**
	* A class that listens to all events and spits out information to the
	* command window, and optionally a file. Specify file name in the constructor
	* to enable file writing.
	* 
	*/
class EventSnooper : public EventListener
{
public:
	/**
	 * Constructor. Finds the path to the current .exe and places the file there..
	 * @param logFileName The name of the log file.
	 */
	explicit EventSnooper(char const * const logFileName = NULL);
	virtual ~EventSnooper();
	char const * getName(void) {return "Snooper";}
	/**
	 * Collects information about an event and then prints it to a file.
	 * @param eventData The event that's firing.
	 */
	bool handleEvent(EventData const &eventData);

private:
	FILE *mOutFile;
};
}
#endif