#ifndef __HASHEDSTRING_H__
#define __HASHEDSTRING_H__

//========================================================================
// HashedString.h : Defines some useful string utility functions
//
// Part of the GameCode3 Application
//
// GameCode3 is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete - 3rd Edition" by Mike McShaffry, published by
// Charles River Media. ISBN-10: 1-58450-680-6   ISBN-13: 978-1-58450-680-5
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the author a favor and buy a new copy of the book - there are 
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here: 
//    http://www.amazon.com/gp/product/1584506806?ie=UTF8&tag=gamecodecompl-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=1584506806
//
// There's a companion web site at http://www.mcshaffry.com/GameCode/
// 
// The source code is managed and maintained through Google Code: http://gamecode3.googlecode.com/svn/trunk/
//
// (c) Copyright 2009 Michael L. McShaffry
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License v2
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//========================================================================


#include <string>

/*
* This is a class that hashes a string to make it easier to use events and stuff.
*/

namespace utilities
{

class HashedString
{
public:

	/**
	* Constructor. Does basically nothing.
	* @param pIdentString the string to be hashed.
	*/
	explicit HashedString(char const* const pIdentString) 
				: mIdent(hashName(pIdentString)), mIdentStr(pIdentString)
	{

	}

	/**
	* Gets the hash value.
	* @return Returns the hash value of the string
	*/
	unsigned long getHashValue(void) const
	{
		return reinterpret_cast<unsigned long>(mIdent);
	}

	/**
	* Gets the string representation.
	*  @return returns the string representation of a hash
	*/
	const std::string &getStr() const
	{
		return mIdentStr;
	}

	/*
	* Relatively simple hash of arbitrary text string into a
	* 32-bit identifier Output value is
	* input-valid-deterministic, but no guarantees are made
	* about the uniqueness of the output per-input
	*
	* Input value is treated as lower-case to cut down on false
	* separations cause by human mistypes. Sure, it could be
	* construed as a programming error to mix up your cases, and
	* it cuts down on permutations, but in Real World Usage
	* making this text case-sensitive will likely just lead to
	* Pain and Suffering.
	*
	* This code loosely based upon the adler32 checksum by Mark
	* Adler and published as part of the zlib compression
	* library sources.
	*
	*/
	static void *hashName(char const *pIdentStr);


	bool operator < (HashedString const & o) const
	{
		bool r = (getHashValue() < o.getHashValue());
		return r;
	}

	bool operator == (HashedString const & o) const
	{
		bool r = (getHashValue() == o.getHashValue());
		return r;
	}


private:
	void* mIdent;
	std::string mIdentStr;
};
}
#endif