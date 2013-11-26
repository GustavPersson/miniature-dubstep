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

#include "TriggerBase.h"

using namespace logic;
using std::vector;

TriggerBase::TriggerBase() : mDependencyList()
{
	mpParent = NULL;
	mTriggerType = TriggerTypes::BASE;
	mTriggerID = -1;
	mTriggerName = "";
	mFlagActive = true;
	mFlagReady = false;
	mFlagWaiting = false;
	mFlagChild = false;
	mFlagRecurring = false;
	mOccuranceTime = 0.0f;
	mOccuranceTimer = 0.0f;
	mRequirement = 0;
	mSubRequirement = 0;
	mValue = 0;
	mSubValue = 0;
	mFloatValue = 0.0f;
	mFloatSubValue = 0.0f;
}

TriggerBase::TriggerBase(const TriggerBase &trigger) : mDependencyList(trigger.mDependencyList)
{
	mpParent = trigger.mpParent;
	mTriggerType = trigger.mTriggerType;
	mTriggerID = trigger.mTriggerID;
	mTriggerName = trigger.mTriggerName;
	mFlagActive = trigger.mFlagActive;
	mFlagReady = trigger.mFlagReady;
	mFlagWaiting = trigger.mFlagWaiting;
	mFlagChild = trigger.mFlagChild;
	mFlagRecurring = trigger.mFlagRecurring;
	mOccuranceTime = trigger.mOccuranceTime;
	mOccuranceTimer = trigger.mOccuranceTimer;
	mRequirement = trigger.mRequirement;
	mSubRequirement = trigger.mSubRequirement;
	mValue = trigger.mValue;
	mSubValue = trigger.mSubValue;
	mFloatValue = 0.0f;
	mFloatSubValue = 0.0f;
}

TriggerBase::~TriggerBase()
{

}

const TriggerBase &TriggerBase::operator=(const TriggerBase &trigger)
{
	if(this != &trigger)
	{
		mpParent = trigger.mpParent;
		mDependencyList = trigger.mDependencyList;
		mTriggerType = trigger.mTriggerType;
		mTriggerID = trigger.mTriggerID;
		mTriggerName = trigger.mTriggerName;
		mFlagActive = trigger.mFlagActive;
		mFlagReady = trigger.mFlagReady;
		mFlagWaiting = trigger.mFlagWaiting;
		mFlagChild = trigger.mFlagChild;
		mFlagRecurring = trigger.mFlagRecurring;
		mOccuranceTime = trigger.mOccuranceTime;
		mOccuranceTimer = trigger.mOccuranceTimer;
		mRequirement = trigger.mRequirement;
		mSubRequirement = trigger.mSubRequirement;
		mValue = trigger.mValue;
		mSubValue = trigger.mSubValue;
		mFloatValue = 0.0f;
		mFloatSubValue = 0.0f;
	}
	return *this;
}

TriggerBase *TriggerBase::clone()
{
	return NULL;
}

bool TriggerBase::handleEvent(EventData const &eventData)
{
	if(mFlagActive)
	{
		updateDependencies();
		updateTrigger(eventData);
	}
	return false;
}

bool TriggerBase::addDependency(TriggerBase *pChild, bool flagDependency)
{
	if(mDependencyList.addTriggerDependency(TriggerBase::castVoid(pChild), flagDependency))
	{
		pChild->setFlagChild(true);
		pChild->setParent(this);
		return true;
	}
	return false;
}

bool TriggerBase::addDependencyUnique(TriggerBase *pChild, bool flagDependency)
{
	if(!mDependencyList.containsDependency(TriggerBase::castVoid(pChild)))
	{
		if(mDependencyList.addTriggerDependency(TriggerBase::castVoid(pChild), flagDependency))
		{
			pChild->setFlagChild(true);
			pChild->setParent(this);
			return true;
		}
	}
	return false;
}

void TriggerBase::updateDependencies()
{
	const vector<TriggerDependency> &list = mDependencyList.getList();
	for(vector<TriggerDependency>::const_iterator listIter = list.begin();
		listIter != list.end(); ++listIter)
	{
		if(TriggerBase::castTrigger((*listIter).pTrigger)->isActive() != (*listIter).flagDependency)
		{
			mFlagWaiting = true;
			return;
		}
	}
	mFlagWaiting = false;
}

void TriggerBase::getTriggerHierarchy(std::vector<TriggerBase *> &hierarchyList)
{
	hierarchyList.push_back(this);
	const vector<TriggerDependency> &list = mDependencyList.getList();
	for(vector<TriggerDependency>::const_iterator listIter = list.begin();
		listIter != list.end(); ++listIter)
	{
		TriggerBase::castTrigger((*listIter).pTrigger)->getTriggerHierarchy(hierarchyList);
	}
}

void TriggerBase::setOccuranceTime(float occuranceTime)
{
	mOccuranceTime = occuranceTime;
	if(mOccuranceTime > 0)
	{
		mFlagRecurring = true;
	}
	else
	{
		mFlagRecurring = false;
	}
}

void *TriggerBase::castVoid(TriggerBase *pTrigger)
{
	return (void *)pTrigger;
}

TriggerBase *TriggerBase::castTrigger(void *pTrigger)
{
	return (TriggerBase *)pTrigger;
}

void TriggerBase::updateTrigger(EventData const &eventData)
{
	//Skip the requirement logic if the trigger already is ready.
	if(!mFlagReady)
	{
		switch(mRequirement)
		{
			case TriggerRequirements::TIME_SINCE_PROGRAM_START:
				break;

			case TriggerRequirements::TIME_SINCE_MAP_START:
				break;

			case TriggerRequirements::OBJECT_CREATED:
				if(eventData.getEventType().getHashValue() == EvtData_From_ActorCreated::mEventType.getHashValue())
				{
					const EvtData_From_ActorCreated &castEvent = (const EvtData_From_ActorCreated &)eventData;
					switch(mSubRequirement)
					{
						case TriggerSubRequirements::OBJECT_ID:
							if(castEvent.mId == mSubValue)
							{
								mFlagReady = true;
							}
							break;

						default:
							break;
					}
				}
				break;

			case TriggerRequirements::OBJECT_DESTROYED:
				if(eventData.getEventType().getHashValue() == EvtData_From_ActorDestroyed::mEventType.getHashValue())
				{
					const EvtData_From_ActorDestroyed &castEvent = (const EvtData_From_ActorDestroyed &)eventData;
					switch(mSubRequirement)
					{
						case TriggerSubRequirements::OBJECT_ID:
							if(castEvent.mId == mSubValue)
							{
								mFlagReady = true;
							}
							break;

						default:
							break;
					}
				}
				break;

			case TriggerRequirements::ENTERED_SOURCE_BV:
				break;

			case TriggerRequirements::EXITED_SOURCE_BV:
				break;

			case TriggerRequirements::ENTERED_DESTINATION_BV:
				break;

			case TriggerRequirements::EXITED_DESTINATION_BV:
				break;

			default:
				break;
		}
	}

	//Activate the trigger if it is ready (requirements met)
	//and not waiting for any child dependencies.
	if(mFlagReady && !mFlagWaiting)
	{
		activate();
	}
}

void TriggerBase::activate()
{
	DEBUG_MESSAGE("Trigger " << mTriggerName << " activated")
	mFlagActive = false;

	//Sends an empty event to the parent.
	if(mpParent)
	{
		//mpParent->handleEvent(EvtData_From_ActorCollision(0.0f));
	}
}