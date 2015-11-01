#include "Robot.h"

using namespace std;

Robot::Robot(const string& name, const string& sprite_path) :	Thing(name, sprite_path),
																mFuelCellAge(0),
																mTurnsToCompleteTask(0),
																mSelfDestruct(false)
{
}


Robot::~Robot()
{}


void Robot::startTask(int turns)
{
	if (turns < 1)
	{
		mTurnsToCompleteTask = 1;
		return;
	}	
		
	mTurnsToCompleteTask = turns;
}


void Robot::updateTask()
{
	if (mSelfDestruct)
	{
		die();
		mSelfDestructCallback();
	}

	mTurnsToCompleteTask--;

	if (mTurnsToCompleteTask == 0)
		mTaskCompleteCallback(this);

	mFuelCellAge++;

	if (mFuelCellAge == 200)
		die();
}