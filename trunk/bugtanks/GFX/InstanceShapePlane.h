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

#ifndef INSTANCE_SHAPE_PLANE_H
#define INSTANCE_SHAPE_PLANE_H
#include "InstanceObject.h"

namespace gfx
{
	/**
	 * Class for instanced plane shapes.
	 * Uses multitexturing.
	 */
	class InstanceShapePlane : public InstanceObject
	{
		public:
			/**
			 * Default constructor.
			 * @param pDevice Pointer to the ID3D10Device.
			 * @param pEffect Pointer to an Effect.
			 */
			InstanceShapePlane(ID3D10Device *pDevice, Effect *pEffect);

			/**
			 * Default copy constructor.
			 */
			InstanceShapePlane(const InstanceShapePlane &plane);

			/**
			 * Default destructor.
			 */
			virtual ~InstanceShapePlane();

			/**
			 * Default assignment operator.
			 */
			const InstanceShapePlane &operator=(const InstanceShapePlane &plane);

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
			 * Creates a new instance to this object.
			 * @return Returns a pointer to the newly created instance.
			 */
			virtual InstanceObject *createInstance();

			/**
			 * Shapes create their own buffers so no input is required.
			 */
			virtual void loadBuffers();

			/**
			 * Updates the weight map array.
			 * @param pWeightMap Pointer to the weight map array.
			 * @param index Index in the array for this plane.
			 */
			virtual void updateWeightMap(ID3D10Resource *pWeightMap, int index);

			/**
			 * Sets the weights for all vertices in a circular area around the supplied position.
			 * @param position Position in world space to set the weights around.
			 * @param radius Radius of the circular area.
			 * @param weights New weights to set.
			 */
			virtual void setWeights(const D3DXVECTOR3 &position, float radius, const D3DXVECTOR4 &weights);

			/**
			 * Increases the weights by an offset for all vertices in a circular area around the supplied position.
			 * @param position Position in world space to set the weights around.
			 * @param radius Radius of the circular area.
			 * @param weightOffset Offset to add to the weights.
			 */
			virtual void increaseWeights(const D3DXVECTOR3 &position, float radius, const D3DXVECTOR4 &weightOffset);

			/**
			 * Sets the array slice index.
			 * @param slice New value for mArraySlice.
			 */
			inline void setArraySlice(unsigned int slice){mArraySlice = slice;}

			/**
			 * Sets the array index of the second texture.
			 * @param index New array index.
			 */
			inline void setTextureArrayIndex2(int index){mArrayIndex2 = index;}

			/**
			 * Sets the array index of the third texture.
			 * @param index New array index.
			 */
			inline void setTextureArrayIndex3(int index){mArrayIndex3 = index;}

			/**
			 * Sets the array index of the fourth texture.
			 * @param index New array index.
			 */
			inline void setTextureArrayIndex4(int index){mArrayIndex4 = index;}

			/**
			 * Sets the texture ID of the second texture.
			 * @param ID New texture ID.
			 */
			inline void setTextureID2(int ID){mTextureID2 = ID;}

			/**
			 * Sets the texture ID of the third texture.
			 * @param ID New texture ID.
			 */
			inline void setTextureID3(int ID){mTextureID3 = ID;}

			/**
			 * Sets the texture ID of the fourth texture.
			 * @param ID New texture ID.
			 */
			inline void setTextureID4(int ID){mTextureID4 = ID;}

			/**
			 * Retrieves the width of the plane (in number of vertices).
			 * @return Returns mWidth.
			 */
			inline int getWidth() const {return mWidth;}

			/**
			 * Retrieves the height of the plane (in number of vertices).
			 * @return Returns mHeight.
			 */
			inline int getHeight() const {return mHeight;}

			/**
			 * Retrieves the array slice index.
			 * @return Returns mArraySlice.
			 */
			inline unsigned int getArraySlice() const {return mArraySlice;}

			/**
			 * Sets all weights in the plane.
			 * @param weights Vector containing weights for all vertices.
			 */
			void setWeights(const std::vector<D3DXVECTOR4> &weights);

			/**
			 * Retrieves the weights in vector form.
			 * @param weights Outgoing reference to the weight vector.
			 */
			void getWeights(std::vector<D3DXVECTOR4> &weights);

		protected:
			/**
			 * Updates the vertex buffer.
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

			/**
			 * Creates the index buffer with adjacent information.
			 */
			virtual void createIndexBufferAdjacent();

		private:
			D3DXVECTOR4 *mpWeights;
			bool mFlagUpdateWeights;
			int mWidth;
			int mHeight;
			int mUVScale;
			float mOffsetX;
			float mOffsetZ;
			unsigned int mArraySlice;
			int mArrayIndex2;
			int mArrayIndex3;
			int mArrayIndex4;
			int mTextureID2;
			int mTextureID3;
			int mTextureID4;
	};
}

#endif