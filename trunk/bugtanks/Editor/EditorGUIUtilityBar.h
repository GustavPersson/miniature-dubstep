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

#ifndef EDITOR_GUI_UTILITY_BAR_H
#define EDITOR_GUI_UTILITY_BAR_H
#include "../GFX/GUIImage.h"
#include "../GFX/GUIText.h"
#include "../Logic/GUITextField.h"

namespace editor
{
	class EditorGUIUtilityBar : public gfx::GUIImage
	{
		public:
			/**
			 * Default constructor.
			 * @param pBaseInstance Pointer to the instance base of a sprite.
			 * @param clientWidth Width of the client area.
			 * @param clientHeight Height of the client area.
			 * @param parentDepth Depth level of the parent.
			 */
			EditorGUIUtilityBar(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

			/**
			 * Subframe constructor.
			 * @param pParent Pointer to the parent.
			 */
			EditorGUIUtilityBar(gfx::GUIFrame *pParent);

			/**
			 * Default copy constructor.
			 */
			EditorGUIUtilityBar(const EditorGUIUtilityBar &bar);

			/**
			 * Default destructor.
			 */
			virtual ~EditorGUIUtilityBar();

			/**
			 * Default assignment operator.
			 */
			const EditorGUIUtilityBar &operator=(const EditorGUIUtilityBar &bar);

			/**
			 * Updates the GUI frame and all sub frames.
			 * @param deltaTime Time difference since the last frame.
			 */
			virtual void update(float deltaTime);

			/**
			 * Retrieves the user supplied map name from the utility bar.
			 * @param name Outgoing reference to the name.
			 */
			void getMapName(std::string &name);

			/**
			 * Sets a pointer to the trigger creation window.
			 * @param pTriggerWindow Pointer to the trigger window.
			 */
			inline void setTriggerWindow(gfx::GUIFrame *pTriggerWindow){mpTriggerWindow = pTriggerWindow;}

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
			virtual void notify(int message, gfx::GUIFrame *pSender);

		private:
			logic::GUITextField *mpMapName;
			gfx::GUIText *mpTextMapName;
			gfx::GUIText *mpTextInstructions;
			logic::GUIButton *mpButtonSaveMap;
			logic::GUIButton *mpButtonLoadMap;
			logic::GUIButton *mpButtonCreateTrigger;
			logic::GUIButton *mpButtonShowTriggers;
			logic::GUIButton *mpButtonHideTriggers;
			gfx::GUIFrame *mpTriggerWindow;
	};
}

#endif