/**
* @file
* @author Gustav Persson <gustav@alltomminrov.se>
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

#ifndef __CUSTOM_GET_PRIVATE_PROFILE_H__
#define __CUSTOM_GET_PRIVATE_PROFILE_H__

#include "../Utilities/Macros.h"

/**
* Gets a float from an .ini-file. Saves trouble in having to convert the int
* function built in to the windows thingymajong.
* @param section The section of the file
* @param key The information key
* @param def_value The default value
* @param filename The path to the file to be read!!!!!!!!!! :):):):):)
* @return Returns the read value as a float, and if unable to read, the default value.
*/
float GetPrivateProfileFloat(const char* section, const char* key, float def_value, const char* filename);

/**
* Writes a float to an .ini-file.
* @param section The section of the file
* @param key The information key
* @param def_value The default value
* @param filename The path to the file to be read
* @return returns true if we successfully wrote the value
*/
bool WritePrivateProfileFloat(const char* section, const char* key, float value, const char* filename);

/**
* Writes an int to an .ini-file.
* @param section The section of the file
* @param key The information key
* @param def_value The default value
* @param filename The path to the file to be read
* @return returns true if we successfully wrote the value
*/
bool WritePrivateProfileInt(const char* section, const char* key, int value, const char* filename);
#endif