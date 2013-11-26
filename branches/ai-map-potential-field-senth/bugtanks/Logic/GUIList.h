/**
* @file GUIList.h
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

#ifndef __G_U_I_LIST_H__
#define __G_U_I_LIST_H__

#include <list>
#include <string>
#include "../GFX/GUIImage.h"
#include "GUIScrollBar.h"
#include "GUIButton.h"

namespace logic
{

/**
* GUIList
*/
class GUIList : public gfx::GUIImage
{
public:
	GUIList(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

	GUIList(gfx::GUIFrame *pParent);

	GUIList(const GUIList &list);
	virtual ~GUIList(void);

	const GUIList &operator=(const GUIList &list);

	virtual void update(float deltaTime);
	virtual void setFlagRender(bool render);

	virtual void notify(int message, GUIFrame *pSender);

	virtual void showNumberOfElements(int n);
	virtual void addItem(std::string item);
	virtual void removeItem(std::string item);
	virtual void clear();
	std::string getSelected()const;
	int getSize()const;

protected:
	virtual void init();

	void rebuildListRequired();
	bool getFlagRebuildListRequired();
private:

	int mElementsVisible;
	int mScrollPos;
	
	std::string mSelected;
	std::list<std::string> mItemList;


	static const int MAX_VISIBLE_ELEMENTS=50;
	GUIButton *mElementButtons[MAX_VISIBLE_ELEMENTS];

	bool mFlagRebuildListRequired;
	bool mNewFlagRebuildListRequired;

	GUIScrollBar *mScrollBar;
};
}

#endif