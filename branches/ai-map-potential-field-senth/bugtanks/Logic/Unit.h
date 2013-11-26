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

#ifndef __UNIT_H__
#define __UNIT_H__

#include "DynamicActor.h"

namespace logic
{
/**
 * This is a mid class between Actor and Enemies and Players(Tank,Driver)
 */

class Unit : public DynamicActor
{

public:
/**
 * Constructor
 */
	Unit(utilities::Position pos, utilities::Direction dir, float hp, int networkId, const utilities::MapCoordinate& size);
/**
 * Destructor
 */
	~Unit();
	
/**
 * An handleEvent function, logically it handles events
 * @param eventMessage an event of any type to be handled
 */
	void handleEventId(EventData const &eventMessage);

/**
* Updates the unit. Currently only set the focalDirection.
* @param deltaTime time elapsed since last frame.
*/
	void update(float deltaTime);

/**
 * Sets the focal point of this unit
 * @param targetPos the target position to look at
 */
	inline void setFocalPoint(utilities::Position targetPos)
	{
		mFocalPoint = targetPos;
		mFocalUnitId = INVALID_KEY;
	}

/**
* Sets a focal actor, i.e. a unit which this unit will keep looking at until set to null
* or when it dies.
* @param focalId the keyId of the unit that we want to use a focal unit
*/
	inline void setFocalUnit(KeyType focalId)
	{
		mFocalUnitId = focalId;
		mFocalPoint = utilities::INVALID_WORLD_POSITON;
	}
	
/**
 * Gets the focal direction of the unit
 * @return returns the focal point of the unit, i.e. the direction the unit is looking at 
 */
	inline const utilities::Direction& getFocalDirection() const
	{
		return mFocalDirection;
	}

protected:
	float mHealthPoints;
	bool mFire;

private:
	utilities::Position mFocalPoint;		/**< The point the unit wants to look at */
	utilities::Direction mFocalDirection;	/**< The direction the unit looks at */
	KeyType mFocalUnitId;					/**< The id of the unit that this unit want to look at */
	
	
};
}

#endif