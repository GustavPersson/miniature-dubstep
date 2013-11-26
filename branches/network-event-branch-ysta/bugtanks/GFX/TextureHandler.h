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

#ifndef TEXTURE_HANDLER_H
#define TEXTURE_HANDLER_H

#include <string>
#include <vector>
#include <d3dx10.h>
#include "../Utilities/Macros.h"
#include "Texture.h"

namespace gfx
{
	/**
	 * Loads and stores textures.
	 * Can create texture arrays based on stored textures.
	 */
	class TextureHandler
	{
		public:
			/**
		     * Default constructor.
			 * @param pDevice. The ID3D10Device that are used.
			 */
			TextureHandler(ID3D10Device *pDevice);

			/**
			 * Destructor.
			 */
			virtual ~TextureHandler();

			/**
			 * Adds a texture.
			 * @param fileName . The file name for the texture.
			 * @param textureName . The name of the texture.
			 */
			void addTexture(std::string fileName,std::string textureName);

			/**
			 * Returns a texture. Returns NULL if the texture dosn't exist.
			 * @param textureName . The name of the texture.
			 */
			ID3D10ShaderResourceView * getTexture(std::string textureName) const;

			/**
			 * Returns a texture array. Write the names of the texturs separated by ",".
			 * For example "texture1,texture2,texture3".
			 * @param textureNames . The names of the textures. 
			 */
			ID3D10ShaderResourceView * getTextureArray(std::string textureNames) const;

		protected:

			/**
			 * Copy constructor.
			 */
			TextureHandler(const TextureHandler &handler);

			/**
			 * Assignment operator.
			 */
			const TextureHandler &operator=(const TextureHandler &handler);

		private:
			std::vector<Texture*> mTextures;
			ID3D10Device *mpDevice;
	};
}

#endif