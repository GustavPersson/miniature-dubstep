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
using namespace gfx;
using std::vector;

TriggerBase::TriggerBase() : mDependencyList()
{
	mpParent = NULL;

	mpSourceBV = DX10Module::getInstance()->createObjectInstanced("TriggerObject");
	if(!mpSourceBV)
	{
		ERROR_MESSAGE("TriggerObject not loaded. Please load an object type with this name.")
	}
	mpSourceBV->setID(KEY_TRIGGER.convertToGfxId());
	mpSourceBV->setActor(this);
	mpSourceBV->setColor(D3DXCOLOR(0.1f, 0.9f, 0.1f, 0.5f));

	mpDestinationBV = DX10Module::getInstance()->createObjectInstanced("TriggerObject");
	if(!mpDestinationBV)
	{
		ERROR_MESSAGE("TriggerObject not loaded. Please load an object type with this name.")
	}
	mpDestinationBV->setID(KEY_TRIGGER.convertToGfxId());
	mpDestinationBV->setActor(this);
	mpDestinationBV->setColor(D3DXCOLOR(0.9f, 0.1f, 0.1f, 0.5f));

	mTriggerType = TriggerBase::TriggerTypes_Base;
	mTriggerID = -1;
	mTriggerName = "";
	mFlagActive = true;
	mFlagReady = false;
	mFlagWaiting = false;
	mFlagChild = false;
	mFlagRecurring = false;
	mOccuranceTime = 0.0f;
	mOccuranceTimer = 0.0f;
	mRequirement = TriggerBase::TriggerRequirements_None;
	mSubRequirement = TriggerBase::TriggerSubRequirements_None;
	mValue = 0;
	mSubValue = 0;
	mFloatValue = 0.0f;
	mFloatSubValue = 0.0f;
}

TriggerBase::TriggerBase(const TriggerBase &trigger) : mDependencyList(trigger.mDependencyList)
{
	mpParent = trigger.mpParent;
	mpSourceBV = trigger.mpSourceBV;
	mpDestinationBV = trigger.mpDestinationBV;
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
		mpSourceBV = trigger.mpSourceBV;
		mpDestinationBV = trigger.mpDestinationBV;
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

void TriggerBase::showSourceBV()
{
	mpSourceBV->setFlagRender(true);
}

void TriggerBase::showDestinationBV()
{
	mpDestinationBV->setFlagRender(true);
}

void TriggerBase::hideSourceBV()
{
	mpSourceBV->setFlagRender(false);
}

void TriggerBase::hideDestinationBV()
{
	mpDestinationBV->setFlagRender(false);
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
			case TriggerBase::TriggerRequirements_TimeSinceMapStart:
				break;

			case TriggerBase::TriggerRequirements_TimeSinceProgramStart:
				break;

			case TriggerBase::TriggerRequirements_ObjectCreated:
				if(eventData.getEventType() == EvtData_From_ActorCreated::msEventType)
				{
					const EvtData_From_ActorCreated &castEvent = (const EvtData_From_ActorCreated &)eventData;
					switch(mSubRequirement)
					{
						case TriggerBase::TriggerSubRequirements_ObjectID:
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

			case TriggerBase::TriggerRequirements_ObjectDestroyed:
				if(eventData.getEventType() == EvtData_From_ActorDestroyed::msEventType)
				{
					const EvtData_From_ActorDestroyed &castEvent = (const EvtData_From_ActorDestroyed &)eventData;
					switch(mSubRequirement)
					{
						case TriggerBase::TriggerSubRequirements_ObjectID:
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

			case TriggerBase::TriggerRequirements_EnteredSourceBV:
				if(eventData.getEventType() == EvtData_To_TriggerCollision::msEventType)
				{
					const EvtData_To_TriggerCollision &castEvent = (const EvtData_To_TriggerCollision &)eventData;
					//Check if the bounding volume provided by the event corresponds with the trigger's.
					if(castEvent.mpBoundingVolume == mpSourceBV->getBoundingVolume())
					{
						mFlagReady = true;
					}
				}
				break;

			case TriggerBase::TriggerRequirements_ExitedSourceBV:
				break;

			case TriggerBase::TriggerRequirements_EnteredDestinationBV:
				break;

			case TriggerBase::TriggerRequirements_ExitedDestinationBV:
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
	DEBUG_MESSAGE(LEVEL_MEDIUM, "Trigger " << mTriggerName << " activated")
	mFlagActive = false;

	//Sends an empty event to the parent.
	if(mpParent)
	{
		//mpParent->handleEvent(EvtData_From_ActorCollision(0.0f));
	}
}