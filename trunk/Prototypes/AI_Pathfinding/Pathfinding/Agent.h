#ifndef __AGENT_H__
#define __AGENT_H__

#include "Macros.h"
#include "PathFinder.h"
#include "Vector.h"
#include "VectorList.h"
#include "PotentialInformation.h"
#include "KeyHandler.h"
#include <pthread.h>

class Agent
{
public:
	Agent(FixedCoordinate startCoord, KeyType key);
	~Agent();
	void update(float deltaTime);
	
	inline Coordinate getPosition() const {return mPosition;}
	inline void setPosition(const Coordinate& newPosition) {mPosition = newPosition;}
	inline Direction getDirection() const {return mDirection;}
	inline void setDirection(const Direction& newDirection) {mDirection = newDirection;}
	inline const PotentialInformation& getPotentialInformation() const {return mPotentialInformation;}
	inline PotentialInformation& getPotentialInformation() {return mPotentialInformation;}
	bool arrivedAtGoal() const;
	inline KeyType getKey() const {return mKey;}
	

private:
	void checkAndHandleNewGoal();

	FixedCoordinate mLastBestGoal;		/**< To see if we have changed the best goal */
	PotentialInformation mPotentialInformation;
	Coordinate mPosition;				/**< The current position */
	Direction mDirection;				/**< The current direction */
	int mFrame;
	KeyType mKey;

};

#endif