#pragma once
/**
* @file
* @author Gustav Persson <gustav@alltomminrov.se>
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
* Handles errors etc.
*/

#ifndef __ERROR_HANDLER_H__
#define __ERROR_HANDLER_H__

#define ERR_ABORT		95000
#define ERR_RETRY		96000
#define ERR_IGNORE		97000

class ErrorHandler
{
public:
	ErrorHandler(const int code) : mCode(code) {};
	
	int handle() { return mCode; }

	ErrorHandler & operator=(ErrorHandler &o)
	{
		if (this != &o)
		{
			int & intRef = const_cast<int&>(mCode);
			intRef = o.mCode;
		}
		return *this;
	}

protected:
	const int mCode;
};
#endif