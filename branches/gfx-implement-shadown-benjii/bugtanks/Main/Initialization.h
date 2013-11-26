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
* Defines utility functions for game initialization.
*/
#ifndef __INITIALIZATION_H__
#define __INITIALIZATION_H__

#include <windows.h>
#include <string>


	/**
	* Checks to see if the user has enough free hard disk space to run the game.
	* @param diskSpaceNeeded The amount of disk space needed.
	*/
	extern void checkHardDisk(const DWORDLONG diskSpaceNeeded);

	/**
	* Checks if the user has enough physical and virtual RAM to run the game.
	* @param physicalRAMNeeded The amount of physical ram needed.
	* @param virtualRAMNeeded The amount of virtual RAM needed.
	*/
	extern void checkMemory(const DWORDLONG physicalRAMNeeded, const DWORDLONG virtualRAMNeeded);
	
	/**
	* Checks to see if there exists more than one instance of the game at once.
	* @param gameTitle The unique title of the game.
	* @return returns false if there is another instance of the game running, otherwise true.
	*/
	extern bool isOnlyInstance(LPCTSTR gameTitle);

	/**
	* Gets the Application's game save directory.
	* @param hWnd the HWND for the current running application
	* @param gameAppDirectory the directory from where the exe is running.
	* @return Returns the path as a TCHAR
	*/
	extern const TCHAR *getSaveGameDir(HWND hWnd, const TCHAR *gameAppDirectory);

/*
* Contains information read from an INI-file, containing game
* options, i.e. resolution, player name, sound volume etc.
*/
struct GameOptions
{
	bool mUseHardwareAccel;
	bool mUsePageFlipping;
	bool mUseDithering;
	bool mUseAntialiasing;
	float mSoundEffectsVolume;
	float mMusicVolume;
	int mScreenWidth;
	int mScreenHeight;
	std::string mPlayerName;

	GameOptions(const char* path);
};

#endif