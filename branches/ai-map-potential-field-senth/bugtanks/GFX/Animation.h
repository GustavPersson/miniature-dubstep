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

#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "../Utilities/Macros.h"
#include "Frame.h"
#include "Joint.h"
#include <fstream>
#include <string>

namespace gfx
{
	/**
	 * Loads and stores a annimation.
	 * Can create a texture from the animation.
	 */
	class Animation
	{
		public:
			/**
			 * Constructor.
			 * @param pDevice . The ID3D10Device.
			 * @param name . The name of the texture.
			 */
			Animation(ID3D10Device * pDevice, std::string name);

			/**
			 * Destructor.
			 */
			~Animation();

			/**
			 * Adds a frame to the frame vector.
			 * @param pFrame Pointer to the new frame.
			 */
			void addFrame(Frame *pFrame);

			/**
			 * Returns a frame.
			 * @param index . The index of the frame. 
			 */
			Frame *getFrame(unsigned int index) const;

			/**
			 * Returns the number of frames.
			 */
			unsigned int getSize() const;

			/**
			 * Creates a texture from the aimation.
			 */
			ID3D10ShaderResourceView *getAsTexture() const;

			/**
			 * Returns the time per frame in seconds.
			 */
			float getTimePerFrame() const;

			/**
			 * Sets the time per frame in seconds.
			 * You have to create a texture from the annimation
			 * and send to the shader to see the difference.
			 */
			void setTimePerFrame(float timePerFrame);

			/**
			 * Sets the name of the animation.
			 */
			inline void setName(const char *name){mName = name;}

			/**
			 * Sets the name of the animation.
			 */
			inline void setName(const std::string &name){mName = name;}

			/**
			 * Returns the name of the animation.
			 */
			std::string getName() const;

		private:
			std::vector<Frame*> mFrames;
			ID3D10Device *mpDevice;
			float mTimePerFrame;
			std::string mName;
	};
}

#endif