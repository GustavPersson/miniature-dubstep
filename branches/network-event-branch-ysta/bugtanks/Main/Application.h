/**
* @file
* @author Gustav Persson <ninjamuff@gmail.com>
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
* Defines the entry point for the application
*/

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Initialization.h"
#include "../Utilities/Macros.h"
#include "../Logic/EventManager.h"
#include "../Logic/ActorHandler.h"
#include "../Logic/InputStates.h"

#include "../GFX/DX10Module.h"

#include "../Sound/SoundSystem.h"

/**
* Wrapper class defining the entry point, 
* main loop and exit point for the application.
*/
class Application
{
public:
	/**
	* Constructor. Initializes local pointers.
	*/
	Application();

	/**
	* The Application's entry point. Tests if the user's computer meets
	* some bare-minimum requirements for running the game.
	*/
	virtual bool InitInstance();

	/**
	* Creates the game logic, setting the stepping-stones for
	* the application to run as intended.
	*/
	void createLogic();
	
	/**
	* Registers events with event listeners.
	*/
	void registerEvents();
	
	/**
	* The main game loop.
	*/
	void runGame();
	
	/**
	* Creates a game world.
	*/
	void setUpGame();
	
	/**
	* Returns the title of the application window.
	* @return Window title as a LPCSTR
	*/
	LPCSTR getGameTitle();
	
	/**
	* Deletes objects used during runtime.
	*/
	void onClose();
	
	
	struct GameOptions * mpGameOptions;


protected:
	bool mbWindowedMode;
	bool mbIsRunning;

	gfx::DX10Module *mdxModule;

	logic::EventManager* mpEventManager;
	logic::EventListenerPtr snooper;
	logic::EventListenerPtr logicListener;
	logic::EventListenerPtr epActorHandler;
	logic::EventListenerPtr soundListener;
	logic::ActorHandler *actorHandler;

	gfx::Camera *mCamera;
	gfx::InstanceMesh *mMesh;

	logic::InputStates *mpInputStates;

	sound::SoundSystem *mSoundSystem;
	
	logic::GlobalTimer *mglobalTimer;
	logic::LogicQuadTree *pQuadTree;

	logic::PlayerInfo *mPlayerInfo;

private:

};

#endif