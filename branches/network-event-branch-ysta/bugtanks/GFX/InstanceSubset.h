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

#ifndef INSTANCE_SUBSET_H
#define INSTANCE_SUBSET_H
#include "InstanceObject.h"

namespace gfx
{
	/**
	 * Class for instanced mesh subsets.
	 */
	class InstanceSubset : public InstanceObject
	{
		public:
			/**
			 * Default constructor.
			 * @param pDevice Pointer to the ID3D10Device.
			 * @param pEffect Pointer to an Effect.
			 */
			InstanceSubset(ID3D10Device *pDevice, Effect *pEffect);

			/**
			 * Default copy constructor.
			 */
			InstanceSubset(const InstanceSubset &subset);

			/**
			 * Default destructor.
			 */
			virtual ~InstanceSubset();

			/**
			 * Default assignment operator.
			 */
			const InstanceSubset &operator=(const InstanceSubset &subset);

			/**
			 * Clones the object.
			 */
			virtual RenderObject *clone();

			/**
			 * Updates the object.
			 * Functionality varies depending on object type.
			 */
			virtual void update();

			/**
			 * Creates a new instance to this object.
			 * @return Returns a pointer to the newly created instance.
			 */
			virtual InstanceObject *createInstance();

			/**
			 * Sets all vertex and index buffers as well as the vertex/index count.
			 * @param vertexCount New vertex count.
			 * @param pVertexBufferStatic New static vertex buffer.
			 * @param indexCount New index count.
			 * @param pIndexBuffer New index buffer.
			 */
			virtual void loadBuffers(unsigned int vertexCount, ID3D10Buffer *pVertexBufferStatic,
									 unsigned int indexCount, ID3D10Buffer *pIndexBuffer);

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
			 * Creates the index buffer.
			 */
			virtual void createIndexBuffer();

		private:
	};
}

#endif