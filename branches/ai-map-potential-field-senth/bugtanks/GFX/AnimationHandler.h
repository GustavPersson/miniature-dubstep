/**
 * @file
 * @author Carl Nordlander <cand05@student.bth.se>
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

#ifndef ANIMATIONHANDLER_H
#define ANIMATIONHANDLER_H

#include <vector>
#include "../Utilities/Macros.h"
#include "Animation.h"

namespace gfx
{
	/**
	 * Loads and stores animations.
	 * Can create a texture array with the stored annimations.
	 */
	class AnimationHandler
	{
		public:
			/**
			 * Constructor.
			 * @param pDevice . The ID3D10Device.
			 */
			AnimationHandler(ID3D10Device * pDevice);

			/**
			 * Destructor.
			 */
			~AnimationHandler();

			/**
			 * Adds an animation to the animation vector.
			 * @param pAnimation Pointer to the new animation.
			 */
			void addAnimation(Animation *pAnimation);

			/**
			 * Creates a texture array from the animations.
			 */
			ID3D10ShaderResourceView *getTextureArray() const;

			/**
			 * Creates a texture array which stores a vector in each texel.
			 */
			ID3D10ShaderResourceView *getTextureArrayVector() const;

			/**
			 * Returns an animation.
			 * @param name The name of the animation.
			 */
			Animation *getAnimation(std::string name);

			/**
			 * Finds the texture index of an animation.
			 * @param name Name of the animation to search for.
			 * @return Returns the index of the animation in the texture array.
			 */
			int getAnimationIndex(const char *name);

			/**
			 * Finds the texture index of an animation.
			 * @param name Name of the animation to search for.
			 * @return Returns the index of the animation in the texture array.
			 */
			int getAnimationIndex(const std::string &name);

		private:
			std::vector<Animation*> mAnimations;
			ID3D10Device * mpDevice;
	};
}

#endif