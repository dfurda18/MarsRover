#include "MarsRover.h"

#define _DIRECTIONS 4

/**
* See MarsRover.h for method definitions
*/

MarsRover::MarsRover(unsigned char initialDirection): Vehicle()
{
	this->direction = initialDirection;		// Set the direction to the initialDirection
}

MarsRover::~MarsRover()
{
	// No need to do anything to delete
}

void MarsRover::TurnToDirection(unsigned char newDirection)
{
	this->direction = newDirection;			// Set the direction to the newDirection
}

unsigned char MarsRover::GetDirection()
{
	return this->direction;					// Returns the rover's direction
}