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

#ifndef __VECTOR_LIST_H__
#define __VECTOR_LIST_H__

#include "Macros.h"
#include "Exception.h"
#include <climits>
#include <cassert>

namespace utilities
{

/**
* A linked list together with the functionality of a Vector.
* However it uses a fixed size. By default it increases the size of the
* list whenever it reaches it's max-size. However it's possible to
* specify that the VectorList throws an exception when you try to add
* an object when the VectorList is full, i.e. at the max-size.
*/
template <typename T>
class VectorList
{
public:
	/**
	* Thrown when trying to dereference an index which is out of bounds.
	*/
	class VectorListIndexOutOfBoundsException : public Exception
	{
	public:
		VectorListIndexOutOfBoundsException() : Exception("IndexOutOfBoundsException: Index is out bounds in VectorList!", 70001) {}
	};

	/**
	* Thrown when trying to add a new element and the VectorList is full.
	*/
	class VectorListFullException : public Exception
	{
	public:
		VectorListFullException() : Exception("VectorListFullException: Can't add more elements to VectorList!", 70000) {}
	};

	/**
	* Thrown when trying to do an action on an empty VectorList that needs at least one element.
	*/
	class VectorListEmptyException : public Exception
	{
	public:
		VectorListEmptyException() : Exception("VectorListEmptyException: Can't operate on a empty VectorList!", 70002) {}
	};

	/**
	* Constructor
	* @param initialSize the initial size the VectorList will have
	* @param sizeMax the absolutely maximum size the VectorList should have
	*/
	VectorList(int initialSize = 0, int sizeMax = INT_MAX)
	{
		mArraySize = initialSize;
		mcElements = 0;
		mBegin = 0;
		mArraySizeMax = sizeMax;
		if (mArraySize == 0)
		{
			mpArray = NULL;
		}
		else
		{
			mpArray = myNew T [mArraySize];
		}
	}

	/**
	* Destructor
	*/
	~VectorList()
	{
		SAFE_DELETE_ARRAY(mpArray);
	}

	// Will only implement copy-constructor once we need it

	/**
	* Adds a new element
	* @throws VectorListIndexOutOfBoundsException
	* @param element the element to add
	*/
	void add(const T& element)
	{
		if (mcElements == mArraySize)
		{
			int oldArraySize = mArraySize;
			
			// Throw exception if full
			if (mArraySize == mArraySizeMax)
			{
				throw VectorListIndexOutOfBoundsException();
			}
			// If we're close to the maximum size we can't create INCREMENT_SIZE objects
			else if (mArraySize + INCREMENT_SIZE > mArraySizeMax)
			{
				mArraySize = mArraySizeMax;
			}
			// It's safe to allocate + INCREMENT_SIZE objects
			else
			{
				mArraySize += INCREMENT_SIZE;
			}

			// Create the new array and copy the old one
			T* tempArray = myNew T [mArraySize + INCREMENT_SIZE];

			for (int i = 0; i < oldArraySize; i++)
			{
				tempArray[i] = mpArray[i];
			}

			// Delete the old one and set the new one
			SAFE_DELETE_ARRAY(mpArray);
			mpArray = tempArray;

		}
		int addPosition = mBegin + mcElements;
		
		// Wrap the position
		if (addPosition >= mArraySize) {
			addPosition -= mArraySize;
		}

		// "Add" the new element
		mpArray[addPosition] = element;

		mcElements++;
	}

	/**
	* Returns a reference to the element at the specified index
	* @throws VectorListIndexOutOfBoundsException
	* @param index the index
	* @return reference to the element at the specified index
	* @note generates an error message when index is out of bounds
	*/
	T& operator[](int index)
	{
		// Check for out of bounds (with assertion too)
		if (index < 0 || index >= mcElements)
		{
			throw VectorListIndexOutOfBoundsException();
		}

		// Wrap the index
		int arrayIndex = mBegin + index;
		if (arrayIndex >= mArraySize) {
			arrayIndex -= mArraySize;
		}

		return mpArray[arrayIndex];
	}
	
	/**
	* Returns a const reference to the element at the specified index
	* @throws VectorListIndexOutOfBoundsException
	* @param index the index
	* @return reference to the element at the specified index
	* @note generates an error message when index is out of bounds
	*/
	const T& operator[](int index) const
	{
		// Check for out of bounds (with assertion too)
		if (index < 0 || index >= mcElements)
		{
			throw IndexOutOfBoundsException();
		}

		// Wrap the index
		int arrayIndex = mBegin + index;
		if (arrayIndex >= mArraySize) {
			arrayIndex -= mArraySize;
		}

		return mpArray[arrayIndex];
	}

	/**
	* Returns the size of the VectorList
	* @return the number of elements in the vectorlist
	*/
	inline int size() const
	{
		return mcElements;
	}

	/**
	* Removes the first element in the array.
	* @throws VectorListEmptyException
	*/
	void removeFirst()
	{
		if (mcElements == 0)
		{
			throw VectorListEmptyException();
		}
		else
		{
			mcElements--;
			mBegin++;
			if (mBegin == mArraySize)
			{
				mBegin = 0;
			}
		}
	}

	/**
	* Removes the last element in the array.
	* @throws VectorListEmptyException
	*/
	void removeLast()
	{
		if (mcElements == 0)
		{
			throw VectorListEmptyException();
		}
		else
		{
			mcElements--;
		}
	}

	/**
	* Returns a const reference to the first element in the array.
	* @throws VectorListEmptyException
	* @return const reference to the first element in the array.
	*/
	inline const T& getFirst() const
	{
		if (mcElements == 0)
		{
			throw VectorListEmptyException();
		}

		return mpArray[mBegin];
	}

	/**
	* Returns a reference the first element in the array.
	* @throws VectorListEmptyException
	* @return reference to the first element in the array.
	*/
	inline T& getFirst()
	{
		if (mcElements == 0)
		{
			throw VectorListEmptyException();
		}

		return mpArray[mBegin];
	}

	/**
	* Returns a const reference the last element in the array.
	* @throws VectorListEmptyException
	* @return const reference to the last element in the array.
	*/
	const T& getLast() const
	{
		if (mcElements == 0)
		{
			throw VectorListEmptyException();
		}

		int index = mBegin + mcElements;
		
		// wrap if necessary
		if (index >= mArraySize)
		{
			index -= mBegin;
		}

		return mpArray[mBegin];
	}

	/**
	* Returns a reference the last element in the array.
	* @throws VectorListEmptyException
	* @return reference to the last element in the array.
	*/
	T& getLast()
	{
		if (mcElements == 0)
		{
			throw VectorListEmptyException();
		}

		int index = mBegin + mcElements;
		
		// wrap if necessary
		if (index >= mArraySize)
		{
			index -= mBegin;
		}

		return mpArray[mBegin];
	}

	/**
	* Checks if the VectorList is empty or not
	* @return true if empty
	*/
	inline bool empty() const
	{
		return mcElements == 0;
	}

	/**
	* "Clears" the array
	*/
	void clear()
	{
		mBegin = 0;
		mcElements = 0;
	}
	
private:
	int mArraySize;
	int mcElements;
	int mArraySizeMax;
	int mBegin;	/**< Where we begin in the VectorList array */

	T* mpArray;	/**< The array that holds all the data */

	static const int INCREMENT_SIZE = 10;
};
}

#endif