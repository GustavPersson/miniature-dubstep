/**
 * @file
 * @author Carl Nordlander <cand05@student.bth.se>
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

#include "Frame.h"

using namespace gfx;

Frame::Frame()
{
	mMatrices = std::vector<D3DXMATRIX>(); 
}

D3DXMATRIX Frame::getMatrix(unsigned int index) const
{
	return mMatrices[index];
}

void Frame::addMatrix(D3DXMATRIX matrix)
{
	mMatrices.push_back(matrix);
}

unsigned int Frame::getSize() const
{
	return mMatrices.size();
}