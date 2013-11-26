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

#ifndef TRIGGER_BASE_H
#define TRIGGER_BASE_H
#include "TriggerDependencyList.h"
#include "Events.h"
#include "../GFX/TransformObject.h"

namespace logic
{
	/**
	 * Abstract base class for the different logical triggers.
	 */
	class TriggerBase
	{
		public:
			enum TriggerTypes
			{
				TriggerTypes_Unknown,
				TriggerTypes_Base,
				TriggerTypes_StartMap,
				TriggerTypes_EndMap,
				TriggerTypes_CreateUnit,
				TriggerTypes_DestroyUnit
			};

			enum TriggerRequirements
			{
				TriggerRequirements_None,
				TriggerRequirements_TimeSinceMapStart,
				TriggerRequirements_TimeSinceProgramStart,
				TriggerRequirements_EnteredSourceBV,
				TriggerRequirements_ExitedSourceBV,
				TriggerRequirements_EnteredDestinationBV,
				TriggerRequirements_ExitedDestinationBV,
				TriggerRequirements_ObjectCreated,
				TriggerRequirements_ObjectDestroyed
			};

			enum TriggerSubRequirements
			{
				TriggerSubRequirements_None,
				TriggerSubRequirements_ObjectID
			};

			/**
			 * Default constructor.
			 */
			TriggerBase();

			/**
			 * Default copy constructor.
			 */
			TriggerBase(const TriggerBase &trigger);

			/**
			 * Default destructor.
			 */
			virtual ~TriggerBase() = 0;

			/**
			 * Default assignment operator.
			 */
			const TriggerBase &operator=(const TriggerBase &trigger);

			/**
			 * Clones the current trigger.
			 * Returns null for TriggerBase::clone()-calls.
			 * @return A new instance of the object.
			 */
			virtual TriggerBase *clone() = 0;

			/**
			 * Checks an event.
			 * @param eventData Event to check.
			 * @return True if event is consumed.
			 */
			virtual bool handleEvent(EventData const &eventData) = 0;

			/**
			 * Adds a trigger dependency.
			 * @param pChild Pointer to the child trigger.
			 * @param flagDependency Requested active state for the dependency to be true.
			 * @return True if successful.
			 */
			bool addDependency(TriggerBase *pChild, bool flagDependency);

			/**
			 * Adds a trigger dependency if the dependency list doesn't already contain the
			 *  supplied TriggerBase pointer.
			 * @param pChild Pointer to the child trigger.
			 * @param flagDependency Requested active state for the dependency to be true.
			 * @return True if successful.
			 */
			bool addDependencyUnique(TriggerBase *pChild, bool flagDependency);

			/**
			 * Goes through all dependencies and updates the flagWaiting accordingly.
			 */
			virtual void updateDependencies();

			/**
			 * Shows the source bounding volume.
			 */
			virtual void showSourceBV();

			/**
			 * Shows the destination bounding volume.
			 */
			virtual void showDestinationBV();

			/**
			 * Hides the source bounding volume.
			 */
			virtual void hideSourceBV();

			/**
			 * Hides the destination bounding volume.
			 */
			virtual void hideDestinationBV();

			/**
			 * Returns a const reference to the dependency list.
			 * @return Reference to mDependencyList.
			 */
			inline const TriggerDependencyList &getDependencyList() const {return mDependencyList;}

			/**
			 * Adds the complete trigger hierarchy (parent, children, all levels of subchildren) to a vector.
			 * @param hierarchyList Reference to a vector to which the triggers should be added.
			 */
			void getTriggerHierarchy(std::vector<TriggerBase *> &hierarchyList);

			/**
			 * Retrieves the source bounding volume.
			 * @return Returns mpSourceBV.
			 */
			inline gfx::TransformObject *getSourceBV(){return mpSourceBV;}

			/**
			 * Retrieves the destination bounding volume.
			 * @return Returns mpDestinationBV.
			 */
			inline gfx::TransformObject *getDestinationBV(){return mpDestinationBV;}

			/**
			 * Retrieves the triggerType.
			 * @return mTriggerType.
			 */
			inline int getTriggerType() const {return mTriggerType;}

			/**
			 * Retrieves the triggerID.
			 * @return mTriggerID.
			 */
			inline int getTriggerID() const {return mTriggerID;}

			/**
			 * Retrieves the triggerName.
			 * @return mTriggerName.
			 */
			inline const char *getTriggerName() const {return mTriggerName;}

			/**
			 * Retrieves the flagActive.
			 * @return mFlagActive;
			 */
			inline bool isActive() const {return mFlagActive;}

			/**
			 * Retrieves the flagReady.
			 * @return mFlagReady.
			 */
			inline bool isReady() const {return mFlagReady;}

