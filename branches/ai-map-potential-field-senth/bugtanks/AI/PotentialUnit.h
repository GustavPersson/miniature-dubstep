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

#ifndef __POTENTIAL_UNIT_H__
#define __POTENTIAL_UNIT_H__

#include "../Utilities/Macros.h"
#include "../Utilities/Vectors.h"
#include "../Logic/Actor.h"
#include "../Logic/GlobalTimer.h"

#if defined(AI_DRAW_MAP_POTENTIAL_FIELD) || \
	defined(AI_DRAW_PLAYER_POTENTIAL_FIELD) || \
	defined(AI_DRAW_AGENT_GOALS) || \
	defined(AI_DRAW_UNIT_SIZE_BLOCKS) || \
	defined(AI_DRAW_UNIT_ORIGIN_LINES)
#include "../GFX/DX10Module.h"
#endif

namespace ai
{

// Forward declarations
class PathFinder;
class DynamicUnit;
class UnitHandler;

const float INVALID_POTENTIAL_VALUE = -100.0f;
const float NEUTRAL_POTENTIAL_VALUE = 0.0f;

/**
* Struct that contains a map position and a potential value
*/
struct PotentialSquare
{
	utilities::MapCoordinate position;
	float value;

	/**
	* Default constructor
	* @param value the value to set, default is INVALID_POTENTIAL_VALUE
	* @param position the position to set, default is INVALID_MAP_COORDINATE
	*/
	PotentialSquare(float value = INVALID_POTENTIAL_VALUE, utilities::MapCoordinate position = utilities::INVALID_MAP_COORDINATE) :
		value(value), position(position) {}
};

/**
* A base class for all potential units. All units in the AI component should
* be derived from this class. This class has information about the potential 
* field that each unit has.
*/
class PotentialUnit
{
public:
	/**
	* Constructor.
	* @param pActor pointer to the unit's actor class
	* @param applyOnGlobalPotentialField if the unit's potential field should be added to the global potential field.
	*/
	PotentialUnit(logic::Actor* pActor, bool applyOnGlobalPotentialField);

	/**
	* Destructor
	*/
	virtual ~PotentialUnit();

	/**
	* Initializes the potential unit
	*/
	static void init();

	/**
	* Update function should be run somewhat each frame, not necessary though
	*/
	virtual void update();

	/**
	* Update function should only be called if we're the owner of the unit
	*/
	virtual inline void updateOnlyOwner() {}

	/**
	* Returns the agent's key id
	* @return the agent's keyId
	*/
	inline logic::KeyType getKey() const {return mpActor->getKey();}

	/**
	* Returns the position the unit has on the map as mapCoordinates
	* @return position the unit has on the map
	*/
	inline utilities::MapCoordinate getMapPosition() const
	{
		return (mpActor->getPosition() + mOriginOffset).convertToMapCoordinates();
	}

	/**
	* Returns the current position of the unit with the offset that has been calculated.
	* @note this doesn't return only the offset.
	* @return current unit position with the offset that has been calculated
	*/
	inline utilities::Vec3Float getPositionWithOffset() const
	{
		return (mpActor->getPosition() + mOriginOffset);
	}

	/**
	* Returns the size of the unit
	* @return the size of the unit on the map
	*/
	inline utilities::MapCoordinate getSize() const {return mCurrentUnitSize;}

	/**
	* Returns the size (columns/rows) of the potential field
	* @return size (columns/rows) of the potential field
	*/
	virtual const utilities::MapCoordinate& getPotentialSize() const = 0;

	/**
	* Returns the value of the potential field on the specified map location.
	* I.e. not in local coordinates but in map coordinates.
	* @param mapPosition the map position we want to get the information from
	* @return the value on the specified location, INVALID_POTENTIAL_VALUE if it's out of bounds.
	*/
	virtual float getPotentialValue(const utilities::MapCoordinate& mapPosition) const = 0;

	/**
	* Returns the current position of the unit
	* @return current position of the unit as a Vec2Float
	*/
	inline utilities::Vec2Float getPosition() const
	{
		return utilities::Vec2Float(mpActor->getPosition().x, mpActor->getPosition().z);
	}

	/**
	* Returns true if the unit is rotated, 1,0 is the standard direction. If the direction
	* is closer to 0,1 then it's rotated.
	* @return true if the unit is rotated
	*/
	inline bool isRotated() const {return mRotated;}

	/**
	* Returns the name of the potential unit
	* @return name of the potential unit
	*/
	virtual std::string getName() const = 0;

