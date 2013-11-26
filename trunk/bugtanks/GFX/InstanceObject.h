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

#ifndef INSTANCE_OBJECT_H
#define INSTANCE_OBJECT_H
#include <vector>
#include "BufferObject.h"

namespace gfx
{
	/**
	 * Abstract base class for object instances.
	 */
	class InstanceObject : public BufferObject
	{
		public:
			/**
			 * Default constructor.
			 * @param pDevice Pointer to the ID3D10Device.
			 * @param pEffect Pointer to an Effect.
			 */
			InstanceObject(ID3D10Device *pDevice, Effect *pEffect);

			/**
			 * Default copy constructor.
			 */
			InstanceObject(const InstanceObject &object);

			/**
			 * Default destructor.
			 */
			virtual ~InstanceObject();

			/**
			 * Default assignment operator.
			 */
			const InstanceObject &operator=(const InstanceObject &object);

			/**
			 * Clones the object.
			 */
			virtual RenderObject *clone() = 0;

			/**
			 * Updates the object.
			 * Functionality varies depending on object type.
			 */
			virtual void update(float deltaTime) = 0;

			/**
			 * Renders the object.
			 * Method varies depending on object type.
			 */
			virtual bool render();

			/**
			 * Creates a new instance to this object.
			 * @return Returns a pointer to the newly created instance.
			 */
			virtual InstanceObject *createInstance() = 0;

			/**
			 * Retrieves an instance based on its ID.
			 * @param instanceID ID of the instance to retrieve.
			 * @return Returns a pointer to the instance.
			 */
			virtual InstanceObject *getInstance(int instanceID);

			/**
			 * Retrieves an instance based on its ID.
			 * @param instanceName Name of the instance to retrieve.
			 * @return Returns a pointer to the instance.
			 */
			virtual InstanceObject *getInstance(const char *instanceName);

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
			 * Sets the shadow flag (if shadows should be rendered for the given object)
			 * @param shadow New value for mFlagShadows
			 */
			virtual void setFlagShadow(bool shadow);

			/**
			 * Sets which render pass to use
			 * @param index Index of render pass (where -1 represents rendering all passes)
			 */
			virtual void setEffectPassIndex(int index);

			/**
			 * Sets the culled flag (if the instance has been culled and shouldn't be rendered).
			 * @param culled New value for mFlagCulled.
			 */
			inline virtual void setFlagCulled(bool culled){mFlagCulled = culled;}

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
			 * Sets the maximum number of possible instances.
			 *  Updates the size of the dynamic vertex buffer.
			 * @param max New value for mInstanceCountMax.
			 */
			virtual void setInstanceCountMax(unsigned int max);

			/**
			 * Sets the object's instance index.
			 * @param index New value for mInstanceIndex.
			 */
			inline void setInstanceIndex(unsigned int index){mInstanceIndex = index;}

			/**
			 * Retrieves the culled flag.
			 * @return Returns mFlagCulled.
			 */
			inline bool getFlagCulled() const {return mFlagCulled;}

			/**
			 * Retrieves the instance base flag.
			 * @return Returns mFlagInstanceBase.
			 */
			inline bool getFlagInstanceBase() const {return mFlagInstanceBase;}

			/**
			 * Retrieves the current number of instances.
			 * @return Returns mInstanceCount.
			 */
			inline unsigned int getInstanceCount() const {return mInstanceCount;}

			/**
			 * Retrieves the maximum number of possible instances.
			 * @return Returns mInstanceCountMax.
			 */
			inline unsigned int getInstanceCountMax() const {return mInstanceCountMax;}

			/**
			 * Retrieves the object's instance index.
			 * @return Returns mInstanceIndex.
			 */
			inline unsigned int getInstanceIndex() const {return mInstanceIndex;}

			/**
			 * Retrieves a const reference to the instance list.
			 * @return Returns mInstanceList as a const reference.
			 */
			inline const std::vector<InstanceObject *> &getInstanceList() const {return mInstanceList;}

		protected:
			/**
			 * Updates the vertex buffer.
			 *  Called from every update().
			 * @return Returns true if any updates have been made.
			 */
			virtual bool updateVertexBufferDynamic() = 0;

			/**
			 * Updates the dynamic vertex buffer from the instance base.
			 *  The base updates the buffer with a single call which improves
			 *  performance significantly if many instances move each frame.
			 *  Called from every update().
			 * @return Returns true if any updates have been made.
			 */
			virtual bool updateVertexBufferDynamicFromBase() = 0;

			/**
			 * Creates the static vertex buffer.
			 */
			virtual void createVertexBufferStatic() = 0;

			/**
			 * Creates the dynamic vertex buffer.
			 */
			virtual void createVertexBufferDynamic() = 0;

			/**
			 * Creates an empty dynamic vertex buffer.
			 */
			virtual void createEmptyVertexBufferDynamic();

			/**
			 * Creates the index buffer.
			 */
			virtual void createIndexBuffer() = 0;

			/**
			 * Creates the index buffer with adjacent information.
			 */
			virtual void createIndexBufferAdjacent() = 0;

			/**
			 * Adds an InstanceObject to the list.
			 * @param pInstance Pointer to the InstanceObject to add.
			 */
			virtual void addInstance(InstanceObject *pInstance);

			/**
			 * Adds an InstanceObject to the list, sorted by its z coordinate.
			 * @param pInstance Pointer to the InstanceObject to add.
			 */
			virtual void addInstanceSorted(InstanceObject *pInstance);

			/**
			 * Resets all instances' mInstanceIndex to their index in
			 *  the mInstanceList.
			 */
			virtual void updateInstanceIndex();

			/**
			 * Links the dynamic vertex buffer to an instance.
			 */
			virtual void linkVertexBufferDynamic(InstanceObject *pInstance);

			/**
			 * Links the dynamic vertex buffer to all instances.
			 */
			virtual void linkVertexBufferDynamic();

			/**
			 * Links the static vertex buffer to an instance.
			 *  Note: should not be used except for special object types!
			 */
			virtual void linkVertexBufferStatic(InstanceObject *pInstance);

			/**
			 * Sets the current number of renderable instances.
			 * @param renderCount New value for mRenderCount.
			 */
			inline void setRenderCount(unsigned int renderCount){mRenderCount = renderCount;}

			/**
			 * Retrieves the current number of renderable instances.
			 * @return Returns mRenderCount.
			 */
			inline unsigned int getRenderCount() const {return mRenderCount;}

		private:
			std::vector<InstanceObject *> mInstanceList;
			bool mFlagInstanceBase;
			bool mFlagUpdateFromBase;
			bool mFlagCulled;
			unsigned int mInstanceCount;
			unsigned int mInstanceCountMax;
			unsigned int mRenderCount;
			unsigned int mInstanceIndex;
	};
}

#endif