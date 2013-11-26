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

#include "TriggerHandler.h"

using namespace logic;
using namespace gfx;
using std::vector;
using std::stack;
using std::fstream;

TriggerHandler::TriggerHandler() : EventListener(), mActiveList(), mCompletedList(),
	mTransferList(), mActiveBVList(), mCompletedBVList(), mCreateTriggerStack()
{
	//Register events.
	EventListenerPtr handlerPtr = boost::shared_ptr<TriggerHandler>(this);
	safeAddListener(handlerPtr, EvtData_To_CreateTrigger::msEventType);
	safeAddListener(handlerPtr, EvtData_To_ShowTriggers::msEventType);
	safeAddListener(handlerPtr, EvtData_To_HideTriggers::msEventType);
	safeAddListener(handlerPtr, EvtData_To_TriggerCollision::msEventType);
}

TriggerHandler::TriggerHandler(const TriggerHandler &handler) : EventListener(), mActiveList(),
	mCompletedList(), mTransferList(), mActiveBVList(), mCompletedBVList(), mCreateTriggerStack()
{
	copyActiveListFrom(handler);
	copyCompletedListFrom(handler);

	//Register events.
	EventListenerPtr handlerPtr = boost::shared_ptr<TriggerHandler>(this);
	safeAddListener(handlerPtr, EvtData_To_CreateTrigger::msEventType);
	safeAddListener(handlerPtr, EvtData_To_ShowTriggers::msEventType);
	safeAddListener(handlerPtr, EvtData_To_HideTriggers::msEventType);
	safeAddListener(handlerPtr, EvtData_To_TriggerCollision::msEventType);
}

TriggerHandler::~TriggerHandler()
{
	removeAllTriggers();
}

const TriggerHandler &TriggerHandler::operator=(const TriggerHandler &handler)
{
	if(this != &handler)
	{
		copyActiveListFrom(handler);
		copyCompletedListFrom(handler);
	}
	return *this;
}

char const *TriggerHandler::getName(void)
{
	return "TriggerHandler";
}

bool TriggerHandler::handleEvent(EventData const &mEvent)
{
	bool dispatchToTriggers = true;
	if(mEvent.getEventType() == EvtData_To_CreateTrigger::msEventType)
	{
		const EvtData_To_CreateTrigger &translatedEvent = (const EvtData_To_CreateTrigger &)mEvent;
		mCreateTriggerStack.push(translatedEvent.mTriggerID);
		dispatchToTriggers = false;
	}
	else if(mEvent.getEventType() == EvtData_To_ShowTriggers::msEventType)
	{
		showAllTriggers();
		dispatchToTriggers = false;
	}
	else if(mEvent.getEventType() == EvtData_To_HideTriggers::msEventType)
	{
		hideAllTriggers();
		dispatchToTriggers = false;
	}

	if(dispatchToTriggers)
	{
		mTransferList.clear();
		//Go through the active list and let the triggers handle the events.
		for(vector<TriggerBase *>::iterator listIter = mActiveList.begin();
			listIter != mActiveList.end(); ++listIter)
		{
			(*listIter)->handleEvent(mEvent);
			//Add any activated triggers to the transfer list (ignore children).
			if(!(*listIter)->isActive() && !(*listIter)->isChild())
			{
				mTransferList.push_back((*listIter));
			}
		}

		//Go through the transfer list and move the triggers together with their children.
		for(vector<TriggerBase *>::iterator listIter = mTransferList.begin();
			listIter != mTransferList.end(); ++listIter)
		{
			completeTriggerAndChildren((*listIter));
		}
	}
	return false;
}

void TriggerHandler::updateGFX()
{
	createTriggers();
}

void TriggerHandler::loadTriggers(const char *filePath)
{

}

void TriggerHandler::loadTriggers(std::fstream &file)
{

}

void TriggerHandler::addTrigger(TriggerBase *pTrigger)
{
	if(pTrigger->isActive())
	{
		mActiveList.push_back(pTrigger);
	}
	else
	{
		mCompletedList.push_back(pTrigger);
	}
}

bool TriggerHandler::removeTrigger(TriggerBase *pTrigger)
{
	bool flagResult = false;
	if(pTrigger->isActive())
	{
		for(vector<TriggerBase *>::iterator listIter =  mActiveList.begin();
			listIter != mActiveList.end();)
		{
			if((*listIter) == pTrigger)
			{
				delete (*listIter);
				(*listIter) = NULL;
				listIter = mActiveList.erase(listIter);
				flagResult = true;
			}
			else
			{
				++listIter;
			}
		}
	}
	else
	{
		for(vector<TriggerBase *>::iterator listIter =  mCompletedList.begin();
			listIter != mCompletedList.end();)
		{
			if((*listIter) == pTrigger)
			{
				delete (*listIter);
				(*listIter) = NULL;
				listIter = mCompletedList.erase(listIter);
				flagResult = true;
			}
			else
			{
				++listIter;
			}
		}
	}
	return flagResult;
}

bool TriggerHandler::removeActiveTrigger(unsigned int index)
{
	if(index > 0 && index < mActiveList.size())
	{
		vector<TriggerBase *>::iterator listIter = mActiveList.begin();
		listIter += index;
		delete (*listIter);
		(*listIter) = NULL;
		mActiveList.erase(listIter);
		return true;
	}
	return false;
}

bool TriggerHandler::removeCompletedTrigger(unsigned int index)
{
	if(index > 0 && index < mCompletedList.size())
	{
		vector<TriggerBase *>::iterator listIter = mCompletedList.begin();
		listIter += index;
		delete (*listIter);
		(*listIter) = NULL;
		mCompletedList.erase(listIter);
		return true;
	}
	return false;
}

