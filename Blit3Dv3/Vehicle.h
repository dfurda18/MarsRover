#pragma once

#include "Blit3D.h"

/**
* A class that represents a Rover program.
* @author Dario Urdapilleta
* @version 1.0
* @since 2022-10-11
*/
class Vehicle
{
public:
	/**
	* The Vehicle's location
	*/
	unsigned int location;
	/**
	* Constructor method.
	* @return A new instance of the Vehicle class
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-11
	*/
	Vehicle();
	/**
	* Vehicle Deconstructor method.
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-11
	*/
	~Vehicle();
	/**
	* Sets the Rover's starting location
	* @param newLocation The Vehicle's new location
	* @author Dario Urdapilleta
	* @version 1.0
	* @since 2022-10-13
	*/
	void SetLocation(unsigned int newLocation);

};