			/**
			 * Retrieves the flagWaiting.
			 * @return mFlagWaiting.
			 */
			inline bool isWaiting() const {return mFlagWaiting;}

			/**
			 * Retrieves the flagChild.
			 * @return mFlagChild.
			 */
			inline bool isChild() const {return mFlagChild;}

			/**
			 * Retrieves the flagRecurring.
			 * @return mFlagRecurring.
			 */
			inline bool isRecurring() const {return mFlagRecurring;}

			/**
			 * Sets the triggerID.
			 * @param triggerID ID to set.
			 */
			inline void setTriggerID(int triggerID){mTriggerID = triggerID;}

			/**
			 * Sets the triggerName.
			 * @param triggerName Name to set.
			 */
			inline void setTriggerName(const char *triggerName){mTriggerName = triggerName;}

			/**
			 * Sets the flagChild.
			 * @param flagChild Flag to set.
			 */
			inline void setFlagChild(bool flagChild){mFlagChild = flagChild;}

			/**
			 * Sets the occurance time. Updates the flagRecurring to the appropiate value.
			 * @param occuranceTime Time in seconds between each trigger occurance.
			 */
			void setOccuranceTime(float occuranceTime);

			/**
			 * Sets the requirement.
			 * @param requirement Requirement to set.
			 */
			inline void setRequirement(int requirement){mRequirement = requirement;}

			/**
			 * Sets the sub requirement.
			 * @param subRequirement Sub requirement to set.
			 */
			inline void setSubRequirement(int subRequirement){mSubRequirement = subRequirement;}

			/**
			 * Sets the value to the main requirement.
			 * @param value Value to set.
			 */
			inline void setValue(int value){mValue = value;}

			/**
			 * Sets the value to the sub requirement.
			 * @param subValue Sub value to set.
			 */
			inline void setSubValue(int subValue){mSubValue = subValue;}

			/**
			 * Sets the float value to the main requirement.
			 * @param value Float value to set.
			 */
			inline void setFloatValue(float value){mFloatValue = value;}

			/**
			 * Sets the float value to the sub requirement.
			 * @param subValue Float sub value to set.
			 */
			inline void setFloatSubValue(float subValue){mFloatSubValue = subValue;}

			/**
			 * Static function to cast a TriggerBase * to void *.
			 *  Should only be used when sending pointers to TriggerDependencyList
			 *  in order to minimize casts from other types.
			 * @param pTrigger Pointer to a TriggerBase.
			 * @return Pointer to a TriggerBase cast into void *.
			 */
			static void *castVoid(TriggerBase *pTrigger);

			/**
			 * Static function to cast a void * to TriggerBase *.
			 *  Should only be used when retrieving pointers from TriggerDependencyList
			 *  in order to minimize casts from other types.
			 * @param pTrigger Pointer to a TriggerBase cast into void *.
			 * @return Pointer to a TriggerBase.
			 */
			static TriggerBase *castTrigger(void *pTrigger);

		protected:
			/**
			 * Sets the parent.
			 * @param pParent Pointer to the parent.
			 */
			inline void setParent(TriggerBase *pParent){mpParent = pParent;}

			/**
			 * Sets the trigger type.
			 * @param type Type of the trigger.
			 */
			inline void setTriggerType(int type){mTriggerType = type;}

			/**
			 * Checks trigger requirements and activates it if it's ready.
			 * @param eventData Reference to the event.
			 */
			virtual void updateTrigger(EventData const &eventData);

			/**
			 * Sets flagActive to false and sends out a predefined event.
			 */
			virtual void activate() = 0;

			/**
			 * Retrieves the activation value of the trigger.
			 * @return Returns mValue.
			 */
			inline int getValue() const {return mValue;}

			/**
			 * Retrieves the activation sub value of the trigger.
			 * @return Returns mSubValue.
			 */
			inline int getSubValue() const {return mSubValue;}

		private:
			TriggerDependencyList mDependencyList;
			TriggerBase *mpParent;
			gfx::TransformObject *mpSourceBV;
			gfx::TransformObject *mpDestinationBV;
			int mTriggerType;
			int mTriggerID;
			const char *mTriggerName;
			bool mFlagActive;						//Active or completed trigger.
			bool mFlagReady;						//True if ready to activate.
			bool mFlagWaiting;						//True if waiting for children to activate.
			bool mFlagChild;						//True if the trigger is a child.
			bool mFlagRecurring;					//True if the trigger is recurring.
			float mOccuranceTime;
			float mOccuranceTimer;
			int mRequirement;
			int mSubRequirement;
			int mValue;
			int mSubValue;
			float mFloatValue;
			float mFloatSubValue;
	};
}

#endif