/**
 * @file
 * @author Viktor Svensson <arkangel88@gmail.com>
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
 * An event listener for the sound component.
 * 
 */


#ifndef __SOUNDLISTENER_H__
#define __SOUNDLISTENER_H__

#include "../Utilities/Macros.h"
#include "../Logic/IEventManager.h"
#include "SoundSystem.h"

namespace sound
{

class SoundListener : public logic::EventListener
{
public:
	
	explicit SoundListener();
	virtual ~SoundListener();
	char const * getName(void) {return "SoundListener";}

	/**
	* Handles the events that is sent by another part of the system. 
	*/
	bool handleEvent(logic::EventData const &eventData);

private:
	logic::ActorHandler *mpActorHandler;
	logic::PlayerInfo *mPlayer;
	sound::SoundSystem *mSoundSystem;
	int mCollisionHouseChanger;
		
};	

}
#endif