	/**
	* Returns if we should apply the potential field on the global field
	* @return true if we should apply the potential field on the global field
	*/
	inline bool applyOnGlobalPotentialField() const {return mApplyOnGlobalPotentialField;}

	friend class DynamicUnit;

protected:
	/**
	* Calculates the potential field for a sub-class.
	* @param potentialField array of the potential field
	* @param potentialRange the number of rows/columns the field has
	* @param negative if the potential field should be negated, default is false
	*/
	static void calculatePotentialField(float* potentialField, const utilities::MapCoordinate& potentialRange, bool negative = false);

	/**
	* This function gets called just before a unit changes it rotation.
	* Does nothing in this class.
	*/
	virtual inline void beforeRotateChange() {}

	/**
	* This function gets called just after a unit changes it rotation.
	* Does nothing in this class.
	*/
	virtual inline void afterRotateChange() {}

	/**
	* This function gets called just after a unit has move it's position
	* Does nothing in this class.
	*/
	virtual void unitMoved() {}

	/**
	* Returns the time elapsed since the last update was called
	* @return elapsed time since the last update was called
	*/
	inline float deltaTime() const {return mDeltaTime;}

	/**
	* Returns the current frame time, i.e. the elapsed seconds since we started the game
	* @return elapsed time in seconds since we started the game
	*/
	inline float currentTime() const {return mLastTime;}

	/**
	* Update the origin offset and current unit size depending on if the unit is rotated or not.
	*/
	void updateOffsetAndUnitSize();

	logic::Actor* mpActor;

	utilities::MapCoordinate mCurrentPos;	/**< The current position, updates when we arrive on a new position */
	utilities::MapCoordinate mPreviousPos;	/**< The previous position we had */
	static PathFinder* mpsPathFinder;
	static UnitHandler* mpsUnitHandler;

#if defined(AI_DRAW_MAP_POTENTIAL_FIELD) || \
	defined(AI_DRAW_PLAYER_POTENTIAL_FIELD) || \
	defined(AI_DRAW_AGENT_GOALS) || \
	defined(AI_DRAW_UNIT_SIZE_BLOCKS) || \
	defined(AI_DRAW_UNIT_ORIGIN_LINES)
	static gfx::DX10Module* mpsDx10Module;
#endif

#ifdef AI_DRAW_UNIT_SIZE_BLOCKS
	gfx::TransformObject** mpPositionBlocks;
	int mcPositionBlock;
#endif

private:
	/**
	* Checks if the unit is rotated or not. This function actually checks the rotation from the actor
	* and not only returns the mRotated variable.
	* @return true if the unit currently is rotated.
	*/
	bool checkRotated();

	/**
	* Handles the rotation of the unit
	*/
	void handleRotation();

	/**
	* Returns the focal point. This is implemented differently on different units
	* so you need to overload this to get the right focal point.
	* @return the direction the unit is looking at
	*/
	virtual inline const utilities::Vec3Float& getFocalDirection() const
	{
		return mpActor->getDirection();
	}

	utilities::Vec3Float mOriginOffset;	/**< The origin offset of the unit, used for goal position etc. */
	utilities::MapCoordinate mCurrentUnitSize; /**< The current size of the unit, this gets rotated when rotated is true */
	bool mRotated;
	bool mApplyOnGlobalPotentialField;
	bool mActive;

	float mLastTime; /**< The last time we updated a frame. */
	float mDeltaTime; /**< Time elapsed since the last update function was called */
	static logic::GlobalTimer* mpsGlobalTimer;

#ifdef AI_DRAW_UNIT_ORIGIN_LINES
	gfx::TransformObject* mpOriginLineX;
	gfx::TransformObject* mpOriginLineZ;
	gfx::TransformObject* mpOffsetOriginLineX;
	gfx::TransformObject* mpOffsetOriginLineZ;
#endif
};
}

/**
* Global stream right operator for PotentialSquare.
* @param in reference to the istream object to read from
* @param potentialSquare the potential square to write to
* @return reference to the istream object (in)
*/
std::istream& operator>>(std::istream& in, ai::PotentialSquare& potentialSquare);

/**
* Global stream left operator for PotentialSquare.
* @param out reference to the ostream object to write to
* @param potentialSquare the potential square to read from
* @return reference to the ostream object (out)
*/
std::ostream& operator<<(std::ostream& out, const ai::PotentialSquare& potentialSquare);

#endif