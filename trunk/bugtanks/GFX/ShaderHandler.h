/**
 * @file
 * @author Martin Jönsson <snowman.mrgames@gmail.com>, Victor Lundberg <lundberg.victor@gmail.com>
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

#ifndef SHADER_HANDLER_H
#define SHADER_HANDLER_H
#include <vector>
#include "Effect.h"

namespace gfx
{
	/**
	 * Class for storing and creating Effect instances.
	 */
	class ShaderHandler
	{
		public:
			/**
			 * Default constructor.
			 */
			ShaderHandler();

			/**
			 * Default destructor.
			 */
			virtual ~ShaderHandler();

			/**
			 * Initialises the shader handler.
			 * @param pDevice Pointer to the ID3D10Device.
			 */
			void init(ID3D10Device *pDevice);

			/**
			 * Retrieves an effect based on its index.
			 * @param index Index of the effect.
			 * @return Returns a pointer to the effect.
			 */
			Effect *getEffect(unsigned int index);

			/**
			 * Retrieves an effect based on its index.
			 * @param name Name of the effect.
			 * @return Returns a pointer to the effect.
			 */
			Effect *getEffect(const char *name);

			/**
			 * Adds (creates) a new shader.
			 * @param fileName Relative/absolute path of the file.
			 * @param effectName Name of the new shader.
			 */
			HRESULT addEffect(const char *fileName, const char *effectName);

			/**
			 * Removes (deletes) all effects.
			 */
			void removeAllEffects();

		protected:
			/**
			 * Default copy constructor.
			 *  Does not copy effect array.
			 */
			ShaderHandler(const ShaderHandler &handler);

			/**
			 * Default assignment operator.
			 *  Does not copy effect array.
			 */
			const ShaderHandler &operator=(const ShaderHandler &handler);

		private:
			ID3D10Device *mpDevice;
			std::vector<Effect *> mEffectList;
	};
}

#endif