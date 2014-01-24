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
* 
* @section DESCRIPTION
*
* Handles statistics for gameplay and such nice little things.
*/

#include "Statistics.h"
#include "Events.h"
#include "ActorHandler.h"
#include "PlayerInfo.h"

using namespace logic;

Statistics* Statistics::mpsInstance = NULL;

Statistics* Statistics::getInstance()
{
	if(mpsInstance == NULL)
	{
		mpsInstance = myNew Statistics();
	}

	return mpsInstance;
}

Statistics::Statistics()
{
	mScore = 0;
	mHits = 0.0f;
	mKills = 0;
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

bool Statistics::handleEvent(const EventData &eventData)
{
	if (EvtData_From_DriverDeath::msEventType == eventData.getEventType())
	{
	}
	else if (EvtData_From_ActorDestroyed::msEventType == eventData.getEventType())
	{ 
		const EvtData_From_ActorDestroyed& evtData = static_cast<const EvtData_From_ActorDestroyed&>(eventData);
		Actor *pActor =  ActorHandler::getInstance()->getActor(evtData.mId);
		
		if (pActor != NULL)
		{
			Actor::ActorTypes type = pActor->getActorType();
			if (type == Actor::ActorType_Enemy && evtData.mKillerId == PlayerInfo::getInstance()->getNetworkId())
			{
				mScore += 100;
				mKills++;
			}
			else if (type == Actor::ActorType_Tank && evtData.mId.isOwner())
			{
				mScore -= 500;
			}
		}
	}
	else if (EvtData_System_EndGame::msEventType == eventData.getEventType())
	{
		calculateAccuracy();
		if(mpOutFile)
		{
			fprintf_s(mpOutFile, "Total Score: %i \n", mScore);
			fprintf_s(mpOutFile, "Total kills: %i \n", mKills);
			fprintf_s(mpOutFile, "Total Shots Fired: %f \nTotal Hits: %f \nTotal Misses: %f \nAccuracy: %f",
				mTimesFired, mHits, mMisses, mAccuracy);
		}

	}
	/*else if (EvtData_From_EnemyHit::msEventType == eventData.getEventType())
	{
		mHits++;
	}*/
	else if (EvtData_From_EnemyHitNetwork::msEventType == eventData.getEventType())
	{
		const EvtData_From_EnemyHitNetwork& evtData = static_cast<const EvtData_From_EnemyHitNetwork&>(eventData);
		if(evtData.mShooter.isOwner())
			mHits++;
	}
	else if (EvtData_From_WeaponFired::msEventType == eventData.getEventType())
	{
		const EvtData_From_EnemyHitNetwork& evtData = static_cast<const EvtData_From_EnemyHitNetwork&>(eventData);
		if(evtData.getNetworkId() == PlayerInfo::getInstance()->getNetworkId())
		{
			mTimesFired++;
		}
	}
	return false;
}

void Statistics::calculateAccuracy()
{
	mMisses = mTimesFired - mHits;
	if(mTimesFired != 0.0f)
		mAccuracy = mHits / mTimesFired;
}

void Statistics::resetStatistics()
{
	mScore = 0;
	mKills = 0;
	mHits = 0.0f;
	mMisses = 0.0f;
	mTimesFired = 0.0f;
	mAccuracy = 0.0f;
}

int Statistics::getScore()
{
	return mScore;
}
int Statistics::getKills()
{
	return mKills;
}
float Statistics::getHits()
{
	return mHits;
}
float Statistics::getMisses()
{
	return mMisses;
}
float Statistics::getTimesFired()
{
	return mTimesFired;
}
float Statistics::getAccuracy()
{
	calculateAccuracy();
	return mAccuracy;
}