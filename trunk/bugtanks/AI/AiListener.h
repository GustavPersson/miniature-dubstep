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

#ifndef __AI__AI_LISTENER_H__
#define __AI__AI_LISTENER_H__

#include "../Utilities/Macros.h"
#include "../Logic/IEventManager.h"
#include "../Logic/PlayerInfo.h"

namespace ai
{

// Forward declarations
class AiFactory;
class UnitHandler;
class PathFinder;

/**
* Listens to different events that the AI component needs to listen to
*/
class AiListener : public logic::EventListener
{
public:
	explicit AiListener();
	virtual ~AiListener();

	char const * getName() {return "AiListener";}

	/**
	* Handles the events that is sent by another part of the system
	* @param eventData the data of the event
	*/
	bool handleEvent(logic::EventData const &eventData);

private:
	AiFactory* mpAiFactory;
	UnitHandler* mpUnitHandler;
	PathFinder* mpPathFinder;
	logic::PlayerInfo* mpPlayerInfo;
};
}

#endif