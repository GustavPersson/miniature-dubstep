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

#include "EditorGUIUniversalList.h"
#include "EditorHandler.h"

using namespace editor;
using namespace gfx;
using namespace logic;
using std::string;
using std::vector;

/******************************************************************
 * GUIListWrapper
 ******************************************************************/

EditorGUIUniversalList::GUIListWrapper::GUIListWrapper(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
	: GUIList(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	init();
}

EditorGUIUniversalList::GUIListWrapper::GUIListWrapper(gfx::GUIFrame *pParent)
	: GUIList(pParent)
{
	init();
}

EditorGUIUniversalList::GUIListWrapper::GUIListWrapper(const GUIListWrapper &list)
	: GUIList(list)
{
	init();
}

EditorGUIUniversalList::GUIListWrapper::~GUIListWrapper()
{

}

const EditorGUIUniversalList::GUIListWrapper &EditorGUIUniversalList::GUIListWrapper::operator=(const GUIListWrapper &list)
{
	if(this != &list)
	{
		GUIList::operator=(list);
	}
	return *this;
}

void EditorGUIUniversalList::GUIListWrapper::init()
{
	addItem(string("Hoho"));
	addItem(string("Testing"));
	addItem(string("Apa"));
}

void EditorGUIUniversalList::GUIListWrapper::notify(int message, gfx::GUIFrame *pSender)
{
	GUIList::notify(message, pSender);
	if(message == GUIFrame::BasicMessage_PressLeft)
	{
		if(getSelected() != "NULL")
		{
			GUIFrame::notify(message, this);
		}
	}
}

/******************************************************************
 * EditorGUIUniversalList
 ******************************************************************/

EditorGUIUniversalList::EditorGUIUniversalList(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
	: GUIFrame(pBaseInstance, clientWidth, clientHeight, parentDepth), mModelList(), mEnemyList()
{
	mpSelectionList = NULL;
	mpButtonContentModel = NULL;
	mpButtonContentEnemy = NULL;
	mpCurrentList = &mModelList;
	init();
}

EditorGUIUniversalList::EditorGUIUniversalList(gfx::GUIFrame *pParent)
	: GUIFrame(pParent), mModelList(), mEnemyList()
{
	mpSelectionList = NULL;
	mpButtonContentModel = NULL;
	mpButtonContentEnemy = NULL;
	mpCurrentList = &mModelList;
	init();
}

EditorGUIUniversalList::EditorGUIUniversalList(const EditorGUIUniversalList &list)
	: GUIFrame(list), mModelList(), mEnemyList()
{
	mpSelectionList = NULL;
	mpButtonContentModel = NULL;
	mpButtonContentEnemy = NULL;
	mpCurrentList = &mModelList;
	init();
}

EditorGUIUniversalList::~EditorGUIUniversalList()
{

}

const EditorGUIUniversalList &EditorGUIUniversalList::operator=(const EditorGUIUniversalList &list)
{
	if(this != &list)
	{
		GUIFrame::operator=(list);
	}
	return *this;
}

void EditorGUIUniversalList::update(float deltaTime)
{
	GUIFrame::update(deltaTime);
	if(getFlagUpdateRequired())
	{
		updateListContent();
	}
}

void EditorGUIUniversalList::onMouseEnter(long mX, long mY)
{
	GUIFrame::onMouseEnter(mX, mY);
}

void EditorGUIUniversalList::onMouseExit(long mX, long mY)
{
	GUIFrame::onMouseExit(mX, mY);
}

void EditorGUIUniversalList::onMouseMove(long mX, long mY)
{
	GUIFrame::onMouseMove(mX, mY);
}

void EditorGUIUniversalList::onMouseLeftDown(long mX, long mY)
{
	GUIFrame::onMouseLeftDown(mX, mY);
}

void EditorGUIUniversalList::onMouseMiddleDown(long mX, long mY)
{
	GUIFrame::onMouseMiddleDown(mX, mY);
}

void EditorGUIUniversalList::onMouseRightDown(long mX, long mY)
{
	GUIFrame::onMouseRightDown(mX, mY);
}

void EditorGUIUniversalList::onMouseLeftUp(long mX, long mY)
{
	GUIFrame::onMouseLeftUp(mX, mY);
}

void EditorGUIUniversalList::onMouseMiddleUp(long mX, long mY)
{
	GUIFrame::onMouseMiddleDown(mX, mY);
}

void EditorGUIUniversalList::onMouseRightUp(long mX, long mY)
{
	GUIFrame::onMouseRightUp(mX, mY);
}

void EditorGUIUniversalList::onKeyDown(int letterKeys, int keys)
{
	GUIFrame::onKeyDown(letterKeys, keys);
}

void EditorGUIUniversalList::addModelContent(int objectID, const char *text)
{
	ObjectSelection object;
	object.mObjectID = objectID;
	object.mText = text;
	mModelList.push_back(object);
	updateRequired();
}

void EditorGUIUniversalList::addEnemyContent(int objectID, const char *text)
{
	ObjectSelection object;
	object.mObjectID = objectID;
	object.mText = text;
	mEnemyList.push_back(object);
	updateRequired();
}

void EditorGUIUniversalList::removeAllModelContent()
{
	mModelList.clear();
	updateRequired();
}

void EditorGUIUniversalList::removeAllEnemyContent()
{
	mEnemyList.clear();
	updateRequired();
}

void EditorGUIUniversalList::init()
{
	mpSelectionList = myNew GUIListWrapper(this);
	mpSelectionList->setRelativePosition(D3DXVECTOR2(0.0f, 0.1f));
	mpSelectionList->setRelativeSize(D3DXVECTOR2(1.0f, 0.9f));
	mpSelectionList->showNumberOfElements(20);
	addSubFrame(mpSelectionList);

	mpButtonContentModel = myNew GUIButton(this);
	mpButtonContentModel->setRelativePosition(D3DXVECTOR2(0.0f, 0.0f));
	mpButtonContentModel->setRelativeSize(D3DXVECTOR2(0.5f, 0.1f));
	mpButtonContentModel->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpButtonContentModel->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpButtonContentModel->getTextFrame()->setText("Buildings");
	mpButtonContentModel->setTextAlignment(logic::GUI_Alignment_Center);
	mpButtonContentModel->enableText();
	addSubFrame(mpButtonContentModel);

	mpButtonContentEnemy = myNew GUIButton(this);
	mpButtonContentEnemy->setRelativePosition(D3DXVECTOR2(0.5f, 0.0f));
	mpButtonContentEnemy->setRelativeSize(D3DXVECTOR2(0.5f, 0.1f));
	mpButtonContentEnemy->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpButtonContentEnemy->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpButtonContentEnemy->getTextFrame()->setText("Bugs");
	mpButtonContentEnemy->setTextAlignment(logic::GUI_Alignment_Center);
	mpButtonContentEnemy->enableText();
	addSubFrame(mpButtonContentEnemy);
}

void EditorGUIUniversalList::notify(int message, gfx::GUIFrame *pSender)
{
	if(pSender == mpSelectionList)
	{
		if(message == GUIFrame::BasicMessage_PressLeft)
		{
			ObjectSelection *pSelection = getObjectSelection(mpSelectionList->getSelected());
			if(pSelection)
			{
				if(mpCurrentList == &mModelList)
				{
					EditorHandler::getInstance()->addMessage(myNew EMListSelectModel(pSelection->mObjectID, pSelection->mText.c_str()));
				}
				else if(mpCurrentList == &mEnemyList)
				{
					EditorHandler::getInstance()->addMessage(myNew EMListSelectEnemy(pSelection->mObjectID, pSelection->mText.c_str()));
				}
			}
		}
	}
	else if(pSender == mpButtonContentModel)
	{
		if(message == GUIFrame::BasicMessage_PressLeft)
		{
			mpCurrentList = &mModelList;
			updateRequired();
		}
	}
	else if(pSender == mpButtonContentEnemy)
	{
		if(message == GUIFrame::BasicMessage_PressLeft)
		{
			mpCurrentList = &mEnemyList;
			updateRequired();
		}
	}
}

void EditorGUIUniversalList::updateListContent()
{
	mpSelectionList->clear();
	for(vector<ObjectSelection>::iterator selectIter = mpCurrentList->begin();
		selectIter != mpCurrentList->end(); ++selectIter)
	{
		mpSelectionList->addItem((*selectIter).mText);
	}
}

EditorGUIUniversalList::ObjectSelection *EditorGUIUniversalList::getObjectSelection(const std::string &selectionText)
{
	for(vector<ObjectSelection>::iterator selectIter = mpCurrentList->begin();
		selectIter != mpCurrentList->end(); ++selectIter)
	{
		if((*selectIter).mText == selectionText)
		{
			return &(*selectIter);
		}
	}
	return NULL;
}