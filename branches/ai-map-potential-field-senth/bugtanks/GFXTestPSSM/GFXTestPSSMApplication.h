/**
* @file
* @author Benjamin Golba <benjii.g@gmail.com>
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

#ifndef __GFXTESTPSSM__GFXTESTPSSMAPPLICATION_H
#define __GFXTESTPSSM__GFXTESTPSSMAPPLICATION_H

#include "../Utilities/Macros.h"
#include "../Logic/Application.h"

/**
* The AI Test Application
*/
class GFXTestPSSMApplication : public logic::Application
{
public:
	/**
	* Destructor
	*/
	virtual ~GFXTestPSSMApplication();

	/**
	* Creates an instance of the GFXTestPSSMApplication.
	*/
	static void createInstance();

protected:
	/**
	* Protected constructor
	*/
	GFXTestPSSMApplication();

	/**
	* The loop that runs while we're in the main game loop.
	* @param deltaTime Time elapsed since last frame
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
};

#endif