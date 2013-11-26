/**
* @file GUIMainFrame
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

#ifndef __G_U_I_MAIN_FRAME_H__
#define __G_U_I_MAIN_FRAME_H__

#include "../GFX/GUIImage.h"

namespace logic
{

/**
* GUIMainFrame is the main frame for the GUI. The main purpose of this class
* is to register mouse events over the whole client screen area.
*/
class GUIMainFrame : public gfx::GUIImage
{
public:
	GUIMainFrame(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

	GUIMainFrame(gfx::GUIFrame *pParent);

	GUIMainFrame(const GUIMainFrame &mainframe);
	virtual ~GUIMainFrame(void);

	const GUIMainFrame &operator=(const GUIMainFrame &mainframe);

	
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
	
	virtual void addSubFrame(GUIFrame *pSubFrame);
protected:
	virtual void init();

private:

};
}

#endif