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

#ifndef EDITOR_GUI_UNIVERSAL_LIST_H
#define EDITOR_GUI_UNIVERSAL_LIST_H
#include "../Logic/GUIList.h"
#include "../Logic/GUIButton.h"

namespace editor
{
	class EditorGUIUniversalList : public gfx::GUIFrame
	{
		public:
			/**
			 * Default constructor.
			 * @param pBaseInstance Pointer to the instance base of a sprite.
			 * @param clientWidth Width of the client area.
			 * @param clientHeight Height of the client area.
			 * @param parentDepth Depth level of the parent.
			 */
			EditorGUIUniversalList(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

			/**
			 * Subframe constructor.
			 * @param pParent Pointer to the parent.
			 */
			EditorGUIUniversalList(gfx::GUIFrame *pParent);

			/**
			 * Default copy constructor.
			 */
			EditorGUIUniversalList(const EditorGUIUniversalList &list);

			/**
			 * Default destructor.
			 */
			virtual ~EditorGUIUniversalList();

			/**
			 * Default assignment operator.
			 */
			const EditorGUIUniversalList &operator=(const EditorGUIUniversalList &list);

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
			 * Adds a content object to the model list.
			 * @param objectID ID of the content.
			 * @param text Text of the content.
			 */
			virtual void addModelContent(int objectID, const char *text);

			/**
			 * Adds a content object to the enemy list.
			 * @param objectID ID of the content.
			 * @param text Text of the content.
			 */
			virtual void addEnemyContent(int objectID, const char *text);

			/**
			 * Removes all content from the model selection list.
			 */
			virtual void removeAllModelContent();

			/**
			 * Removes all content from the enemy selection list.
			 */
			virtual void removeAllEnemyContent();

		protected:
			struct ObjectSelection
			{
				int mObjectID;
				std::string mText;

				ObjectSelection() : mText("")
				{
					mObjectID = -1;
				}

				~ObjectSelection(){}
			};

			class GUIListWrapper : public logic::GUIList
			{
				public:
					/**
					 * Default constructor.
					 * @param pBaseInstance Pointer to the instance base of a sprite.
					 * @param clientWidth Width of the client area.
					 * @param clientHeight Height of the client area.
					 * @param parentDepth Depth level of the parent.
					 */
					GUIListWrapper(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

					/**
					 * Subframe constructor.
					 * @param pParent Pointer to the parent.
					 */
					GUIListWrapper(gfx::GUIFrame *pParent);

					/**
					 * Default copy constructor.
					 */
					GUIListWrapper(const GUIListWrapper &list);

					/**
					 * Default destructor.
					 */
					virtual ~GUIListWrapper();

					/**
					 * Default assignment operator.
					 */
					const GUIListWrapper &operator=(const GUIListWrapper &list);

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
			virtual void notify(int message, gfx::GUIFrame *pSender);

			/**
			 * Updates the content of the selection list.
			 */
			virtual void updateListContent();

			/**
			 * Converts the provided selection text to the corresponding ObjectSelection.
			 * @param selectionText Text string of the selected list content.
			 * @return Returns a pointer to the stored ObjectSelection.
			 */
			EditorGUIUniversalList::ObjectSelection *getObjectSelection(const std::string &selectionText);

		private:
			GUIListWrapper *mpSelectionList;
			logic::GUIButton *mpButtonContentModel;
			logic::GUIButton *mpButtonContentEnemy;
			std::vector<ObjectSelection> *mpCurrentList;
			std::vector<ObjectSelection> mModelList;
			std::vector<ObjectSelection> mEnemyList;
	};
}

#endif