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

#ifndef FRAME_H
#define FRAME_H

#include <d3dx10.h>
#include <vector>

namespace gfx
{
	/**
	 * A frame in a animation.
	 */
	class Frame
	{
		public:
			/**
			 * Constructor.
			 */
			Frame();

			/**
			 * Returns a matrix.
			 * @param index . The index of the matrix.
			 */
			D3DXMATRIX getMatrix(unsigned int index) const;

			/**
			 * Adds a matrix.
			 */
			void addMatrix(D3DXMATRIX matrix);

			/**
			 * Returns the number of matrices.
			 */
			unsigned int getSize() const;

		private:
			std::vector<D3DXMATRIX> mMatrices;
	};
}


#endif