void TriggerHandler::removeAllActiveTriggers()
{
	mActiveBVList.clear();
	for(vector<TriggerBase *>::iterator listIter =  mActiveList.begin();
		listIter != mActiveList.end(); ++listIter)
	{
		delete (*listIter);
		(*listIter) = NULL;
	}
	mActiveList.clear();
}

void TriggerHandler::removeAllCompletedTriggers()
{
	mCompletedBVList.clear();
	for(vector<TriggerBase *>::iterator listIter =  mCompletedList.begin();
		listIter != mCompletedList.end(); ++listIter)
	{
		delete (*listIter);
		(*listIter) = NULL;
	}
	mCompletedList.clear();
}

void TriggerHandler::removeAllTriggers()
{
	removeAllActiveTriggers();
	removeAllCompletedTriggers();
}

void TriggerHandler::showAllTriggers()
{
	for(vector<TriggerBase *>::iterator listIter =  mActiveList.begin();
		listIter != mActiveList.end(); ++listIter)
	{
		(*listIter)->showSourceBV();
		(*listIter)->showDestinationBV();
	}
}

void TriggerHandler::hideAllTriggers()
{
	for(vector<TriggerBase *>::iterator listIter =  mActiveList.begin();
		listIter != mActiveList.end(); ++listIter)
	{
		(*listIter)->hideSourceBV();
		(*listIter)->hideDestinationBV();
	}
}

void TriggerHandler::completeTrigger(TriggerBase *pTrigger)
{
	//Move the trigger to the completed list.
	for(vector<TriggerBase *>::iterator listIter =  mActiveList.begin();
		listIter != mActiveList.end(); ++listIter)
	{
		if((*listIter) == pTrigger)
		{
			DEBUG_MESSAGE(LEVEL_LOW, "Trigger completed")
			mActiveList.erase(listIter);
			mCompletedList.push_back(pTrigger);
			break;
		}
	}
}

void TriggerHandler::completeTriggerAndChildren(TriggerBase *pParent)
{
	//Add the children to the completed list.
	//Note: the children will be added even if they haven't activated yet, according to their dependency.
	vector<TriggerBase *> hierarchyList;
	pParent->getTriggerHierarchy(hierarchyList);
	for(vector<TriggerBase *>::const_iterator listIter = hierarchyList.begin();
		listIter != hierarchyList.end(); ++listIter)
	{
		completeTrigger((*listIter));
	}
}

void TriggerHandler::updateAllDependencies()
{
	for(vector<TriggerBase *>::iterator listIter = mActiveList.begin();
		listIter != mActiveList.end(); ++listIter)
	{
		(*listIter)->updateDependencies();
	}
}

void TriggerHandler::copyActiveListTo(TriggerHandler &handler)
{
	handler.removeAllActiveTriggers();
	for(vector<TriggerBase *>::iterator listIter = mActiveList.begin();
		listIter != mActiveList.end(); ++listIter)
	{
		handler.addTrigger((*listIter)->clone());
	}
}

void TriggerHandler::copyCompletedListTo(TriggerHandler &handler)
{
	handler.removeAllCompletedTriggers();
	for(vector<TriggerBase *>::iterator listIter = mCompletedList.begin();
		listIter != mCompletedList.end(); ++listIter)
	{
		handler.addTrigger((*listIter)->clone());
	}
}

void TriggerHandler::copyActiveListFrom(const TriggerHandler &handler)
{
	removeAllActiveTriggers();
	const vector<TriggerBase *> &copyFromActiveList = handler.mActiveList;
	for(vector<TriggerBase *>::const_iterator listIter = copyFromActiveList.begin();
		listIter != copyFromActiveList.end(); ++listIter)
	{
		addTrigger((*listIter)->clone());
	}
}

void TriggerHandler::copyCompletedListFrom(const TriggerHandler &handler)
{
	removeAllCompletedTriggers();
	const vector<TriggerBase *> &copyFromCompletedList = handler.mCompletedList;
	for(vector<TriggerBase *>::const_iterator listIter = copyFromCompletedList.begin();
		listIter != copyFromCompletedList.end(); ++listIter)
	{
		addTrigger((*listIter)->clone());
	}
}

void TriggerHandler::createTriggers()
{
	while(!mCreateTriggerStack.empty())
	{
		int triggerID = mCreateTriggerStack.top();
		triggerID = 0;

		TriggerCreateUnit *pTestTrigger = myNew TriggerCreateUnit();

		pTestTrigger->setRequirement(TriggerBase::TriggerRequirements_EnteredSourceBV);
		pTestTrigger->setSubRequirement(TriggerBase::TriggerSubRequirements_None);
		pTestTrigger->setValue(Actor::ActorType_Enemy);
		pTestTrigger->setSubValue(Enemy::EnemyType_Jumper);
		pTestTrigger->setCreateCount(100);

		D3DXVECTOR3 scale(50.0f, 70.0f, 50.0f);
		pTestTrigger->getSourceBV()->setPosition(D3DXVECTOR3(0.0f, scale.y * 0.5f, 0.0f));
		pTestTrigger->getSourceBV()->setScale(scale);
		pTestTrigger->getDestinationBV()->setPosition(D3DXVECTOR3(0.0f, scale.y * 0.5f, 0.0f));
		pTestTrigger->getDestinationBV()->setScale(D3DXVECTOR3(50.0f, 70.0f, 50.0f));
		addTrigger(pTestTrigger);

		mCreateTriggerStack.pop();
	}
}