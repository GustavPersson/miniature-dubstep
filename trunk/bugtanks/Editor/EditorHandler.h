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

#ifndef EDITOR_HANDLER_H
#define EDITOR_HANDLER_H
#include "EditorGUI.h"
#include "EditorFileHandler.h"
#include "MouseMarker.h"
#include "EditorMessages.h"

namespace editor
{
	/**
	 * Main interface to the editor functionality.
	 * Note: creates a GUI sprite instance base in GFX
	 *  during construction.
	 */
	class EditorHandler : public logic::EventListener
	{
		public:
			/**
			 * Default destructor.
			 */
			virtual ~EditorHandler();

			/**
			 * Retrieves the EditorHandler instance.
			 *  Note: The graphics thread must be locked during the first
			 *  call to this method.
			 * @return Returns mpEditorHandlerInstance.
			 */
			static EditorHandler *getInstance();

			/**
			 * Returns the name of the listener, as a string.
			 * @return Returns the name as a stdstring
			 */
			virtual char const *getName(void);

			/**
			 * Handles events.
			 * @param eventData the event that should be handled.
			 * @return returns true if the event was consumed, and should not be propogated
			 * @return returns false if the event was not consumed.
			 *
			 */
			virtual bool handleEvent(logic::EventData const &eventData);

			/**
			 * Handles messages sent from editor components.
			 * @param message Message to handle.
			 */
			virtual void handleMessage(const EM &message);

			/**
			 * Activates the editor state.
			 */
			virtual void activate();

			/**
			 * Deactivates the editor state.
			 */
			virtual void deactivate();

			/**
			 * Updates editor functions.
			 * @param deltaTime Time since last frame.
			 */
			virtual void update(float deltaTime);

			/**
			 * Updates editor graphical functions.
			 *  Note: The graphics thread must be locked during calls
			 *  to this method.
			 * @param deltaTime Time since last frame.
			 */
			virtual void updateGFX(float deltaTime);

			/**
			 * Adds a message to the message vector.
			 * @param pMessage Pointer to the message.
			 */
			virtual void addMessage(EM *pMessage);

			/**
			 * Saves the current map.
			 *  Note: the GFX thread must be locked.
			 * @param mapName Name of the map to save.
			 */
			virtual void saveMapNonThreadSafe(const std::string &mapName);

			/**
			 * Saves the current map.
			 *  Note: the GFX thread must be locked.
			 * @param mapName Name of the map to save.
			 */
			virtual void saveMapNonThreadSafe(const char *mapName);

			/**
			 * Loads the current map.
			 *  Note: the GFX thread must be locked.
			 * @param mapName Name of the map to load.
			 */
			virtual void loadMapNonThreadSafe(const std::string &mapName);

			/**
			 * Loads the current map.
			 *  Note: the GFX thread must be locked.
			 * @param mapName Name of the map to load.
			 */
			virtual void loadMapNonThreadSafe(const char *mapName);

			/**
			 * Retrieves the active flag.
			 * @return Returns mActive.
			 */
			inline bool isActive() const {return mActive;}

			/**
			 * Retrieves the coordinates of the mouse pointer on the world plane.
			 * @return Returns the plane position of the mouse.
			 */
			inline const D3DXVECTOR3 &getMousePlanePosition() const {return mMouseMarker.getPlanePosition();}

		protected:

		private:
			/**
			 * Default constructor.
			 */
			EditorHandler();

			/**
			 * Default copy constructor.
			 */
			EditorHandler(const EditorHandler &handler);

			/**
			 * Default assignment operator.
			 */
			const EditorHandler &operator=(const EditorHandler &handler);

			/**
			 * Checks all mouse button flags.
			 * @return Returns true if any flag is set.
			 */
			bool checkMouseFlags();

			/**
			 * Resets all mouse flags.
			 */
			void resetMouseFlags();

			static EditorHandler *mpEditorHandlerInstance;
			EditorGUI *mpEditorGUI;
			EditorFileHandler *mpEditorFileHandler;
			std::vector<EM *> mMessages;
			MouseMarker mMouseMarker;
			bool mActive;
			bool mFlagMouseLeftDown;
			bool mFlagMouseMiddleDown;
			bool mFlagMouseRightDown;
			bool mFlagMouseClip;
			long mMouseClipX;
			long mMouseClipY;
			EM *mpCreateObjectMessage;
			MapDataWorldPlane mPlaneData;
			std::vector<D3DXVECTOR4> mPlaneWeights;
	};
}

#endif