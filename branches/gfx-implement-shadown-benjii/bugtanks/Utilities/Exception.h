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
*/

#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include "Macros.h"
#include <string>

namespace utilities
{

/**
* Base Class for exception handling.
* When an exception is catched, print the error using ERROR_MESSAGE and with the
* functions getMessage and getErrorCode.
*/
class Exception
{
public:
	/**
	* Constructor.
	* @param message a string message describing the error.
	* @param errorCode the errorCode that should be described in trac.
	*/
	Exception(const std::string& message, int errorCode);

	/**
	* Returns the error message.
	* @return error message of the exception.
	*/
	inline const std::string& getMessage() const
	{
		return mMessage;
	}

	/**
	* Returns the error code
	* @return error code of the exception.
	*/
	inline int getErrorCode() const
	{
		return mErrorCode;
	}

private:
	std::string mMessage;
	int mErrorCode;
};
}

#endif