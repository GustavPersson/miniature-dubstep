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

#ifndef INPUT_LAYOUT_HANDLER_H
#define INPUT_LAYOUT_HANDLER_H
#include <vector>
#include "InputLayout.h"

namespace gfx
{
	/**
	 * Handler for input layouts.
	 */
	class InputLayoutHandler
	{
		public:
			/**
			 * Default constructor.
			 */
			InputLayoutHandler(ID3D10Device *pDevice);

			/**
			 * Default destructor.
			 */
			virtual ~InputLayoutHandler();

			/**
			 * Retrieves an InputLayout based on its ID.
			 * @param techIndex ID/technique index of the InputLayout.
			 * @return Returns a pointer to the InputLayout.
			 */
			InputLayout *getInputLayout(unsigned int techIndex);

			/**
			 * Retrieves an InputLayout based on its name.
			 * @param name Name of the InputLayout.
			 * @return Returns a pointer to the InputLayout.
			 */
			InputLayout *getInputLayout(const char *name);

			/**
			 * Creates a new InputLayout.
			 * @param pEffect Pointer to the ID3D10Effect.
			 * @param techIndex Index of the shader technique.
			 * @param vertexType Type of the vertex structure.
			 * @return Returns S_OK or E_FAIL.
			 */
			HRESULT createInputLayout(ID3D10Effect *pEffect, unsigned int techIndex, VertexTypes::Types vertexType);

			/**
			 * Destroys all input layouts.
			 */
			void clearInputLayouts();

		protected:
			/**
			 * Default copy constructor.
			 */
			InputLayoutHandler(const InputLayoutHandler &handler);

			/**
			 * Default assignment operator.
			 */
			const InputLayoutHandler &operator=(const InputLayoutHandler &handler);

		private:
			std::vector<InputLayout *> mInputLayoutList;
			ID3D10Device *mpDevice;
	};
}

#endif