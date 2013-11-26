/**
* @file GUIMenuHandler.h
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

#ifndef __G_U_I_MENU_HANDLER_H__
#define __G_U_I_MENU_HANDLER_H__

#include <list>
#include "../GFX/GUIHandler.h"
#include "../GFX/GUIFrame.h"
#include "GUIMainFrame.h"

namespace logic
{

/**
* GUIMenuHandler.h
*/
class GUIMenuHandler : public gfx::GUIHandler
{
public:
	GUIMenuHandler(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight);
	GUIMenuHandler(const GUIMenuHandler &handler);
	
	virtual ~GUIMenuHandler(void);

	const GUIMenuHandler &operator=(const GUIMenuHandler &handler);

	virtual const char *getName();

	void setFlagRender(bool renderFlag);

	void addMenuItem(gfx::GUIFrame* menuItem);
	
protected:
	virtual void init();

private:
	std::list<gfx::GUIFrame*> mMenuItems;
};
}

#endif