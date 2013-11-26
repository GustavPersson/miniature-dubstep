#pragma once
/**
* @file
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
* 
* @section DESCRIPTION
*/
#ifndef __GUI_HANDLER_START_MENU_H__
#define __GUI_HANDLER_START_MENU_H__

#include <map>
#include <string>
#include "../GFX/GUIHandler.h"
#include "../GFX/GUIFrame.h"
#include "../GFX/GUIImage.h"
#include "../Network/Network.h"
#include "GUIButton.h"
#include "GUIButtonListener.h"
#include "GUIScrollBar.h"
#include "GUIRadioButtons.h"
#include "GUIList.h"
#include "GUITextField.h"



namespace logic
{

class Application;
/**
*
* The GUI handler for start and join menus.
*/
class GUIHandlerStartMenu : public gfx::GUIHandler
{
public:
	GUIHandlerStartMenu(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight);
	GUIHandlerStartMenu(const GUIHandlerStartMenu &handler);
	
	virtual ~GUIHandlerStartMenu(void);

	const GUIHandlerStartMenu &operator=(const GUIHandlerStartMenu &handler);

	virtual void updateFrames(float deltaTime);

	virtual const char *getName();

	void setFlagRender(bool renderFlag);

	GUITextField* getChatBox()const;
	void goToMenu(std::string menuName);
	void resetFields();
protected:
	virtual void init();

private:
	class myButtonListener : public GUIButtonListener
	{
	public:
		myButtonListener(GUIHandlerStartMenu *startMenu);
		~myButtonListener();
		void onMouseLeftUp(logic::GUIButton* button, long mX, long mY);
		void onKeyDown(logic::GUIButton* button, int letterKeys, int keys);
	private:
		GUIHandlerStartMenu *mStartMenu;
	};
	std::map<std::string, gfx::GUIFrame*> mMenus;
	gfx::GUIFrame *mCurrentMenu;
	myButtonListener *mButtonListener;

	// Sign in menu
	GUITextField *mSignInTextField;
	GUIButton *mSignInButton;

	// Main menu
	std::map<std::string, network::ServerInfo> mGameServers;
	GUIList *mGameList;
	GUIButton *mCreateButton,
			  *mJoinButton,
			  *mMapEditButton,
			  *mQuitButton;

	// Loby Menu
	GUIRadioButtons *mTankSelect;
	GUITextField *mPlayer1;
	GUITextField *mPlayer2;
	GUITextField *mPlayer3;
	GUITextField *mPlayer4;
	GUITextField *mTextBattleTank;
	GUITextField *mTextHoverTank;
	GUITextField *mChatBox;
	GUITextField *mTextToSend;
	GUITextField *mLevelSelectText;
	GUITextField *mLevelSelect;
	GUIButton *mSendButton,
		      *mReadyButton;

	Application				*mApplication;
	network::Network		*mNetwork;
	network::ServerLookup	*mServerLookup;
};
}

#endif
