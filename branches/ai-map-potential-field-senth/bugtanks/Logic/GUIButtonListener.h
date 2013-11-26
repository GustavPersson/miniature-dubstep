/**
* @file GUIButtonListener.h
* @author  Martin Wexö Olsson <mawm06@student.bth.se>
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

#ifndef __G_U_I_BUTTON_LISTENER_H__
#define __G_U_I_BUTTON_LISTENER_H__

namespace logic
{

class GUIButton;

/**
* GUIButtonListener is an abstract class that. The GUIButton has a list with this kind of listeners
* and when the button gets an event of any of the below types it will send it to the listeners aswell.
* Define you own listener to control what happens for example when you click a button.
*/
class GUIButtonListener
{
public:
	virtual ~GUIButtonListener(){}

	/**
	* This function is called when the mouse rolls over the button.
	* @param button is the button where the event came from.
	* @param mX is the mouse X coordinate.
	* @param mY is the mouse Y coordinate.
	*/
	virtual void onMouseEnter(GUIButton* button, long mX, long mY)
	{
	
	}

	/**
	* This function is called when the mouse exits the button.
	* @param button is the button where the event came from.
	* @param mX is the mouse X coordinate.
	* @param mY is the mouse Y coordinate.
	*/
	virtual void onMouseExit(GUIButton* button, long mX, long mY)
	{
	
	}

	/**
	* This function is called when the mouse rolls over the button.
	* @param button is the button where the event came from.
	* @param mX is the mouse X coordinate.
	* @param mY is the mouse Y coordinate.
	*/
	virtual void onMouseMove(GUIButton* button, long mX, long mY)
	{

	}

	/**
	* This function is called when the mouse rolls over the button.
	* @param button is the button where the event came from.
	* @param mX is the mouse X coordinate.
	* @param mY is the mouse Y coordinate.
	*/
	virtual void onMouseLeftDown(GUIButton* button, long mX, long mY)
	{

	}

	/**
	* This function is called when the mouse rolls over the button.
	* @param button is the button where the event came from.
	* @param mX is the mouse X coordinate.
	* @param mY is the mouse Y coordinate.
	*/
	virtual void onMouseMiddleDown(GUIButton* button, long mX, long mY)
	{

	}

	/**
	* This function is called when the mouse rolls over the button.
	* @param button is the button where the event came from.
	* @param mX is the mouse X coordinate.
	* @param mY is the mouse Y coordinate.
	*/
	virtual void onMouseRightDown(GUIButton* button, long mX, long mY)
	{

	}

	/**
	* This function is called when the mouse rolls over the button.
	* @param button is the button where the event came from.
	* @param mX is the mouse X coordinate.
	* @param mY is the mouse Y coordinate.
	*/
	virtual void onMouseLeftUp(GUIButton* button, long mX, long mY)
	{

	}

	/**
	* This function is called when the mouse rolls over the button.
	* @param button is the button where the event came from.
	* @param mX is the mouse X coordinate.
	* @param mY is the mouse Y coordinate.
	*/
	virtual void onMouseMiddleUp(GUIButton* button, long mX, long mY)
	{

	}

	/**
	* This function is called when the mouse rolls over the button.
	* @param button is the button where the event came from.
	* @param mX is the mouse X coordinate.
	* @param mY is the mouse Y coordinate.
	*/
	virtual void onMouseRightUp(GUIButton* button, long mX, long mY)
	{

	}

	/**
	* This function is called when a keyboard key is pressed.
	* @param button is the button where the event came from.
	* @param letterKeys Integer containing letter key status information.
	* @param keys Integer containing key status information.
	*/
	virtual void onKeyDown(GUIButton* button, int letterKeys, int keys)
	{


	}
};
}

#endif