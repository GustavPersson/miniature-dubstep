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

TriggerHandler *TriggerHandler::mpTriggerHandlerInstance = NULL;

TriggerHandler::TriggerHandler() : EventListener(), mActiveList(), mCompletedList(),
	mTransferList(), mActiveBVList(), mCompletedBVList(), mCreateTriggerStack(), mDeleteTriggerStack(),
	mTriggerScale(50.0f, 70.0f, 50.0f)
{
	mAvailableID = 0;

	//Register events.
	EventListenerPtr handlerPtr = boost::shared_ptr<TriggerHandler>(this);
	safeAddListener(handlerPtr, EvtData_To_TriggerCreate::msEventType);
	safeAddListener(handlerPtr, EvtData_To_TriggerRemove::msEventType);
	safeAddListener(handlerPtr, EvtData_To_TriggerRemoveAll::msEventType);
	safeAddListener(handlerPtr, EvtData_To_TriggerActivate::msEventType);
	safeAddListener(handlerPtr, EvtData_To_TriggerComplete::msEventType);
	safeAddListener(handlerPtr, EvtData_To_TriggerShow::msEventType);
	safeAddListener(handlerPtr, EvtData_To_TriggerHide::msEventType);
	safeAddListener(handlerPtr, EvtData_To_TriggerCollision::msEventType);
}

TriggerHandler::TriggerHandler(const TriggerHandler &handler) : EventListener(), mActiveList(),
	mCompletedList(), mTransferList(), mActiveBVList(), mCompletedBVList(), mCreateTriggerStack(),
	mDeleteTriggerStack(), mTriggerScale(handler.mTriggerScale)
{
	mAvailableID = 0;

	copyActiveListFrom(handler);
	copyCompletedListFrom(handler);

	//Register events.
	EventListenerPtr handlerPtr = boost::shared_ptr<TriggerHandler>(this);
	safeAddListener(handlerPtr, EvtData_To_TriggerCreate::msEventType);
	safeAddListener(handlerPtr, EvtData_To_TriggerRemove::msEventType);
	safeAddListener(handlerPtr, EvtData_To_TriggerRemoveAll::msEventType);
	safeAddListener(handlerPtr, EvtData_To_TriggerActivate::msEventType);
	safeAddListener(handlerPtr, EvtData_To_TriggerComplete::msEventType);
	safeAddListener(handlerPtr, EvtData_To_TriggerShow::msEventType);
	safeAddListener(handlerPtr, EvtData_To_TriggerHide::msEventType);
	safeAddListener(handlerPtr, EvtData_To_TriggerCollision::msEventType);
}

