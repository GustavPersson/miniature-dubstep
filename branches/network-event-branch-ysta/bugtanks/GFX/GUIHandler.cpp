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

#include "GUIHandler.h"

using namespace gfx;

GUIHandler::GUIHandler() : EventListener()
{

}

GUIHandler::GUIHandler(const GUIHandler &handler) : EventListener()
{

}

GUIHandler::~GUIHandler()
{

}

const GUIHandler &GUIHandler::operator=(const GUIHandler &handler)
{
	if(this != &handler)
	{

	}
	return *this;
}

const char *GUIHandler::getName()
{
	return "GUIHandler";
}

bool GUIHandler::handleEvent(const EventData &mEvent)
{
	return false;
}