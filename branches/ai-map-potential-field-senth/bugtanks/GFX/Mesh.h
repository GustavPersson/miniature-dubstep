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

#ifndef MESH_H
#define MESH_H
#include <vector>
#include "Subset.h"

namespace gfx
{
	/**
	 * Class containing methods for creating and rendering subsets.
	 */
	class Mesh : public TransformObject
	{
		public:
			/**
			 * Default constructor.
			 * @param pDevice Pointer to the ID3D10Device.
			 * @param pEffect Pointer to an Effect.
			 */
			Mesh(ID3D10Device *pDevice, Effect *pEffect);

			/**
			 * Default copy constructor.
			 */
			Mesh(const Mesh &mesh);

			/**
			 * Default destructor.
			 */
			virtual ~Mesh();

			/**
			 * Default assignment operator.
			 */
			const Mesh &operator=(const Mesh &mesh);

			/**
			 * Clones the object.
			 */
			virtual RenderObject *clone();

			/**
			 * Updates the object.
			 * Functionality varies depending on object type.
			 */
			virtual void update(float deltaTime);

			/**
			 * Renders the object.
			 * Method varies depending on object type.
			 */
			virtual bool render();

			/**
			 * Updates the position, scale and rotation of all subsets
			 * to be the same as the core mesh.
			 */
			virtual void updateTransformAbsolute();

			/**
			 * Updates the position, scale and rotation of all subsets
			 * based on their relative offset to the core mesh.
			 */
			virtual void updateTransformRelative();

			/**
			 * Adds a Subset to the mSubsetList.
			 * @param pSubset Pointer to the Subset to add.
			 */
			virtual void addSubset(Subset *pSubset);

			/**
			 * Retrieves a subset based on its location in mSubsetList.
			 * @param subsetIndex Index in the mSubsetList.
			 * @return Returns a pointer to the subset or NULL.
			 */
			virtual Subset *getSubsetByIndex(unsigned int subsetIndex);

			/**
			 * Retrieves a subset based on its ID.
			 * @param subsetID ID of the subset.
			 * @return Returns a pointer to the subset or NULL.
			 */
			virtual Subset *getSubset(int subsetID);

			/**
			 * Retrieves a subset based on its name.
			 * @param subsetName Name of the subset.
			 * @return Returns a pointer to the subset or NULL.
			 */
			virtual Subset *getSubset(const char *subsetName);

			/**
			 * Removes a Subset from the mSubsetList.
			 * @param pSubset Pointer to the Subset to remove.
			 */
			virtual void removeSubset(Subset *pSubset);

			/**
			 * Removes a Subset based on its ID.
			 * @param subsetID ID of the Subset.
			 */
			virtual void removeSubset(int subsetID);

			/**
			 * Removes a Subset based on its name.
			 * @param subsetName Name of the Subset.
			 */
			virtual void removeSubset(const char *subsetName);

			/**
			 * Removes all Subsets.
			 */
			virtual void removeAllSubsets();

			/**
			 * Sets the color.
			 * @param color New D3DXCOLOR.
			 */
			virtual void setColor(const D3DXCOLOR &color);

			/**
			 * Sets the texture array index.
			 *  Note: This is the index of the texture array to be used.
			 * @param arrayIndex New array index.
			 */
			virtual void setTextureArrayIndex(int arrayIndex);

			/**
			 * Sets the texture ID.
			 *  Note: This is the texture index in the Texture2DArray.
			 * @param textureID New texture ID.
			 */
			virtual void setTextureID(int textureID);

			/**
			 * Sets the render flag (if the object should be rendered).
			 * @param render New value for mFlagRender.
			 */
			virtual void setFlagRender(bool render);

			/**
			 * Sets the effect tech index.
			 * @param techIndex New value for mEffectTechIndex.
			 */
			virtual void setEffectTechIndex(unsigned int techIndex);

			/**
			 * Sets the current actor pointer.
			 * @param pActor Pointer to the new actor.
			 */
			virtual void setActor(void *pActor);

			/**
			 * Sets the update absolute flag (if the base mesh should update all its subsets
			 *  to the same position, scale and rotation. Set this to false in order to only
			 *  update position and scale).
			 * @param updateAbsolute New value for mFlagUpdateAbsolute.
			 */
			virtual void setFlagUpdateAbsolute(bool updateAbsolute);

		protected:
			/**
			 * Copy all subsets from a mesh.
			 * @param sourceMesh Mesh to copy from.
			 */
			void copySubsetsFrom(const Mesh &sourceMesh);

		private:
			std::vector<Subset *> mSubsetList;
			bool mFlagUpdateAbsolute;
	};
}

#endif