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

#ifndef __MACROS_H__
#define __MACROS_H__

#define WINDOWS_LEAN_AND_MEAN

//---------------------------------------------------------------------
// Include Client BuildInfo
// If you don't have this file. Create the file as:
// .../trunk/bugtanks/BuildInfo.h
// To enable a feature define the following:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Available features:
// USE_NETWORK - Enables network in Application
// USE_EDITOR - Enables editor
// USE_GUI - Enables gui
// USE_AI - Enables the AI in the logic.
// AI_DRAW_MAP_POTENTIAL_FIELD
// AI_DRAW_PLAYER_POTENTIAL_FIELD
// AI_DRAW_AGENT_GOALS
// AI_DRAW_UNIT_SIZE_BLOCKS
// AI_DRAW_UNIT_ORIGIN_LINES
//---------------------------------------------------------------------
#include "../BuildInfo.h"

//-----------------------------------------------------------------------------
// Pragma these warnings under development, should be removed when we're done
//-----------------------------------------------------------------------------
#pragma warning(disable: 4100)

//-----------------------------------
// Memory leaks
//-----------------------------------
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#define myNew new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define initMemoryCheck() _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)

#include <cstdlib>
#include <crtdbg.h>
#else
#define myNew new
#define initMemoryCheck()
#endif

// Safe pthread_mutex_destroy, you must include pthreads yourself!
#define SAFE_MUTEX_DESTROY(x) {pthread_mutex_destroy(x);}

// Safe delete 
#define SAFE_DELETE(x) {if (x) {delete x; x = NULL;}}
#define SAFE_DELETE_ARRAY(x) {if (x) {delete [] x; x = NULL;}}

// Safe resource delete
#define SAFE_RELEASE(x) {if(x) { x->Release(); x = 0; }}

//------------------------------------
// Debug messages and error messages
//------------------------------------
#include <iostream>
#include <iomanip>
#include <sstream>

// The verbosity levels to use
#define LEVEL_NONE		0	/**< Displays no messages */
#define LEVEL_HEATON	1337 /**< Ultimate level. Very hard to achieve. */
#define LEVEL_LOWEST	7	/**< Displays all messages */
#define LEVEL_LOWER		6	/**< Displays most messages */
#define LEVEL_LOW		5	/**< Displays some messages */
#define LEVEL_MEDIUM	4	/**< Displays all messages of medium importance, this should not be used in the "deep" functions */
#define LEVEL_HIGH		3	/**< Displays all messages that have high importance */
#define LEVEL_HIGHER	2	/**< Displays all messages that are very important */
#define LEVEL_HIGHEST	1	/**< Only displays the extremely important messages */

#ifdef _DEBUG
#include <windows.h>

/**
* Prints a debug message. Only active if _DEBUG is defined
* Examples:
* DEBUG_MESSAGE("This is a message", LEVEL_MEDIUM); // A regular
* DEBUG_MESSAGE("Value of " << strMessage << " is: " << intValue, LEVEL_LOWER);
* @param message the message to display, using streams are possible
* @param verbosity the verbosity level to use
*/
#define DEBUG_MESSAGE(verbosity, message) {std::stringstream ss; ss << message; printDebugMessage(verbosity, ss.str(), false);}

/**
* Prints a debug message and waits for input after the message has been displayed.
* Only active if _DEBUG is defined
* @param message the message to display, using streams are possible
* @param verbosity the verbosity level to use
*/
#define DEBUG_MESSAGE_STOP(verbosity, message) {std::stringstream ss; ss << message; printDebugMessage(verbosity, ss.str(), true);}
#define DEBUG_WINDOW(x) {MessageBox(0, x, 0, 0);}
#else
#define DEBUG_MESSAGE(message, verbosity)
#define DEBUG_MESSAGE_STOP(message, verbosity)
#endif

// The types of output targets that are available
#define OUTPUT_CONSOLE		0x0001	/**< Output messages to the console window */
#define OUTPUT_FILE			0x0002	/**< Output messages to a file */

extern int gVerbosityLevelConsole;
extern int gVerbosityLevelFile;
extern int gOutputTargetError;
extern int gOutputTargetDebugMessage;
extern int gcError;
extern std::string gErrorFilePath;
extern std::string gDebugMessageFilePath;

/**
* Initializes the logger with a network id. You don't have to run this if you don't want output
* to different files. It opens the appropriate log files depending on the specified network id.
* This function should be used if you want multiple log files (if you run several clients)
* on your own computer.
* @param networkId the network id we have.
*/
void initLogger(int networkId);

/**
* Set the output target for the error messages. The parameter values can be combined
* with the binary | operator.
* @param targets the specified output targets, can be OUTPUT_CONSOLE, OUTPUT_FILE.
*/
inline void setOutputTargetError(int targets) {gOutputTargetError = targets;}

/**
* Set the output target for the error messages. The parameter values can be combined
* with the binary | operator.
* @param targets the specified output targets, can be OUTPUT_CONSOLE, OUTPUT_FILE.
*/
inline void setOutputTargetDebugMessage(int targets) {gOutputTargetDebugMessage = targets;}

/**
* Sets the verbosity level.
* @param verbosity the level of verbosity we want to set, between LEVEL_HIGHEST and LEVEL_LOWEST.
* @param the target to set the verbosity level in, can be OUTPUT_CONSOLE or OUTPUT_FILE
* @note LEVEL_HIGHEST only displays messages that are of highest priority and LEVEL_LOWEST
* displays all messages
*/
void setVerbosityLevel(int verbosity, int target);

/**
* Error messages, uses streams so you can use it in the same manner as DEBUG_MESSAGE
* @param message is the error message to display
*/
#define ERROR_MESSAGE(message) \
{ \
	std::stringstream ss; \
	ss << message; \
	printErrorMessage(ss.str(), __FILE__, __LINE__);\
}

/**
* Checks for error. Should be called 'last' in the program. Checks if there
* were any errors, prints how many there were and gives the option to open
* the error_log file.
*/
void checkForErrors();

/**
* Actually prints the error message
* @param errorMessage the error message
* @param file the file which the error is in
* @param line the line on which the error message appeared on
* @param time the timestamp of the error
*/
void printErrorMessage(const std::string& errorMessage, const char* file, long line);

/**
* Actually prints the debug message
* @param verbosity the verbosity level to use
* @param debugMessage the debug message
* @param stop if we should stop at the message, default is false
*/
void printDebugMessage(int verbosity, const std::string& debugMessage, bool stop = false);

/**
* Returns a string with the current timestamp in hours, minutes, seconds and milliseconds.
* @return current timestamp in hours, minutes, seconds and milliseconds.
*/
std::string getTimeStamp();

//Other useful defines
#define MEGABYTE (1024 * 1024)
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#endif