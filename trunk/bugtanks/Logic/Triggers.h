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

#ifndef TRIGGERS_H
#define TRIGGERS_H
#include "TriggerBase.h"

namespace logic
{
	class TriggerEndMap : public TriggerBase
	{
		public:
			/**
			 * Default constructor.
			 */
			TriggerEndMap();

			/**
			 * Default copy constructor.
			 */
			TriggerEndMap(const TriggerEndMap &trigger);

			/**
			 * Default destructor.
			 */
			virtual ~TriggerEndMap();

			/**
			 * Default assignment operator.
			 */
			const TriggerEndMap &operator=(const TriggerEndMap &trigger);

			/**
			 * Clones the current trigger.
			 * Returns null for TriggerBase::clone()-calls.
			 * @return A new instance of the object.
			 */
			virtual TriggerBase *clone();

			/**
			 * Checks an event.
			 * @param eventData Event to check.
			 * @return True if event is consumed.
			 */
			virtual bool handleEvent(EventData const &eventData);

		protected:
			/**
			 * Sets flagActive to false and sends out a predefined event.
			 */
			virtual void activate();

		private:
	};

	class TriggerCreateUnit : public TriggerBase
	{
		public:
			/**
			 * Default constructor.
			 */
			TriggerCreateUnit();

			/**
			 * Default copy constructor.
			 */
			TriggerCreateUnit(const TriggerCreateUnit &trigger);

			/**
			 * Default destructor.
			 */
			virtual ~TriggerCreateUnit();

			/**
			 * Default assignment operator.
			 */
			const TriggerCreateUnit &operator=(const TriggerCreateUnit &trigger);

			/**
			 * Clones the current trigger.
			 * Returns null for TriggerBase::clone()-calls.
			 * @return A new instance of the object.
			 */
			virtual TriggerBase *clone();

			/**
			 * Checks an event.
			 * @param eventData Event to check.
			 * @return True if event is consumed.
			 */
			virtual bool handleEvent(EventData const &eventData);

		protected:
			/**
			 * Sets flagActive to false and sends out a predefined event.
			 */
			virtual void activate();

		private:
	};

	class TriggerDestroyUnit : public TriggerBase
	{
		public:
			/**
			 * Default constructor.
			 */
			TriggerDestroyUnit();

			/**
			 * Default copy constructor.
			 */
			TriggerDestroyUnit(const TriggerDestroyUnit &trigger);

			/**
			 * Default destructor.
			 */
			virtual ~TriggerDestroyUnit();

			/**
			 * Default assignment operator.
			 */
			const TriggerDestroyUnit &operator=(const TriggerDestroyUnit &trigger);

			/**
			 * Clones the current trigger.
			 * Returns null for TriggerBase::clone()-calls.
			 * @return A new instance of the object.
			 */
			virtual TriggerBase *clone();

			/**
			 * Checks an event.
			 * @param eventData Event to check.
			 * @return True if event is consumed.
			 */
			virtual bool handleEvent(EventData const &eventData);

		protected:
			/**
			 * Sets flagActive to false and sends out a predefined event.
			 */
			virtual void activate();

		private:
	};
}

#endif