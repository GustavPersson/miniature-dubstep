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

#ifndef GUI_HANDLER_H
#define GUI_HANDLER_H
#include "../Logic/Events.h"
#include "GUIFrame.h"

namespace gfx
{
	/**
	 * Abstract base class for GUI managers.
	 * Each GUI component (menu, game HUD etc.) should be a GUIHandler.
	 */
	class GUIHandler : public logic::EventListener
	{
		public:
			/**
			 * Default constructor.
			 * @param pBaseInstance Pointer to a sprite instance base which will render all GUI frames.
			 * @param clientWidth Width of the client area.
			 * @param clientHeight Height of the client area.
			 */
			GUIHandler(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight);

			/**
			 * Default copy constructor.
			 */
			GUIHandler(const GUIHandler &handler);

			/**
			 * Default destructor.
			 */
			virtual ~GUIHandler() = 0;

			/**
			 * Default assignment operator.
			 */
			const GUIHandler &operator=(const GUIHandler &handler);

			/**
			 * Returns the name of the listener, as a string.
			 * @return Returns the name as a stdstring
			 */
			virtual const char *getName() = 0;

			/**
			 * Handles events.
			 * @param eventMessage The event that should be handled.
			 * @return Returns true if the event was consumed, and should not be propogated
			 * @return Returns false if the event was not consumed.
			 */
			virtual bool handleEvent(const logic::EventData &eventMessage);

			/**
			 * Updates all GUI frames.
			 *  GFX must be locked.
			 * @param deltaTime Time difference since last frame.
			 */
			virtual void updateFrames(float deltaTime);

			/**
			 * Returns the base instance of the sprites.
			 *  Should be sent to all created GUIFrames.
			 * @return Returns mpBaseInstance.
			 */
			inline InstanceSprite *getInstanceBase(){return mpBaseInstance;}

			/**
			 * Returns the client width set to this GUI handler.
			 * @return Returns mClientWidth;
			 */
			inline long getClientWidth() const {return mClientWidth;}

			/**
			 * Returns the client height set to this GUI handler.
			 * @return Returns mClientHeight;
			 */
			inline long getClientHeight() const {return mClientHeight;}

			/**
			 * Returns the saved X coordinate of the mouse cursor.
			 * @return Returns mMouseX.
			 */
			inline long getMouseX() const {return mMouseX;}

			/**
			 * Returns the saved Y coordinate of the mouse cursor.
			 * @return Returns mMouseY.
			 */
			inline long getMouseY() const {return mMouseY;}

			/**
			 * Returns the mouse in frame flag. If true, the mouse pointer is over
			 *  at least one frame.
			 * @return Returns mFlagMouseInFrame.
			 */
			inline bool getFlagMouseInFrame() const {return mFlagMouseInFrame;}

		protected:
			/**
			 * Create all frames for the GUI.
			 */
			virtual void init() = 0;

			/**
			 * Adds a GUI frame to the frame list.
			 * @param pFrame Pointer to a GUI frame.
			 */
			virtual void addFrame(GUIFrame *pFrame);

			/**
			 * Removes (deletes) all GUI frames.
			 */
			virtual void removeAllFrames();

		private:
			std::vector<GUIFrame *> mFrameList;
			InstanceSprite *mpBaseInstance;
			long mClientWidth;
			long mClientHeight;
			long mMouseX;
			long mMouseY;
			bool mFlagMouseInFrame;
	};
}

#endif