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
using std::vector;
using std::fstream;

TriggerHandler::TriggerHandler() : EventListener(), mActiveList(), mCompletedList(),
								   mTransferList(), mActiveBVList(), mCompletedBVList()
{
	mEventListenerPointer = boost::shared_ptr<TriggerHandler>(this);
}

TriggerHandler::TriggerHandler(const TriggerHandler &handler) : EventListener(), mActiveList(),
																mCompletedList(), mTransferList(), 
																mActiveBVList(), mCompletedBVList()
{
	mEventListenerPointer = boost::shared_ptr<TriggerHandler>(this);
	copyActiveListFrom(handler);
	copyCompletedListFrom(handler);
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
	DEBUG_MESSAGE("Received event.")
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
	return true;
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

void TriggerHandler::completeTrigger(TriggerBase *pTrigger)
{
	//Move the trigger to the completed list.
	for(vector<TriggerBase *>::iterator listIter =  mActiveList.begin();
		listIter != mActiveList.end(); ++listIter)
	{
		if((*listIter) == pTrigger)
		{
			DEBUG_MESSAGE("Trigger completed")
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