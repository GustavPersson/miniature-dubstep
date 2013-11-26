/**
* @file GUIScrollBar.h
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

#ifndef __G_U_I_SCROLL_BAR_H__
#define __G_U_I_SCROLL_BAR_H__

#include "../GFX/GUIImage.h"
#include "GUIButton.h"

namespace logic
{

/**
* GUIScrollBar
*/
class GUIScrollBar : public gfx::GUIImage
{
public:
	GUIScrollBar(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

	GUIScrollBar(gfx::GUIFrame *pParent);

	GUIScrollBar(const GUIScrollBar &scrollbar);
	virtual ~GUIScrollBar(void);


	const GUIScrollBar &operator=(const GUIScrollBar &scrollbar);

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

	/**
	* setNumberOfIndices set the number of indices you can scroll to.
	* @param indices is the number of indices you can scroll.
	*/
	void setNumberOfIndices(int indices);

	/**
	* getNumberOfIndices get the number of indices you can scroll.
	* @return the number of indices you can scroll.
	*/
	int getNumberOfIndices()const;

	/**
	* setCurrentIndexPosition set the current index position, this a float becaus you can
	* be somewhere in between two indices aswell.
	* @param indexPosition the position to set.
	*/
	void setCurrentIndexPosition(float indexPosition);

	/**
	* getCurrentIndexPosition get the current index position.
	* @return the current index position.
	*/
	float getCurrentIndexPosition()const;

	virtual void notify(int message, GUIFrame *pSender);
protected:
	
	virtual void init();

private:
	int mNumberOfIndices;
	float mCurrentIndexPosition;

	float mLengthOfSlide;
	float mButtonWidth;

	GUIButton *mButtonUp, *mButtonDown, *mSlideBar;
	D3DXVECTOR2 mOldSlideBarPos, mRefPoint, mDragPoint;

	bool mHoldSlideBar;
};
}

#endif