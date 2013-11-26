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

#include "EditorGUITriggerWindow.h"
#include "../Logic/Events.h"
#include "../Logic/Triggers.h"
#include "EditorHandler.h"
#include <sstream>

using namespace editor;
using namespace gfx;
using namespace logic;
using std::stringstream;

/******************************************************************
 * SelectWindow
 ******************************************************************/

EditorGUITriggerWindow::SelectWindow::SelectWindow(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
	: GUIImage(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	mpTextTitle = NULL;
	mpButtonClose = NULL;
	mpButtons = NULL;
	mpButtonData = NULL;
	mButtonCount = 18;
	mType = -1;
	mRequirement = -1;
	mSubRequirement = -1;
	mValue = -1;
	mSubValue = -1;
	init();
}

EditorGUITriggerWindow::SelectWindow::SelectWindow(gfx::GUIFrame *pParent)
	: GUIImage(pParent)
{
	mpTextTitle = NULL;
	mpButtonClose = NULL;
	mpButtons = NULL;
	mpButtonData = NULL;
	mButtonCount = 18;
	mType = -1;
	mRequirement = -1;
	mSubRequirement = -1;
	mValue = -1;
	mSubValue = -1;
	init();
}

EditorGUITriggerWindow::SelectWindow::SelectWindow(const SelectWindow &window)
	: GUIImage(window)
{
	mpTextTitle = NULL;
	mpButtonClose = NULL;
	mpButtons = NULL;
	mpButtonData = NULL;
	mButtonCount = 18;
	mType = -1;
	mRequirement = -1;
	mSubRequirement = -1;
	mValue = -1;
	mSubValue = -1;
	init();
}

EditorGUITriggerWindow::SelectWindow::~SelectWindow()
{
	if(mpButtons)
	{
		delete [] mpButtons;
	}
	if(mpButtonData)
	{
		delete [] mpButtonData;
	}
}

const EditorGUITriggerWindow::SelectWindow &EditorGUITriggerWindow::SelectWindow::operator=(const SelectWindow &window)
{
	if(this != &window)
	{
		GUIImage::operator=(window);
	}
	return *this;
}

void EditorGUITriggerWindow::SelectWindow::update(float deltaTime)
{
	GUIImage::update(deltaTime);
}

void EditorGUITriggerWindow::SelectWindow::updateButtons()
{

}

void EditorGUITriggerWindow::SelectWindow::resetTriggerVariables()
{
	mType = -1;
	mRequirement = -1;
	mSubRequirement = -1;
	mValue = -1;
	mSubValue = -1;
}

void EditorGUITriggerWindow::SelectWindow::init()
{
	mpTextTitle = myNew GUIText(this);
	mpTextTitle->setRelativePosition(D3DXVECTOR2(0.35f, 0.1f));
	mpTextTitle->setRelativeSize(D3DXVECTOR2(0.3f, 0.05f));
	mpTextTitle->setTextFormat(GUIText::TextFormat_ScaledLine);
	mpTextTitle->setText("");
	addSubFrame(mpTextTitle);

	mpButtonClose = myNew GUIButton(this);
	mpButtonClose->setRelativePosition(D3DXVECTOR2(0.35f, 0.8f));
	mpButtonClose->setRelativeSize(D3DXVECTOR2(0.3f, 0.05f));
	mpButtonClose->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpButtonClose->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpButtonClose->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	mpButtonClose->getTextFrame()->setText("Close");
	mpButtonClose->enableText();
	addSubFrame(mpButtonClose);

	mpButtons = myNew GUIButton *[mButtonCount];
	for(int n = 0; n < mButtonCount / 2; ++n)
	{
		mpButtons[n] = myNew GUIButton(this);
		mpButtons[n]->setRelativePosition(D3DXVECTOR2(0.15f, 0.1f + (n + 1) * 0.07f));
		mpButtons[n]->setRelativeSize(D3DXVECTOR2(0.3f, 0.05f));
		mpButtons[n]->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
		mpButtons[n]->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
		mpButtons[n]->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
		mpButtons[n]->getTextFrame()->setText("Button");
		mpButtons[n]->enableText();
		addSubFrame(mpButtons[n]);
	}
	for(int n = mButtonCount / 2; n < mButtonCount; ++n)
	{
		mpButtons[n] = myNew GUIButton(this);
		mpButtons[n]->setRelativePosition(D3DXVECTOR2(0.55f, 0.1f + (n - mButtonCount / 2 + 1) * 0.07f));
		mpButtons[n]->setRelativeSize(D3DXVECTOR2(0.3f, 0.05f));
		mpButtons[n]->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
		mpButtons[n]->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
		mpButtons[n]->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
		mpButtons[n]->getTextFrame()->setText("Button");
		mpButtons[n]->enableText();
		addSubFrame(mpButtons[n]);
	}

	mpButtonData = myNew int[mButtonCount];
}

void EditorGUITriggerWindow::SelectWindow::notify(int message, gfx::GUIFrame *pSender)
{
	if(pSender == mpButtonClose)
	{
		if(message == GUIFrame::BasicMessage_PressLeft)
		{
			setFlagVisible(false);

			//Send a notification to the trigger window.
			GUIFrame::notify(SelectWindow::SelectMessages_Close, this);
		}
	}
	//Check all selection buttons.
	for(int nButton = 0; nButton < mButtonCount; ++nButton)
	{
		if(pSender == mpButtons[nButton])
		{
			if(message == GUIFrame::BasicMessage_PressLeft)
			{
				//Send a notification and close the window if the button data isn't -1.
				if(mpButtonData[nButton] != -1)
				{
					//Send a notification to itself in order to update trigger variables.
					notify(SelectWindow::SelectMessages_Data + mpButtonData[nButton], this);

					setFlagVisible(false);

					//Send the button data value as notification.
					GUIFrame::notify(SelectWindow::SelectMessages_Selection, this);
				}
			}
		}
	}
}

/******************************************************************
 * SelectType
 ******************************************************************/

EditorGUITriggerWindow::SelectType::SelectType(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
	: SelectWindow(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	init();
}

EditorGUITriggerWindow::SelectType::SelectType(gfx::GUIFrame *pParent)
	: SelectWindow(pParent)
{
	init();
}

EditorGUITriggerWindow::SelectType::SelectType(const SelectType &window)
	: SelectWindow(window)
{
	init();
}

EditorGUITriggerWindow::SelectType::~SelectType()
{

}

const EditorGUITriggerWindow::SelectType &EditorGUITriggerWindow::SelectType::operator=(const SelectType &window)
{
	if(this != &window)
	{
		SelectWindow::operator=(window);
	}
	return *this;
}

void EditorGUITriggerWindow::SelectType::update(float deltaTime)
{
	SelectWindow::update(deltaTime);
}

void EditorGUITriggerWindow::SelectType::updateButtons()
{
	//Trigger type is the highest in the trigger variable hierarchy and the
	//available selections are therefore always the same.
	for(int nButton = 0; nButton < mButtonCount; ++nButton)
	{
		if(nButton == 0)
		{
			mpButtons[nButton]->getTextFrame()->setText("Create Unit");
			mpButtonData[nButton] = TriggerBase::TriggerTypes_CreateUnit;
		}
		else
		{
			mpButtons[nButton]->getTextFrame()->setText("");
			mpButtonData[nButton] = -1;
		}
	}
}

void EditorGUITriggerWindow::SelectType::init()
{
	mpTextTitle->setText("Select Type");
	updateButtons();
}

void EditorGUITriggerWindow::SelectType::notify(int message, gfx::GUIFrame *pSender)
{
	//Check for trigger variable updates.
	if(pSender == this)
	{
		if(message >= SelectWindow::SelectMessages_Data)
		{
			mType = message - SelectWindow::SelectMessages_Data;
		}
	}
	SelectWindow::notify(message, pSender);
}

/******************************************************************
 * SelectRequirement
 ******************************************************************/

EditorGUITriggerWindow::SelectRequirement::SelectRequirement(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
	: SelectWindow(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	init();
}

EditorGUITriggerWindow::SelectRequirement::SelectRequirement(gfx::GUIFrame *pParent)
	: SelectWindow(pParent)
{
	init();
}

EditorGUITriggerWindow::SelectRequirement::SelectRequirement(const SelectRequirement &window)
	: SelectWindow(window)
{
	init();
}

EditorGUITriggerWindow::SelectRequirement::~SelectRequirement()
{

}

const EditorGUITriggerWindow::SelectRequirement &EditorGUITriggerWindow::SelectRequirement::operator=(const SelectRequirement &window)
{
	if(this != &window)
	{
		SelectWindow::operator=(window);
	}
	return *this;
}

void EditorGUITriggerWindow::SelectRequirement::update(float deltaTime)
{
	SelectWindow::update(deltaTime);
}

void EditorGUITriggerWindow::SelectRequirement::updateButtons()
{
	//Switch selections based on the current trigger type.
	switch(mType)
	{
		case TriggerBase::TriggerTypes_StartMap:
			break;

		case TriggerBase::TriggerTypes_EndMap:
			break;

		case TriggerBase::TriggerTypes_CreateUnit:
			for(int nButton = 0; nButton < mButtonCount; ++nButton)
			{
				if(nButton == 0)
				{
					mpButtons[nButton]->getTextFrame()->setText("Entered Source");
					mpButtonData[nButton] = TriggerBase::TriggerRequirements_EnteredSourceBV;
				}
				else if(nButton == 1)
				{
					mpButtons[nButton]->getTextFrame()->setText("Entered Destination");
					mpButtonData[nButton] = TriggerBase::TriggerRequirements_EnteredDestinationBV;
				}
				else
				{
					mpButtons[nButton]->getTextFrame()->setText("");
					mpButtonData[nButton] = -1;
				}
			}
			break;

		case TriggerBase::TriggerTypes_DestroyUnit:
			break;

		default:
			for(int nButton = 0; nButton < mButtonCount; ++nButton)
			{
				mpButtons[nButton]->getTextFrame()->setText("");
				mpButtonData[nButton] = -1;
			}
			break;
	}
}

void EditorGUITriggerWindow::SelectRequirement::init()
{
	mpTextTitle->setText("Select Requirement");
	updateButtons();
}

void EditorGUITriggerWindow::SelectRequirement::notify(int message, gfx::GUIFrame *pSender)
{
	//Check for trigger variable updates.
	if(pSender == this)
	{
		if(message >= SelectWindow::SelectMessages_Data)
		{
			mRequirement = message - SelectWindow::SelectMessages_Data;
		}
	}
	SelectWindow::notify(message, pSender);
}

/******************************************************************
 * SelectSubRequirement
 ******************************************************************/

EditorGUITriggerWindow::SelectSubRequirement::SelectSubRequirement(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
	: SelectWindow(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	init();
}

EditorGUITriggerWindow::SelectSubRequirement::SelectSubRequirement(gfx::GUIFrame *pParent)
	: SelectWindow(pParent)
{
	init();
}

EditorGUITriggerWindow::SelectSubRequirement::SelectSubRequirement(const SelectSubRequirement &window)
	: SelectWindow(window)
{
	init();
}

EditorGUITriggerWindow::SelectSubRequirement::~SelectSubRequirement()
{

}

const EditorGUITriggerWindow::SelectSubRequirement &EditorGUITriggerWindow::SelectSubRequirement::operator=(const SelectSubRequirement &window)
{
	if(this != &window)
	{
		SelectWindow::operator=(window);
	}
	return *this;
}

void EditorGUITriggerWindow::SelectSubRequirement::update(float deltaTime)
{
	SelectWindow::update(deltaTime);
}

void EditorGUITriggerWindow::SelectSubRequirement::updateButtons()
{
	//Sub requirements are currently not available. Disable all.
	for(int nButton = 0; nButton < mButtonCount; ++nButton)
	{
		mpButtons[nButton]->getTextFrame()->setText("");
		mpButtonData[nButton] = -1;
	}

	//Switch selections based on the current trigger type and requirement.
	/*switch(mType)
	{
		case TriggerBase::TriggerTypes_StartMap:
			switch(mRequirement)
			{
				case TriggerBase::TriggerRequirements_TimeSinceMapStart:
					break;

				case TriggerBase::TriggerRequirements_TimeSinceProgramStart:
					break;

				case TriggerBase::TriggerRequirements_EnteredSourceBV:
					break;

				case TriggerBase::TriggerRequirements_ExitedSourceBV:
					break;

				case TriggerBase::TriggerRequirements_EnteredDestinationBV:
					break;

				case TriggerBase::TriggerRequirements_ExitedDestinationBV:
					break;

				case TriggerBase::TriggerRequirements_ObjectCreated:
					break;

				case TriggerBase::TriggerRequirements_ObjectDestroyed:
					break;

				default:
					break;
			}
			break;

		case TriggerBase::TriggerTypes_EndMap:
			switch(mRequirement)
			{
				case TriggerBase::TriggerRequirements_TimeSinceMapStart:
					break;

				case TriggerBase::TriggerRequirements_TimeSinceProgramStart:
					break;

				case TriggerBase::TriggerRequirements_EnteredSourceBV:
					break;

				case TriggerBase::TriggerRequirements_ExitedSourceBV:
					break;

				case TriggerBase::TriggerRequirements_EnteredDestinationBV:
					break;

				case TriggerBase::TriggerRequirements_ExitedDestinationBV:
					break;

				case TriggerBase::TriggerRequirements_ObjectCreated:
					break;

				case TriggerBase::TriggerRequirements_ObjectDestroyed:
					break;

				default:
					break;
			}
			break;

		case TriggerBase::TriggerTypes_CreateUnit:
			switch(mRequirement)
			{
				case TriggerBase::TriggerRequirements_TimeSinceMapStart:
					break;

				case TriggerBase::TriggerRequirements_TimeSinceProgramStart:
					break;

				case TriggerBase::TriggerRequirements_EnteredSourceBV:
					break;

				case TriggerBase::TriggerRequirements_ExitedSourceBV:
					break;

				case TriggerBase::TriggerRequirements_EnteredDestinationBV:
					break;

				case TriggerBase::TriggerRequirements_ExitedDestinationBV:
					break;

				case TriggerBase::TriggerRequirements_ObjectCreated:
					break;

				case TriggerBase::TriggerRequirements_ObjectDestroyed:
					break;

				default:
					break;
			}
			break;

		case TriggerBase::TriggerTypes_DestroyUnit:
			switch(mRequirement)
			{
				case TriggerBase::TriggerRequirements_TimeSinceMapStart:
					break;

				case TriggerBase::TriggerRequirements_TimeSinceProgramStart:
					break;

				case TriggerBase::TriggerRequirements_EnteredSourceBV:
					break;

				case TriggerBase::TriggerRequirements_ExitedSourceBV:
					break;

				case TriggerBase::TriggerRequirements_EnteredDestinationBV:
					break;

				case TriggerBase::TriggerRequirements_ExitedDestinationBV:
					break;

				case TriggerBase::TriggerRequirements_ObjectCreated:
					break;

				case TriggerBase::TriggerRequirements_ObjectDestroyed:
					break;

				default:
					break;
			}
			break;

		default:
			break;
	}*/
}

void EditorGUITriggerWindow::SelectSubRequirement::init()
{
	mpTextTitle->setText("Select Subrequirement");
	updateButtons();
}

void EditorGUITriggerWindow::SelectSubRequirement::notify(int message, gfx::GUIFrame *pSender)
{
	//Check for trigger variable updates.
	if(pSender == this)
	{
		if(message >= SelectWindow::SelectMessages_Data)
		{
			mSubRequirement = message - SelectWindow::SelectMessages_Data;
		}
	}
	SelectWindow::notify(message, pSender);
}

/******************************************************************
 * SelectValue
 ******************************************************************/

EditorGUITriggerWindow::SelectValue::SelectValue(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
	: SelectWindow(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	init();
}

EditorGUITriggerWindow::SelectValue::SelectValue(gfx::GUIFrame *pParent)
	: SelectWindow(pParent)
{
	init();
}

EditorGUITriggerWindow::SelectValue::SelectValue(const SelectValue &window)
	: SelectWindow(window)
{
	init();
}

EditorGUITriggerWindow::SelectValue::~SelectValue()
{

}

const EditorGUITriggerWindow::SelectValue &EditorGUITriggerWindow::SelectValue::operator=(const SelectValue &window)
{
	if(this != &window)
	{
		SelectWindow::operator=(window);
	}
	return *this;
}

void EditorGUITriggerWindow::SelectValue::update(float deltaTime)
{
	SelectWindow::update(deltaTime);
}

void EditorGUITriggerWindow::SelectValue::updateButtons()
{
	//Switch selections based on the current trigger type.
	switch(mType)
	{
		case TriggerBase::TriggerTypes_StartMap:
			for(int nButton = 0; nButton < mButtonCount; ++nButton)
			{
				mpButtons[nButton]->getTextFrame()->setText("");
				mpButtonData[nButton] = -1;
			}
			break;

		case TriggerBase::TriggerTypes_EndMap:
			for(int nButton = 0; nButton < mButtonCount; ++nButton)
			{
				mpButtons[nButton]->getTextFrame()->setText("");
				mpButtonData[nButton] = -1;
			}
			break;

		case TriggerBase::TriggerTypes_CreateUnit:
			for(int nButton = 0; nButton < mButtonCount; ++nButton)
			{
				if(nButton == 0)
				{
					mpButtons[nButton]->getTextFrame()->setText("Enemy");
					mpButtonData[nButton] = Actor::ActorType_Enemy;
				}
				else if(nButton == 1)
				{
					mpButtons[nButton]->getTextFrame()->setText("Environment");
					mpButtonData[nButton] = Actor::ActorType_StaticEnvironment;
				}
				else
				{
					mpButtons[nButton]->getTextFrame()->setText("");
					mpButtonData[nButton] = -1;
				}
			}
			break;

		case TriggerBase::TriggerTypes_DestroyUnit:
			for(int nButton = 0; nButton < mButtonCount; ++nButton)
			{
				mpButtons[nButton]->getTextFrame()->setText("");
				mpButtonData[nButton] = -1;
			}
			break;

		default:
			for(int nButton = 0; nButton < mButtonCount; ++nButton)
			{
				mpButtons[nButton]->getTextFrame()->setText("");
				mpButtonData[nButton] = -1;
			}
			break;
	}
}

void EditorGUITriggerWindow::SelectValue::init()
{
	mpTextTitle->setText("Select Value");
	updateButtons();
}

void EditorGUITriggerWindow::SelectValue::notify(int message, gfx::GUIFrame *pSender)
{
	//Check for trigger variable updates.
	if(pSender == this)
	{
		if(message >= SelectWindow::SelectMessages_Data)
		{
			mValue = message - SelectWindow::SelectMessages_Data;
		}
	}
	SelectWindow::notify(message, pSender);
}

/******************************************************************
 * SelectSubValue
 ******************************************************************/

EditorGUITriggerWindow::SelectSubValue::SelectSubValue(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
	: SelectWindow(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	init();
}

EditorGUITriggerWindow::SelectSubValue::SelectSubValue(gfx::GUIFrame *pParent)
	: SelectWindow(pParent)
{
	init();
}

EditorGUITriggerWindow::SelectSubValue::SelectSubValue(const SelectSubValue &window)
	: SelectWindow(window)
{
	init();
}

EditorGUITriggerWindow::SelectSubValue::~SelectSubValue()
{

}

const EditorGUITriggerWindow::SelectSubValue &EditorGUITriggerWindow::SelectSubValue::operator=(const SelectSubValue &window)
{
	if(this != &window)
	{
		SelectWindow::operator=(window);
	}
	return *this;
}

void EditorGUITriggerWindow::SelectSubValue::update(float deltaTime)
{
	SelectWindow::update(deltaTime);
}

void EditorGUITriggerWindow::SelectSubValue::updateButtons()
{
	//Switch selections based on the current trigger type and value.
	switch(mType)
	{
		case TriggerBase::TriggerTypes_StartMap:
			break;

		case TriggerBase::TriggerTypes_EndMap:
			break;

		case TriggerBase::TriggerTypes_CreateUnit:
			switch(mValue)
			{
				case Actor::ActorType_Enemy:
					for(int nButton = 0; nButton < mButtonCount; ++nButton)
					{
						if(nButton == 0)
						{
							mpButtons[nButton]->getTextFrame()->setText("Ant");
							mpButtonData[nButton] = Enemy::EnemyType_Ant;
						}
						else if(nButton == 1)
						{
							mpButtons[nButton]->getTextFrame()->setText("Kamikaze Ant");
							mpButtonData[nButton] = Enemy::EnemyType_KamikazeAnt;
						}
						else if(nButton == 2)
						{
							mpButtons[nButton]->getTextFrame()->setText("Fire Ant");
							mpButtonData[nButton] = Enemy::EnemyType_FireAnt;
						}
						else if(nButton == 3)
						{
							mpButtons[nButton]->getTextFrame()->setText("Tarantula");
							mpButtonData[nButton] = Enemy::EnemyType_Tarantula;
						}
						else if(nButton == 4)
						{
							mpButtons[nButton]->getTextFrame()->setText("Blackwidow");
							mpButtonData[nButton] = Enemy::EnemyType_BlackWidow;
						}
						else if(nButton == 5)
						{
							mpButtons[nButton]->getTextFrame()->setText("Jumper");
							mpButtonData[nButton] = Enemy::EnemyType_Jumper;
						}
						else if(nButton == 6)
						{
							mpButtons[nButton]->getTextFrame()->setText("Scarab");
							mpButtonData[nButton] = Enemy::EnemyType_Scarab;
						}
						else if(nButton == 7)
						{
							mpButtons[nButton]->getTextFrame()->setText("Plasma Beetle");
							mpButtonData[nButton] = Enemy::EnemyType_PlasmaBeetle;
						}
						else
						{
							mpButtons[nButton]->getTextFrame()->setText("");
							mpButtonData[nButton] = -1;
						}
					}
					break;

				case Actor::ActorType_StaticEnvironment:
					for(int nButton = 0; nButton < mButtonCount; ++nButton)
					{
						if(nButton == 0)
						{
							mpButtons[nButton]->getTextFrame()->setText("City Mall");
							mpButtonData[nButton] = StaticEnvironment::StaticEnvironmentType_CityMall;
						}
						else if(nButton == 1)
						{
							mpButtons[nButton]->getTextFrame()->setText("Corner Store");
							mpButtonData[nButton] = StaticEnvironment::StaticEnvironmentType_CornerStore;
						}
						else if(nButton == 2)
						{
							mpButtons[nButton]->getTextFrame()->setText("Green Office");
							mpButtonData[nButton] = StaticEnvironment::StaticEnvironmentType_GreenOffice;
						}
						else if(nButton == 3)
						{
							mpButtons[nButton]->getTextFrame()->setText("L Building");
							mpButtonData[nButton] = StaticEnvironment::StaticEnvironmentType_LBuidling;
						}
						else if(nButton == 4)
						{
							mpButtons[nButton]->getTextFrame()->setText("Scyscraper");
							mpButtonData[nButton] = StaticEnvironment::StaticEnvironmentType_Skyscraper;
						}
						else if(nButton == 5)
						{
							mpButtons[nButton]->getTextFrame()->setText("Taipei");
							mpButtonData[nButton] = StaticEnvironment::StaticEnvironmentType_Taipei;
						}
						else if(nButton == 6)
						{
							mpButtons[nButton]->getTextFrame()->setText("Tall Glass Building");
							mpButtonData[nButton] = StaticEnvironment::StaticEnvironmentType_TallGlassBuilding;
						}
						else if(nButton == 7)
						{
							mpButtons[nButton]->getTextFrame()->setText("Window Building");
							mpButtonData[nButton] = StaticEnvironment::StaticEnvironmentType_WindowBuilding;
						}
						else if(nButton == 8)
						{
							mpButtons[nButton]->getTextFrame()->setText("World Trade Building");
							mpButtonData[nButton] = StaticEnvironment::StaticEnvironmentType_WTFBuilding;
						}
						else
						{
							mpButtons[nButton]->getTextFrame()->setText("");
							mpButtonData[nButton] = -1;
						}
					}
					break;

				default:
					for(int nButton = 0; nButton < mButtonCount; ++nButton)
					{
						mpButtons[nButton]->getTextFrame()->setText("");
						mpButtonData[nButton] = -1;
					}
					break;
			}
			break;

		case TriggerBase::TriggerTypes_DestroyUnit:
			break;

		default:
			for(int nButton = 0; nButton < mButtonCount; ++nButton)
			{
				mpButtons[nButton]->getTextFrame()->setText("");
				mpButtonData[nButton] = -1;
			}
			break;
	}
}

void EditorGUITriggerWindow::SelectSubValue::init()
{
	mpTextTitle->setText("Select Subvalue");
	updateButtons();
}

void EditorGUITriggerWindow::SelectSubValue::notify(int message, gfx::GUIFrame *pSender)
{
	//Check for trigger variable updates.
	if(pSender == this)
	{
		if(message >= SelectWindow::SelectMessages_Data)
		{
			mSubValue = message - SelectWindow::SelectMessages_Data;
		}
	}
	SelectWindow::notify(message, pSender);
}

/******************************************************************
 * EditorGUITriggerWindow
 ******************************************************************/

EditorGUITriggerWindow::EditorGUITriggerWindow(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
	: GUIImage(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	mpTextTitle = NULL;
	mpSelectType = NULL;
	mpSelectRequirement = NULL;
	mpSelectSubRequirement = NULL;
	mpSelectValue = NULL;
	mpSelectSubValue = NULL;
	mpButtonType = NULL;
	mpButtonRequirement = NULL;
	mpButtonSubRequirement = NULL;
	mpButtonValue = NULL;
	mpButtonSubValue = NULL;
	mpFieldType = NULL;
	mpFieldRequirement = NULL;
	mpFieldSubRequirement = NULL;
	mpFieldValue = NULL;
	mpFieldSubValue = NULL;
	mpFieldMisc = NULL;
	mpTextMisc = NULL;
	mpButtonCreate = NULL;
	mpButtonClose = NULL;
	mTriggerType = -1;
	mTriggerRequirement = -1;
	mTriggerSubRequirement = -1;
	mTriggerValue = -1;
	mTriggerSubValue = -1;
	init();
}

EditorGUITriggerWindow::EditorGUITriggerWindow(gfx::GUIFrame *pParent)
	: GUIImage(pParent)
{
	mpTextTitle = NULL;
	mpSelectType = NULL;
	mpSelectRequirement = NULL;
	mpSelectSubRequirement = NULL;
	mpSelectValue = NULL;
	mpSelectSubValue = NULL;
	mpButtonType = NULL;
	mpButtonRequirement = NULL;
	mpButtonSubRequirement = NULL;
	mpButtonValue = NULL;
	mpButtonSubValue = NULL;
	mpFieldType = NULL;
	mpFieldRequirement = NULL;
	mpFieldSubRequirement = NULL;
	mpFieldValue = NULL;
	mpFieldSubValue = NULL;
	mpFieldMisc = NULL;
	mpTextMisc = NULL;
	mpButtonCreate = NULL;
	mpButtonClose = NULL;
	mTriggerType = -1;
	mTriggerRequirement = -1;
	mTriggerSubRequirement = -1;
	mTriggerValue = -1;
	mTriggerSubValue = -1;
	init();
}

EditorGUITriggerWindow::EditorGUITriggerWindow(const EditorGUITriggerWindow &window)
	: GUIImage(window)
{
	mpTextTitle = NULL;
	mpSelectType = NULL;
	mpSelectRequirement = NULL;
	mpSelectSubRequirement = NULL;
	mpSelectValue = NULL;
	mpSelectSubValue = NULL;
	mpButtonType = NULL;
	mpButtonRequirement = NULL;
	mpButtonSubRequirement = NULL;
	mpButtonValue = NULL;
	mpButtonSubValue = NULL;
	mpFieldType = NULL;
	mpFieldRequirement = NULL;
	mpFieldSubRequirement = NULL;
	mpFieldValue = NULL;
	mpFieldSubValue = NULL;
	mpFieldMisc = NULL;
	mpTextMisc = NULL;
	mpButtonCreate = NULL;
	mpButtonClose = NULL;
	mTriggerType = -1;
	mTriggerRequirement = -1;
	mTriggerSubRequirement = -1;
	mTriggerValue = -1;
	mTriggerSubValue = -1;
	init();
}

EditorGUITriggerWindow::~EditorGUITriggerWindow()
{

}

const EditorGUITriggerWindow &EditorGUITriggerWindow::operator=(const EditorGUITriggerWindow &window)
{
	if(this != &window)
	{
		GUIImage::operator=(window);
	}
	return *this;
}

void EditorGUITriggerWindow::update(float deltaTime)
{
	GUIImage::update(deltaTime);
}

void EditorGUITriggerWindow::setFlagVisible(bool flag)
{
	GUIImage::setFlagVisible(flag);

	//Hide all selection windows.
	mpSelectType->setFlagVisible(false);
	mpSelectRequirement->setFlagVisible(false);
	mpSelectSubRequirement->setFlagVisible(false);
	mpSelectValue->setFlagVisible(false);
	mpSelectSubValue->setFlagVisible(false);
}

void EditorGUITriggerWindow::init()
{
	mpTextTitle = myNew GUIText(this);
	mpTextTitle->setRelativePosition(D3DXVECTOR2(0.1f, 0.05f));
	mpTextTitle->setRelativeSize(D3DXVECTOR2(0.2f, 0.05f));
	mpTextTitle->setTextFormat(GUIText::TextFormat_ScaledLine);
	mpTextTitle->setText("Trigger Creation");
	addSubFrame(mpTextTitle);

	//Temporarily increase this frame's depth level in order to create
	//the selection windows two extra levels above.
	setDepthLevel(getDepthLevel() + 2);

	mpSelectType = myNew SelectType(this);
	mpSelectType->setRelativePosition(D3DXVECTOR2(0.0f, 0.0f));
	mpSelectType->setRelativeSize(D3DXVECTOR2(1.0f, 1.0f));
	mpSelectType->setColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	mpSelectType->setFlagVisible(false);
	addSubFrame(mpSelectType);

	mpSelectRequirement = myNew SelectRequirement(this);
	mpSelectRequirement->setRelativePosition(D3DXVECTOR2(0.0f, 0.0f));
	mpSelectRequirement->setRelativeSize(D3DXVECTOR2(1.0f, 1.0f));
	mpSelectRequirement->setColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	mpSelectRequirement->setFlagVisible(false);
	addSubFrame(mpSelectRequirement);

	mpSelectSubRequirement = myNew SelectSubRequirement(this);
	mpSelectSubRequirement->setRelativePosition(D3DXVECTOR2(0.0f, 0.0f));
	mpSelectSubRequirement->setRelativeSize(D3DXVECTOR2(1.0f, 1.0f));
	mpSelectSubRequirement->setColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	mpSelectSubRequirement->setFlagVisible(false);
	addSubFrame(mpSelectSubRequirement);

	mpSelectValue = myNew SelectValue(this);
	mpSelectValue->setRelativePosition(D3DXVECTOR2(0.0f, 0.0f));
	mpSelectValue->setRelativeSize(D3DXVECTOR2(1.0f, 1.0f));
	mpSelectValue->setColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	mpSelectValue->setFlagVisible(false);
	addSubFrame(mpSelectValue);

	mpSelectSubValue = myNew SelectSubValue(this);
	mpSelectSubValue->setRelativePosition(D3DXVECTOR2(0.0f, 0.0f));
	mpSelectSubValue->setRelativeSize(D3DXVECTOR2(1.0f, 1.0f));
	mpSelectSubValue->setColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	mpSelectSubValue->setFlagVisible(false);
	addSubFrame(mpSelectSubValue);

	//Decrease the depth level to normal.
	setDepthLevel(getDepthLevel() - 2);

	mpButtonType = myNew GUIButton(this);
	mpButtonType->setRelativePosition(D3DXVECTOR2(0.6f, 0.3f));
	mpButtonType->setRelativeSize(D3DXVECTOR2(0.3f, 0.05f));
	mpButtonType->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpButtonType->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpButtonType->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	mpButtonType->getTextFrame()->setText("Select Type");
	mpButtonType->enableText();
	addSubFrame(mpButtonType);

	mpButtonRequirement = myNew GUIButton(this);
	mpButtonRequirement->setRelativePosition(D3DXVECTOR2(0.6f, 0.36f));
	mpButtonRequirement->setRelativeSize(D3DXVECTOR2(0.3f, 0.05f));
	mpButtonRequirement->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpButtonRequirement->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpButtonRequirement->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	mpButtonRequirement->getTextFrame()->setText("Select Requirement");
	mpButtonRequirement->enableText();
	addSubFrame(mpButtonRequirement);

	mpButtonSubRequirement = myNew GUIButton(this);
	mpButtonSubRequirement->setRelativePosition(D3DXVECTOR2(0.6f, 0.42f));
	mpButtonSubRequirement->setRelativeSize(D3DXVECTOR2(0.3f, 0.05f));
	mpButtonSubRequirement->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpButtonSubRequirement->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpButtonSubRequirement->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	mpButtonSubRequirement->getTextFrame()->setText("Select Subrequirement");
	mpButtonSubRequirement->enableText();
	addSubFrame(mpButtonSubRequirement);

	mpButtonValue = myNew GUIButton(this);
	mpButtonValue->setRelativePosition(D3DXVECTOR2(0.6f, 0.48f));
	mpButtonValue->setRelativeSize(D3DXVECTOR2(0.3f, 0.05f));
	mpButtonValue->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpButtonValue->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpButtonValue->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	mpButtonValue->getTextFrame()->setText("Select Value");
	mpButtonValue->enableText();
	addSubFrame(mpButtonValue);

	mpButtonSubValue = myNew GUIButton(this);
	mpButtonSubValue->setRelativePosition(D3DXVECTOR2(0.6f, 0.54f));
	mpButtonSubValue->setRelativeSize(D3DXVECTOR2(0.3f, 0.05f));
	mpButtonSubValue->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpButtonSubValue->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpButtonSubValue->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	mpButtonSubValue->getTextFrame()->setText("Select Subvalue");
	mpButtonSubValue->enableText();
	addSubFrame(mpButtonSubValue);

	mpFieldType = myNew GUITextField(this);
	mpFieldType->setRelativePosition(D3DXVECTOR2(0.25f, 0.3f));
	mpFieldType->setRelativeSize(D3DXVECTOR2(0.3f, 0.05f));
	mpFieldType->setColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	mpFieldType->setInputMode(GUITextField::input_none);
	mpFieldType->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpFieldType->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpFieldType->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	addSubFrame(mpFieldType);

	mpFieldRequirement = myNew GUITextField(this);
	mpFieldRequirement->setRelativePosition(D3DXVECTOR2(0.25f, 0.36f));
	mpFieldRequirement->setRelativeSize(D3DXVECTOR2(0.3f, 0.05f));
	mpFieldRequirement->setColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	mpFieldRequirement->setInputMode(GUITextField::input_none);
	mpFieldRequirement->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpFieldRequirement->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpFieldRequirement->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	addSubFrame(mpFieldRequirement);

	mpFieldSubRequirement = myNew GUITextField(this);
	mpFieldSubRequirement->setRelativePosition(D3DXVECTOR2(0.25f, 0.42f));
	mpFieldSubRequirement->setRelativeSize(D3DXVECTOR2(0.3f, 0.05f));
	mpFieldSubRequirement->setColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	mpFieldSubRequirement->setInputMode(GUITextField::input_none);
	mpFieldSubRequirement->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpFieldSubRequirement->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpFieldSubRequirement->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	addSubFrame(mpFieldSubRequirement);

	mpFieldValue = myNew GUITextField(this);
	mpFieldValue->setRelativePosition(D3DXVECTOR2(0.25f, 0.48f));
	mpFieldValue->setRelativeSize(D3DXVECTOR2(0.3f, 0.05f));
	mpFieldValue->setColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	mpFieldValue->setInputMode(GUITextField::input_none);
	mpFieldValue->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpFieldValue->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpFieldValue->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	addSubFrame(mpFieldValue);

	mpFieldSubValue = myNew GUITextField(this);
	mpFieldSubValue->setRelativePosition(D3DXVECTOR2(0.25f, 0.54f));
	mpFieldSubValue->setRelativeSize(D3DXVECTOR2(0.3f, 0.05f));
	mpFieldSubValue->setColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	mpFieldSubValue->setInputMode(GUITextField::input_none);
	mpFieldSubValue->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpFieldSubValue->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpFieldSubValue->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	addSubFrame(mpFieldSubValue);

	mpFieldMisc = myNew GUITextField(this);
	mpFieldMisc->setRelativePosition(D3DXVECTOR2(0.25f, 0.6f));
	mpFieldMisc->setRelativeSize(D3DXVECTOR2(0.3f, 0.05f));
	mpFieldMisc->setColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	mpFieldMisc->setInputMode(GUITextField::input_key | GUITextField::input_ignore_enter | GUITextField::input_ignore_space);
	mpFieldMisc->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpFieldMisc->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpFieldMisc->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	addSubFrame(mpFieldMisc);

	mpTextMisc = myNew GUIButton(this);
	mpTextMisc->setRelativePosition(D3DXVECTOR2(0.6f, 0.6f));
	mpTextMisc->setRelativeSize(D3DXVECTOR2(0.3f, 0.05f));
	mpTextMisc->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpTextMisc->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpTextMisc->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	mpTextMisc->getTextFrame()->setText("Misc");
	mpTextMisc->enableText();
	addSubFrame(mpTextMisc);

	mpButtonClose = myNew GUIButton(this);
	mpButtonClose->setRelativePosition(D3DXVECTOR2(0.07f, 0.93f));
	mpButtonClose->setRelativeSize(D3DXVECTOR2(0.1f, 0.05f));
	mpButtonClose->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpButtonClose->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpButtonClose->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	mpButtonClose->getTextFrame()->setText("Close");
	mpButtonClose->enableText();
	addSubFrame(mpButtonClose);

	mpButtonCreate = myNew GUIButton(this);
	mpButtonCreate->setRelativePosition(D3DXVECTOR2(0.83f, 0.93f));
	mpButtonCreate->setRelativeSize(D3DXVECTOR2(0.1f, 0.05f));
	mpButtonCreate->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	mpButtonCreate->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	mpButtonCreate->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
	mpButtonCreate->getTextFrame()->setText("Create");
	mpButtonCreate->enableText();
	addSubFrame(mpButtonCreate);

	resetTriggerVariables();
	updateFieldAll();
}

void EditorGUITriggerWindow::notify(int message, gfx::GUIFrame *pSender)
{
	if(pSender == mpButtonType)
	{
		if(message == GUIFrame::BasicMessage_PressLeft)
		{
			switchWindow(false);
			mpSelectType->setFlagVisible(true);
		}
	}
	else if(pSender == mpSelectType)
	{
		if(message == SelectWindow::SelectMessages_Close)
		{
			switchWindow(true);
		}
		else if(message == SelectWindow::SelectMessages_Selection)
		{
			mTriggerType = mpSelectType->getType();
			mTriggerRequirement = -1;
			mTriggerSubRequirement = -1;
			mTriggerValue = -1;
			mTriggerSubValue = -1;
			updateFieldAll();
			switchWindow(true);
		}
	}
	else if(pSender == mpButtonRequirement)
	{
		if(message == GUIFrame::BasicMessage_PressLeft)
		{
			switchWindow(false);
			mpSelectRequirement->setFlagVisible(true);
		}
	}
	else if(pSender == mpSelectRequirement)
	{
		if(message == SelectWindow::SelectMessages_Close)
		{
			switchWindow(true);
		}
		else if(message == SelectWindow::SelectMessages_Selection)
		{
			mTriggerRequirement = mpSelectRequirement->getRequirement();
			updateFieldAll();
			switchWindow(true);
		}
	}
	else if(pSender == mpButtonSubRequirement)
	{
		if(message == GUIFrame::BasicMessage_PressLeft)
		{
			switchWindow(false);
			mpSelectSubRequirement->setFlagVisible(true);
		}
	}
	else if(pSender == mpSelectSubRequirement)
	{
		if(message == SelectWindow::SelectMessages_Close)
		{
			switchWindow(true);
		}
		else if(message == SelectWindow::SelectMessages_Selection)
		{
			mTriggerSubRequirement = mpSelectSubRequirement->getSubRequirement();
			mTriggerValue = -1;
			mTriggerSubValue = -1;
			updateFieldAll();
			switchWindow(true);
		}
	}
	else if(pSender == mpButtonValue)
	{
		if(message == GUIFrame::BasicMessage_PressLeft)
		{
			switchWindow(false);
			mpSelectValue->setFlagVisible(true);
		}
	}
	else if(pSender == mpSelectValue)
	{
		if(message == SelectWindow::SelectMessages_Close)
		{
			switchWindow(true);
		}
		else if(message == SelectWindow::SelectMessages_Selection)
		{
			mTriggerValue = mpSelectValue->getValue();
			mTriggerSubValue = -1;
			updateFieldAll();
			switchWindow(true);
		}
	}
	else if(pSender == mpButtonSubValue)
	{
		if(message == GUIFrame::BasicMessage_PressLeft)
		{
			switchWindow(false);
			mpSelectSubValue->setFlagVisible(true);
		}
	}
	else if(pSender == mpSelectSubValue)
	{
		if(message == SelectWindow::SelectMessages_Close)
		{
			switchWindow(true);
		}
		else if(message == SelectWindow::SelectMessages_Selection)
		{
			mTriggerSubValue = mpSelectSubValue->getSubValue();
			updateFieldAll();
			switchWindow(true);
		}
	}
	else if(pSender == mpButtonClose)
	{
		if(message == GUIFrame::BasicMessage_PressLeft)
		{
			resetTriggerVariables();
			updateFieldAll();
			setFlagVisible(false);
		}
	}
	else if(pSender == mpButtonCreate)
	{
		if(message == GUIFrame::BasicMessage_PressLeft)
		{
			//Creates a trigger and hides this window.
			createTrigger();
			resetTriggerVariables();
			updateFieldAll();
			setFlagVisible(false);
		}
	}
}

void EditorGUITriggerWindow::switchWindow(bool visible)
{
	mpButtonType->setFlagVisible(visible);
	mpButtonRequirement->setFlagVisible(visible);
	mpButtonSubRequirement->setFlagVisible(visible);
	mpButtonValue->setFlagVisible(visible);
	mpButtonSubValue->setFlagVisible(visible);
	mpButtonCreate->setFlagVisible(visible);
	mpButtonClose->setFlagVisible(visible);
}

void EditorGUITriggerWindow::resetTriggerVariables()
{
	mTriggerType = -1;
	mTriggerRequirement = -1;
	mTriggerSubRequirement = -1;
	mTriggerValue = -1;
	mTriggerSubValue = -1;
}

void EditorGUITriggerWindow::updateFieldAll()
{
	updateFieldType();
	updateFieldRequirement();
	updateFieldSubRequirement();
	updateFieldValue();
	updateFieldSubValue();

	//Update selection windows.
	mpSelectType->setType(mTriggerType);
	mpSelectRequirement->setType(mTriggerType);
	mpSelectSubRequirement->setType(mTriggerType);
	mpSelectValue->setType(mTriggerType);
	mpSelectSubValue->setType(mTriggerType);

	mpSelectType->setRequirement(mTriggerRequirement);
	mpSelectRequirement->setRequirement(mTriggerRequirement);
	mpSelectSubRequirement->setRequirement(mTriggerRequirement);
	mpSelectValue->setRequirement(mTriggerRequirement);
	mpSelectSubValue->setRequirement(mTriggerRequirement);

	mpSelectType->setSubRequirement(mTriggerSubRequirement);
	mpSelectRequirement->setSubRequirement(mTriggerSubRequirement);
	mpSelectSubRequirement->setSubRequirement(mTriggerSubRequirement);
	mpSelectValue->setSubRequirement(mTriggerSubRequirement);
	mpSelectSubValue->setSubRequirement(mTriggerSubRequirement);

	mpSelectType->setValue(mTriggerValue);
	mpSelectRequirement->setValue(mTriggerValue);
	mpSelectSubRequirement->setValue(mTriggerValue);
	mpSelectValue->setValue(mTriggerValue);
	mpSelectSubValue->setValue(mTriggerValue);

	mpSelectType->setSubValue(mTriggerValue);
	mpSelectRequirement->setSubValue(mTriggerValue);
	mpSelectSubRequirement->setSubValue(mTriggerValue);
	mpSelectValue->setSubValue(mTriggerValue);
	mpSelectSubValue->setSubValue(mTriggerValue);

	mpSelectType->updateButtons();
	mpSelectRequirement->updateButtons();
	mpSelectSubRequirement->updateButtons();
	mpSelectValue->updateButtons();
	mpSelectSubValue->updateButtons();
}

void EditorGUITriggerWindow::updateFieldType()
{
	switch(mTriggerType)
	{
		case TriggerBase::TriggerTypes_CreateUnit:
			mpFieldType->getTextFrame()->setText("Create Unit");
			mpTextMisc->getTextFrame()->setText("Amount");
			mpFieldMisc->getTextFrame()->setText("");
			break;

		default:
			mpFieldType->getTextFrame()->setText("");
			mpTextMisc->getTextFrame()->setText("");
			mpFieldMisc->getTextFrame()->setText("");
			break;
	}
}

void EditorGUITriggerWindow::updateFieldRequirement()
{
	switch(mTriggerType)
	{
		case TriggerBase::TriggerTypes_CreateUnit:
			switch(mTriggerRequirement)
			{
				case TriggerBase::TriggerRequirements_TimeSinceMapStart:
					mpFieldRequirement->getTextFrame()->setText("Time Since Map Start");
					break;

				case TriggerBase::TriggerRequirements_TimeSinceProgramStart:
					mpFieldRequirement->getTextFrame()->setText("Time Since Program Start");
					break;

				case TriggerBase::TriggerRequirements_EnteredSourceBV:
					mpFieldRequirement->getTextFrame()->setText("Entered Source");
					break;

				case TriggerBase::TriggerRequirements_ExitedSourceBV:
					mpFieldRequirement->getTextFrame()->setText("Exited Source");
					break;

				case TriggerBase::TriggerRequirements_EnteredDestinationBV:
					mpFieldRequirement->getTextFrame()->setText("Entered Destination");
					break;

				case TriggerBase::TriggerRequirements_ExitedDestinationBV:
					mpFieldRequirement->getTextFrame()->setText("Exited Destination");
					break;

				case TriggerBase::TriggerRequirements_ObjectCreated:
					mpFieldRequirement->getTextFrame()->setText("Object Created");
					break;

				case TriggerBase::TriggerRequirements_ObjectDestroyed:
					mpFieldRequirement->getTextFrame()->setText("Object Destroyed");
					break;

				default:
					mpFieldRequirement->getTextFrame()->setText("");
					break;
			}
			break;

		default:
			mpFieldRequirement->getTextFrame()->setText("");
			break;
	}
}

void EditorGUITriggerWindow::updateFieldSubRequirement()
{
	switch(mTriggerType)
	{
		case TriggerBase::TriggerTypes_CreateUnit:
			switch(mTriggerRequirement)
			{
				case TriggerBase::TriggerRequirements_TimeSinceMapStart:
					break;

				case TriggerBase::TriggerRequirements_TimeSinceProgramStart:
					break;

				case TriggerBase::TriggerRequirements_EnteredSourceBV:
					switch(mTriggerSubRequirement)
					{
						case TriggerBase::TriggerSubRequirements_ObjectID:
							mpFieldSubValue->getTextFrame()->setText("Object ID");
							break;

						default:
							mpFieldSubRequirement->getTextFrame()->setText("");
							break;
					}
					break;

				case TriggerBase::TriggerRequirements_ExitedSourceBV:
					switch(mTriggerSubRequirement)
					{
						case TriggerBase::TriggerSubRequirements_ObjectID:
							mpFieldSubValue->getTextFrame()->setText("Object ID");
							break;

						default:
							mpFieldSubRequirement->getTextFrame()->setText("");
							break;
					}
					break;

				case TriggerBase::TriggerRequirements_EnteredDestinationBV:
					switch(mTriggerSubRequirement)
					{
						case TriggerBase::TriggerSubRequirements_ObjectID:
							mpFieldSubValue->getTextFrame()->setText("Object ID");
							break;

						default:
							mpFieldSubRequirement->getTextFrame()->setText("");
							break;
					}
					break;

				case TriggerBase::TriggerRequirements_ExitedDestinationBV:
					switch(mTriggerSubRequirement)
					{
						case TriggerBase::TriggerSubRequirements_ObjectID:
							mpFieldSubValue->getTextFrame()->setText("Object ID");
							break;

						default:
							mpFieldSubRequirement->getTextFrame()->setText("");
							break;
					}
					break;

				case TriggerBase::TriggerRequirements_ObjectCreated:
					switch(mTriggerSubRequirement)
					{
						case TriggerBase::TriggerSubRequirements_ObjectID:
							mpFieldSubValue->getTextFrame()->setText("Object ID");
							break;

						default:
							mpFieldSubRequirement->getTextFrame()->setText("");
							break;
					}
					break;

				case TriggerBase::TriggerRequirements_ObjectDestroyed:
					switch(mTriggerSubRequirement)
					{
						case TriggerBase::TriggerSubRequirements_ObjectID:
							mpFieldSubValue->getTextFrame()->setText("Object ID");
							break;

						default:
							break;
					}
					break;

				default:
					mpFieldSubRequirement->getTextFrame()->setText("");
					break;
			}
			break;

		default:
			mpFieldSubRequirement->getTextFrame()->setText("");
			break;
	}
}

void EditorGUITriggerWindow::updateFieldValue()
{
	switch(mTriggerType)
	{
		case TriggerBase::TriggerTypes_CreateUnit:
			//Trigger value is equal to actor type.
			switch(mTriggerValue)
			{
				case Actor::ActorType_Enemy:
					mpFieldValue->getTextFrame()->setText("Enemy");
					break;

				case Actor::ActorType_StaticEnvironment:
					mpFieldValue->getTextFrame()->setText("Environment");
					break;

				default:
					mpFieldValue->getTextFrame()->setText("");
					break;
			}
			break;

		default:
			mpFieldValue->getTextFrame()->setText("");
			break;
	}
}

void EditorGUITriggerWindow::updateFieldSubValue()
{
	switch(mTriggerType)
	{
		case TriggerBase::TriggerTypes_CreateUnit:
			//Trigger value is equal to actor type.
			switch(mTriggerValue)
			{
				case Actor::ActorType_Enemy:
					switch(mTriggerSubValue)
					{
						case Enemy::EnemyType_Ant:
							mpFieldSubValue->getTextFrame()->setText("Ant");
							break;

						case Enemy::EnemyType_KamikazeAnt:
							mpFieldSubValue->getTextFrame()->setText("Kamikaze Ant");
							break;

						case Enemy::EnemyType_FireAnt:
							mpFieldSubValue->getTextFrame()->setText("Fire Ant");
							break;

						case Enemy::EnemyType_Tarantula:
							mpFieldSubValue->getTextFrame()->setText("Tarantula");
							break;

						case Enemy::EnemyType_BlackWidow:
							mpFieldSubValue->getTextFrame()->setText("Blackwidow");
							break;

						case Enemy::EnemyType_Jumper:
							mpFieldSubValue->getTextFrame()->setText("Jumper");
							break;

						case Enemy::EnemyType_Scarab:
							mpFieldSubValue->getTextFrame()->setText("Scarab");
							break;

						case Enemy::EnemyType_PlasmaBeetle:
							mpFieldSubValue->getTextFrame()->setText("Plasmabeetle");
							break;

						default:
							mpFieldSubValue->getTextFrame()->setText("");
							break;
					}
					break;

				case Actor::ActorType_StaticEnvironment:
					switch(mTriggerSubValue)
					{
						case StaticEnvironment::StaticEnvironmentType_CityMall:
							mpFieldSubValue->getTextFrame()->setText("City Mall");
							break;

						case StaticEnvironment::StaticEnvironmentType_CornerStore:
							mpFieldSubValue->getTextFrame()->setText("Corner Store");
							break;

						case StaticEnvironment::StaticEnvironmentType_GreenOffice:
							mpFieldSubValue->getTextFrame()->setText("Green Office");
							break;

						case StaticEnvironment::StaticEnvironmentType_LBuidling:
							mpFieldSubValue->getTextFrame()->setText("L Building");
							break;

						case StaticEnvironment::StaticEnvironmentType_Skyscraper:
							mpFieldSubValue->getTextFrame()->setText("Scyscraper");
							break;

						case StaticEnvironment::StaticEnvironmentType_StandAloneBuilding:
							mpFieldSubValue->getTextFrame()->setText("Stand Alone Building");
							break;

						case StaticEnvironment::StaticEnvironmentType_Taipei:
							mpFieldSubValue->getTextFrame()->setText("Taipei");
							break;

						case StaticEnvironment::StaticEnvironmentType_TallGlassBuilding:
							mpFieldSubValue->getTextFrame()->setText("Tall Glass Building");
							break;

						case StaticEnvironment::StaticEnvironmentType_TurningTorso:
							mpFieldSubValue->getTextFrame()->setText("Turning Torse");
							break;

						case StaticEnvironment::StaticEnvironmentType_WindowBuilding:
							mpFieldSubValue->getTextFrame()->setText("Window Building");
							break;

						case StaticEnvironment::StaticEnvironmentType_WTFBuilding:
							mpFieldSubValue->getTextFrame()->setText("World Trade Building");
							break;

						default:
							mpFieldSubValue->getTextFrame()->setText("");
							break;
					}
					break;

				default:
					mpFieldSubValue->getTextFrame()->setText("");
					break;
			}
			break;

		default:
			mpFieldSubValue->getTextFrame()->setText("");
			break;
	}
}

bool EditorGUITriggerWindow::createTrigger()
{
	//Retrieve the misc value.
	int misc = 0;
	stringstream sstream;
	sstream << mpFieldMisc->getTextFrame()->getText();
	sstream >> misc;

	D3DXVECTOR3 sourcePosition = EditorHandler::getInstance()->getMousePlanePosition();
	D3DXVECTOR3 sourceRotation(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 sourceScale(50.0f, 70.0f, 50.0f);
	D3DXVECTOR3 destinationPosition = EditorHandler::getInstance()->getMousePlanePosition();
	D3DXVECTOR3 destinationRotation(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 destinationScale(50.0f, 70.0f, 50.0f);

	//Send the creation event.
	safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_TriggerCreate(mTriggerType, mTriggerRequirement,
		mTriggerSubRequirement, mTriggerValue, mTriggerSubValue, misc, sourcePosition,
		sourceRotation, sourceScale, destinationPosition, destinationRotation, destinationScale, true)));

	//Send the set visible event.
	//safeThreadSafeQueueEvent(EventDataPtr(myNew logic::EvtData_To_TriggerShow()));
	return true;
}