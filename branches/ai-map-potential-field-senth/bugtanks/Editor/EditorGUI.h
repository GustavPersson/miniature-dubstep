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

#ifndef EDITOR_GUI_H
#define EDITOR_GUI_H
#include "../GFX/GUIHandler.h"
#include "EditorGUIUniversalList.h"
#include "EditorGUIUtilityBar.h"

namespace editor
{
	class EditorGUI : public gfx::GUIHandler
	{
		public:
			/**
			 * Default constructor.
			 */
			EditorGUI(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight);

			/**
			 * Default destructor.
			 */
			virtual ~EditorGUI();

			/**
			 * Returns the name of the listener, as a string.
			 * @return Returns the name as a stdstring
			 */
			virtual const char *getName();

			/**
			 * Handles events.
			 * @param eventMessage The event that should be handled.
			 * @return Returns true if the event was consumed, and should not be propogated
			 * @return Returns false if the event was not consumed.
			 */
			virtual bool handleEvent(const logic::EventData &eventMessage);

			/**
			 * Shows all frame sprites.
			 */
			virtual void showAllFrames();

			/**
			 * Hides all frame sprites.
			 */
			virtual void hideAllFrames();

			/**
			 * Updates all GUI frames.
			 *  GFX must be locked.
			 * @param deltaTime Time difference since last frame.
			 */
			virtual void updateFrames(float deltaTime);

			/**
			 * Updates the content of the selection list.
			 */
			virtual void updateSelectionList();

			/**
			 * Retrieves a pointer to the utility bar.
			 * @return Returns mpUtilityBar.
			 */
			inline EditorGUIUtilityBar *getUtilityBar(){return mpUtilityBar;}

		protected:
			/**
			 * Default copy constructor.
			 */
			EditorGUI(const EditorGUI &GUI);

			/**
			 * Default assignment operator.
			 */
			const EditorGUI &operator=(const EditorGUI &GUI);

			/**
			 * Create all frames for the GUI.
			 */
			virtual void init();

		private:
			EditorGUIUniversalList *mpUniversalList;
			EditorGUIUtilityBar *mpUtilityBar;
	};
}

#endif