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
	class IndexOutOfBoundsException : public Exception
	{
	public:
		IndexOutOfBoundsException() : Exception("VectorListIndexOutOfBoundsException: Index is out of bounds in VectorList!", 70001) {}
	};

	/**
	* Thrown when trying to add a new element and the VectorList is full.
	*/
	class FullException : public Exception
	{
	public:
		FullException() : Exception("VectorListFullException: Can't add more elements to VectorList!", 70000) {}
	};

	/**
	* Thrown when trying to do an action on an empty VectorList that needs at least one element.
	*/
	class EmptyException : public Exception
	{
	public:
		EmptyException() : Exception("VectorListEmptyException: Can't operate on a empty VectorList!", 70002) {}
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

	/**
	* Adds a new element
	* @throws FullException
	* @param element the element to add
	*/
	void add(const T& element)
	{
		if (mcElements == mArraySize)
		{
			int newArraySize = 0;
			
			// Throw exception if full
			if (mArraySize == mArraySizeMax)
			{
				throw FullException();
			}
			// If we're close to the maximum size we can't create INCREMENT_SIZE objects
			else if (mArraySize + INCREMENT_SIZE > mArraySizeMax)
			{
				newArraySize = mArraySizeMax;
			}
			// It's safe to allocate + INCREMENT_SIZE objects
			else
			{
				newArraySize = mArraySize + INCREMENT_SIZE;
			}

			// Create the new array and copy the old one
			T* tempArray = myNew T [newArraySize];

			// Because we want to keep the order we use the [] operator
			for (int i = 0; i < mArraySize; i++)
			{
				tempArray[i] = (*this)[i];
			}

			// Delete the old one and set the new one
			SAFE_DELETE_ARRAY(mpArray);
			mpArray = tempArray;

			// Set mBegin to 0 with a new array
			mBegin = 0;
			mArraySize = newArraySize;

		}
		int addPosition = mBegin + mcElements;
		
		// Wrap the position
		if (addPosition >= mArraySize)
		{
			addPosition -= mArraySize;
		}

		// "Add" the new element
		mpArray[addPosition] = element;

		mcElements++;
	}

	/**
	* Returns a reference to the element at the specified index
	* @throws IndexOutOfBoundsException
	* @param index the index
	* @note generates an error message when index is out of bounds
	*/
	T& operator[](int index)
	{
		// Check for out of bounds (with assertion too)
		if (index < 0 || index >= mcElements)
		{
			throw IndexOutOfBoundsException();
		}

		// Wrap the index
		int arrayIndex = mBegin + index;
		if (arrayIndex >= mArraySize)
		{
			arrayIndex -= mArraySize;
		}

		return mpArray[arrayIndex];
	}
	
	/**
	* Returns a const reference to the element at the specified index
	* @throws IndexOutOfBoundsException
	* @param index the index
	* @return reference to the element at the specified index
	*/
	const T& operator[](int index) const
	{
		// Check for out of bounds
		if (index < 0 || index >= mcElements)
		{
			throw IndexOutOfBoundsException();
		}

		// Wrap the index
		int arrayIndex = mBegin + index;
		if (arrayIndex >= mArraySize)
		{
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
	* @throws EmptyException
	*/
	void removeFirst()
	{
		if (mcElements == 0)
		{
			throw EmptyException();
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
	* @throws EmptyException
	*/
	void removeLast()
	{
		if (mcElements == 0)
		{
			throw EmptyException();
		}
		else
		{
			mcElements--;
		}
	}

	/**
	* Removes an element at the specified location. If you want to keep
	* the order of the array be sure to set the keep order to true. Default
	* is false and it will then set the lastPos element on the removed.
	* @param index the index we want to remove an element from
	* @param keepOrder if we want to keep the order of the vectorList, default is false
	* @throws IndexOutOfBoundsException if the index is out of bounds.
	* @throws EmptyException if the array is empty
	*/
	void remove(int index, bool keepOrder = false)
	{
		// If it's the last element just decrease the number of elements
		if (index == (mcElements-1))
		{
			// If index is below zero then mcElements needs to be zero and thus it's empty
			if (index < 0)
			{
				throw EmptyException();
			}
		}
		// If it's the first just increase the start point
		else if (index == 0)
		{
			if (mcElements == 0)
			{
				throw EmptyException();
			}
			mBegin++;

			// Wrap
			if (mBegin >= mArraySize)
			{
				mBegin -= mArraySize;
			}
		}
		else
		{
			if (!keepOrder)
			{
				// Just set the last element to the removed.
				// Indexing automatically checks for out of bounds.
				(*this)[index] = getLast();
			}
			else
			{
				// Check for out of bounds
				if (index < 0 || index >= mcElements)
				{
					// If it's empty throw empty instead
					if (mcElements == 0)
					{
						throw EmptyException();
					}
					else
					{
						throw IndexOutOfBoundsException();
					}
				}

				// Push back all elements that are after the index to keep the order.
				int arrayIndex = mBegin + index + 1;
				int lastIndex = arrayIndex - 1;

				for (int i = index + 1; i < mcElements; i++, arrayIndex++)
				{
					if (arrayIndex >= mArraySize)
					{
						arrayIndex -= mArraySize;
					}

					mpArray[lastIndex] = mpArray[arrayIndex];
					
					lastIndex = arrayIndex;
				}
			}
		}
		
		// Decrease the number of elements, all branches need that
		mcElements--;
	}


	/**
	* Returns a const reference to the first element in the array.
	* @throws EmptyException
	* @return const reference to the first element in the array.
	*/
	inline const T& getFirst() const
	{
		if (mcElements == 0)
		{
			throw EmptyException();
		}

		return mpArray[mBegin];
	}

	/**
	* Returns a reference the first element in the array.
	* @throws EmptyException
	* @return reference to the first element in the array.
	*/
	inline T& getFirst()
	{
		if (mcElements == 0)
		{
			throw EmptyException();
		}

		return mpArray[mBegin];
	}

	/**
	* Returns a const reference the last element in the array.
	* @throws EmptyException
	* @return const reference to the last element in the array.
	*/
	const T& getLast() const
	{
		if (mcElements == 0)
		{
			throw EmptyException();
		}

		int index = mBegin + mcElements -1;
		
		// wrap if necessary
		if (index >= mArraySize)
		{
			index -= mBegin;
		}

		return mpArray[index];
	}

	/**
	* Returns a reference the last element in the array.
	* @throws EmptyException
	* @return reference to the last element in the array.
	*/
	T& getLast()
	{
		if (mcElements == 0)
		{
			throw EmptyException();
		}

		int index = mBegin + mcElements -1;
		
		// wrap if necessary
		if (index >= mArraySize)
		{
			index -= mBegin;
		}

		return mpArray[index];
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