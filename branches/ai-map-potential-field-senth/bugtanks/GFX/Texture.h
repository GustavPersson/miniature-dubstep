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

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <d3dx10.h>
#include "../Utilities/Macros.h"

namespace gfx
{
	/**
	 * A texture.
	 */
	class Texture
	{
		public:

			/**
			 * Default constructor.
			 * @param pGPUTexture . The ID3D10ShaderResourceView for the texture.
			 * @param pCPUTexture . The ID3D10Texture2D for the texture. Used to create texture arrays.
			 * @param name . The name of the texture.
			 */
			Texture(ID3D10ShaderResourceView* pGPUTexture, ID3D10Texture2D* pCPUTexture, std::string name);

			/**
			 * Destructor.
			 */
			virtual ~Texture();

			/**
			 * Returns mpGPUTexture.
			 */
			ID3D10ShaderResourceView* getGPUTexture();

			/**
			 * Returns mCPUTexture.
			 */
			ID3D10Texture2D* getCPUTexture();

			/**
			 * Returns mName.
			 */
			std::string getName();

		private:
			ID3D10ShaderResourceView* mpGPUTexture;
			ID3D10Texture2D* mpCPUTexture;
			std::string mName;
	};
}

#endif