TriggerHandler::~TriggerHandler()
{
	mActiveBVList.clear();
	for(vector<TriggerBase *>::iterator listIter =  mActiveList.begin();
		listIter != mActiveList.end(); ++listIter)
	{
		delete (*listIter);
		(*listIter) = NULL;
	}
	mActiveList.clear();

	mCompletedBVList.clear();
	for(vector<TriggerBase *>::iterator listIter =  mCompletedList.begin();
		listIter != mCompletedList.end(); ++listIter)
	{
		delete (*listIter);
		(*listIter) = NULL;
	}
	mCompletedList.clear();
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

TriggerHandler *TriggerHandler::getInstance()
{
	if(!mpTriggerHandlerInstance)
	{
		mpTriggerHandlerInstance = myNew TriggerHandler();
	}
	return mpTriggerHandlerInstance;
}

char const *TriggerHandler::getName(void)
{
	return "TriggerHandler";
}

bool TriggerHandler::handleEvent(EventData const &mEvent)
{
	bool dispatchToTriggers = true;
	if(mEvent.getEventType() == EvtData_To_TriggerCreate::msEventType)
	{
		const EvtData_To_TriggerCreate &translatedEvent = (const EvtData_To_TriggerCreate &)mEvent;

		TriggerData data;
		data.mType = translatedEvent.mTriggerType;
		data.mRequirement = translatedEvent.mTriggerRequirement;
		data.mSubRequirement = translatedEvent.mTriggerSubRequirement;
		data.mValue = translatedEvent.mTriggerValue;
		data.mSubValue = translatedEvent.mTriggerSubValue;
		data.mMisc = translatedEvent.mMisc;
		data.mSourcePosition = translatedEvent.mSourcePosition;
		data.mSourceRotation = translatedEvent.mSourceRotation;
		data.mSourceScale = translatedEvent.mSourceScale;
		data.mDestinationPosition = translatedEvent.mDestinationPosition;
		data.mDestinationRotation = translatedEvent.mDestinationRotation;
		data.mDestinationScale = translatedEvent.mDestinationScale;
		data.mShow = translatedEvent.mShow;

		mCreateTriggerStack.push(data);
		dispatchToTriggers = false;
	}
	else if(mEvent.getEventType() == EvtData_To_TriggerRemove::msEventType)
	{
		const EvtData_To_TriggerRemove &translatedEvent = (const EvtData_To_TriggerRemove &)mEvent;
		removeActiveTrigger(translatedEvent.mTriggerID);
		removeCompletedTrigger(translatedEvent.mTriggerID);
		dispatchToTriggers = false;
	}
	else if(mEvent.getEventType() == EvtData_To_TriggerRemoveAll::msEventType)
	{
		removeAllTriggers();
		dispatchToTriggers = false;
	}
	else if(mEvent.getEventType() == EvtData_To_TriggerActivate::msEventType)
	{
		const EvtData_To_TriggerActivate &translatedEvent = (const EvtData_To_TriggerActivate &)mEvent;
		forceTriggerActivation(translatedEvent.mTriggerID);
		dispatchToTriggers = false;
	}
	else if(mEvent.getEventType() == EvtData_To_TriggerComplete::msEventType)
	{
		const EvtData_To_TriggerComplete &translatedEvent = (const EvtData_To_TriggerComplete &)mEvent;
		forceTriggerCompletion(translatedEvent.mTriggerID);
		dispatchToTriggers = false;
	}
	else if(mEvent.getEventType() == EvtData_To_TriggerShow::msEventType)
	{
		showAllTriggers();
		dispatchToTriggers = false;
	}
	else if(mEvent.getEventType() == EvtData_To_TriggerHide::msEventType)
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
	deleteTriggers();
}

void TriggerHandler::loadTriggers(const char *filePath)
{

}

void TriggerHandler::loadTriggers(std::fstream &file)
{

}

bool TriggerHandler::forceTriggerActivation(int ID)
{
	for(vector<TriggerBase *>::iterator listIter =  mActiveList.begin();
			listIter != mActiveList.end(); ++listIter)
	{
		if((*listIter)->getTriggerID() == ID)
		{
			(*listIter)->forceActivation();

			//Add to transfer list.
			mTransferList.push_back((*listIter));
		}
	}
	return false;
}

bool TriggerHandler::forceTriggerCompletion(int ID)
{
	for(vector<TriggerBase *>::iterator listIter =  mActiveList.begin();
			listIter != mActiveList.end(); ++listIter)
	{
		if((*listIter)->getTriggerID() == ID)
		{
			(*listIter)->forceCompletion();

			//Add to transfer list.
			mTransferList.push_back((*listIter));
		}
	}
	return false;
}

void TriggerHandler::addTrigger(TriggerBase *pTrigger)
{
	//Set and increase the trigger ID.
	pTrigger->setTriggerID(mAvailableID);
	++mAvailableID;

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
				mDeleteTriggerStack.push((*listIter));
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
				mDeleteTriggerStack.push((*listIter));
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
		mDeleteTriggerStack.push((*listIter));
		mActiveList.erase(listIter);
		return true;
	}
	return false;
}

bool TriggerHandler::removeActiveTrigger(int ID)
{
	for(vector<TriggerBase *>::iterator listIter =  mActiveList.begin();
		listIter != mActiveList.end(); ++listIter)
	{
		if((*listIter)->getTriggerID() == ID)
		{
			mDeleteTriggerStack.push((*listIter));
			mActiveList.erase(listIter);
			return true;
		}
	}
	return false;
}

bool TriggerHandler::removeCompletedTrigger(unsigned int index)
{
	if(index > 0 && index < mCompletedList.size())
	{
		vector<TriggerBase *>::iterator listIter = mCompletedList.begin();
		listIter += index;
		mDeleteTriggerStack.push((*listIter));
		mCompletedList.erase(listIter);
		return true;
	}
	return false;
}

bool TriggerHandler::removeCompletedTrigger(int ID)
{
	for(vector<TriggerBase *>::iterator listIter =  mCompletedList.begin();
		listIter != mCompletedList.end(); ++listIter)
	{
		if((*listIter)->getTriggerID() == ID)
		{
			mDeleteTriggerStack.push((*listIter));
			mCompletedList.erase(listIter);
			return true;
		}
	}
	return false;
}

