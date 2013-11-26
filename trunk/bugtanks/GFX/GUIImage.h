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

#ifndef GUI_IMAGE_H
#define GUI_IMAGE_H
#include "GUIFrame.h"

namespace gfx
{
	/**
	 * Basic GUI component for displaying an image
	 * or coloured quad.
	 * Contains a single InstanceSprite to render with.
	 */
	class GUIImage : public GUIFrame
	{
		public:
			/**
			 * Default constructor.
			 * @param pBaseInstance Pointer to the instance base of a sprite.
			 * @param clientWidth Width of the client area.
			 * @param clientHeight Height of the client area.
			 * @param parentDepth Depth level of the parent.
			 */
			GUIImage(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

			/**
			 * Subframe constructor.
			 * @param pParent Pointer to the parent.
			 */
			GUIImage(GUIFrame *pParent);

			/**
			 * Default copy constructor.
			 */
			GUIImage(const GUIImage &frame);

			/**
			 * Default destructor.
			 */
			virtual ~GUIImage();

			/**
			 * Default assignment operator.
			 */
			const GUIImage &operator=(const GUIImage &frame);

			/**
			 * Updates the GUI frame and all sub frames.
			 * @param deltaTime Time difference since the last frame.
			 */
			virtual void update(float deltaTime);

			/**
			 * Method that is called when the mouse pointer enters
			 * the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseEnter(long mX, long mY);

			/**
			 * Method that is called when the mouse pointer exits
			 * the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseExit(long mX, long mY);

			/**
			 * Method that is called when the mouse pointer moves within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseMove(long mX, long mY);

			/**
			 * Method that is called when the left mouse button
			 * is pressed down within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseLeftDown(long mX, long mY);

			/**
			 * Method that is called when the middle mouse button
			 * is pressed down within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseMiddleDown(long mX, long mY);

			/**
			 * Method that is called when the right mouse button
			 * is pressed down within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseRightDown(long mX, long mY);

			/**
			 * Method that is called when the left mouse button
			 * is released within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseLeftUp(long mX, long mY);

			/**
			 * Method that is called when the middle mouse button
			 * is released within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseMiddleUp(long mX, long mY);

			/**
			 * Method that is called when the right mouse button
			 * is released within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseRightUp(long mX, long mY);

			/**
			 * Method that is called when a keyboard key is pressed.
			 * @param letterKeys Integer containing letter key status information.
			 * @param keys Integer containing key status information.
			 */
			virtual void onKeyDown(int letterKeys, int keys);

			/**
			 * Sets the render flag for the frame and all sub frames.
			 * @param render New value for the render flag.
			 */
			virtual void setFlagRender(bool render);

			/**
			 * Sets the current color and updates the render instance
			 *  during next update call.
			 * @param color New color.
			 */
			virtual void setColor(const D3DXCOLOR &color);

			/**
			 * Sets the current texture ID and updates the render instance
			 *  during next update call.
			 * @param textureID New texture ID.
			 */
			virtual void setTextureID(int textureID);

			/**
			 * Retrieves the pointer to the render instance of the sprite.
			 * @return Returns mpRenderInstance.
			 */
			inline InstanceSprite *getRenderInstance(){return mpRenderInstance;}

			/**
			 * Retrieves the current color.
			 * @return Returns mColor.
			 */
			inline const D3DXCOLOR &getColor() const {return mColor;}

			/**
			 * Retrieves the current texture ID.
			 * @return Returns mTextureID.
			 */
			inline int getTextureID() const {return mTextureID;}

		protected:
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

		private:
			InstanceSprite *mpRenderInstance;
			D3DXCOLOR mColor;
			int mTextureID;
	};
}

#endif