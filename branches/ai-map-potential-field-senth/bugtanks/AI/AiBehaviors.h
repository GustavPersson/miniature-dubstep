/**
* @file
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

#ifndef __AI__AI_BEHAVIORS_H__
#define __AI__AI_BEHAVIORS_H__

namespace ai
{

/**
* Enumaration about the Ai Behaviors that are available in the 
*/
enum AiBehaviors
{
	AiBehavior_Invalid,
	AiBehavior_MeleeAttacker,
	AiBehavior_RangedAttacker,
	AiBehavior_Jumper,
	AiBehavior_Kamikaze,
	AiBehavior_Scout,
	AiBehavior_Pedestrian
};
}

#endif