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

#ifndef INSTANCE_MESH_H
#define INSTANCE_MESH_H
#include "InstanceSubset.h"

namespace gfx
{
	/**
	 * Class for rendering instanced meshes.
	 */
	class InstanceMesh : public InstanceObject
	{
		public:
			/**
			 * Default constructor.
			 * @param pDevice Pointer to the ID3D10Device.
			 * @param pEffect Pointer to an Effect.
			 */
			InstanceMesh(ID3D10Device *pDevice, Effect *pEffect);

			/**
			 * Default copy constructor.
			 */
			InstanceMesh(const InstanceMesh &mesh);

			/**
			 * Default destructor.
			 */
			virtual ~InstanceMesh();

			/**
			 * Default assignment operator.
			 */
			const InstanceMesh &operator=(const InstanceMesh &mesh);

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
			 * Creates a new instance to this object.
			 * @return Returns a pointer to the newly created instance.
			 */
			virtual InstanceObject *createInstance();

			/**
			 * Removes an instance to this object.
			 * @param pInstance Pointer to the instance to remove.
			 */
			virtual void removeInstance(InstanceObject *pInstance);

			/**
			 * Removes an instance to this object based on its ID.
			 * @param instanceID ID for the instance to remove.
			 */
			virtual void removeInstance(int instanceID);

			/**
			 * Removes an instance to this object based on its name.
			 * @param instanceName Name for the instance to remove.
			 */
			virtual void removeInstance(const char *instanceName);

			/**
			 * Removes all instances to this object.
			 */
			virtual void removeAllInstances();

			/**
			 * Sets the maximum number of possible instances.
			 *  Updates the size of the dynamic vertex buffer.
			 * @param max New value for mInstanceCountMax.
			 */
			virtual void setInstanceCountMax(unsigned int max);

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
			virtual void addSubset(InstanceSubset *pSubset);

			/**
			 * Retrieves a subset based on its location in mSubsetList.
			 * @param subsetIndex Index in the mSubsetList.
			 * @return Returns a pointer to the subset or NULL.
			 */
			virtual InstanceSubset *getSubsetByIndex(unsigned int subsetIndex);

			/**
			 * Retrieves a subset based on its ID.
			 * @param subsetID ID of the subset.
			 * @return Returns a pointer to the subset or NULL.
			 */
			virtual InstanceSubset *getSubset(int subsetID);

			/**
			 * Retrieves a subset based on its name.
			 * @param subsetName Name of the subset.
			 * @return Returns a pointer to the subset or NULL.
			 */
			virtual InstanceSubset *getSubset(const char *subsetName);

			/**
			 * Removes a Subset from the mSubsetList.
			 * @param pSubset Pointer to the Subset to remove.
			 */
			virtual void removeSubset(InstanceSubset *pSubset);

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
			 * Sets the object ID.
			 * @param ID New ID.
			 */
			virtual void setID(int ID);

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
			 * Sets the shadow flag (if shadows should be rendered for the given object)
			 * @param shadow New value for mFlagShadows
			 */
			virtual void setFlagShadow(bool shadow);

			/**
			 * Sets which renderpass to render
			 * @param index Index of render pass (where -1 represents rendering all passes)
			 */
			virtual void setEffectPassIndex(int index);

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
			 * Sets the instance base flag (if the object is a base or actual instance).
			 *  Also changes the mFlagReleaseBuffers. Pure instances should never release any buffers.
			 * @param instanceBase New value for mFlagInstanceBase.
			 */
			virtual void setFlagInstanceBase(bool instanceBase);

			/**
			 * Sets the update from base flag (if all instances should be updated at the same time
			 *  from the base. This saves huge amount of time at the expense of some CPU power and should
			 *  be used for all moveable instanced objects).
			 * Also updates all child instances.
			 * @param updateBase New value for mFlagUpdateFromBase.
			 */
			virtual void setFlagUpdateFromBase(bool updateBase);

			/**
			 * Sets the update absolute flag (if the base mesh should update all its subsets
			 *  to the same position, scale and rotation. Set this to false in order to only
			 *  update position and scale).
			 * @param updateAbsolute New value for mFlagUpdateAbsolute.
			 */
			virtual void setFlagUpdateAbsolute(bool updateAbsolute);

			virtual const std::vector<InstanceSubset *> &getSubsetList() const {return mSubsetList;}

		protected:
			/**
			 * Updates the dynamic vertex buffer.
			 *  Called from every update().
			 * @return Returns true if any updates have been made.
			 */
			virtual bool updateVertexBufferDynamic();

			/**
			 * Updates the dynamic vertex buffer from the instance base.
			 *  The base updates the buffer with a single call which improves
			 *  performance significantly if many instances move each frame.
			 *  Called from every update().
			 * @return Returns true if any updates have been made.
			 */
			virtual bool updateVertexBufferDynamicFromBase();

			/**
			 * Creates the static vertex buffer.
			 */
			virtual void createVertexBufferStatic();

			/**
			 * Creates the dynamic vertex buffer.
			 */
			virtual void createVertexBufferDynamic();

			/**
			 * Creates an empty dynamic vertex buffer.
			 */
			virtual void createEmptyVertexBufferDynamic();

			/**
			 * Creates the index buffer.
			 */
			virtual void createIndexBuffer();

			/**
			 * Creates the index buffer with adjacent information.
			 */
			virtual void createIndexBufferAdjacent();

		private:
			std::vector<InstanceSubset *> mSubsetList;
			bool mFlagUpdateAbsolute;
	};
}

#endif