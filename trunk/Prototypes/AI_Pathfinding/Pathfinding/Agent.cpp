#include "Macros.h"
#include "Agent.h"
#include <cmath>

const float AGENT_SPEED = 1.0f;
const float GOAL_CLOSE = 0.1f;
const float GOAL_MAX_FAR = sqrtf(2 + GOAL_CLOSE*2);



Agent::Agent(FixedCoordinate startCoord, KeyType key) : mPosition(startCoord), mKey(key), mLastBestGoal(INVALID_COORDINATE)
{
	mDirection.x = 0.0f;
	mDirection.y = 0.0f;
}

Agent::~Agent()
{
}

void Agent::update(float deltaTime)
{
	checkAndHandleNewGoal();

	// Update the position
	mPosition += mDirection * AGENT_SPEED * deltaTime;
}

bool Agent::arrivedAtGoal() const
{
	float diffLength = (mPosition - mPotentialInformation.getBestGoalPosition()).length();

	// If we're not close
	if ((mPosition - mPotentialInformation.getBestGoalPosition()).longerThan(GOAL_CLOSE))
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Agent::checkAndHandleNewGoal()
{
	// If we have a new goal we can change the direction and update our pheromones
	FixedCoordinate bestGoal = mPotentialInformation.getBestGoalPosition();
	if ((mLastBestGoal != bestGoal && bestGoal != INVALID_COORDINATE) || 
		(mPosition - mLastBestGoal).longerThan(GOAL_MAX_FAR))
	{
		mLastBestGoal = bestGoal;
		mDirection = static_cast<Coordinate>(mLastBestGoal) - mPosition;
		mDirection.normalize();
	}
}