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

#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H
#include "TransformObject.h"

namespace gfx
{
	/**
	 * Abstract base class for non instanced geometry objects.
	 */
	class BufferObject : public TransformObject
	{
		public:
			/**
			 * Default constructor.
			 * @param pDevice Pointer to the ID3D10Device.
			 * @param pEffect Pointer to an Effect.
			 */
			BufferObject(ID3D10Device *pDevice, Effect *pEffect);

			/**
			 * Default copy constructor.
			 */
			BufferObject(const BufferObject &object);

			/**
			 * Default destructor.
			 */
			virtual ~BufferObject();

			/**
			 * Default assignment operator.
			 */
			const BufferObject &operator=(const BufferObject &object);

			/**
			 * Clones the object.
			 */
			virtual RenderObject *clone();

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
			 * Releases and recreates the vertex and index buffers.
			 * Should only be used for Shapes as Meshes are unable to
			 * recreate the static vertex buffers.
			 */
			virtual void recreateBuffers();

			/**
			 * Retrieves the number of vertices in the vertex buffer.
			 * @return Returns mVertexCount.
			 */
			inline unsigned int getVertexCount() const {return mVertexCount;}

			/**
			 * Retrieves the number of indicies in the index buffer.
			 * @return Returns mIndexCount.
			 */
			inline unsigned int getIndexCount() const {return mIndexCount;}

		protected:
			/**
			 * Updates the dynamic vertex buffer.
			 *  Called from every update().
			 * @return Returns true if any updates have been made.
			 */
			virtual bool updateVertexBufferDynamic() = 0;

			/**
			 * Creates the static vertex buffer.
			 */
			virtual void createVertexBufferStatic() = 0;

			/**
			 * Creates the dynamic vertex buffer.
			 */
			virtual void createVertexBufferDynamic() = 0;

			/**
			 * Creates an empty static vertex buffer.
			 */
			virtual void createEmptyVertexBufferStatic();

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
			 * Creates an empty index buffer.
			 */
			virtual void createEmptyIndexBuffer();

			/**
			 * Releases the vertex buffers.
			 */
			virtual void clearVertexBuffers();

			/**
			 * Releases the static vertex buffer.
			 */
			virtual void clearVertexBufferStatic();

			/**
			 * Releases the dynamic vertex buffer.
			 */
			virtual void clearVertexBufferDynamic();

			/**
			 * Releases the index buffer.
			 */
			virtual void clearIndexBuffer();

			/**
			 * Sets the static vertex buffer.
			 *  Releases the old one first.
			 * @param pVertexBuffer Pointer to a new vertex buffer.
			 */
			virtual void setVertexBufferStatic(ID3D10Buffer *pVertexBuffer);

			/**
			 * Sets the dynamic vertex buffer.
			 *  Releases the old one first.
			 * @param pVertexBuffer Pointer to a new vertex buffer.
			 */
			virtual void setVertexBufferDynamic(ID3D10Buffer *pVertexBuffer);

			/**
			 * Sets the index buffer.
			 *  Releases the old one first.
			 * @param pIndexBuffer Pointer to a new index buffer.
			 */
			virtual void setIndexBuffer(ID3D10Buffer *pIndexBuffer);

			/**
			 * Sets the vertex count.
			 * @param count Value to set mVertexCount to.
			 */
			inline void setVertexCount(unsigned int count){mVertexCount = count;}

			/**
			 * Sets the index count.
			 * @param count Value to set mIndexCount to.
			 */
			inline void setIndexCount(unsigned int count){mIndexCount = count;}

			/**
			 * Sets the copy buffers flag (if vertex and index buffers should be copied
			 *  during the copy constructor and assignment operation).
			 * @param copyBuffer New value for mFlagCopyBuffers.
			 */
			inline void setFlagCopyBuffers(bool copyBuffer){mFlagCopyBuffers = copyBuffer;}

			/**
			 * Sets the release buffers flag (if vertex and index buffers should be released
			 *  during destruction of this object).
			 * @param releaseBuffer New value for mFlagReleaseBuffers.
			 */
			inline void setFlagReleaseBuffers(bool releaseBuffer){mFlagReleaseBuffers = releaseBuffer;}

			/**
			 * Retrieves the static vertex buffer pointer.
			 * @return Returns mpVertexBufferStatic.
			 */
			inline ID3D10Buffer *getVertexBufferStatic(){return mpVertexBufferStatic;}

			/**
			 * Retrieves the dynamic vertex buffer pointer.
			 * @return Returns mpVertexBufferDynamic.
			 */
			inline ID3D10Buffer *getVertexBufferDynamic(){return mpVertexBufferDynamic;}

			/**
			 * Retrieves a pointer to the static vertex buffer pointer.
			 * @return Returns &mpVertexBufferStatic.
			 */
			inline ID3D10Buffer **getVertexBufferStaticPointer(){return &mpVertexBufferStatic;}

			/**
			 * Retrieves a pointer to the dynamic vertex buffer pointer.
			 * @return Returns &mpVertexBufferDynamic.
			 */
			inline ID3D10Buffer **getVertexBufferDynamicPointer(){return &mpVertexBufferDynamic;}

			/**
			 * Retrieves the index buffer.
			 * @return Returns mpIndexBuffer.
			 */
			inline ID3D10Buffer *getIndexBuffer(){return mpIndexBuffer;}

			/**
			 * Retrieves a pointer to the index buffer pointer.
			 * @return Returns &mpIndexBuffer.
			 */
			inline ID3D10Buffer **getIndexBufferPointer(){return &mpIndexBuffer;}

		private:
			/**
			 * Copy vertex and index buffers from the supplied object.
			 * @param object Reference to the object to copy from.
			 */
			void copyBuffers(const BufferObject &object);

			ID3D10Buffer *mpVertexBufferStatic;
			ID3D10Buffer *mpVertexBufferDynamic;
			ID3D10Buffer *mpIndexBuffer;
			unsigned int mVertexCount;
			unsigned int mIndexCount;
			bool mFlagCopyBuffers;
			bool mFlagReleaseBuffers;
	};
}

#endif