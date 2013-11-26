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

#ifndef INSTANCE_ANIMATION_SUBSET_H
#define INSTANCE_ANIMATION_SUBSET_H
#include "InstanceSubset.h"

namespace gfx
{
	class InstanceAnimationSubset : public InstanceSubset
	{
		public:
			/**
			 * Default constructor.
			 * @param pDevice Pointer to the ID3D10Device.
			 * @param pEffect Pointer to an Effect.
			 */
			InstanceAnimationSubset(ID3D10Device *pDevice, Effect *pEffect);

			/**
			 * Default copy constructor.
			 */
			InstanceAnimationSubset(const InstanceAnimationSubset &subset);

			/**
			 * Default destructor.
			 */
			virtual ~InstanceAnimationSubset();

			/**
			 * Default assignment operator.
			 */
			const InstanceAnimationSubset &operator=(const InstanceAnimationSubset &subset);

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
			 * Sets the current animation based on its texture index.
			 * @param index Index of the animation in the texture array.
			 */
			inline void setAnimationIndex(int index){mAnimationIndex = index;}

			/**
			 * Sets the play speed factor of the animation.
			 * @param factor New play speed factor.
			 */
			inline void setAnimationSpeedFactor(float factor){mAnimationSpeedFactor = factor;}

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

		private:
			int mAnimationIndex;
			float mAnimationTime;
			float mAnimationSpeedFactor;
			float mAnimationLength;
			bool mPlayOnce;
	};
}

#endif