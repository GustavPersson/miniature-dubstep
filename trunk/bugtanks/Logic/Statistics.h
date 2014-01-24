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
*/

#ifndef __STATISTICS_H__
#define __STATISTICS_H__


#include "IEventManager.h"
#include <stdio.h>
#include "windows.h"

namespace logic
{

/**
 * Handles statistics for gameplay and such nice little things.
 * Basically a freaky event listener. Check it ouuuttt!!
 */

class Statistics : public EventListener
{
public:

	static Statistics* getInstance();

	virtual ~Statistics();
	
	/**
	 * Returns the name of the listener, as a string.
	 * @return Returns the name as a stdstring
	 */
	char const * getName(void) {return "Statistics";}

	/**
	* Handles events sent, to check if there's any reason to increase or decrease the player's points.
	* @param event The event recieved.
	*/
	bool handleEvent(EventData const & event);

	/**
	* Calculates the player's accuracy.
	*/
	void calculateAccuracy();

	/**
	 * Resets the player's statistics.
	 */
	void resetStatistics();

	int getScore();
	int getKills();
	float getHits();
	float getMisses();
	float getTimesFired();
	float getAccuracy();

private:

	explicit Statistics();

	static Statistics* mpsInstance;

	int mScore;
	int mKills;
	float mHits;
	float mMisses;
	float mTimesFired;
	float mAccuracy;
	FILE * mpOutFile;

};
}

#endif