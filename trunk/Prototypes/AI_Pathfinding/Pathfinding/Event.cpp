#include "Macros.h"
#include "Event.h"

Event::Event(Events eventType, KeyType key, int playerId, Data* pData)
{
	mEvent = eventType;
	mpData = pData;
	mKey = key;
	mPlayerId = playerId;
}

Event::Event(const Event& e)
{
	mEvent = e.mEvent;
	if (e.mpData != NULL)
	{
		mpData = e.mpData->copy();
	}
	else
	{
		mpData = NULL;
	}
	mKey = e.mKey;
	mPlayerId = e.mPlayerId;
}

Event::~Event()
{
	SAFE_DELETE(mpData);
}

Data* Event::getEventData()
{
	return mpData;
}

Events Event::getEventType()
{
	return mEvent;
}

KeyType Event::getKey()
{
	return mKey;
}

int Event::getPlayerId()
{
	return mPlayerId;
}

//----------------------------
// FixedCoordinate
//----------------------------
FixedCoordinateData::FixedCoordinateData(FixedCoordinate coord)
{
	mCoord = coord;
}

FixedCoordinateData::FixedCoordinateData(int x, int y)
{
	mCoord.x = x;
	mCoord.y = y;
}

FixedCoordinate FixedCoordinateData::getData()
{
	return mCoord;
}

Data* FixedCoordinateData::copy()
{
	return new FixedCoordinateData(mCoord);
}

//----------------------------
// Vec2
//----------------------------
Vec2Data::Vec2Data(Vec2 vec2)
{
	mVec2 = vec2;
}

Vec2 Vec2Data::getData()
{
	return mVec2;
}

Data* Vec2Data::copy()
{
	return new Vec2Data(mVec2);
}