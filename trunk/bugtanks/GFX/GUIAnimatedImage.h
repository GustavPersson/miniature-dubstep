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

#ifndef GUI_ANIMATED_IMAGE_H
#define GUI_ANIMATED_IMAGE_H
#include "GUIImage.h"

namespace gfx
{
	/**
	 * Basic GUI component for displaying a texture animation.
	 */
	class GUIAnimatedImage : public GUIImage
	{
		public:
			/**
			 * Default constructor.
			 * @param pBaseInstance Pointer to the instance base of a sprite.
			 * @param clientWidth Width of the client area.
			 * @param clientHeight Height of the client area.
			 * @param parentDepth Depth level of the parent.
			 */
			GUIAnimatedImage(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

			/**
			 * Subframe constructor.
			 * @param pParent Pointer to the parent.
			 */
			GUIAnimatedImage(GUIFrame *pParent);

			/**
			 * Default copy constructor.
			 */
			GUIAnimatedImage(const GUIAnimatedImage &frame);

			/**
			 * Default destructor.
			 */
			virtual ~GUIAnimatedImage();

			/**
			 * Default assignment operator.
			 */
			const GUIAnimatedImage &operator=(const GUIAnimatedImage &frame);

			/**
			 * Updates the GUI frame and all sub frames.
			 * @param deltaTime Time difference since the last frame.
			 */
			virtual void update(float deltaTime);

		protected:
			struct AnimationFrame
			{
				int mTextureID;
				D3DXCOLOR mColor;
				float mTime;

				AnimationFrame() : mColor(1.0f, 1.0f, 1.0f, 1.0f)
				{
					mTextureID = -1;
					mTime = 0.0f;
				}

				~AnimationFrame(){}
			};

			/**
			 * Initializes the frame.
			 */
			virtual void init();

			/**
			 * Sends/receives a notification.
			 * @param message Message to send/receive.
			 * @param pSender Pointer to the sender.
			 */
			virtual void notify(int message, GUIFrame *pSender);

			/**
			 * Adds a copy of an animation frame to the frame list.
			 * @param frame Animation frame to add to mAnimationFrameList.
			 */
			virtual void addAnimation(const AnimationFrame &frame);

			/**
			 * Updates the current animation.
			 */
			virtual void updateAnimation(float deltaTime);

		private:
			std::vector<AnimationFrame> mAnimationFrameList;
			std::vector<AnimationFrame>::iterator mAnimationIter;
			float mTimer;
			float mAnimationLength;
	};
}

#endif