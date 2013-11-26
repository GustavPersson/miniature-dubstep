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

#ifndef GUI_FRAME_H
#define GUI_FRAME_H
#include <vector>
#include "InstanceSprite.h"

namespace gfx
{
	/**
	 * Namespace for frame types.
	 */
	namespace FrameTypes
	{
		enum Types
		{
			FRAME = 1,
			ANIMATED_FRAME = 2,
			BUTTON = 3,
			ANIMATED_BUTTON = 4,
			LIST = 5,
			SCROLL_LIST = 6
		};
	}
	
	/**
	 * Base class for all GUI components.
	 */
	class GUIFrame
	{
		public:
			/**
			 * Default constructor.
			 */
			GUIFrame(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight);

			/**
			 * Default copy constructor.
			 */
			GUIFrame(const GUIFrame &frame);

			/**
			 * Default destructor.
			 */
			virtual ~GUIFrame();

			/**
			 * Default assignment operator.
			 */
			const GUIFrame &operator=(const GUIFrame &frame);

			/**
			 * Updates the GUI frame and all sub frames.
			 * @param deltaTime Time difference since the last frame.
			 */
			virtual void update(float deltaTime);

			/**
			 * Method that is called when the mouse pointer enters
			 * the frame.
			 */
			virtual void onMouseEnter(long mX, long mY);

			/**
			 * Method that is called when the mouse pointer exits
			 * the frame.
			 */
			virtual void onMouseExit(long mX, long mY);

			/**
			 * Method that is called when the left mouse button
			 * is pressed down within the frame.
			 */
			virtual void onMouseLeftDown(long mX, long mY);

			/**
			 * Method that is called when the middle mouse button
			 * is pressed down within the frame.
			 */
			virtual void onMouseMiddleDown(long mX, long mY);

			/**
			 * Method that is called when the right mouse button
			 * is pressed down within the frame.
			 */
			virtual void onMouseRightDown(long mX, long mY);

			/**
			 * Method that is called when the left mouse button
			 * is released within the frame.
			 */
			virtual void onMouseLeftUp(long mX, long mY);

			/**
			 * Method that is called when the middle mouse button
			 * is released within the frame.
			 */
			virtual void onMouseMiddleUp(long mX, long mY);

			/**
			 * Method that is called when the right mouse button
			 * is released within the frame.
			 */
			virtual void onMouseRightUp(long mX, long mY);

			/**
			 * Checks if a point is inside the frame.
			 * @param X X coordinate of the point.
			 * @param Y Y coordinate of the point.
			 * @return Returns true or false.
			 */
			virtual bool pointInFrame(long X, long Y);

			/**
			 * Adds all sub frames to another vector.
			 * @param frameList Vector to add the sub frames to.
			 */
			virtual void addSubFrames(std::vector<GUIFrame *> &frameList) const;

			/**
			 * Adds all visible sub frames to another vector.
			 * @param frameList Vector to add the sub frames to.
			 */
			virtual void addVisibleSubFrames(std::vector<GUIFrame *> &frameList) const;

			/**
			 * Increases the relative position with an offset.
			 * @param offset Offset to add.
			 */
			void increaseRelativePosition(const D3DXVECTOR2 &offset);

			/**
			 * Increases the relative size with an offset.
			 * @param offset Offset to add.
			 */
			void increaseRelativeSize(const D3DXVECTOR2 &offset);

			/**
			 * Sets the relative position of this frame.
			 * @param position New relative position.
			 */
			void setRelativePosition(const D3DXVECTOR2 &position);

			/**
			 * Sets the relative size of this frame.
			 * @param position New relative size.
			 */
			void setRelativeSize(const D3DXVECTOR2 &size);

			/**
			 * Retrieves the sub frame list.
			 */
			inline const std::vector<GUIFrame *> &getSubFrameList() const {return mSubFrameList;}

			/**
			 * Retrieves the frame type.
			 * @return Returns mFrameType.
			 */
			inline int getFrameType() const {return mFrameType;}

			/**
			 * Retrieves the relative position of this frame.
			 * @return Returns mRelativePosition.
			 */
			inline const D3DXVECTOR2 &getRelativePosition() const {return mRelativePosition;}

			/**
			 * Retrieves the relative size of this frame.
			 * @return Returns mRelativeSize.
			 */
			inline const D3DXVECTOR2 &getRelativeSize() const {return mRelativeSize;}

			/**
			 * Retrieves the absolute x coordinate of this frame.
			 * @return Returns the x coordinate in screen space.
			 */
			inline long getAbsoluteX() const {return mAbsoluteX;}

			/**
			 * Retrieves the absolute y coordinate of this frame.
			 * @return Returns the y coordinate in screen space.
			 */
			inline long getAbsoluteY() const {return mAbsoluteY;}

			/**
			 * Retrieves the absolute x size of this frame.
			 * @return Returns the x size in screen space.
			 */
			inline long getAbsoluteSizeX() const {return mAbsoluteSizeX;}

			/**
			 * Retrieves the absolute y size of this frame.
			 * @return Returns the y size in screen space.
			 */
			inline long getAbsoluteSizeY() const {return mAbsoluteSizeY;}

			/**
			 * Retrieves the mouse focus flag; if the frame currently has mouse focus.
			 * @return Returns mFlagMouseFocus.
			 */
			inline bool getFlagMouseFocus() const {return mFlagMouseFocus;}

			/**
			 * Retrieves the mouse left down flag; if the left mouse button currently is
			 * pressed down over this frame.
			 * @return Returns mFlagMouseLeftDown.
			 */
			inline bool getFlagMouseLeftDown() const {return mFlagMouseLeftDown;}

			/**
			 * Retrieves the mouse middle down flag; if the middle mouse button currently is
			 * pressed down over this frame.
			 * @return Returns mFlagMouseMiddleDown.
			 */
			inline bool getFlagMouseMiddleDown() const {return mFlagMouseMiddleDown;}

			/**
			 * Retrieves the mouse right down flag; if the right mouse button currently is
			 * pressed down over this frame.
			 * @return Returns mFlagMouseRightDown.
			 */
			inline bool getFlagMouseRightDown() const {return mFlagMouseRightDown;}

		protected:
			/**
			 * Updates the absolute position and size from the relative.
			 */
			virtual void updateAbsolutePosition();

			/**
			 * Removes (deletes) all sub frames.
			 */
			virtual void removeAllSubFrames();

		private:
			std::vector<GUIFrame *> mSubFrameList;
			int mFrameType;
			InstanceSprite *mpBaseInstance;
			InstanceSprite *mpRenderInstance;
			long mClientWidth;
			long mClientHeight;
			D3DXVECTOR2 mRelativePosition;
			D3DXVECTOR2 mRelativeSize;
			long mAbsoluteX;
			long mAbsoluteY;
			long mAbsoluteSizeX;
			long mAbsoluteSizeY;
			bool mFlagMouseFocus;
			bool mFlagMouseLeftDown;
			bool mFlagMouseMiddleDown;
			bool mFlagMouseRightDown;
	};
}

#endif