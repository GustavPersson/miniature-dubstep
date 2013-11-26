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
#include "DX10Module.h"
#include "InstanceSprite.h"
#include "../Logic/InputStates.h"

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
			SUBFRAME,
			IMAGE,
			SUBIMAGE,
			TEXT,
			SUBTEXT
		};
	}
	
	/**
	 * Base class for all GUI components.
	 * Contains virtual methods for handling events and
	 * converting relative screen position/size to absolute.
	 * Also handles updates for subframes.
	 */
	class GUIFrame
	{
		public:
			enum BasicMessage
			{
				BasicMessage_PressLeft = 1,
				BasicMessage_PressMiddle,
				BasicMessage_PressRight,
				BasicMessage_MouseEnter,
				BasicMessage_MouseExit,
				BasicMessage_MouseMove
			};

			/**
			 * Default constructor.
			 * @param pBaseInstance Pointer to the instance base of a sprite.
			 * @param clientWidth Width of the client area.
			 * @param clientHeight Height of the client area.
			 * @param parentDepth Depth level of the parent.
			 */
			GUIFrame(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

			/**
			 * Subframe constructor.
			 * @param pParent Pointer to the parent.
			 */
			GUIFrame(GUIFrame *pParent);

			/**
			 * Default copy constructor.
			 */
			GUIFrame(const GUIFrame &frame);

			/**
			 * Default destructor.
			 */
			virtual ~GUIFrame() = 0;

			/**
			 * Default assignment operator.
			 */
			const GUIFrame &operator=(const GUIFrame &frame);

			/**
			 * Updates the GUI frame and all sub frames.
			 * @param deltaTime Time difference since the last frame.
			 */
			virtual void update(float deltaTime) = 0;

			/**
			 * Method that is called when the mouse pointer enters
			 * the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseEnter(long mX, long mY) = 0;

			/**
			 * Method that is called when the mouse pointer exits
			 * the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseExit(long mX, long mY) = 0;

			/**
			 * Method that is called when the mouse pointer moves within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseMove(long mX, long mY) = 0;

			/**
			 * Method that is called when the left mouse button
			 * is pressed down within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseLeftDown(long mX, long mY) = 0;

			/**
			 * Method that is called when the middle mouse button
			 * is pressed down within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseMiddleDown(long mX, long mY) = 0;

			/**
			 * Method that is called when the right mouse button
			 * is pressed down within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseRightDown(long mX, long mY) = 0;

			/**
			 * Method that is called when the left mouse button
			 * is released within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseLeftUp(long mX, long mY) = 0;

			/**
			 * Method that is called when the middle mouse button
			 * is released within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseMiddleUp(long mX, long mY) = 0;

			/**
			 * Method that is called when the right mouse button
			 * is released within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseRightUp(long mX, long mY) = 0;

			/**
			 * Method that is called when a keyboard key is pressed.
			 * @param letterKeys Integer containing letter key status information.
			 * @param keys Integer containing key status information.
			 */
			virtual void onKeyDown(int letterKeys, int keys) = 0;

			/**
			 * Checks if a point is inside the frame.
			 * @param X X coordinate of the point.
			 * @param Y Y coordinate of the point.
			 * @return Returns true or false.
			 */
			virtual bool pointInFrame(long X, long Y);

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
			 * Sets the render flag for the frame and all sub frames.
			 * @param render New value for the render flag.
			 */
			virtual void setFlagRender(bool render);

			/**
			 * Sets the relative position of this frame.
			 * @param position New relative position.
			 */
			void setRelativePosition(const D3DXVECTOR2 &position);

			/**
			 * Sets the relative size of this frame.
			 * @param size New relative size.
			 */
			void setRelativeSize(const D3DXVECTOR2 &size);

			/**
			 * Sets the depth level for this frame.
			 * @param depth New depth level.
			 */
			inline void setDepthLevel(long depth){mDepthLevel = depth;}

			/**
			 * Sets the omni focus flag (if the frame should receive focus when one of its
			 *  subframes does).
			 * @param omniFocus New value for mFlagMouseOmniFocus.
			 */
			inline void setMouseOmniFocus(bool omniFocus){mFlagMouseOmniFocus = omniFocus;}

			/**
			 * Sets the visibility flag.
			 * @param flag New value for mFlagVisible.
			 */
			virtual void setFlagVisible(bool flag);

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
			 * Retrieves the depth level for this frame.
			 * @return Returns mDepthLevel.
			 */
			inline long getDepthLevel() const {return mDepthLevel;}

			/**
			 * Retrieves the mouse frame focus flag; if the frame or any subframe
			 *  currently has mouse focus.
			 * @return Returns mFlagMouseFrameFocus.
			 */
			inline bool getFlagMouseFrameFocus() const {return mFlagMouseFrameFocus;}

			/**
			 * Retrieves the mouse omni focus flag; if both the frame and subframe
			 *  can get mouse focus at the same time.
			 * @return Returns mFlagMouseOmniFocus.
			 */
			inline bool getFlagMouseOmniFocus() const {return mFlagMouseOmniFocus;}

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

			/**
			 * Retrieves the visibility flag.
			 * @return Returns mFlagVisible.
			 */
			inline bool getFlagVisible() const {return mFlagVisible;}

		protected:
			/**
			 * Initializes the frame.
			 */
			virtual void init() = 0;

			/**
			 * Sends/receives a notification.
			 * @param message Message to send/receive.
			 * @param pSender Pointer to the sender.
			 */
			virtual void notify(int message, GUIFrame *pSender);

			/**
			 * Flags that the frame should be updated during next update().
			 */
			virtual void updateRequired();

			/**
			 * Updates the absolute position and size from the relative.
			 */
			virtual void updateAbsolutePosition();

			/**
			 * Adds a subframe to the sub frame list.
			 * @param pSubFrame Pointer to the subframe.
			 */
			virtual void addSubFrame(GUIFrame *pSubFrame);

			/**
			 * Removes (deletes) a sub frame.
			 * @param pSubFrame Pointer to the sub frame.
			 */
			virtual void removeSubFrame(GUIFrame *pSubFrame);

			/**
			 * Removes (deletes) all sub frames.
			 */
			virtual void removeAllSubFrames();

			/**
			 * Sets the current frame type.
			 * @param type New value for mFrameType.
			 */
			inline void setFrameType(int type){mFrameType = type;}

			/**
			 * Sets the instance base.
			 * @param pBase New instance base.
			 */
			inline void setBaseInstance(InstanceSprite *pBase){mpBaseInstance = pBase;}

			/**
			 * Retrieves the pointer to the parent of the frame.
			 * @return Returns mpParent.
			 */
			inline GUIFrame *getParent(){return mpParent;}

			/**
			 * Retrieves the pointer to the base instance of the sprite.
			 * @return Returns mpRenderInstance.
			 */
			inline InstanceSprite *getBaseInstance(){return mpBaseInstance;}

			/**
			 * Retrieves the update required flag. This means that the object has
			 *  moved or changed in some way that requires an update.
			 * @return Returns mFlagUpdateRequired.
			 */
			inline bool getFlagUpdateRequired() const {return mFlagUpdateRequired;}

		private:
			std::vector<GUIFrame *> mSubFrameList;
			int mFrameType;
			GUIFrame *mpParent;
			InstanceSprite *mpBaseInstance;
			long mClientWidth;
			long mClientHeight;
			D3DXVECTOR2 mRelativePosition;
			D3DXVECTOR2 mRelativeSize;
			long mAbsoluteX;
			long mAbsoluteY;
			long mAbsoluteSizeX;
			long mAbsoluteSizeY;
			long mDepthLevel;
			bool mFlagUpdateRequired;
			bool mFlagNewUpdateRequired;
			bool mFlagMouseFrameFocus;
			bool mFlagMouseOmniFocus;
			bool mFlagMouseFocus;
			bool mFlagMouseLeftDown;
			bool mFlagMouseMiddleDown;
			bool mFlagMouseRightDown;
			bool mFlagVisible;
	};
}

#endif