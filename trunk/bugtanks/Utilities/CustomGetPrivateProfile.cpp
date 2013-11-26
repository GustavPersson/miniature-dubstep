/**
* @file
* @author Gustav Persson <ninjamuff@gmail.com>
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

#include <shlobj.h>
#include <cstdio>

float GetPrivateProfileFloat(const char* section, const char* key, float def_value, const char* filename)   
{   
     char buffer[64];

	 if(::GetPrivateProfileString(section, key, "", buffer, sizeof(buffer), filename))    
         return static_cast<float>(atof(buffer)); 

     return static_cast<float>(def_value); 
}

bool WritePrivateProfileFloat(const char* section, const char* key, float value, const char* filename) 
{ 
	char buffer[12]; 
	sprintf_s(buffer,"%.4f",value); 
	if(WritePrivateProfileString(section,key,buffer,filename)==ERROR_SUCCESS) 
		return true; 
	return false; 
}

bool WritePrivateProfileInt(const char* section, const char* key, int value, const char* filename) 
{ 
	char buffer[8]; 
	sprintf_s(buffer,"%i",value); 
	WritePrivateProfileString(section,key,buffer,filename); 
	return true; 
} 