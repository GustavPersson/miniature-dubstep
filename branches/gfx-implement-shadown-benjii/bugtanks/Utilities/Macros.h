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

#ifdef _DEBUG
#include <windows.h>
// Prints a debug message with the help of cout.
// Examples:
// DEBUG_MESSAGE("This is a message"); // A regular
// DEBUG_MESSAGE("Value of " << strMessage << " is: " << intValue);
#define DEBUG_MESSAGE(message) {std::cout << message << std::endl;}
#define DEBUG_WINDOW(x) {MessageBox(0, x, 0, 0);}
#else
#define DEBUG_MESSAGE(message)
#endif

// Error messages, uses streams so you can use it in the same manner as DEBUG_MESSAGE
// errorCode is the code number for the error
// message is the error message to display
#define ERROR_MESSAGE(errorCode, message) {std::cerr << std::left << "*****************\n" << std::setw(10) << "ERROR: " << errorCode << "\n" << std::setw(10) << "FILE: " << __FILE__ << "\n" << std::setw(10) << "LINE: " << __LINE__ << "\n" << std::setw(10) << "MESSAGE: " << message << "\n*****************" << std::endl;}

//Other useful defines
#define MEGABYTE (1024 * 1024)
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

//common warnings that are uninteresting
#pragma warning(disable:4244) //dowrdlong to unsigned int, 1 place

#endif