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

#ifndef EDITOR_GUI_TRIGGER_WINDOW_H
#define EDITOR_GUI_TRIGGER_WINDOW_H
#include "../GFX/GUIImage.h"
#include "../GFX/GUIText.h"
#include "../Logic/GUITextField.h"

namespace editor
{
	class EditorGUITriggerWindow : public gfx::GUIImage
	{
		public:
			/**
			 * Default constructor.
			 * @param pBaseInstance Pointer to the instance base of a sprite.
			 * @param clientWidth Width of the client area.
			 * @param clientHeight Height of the client area.
			 * @param parentDepth Depth level of the parent.
			 */
			EditorGUITriggerWindow(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

			/**
			 * Subframe constructor.
			 * @param pParent Pointer to the parent.
			 */
			EditorGUITriggerWindow(gfx::GUIFrame *pParent);

			/**
			 * Default copy constructor.
			 */
			EditorGUITriggerWindow(const EditorGUITriggerWindow &window);

			/**
			 * Default destructor.
			 */
			virtual ~EditorGUITriggerWindow();

			/**
			 * Default assignment operator.
			 */
			const EditorGUITriggerWindow &operator=(const EditorGUITriggerWindow &window);

			/**
			 * Updates the GUI frame and all sub frames.
			 * @param deltaTime Time difference since the last frame.
			 */
			virtual void update(float deltaTime);

			/**
			 * Sets the visibility flag.
			 * @param flag New value for mFlagVisible.
			 */
			virtual void setFlagVisible(bool flag);

		protected:
			class SelectWindow : public gfx::GUIImage
			{
				public:
					enum SelectMessages
					{
						SelectMessages_Close = 100,
						SelectMessages_Selection = 101,
						SelectMessages_Data = 10000
					};

					/**
					 * Default constructor.
					 * @param pBaseInstance Pointer to the instance base of a sprite.
					 * @param clientWidth Width of the client area.
					 * @param clientHeight Height of the client area.
					 * @param parentDepth Depth level of the parent.
					 */
					SelectWindow(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

					/**
					 * Subframe constructor.
					 * @param pParent Pointer to the parent.
					 */
					SelectWindow(gfx::GUIFrame *pParent);

					/**
					 * Default copy constructor.
					 */
					SelectWindow(const SelectWindow &window);

					/**
					 * Default destructor.
					 */
					virtual ~SelectWindow();

					/**
					 * Default assignment operator.
					 */
					const SelectWindow &operator=(const SelectWindow &window);

					/**
					 * Updates the GUI frame and all sub frames.
					 * @param deltaTime Time difference since the last frame.
					 */
					virtual void update(float deltaTime);

					/**
					 * Updates the selection buttons.
					 */
					virtual void updateButtons();

					/**
					 * Sets all trigger variables to -1.
					 */
					virtual void resetTriggerVariables();

					/**
					 * Sets the trigger type which this window will base its available selection on.
					 * @param type New value for mType.
					 */
					inline void setType(int type){mType = type;}

					/**
					 * Sets the trigger requirement which this window will base its available selection on.
					 * @param requirement New value for mRequirement.
					 */
					inline void setRequirement(int requirement){mRequirement = requirement;}

					/**
					 * Sets the trigger sub requirement which this window will base its available selection on.
					 * @param subrequirement New value for mSubRequirement.
					 */
					inline void setSubRequirement(int subrequirement){mSubRequirement = subrequirement;}

					/**
					 * Sets the trigger value which this window will base its available selection on.
					 * @param value New value for mValue.
					 */
					inline void setValue(int value){mValue = value;}

					/**
					 * Sets the trigger sub value which this window will base its available selection on.
					 * @param subvalue New value for mSubValue.
					 */
					inline void setSubValue(int subvalue){mSubValue = subvalue;}

					/**
					 * Retrieves the stored trigger type.
					 * @return Returns mType.
					 */
					inline int getType() const {return mType;}

					/**
					 * Retrieves the stored trigger requirement.
					 * @return Returns mRequirement.
					 */
					inline int getRequirement() const {return mRequirement;}

					/**
					 * Retrieves the stored trigger sub requirement.
					 * @return Returns mSubRequirement.
					 */
					inline int getSubRequirement() const {return mSubRequirement;}

					/**
					 * Retrieves the stored trigger value.
					 * @return Returns mValue.
					 */
					inline int getValue() const {return mValue;}

					/**
					 * Retrieves the stored trigger sub value.
					 * @return Returns mSubValue.
					 */
					inline int getSubValue() const {return mSubValue;}

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

					gfx::GUIText *mpTextTitle;
					logic::GUIButton *mpButtonClose;
					logic::GUIButton **mpButtons;
					int *mpButtonData;
					int mButtonCount;
					int mType;
					int mRequirement;
					int mSubRequirement;
					int mValue;
					int mSubValue;

				private:
			};

			class SelectType : public SelectWindow
			{
				public:
					/**
					 * Default constructor.
					 * @param pBaseInstance Pointer to the instance base of a sprite.
					 * @param clientWidth Width of the client area.
					 * @param clientHeight Height of the client area.
					 * @param parentDepth Depth level of the parent.
					 */
					SelectType(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

					/**
					 * Subframe constructor.
					 * @param pParent Pointer to the parent.
					 */
					SelectType(gfx::GUIFrame *pParent);

					/**
					 * Default copy constructor.
					 */
					SelectType(const SelectType &window);

					/**
					 * Default destructor.
					 */
					virtual ~SelectType();

					/**
					 * Default assignment operator.
					 */
					const SelectType &operator=(const SelectType &window);

					/**
					 * Updates the GUI frame and all sub frames.
					 * @param deltaTime Time difference since the last frame.
					 */
					virtual void update(float deltaTime);

					/**
					 * Updates the selection buttons.
					 */
					virtual void updateButtons();

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

			class SelectRequirement : public SelectWindow
			{
				public:
					/**
					 * Default constructor.
					 * @param pBaseInstance Pointer to the instance base of a sprite.
					 * @param clientWidth Width of the client area.
					 * @param clientHeight Height of the client area.
					 * @param parentDepth Depth level of the parent.
					 */
					SelectRequirement(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

					/**
					 * Subframe constructor.
					 * @param pParent Pointer to the parent.
					 */
					SelectRequirement(gfx::GUIFrame *pParent);

					/**
					 * Default copy constructor.
					 */
					SelectRequirement(const SelectRequirement &window);

					/**
					 * Default destructor.
					 */
					virtual ~SelectRequirement();

					/**
					 * Default assignment operator.
					 */
					const SelectRequirement &operator=(const SelectRequirement &window);

					/**
					 * Updates the GUI frame and all sub frames.
					 * @param deltaTime Time difference since the last frame.
					 */
					virtual void update(float deltaTime);

					/**
					 * Updates the selection buttons.
					 */
					virtual void updateButtons();

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

			class SelectSubRequirement : public SelectWindow
			{
				public:
					/**
					 * Default constructor.
					 * @param pBaseInstance Pointer to the instance base of a sprite.
					 * @param clientWidth Width of the client area.
					 * @param clientHeight Height of the client area.
					 * @param parentDepth Depth level of the parent.
					 */
					SelectSubRequirement(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

					/**
					 * Subframe constructor.
					 * @param pParent Pointer to the parent.
					 */
					SelectSubRequirement(gfx::GUIFrame *pParent);

					/**
					 * Default copy constructor.
					 */
					SelectSubRequirement(const SelectSubRequirement &window);

					/**
					 * Default destructor.
					 */
					virtual ~SelectSubRequirement();

					/**
					 * Default assignment operator.
					 */
					const SelectSubRequirement &operator=(const SelectSubRequirement &window);

					/**
					 * Updates the GUI frame and all sub frames.
					 * @param deltaTime Time difference since the last frame.
					 */
					virtual void update(float deltaTime);

					/**
					 * Updates the selection buttons.
					 */
					virtual void updateButtons();

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

			class SelectValue : public SelectWindow
			{
				public:
					/**
					 * Default constructor.
					 * @param pBaseInstance Pointer to the instance base of a sprite.
					 * @param clientWidth Width of the client area.
					 * @param clientHeight Height of the client area.
					 * @param parentDepth Depth level of the parent.
					 */
					SelectValue(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

					/**
					 * Subframe constructor.
					 * @param pParent Pointer to the parent.
					 */
					SelectValue(gfx::GUIFrame *pParent);

					/**
					 * Default copy constructor.
					 */
					SelectValue(const SelectValue &window);

					/**
					 * Default destructor.
					 */
					virtual ~SelectValue();

					/**
					 * Default assignment operator.
					 */
					const SelectValue &operator=(const SelectValue &window);

					/**
					 * Updates the GUI frame and all sub frames.
					 * @param deltaTime Time difference since the last frame.
					 */
					virtual void update(float deltaTime);

					/**
					 * Updates the selection buttons.
					 */
					virtual void updateButtons();

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

			class SelectSubValue : public SelectWindow
			{
				public:
					/**
					 * Default constructor.
					 * @param pBaseInstance Pointer to the instance base of a sprite.
					 * @param clientWidth Width of the client area.
					 * @param clientHeight Height of the client area.
					 * @param parentDepth Depth level of the parent.
					 */
					SelectSubValue(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

					/**
					 * Subframe constructor.
					 * @param pParent Pointer to the parent.
					 */
					SelectSubValue(gfx::GUIFrame *pParent);

					/**
					 * Default copy constructor.
					 */
					SelectSubValue(const SelectSubValue &window);

					/**
					 * Default destructor.
					 */
					virtual ~SelectSubValue();

					/**
					 * Default assignment operator.
					 */
					const SelectSubValue &operator=(const SelectSubValue &window);

					/**
					 * Updates the GUI frame and all sub frames.
					 * @param deltaTime Time difference since the last frame.
					 */
					virtual void update(float deltaTime);

					/**
					 * Updates the selection buttons.
					 */
					virtual void updateButtons();

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
			 * Hides/shows all buttons on this frame.
			 * @param visible Visibility state of the buttons.
			 */
			void switchWindow(bool visible);

			/**
			 * Resets all trigger variables to -1.
			 */
			void resetTriggerVariables();

			/**
			 * Updates all trigger variable text fields.
			 */
			void updateFieldAll();

			/**
			 * Updates the trigger type text field.
			 */
			void updateFieldType();

			/**
			 * Updates the trigger requirement text field.
			 */
			void updateFieldRequirement();

			/**
			 * Updates the trigger sub requirement text field.
			 */
			void updateFieldSubRequirement();

			/**
			 * Updates the trigger value text field.
			 */
			void updateFieldValue();

			/**
			 * Updates the trigger sub value text field.
			 */
			void updateFieldSubValue();

			/**
			 * Retrieves data from the text fields and sends a create trigger event.
			 * @return Returns true if an event was successfully sent.
			 */
			bool createTrigger();

		private:
			gfx::GUIText *mpTextTitle;
			SelectType *mpSelectType;
			SelectRequirement *mpSelectRequirement;
			SelectSubRequirement *mpSelectSubRequirement;
			SelectValue *mpSelectValue;
			SelectSubValue *mpSelectSubValue;
			logic::GUIButton *mpButtonType;
			logic::GUIButton *mpButtonRequirement;
			logic::GUIButton *mpButtonSubRequirement;
			logic::GUIButton *mpButtonValue;
			logic::GUIButton *mpButtonSubValue;
			logic::GUITextField *mpFieldType;
			logic::GUITextField *mpFieldRequirement;
			logic::GUITextField *mpFieldSubRequirement;
			logic::GUITextField *mpFieldValue;
			logic::GUITextField *mpFieldSubValue;
			logic::GUITextField *mpFieldMisc;
			logic::GUIButton *mpTextMisc;
			logic::GUIButton *mpButtonCreate;
			logic::GUIButton *mpButtonClose;
			int mTriggerType;
			int mTriggerRequirement;
			int mTriggerSubRequirement;
			int mTriggerValue;
			int mTriggerSubValue;

	};
}

#endif