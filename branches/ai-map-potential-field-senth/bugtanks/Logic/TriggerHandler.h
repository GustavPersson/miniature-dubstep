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

#ifndef TRIGGER_HANDLER_H
#define TRIGGER_HANDLER_H
#include <fstream>
#include <stack>
#include "IEventManager.h"
#include "Triggers.h"

namespace logic
{
	/**
	 * Main interface to the trigger functionality.
	 */
	class TriggerHandler : public EventListener
	{
		public:
			/**
			 * Default constructor.
			 */
			TriggerHandler();

			/**
			 * Default copy constructor.
			 */
			TriggerHandler(const TriggerHandler &handler);

			/**
			 * Default destructor.
			 */
			virtual ~TriggerHandler();

			/**
			 * Default assignment operator.
			 */
			const TriggerHandler &operator=(const TriggerHandler &trigger);

			/**
			 * Returns the name of the listener, as a string.
			 * @return Returns the name as a stdstring
			 */
			virtual char const *getName(void);

			/**
			 * Handles events.
			 * @param mEvent The event that should be handled.
			 * @return Returns true if the event was consumed, and should not be propogated
			 * @return Returns false if the event was not consumed.
			 */
			virtual bool handleEvent(EventData const &mEvent);

			/**
			 * Updates non-thread-safe parts of the trigger logic.
			 */
			virtual void updateGFX();

			/**
			 * Loads all triggers from a map logic file.
			 * @param filePath Null-ended string containing the absolute/relative
			 *  filepath.
			 */
			void loadTriggers(const char *filePath);

			/**
			 * Loads all triggers from a map logic file.
			 * @param file Reference to the fstream containing the file.
			 */
			void loadTriggers(std::fstream &file);

			/**
			 * Adds a trigger to the appropiate list.
			 * @param pTrigger Pointer to a trigger.
			 */
			void addTrigger(TriggerBase *pTrigger);

			/**
			 * Removes (deletes) a trigger.
			 * @param pTrigger Pointer to a trigger.
			 */
			bool removeTrigger(TriggerBase *pTrigger);

			/**
			 * Removes (deletes) a trigger from the active list.
			 * @param index Index in the active list.
			 */
			bool removeActiveTrigger(unsigned int index);

			/**
			 * Removes (deletes) a trigger from the completed list.
			 * @param index Index in the completed list.
			 */
			bool removeCompletedTrigger(unsigned int index);

			/**
			 * Removes (deletes) all triggers from the active list.
			 */
			void removeAllActiveTriggers();

			/**
			 * Removes (deletes) all triggers from the completed list.
			 */
			void removeAllCompletedTriggers();

			/**
			 * Removes (deletes) all triggers from both lists.
			 */
			void removeAllTriggers();

			/**
			 * Shows all trigger's bounding volumes.
			 */
			void showAllTriggers();

			/**
			 * Hides all trigger's bounding volumes.
			 */
			void hideAllTriggers();

			/**
			 * Returns all non-null bounding volumes from active
			 *  triggers. (Note: since the bounding volumes must be
			 *  renderable, they might be derived from a base render-class
			 *  instead).
			 * @return A vector with pointers to the triggers' bounding volumes.
			 */
			inline const std::vector<gfx::RenderObject *> &getActiveBVList() const {return mActiveBVList;}

			/**
			 * Returns all non-null bounding volumes from completed
			 *  triggers. (Note: since the bounding volumes must be
			 *  renderable, they might be derived from a base render-class
			 *  instead).
			 * @return A vector with pointers to the triggers' bounding volumes.
			 */
			inline const std::vector<gfx::RenderObject *> &getCompletedBVList() const {return mCompletedBVList;}

		protected:

		private:
			/**
			 * Moves an activated trigger from the active to the completed list.
			 * @param pTrigger Pointer to a trigger.
			 */
			void completeTrigger(TriggerBase *pTrigger);

			/**
			 * Moves a trigger hierarchy to the completed list.
			 *  (Parent, children, all subchildren).
			 * @param pParent Pointer to the parent.
			 */
			void completeTriggerAndChildren(TriggerBase *pParent);

			/**
			 * Updates dependencies for all active triggers.
			 */
			void updateAllDependencies();

			/**
			 * Attempts to copy the mActiveList to the supplied trigger handler.
			 * @param handler Reference to the TriggerHandler to copy to.
			 */
			void copyActiveListTo(TriggerHandler &handler);

			/**
			 * Attempts to copy the mCompletedList to the supplied trigger handler.
			 * @param handler Reference to the TriggerHandler to copy to.
			 */
			void copyCompletedListTo(TriggerHandler &handler);

			/**
			 * Attempts to copy the mActiveList from the supplied trigger handler.
			 * @param handler Reference to the TriggerHandler to copy from.
			 */
			void copyActiveListFrom(const TriggerHandler &handler);

			/**
			 * Attempts to copy the mCompletedList from the supplied trigger handler.
			 * @param handler Const reference to the TriggerHandler to copy from.
			 */
			void copyCompletedListFrom(const TriggerHandler &handler);

			/**
			 * Creates all pending triggers from the mCreateTriggerStack.
			 */
			void createTriggers();

			std::vector<TriggerBase *> mActiveList;
			std::vector<TriggerBase *> mCompletedList;
			std::vector<TriggerBase *> mTransferList;
			std::vector<gfx::RenderObject *> mActiveBVList;
			std::vector<gfx::RenderObject *> mCompletedBVList;
			std::stack<int> mCreateTriggerStack;
	};
}

#endif