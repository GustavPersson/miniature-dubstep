/**
 * @file
 * @author Dennis Jönsson <zelebsel@gmail.com>
 * @author Matteus Magnusson <senth.wallace@gmail.com>
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

#include "../utilities/macros.h"
#include "AgentAi.h"

#ifndef __JUMPERAI_H__
#define __JUMPERAI_H__

namespace ai
{
class JumperAi :public AgentAi
{

public:
	/**
	* Constructor
	* @param pEnemy the logic enemy of the jumper
	*/
	JumperAi(logic::Enemy *pEnemy);

	/**
	* Destructor
	*/
	~JumperAi();

	/**
	* Update function
	*/
	virtual void update();

	/**
	* Update function should only be called if we're the owner of the unit
	*/
	virtual void updateOnlyOwner();

	/**
	* Calls when the ai resumes. continues to jump if we were jumping.
	*/
	virtual void resume();

	/**
	* Jump to the specified target position
	* @param position the position to jump
	*/
	void jump(const utilities::Position& position);

private:
	/**
	* Jump to the specified target
	* @param pTarget the target to jump to
	*/
	void jump(logic::Actor *pTarget);

	/**
	* Call this function to restore the state when
	*/
	void jumpFinished();

	bool mJumping;

	float mLastJumpTime;
	float mLastTestTime;
};
}

#endif