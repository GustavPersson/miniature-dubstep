#include "Macros.h"
#include "Agent.h"
#include "PotentialInformation.h"

const int PHEROMONE_TRAIL_ON_MAX = 2;
const int PHEROMONE_TRAIL_NEAR = 15;
const int PHEROMONE_TRAIL_FAR = 45;
const int PHEROMONE_TRAIL_MAX_NEGATIVE = PHEROMONE_TRAIL_FAR;

PotentialInformation::PotentialInformation()
{

}

void PotentialInformation::setGoals(const PotentialSquare *newGoals)
{
	for (int i = 0; i < GOAL_FIELD_SIZE; i++)
	{
		mSortedGoals[i] = newGoals[i];
	}
}

void PotentialInformation::setNextGoals(const PotentialSquare *newGoals)
{
	for (int i = 0; i < GOAL_FIELD_SIZE; i++)
	{
		mSortedNextGoals[i] = newGoals[i];
	}
}

void PotentialInformation::updateGoalAndPheromones()
{
	// TODO
	//// Set a new pheromone trail
	//mPheromones.add(Pheromone(PHEROMONE_TRAIL_FAR, mSortedGoalPositions[0].coord));

	//// Decrease the value of the others
	//for (int i = 0; i < mPheromones.size(); i++)
	//{
	//	mPheromones[i].value--;
	//}

	//// If the last value is 0, remove it
	//if (!mPheromones.empty() && mPheromones.getLast().value == 0)
	//{
	//	mPheromones.removeLast();
	//}

	// If we got a next goal, set goal to next goal and invalidate next goal.
	for (int i = 0; i < GOAL_FIELD_SIZE; i++)
	{
		mSortedGoals[i] = mSortedNextGoals[i];
	}

	mSortedNextGoals[0].position = INVALID_COORDINATE;
	mSortedNextGoals[0].value = INVALID_POTENTIAL_VALUE;
}