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

#include "HUDHandler.h"

using namespace logic;
using namespace gfx;

HUDHandler *HUDHandler::mpHUDInstance = NULL;

HUDHandler::HUDHandler()
{
	//Initialize HUD GUI handler.
	DX10Module *module = DX10Module::getInstance();
	module->loadGUISpriteInstance(2, "HUDGUI", 5000);
	long cW = module->getClientWidth();
	long cH = module->getClientHeight();
	mpHUDGUI = myNew HUDGUI(module->getGUISpriteInstanceBase("HUDGUI"), cW, cH);
}

HUDHandler::HUDHandler(const HUDHandler &handler)
{

}

const HUDHandler &HUDHandler::operator=(const HUDHandler &handler)
{
	if(this != &handler)
	{

	}
	return *this;
}

HUDHandler::~HUDHandler()
{
	mpHUDInstance = NULL;
}

HUDHandler *HUDHandler::getInstance()
{
	if(!mpHUDInstance)
	{
		mpHUDInstance = myNew HUDHandler();
	}
	return mpHUDInstance;
}

void HUDHandler::update(float deltaTime)
{
	mpHUDGUI->update(deltaTime);
}

void HUDHandler::updateGFX(float deltaTime)
{
	mpHUDGUI->updateFrames(deltaTime);
}

void HUDHandler::show()
{
	mpHUDGUI->show();
}

void HUDHandler::hide()
{
	mpHUDGUI->hide();
}