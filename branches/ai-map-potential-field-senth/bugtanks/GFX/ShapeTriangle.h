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

#ifndef SHAPE_TRIANGLE_H
#define SHAPE_TRIANGLE_H
#include "BufferObject.h"

namespace gfx
{
	/**
	 * Basic shape in the form of a triangle.
	 */
	class ShapeTriangle : public BufferObject
	{
		public:
			/**
			 * Default constructor.
			 * @param pDevice Pointer to the ID3D10Device.
			 * @param pEffect Pointer to an Effect.
			 */
			ShapeTriangle(ID3D10Device *pDevice, Effect *pEffect);

			/**
			 * Default copy constructor.
			 */
			ShapeTriangle(const ShapeTriangle &triangle);

			/**
			 * Default destructor.
			 */
			virtual ~ShapeTriangle();

			/**
			 * Default assignment operator.
			 */
			const ShapeTriangle &operator=(const ShapeTriangle &triangle);

			/**
			 * Clones the object.
			 */
			virtual RenderObject *clone();

			/**
			 * Updates the object.
			 * Functionality varies depending on object type.
			 */
			virtual void update(float deltaTime);

		protected:
			/**
			 * Updates the dynamic vertex buffer.
			 *  Called from every update().
			 * @return Returns true if any updates have been made.
			 */
			virtual bool updateVertexBufferDynamic();

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
	};
}

#endif