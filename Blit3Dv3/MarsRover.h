#pragma once

#include "Vehicle.h"

/**
* A class that represents a Mars Rover.
* @author Dario Urdapilleta
* @version 1.0
* @since 2022-10-11
*/
class MarsRover: public Vehicle
{
private:
	/**
	* The Mars Rover facing direction 0 = North, 1 = East, 2 = South, and 3 = West
	*/
	unsigned char direction;
public:
	/**
	* Constructor method.
	* @param initialDirection The Mars Rover initial direction.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-11
	*/
	MarsRover(unsigned char initialDirection);
	/**
	* Mars Rover Deconstructor method.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-11
	*/
	~MarsRover();
	/**
	* Turns to the specified direction
	* @param newDirection The new direction the rover should turn to
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-11
	*/
	void TurnToDirection(unsigned char newDirection);
	/**
	* Returns the current rover's direction.
	* @return The current's rover direction
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-11
	*/
	unsigned char GetDirection();
};