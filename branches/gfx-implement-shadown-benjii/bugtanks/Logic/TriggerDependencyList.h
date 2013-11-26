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

#ifndef TRIGGER_DEPENDENCY_LIST_H
#define TRIGGER_DEPENDENCY_LIST_H
#include <vector>
#include "../Utilities/Macros.h"

namespace logic
{
	/**
	 * Storage struct for trigger dependencies.
	 */
	struct TriggerDependency
	{
		void *pTrigger;
		bool flagDependency;

		/**
		 * Default constructor.
		 */
		TriggerDependency();

		/**
		 * Constructor.
		 * @param pTrigger Void-pointer to a TriggerBase object.
		 * @param flagDependency Flag depicting the trigger's dependency.
		 */
		TriggerDependency(void *pTrigger, bool flagDependency);

		/**
		 * Default copy constructor.
		 */
		TriggerDependency(const TriggerDependency &dependency);

		/**
		 * Default destructor.
		 */
		~TriggerDependency();

		/**
		 * Default assignment operator.
		 */
		const TriggerDependency &operator=(const TriggerDependency &dependency);
	};

	/**
	 * Simple class containing a list with TriggerBase-dependencies stored
	 * in void *-format.
	 */
	class TriggerDependencyList
	{
		public:
			/**
			 * Default constructor.
			 */
			TriggerDependencyList();

			/**
			 * Default copy constructor.
			 */
			TriggerDependencyList(const TriggerDependencyList &list);

			/**
			 * Default destructor.
			 */
			virtual ~TriggerDependencyList();

			/**
			 * Default assignment operator.
			 */
			const TriggerDependencyList &operator=(const TriggerDependencyList &list);

			/**
			 * Checks if the list contains a trigger dependency.
			 * @param dependency TriggerDependency struct to look for.
			 *  Only checks TriggerDependency::pTrigger.
			 * @return True if found.
			 */
			bool containsDependency(TriggerDependency dependency) const;

			/**
			 * Checks if the list contains a trigger dependency.
			 * @param pTrigger Void-pointer to the TriggerBase object to look for.
			 * @return True if found.
			 */
			bool containsDependency(void *pTrigger);

			/**
			 * Adds a trigger dependency to the list.
			 * @param dependency TriggerDependency struct to add.
			 * @return True if successful.
			 */
			bool addTriggerDependency(TriggerDependency dependency);

			/**
			 * Creates a TriggerDependency struct and adds to the list.
			 * @param pTrigger Void-pointer to a TriggerBase object.
			 * @param flagDependency Flag depicting the trigger's dependency.
			 * @return True if successful.
			 */
			bool addTriggerDependency(void *pTrigger, bool flagDependency);

			/**
			 * Attemps to remove all TriggerDependencies that are similar to
			 *  the supplied one from the list.
			 * @param dependency (Similar) TriggerDependency struct to remove.
			 *  Only checks TriggerDependency::pTrigger.
			 * @return True if any are removed.
			 */
			bool removeTriggerDepedency(TriggerDependency dependency);

			/**
			 * Attemps to remove all TriggerDependencies which contain the supplied 
			 *  pTrigger-pointer from the list.
			 * @param pTrigger Void-pointer to a TriggerBase object to look for.
			 * @return True if any are removed.
			 */
			bool removeTriggerDependency(void *pTrigger);

			/**
			 * Attemps to remove all TriggerDependencies with the same flagDependency
			 *  as the supplied one.
			 * @param dependencyFlag Flag depicting the triggers' dependencies
			 *  to look for.
			 * @return True if any are removed.
			 */
			bool removeTriggerDependency(bool dependencyFlag);

			/**
			 * Attemps to remove all TriggerDependencies with the same pTrigger-pointer
			 *  and flagDependency as the supplied ones.
			 * @param pTrigger Void-pointer to a TriggerBase object to look for.
			 * @param flagDependency Flag depicting the triggers' dependencies
			 *  to look for.
			 * @return True if any are removed.
			 */
			bool removeTriggerDependency(void *pTrigger, bool flagDependency);

			/**
			 * Attemps to remove the TriggerDependency at the supplied index.
			 * @param index Index in the dependencyList.
			 * @return True if removed.
			 */
			bool removeTriggerDependecy(unsigned int index);

			/**
			 * Removes all TriggerDependencies from the list.
			 */
			void removeAll();

			/**
			 * Retrieves the size of the list.
			 */
			inline const unsigned int getSize() const {return mDependencyList.size();}

			/**
			 * Retrieves a const reference to the list.
			 */
			inline const std::vector<TriggerDependency> &getList() const {return mDependencyList;}

		protected:

		private:
			/**
			 * Attempts to copy the dependencyList to the supplied list.
			 * @param list Reference to the TriggerDependencyList to copy to.
			 */
			void copyDependencyListTo(TriggerDependencyList &list);

			/**
			 * Attempts to copy the dependencyList from the supplied list.
			 * @param list Const reference to the TriggerDependencyList to copy from.
			 */
			void copyDependencyListFrom(const TriggerDependencyList &list);

			std::vector<TriggerDependency> mDependencyList;
	};
}

#endif