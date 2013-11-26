/**
* @file TestApp
* @author Martin Wexö Olsson <mawm06@student.bth.se>
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

#ifndef __TEST_APP_H__
#define __TEST_APP_H__

#include "../Utilities/Macros.h"
#include "../Logic/Application.h"
#include "../Logic/GUIMenuHandler.h"
#include "../Logic/GUIButton.h"
#include "../Logic/GUIScrollBar.h"
#include "../Logic/GUIRadioButtons.h"
#include "../Logic/GUIList.h"
#include "../Logic/GUITextField.h"
#include "../logic/GUIHandlerStartMenu.h"

namespace guitest
{

class myButtonListener : public logic::GUIButtonListener
{
public:
	myButtonListener(logic::GUIList *list, logic::GUITextField* textField){mGUIList = list; mGUITextField = textField;}
	~myButtonListener() {}

	void onMouseLeftUp(logic::GUIButton* button, long mX, long mY)
	{
		std::cout << button->getTextFrame()->getText() << " says: ouch! dont pich me!" << std::endl;
		button->getTextFrame()->setText("outch!");
		button->toggleSwitch();

		if(mGUITextField->isScrollBarEnabled())
		{
			mGUITextField->disableScrollBar();
		}
		else
		{
			mGUITextField->enableScrollBar();
		}

		mGUIList->addItem(std::string("ouch "));
	}

	void onKeyDown(logic::GUIButton* button, int letterKeys, int keys)
	{
		button->getTextFrame()->addCharFromKey(letterKeys, keys);
		
	}

private:
	logic::GUIList *mGUIList;
	logic::GUITextField *mGUITextField;
};


/**
* TestApp
*/
class TestApp : public logic::Application
{
public:
	static void createInstance();
	virtual void gameLoop(float deltaTime);
	void finalGameInitialization();
	void loadMeshes();
	void createObjects();
	
private:
	/**
	* Constructor. Initializes local pointers and creates a new instance.
	*/
	TestApp();

	logic::GUIScrollBar* mScrollBar;
	logic::GUIButton *mButton;
	logic::GUIRadioButtons *mRadioButtons;
	logic::GUIMenuHandler *mMenuHandler;
	logic::GUIList *mGUIList;
	logic::GUITextField *mTextField;

	myButtonListener *mButtonListener;

	logic::GUIHandlerStartMenu* mGUIHandlerStartMenu;

};
}

#endif