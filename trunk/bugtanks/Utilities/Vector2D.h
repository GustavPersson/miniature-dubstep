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

#ifndef __VECTOR2_D_H__
#define __VECTOR2_D_H__

#include "Macros.h"
#include "Exception.h"

namespace utilities
{

/**
* A template 2D vector with shifting functionality
*/
template <typename T>
class Vector2D
{
public:
	/**
	* Throw when trying to derefence an index which is out of bounds
	*/
	class IndexOutOfBoundsException : public Exception
	{
	public:
		IndexOutOfBoundsException() : Exception("Vector2DIndexOutOfBoundsException: Index is out of bounds!", 70003) {}
	};

	/**
	* Constructor.
	* @param width the width of the vector
	* @param height the height of the vector
	* @param defaultValue the default value we want to use in the vector
	*/
	Vector2D(int width, int height, const T& defaultValue = T())
		: mWidth(width), mHeight(height), mDefaultValue(defaultValue)
	{
		mWidthOffset = 0;
		mHeightOffset = 0;

		// Calculate the size
		mSize = mWidth * mHeight;

		// Create the array
		mpArray = myNew T [mWidth * mHeight];

		// Set the default value
		for (int i = 0; i < mSize; i++)
		{
			mpArray[i] = mDefaultValue;
		}
	}

	/**
	* Destructor
	*/
	~Vector2D()
	{
		SAFE_DELETE_ARRAY(mpArray);
	}

	/**
	* Return a reference to the element at the specified location
	* @param x the x-coordinate
	* @param y the y-coordinate
	* @return reference to the element at the specified location
	* @throws Vector2DIndexOutOfBoundsException
	*/
	T& get(int x, int y)
	{
		if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
		{
			throw IndexOutOfBoundsException();
		}

		int actualX = x + mWidthOffset;
		int actualY = y + mHeightOffset;

		// wrap the actual coordinates
		if (actualX >= mWidth)
		{
			actualX -= mWidth;
		}
		if (actualY >= mHeight)
		{
			actualY -= mHeight;
		}

		return mpArray[actualY*mWidth + actualX];
	}

	/**
	* Return a const reference to the element at the specified location
	* @param x the x-coordinate
	* @param y the y-coordinate
	* @return reference to the element at the specified location
	* @throws Vector2DIndexOutOfBoundsException
	*/
	inline const T& get(int x, int y) const
	{
		return get(x, y);
	}

	/**
	* Shifts the whole array to the left. If we don't wrap the vector's elements
	* are set to the default value.
	* @param wrap if we should wrap the values.
	*/
	void shiftLeft(bool wrap)
	{
		mWidthOffset++;
		if (mWidthOffset >= mWidth)
		{
			mWidthOffset -= mWidth;
		}

		// If we shouldn't wrap we have to set the "out of bound" to zero
		if (!wrap)
		{
			int x = mWidthOffset - 1;

			// wrap x coordinate
			if (x < 0)
			{
				x += mWidth;
			}

			// Iterate through all the rows
			for (int y = 0; y < mHeight; y++)
			{
				mpArray[y*mWidth + x] = mDefaultValue;
			}
		}		
	}

	/**
	* Shifts the whole array to the right. If we don't wrap the vector's elements
	* are set to the default value.
	* @param wrap if we should wrap the values.
	*/
	void shiftRight(bool wrap)
	{
		mWidthOffset--;
		if (mWidthOffset < 0)
		{
			mWidthOffset += mWidth;
		}

		if (!wrap)
		{
			// Iterate through all the rows
			for (int y = 0; y < mHeight; y++)
			{
				mpArray[y*mWidth + mWidthOffset] = mDefaultValue;
			}
		}
	}

	/**
	* Shifts the whole array upwards. If we don't wrap the vector's elements
	* are set to the default value.
	* @param wrap if we should wrap the values.
	*/
	void shiftUp(bool wrap)
	{
		mHeightOffset++;
		if (mHeightOffset >= mHeight)
		{
			mHeightOffset -= mHeight;
		}

		if (!wrap)
		{
			int y = mHeightOffset - 1;

			// wrap y coordinate
			if (y < 0)
			{
				y += mHeight;
			}

			// Set the right offset
			int offset = y * mWidth;

			// Iterate through all the columns
			for (int x = 0; x < mWidth; x++)
			{
				mpArray[offset + x] = mDefaultValue;
			}
		}
	}

	/**
	* Shifts the whole array downwards. If we don't wrap the vector's elements
	* are set to the default value.
	* @param wrap if we should wrap the values.
	*/
	void shiftDown(bool wrap)
	{
		mHeightOffset--;
		if (mHeightOffset < 0)
		{
			mHeightOffset += mHeight;
		}

		if (!wrap)
		{
			// Set the right offset
			int offset = mHeightOffset * mWidth;

			// Iterate through all the columns
			for (int x = 0; x < mWidth; x++)
			{
				mpArray[offset + x] = mDefaultValue;
			}
		}
	}

private:
	T	mDefaultValue;	/**< The default value that all elements in the vector will have */
	T*	mpArray;		/**< The 2D array we're simulating */
	int mHeight;		/**< The height of the array */
	int mWidth;			/**< The width of the array */
	int mHeightOffset;	/**< The height offset that is the actual start y-position */
	int mWidthOffset;	/**< The width offset that is the actual start x-position */
	int mSize;			/**< The size/elements the vector contains */
};
}

#endif