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
 * 
 * @section DESCRIPTION
 *
 * Includes macros for debugging information along with memory leaks.
 * Macros for deallocation of dynamic allocated memory.
 */

#include "Macros.h"
#include <fstream>
#include <windows.h>
#include <set>
#include <map>

int gVerbosityLevelConsole = LEVEL_NONE;
int gVerbosityLevelFile = LEVEL_NONE;
int gOutputTargetError = OUTPUT_CONSOLE | OUTPUT_FILE;
int gOutputTargetDebugMessage = OUTPUT_CONSOLE;
int gcError = 0;
std::string gErrorFilePath = "../Logs/error.log";
std::string gDebugMessageFilePath = "../Logs/debug_message.log";

std::ofstream gErrorFile(gErrorFilePath.c_str());
std::ofstream gDebugMessageFile(gDebugMessageFilePath.c_str());

std::set<std::string> gDefinitions;
std::map<std::string, std::ofstream*> gOutputFiles;

void initLogger(int networkId)
{
	// Error file
	if (gErrorFile.is_open())
	{
		gErrorFile.close();
		gErrorFile.clear();
	}

	std::stringstream errorFile;
	errorFile << "../Logs/error_" << networkId << ".log";
	gErrorFilePath = errorFile.str();
	gErrorFile.open(gErrorFilePath.c_str());

	// Debug message file
	if (gDebugMessageFile.is_open())
	{
		gDebugMessageFile.close();
		gDebugMessageFile.clear();
	}

	std::stringstream debugFile;
	debugFile << "../Logs/debug_message_" << networkId << ".log";
	gDebugMessageFilePath = debugFile.str();
	gDebugMessageFile.open(gDebugMessageFilePath.c_str());
}

void releaseLogger()
{
	std::map<std::string, std::ofstream*>::iterator it;
	for (it = gOutputFiles.begin(); it != gOutputFiles.end(); ++it)
	{
		SAFE_DELETE(it->second);
	}

	gOutputFiles.clear();
}

void setVerbosityLevel(int verbosity, int target)
{
	if (target & OUTPUT_CONSOLE)
	{
		gVerbosityLevelConsole = verbosity;
	}

	if (target & OUTPUT_FILE)
	{
		gVerbosityLevelFile = verbosity;
	}
}

void checkForErrors()
{
	if (gcError > 0)
	{
		int choice = MessageBoxA(NULL, "There were some errors during the run-time. Do you want to open the error_log file?", "Errors occured!", MB_YESNO | MB_ICONSTOP);

		if (choice == IDYES)
		{
			std::string command = "notepad ";
			command += gErrorFilePath;
			system(command.c_str());
		}
	}
}

void printErrorMessage(const std::string& errorMessage, const char* file, long line)
{
	std::string time = getTimeStamp();

	gcError++;
	std::stringstream outMessage;
	outMessage << std::left << "****************************************************\n" << std::setw(10) <<
		"MESSAGE: " << errorMessage << "\n" << std::setw(10) <<
		"TIME: " << time << "\n" << std::setw(10) <<
		"FILE: " << file << "\n" << std::setw(10) <<
		"LINE: " << line << "\n****************************************************" << std::endl;

	if (gOutputTargetError & OUTPUT_FILE)
	{
		gErrorFile << outMessage.str() << std::endl;
	}

	if (gOutputTargetError & OUTPUT_CONSOLE)
	{
		std::cerr << outMessage.str() << std::endl;
	}
}

void printDebugMessage(int verbosity, const std::string& debugMessage, bool stop)
{
	std::string timeStamp = getTimeStamp();

	// File output
	if (verbosity <= gVerbosityLevelFile && gOutputTargetDebugMessage & OUTPUT_FILE)
	{
		gDebugMessageFile << std::left << std::setw(14) << timeStamp << debugMessage << std::endl;
	}

	// console output
	if (verbosity <= gVerbosityLevelConsole)
	{
		if (gOutputTargetDebugMessage & OUTPUT_CONSOLE)
		{
			std::cout << std::left << std::setw(15) << timeStamp << debugMessage << std::endl;
		}

		if (stop)
		{
			std::cin.ignore(100, '\n');
		}
	}
}

void printFileMessage(const std::string& defined, const std::string& filePath, const std::string& message)
{
	// If defined is defined, print the output
	if (gDefinitions.count(defined) > 0)
	{
		std::string timeStamp = getTimeStamp();

		// Find the output
		std::map<std::string, std::ofstream*>::iterator it = gOutputFiles.find(filePath);
		std::ofstream* pFile = NULL;

		// Use the output
		if (it != gOutputFiles.end())
		{
			pFile = it->second;
		}
		// Open the file
		else
		{
			pFile = gOutputFiles[filePath] = myNew std::ofstream(filePath.c_str());
		}

		(*pFile) << std::left << std::setw(15) << timeStamp << message << std::endl;
	}
}

void define(const std::string& definition)
{
	gDefinitions.insert(definition);
}

std::string getTimeStamp()
{
	SYSTEMTIME curTime;
	GetSystemTime(&curTime);

	std::stringstream timeStamp;

	timeStamp << std::setfill('0') << std::setw(2) << curTime.wHour+1 << ":" <<
		std::setw(2) << curTime.wMinute << ":" <<
		std::setw(2) << curTime.wSecond << ":" <<
		std::setw(3) << curTime.wMilliseconds << std::setfill(' ') << " ";

	return timeStamp.str();
}