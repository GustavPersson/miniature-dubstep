/**
 * @file
 * @author Victor Lundberg <lundberg.victor@gmail.com>
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

#include "InputLayout.h"

using namespace gfx;

InputLayout::InputLayout(ID3D10InputLayout *pDXInputLayout, unsigned int layoutSize) : mName("Unknown")
{
	mID = -1;
	mpDXInputLayout = pDXInputLayout;
	mpDXInputLayoutInstanced = NULL;
	mLayoutSize = layoutSize;
	mLayoutSizeStatic = 0;
	mLayoutSizeDynamic = 0;
}

InputLayout::InputLayout(const InputLayout &layout) : mName(layout.mName)
{
	mID = layout.mID;
	mpDXInputLayout = layout.mpDXInputLayout;
	mpDXInputLayoutInstanced = layout.mpDXInputLayoutInstanced;
	mLayoutSize = layout.mLayoutSize;
	mLayoutSizeStatic = layout.mLayoutSizeStatic;
	mLayoutSizeDynamic = layout.mLayoutSizeDynamic;
}

InputLayout::~InputLayout()
{
	if(mpDXInputLayout)
	{
		mpDXInputLayout->Release();
		mpDXInputLayout = NULL;
	}
	if(mpDXInputLayoutInstanced)
	{
		mpDXInputLayoutInstanced->Release();
		mpDXInputLayout = NULL;
	}
}

const InputLayout &InputLayout::operator=(const InputLayout &layout)
{
	if(this != &layout)
	{
		mID = layout.mID;
		mName = layout.mName;
		mpDXInputLayout = layout.mpDXInputLayout;
		mpDXInputLayoutInstanced = layout.mpDXInputLayoutInstanced;
		mLayoutSize = layout.mLayoutSize;
		mLayoutSizeStatic = layout.mLayoutSizeStatic;
		mLayoutSizeDynamic = layout.mLayoutSizeDynamic;
	}
	return *this;
}