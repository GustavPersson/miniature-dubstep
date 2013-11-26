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

#include "../Utilities/Macros.h"
#include "../Utilities/ErrorHandler.h"
#include "../Utilities/CustomGetPrivateProfile.h"
#include <shlobj.h>
#include <direct.h>
#include "Initialization.h"

using namespace logic;
namespace logic {

void checkHardDisk(const DWORDLONG diskSpaceNeeded)
{
	int const mDrive = _getdrive();
	struct _diskfree_t mDiskFree;

	_getdiskfree(mDrive, &mDiskFree);

	unsigned __int64 const neededClusters = 
		diskSpaceNeeded /
		(mDiskFree.sectors_per_cluster * mDiskFree.bytes_per_sector);

	if (mDiskFree.avail_clusters < neededClusters)
	{
		//not enough disk space!
		ERROR_MESSAGE("You don't have enough disk space to run the game!");
		throw ErrorHandler(90001);
	}
}

void checkMemory(const DWORDLONG physicalRAMNeeded, const DWORDLONG virtualRAMNeeded)
{
	MEMORYSTATUSEX mMemStatus;
	
	mMemStatus.dwLength = sizeof(mMemStatus);

	GlobalMemoryStatusEx(&mMemStatus);
	if (mMemStatus.ullTotalPhys < physicalRAMNeeded)
	{
		//if there's not enough memory, the player should probably get a real computer
		ERROR_MESSAGE("You don't have enough physical memory, loser.");
		throw ErrorHandler(90002);
	}

	if (mMemStatus.ullAvailVirtual < virtualRAMNeeded)
	{
		//Not enough virtual memory. How is this even possible.
		ERROR_MESSAGE("You don't have enough virtual memory. Try closing other programs.");
		throw ErrorHandler(90003);
	}

	char *mBuffer = myNew char[static_cast<unsigned int>(virtualRAMNeeded)];

	if(mBuffer)
		delete[] mBuffer;
	else
	{	
		//The system lied. When attempting to grab as much memory as needed,
		//we failed to do so. SOmething else is eating up memory in the background.
		ERROR_MESSAGE("You don't have enough contiguous RAM. Try closing some other programs.");
		throw ErrorHandler(90004);
	}
}
//TODO: Fix this.
bool isOnlyInstance(LPCSTR gameTitle)
{
	HANDLE handle;
	handle = CreateMutex(NULL, TRUE, gameTitle);

	if (GetLastError() != ERROR_SUCCESS)
	{
		HWND hWnd = FindWindow(NULL, "BugTanks");
		if (hWnd) //An instance is already running
		{
			ShowWindow(hWnd, SW_SHOWNORMAL);
			SetFocus(hWnd);
			SetForegroundWindow(hWnd);
			SetActiveWindow(hWnd);
			MessageBox(hWnd, "Another instance of the game is already running! You can only run one at a time.", "Error", 0);
			return false;
		}
	}
	return true;
}

const TCHAR *getSaveGameDir(HWND hWnd, const TCHAR *gameAppDirectory)
{
	if (hWnd)
		return gameAppDirectory;

	return gameAppDirectory;
}
}

GameOptions::GameOptions(const char *path)
{
	mUseHardwareAccel = GetPrivateProfileIntA(
		"DISPLAY", "Hardware_Acceleration", false, path) ? true : false;

	mScreenWidth = ::GetPrivateProfileIntA(
		"DISPLAY", "Screen_Width", 640, path);

	mScreenHeight = ::GetPrivateProfileIntA(
		"DISPLAY", "Screen_Height", 480, path);

	mMusicVolume = ::GetPrivateProfileFloat(
		"SOUND", "Music_Volume", false, path);

	mSoundEffectsVolume = ::GetPrivateProfileFloat(
		"SOUND", "Sound_Effects_Volume", false, path);

	char buffer[256];
	::GetPrivateProfileStringA(
		"MULTIPLAYER", "Player_Name", "Unknown Player", buffer, 256, path);
	mPlayerName = buffer;

	mIsServer = ::GetPrivateProfileIntA(
		"MULTIPLAYER", "Is_Server", true, path) ? true : false;

	::GetPrivateProfileStringA(
		"MULTIPLAYER", "Server_Address", "localhost", buffer, 256, path);
	mServerAddress = buffer;

	mTankType = static_cast<Tank::TankTypes>(::GetPrivateProfileIntA(
		"MULTIPLAYER", "Tank_Type", 1, path));

	mNumberOfPlayers = ::GetPrivateProfileIntA(
		"MULTIPLAYER", "Number_Of_Players", -1, path);

	mLoadAllMeshes = ::GetPrivateProfileIntA(
		"DEBUGGING", "Load_All_Meshes", 1, path) ? true : false;

	mLoadAdvancedMeshes = ::GetPrivateProfileIntA(
		"DEBUGGING", "Load_Advanced_Meshes", 0, path) ? true : false;

	mDisableAI = ::GetPrivateProfileIntA(
		"DEBUGGING", "Disable_AI", 0, path) ? true : false;

	mEnableEditor = ::GetPrivateProfileIntA(
		"DEBUGGING", "Editor_Enabled", 0, path) ? true : false;

	mEditorMode = false;
}