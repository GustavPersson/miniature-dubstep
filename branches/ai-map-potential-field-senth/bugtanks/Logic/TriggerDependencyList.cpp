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

#include "TriggerDependencyList.h"

using namespace logic;
using std::vector;

TriggerDependency::TriggerDependency()
{
	pTrigger = NULL;
	flagDependency = false;
}

TriggerDependency::TriggerDependency(void *pTrigger, bool flagDependency)
{
	this->pTrigger = pTrigger;
	this->flagDependency = flagDependency;
}

TriggerDependency::TriggerDependency(const TriggerDependency &dependency)
{
	pTrigger = dependency.pTrigger;
	flagDependency = dependency.flagDependency;
}

TriggerDependency::~TriggerDependency()
{

}

const TriggerDependency &TriggerDependency::operator=(const TriggerDependency &dependency)
{
	if(this != &dependency)
	{
		pTrigger = dependency.pTrigger;
		flagDependency = dependency.flagDependency;
	}
	return *this;
}

TriggerDependencyList::TriggerDependencyList() : mDependencyList()
{

}

TriggerDependencyList::TriggerDependencyList(const TriggerDependencyList &list)  : mDependencyList()
{
	copyDependencyListFrom(list);
}

TriggerDependencyList::~TriggerDependencyList()
{
	removeAll();
}

const TriggerDependencyList &TriggerDependencyList::operator =(const TriggerDependencyList &list)
{
	if(this != &list)
	{
		copyDependencyListFrom(list);
	}
	return *this;
}

bool TriggerDependencyList::containsDependency(TriggerDependency dependency) const
{
	for(vector<TriggerDependency>::const_iterator dependencyIter = mDependencyList.begin();
		dependencyIter != mDependencyList.end(); ++dependencyIter)
	{
		if(dependencyIter->pTrigger == dependency.pTrigger)
		{
			return true;
		}
	}
	return false;
}

bool TriggerDependencyList::containsDependency(void *pTrigger)
{
	for(vector<TriggerDependency>::iterator dependencyIter = mDependencyList.begin();
		dependencyIter != mDependencyList.end(); ++dependencyIter)
	{
		if(dependencyIter->pTrigger == pTrigger)
		{
			return true;
		}
	}
	return false;
}

bool TriggerDependencyList::addTriggerDependency(TriggerDependency dependency)
{
	if(dependency.pTrigger)
	{
		mDependencyList.push_back(dependency);
		return true;
	}
	return false;
}

bool TriggerDependencyList::addTriggerDependency(void *pTrigger, bool flagDependency)
{
	if(pTrigger)
	{
		mDependencyList.push_back(TriggerDependency(pTrigger, flagDependency));
		return true;
	}
	return false;
}

bool TriggerDependencyList::removeTriggerDepedency(TriggerDependency dependency)
{
	bool flagResult = false;
	for(vector<TriggerDependency>::iterator dependencyIter = mDependencyList.begin();
		dependencyIter != mDependencyList.end();)
	{
		if(dependencyIter->pTrigger == dependency.pTrigger)
		{
			dependencyIter = mDependencyList.erase(dependencyIter);
			flagResult = true;
		}
		else
		{
			++dependencyIter;
		}
	}
	return flagResult;
}

bool TriggerDependencyList::removeTriggerDependency(void *pTrigger)
{
	bool flagResult = false;
	for(vector<TriggerDependency>::iterator dependencyIter = mDependencyList.begin();
		dependencyIter != mDependencyList.end();)
	{
		if(dependencyIter->pTrigger == pTrigger)
		{
			dependencyIter = mDependencyList.erase(dependencyIter);
			flagResult = true;
		}
		else
		{
			++dependencyIter;
		}
	}
	return flagResult;
}

bool TriggerDependencyList::removeTriggerDependency(bool flagDependency)
{
	bool flagResult = false;
	for(vector<TriggerDependency>::iterator dependencyIter = mDependencyList.begin();
		dependencyIter != mDependencyList.end();)
	{
		if(dependencyIter->flagDependency == flagDependency)
		{
			dependencyIter = mDependencyList.erase(dependencyIter);
			flagResult = true;
		}
		else
		{
			++dependencyIter;
		}
	}
	return flagResult;
}

bool TriggerDependencyList::removeTriggerDependency(void *pTrigger, bool flagDependency)
{
	bool flagResult = false;
	for(vector<TriggerDependency>::iterator dependencyIter = mDependencyList.begin();
		dependencyIter != mDependencyList.end();)
	{
		if(dependencyIter->pTrigger == pTrigger &&
		   dependencyIter->flagDependency == flagDependency)
		{
			dependencyIter = mDependencyList.erase(dependencyIter);
			flagResult = true;
		}
		else
		{
			++dependencyIter;
		}
	}
	return flagResult;
}

bool TriggerDependencyList::removeTriggerDependecy(unsigned int index)
{
	if(index > 0 && index < mDependencyList.size())
	{
		vector<TriggerDependency>::iterator dependencyIter = mDependencyList.begin();
		dependencyIter += index;
		mDependencyList.erase(dependencyIter);
		return true;
	}
	return false;
}

void TriggerDependencyList::removeAll()
{
	mDependencyList.clear();
}

void TriggerDependencyList::copyDependencyListTo(TriggerDependencyList &list)
{
	list.removeAll();
	for(vector<TriggerDependency>::iterator dependencyIter = mDependencyList.begin();
		dependencyIter != mDependencyList.end(); ++dependencyIter)
	{
		list.addTriggerDependency(*dependencyIter);
	}
}

void TriggerDependencyList::copyDependencyListFrom(const TriggerDependencyList &list)
{
	mDependencyList.clear();
	const vector<TriggerDependency> &copyFromList = list.mDependencyList;
	for(vector<TriggerDependency>::const_iterator dependencyIter = copyFromList.begin();
		dependencyIter != copyFromList.end(); ++dependencyIter)
	{
		addTriggerDependency(*dependencyIter);
	}
}