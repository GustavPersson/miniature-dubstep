/**
* @file GUIRadioButtons.h
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

#ifndef __G_U_I_RADIO_BUTTONS_H__
#define __G_U_I_RADIO_BUTTONS_H__

#include <list>
#include <map>
#include <string>

#include "../GFX/GUIImage.h"
#include "GUIButton.h"

namespace logic
{

/**
* GUIRadioButtons is a class that connects a set of regular GUIButtons and make them behave like radiobuttons.
*/
class GUIRadioButtons : public gfx::GUIImage
{
public:
	GUIRadioButtons(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

	GUIRadioButtons(gfx::GUIFrame *pParent);

	GUIRadioButtons(const GUIRadioButtons &radioButtons);
	virtual ~GUIRadioButtons(void);


	const GUIRadioButtons &operator=(const GUIRadioButtons &radioButtons);

	virtual void update(float deltaTime);

	/**
	* Add a button to the set of radiobuttons.
	* @param button is a pointer to a button object.
	* @param option is a string associated with the button.
	*/
	virtual void addButton(GUIButton *button, std::string option);

	/**
	* Activate the specified button.
	* @param button a pointer to a button object. If the button is not registered no new button will be selected.
	*/
	virtual void select(GUIButton *button);

	/**
	* Get the sting associated with the selected button.
	* @return a sting associated with the selected button.
	*/
	std::string getOption()const;

protected:
	virtual void notify(int message, GUIFrame *pSender);
	virtual void init();

private:
	std::map<GUIButton*, std::string> mRadioButtons;
	std::string mOption;
	GUIButton *mSelected;
};
}

#endif