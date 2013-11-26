/**
* @file GUITextField.h
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

#ifndef __G_U_I_TEXT_FIELD_H__
#define __G_U_I_TEXT_FIELD_H__

#include <sstream>
#include "../GFX/GUIComboFrame.h"
#include "GUIScrollBar.h"

namespace logic
{

/**
* GUITextField
*/
class GUITextField : public gfx::GUIComboFrame
{
public:

	enum input_mode
	{
		input_none=0,
		input_key=1,
		input_ignore_enter=2,
		input_ignore_space=4
	};

	GUITextField(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

	GUITextField(gfx::GUIFrame *pParent);

	GUITextField(const GUITextField &textField);
	virtual ~GUITextField(void);

	const GUITextField &operator=(const GUITextField &textField);

	virtual void update(float deltaTime);


	virtual void onKeyDown(int letterKeys, int keys);


	/**
	* Show scrollbar in the right side of the textfield.
	*/
	void enableScrollBar();

	/**
	* Disable the scrollbar.
	*/
	void disableScrollBar();

	/**
	* Check if the scrollbar is enabled.
	* @return true or false.
	*/
	bool isScrollBarEnabled()const;

	/**
	* Set the input mode for the textfield.
	* @param mode is a bit-flag. The bit-flags are (input_key | input_ignore_enter | input_ignore_space)
	*/
	void setInputMode(int mode);

protected:
	virtual void init();

private:
	bool mShowScrollBar;
	int mMode;
	GUIScrollBar *mScrollBar;
};
}

#endif