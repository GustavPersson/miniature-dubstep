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

#include "EditorGUIUtilityBar.h"
#include "../Logic/Events.h"
#include "EditorHandler.h"

using namespace editor;
using namespace gfx;
using namespace logic;
using std::string;

EditorGUIUtilityBar::EditorGUIUtilityBar(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
	: GUIImage(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	mpMapName = NULL;
	mpTextMapName = NULL;
	mpTextInstructions = NULL;
	mpButtonSaveMap = NULL;
	mpButtonLoadMap = NULL;
	mpButtonCreateTrigger = NULL;
	mpButtonShowTriggers = NULL;
	mpButtonHideTriggers = NULL;
	mpTriggerWindow = NULL;
	init();
}

EditorGUIUtilityBar::EditorGUIUtilityBar(gfx::GUIFrame *pParent)
	: GUIImage(pParent)
{
	mpMapName = NULL;
	mpTextMapName = NULL;
	mpTextInstructions = NULL;
	mpButtonSaveMap = NULL;
	mpButtonLoadMap = NULL;
	mpButtonCreateTrigger = NULL;
	mpButtonShowTriggers = NULL;
	mpButtonHideTriggers = NULL;
	mpTriggerWindow = NULL;
	init();
}

EditorGUIUtilityBar::EditorGUIUtilityBar(const EditorGUIUtilityBar &bar)
	: GUIImage(bar)
{
	mpMapName = NULL;
	mpTextMapName = NULL;
	mpTextInstructions = NULL;
	mpButtonSaveMap = NULL;
	mpButtonLoadMap = NULL;
	mpButtonCreateTrigger = NULL;
	mpButtonShowTriggers = NULL;
	mpButtonHideTriggers = NULL;
	mpTriggerWindow = NULL;
	init();
}

EditorGUIUtilityBar::~EditorGUIUtilityBar()
{

}

const EditorGUIUtilityBar &EditorGUIUtilityBar::operator=(const EditorGUIUtilityBar &bar)
{
	if(this != &bar)
	{
		GUIImage::operator=(bar);
	}
	return *this;
}

void EditorGUIUtilityBar::update(float deltaTime)
{
	GUIImage::update(deltaTime);
	if(mpMapName->getFlagMouseFocus())
	{
		mpMapName->setInputMode(GUITextField::input_key | GUITextField::input_ignore_space | GUITextField::input_ignore_enter);
	}
	else
	{
		mpMapName->setInputMode(GUITextField::input_none);
	}
}

void EditorGUIUtilityBar::getMapName(std::string &name)
{
	name.clear();
	name = mpMapName->getTextFrame()->getText();
}

void EditorGUIUtilityBar::init()
{
	mpMapName = myNew GUITextField(this);
	mpMapName->setRelativePosition(D3DXVECTOR2(0.1f, 0.2f));
	mpMapName->setRelativeSize(D3DXVECTOR2(0.8f, 0.05f));
	mpMapName->setColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	mpMapName->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpMapName->getTextFrame()->setTextFormat(GUIText::TextFormat_FixedLine);
	mpMapName->getTextFrame()->setTextSize(D3DXVECTOR2(15.0f, 25.0f));
	addSubFrame(mpMapName);

	mpTextMapName = myNew GUIText(this);
	mpTextMapName->setRelativePosition(D3DXVECTOR2(0.1f, 0.15f));
	mpTextMapName->setRelativeSize(D3DXVECTOR2(0.8f, 0.05f));
	mpTextMapName->setTextFormat(GUIText::TextFormat_FixedLine);
	mpTextMapName->setTextSize(D3DXVECTOR2(15.0f, 25.0f));
	mpTextMapName->setText("Map Name");
	addSubFrame(mpTextMapName);

	mpTextInstructions = myNew GUIText(this);
	mpTextInstructions->setRelativePosition(D3DXVECTOR2(0.02f, 0.8f));
	mpTextInstructions->setRelativeSize(D3DXVECTOR2(0.96f, 0.23f));
	mpTextInstructions->setTextFormat(GUIText::TextFormat_FixedRows);
	mpTextInstructions->setTextSize(D3DXVECTOR2(8.0f, 20.0f));
	mpTextInstructions->setText(string("Select      Left Mouse\n")
							  + string("Unselect    Right Mouse\n")
							  + string("Scale       Z X\n")
							  + string("Rotate      Shift Z X\n")
							  + string("Delete      Backspace\n")
							  + string("Map Texture F1 F2 F3 F4\n"));
	addSubFrame(mpTextInstructions);

	mpButtonSaveMap = myNew GUIButton(this);
	mpButtonSaveMap->setRelativePosition(D3DXVECTOR2(0.1f, 0.3f));
	mpButtonSaveMap->setRelativeSize(D3DXVECTOR2(0.8f, 0.05f));
	mpButtonSaveMap->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpButtonSaveMap->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpButtonSaveMap->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	mpButtonSaveMap->getTextFrame()->setText("Save Map");
	mpButtonSaveMap->enableText();
	addSubFrame(mpButtonSaveMap);

	mpButtonLoadMap = myNew GUIButton(this);
	mpButtonLoadMap->setRelativePosition(D3DXVECTOR2(0.1f, 0.375f));
	mpButtonLoadMap->setRelativeSize(D3DXVECTOR2(0.8f, 0.05f));
	mpButtonLoadMap->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpButtonLoadMap->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpButtonLoadMap->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	mpButtonLoadMap->getTextFrame()->setText("Load Map");
	mpButtonLoadMap->enableText();
	addSubFrame(mpButtonLoadMap);

	mpButtonCreateTrigger = myNew GUIButton(this);
	mpButtonCreateTrigger->setRelativePosition(D3DXVECTOR2(0.1f, 0.45f));
	mpButtonCreateTrigger->setRelativeSize(D3DXVECTOR2(0.8f, 0.05f));
	mpButtonCreateTrigger->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpButtonCreateTrigger->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpButtonCreateTrigger->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	mpButtonCreateTrigger->getTextFrame()->setText("Create Trigger");
	mpButtonCreateTrigger->enableText();
	addSubFrame(mpButtonCreateTrigger);

	mpButtonShowTriggers = myNew GUIButton(this);
	mpButtonShowTriggers->setRelativePosition(D3DXVECTOR2(0.1f, 0.525f));
	mpButtonShowTriggers->setRelativeSize(D3DXVECTOR2(0.8f, 0.05f));
	mpButtonShowTriggers->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpButtonShowTriggers->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpButtonShowTriggers->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	mpButtonShowTriggers->getTextFrame()->setText("Show Triggers");
	mpButtonShowTriggers->enableText();
	addSubFrame(mpButtonShowTriggers);

	mpButtonHideTriggers = myNew GUIButton(this);
	mpButtonHideTriggers->setRelativePosition(D3DXVECTOR2(0.1f, 0.6f));
	mpButtonHideTriggers->setRelativeSize(D3DXVECTOR2(0.8f, 0.05f));
	mpButtonHideTriggers->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpButtonHideTriggers->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpButtonHideTriggers->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	mpButtonHideTriggers->getTextFrame()->setText("Hide Triggers");
	mpButtonHideTriggers->enableText();
	addSubFrame(mpButtonHideTriggers);
}

void EditorGUIUtilityBar::notify(int message, gfx::GUIFrame *pSender)
{
	if(pSender == mpButtonSaveMap)
	{
		if(message == GUIFrame::BasicMessage_PressLeft)
		{
			string name;
			getMapName(name);
			EditorHandler::getInstance()->addMessage(myNew EMSaveMap(name.c_str(), false));
		}
	}
	else if(pSender == mpButtonLoadMap)
	{
		if(message == GUIFrame::BasicMessage_PressLeft)
		{
			string name;
			getMapName(name);
			EditorHandler::getInstance()->addMessage(myNew EMLoadMap(name.c_str()));
		}
	}
	else if(pSender == mpButtonCreateTrigger)
	{
		if(message == GUIFrame::BasicMessage_PressLeft)
		{
			if(mpTriggerWindow->getFlagVisible())
			{
				mpTriggerWindow->setFlagVisible(false);
			}
			else
			{
				mpTriggerWindow->setFlagVisible(true);
			}
		}
	}
	else if(pSender == mpButtonShowTriggers)
	{
		if(message == GUIFrame::BasicMessage_PressLeft)
		{
			safeThreadSafeQueueEvent(EventDataPtr(myNew logic::EvtData_To_TriggerShow()));
		}
	}
	else if(pSender == mpButtonHideTriggers)
	{
		if(message == GUIFrame::BasicMessage_PressLeft)
		{
			safeThreadSafeQueueEvent(EventDataPtr(myNew logic::EvtData_To_TriggerHide()));
		}
	}
}