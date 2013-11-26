/**
* @file
* @author Gustav Persson <ninjamuff@gmail.com>
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
* 
* @section DESCRIPTION
*
* Handles statistics for gameplay and such nice little things.
*/

#include "Statistics.h"
#include "Events.h"
#include "ActorHandler.h"

using namespace logic;

Statistics::Statistics()
{
	mScore = 0;
	mHits = 0.0f;
	mMisses = 0.0f;
	mTimesFired = 0.0f;
	mAccuracy = 0.0f;

	fopen_s(&mpOutFile, "score.txt", "w+");
}

Statistics::~Statistics()
{
	if (mpOutFile)
		fclose(mpOutFile);
}

bool Statistics::handleEvent(const EventData &event)
{
	if (EvtData_From_DriverDeath::mEventType == event.getEventType())
	{
	}
	else if (EvtData_From_ActorDestroyed::mEventType == event.getEventType())
	{
		const EvtData_From_ActorDestroyed& evtData = static_cast<const EvtData_From_ActorDestroyed&>(event);
		Actor::ActorType type = ActorHandler::getInstance()->getActor(evtData.mId)->getActorType();
		if (type == Actor::ActorType_Enemy)
		{
			mScore += 100;
		}
		else if (type == Actor::ActorType_Tank)
		{
			mScore -= 500;
		}
	}
	else if (EvtData_System_EndGame::mEventType == event.getEventType())
	{
		calculateAccuracy();
		if(mpOutFile)
		{
			fprintf_s(mpOutFile, "Total Score: %i \n", mScore);
			fprintf_s(mpOutFile, "Total Shots Fired: %f \n Total Hits: %f \n Total Misses: %f \n Accuracy: %f",
				mTimesFired, mHits, mMisses, mAccuracy);
		}

	}
	else if (EvtData_From_EnemyHit::mEventType == event.getEventType())
	{
		mHits++;
	}
	else if (EvtData_From_WeaponFired::mEventType == event.getEventType())
	{
		mTimesFired++;
	}

	return false;
}

void Statistics::calculateAccuracy()
{
	mMisses = mTimesFired - mHits;
	mAccuracy = mHits / mTimesFired;
}