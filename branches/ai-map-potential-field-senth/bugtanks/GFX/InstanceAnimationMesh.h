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

#ifndef INSTANCE_ANIMATION_MESH_H
#define INSTANCE_ANIMATION_MESH_H
#include "InstanceMesh.h"
#include "InstanceAnimationSubset.h"

namespace gfx
{
	class InstanceAnimationMesh : public InstanceMesh
	{
		public:
			/**
			 * Default constructor.
			 * @param pDevice Pointer to the ID3D10Device.
			 * @param pEffect Pointer to an Effect.
			 */
			InstanceAnimationMesh(ID3D10Device *pDevice, Effect *pEffect);

			/**
			 * Default copy constructor.
			 */
			InstanceAnimationMesh(const InstanceAnimationMesh &mesh);

			/**
			 * Default destructor.
			 */
			virtual ~InstanceAnimationMesh();

			/**
			 * Default assignment operator.
			 */
			const InstanceAnimationMesh &operator=(const InstanceAnimationMesh &mesh);

			/**
			 * Clones the object.
			 */
			virtual RenderObject *clone();

			/**
			 * Creates a new instance to this object.
			 * @return Returns a pointer to the newly created instance.
			 */
			virtual InstanceObject *createInstance();

			/**
			 * Sets the current animation based on its name.
			 * @param animationName Name of the animation to select.
			 */
			virtual void setAnimation(const char *animationName);

			/**
			 * Sets the current animation based on its name.
			 * @param animationName Name of the animation to select.
			 */
			virtual void setAnimation(const std::string &animationName);

			/**
			 * Sets the current animation based on its texture index.
			 * @param index Index of the animation in the texture array.
			 */
			virtual void setAnimationIndex(int index);

			/**
			 * Sets the play speed factor of the animation.
			 * @param factor New play speed factor.
			 */
			virtual void setAnimationSpeedFactor(float factor);

		protected:

		private:
	};
}

#endif