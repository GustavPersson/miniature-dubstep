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

#ifndef INPUT_LAYOUT_H
#define INPUT_LAYOUT_H
#include <string>
#include "Vertex.h"

namespace gfx
{
	/**
	 * Storage class for input layouts.
	 */
	class InputLayout
	{
		public:
			/**
			 * Default constructor.
			 * @param pDXInputLayout Pointer to an ID3D10InputLayout.
			 * @param layoutSize Size of the vertex struct.
			 */
			InputLayout(ID3D10InputLayout *pDXInputLayout, unsigned int layoutSize);

			/**
			 * Default copy constructor.
			 */
			InputLayout(const InputLayout &layout);

			/**
			 * Default destructor.
			 */
			virtual ~InputLayout();

			/**
			 * Default assignment operator.
			 */
			const InputLayout &operator=(const InputLayout &layout);

			/**
			 * Sets mID.
			 * @param ID ID to set.
			 */
			inline void setID(int ID){mID = ID;}

			/**
			 * Sets mName.
			 * @param name Name to set.
			 */
			inline void setName(const char *name){mName = name;}

			/**
			 * Sets mpDXInputLayout.
			 * @param pDXInputLayout ID3D10InputLayout to set.
			 */
			inline void setDXInputLayout(ID3D10InputLayout *pDXInputLayout){mpDXInputLayout = pDXInputLayout;}

			/**
			 * Sets mpDXInputLayoutInstanced.
			 * @param pDXInputLayout ID3D10InputLayout to set for instanced rendering.
			 */
			inline void setDXInputLayoutInstanced(ID3D10InputLayout *pDXInputLayout){mpDXInputLayoutInstanced = pDXInputLayout;}

			/**
			 * Sets mLayoutSize.
			 * @param layoutSize Layout size to set.
			 */
			inline void setLayoutSize(unsigned int layoutSize){mLayoutSize = layoutSize;}

			/**
			 * Sets mLayoutSizeStatic.
			 * @param layoutSize Layout size to set.
			 */
			inline void setLayoutSizeStatic(unsigned int layoutSize){mLayoutSizeStatic = layoutSize;}

			/**
			 * Sets mLayoutSizeDynamic.
			 * @param layoutSize Layout size to set.
			 */
			inline void setLayoutSizeDynamic(unsigned int layoutSize){mLayoutSizeDynamic = layoutSize;}

			/**
			 * Retrieves the ID.
			 * @return Returns mID.
			 */
			inline const int &getID() const {return mID;}

			/**
			 * Retrieves the name as a const char pointer.
			 * @return Returns mName as a const char pointer.
			 */
			inline const char *getName() const {return mName.c_str();}

			/**
			 * Retrieves the name as a string.
			 * @return Returns mName.
			 */
			inline const std::string &getNameString() const {return mName;}

			/**
			 * Retrieves a pointer to the ID3D10InputLayout.
			 * @return Returns mpDXInputLayout.
			 */
			inline ID3D10InputLayout *getDXInputLayout(){return mpDXInputLayout;}

			/**
			 * Retrieves a pointer to the ID3D10InputLayout for instanced rendering.
			 * @return Returns mpDXInputLayoutInstanced.
			 */
			inline ID3D10InputLayout *getDXInputLayoutInstanced(){return mpDXInputLayoutInstanced;}

			/**
			 * Retrieves the size of the vertex struct.
			 * @return Returns mLayoutSize.
			 */
			inline const unsigned int &getLayoutSize() const {return mLayoutSize;}

			/**
			 * Retrieves the size of the static part of the vertex struct.
			 * @return Returns mLayoutSizeStatic.
			 */
			inline const unsigned int &getLayoutSizeStatic() const {return mLayoutSizeStatic;}

			/**
			 * Retrieves the size of the dynamic part of the vertex struct.
			 * @return Returns mLayoutSizeDynamic.
			 */
			inline const unsigned int &getLayoutSizeDynamic() const {return mLayoutSizeDynamic;}

		protected:

		private:
			int mID;
			std::string mName;
			ID3D10InputLayout *mpDXInputLayout;
			ID3D10InputLayout *mpDXInputLayoutInstanced;
			unsigned int mLayoutSize;
			unsigned int mLayoutSizeStatic;
			unsigned int mLayoutSizeDynamic;
	};
}

#endif