void TriggerHandler::removeAllActiveTriggers()
{
	mActiveBVList.clear();
	for(vector<TriggerBase *>::iterator listIter =  mActiveList.begin();
		listIter != mActiveList.end(); ++listIter)
	{
		mDeleteTriggerStack.push((*listIter));
	}
	mActiveList.clear();
}

void TriggerHandler::removeAllCompletedTriggers()
{
	mCompletedBVList.clear();
	for(vector<TriggerBase *>::iterator listIter =  mCompletedList.begin();
		listIter != mCompletedList.end(); ++listIter)
	{
		mDeleteTriggerStack.push((*listIter));
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
	for(vector<TriggerBase *>::iterator listIter =  mCompletedList.begin();
		listIter != mCompletedList.end(); ++listIter)
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
	for(vector<TriggerBase *>::iterator listIter =  mCompletedList.begin();
		listIter != mCompletedList.end(); ++listIter)
	{
		(*listIter)->hideSourceBV();
		(*listIter)->hideDestinationBV();
	}
}

int TriggerHandler::getHiddenEnemyCount()
{
	int enemyCount = 0;
	for(vector<TriggerBase *>::iterator listIter =  mActiveList.begin();
		listIter != mActiveList.end(); ++listIter)
	{
		//Find active create unit triggers.
		if((*listIter)->getTriggerType() == TriggerBase::TriggerTypes_CreateUnit)
		{
			//Check the trigger creates enemies.
			if((*listIter)->getValue() == Actor::ActorType_Enemy)
			{
				//Add the misc value to enemyCount.
				enemyCount += (*listIter)->getMisc();
			}
		}
	}
	return enemyCount;
}

void TriggerHandler::getAllTriggers(std::vector<TriggerBase *> &triggers)
{
	for(vector<TriggerBase *>::iterator listIter =  mActiveList.begin();
		listIter != mActiveList.end(); ++listIter)
	{
		triggers.push_back((*listIter));
	}
	for(vector<TriggerBase *>::iterator listIter =  mCompletedList.begin();
		listIter != mCompletedList.end(); ++listIter)
	{
		triggers.push_back((*listIter));
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
		TriggerData data = mCreateTriggerStack.top();

		if(data.mType == TriggerBase::TriggerTypes_CreateUnit)
		{
			TriggerCreateUnit *pCreateTrigger = myNew TriggerCreateUnit();

			pCreateTrigger->setRequirement(data.mRequirement);
			pCreateTrigger->setSubRequirement(data.mSubRequirement);
			pCreateTrigger->setValue(data.mValue);
			pCreateTrigger->setSubValue(data.mSubValue);
			pCreateTrigger->setMisc(data.mMisc);

			data.mSourcePosition.y = data.mSourceScale.y * 0.5f;
			data.mDestinationPosition.y = data.mDestinationScale.y * 0.5f;

			pCreateTrigger->getSourceBV()->setPosition(data.mSourcePosition);
			pCreateTrigger->getSourceBV()->setRotation(data.mSourceRotation);
			pCreateTrigger->getSourceBV()->setScale(data.mSourceScale);
			pCreateTrigger->getDestinationBV()->setPosition(data.mDestinationPosition);
			pCreateTrigger->getDestinationBV()->setRotation(data.mDestinationRotation);
			pCreateTrigger->getDestinationBV()->setScale(data.mDestinationScale);
			if(data.mShow)
			{
				pCreateTrigger->showSourceBV();
				pCreateTrigger->showDestinationBV();
			}
			else
			{
				pCreateTrigger->hideSourceBV();
				pCreateTrigger->hideDestinationBV();
			}
			addTrigger(pCreateTrigger);
		}

		mCreateTriggerStack.pop();
	}
}

void TriggerHandler::deleteTriggers()
{
	while(!mDeleteTriggerStack.empty())
	{
		TriggerBase *pTrigger = mDeleteTriggerStack.top();

		//Attempt to remove the trigger's bounding volumes.
		if(pTrigger->getSourceBV())
		{
			DX10Module::getInstance()->removeRenderObject(pTrigger->getSourceBV());
		}

		if(pTrigger->getDestinationBV())
		{
			DX10Module::getInstance()->removeRenderObject(pTrigger->getDestinationBV());
		}

		if(pTrigger)
		{
			delete pTrigger;
			pTrigger = NULL;
		}

		mDeleteTriggerStack.pop();
	}
}