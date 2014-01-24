#pragma once
/**
* @file
* @author Gustav Persson <gustav@alltomminrov.se>
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
*/
#ifndef __GUI_FRAME_START_MENU_H__
#define __GUI_FRAME_START_MENU_H__
#include "../GFX/GUIImage.h"
#include "../GFX/GUIText.h"
#include "GUISubFrameStartMenu.h"

namespace logic
{

class GUIFrameStartMenu : public gfx::GUIImage
{
public:
	GUIFrameStartMenu(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

	GUIFrameStartMenu(gfx::GUIFrame *pParent);

	GUIFrameStartMenu(const GUIFrameStartMenu &frame);
	virtual ~GUIFrameStartMenu(void);


	const GUIFrameStartMenu &operator=(const GUIFrameStartMenu &frame);

	virtual void update(float deltaTime);
	virtual void setFlagRender(bool render);

	virtual void onMouseEnter(long mX, long mY);
	virtual void onMouseExit(long mX, long mY);
	virtual void onMouseMove(long mX, long mY);
	virtual void onMouseLeftDown(long mX, long mY);
	virtual void onMouseMiddleDown(long mX, long mY);
	virtual void onMouseRightDown(long mX, long mY);
	virtual void onMouseLeftUp(long mX, long mY);
	virtual void onMouseMiddleUp(long mX, long mY);
	virtual void onMouseRightUp(long mX, long mY);
	virtual void onKeyDown(int letterKeys, int keys);

protected:
	virtual void init();

private:
	gfx::GUIText* mpText;
	GUISubFrameStartMenu* mpSubFrame;
};

}
#endif