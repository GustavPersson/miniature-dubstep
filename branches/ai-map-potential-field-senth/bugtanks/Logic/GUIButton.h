/**
* @file GUIButton.h
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

#ifndef __G_U_I_BUTTON_H__
#define __G_U_I_BUTTON_H__

#include <list>

#include "../GFX/GUIComboFrame.h"
#include "GUIButtonListener.h"

namespace logic
{

enum GUIButtonState
{
	GUIB_DEFAULT=0,
	GUIB_MOUSE_OVER,
	GUIB_MOUSE_OVER_AND_PRESS,
	GUIB_PRESSED,
};
enum GUIButtonSwitchState
{
	GUIB_SWITCH_OFF=0,
	GUIB_SWITCH_ON,
};
enum GUI_Alignments
{
	GUI_Alignment_Center=0,
	GUI_Alignment_CenterLeft,
	GUI_Alignment_CenterRight,
	GUI_Alignment_TopCenter,
	GUI_Alignment_TopLeft,
	GUI_Alignment_TopRight,
	GUI_Alignment_BottomCenter,
	GUI_Alignment_BottomLeft,
	GUI_Alignment_BottomRight,
};
/**
* Button
*/
class GUIButton : public gfx::GUIComboFrame
{
public:
	GUIButton(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

	GUIButton(gfx::GUIFrame *pParent);

	GUIButton(const GUIButton &button);
	virtual ~GUIButton(void);

	const GUIButton &operator=(const GUIButton &button);

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
	* Get the state of the button. The different states are: 
	* GUIB_DEFAULT,
	* GUIB_MOUSE_OVER,
	* GUIB_MOUSE_OVER_AND_PRESS,
	* GUIB_PRESSED.
	* @return the current state.
	*/
	GUIButtonState getButtonState()const;

	/**
	* Set the button state.
	*/
	void setButtonState(GUIButtonState state);

	/**
	* Set the switch.
	* @param on, can be wither true or false.
	*/
	void setSwitch(bool on);

	/**
	* Toggle the state ( mSwitchON = !mSwitchON ).
	*/
	void toggleSwitch();

	/**
	* Check if the switch is on.
	*/
	bool isSwitchON()const;

	/**
	* Set the offset to the texture array where the buttons begin.
	* @param offset is the offset number.
	*/
	void setTextureIndexOffset(int offset);


	/**
	* Set the button text alignment.
	* The alignment flags are:
	* GUI_Alignment_Center,
	* GUI_Alignment_CenterLeft,
	* GUI_Alignment_CenterRight,
	* GUI_Alignment_TopCenter
	* GUI_Alignment_TopLeft,
	* GUI_Alignment_TopRight,
	* GUI_Alignment_BottomCenter,
	* GUI_Alignment_BottomLeft,
	* GUI_Alignment_BottomRight,
	* @param alignment is the alignmentflag that sould be used.
	*/
	void setTextAlignment(GUI_Alignments alignment);

	/**
	* Add a button listener to this button. if you register the same listener twice
	* the listener will receive the same button event twice also.
	* @param listener is a pointer to a listener object.
	*/
	void addButtonListener(GUIButtonListener *listener);

	/**
	* Remove a listener from this button.
	* @param listener is a pointer to a listener object.
	*/
	void removeListener(GUIButtonListener *listener);

	/**
	* Enable text to be displayed on the button.
	*/
	void enableText();

	/**
	* Enable text to be displayed on the button.
	*/
	void disableText();

protected:
	virtual void init();

private:
	GUIButtonState mState;
	bool mSwitchON, mTextEnable;
	int mTexIndexOffset;

	GUI_Alignments mTextAlignment;

	std::list<GUIButtonListener*> mButtonListeners;
};
}

#endif