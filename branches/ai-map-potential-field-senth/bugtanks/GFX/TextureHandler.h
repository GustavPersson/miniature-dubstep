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
#include <dxerr.h>
#include "../Utilities/Macros.h"
#include "Texture.h"

#pragma comment(lib, "dxerr.lib")

namespace gfx
{
	/**
	 * Loads and stores textures.
	 * Can create texture arrays based on stored textures.
	 */
	class TextureHandler
	{
		public:

			enum SizeTypes
			{
				SizeType_128,
				SizeType_256,
				SizeType_512,
				SizeType_1024
			};

			/**
		     * Default constructor.
			 * @param pDevice . The ID3D10Device that are used.
			 */
			TextureHandler(ID3D10Device *pDevice);

			/**
			 * Destructor.
			 */
			virtual ~TextureHandler();

			/**
			 * Retrieves the array index and texture ID of a loaded texture.
			 * @param textureName Name of the texture to look for.
			 * @param arrayIndex Outgoing reference to the array index.
			 * @param textureID Outgoing reference to the texture ID.
			 * @return Returns true if the texture was found.
			 */
			bool getTextureIndex(const char *textureName, int &arrayIndex, int &textureID);

			/**
			 * Retrieves the array index and texture ID of a loaded texture.
			 * @param textureName Name of the texture to look for.
			 * @param arrayIndex Outgoing reference to the array index.
			 * @param textureID Outgoing reference to the texture ID.
			 * @return Returns true if the texture was found.
			 */
			bool getTextureIndex(const std::string &textureName, int &arrayIndex, int &textureID);

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

			/**
			 * Returns a texture array. Write the names of the texturs separated by ",".
			 * For example "texture1,texture2,texture3".
			 * Allows textures of different size.
			 * @param textureNames . The names of the textures. 
			 */
			ID3D10ShaderResourceView * getTextureArraySize(std::string textureNames) const;

			ID3D10ShaderResourceView * getTextureArraySize(SizeTypes sizeType) const;

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
			byte percent(unsigned int size, SizeTypes textureSize);

			std::vector<Texture*> mTextures;
			std::vector<Texture*> mTextures128;
			std::vector<Texture*> mTextures256;
			std::vector<Texture*> mTextures512;
			std::vector<Texture*> mTextures1024;
			ID3D10Device *mpDevice;
	};
}

#endif