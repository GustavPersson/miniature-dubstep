/**
* @file
* @author Jonathan Udd <jonis.x@gmail.com>
* @version 1.0
* Copyright (�) A-Team.
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

#ifndef __LOGICTEST__LOGIC_APPLICATION_H__
#define __LOGICTEST__LOGIC_APPLICATION_H__

#include "../Utilities/Macros.h"
#include "../Logic/Application.h"

/**
* The AI Test Application
*/
class LogicApplication : public logic::Application
{
public:
	/**
	* Destructor
	*/
	virtual ~LogicApplication();

	/**
	* Creates an instance of the AiApplication.
	*/
	static void createInstance();

protected:
	/**
	* Protected constructor
	*/
	LogicApplication();

	/**
	* Virtual function to create listeners
	*/
	virtual void createListeners();

	/**
	* The loop that runs while we're in the main game loop.
	* @param deltaTime Time elapsed since last frame.
	*/
	virtual void gameLoop(float deltaTime);

	/**
	* Loads and instances meshes to be made.
	* Override this to only load the meshes you need.
	* @note Does NOT make mashed potatoes.
	*/
	virtual void loadMeshes();

	/**
	* Creates all objects we want to have in the game.
	* Override this if you want to create you're own functions
	*/
	virtual void createObjects();

	/**
	* Sets the command window, only called in _DEBUG
	* Override this to set it to your own position.
	*/
	virtual void setCommandWindow();

	/**
	* Registers events with event listeners.
	*/
	virtual void registerEvents();

private:

	logic::EventListenerPtr mGFXListener;
};

#endif